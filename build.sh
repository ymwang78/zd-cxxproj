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
# build_module_dir <path> <build dir> [cmake args...]: configure, build and install a module
# in the given build directory under it.
build_module_dir() {
    if [[ $# -lt 2 ]]; then
        echo "build_module_dir: usage: build_module_dir <path> <build dir> [cmake args...]" >&2
        return 1
    fi
    local path="$1"
    local build_dir="$2"
    shift 2
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
        echo "WARN: $path/$build_dir is not writable, fallback to ${build_dir}_user"
        build_dir="${build_dir}_user"
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

# build_module <path> [cmake args...]: the same in the module's build/ directory
build_module() {
    local path="$1"
    shift
    build_module_dir "$path" build "$@"
}

# -----------------------------------------
# 按顺序构建
# -----------------------------------------
build_module libsrc/libzce -DBUILD_TESTS=ON
# libzce.so / libzcepy.so (ymwang78/HostVM#42), which HostVM links; libzce.a stays for the
# programs that link it statically
build_module_dir libsrc/libzce .build-shared -DBUILD_SHARED_LIBS=ON
# HostVM needs the shared libzce alone (ymwang78/HostVM#47): hostvm.bin is the core, and every
# module below is a plugin of its own in bin/<arch>/modules/<Name>. Neither waits for the other; a
# module built later is loaded by the next instance that starts.
build_module hosts/HostVM
build_module libsrc/libzdl -DBUILD_TESTS=ON
build_module libsrc/libidh -DUSE_CUSTOM_STATIC_LIBS=OFF -DBUILD_TESTS=ON
build_module modules/ident
build_module modules/coin
build_module modules/mpc -DBUILD_TESTS=ON -DENABLE_PYTHON_BINDING=ON
build_module modules/fgw -DFGW_BUILD_TESTS=OFF
# KeelTdb on the shared libzce (ymwang78/HostVM#45): its plugin modules/KeelTdb, which carries
# libxds_client.so for SoftSensor and KeelTrace, so it comes before them. HostVM no longer builds it.
ZCE_SHARED_LIB="$CXXPROJ_PATH/lib/linux_$(uname -m)/libzce.so"
# Its own install rules (libktdb.a, ktdb_api.h) go to a private prefix; the plugin installs into
# bin/<arch>/modules/KeelTdb by itself.
KTDB_ARGS=(-DBUILD_TESTS=OFF -DBUILD_EXAMPLES=OFF -DBUILD_BENCH=OFF -DKTDB_ZDL_LIBRARY=
           -DCMAKE_INSTALL_PREFIX="$CXXPROJ_PATH/modules/tdb/build/install")
build_module modules/tdb "${KTDB_ARGS[@]}" -DKTDB_ZCE_LIBRARY="$ZCE_SHARED_LIB" -DZCE_LIBRARY="$ZCE_SHARED_LIB"
build_module modules/softsensor
build_module modules/KeelTrace -DKEELTRACE_BUILD_PLUGIN=OFF
build_module modules/KeelSim -DKEELSIM_BUILD_PLUGIN=OFF
build_module tools/zGen

#cd hosts/HostVM/dist/debian
#./x_package.sh hostvm
