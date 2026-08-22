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

    # If a source directory was moved, an existing CMakeCache.txt still points at
    # the old absolute path and CMake will fail or reuse stale paths. Reconfigure
    # from scratch automatically for relocated modules.
    if [[ -f "$build_dir/CMakeCache.txt" ]] && grep -q '^CMAKE_HOME_DIRECTORY:' "$build_dir/CMakeCache.txt"; then
        cached_src=$(grep '^CMAKE_HOME_DIRECTORY:' "$build_dir/CMakeCache.txt" | cut -d= -f2-)
        if [[ "$cached_src" != "$module_dir" ]]; then
            echo "WARN: $path/$build_dir cache points to $cached_src, removing stale build dir"
            rm -rf "$build_dir"
            mkdir -p "$build_dir"
        fi
    fi

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
build_module libsrc/libzdl -DBUILD_TESTS=ON
build_module libsrc/libidh -DUSE_CUSTOM_STATIC_LIBS=OFF -DBUILD_TESTS=ON
build_module modules/ident
build_module modules/coin
build_module modules/mpc -DBUILD_TESTS=ON -DENABLE_PYTHON_BINDING=ON
build_module modules/fgw -DFGW_BUILD_TESTS=OFF
build_module modules/softsensor
build_module tools/zGen
build_module hosts/HostVM

#cd hosts/HostVM/dist/debian
#./x_package.sh hostvm
