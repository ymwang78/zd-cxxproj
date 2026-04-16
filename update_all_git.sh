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

SOCKS_PROXY="${SOCKS_PROXY:-socks5h://192.168.200.13:32029}"
export ALL_PROXY="$SOCKS_PROXY"
export all_proxy="$SOCKS_PROXY"
export GIT_TERMINAL_PROMPT=0

git_pull_with_retry() {
    local repo="$1"
    local attempts=3
    local i

    for ((i = 1; i <= attempts; i++)); do
        if (cd "$repo" && git \
            -c http.proxy="$SOCKS_PROXY" \
            -c https.proxy="$SOCKS_PROXY" \
            -c http.version=HTTP/1.1 \
            pull --ff-only); then
            return 0
        fi

        echo "  pull attempt $i/$attempts failed for $repo"
        if [ "$i" -lt "$attempts" ]; then
            sleep 2
        fi
    done

    return 1
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
echo "SOCKS proxy: $SOCKS_PROXY"
echo

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
        echo "[Self] Updating repo: $D"
        git_pull_with_retry "$D"
    else
        echo "[Self] Not a git repo: $D"
    fi

    # ---- 更新一级子目录 ----
    for S in "$D"/*; do
        [ -d "$S" ] || continue
        if [ -d "$S/.git" ]; then
            echo
            echo "[Subdir] Updating repo: $S"
            git_pull_with_retry "$S"
        fi
    done
done

echo
echo "==============================================="
echo "All Git repositories updated."
echo "==============================================="
