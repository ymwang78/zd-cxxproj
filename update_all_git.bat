@echo off
chcp 65001 >nul
call "%USERPROFILE%\.env.bat"
setlocal enabledelayedexpansion

rem ========================================
rem 指定要扫描的目录列表
rem ========================================
set DIRS="%CD%" libsrc apps hosts modules services tools
echo ===============================================
echo   Updating Git repositories
echo ===============================================
echo Directories:
echo   %DIRS%
echo.


rem ========================================
rem 遍历目录列表（每个目录本身 + 它的一级子目录）
rem ========================================
for %%D in (%DIRS%) do (
    echo.
    echo ======================================
    echo Processing directory: %%D
    echo ======================================

    rem ---- 更新目录自身 ----
    if exist "%%D\.git" (
        echo [Self] Updating repo: %%D
        pushd "%%D"
        git diff
        git pull
        popd
    ) else (
        echo [Self] Not a git repo: %%D
    )

    rem ---- 更新一级子目录 ----
    for /d %%S in ("%%D\*") do (
        if exist "%%S\.git" (
            echo.
            echo [Subdir] Updating repo: %%S
            pushd "%%S"
            git diff
            git pull
            popd
        )
    )

    rem ---- 更新bin/x64子目录 ----
    if exist "%%D\bin\x64\.git" (
        echo [Self] Updating repo: %%D\bin\x64
        pushd "%%D\bin\x64"
        git diff
        git pull
        popd
    )
)

echo.
echo ===============================================
echo All Git repositories updated.
echo ===============================================
pause

