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
assert_ok "manifest starts with a header row" \
    [ "$(head -n1 "$t3_manifest")" = $'# path\tbranch\thead' ]

# --- 4) --manifest parent directory missing must fail the script ---
echo
echo "[4] missing manifest parent directory exits non-zero"
t4=$(mktemp -d)
KEEP_LOGS+=("$t4")
mkdir -p "$t4/scan"

t4_log="$t4/run.log"
set +e
run_update "$t4/scan" --manifest "$t4/no_such_dir/manifest.tsv" >"$t4_log" 2>&1
t4_rc=$?
set -e

assert_ok "exit code is non-zero (got $t4_rc)" [ "$t4_rc" -ne 0 ]
assert_ok "error names the missing parent directory" \
    grep -q "parent directory does not exist" "$t4_log"
assert_ok "success banner is not printed" \
    log_lacks "$t4_log" "All Git repositories updated successfully!"
assert_ok "manifest file was not created" \
    [ ! -e "$t4/no_such_dir/manifest.tsv" ]

# --- 5) top-level DIRS entry is updated and recorded only once ---
echo
echo "[5] libsrc as both Self and Subdir is deduplicated"
t5=$(mktemp -d)
KEEP_LOGS+=("$t5")
# libsrc itself is the git repo, so $PWD/libsrc (Subdir) and D=libsrc (Self)
# would both hit it without dedupe.
git init -b main --bare "$t5/remote.git"
git init -b main "$t5/src"
make_commit "$t5/src" "initial libsrc"
git -C "$t5/src" remote add origin "$t5/remote.git"
git -C "$t5/src" push -u origin main
mkdir -p "$t5/scan"
git clone -b main "$t5/remote.git" "$t5/scan/libsrc"

t5_manifest="$t5/source-manifest.tsv"
t5_log="$t5/run.log"
set +e
run_update "$t5/scan" --manifest "$t5_manifest" >"$t5_log" 2>&1
t5_rc=$?
set -e

t5_head=$(git -C "$t5/scan/libsrc" rev-parse HEAD)
t5_libsrc_rows=$(grep -c $'^libsrc\t' "$t5_manifest" || true)
t5_updates=$(grep -c 'Updating repo:.*libsrc' "$t5_log" || true)

assert_ok "exit code is 0 (got $t5_rc)" [ "$t5_rc" -eq 0 ]
assert_ok "manifest has exactly one libsrc row (got $t5_libsrc_rows)" \
    [ "$t5_libsrc_rows" -eq 1 ]
assert_ok "manifest records the single libsrc HEAD" \
    grep -q $'libsrc\tmain\t'"$t5_head" "$t5_manifest"
assert_ok "libsrc is pulled only once (Updating count=$t5_updates)" \
    [ "$t5_updates" -eq 1 ]
assert_ok "second visit is skipped" \
    grep -q "Already updated: libsrc" "$t5_log"

# --- 6) empty tree + --manifest still writes a header ---
echo
echo "[6] empty tree writes a header-only manifest"
t6=$(mktemp -d)
KEEP_LOGS+=("$t6")
mkdir -p "$t6/scan"

t6_manifest="$t6/source-manifest.tsv"
t6_log="$t6/run.log"
set +e
run_update "$t6/scan" --manifest "$t6_manifest" >"$t6_log" 2>&1
t6_rc=$?
set -e

t6_lines=$(wc -l < "$t6_manifest" | tr -d ' ')

assert_ok "exit code is 0 (got $t6_rc)" [ "$t6_rc" -eq 0 ]
assert_ok "success banner is printed" \
    grep -q "All Git repositories updated successfully!" "$t6_log"
assert_ok "manifest exists" [ -f "$t6_manifest" ]
assert_ok "manifest is header-only (lines=$t6_lines)" [ "$t6_lines" -eq 1 ]
assert_ok "manifest header is present" \
    [ "$(cat "$t6_manifest")" = $'# path\tbranch\thead' ]

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
echo "---- missing-parent log ----"
sed -n '1,80p' "$t4_log" || true
echo "---- dedupe log ----"
sed -n '1,160p' "$t5_log" || true
echo "---- empty-tree log ----"
sed -n '1,80p' "$t6_log" || true
exit 1
