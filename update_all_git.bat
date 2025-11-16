@echo off
chcp 65001 >nul
set HTTP_PROXY=http://192.168.200.8:32080
set HTTPS_PROXY=http://192.168.200.8:32080

setlocal enabledelayedexpansion

rem ========================================
rem 指定要扫描的目录列表
rem ========================================
set DIRS="%CD%" libsrc

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
        git pull --ff-only
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
            git pull --ff-only
            popd
        )
    )
)

echo.
echo ===============================================
echo All Git repositories updated.
echo ===============================================
pause

