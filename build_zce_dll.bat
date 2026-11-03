@echo off
rem Builds and installs the shared libzce on Windows (ymwang78/HostVM#42, ymwang78/libzce#213):
rem zce.dll / zcepy.dll into bin\win_amd64, zce.lib / zcepy.lib into lib\win_amd64.
rem msbuild\zce_dll.props links MSBuild projects against them.
rem Release only: the DLLs use the release CRT, which every user of them must match.
rem Needs VCPKG_ROOT (triplet x64-windows-static-md, as the MSBuild projects use) and the Visual
rem Studio C++ tools; CMake and Ninja come with them.
rem Usage: build_zce_dll.bat
setlocal
set "ROOT=%~dp0"
set "SRC=%ROOT%libsrc\libzce"
rem a dot directory: libzce's .gitignore leaves those out
set "BLD=%SRC%\.build-shared-win"

if not defined VCPKG_ROOT (
    echo *** VCPKG_ROOT is not set; it must point at the vcpkg checkout the MSBuild projects use. ***
    exit /b 1
)

rem --- the compiler: already set up (Developer Command Prompt) or found with vswhere ---
where cl.exe >nul 2>nul && goto :configure
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" set "VSWHERE=%ProgramFiles%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" goto :notools
set "VSDIR="
for /f "usebackq delims=" %%I in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VSDIR=%%I"
if not defined VSDIR goto :notools
call "%VSDIR%\VC\Auxiliary\Build\vcvars64.bat" >nul || goto :notools

:configure
rem PostgreSQL and Redis on, as libzcemb.vcxproj has them; the checked-in ZDL parser sources,
rem so that no bison is needed.
cmake -S "%SRC%" -B "%BLD%" -G Ninja -DCMAKE_BUILD_TYPE=Release ^
    -DBUILD_SHARED_LIBS=ON -DBUILD_TESTS=OFF -DZDL_REGENERATE_PARSER=OFF ^
    -DENABLE_ZDB_PGSQL=ON -DENABLE_ZDB_REDIS=ON ^
    "-DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake" ^
    -DVCPKG_TARGET_TRIPLET=x64-windows-static-md -DVCPKG_MANIFEST_MODE=OFF ^
    "-DPostgreSQL_LIBRARY=%ROOT%lib\x64\Release\libpq.lib" ^
    "-DPostgreSQL_INCLUDE_DIR=%ROOT%include\win64\pgsql" ^
    "-DPython3_INCLUDE_DIR=%ROOT%include\win64\python3.13" ^
    "-DPython3_LIBRARY=%ROOT%lib\x64\Release\python313.lib" || goto :failed
cmake --build "%BLD%" || goto :failed
cmake --install "%BLD%" || goto :failed
echo zce.dll / zcepy.dll installed into %ROOT%bin\win_amd64
endlocal
exit /b 0

:notools
echo *** Visual Studio C++ tools not found. Run this from a Developer Command Prompt. ***
exit /b 1

:failed
echo *** building the shared libzce failed ***
exit /b 1
