@echo off
rem Build and run tests/zdl/test_zdl_vec_metas.cpp.
rem
rem The vcpkg-triplet dependency (zce, gtest, libuv, openssl) and the libzdl
rem sources it links against are not part of this repository's build, so this
rem is a standalone runner rather than a CMake target.
rem
rem   build_and_run.cmd [zdl-src-dir] [lib-dir]
rem
rem Override the defaults with environment variables if the checkouts live
rem elsewhere:
rem   ZDL_SRC      libzdl checkout   (default: ..\..\libsrc\libzdl)
rem   ZDL_LIB_DIR  directory holding libzce.lib (default: ..\..\lib\win_amd64)
rem   VCPKG_DIR    vcpkg installed   (default: D:\Github\vcpkg\installed\x64-windows-static-md)
setlocal enabledelayedexpansion
set HERE=%~dp0
set WT=%HERE%..\..
set ZDLINC=%WT%\include
if "%~1" neq "" set ZDL_SRC=%~1
if "%~2" neq "" set ZDL_LIB_DIR=%~2
if "%ZDL_SRC%"=="" set ZDL_SRC=%WT%\libsrc\libzdl
if "%ZDL_LIB_DIR%"=="" set ZDL_LIB_DIR=%WT%\lib\win_amd64
if "%VCPKG_DIR%"=="" set VCPKG_DIR=D:\Github\vcpkg\installed\x64-windows-static-md
set OUT=%WT%\build\vec_metas_test

if not exist "%ZDL_SRC%\zdl_type.cpp" (
    echo zdl sources not found in "%ZDL_SRC%"; set ZDL_SRC= && exit /b 2
)
if not exist "%ZDL_LIB_DIR%\libzce.lib" (
    echo libzce.lib not found in "%ZDL_LIB_DIR%"; pass the directory as the second argument
    exit /b 2
)
if not exist "%OUT%\obj" mkdir "%OUT%\obj"
call "C:\Program Files\Microsoft Visual Studio\18\Enterprise\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64 >nul || exit /b 1

set CLFLAGS=/nologo /std:c++17 /EHsc /MD /O2 /utf-8 /W3 /wd4996 /DWIN32 /DNDEBUG /D_CRT_SECURE_NO_WARNINGS /I"%ZDLINC%" /I"%ZDL_SRC%" /I"%VCPKG_DIR%\include"

echo === compile libzdl
cl %CLFLAGS% /c /D_LIB /DZDP /DYY_NEVER_INTERACTIVE /Fo"%OUT%\obj\\" ^
    "%ZDL_SRC%\zdl_builtin.cpp" "%ZDL_SRC%\zdl_enum.cpp" "%ZDL_SRC%\zdl_grammar.tab.cpp" ^
    "%ZDL_SRC%\zdl_lex.cpp" "%ZDL_SRC%\zdl_member.cpp" "%ZDL_SRC%\zdl_module.cpp" ^
    "%ZDL_SRC%\zdl_parser.cpp" "%ZDL_SRC%\zdl_parser_context.cpp" "%ZDL_SRC%\zdl_re.cpp" ^
    "%ZDL_SRC%\zdl_struct.cpp" "%ZDL_SRC%\zdl_type.cpp" || exit /b 1
lib /nologo /OUT:"%OUT%\libzdl.lib" "%OUT%\obj\*.obj" || exit /b 1

set LINKLIBS=/LIBPATH:"%VCPKG_DIR%\lib" "%OUT%\libzdl.lib" "%ZDL_LIB_DIR%\libzce.lib" gtest.lib libuv.lib libssl.lib libcrypto.lib bz2.lib zlib.lib ws2_32.lib iphlpapi.lib userenv.lib psapi.lib advapi32.lib crypt32.lib shlwapi.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib secur32.lib bcrypt.lib user32.lib dbghelp.lib

echo === build test_zdl_vec_metas
cl %CLFLAGS% "%HERE%test_zdl_vec_metas.cpp" /Fo"%OUT%\test_zdl_vec_metas.obj" /Fe"%OUT%\test_zdl_vec_metas.exe" /link %LINKLIBS% || exit /b 1

echo === run
"%OUT%\test_zdl_vec_metas.exe"
exit /b %errorlevel%
