#!/usr/bin/env bash
# UTF-8 locale
export LANG=en_US.UTF-8
export LC_ALL=en_US.UTF-8

# ================================
# 设置代理（可选）
# ================================
if [ -f ~/.proxy ]; then
    source ~/.proxy
fi
#export HTTP_PROXY="http://192.168.200.8:32080"
#export HTTPS_PROXY="http://192.168.200.8:32080"

#SOCKS_PROXY="${SOCKS_PROXY:-socks5h://192.168.200.13:32029}"
#export ALL_PROXY="$SOCKS_PROXY"
#export all_proxy="$SOCKS_PROXY"
export GIT_TERMINAL_PROMPT=0

GIT_PULL_ATTEMPTS="${GIT_PULL_ATTEMPTS:-3}"
GIT_PULL_RETRY_SLEEP="${GIT_PULL_RETRY_SLEEP:-2}"

MANIFEST_FILE=""
MANIFEST_ROWS=()
MANIFEST_WRITE_FAILED=0
FAILED_REPOS=()
FAILED_ERRORS=()
declare -A SEEN_REPOS=()

usage() {
    cat <<'EOF'
Usage: update_all_git.sh [--manifest FILE]

Update every Git repository under the configured scan directories.
Every discovered repository is required: any failed update exits non-zero
and prints a summary with the repository path and original Git error.

  --manifest FILE   Write a TSV of path, branch, HEAD (sorted by path)
                    so release builders can compare source provenance.
                    The script exits non-zero if FILE cannot be written.
  -h, --help        Show this help
EOF
}

while [ $# -gt 0 ]; do
    case "$1" in
        --manifest)
            if [ -z "${2:-}" ]; then
                echo "error: --manifest requires a file path" >&2
                exit 1
            fi
            MANIFEST_FILE="$2"
            shift 2
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        *)
            echo "error: unknown option: $1" >&2
            usage >&2
            exit 1
            ;;
    esac
done

git_in_repo() {
    local repo="$1"
    shift
    git -C "$repo" \
        -c http.proxy="${SOCKS_PROXY:-}" \
        -c https.proxy="${SOCKS_PROXY:-}" \
        -c http.version=HTTP/1.1 \
        "$@"
}

# Normalize a repo path relative to $PWD so amd64/arm64 manifests can be diffed.
normalize_repo_path() {
    local repo="$1"
    local abs
    abs=$(cd "$repo" && pwd)
    if [ "$abs" = "$PWD" ]; then
        echo "."
    else
        echo "${abs#"$PWD"/}"
    fi
}

record_failure() {
    local repo="$1"
    local err="$2"
    FAILED_REPOS+=("$(normalize_repo_path "$repo")")
    FAILED_ERRORS+=("$err")
}

record_manifest() {
    local repo="$1"
    [ -n "$MANIFEST_FILE" ] || return 0

    local path branch head
    path=$(normalize_repo_path "$repo")
    branch=$(git -C "$repo" rev-parse --abbrev-ref HEAD 2>/dev/null || echo "?")
    head=$(git -C "$repo" rev-parse HEAD 2>/dev/null || echo "?")
    MANIFEST_ROWS+=("$path"$'\t'"$branch"$'\t'"$head")
}

# Fail early if --manifest cannot be created (missing parent, not writable).
prepare_manifest() {
    [ -n "$MANIFEST_FILE" ] || return 0

    local manifest_dir
    manifest_dir=$(dirname "$MANIFEST_FILE")
    if [ -d "$MANIFEST_FILE" ]; then
        echo "error: cannot write manifest: path is a directory: $MANIFEST_FILE" >&2
        return 1
    fi
    if [ ! -d "$manifest_dir" ]; then
        echo "error: cannot write manifest: parent directory does not exist: $manifest_dir" >&2
        return 1
    fi
    if [ -e "$MANIFEST_FILE" ] && [ ! -w "$MANIFEST_FILE" ]; then
        echo "error: cannot write manifest: file is not writable: $MANIFEST_FILE" >&2
        return 1
    fi
    if [ ! -e "$MANIFEST_FILE" ] && [ ! -w "$manifest_dir" ]; then
        echo "error: cannot write manifest: directory is not writable: $manifest_dir" >&2
        return 1
    fi
    return 0
}

# Write header even when no repositories were found, so builders can diff
# an empty provenance file instead of a 0-byte leftover.
# Each write is checked on its own: a later `if` must not mask a failed
# header printf (empty tree used to leave rc=0 and still mv + succeed).
write_manifest() {
    [ -n "$MANIFEST_FILE" ] || return 0

    local tmp
    tmp="${MANIFEST_FILE}.tmp.$$"

    if ! printf '# path\tbranch\thead\n' >"$tmp"; then
        rm -f "$tmp"
        echo "error: failed to write manifest: $MANIFEST_FILE" >&2
        return 1
    fi

    if [ "${#MANIFEST_ROWS[@]}" -gt 0 ]; then
        set -o pipefail
        if ! printf '%s\n' "${MANIFEST_ROWS[@]}" | sort -t $'\t' -k1,1 >>"$tmp"; then
            set +o pipefail
            rm -f "$tmp"
            echo "error: failed to write manifest: $MANIFEST_FILE" >&2
            return 1
        fi
        set +o pipefail
    fi

    if ! mv "$tmp" "$MANIFEST_FILE"; then
        rm -f "$tmp"
        echo "error: failed to write manifest: $MANIFEST_FILE" >&2
        return 1
    fi
    echo "Source manifest written to: $MANIFEST_FILE"
    return 0
}

# Return 0 if there is no upstream or it still exists on the remote.
# Return 1 if the tracked upstream branch has been deleted, or ls-remote
# cannot be completed after retries. Prints the error on stdout.
check_upstream_branch() {
    local repo="$1"
    local branch remote merge_ref remote_branch
    local output="" status i

    branch=$(git -C "$repo" rev-parse --abbrev-ref HEAD 2>/dev/null || true)
    if [ -z "$branch" ] || [ "$branch" = "HEAD" ]; then
        return 0
    fi

    remote=$(git -C "$repo" config --get "branch.${branch}.remote" || true)
    merge_ref=$(git -C "$repo" config --get "branch.${branch}.merge" || true)
    if [ -z "$remote" ] || [ -z "$merge_ref" ]; then
        return 0
    fi

    remote_branch="${merge_ref#refs/heads/}"

    for ((i = 1; i <= GIT_PULL_ATTEMPTS; i++)); do
        output=$(git_in_repo "$repo" ls-remote --exit-code --heads "$remote" "$merge_ref" 2>&1)
        status=$?
        if [ "$status" -eq 0 ]; then
            return 0
        fi
        # git ls-remote --exit-code: 2 means the ref does not exist on the remote.
        if [ "$status" -eq 2 ]; then
            echo "upstream branch '${remote}/${remote_branch}' has been deleted"
            return 1
        fi
        echo "  ls-remote attempt $i/$GIT_PULL_ATTEMPTS failed for $repo" >&2
        if [ "$i" -lt "$GIT_PULL_ATTEMPTS" ]; then
            sleep "$GIT_PULL_RETRY_SLEEP"
        fi
    done

    if [ -n "$output" ]; then
        echo "$output"
    else
        echo "git ls-remote failed for ${remote}/${remote_branch}"
    fi
    return 1
}

is_deleted_upstream_error() {
    local text="$1"
    echo "$text" | grep -qiE \
        "couldn't find remote ref|no such ref was fetched|your configuration specifies to merge"
}

git_pull_with_retry() {
    local repo="$1"
    local i output status
    local last_error=""
    local upstream_err

    if ! upstream_err=$(check_upstream_branch "$repo"); then
        if echo "$upstream_err" | grep -q "has been deleted"; then
            echo "  ERROR: $repo tracks a deleted upstream branch"
        else
            echo "  ERROR: $repo failed to verify its upstream branch"
        fi
        echo "  $upstream_err"
        LAST_ERROR="$upstream_err"
        return 1
    fi

    for ((i = 1; i <= GIT_PULL_ATTEMPTS; i++)); do
        output=$(git_in_repo "$repo" pull --ff-only 2>&1)
        status=$?
        if [ -n "$output" ]; then
            echo "$output"
        fi
        if [ "$status" -eq 0 ]; then
            return 0
        fi

        last_error="$output"
        if is_deleted_upstream_error "$output"; then
            echo "  ERROR: $repo tracks a deleted upstream branch"
            LAST_ERROR="$output"
            return 1
        fi

        echo "  pull attempt $i/$GIT_PULL_ATTEMPTS failed for $repo"
        if [ "$i" -lt "$GIT_PULL_ATTEMPTS" ]; then
            sleep "$GIT_PULL_RETRY_SLEEP"
        fi
    done

    LAST_ERROR="$last_error"
    return 1
}

update_repo() {
    local repo="$1"
    LAST_ERROR=""
    if git_pull_with_retry "$repo"; then
        record_manifest "$repo"
        return 0
    fi
    record_failure "$repo" "${LAST_ERROR:-unknown error}"
    record_manifest "$repo"
    return 1
}

# Update and record each repository at most once. Top-level DIRS entries
# (libsrc, apps, ...) can also appear as a first-level child of $PWD.
visit_repo() {
    local tag="$1"
    local repo="$2"
    local key
    key=$(cd "$repo" && pwd) || {
        record_failure "$repo" "cannot access repository"
        return 1
    }
    if [ -n "${SEEN_REPOS[$key]:-}" ]; then
        echo "[Skip] Already updated: $repo"
        return 0
    fi
    SEEN_REPOS[$key]=1
    echo "[$tag] Updating repo: $repo"
    update_repo "$repo"
}

# ================================
# 指定要扫描的目录列表
# 可自行修改
# ================================
DIRS=(
    "$PWD"
    "libsrc"
    "apps"
    "tools"
    "modules"
    "hosts"
    "services"
)

echo "==============================================="
echo "  Updating Git repositories"
echo "==============================================="
echo "Directories:"
for d in "${DIRS[@]}"; do
    echo "  $d"
done
echo "SOCKS proxy: ${SOCKS_PROXY:-}"
echo

if ! prepare_manifest; then
    exit 1
fi

# ================================
# 遍历目录列表
# ================================
for D in "${DIRS[@]}"; do
    echo
    echo "======================================"
    echo "Processing directory: $D"
    echo "======================================"

    # ---- 更新目录自身 ----
    if [ -d "$D/.git" ]; then
        visit_repo "Self" "$D"
    else
        echo "[Self] Not a git repo: $D"
    fi

    # ---- 更新一级子目录 ----
    for S in "$D"/*; do
        [ -d "$S" ] || continue
        if [ -d "$S/.git" ]; then
            echo
            visit_repo "Subdir" "$S"
        fi
    done
done

if ! write_manifest; then
    MANIFEST_WRITE_FAILED=1
fi

if [ "${#FAILED_REPOS[@]}" -gt 0 ]; then
    echo
    echo "==============================================="
    echo "FAILED: ${#FAILED_REPOS[@]} repository update(s) failed"
    echo "==============================================="
    for i in "${!FAILED_REPOS[@]}"; do
        echo
        echo "  ${FAILED_REPOS[$i]}"
        while IFS= read -r line; do
            echo "    $line"
        done <<< "${FAILED_ERRORS[$i]}"
    done
    echo
    exit 1
fi

if [ "$MANIFEST_WRITE_FAILED" -ne 0 ]; then
    echo
    echo "==============================================="
    echo "FAILED: could not write source manifest"
    echo "==============================================="
    echo
    exit 1
fi

echo
echo "==============================================="
echo "All Git repositories updated successfully!"
echo "==============================================="
exit 0
