#!/bin/bash

set -e

source /opt/venv/bin/activate

export CXXPROJ_PATH=/zdata/cxxproj
cd ${CXXPROJ_PATH}

# 自动检测架构，如果提供了参数则使用参数
if [ -n "$1" ]; then
    ARCH="$1"
else
    # 使用 uname -m 自动检测架构
    MACHINE_ARCH=$(uname -m)
    case "$MACHINE_ARCH" in
        x86_64|amd64)
            ARCH="x86_64"
            ;;
        aarch64|arm64)
            ARCH="aarch64"
            ;;
        *)
            echo "Error: Unsupported architecture: $MACHINE_ARCH"
            echo "Supported architectures: x86_64, aarch64"
            echo "Usage: $0 [x86_64|aarch64]"
            exit 1
            ;;
    esac
fi

echo "Using architecture: $ARCH"

# 根据架构复制文件
if [ "$ARCH" == "x86_64" ]; then
    cp bin/linux_x86_64/hostvm.bin /opt/hostvm/
    cp -L lib/linux_x86_64/libidh.so.1 /opt/hostvm/
elif [ "$ARCH" == "aarch64" ]; then
    cp bin/linux_aarch64/hostvm.bin /opt/hostvm/
    cp -L lib/linux_aarch64/libidh.so.1 /opt/hostvm/
else
    echo "Error: Invalid architecture: $ARCH"
    echo "Supported architectures: x86_64, aarch64"
    echo "Usage: $0 [x86_64|aarch64]"
    exit 1
fi

cp HostVM/dist/docker/*.xml /opt/hostvm/ || true   
cp HostVM/dist/docker/*.ptl /opt/hostvm/ || true
cp HostVM/dist/docker/*.lua /opt/hostvm/ || true
cp HostVM/dist/docker/*.py  /opt/hostvm/ || true
