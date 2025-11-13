rem 设置环境变量 VCPKG_ROOT=D:\Github\vcpkg
rem .\vcpkg integrate install

vcpkg install boost-algorithm:x64-windows-static-md 
vcpkg install boost-tokenizer:x64-windows-static-md 
vcpkg install boost-lexical-cast:x64-windows-static-md
vcpkg install boost-program-options:x64-windows-static-md
vcpkg install libuv:x64-windows-static-md
vcpkg install open62541:x64-windows-static-md
vcpkg install openssl:x64-windows-static-md 
vcpkg install uriparser:x64-windows-static-md 
vcpkg install bzip2:x64-windows-static-md 
vcpkg install liblzma:x64-windows-static-md
vcpkg install minizip-ng:x64-windows-static-md
vcpkg install coinutils:x64-windows-static-md
vcpkg install sol2:x64-windows-static-md 
vcpkg install nlohmann-json:x64-windows-static-md
vcpkg install lua:x64-windows-static-md 
vcpkg install gtest:x64-windows-static-md

rem vcpkg install cxxopts:x64-windows-static-md 
rem vcpkg install coin-or-ipopt:x64-windows-static-md
rem vcpkg install mongo-cxx-driver:x64-windows-static-md


rem TaijiMPC client requirements
vcpkg install bzip2:x86-windows-static
vcpkg install open62541:x86-windows-static
vcpkg install openssl:x86-windows-static
vcpkg install boost-algorithm:x86-windows-static
vcpkg install boost-tokenizer:x86-windows-static
vcpkg install lua:x64-windows-static