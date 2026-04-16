rem 设置环境变量 VCPKG_ROOT=D:\Github\vcpkg
rem .\vcpkg integrate install

vcpkg install boost-algorithm:x64-windows-static-md 
vcpkg install boost-lexical-cast:x64-windows-static-md
vcpkg install boost-tokenizer:x64-windows-static-md 
vcpkg install boost-program-options:x64-windows-static-md
vcpkg install bzip2:x64-windows-static-md 
vcpkg install CLI11:x64-windows-static-md
vcpkg install coinutils:x64-windows-static-md
vcpkg install expat:x64-windows-static-md
vcpkg install gtest:x64-windows-static-md
vcpkg install hiredis[core,ssl]:x64-windows-static-md
vcpkg install libbson:x64-windows-static-md
vcpkg install liblzma:x64-windows-static-md
vcpkg install libuv:x64-windows-static-md
vcpkg install lua:x64-windows-static-md 
vcpkg install minizip-ng:x64-windows-static-md
vcpkg install ms-gsl:x64-windows-static-md
vcpkg install nlohmann-json:x64-windows-static-md
vcpkg install open62541:x64-windows-static-md
vcpkg install openssl:x64-windows-static-md 
vcpkg install sol2:x64-windows-static-md 
vcpkg install uriparser:x64-windows-static-md 

vcpkg install nlohmann-json:x64-windows
vcpkg install casadi:x64-windows
vcpkg install ms-gsl:x64-windows

vcpkg install boost-algorithm:x86-windows-static
vcpkg install boost-tokenizer:x86-windows-static
vcpkg install bzip2:x86-windows-static
vcpkg install CLI11:x86-windows-static
vcpkg install expat:x86-windows-static
vcpkg install libuv:x86-windows-static
vcpkg install lua:x64-windows-static
vcpkg install nlohmann-json:x86-windows-static
vcpkg install open62541:x86-windows-static
vcpkg install openssl:x86-windows-static
