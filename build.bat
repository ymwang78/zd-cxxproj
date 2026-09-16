@echo off
REM ==========================================================================
REM  build.bat  --  xRto one-click build (cmd wrapper for build.ps1)
REM
REM  Examples:
REM    build.bat
REM    build.bat -TaoTripletDir E:\vcpkg\installed\x64-windows-static-md
REM    build.bat -AppOnly
REM    build.bat -Configuration Debug -Rebuild
REM
REM  All arguments are passed straight through to build.ps1
REM  (see the header of build.ps1 for the full option list).
REM ==========================================================================
setlocal
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0build.ps1" %*
exit /b %ERRORLEVEL%
