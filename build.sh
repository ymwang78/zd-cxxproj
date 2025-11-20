#!/bin/bash

set -e

source /zdata/venv/bin/activate

export CXXPROJ_PATH=/zdata/share/cxxproj
cd ${CXXPROJ_PATH}

# -----------------------------------------
# 处理参数
# -----------------------------------------
CLEAN_BUILD=false

if [[ "$1" == "--clean" ]]; then
    CLEAN_BUILD=true
fi

# 封装成函数，避免重复
build_module() {
    local path="$1"
    shift

    echo "========================================"
    echo "Building: $path    (clean=$CLEAN_BUILD)"
    echo "Extra CMake args: $@"
    echo "========================================"

    cd "$CXXPROJ_PATH/$path"

    if $CLEAN_BUILD; then
        rm -rf build
    fi

    mkdir -p build
    cd build

    cmake .. "$@"
    make -j$(nproc)
    make install

    cd "$CXXPROJ_PATH"
}

# -----------------------------------------
# 按顺序构建
# -----------------------------------------
build_module libsrc/libzce
build_module libsrc/libzdl
build_module libsrc/libidh -DUSE_CUSTOM_STATIC_LIBS=OFF
build_module libsrc/libcoin
build_module libsrc/libmpc
build_module HostVM

#cd HostVM/dist/debian
#./x_package.sh hostvm
