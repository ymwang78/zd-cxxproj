@echo off
rem Moves hosts\HostVM to hosts\KeelHost (HostVM is KeelHost since ymwang78/KeelHost#48) and leaves
rem hosts\HostVM as a junction to it for one major version, so that every path written with the old
rem name keeps working, and points the checkout's origin at ymwang78/KeelHost. Running it again does
rem nothing.
rem Close Visual Studio, terminals and Claude Code sessions in hosts\HostVM first: Windows does not
rem rename a directory that a process has open.
rem Usage: migrate_keelhost_dir.bat
setlocal
set "HOSTS=%~dp0hosts"
for %%A in ("%HOSTS%\HostVM") do set "ATTR=%%~aA"
if not "%ATTR:l=%"=="%ATTR%" (
    echo hosts\HostVM is already a link: nothing to do
    exit /b 0
)
if exist "%HOSTS%\KeelHost" (
    echo *** hosts\KeelHost exists already, next to a real hosts\HostVM: sort them out by hand ***
    exit /b 1
)
if not exist "%HOSTS%\HostVM\" (
    echo *** there is no hosts\HostVM here ***
    exit /b 1
)
ren "%HOSTS%\HostVM" KeelHost || (
    echo *** hosts\HostVM cannot be renamed: something still has it open ***
    exit /b 1
)
mklink /J "%HOSTS%\HostVM" "%HOSTS%\KeelHost" >nul || exit /b 1
rem Linked worktrees keep naming hosts\HostVM, which the junction keeps valid. When it goes away
rem with ymwang78/KeelHost#49, git worktree repair in hosts\KeelHost points them at the new place.
set "URL="
for /f "usebackq delims=" %%U in (`git -C "%HOSTS%\KeelHost" remote get-url origin 2^>nul`) do set "URL=%%U"
if defined URL git -C "%HOSTS%\KeelHost" remote set-url origin "%URL:ymwang78/HostVM=ymwang78/KeelHost%"
echo hosts\KeelHost, with the junction hosts\HostVM pointing at it
endlocal
