#!/usr/bin/env bash
# Exercise update_all_git.sh against temporary local remotes.
# Usage: ./test_update_all_git.sh
set -euo pipefail

ROOT=$(cd "$(dirname "$0")" && pwd)
UPDATE_SH="$ROOT/update_all_git.sh"

export GIT_PULL_ATTEMPTS=1
export GIT_PULL_RETRY_SLEEP=0
export GIT_AUTHOR_NAME=test
export GIT_AUTHOR_EMAIL=test@example.com
export GIT_COMMITTER_NAME=test
export GIT_COMMITTER_EMAIL=test@example.com
export GIT_TERMINAL_PROMPT=0

# Keep the harness isolated from the operator's git identity / default branch.
export GIT_CONFIG_COUNT=2
export GIT_CONFIG_KEY_0=user.name
export GIT_CONFIG_VALUE_0=test
export GIT_CONFIG_KEY_1=user.email
export GIT_CONFIG_VALUE_1=test@example.com

passed=0
failed=0
KEEP_LOGS=()

assert_ok() {
    local name="$1"
    shift
    if "$@"; then
        echo "  PASS: $name"
        passed=$((passed + 1))
    else
        echo "  FAIL: $name"
        failed=$((failed + 1))
    fi
}

# grep -q would make `assert_ok ... ! grep` try to exec `!`.
log_lacks() {
    local file="$1"
    local pattern="$2"
    ! grep -q "$pattern" "$file"
}

run_update() {
    local workdir="$1"
    shift
    (cd "$workdir" && bash "$UPDATE_SH" "$@")
}

make_commit() {
    local repo="$1"
    local message="$2"
    local file="${3:-README}"
    echo "$message" >> "$repo/$file"
    git -C "$repo" add "$file"
    git -C "$repo" commit -m "$message"
}

# Layout:
#   $base/remote.git   bare remote (outside the scan root)
#   $base/src          helper working tree (outside the scan root)
#   $base/scan/libsrc/$name   clone seen by update_all_git.sh
setup_remote_and_clone() {
    local base="$1"
    local name="$2"
    local src="$base/src"
    local remote="$base/remote.git"
    local clone="$base/scan/libsrc/$name"

    git init -b main --bare "$remote"
    git init -b main "$src"
    make_commit "$src" "initial $name"
    git -C "$src" remote add origin "$remote"
    git -C "$src" push -u origin main

    mkdir -p "$base/scan/libsrc"
    git clone -b main "$remote" "$clone"
}

cleanup() {
    local dir
    for dir in "${KEEP_LOGS[@]}"; do
        rm -rf "$dir"
    done
}
trap cleanup EXIT

echo "==============================================="
echo "  test_update_all_git.sh"
echo "==============================================="

# --- 1) simulated failed git pull (diverged, --ff-only cannot succeed) ---
echo
echo "[1] failed pull exits non-zero and suppresses the success banner"
t1=$(mktemp -d)
KEEP_LOGS+=("$t1")
setup_remote_and_clone "$t1" "diverged"

# Advance remote and create a conflicting local commit so pull --ff-only fails.
make_commit "$t1/src" "remote-only"
git -C "$t1/src" push origin main
make_commit "$t1/scan/libsrc/diverged" "local-only" "LOCAL"

t1_log="$t1/run.log"
set +e
run_update "$t1/scan" >"$t1_log" 2>&1
t1_rc=$?
set -e

assert_ok "exit code is non-zero (got $t1_rc)" [ "$t1_rc" -ne 0 ]
assert_ok "failure summary names the repo" \
    grep -q "libsrc/diverged" "$t1_log"
assert_ok "failure summary keeps the original git error" \
    grep -qiE "Not possible to fast-forward|refusing to merge unrelated|divergent" "$t1_log"
assert_ok "success banner is not printed" \
    log_lacks "$t1_log" "All Git repositories updated successfully!"

# --- 2) deleted upstream branch is reported instead of silent success ---
echo
echo "[2] deleted upstream branch is reported and fails the script"
t2=$(mktemp -d)
KEEP_LOGS+=("$t2")
setup_remote_and_clone "$t2" "gonebranch"

git -C "$t2/src" checkout -b wangym/tj010_native_backend
make_commit "$t2/src" "feature commit"
git -C "$t2/src" push -u origin wangym/tj010_native_backend
git -C "$t2/scan/libsrc/gonebranch" fetch origin
git -C "$t2/scan/libsrc/gonebranch" checkout wangym/tj010_native_backend

# Delete the branch on the remote. Depending on Git version, a later
# `git pull --ff-only` may either fail with a cryptic ref error or
# succeed against a stale origin/* tracking ref. The script must fail
# with an explicit deleted-branch message in either case.
git -C "$t2/src" push origin --delete wangym/tj010_native_backend

t2_log="$t2/run.log"
set +e
run_update "$t2/scan" >"$t2_log" 2>&1
t2_rc=$?
set -e

assert_ok "script exits non-zero for the deleted branch (got $t2_rc)" \
    [ "$t2_rc" -ne 0 ]
assert_ok "deleted-branch message names the repo" \
    grep -q "libsrc/gonebranch" "$t2_log"
assert_ok "deleted-branch message is explicit" \
    grep -qi "has been deleted" "$t2_log"
assert_ok "success banner is not printed" \
    log_lacks "$t2_log" "All Git repositories updated successfully!"

# --- 3) success path + machine-readable manifest ---
echo
echo "[3] success banner and source manifest when every repo updates"
t3=$(mktemp -d)
KEEP_LOGS+=("$t3")
setup_remote_and_clone "$t3" "okrepo"

# Fast-forward the clone so a real pull happens.
make_commit "$t3/src" "remote advance"
git -C "$t3/src" push origin main

t3_manifest="$t3/source-manifest.tsv"
t3_log="$t3/run.log"
set +e
run_update "$t3/scan" --manifest "$t3_manifest" >"$t3_log" 2>&1
t3_rc=$?
set -e

t3_head=$(git -C "$t3/scan/libsrc/okrepo" rev-parse HEAD)
t3_remote_head=$(git -C "$t3/src" rev-parse HEAD)

assert_ok "exit code is 0 (got $t3_rc)" [ "$t3_rc" -eq 0 ]
assert_ok "success banner is printed" \
    grep -q "All Git repositories updated successfully!" "$t3_log"
assert_ok "clone fast-forwarded to the remote HEAD" \
    [ "$t3_head" = "$t3_remote_head" ]
assert_ok "manifest file exists" [ -f "$t3_manifest" ]
assert_ok "manifest records path, branch and HEAD" \
    grep -q $'libsrc/okrepo\tmain\t'"$t3_head" "$t3_manifest"

echo
echo "==============================================="
if [ "$failed" -eq 0 ]; then
    echo "All $passed checks passed."
    echo "==============================================="
    exit 0
fi
echo "$failed check(s) failed, $passed passed."
echo "==============================================="
echo "---- failed-pull log ----"
sed -n '1,160p' "$t1_log" || true
echo "---- deleted-branch log ----"
sed -n '1,160p' "$t2_log" || true
echo "---- success log ----"
sed -n '1,160p' "$t3_log" || true
exit 1
