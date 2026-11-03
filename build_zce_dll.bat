@echo off
rem Builds and installs the shared libzce on Windows (ymwang78/HostVM#42, ymwang78/libzce#213):
rem zce.dll / zcepy.dll into bin\win_amd64, zce.lib / zcepy.lib into lib\win_amd64; a Debug build
rem (ymwang78/HostVM#47) into bin\win_amd64\debug and lib\win_amd64\debug, with the debug CRT, for
rem the Debug configurations. msbuild\zce_dll.props links MSBuild projects against the one that
rem matches their configuration.
rem Needs VCPKG_ROOT (triplet x64-windows-static-md, as the MSBuild projects use) and the Visual
rem Studio C++ tools; CMake and Ninja come with them.
rem Usage: build_zce_dll.bat [Release|Debug]
setlocal
set CFG=%1
if "%CFG%"=="" set CFG=Release
if /i "%CFG%"=="Release" (set CFG=Release) else if /i "%CFG%"=="Debug" (set CFG=Debug) else (
    echo *** build_zce_dll.bat: unknown configuration %CFG%; Release or Debug ***
    exit /b 1
)
set "ROOT=%~dp0"
set "SRC=%ROOT%libsrc\libzce"
rem dot directories: libzce's .gitignore leaves those out
set "BLD=%SRC%\.build-shared-win"
if "%CFG%"=="Debug" set "BLD=%SRC%\.build-shared-win-debug"

if not defined VCPKG_ROOT (
    echo *** VCPKG_ROOT is not set; it must point at the vcpkg checkout the MSBuild projects use. ***
    exit /b 1
)
rem vcvars64.bat runs Visual Studio's vcpkg-init, which points VCPKG_ROOT at the vcpkg bundled with
rem Visual Studio: keep ours
set "ZCE_VCPKG_ROOT=%VCPKG_ROOT%"

rem --- the compiler: already set up (Developer Command Prompt) or found with vswhere ---
where cl.exe >nul 2>nul && goto :configure
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" set "VSWHERE=%ProgramFiles%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" goto :notools
set "VSDIR="
for /f "usebackq delims=" %%I in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VSDIR=%%I"
if not defined VSDIR goto :notools
call "%VSDIR%\VC\Auxiliary\Build\vcvars64.bat" >nul || goto :notools
set "VCPKG_ROOT=%ZCE_VCPKG_ROOT%"

:configure
rem A Developer Command Prompt has already pointed VCPKG_ROOT at Visual Studio's vcpkg, which has
rem none of the packages
if not exist "%VCPKG_ROOT%\installed\x64-windows-static-md\include" (
    echo *** %VCPKG_ROOT% has no x64-windows-static-md packages: set VCPKG_ROOT to the vcpkg checkout the MSBuild projects use. ***
    exit /b 1
)
rem PostgreSQL and Redis on, as libzcemb.vcxproj has them; the checked-in ZDL parser sources,
rem so that no bison is needed.
cmake -S "%SRC%" -B "%BLD%" -G Ninja -DCMAKE_BUILD_TYPE=%CFG% ^
    -DBUILD_SHARED_LIBS=ON -DBUILD_TESTS=OFF -DZDL_REGENERATE_PARSER=OFF ^
    -DENABLE_ZDB_PGSQL=ON -DENABLE_ZDB_REDIS=ON ^
    "-DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake" ^
    -DVCPKG_TARGET_TRIPLET=x64-windows-static-md -DVCPKG_MANIFEST_MODE=OFF ^
    "-DPostgreSQL_LIBRARY=%ROOT%lib\x64\%CFG%\libpq.lib" ^
    "-DPostgreSQL_INCLUDE_DIR=%ROOT%include\win64\pgsql" ^
    "-DPython3_INCLUDE_DIR=%ROOT%include\win64\python3.13" ^
    "-DPython3_LIBRARY=%ROOT%lib\x64\%CFG%\python313.lib" || goto :failed
cmake --build "%BLD%" || goto :failed
if "%CFG%"=="Debug" goto :install_debug
cmake --install "%BLD%" || goto :failed
echo zce.dll / zcepy.dll installed into %ROOT%bin\win_amd64
endlocal
exit /b 0

rem libzce's install rules know one place only, lib\win_amd64 and bin\win_amd64: the Debug build
rem is copied beside them
:install_debug
if not exist "%ROOT%lib\win_amd64\debug" mkdir "%ROOT%lib\win_amd64\debug" || goto :failed
if not exist "%ROOT%bin\win_amd64\debug" mkdir "%ROOT%bin\win_amd64\debug" || goto :failed
for %%F in (zce.lib zcepy.lib) do copy /y "%BLD%\%%F" "%ROOT%lib\win_amd64\debug\" >nul || goto :failed
for %%F in (zce.dll zce.pdb zcepy.dll zcepy.pdb) do copy /y "%BLD%\%%F" "%ROOT%bin\win_amd64\debug\" >nul || goto :failed
echo zce.dll / zcepy.dll (Debug) installed into %ROOT%bin\win_amd64\debug
endlocal
exit /b 0

:notools
echo *** Visual Studio C++ tools not found. Run this from a Developer Command Prompt. ***
exit /b 1

:failed
echo *** building the shared libzce failed ***
exit /b 1
