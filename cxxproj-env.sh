#!/usr/bin/env bash
# Shared environment for projects under cxxproj

_script_source="${BASH_SOURCE[0]:-$0}"
_script_dir="$(CDPATH= cd -- "$(dirname -- "${_script_source}")" && pwd)"
export CXXPROJ_PATH="${_script_dir}"
echo "CXXPROJ_PATH=${CXXPROJ_PATH}"
