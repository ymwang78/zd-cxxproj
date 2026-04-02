#!/bin/bash

set -e

source /opt/venv/bin/activate

export CXXPROJ_PATH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
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
    local build_dir="build"
    local module_dir="$CXXPROJ_PATH/$path"

    if [[ ! -d "$module_dir" ]]; then
        echo "========================================"
        echo "Skipping: $path (directory not found)"
        echo "========================================"
        return 0
    fi

    echo "========================================"
    echo "Building: $path    (clean=$CLEAN_BUILD)"
    echo "Extra CMake args: $@"
    echo "========================================"

    cd "$module_dir"

    if [[ -d "$build_dir" && ! -w "$build_dir" ]]; then
        echo "WARN: $path/$build_dir is not writable, fallback to build_user"
        build_dir="build_user"
    fi

    if $CLEAN_BUILD; then
        rm -rf "$build_dir"
    fi

    mkdir -p "$build_dir"
    cd "$build_dir"

    cmake .. "$@"
    make -j$(nproc)
    make install

    cd "$CXXPROJ_PATH"
}

# -----------------------------------------
# 按顺序构建
# -----------------------------------------
build_module libsrc/libzce -DBUILD_TESTS=ON
build_module libsrc/libzdl
build_module zGen
build_module libsrc/libidh -DUSE_CUSTOM_STATIC_LIBS=OFF -DBUILD_TESTS=ON
build_module libsrc/libcoin
build_module libsrc/libmpc -DBUILD_TESTS=ON -DENABLE_PYTHON_BINDING=ON
build_module HostVM

#cd HostVM/dist/debian
#./x_package.sh hostvm
