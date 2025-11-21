#!/bin/bash

set -e

source /opt/venv/bin/activate

export CXXPROJ_PATH=/zdata/share/cxxproj
cd ${CXXPROJ_PATH}

if [ "$1" == "x86_64" ]; then
    cp bin/linux_x86_64/hostvm.bin /opt/hostvm/
    cp -L lib/linux_x86_64/libidh.so.1 /opt/hostvm/
elif [ "$1" == "aarch64" ]; then
    cp bin/linux_aarch64/hostvm.bin /opt/hostvm/
    cp -L lib/linux_aarch64/libidh.so.1 /opt/hostvm/
else
    echo "Usage: $0 <x86_64|aarch64>"
    exit 1
fi

cp HostVM/dist/docker/hostvm.xml /opt/hostvm/hostvm.xml
