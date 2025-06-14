rem 设置环境变量 VCPKG_ROOT=D:\Github\vcpkg
rem .\vcpkg integrate install

vcpkg install bzip2:x64-windows-static-md 
vcpkg install boost-algorithm:x64-windows-static-md 
vcpkg install boost-tokenizer:x64-windows-static-md 
vcpkg install openssl:x64-windows-static-md 
vcpkg install uriparser:x64-windows-static-md 
vcpkg install sol2:x64-windows-static-md 
vcpkg install minizip-ng:x64-windows-static-md

rem TaijiMPC client requirements
vcpkg install bzip2:x86-windows-static
vcpkg install open62541:x86-windows-static
vcpkg install openssl:x86-windows-static