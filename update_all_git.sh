#!/usr/bin/env bash
# UTF-8 locale
export LANG=en_US.UTF-8
export LC_ALL=en_US.UTF-8

# ================================
# 设置代理（可选）
# ================================
source ~/.proxy
#export HTTP_PROXY="http://192.168.200.8:32080"
#export HTTPS_PROXY="http://192.168.200.8:32080"

# ================================
# 指定要扫描的目录列表
# 可自行修改
# ================================
DIRS=(
    "$PWD"
    "libsrc"
)

echo "==============================================="
echo "  Updating Git repositories"
echo "==============================================="
echo "Directories:"
for d in "${DIRS[@]}"; do
    echo "  $d"
done
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
        (cd "$D" && git pull --ff-only)
    else
        echo "[Self] Not a git repo: $D"
    fi

    # ---- 更新一级子目录 ----
    for S in "$D"/*; do
        [ -d "$S" ] || continue
        if [ -d "$S/.git" ]; then
            echo
            echo "[Subdir] Updating repo: $S"
            (cd "$S" && git pull --ff-only)
        fi
    done
done

echo
echo "==============================================="
echo "All Git repositories updated."
echo "==============================================="
