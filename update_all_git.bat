@echo off
chcp 65001 >nul
set HTTP_PROXY=http://192.168.200.8:32080
set HTTPS_PROXY=http://192.168.200.8:32080

setlocal enabledelayedexpansion

echo 正在扫描当前目录及所有子目录中的 Git 仓库...
echo.

for /f "delims=" %%G in ('dir /s /b /a:d ".git" 2^>nul') do (
    call :UpdateRepo "%%~dpG.."
)

echo.
echo 所有仓库处理完毕。
goto :EOF

:UpdateRepo
set "repo=%~1"
if not exist "%repo%\.git" (
    echo [跳过] %repo% 不是标准 Git 仓库
    goto :EOF
)

echo [处理] %repo%
pushd "%repo%" >nul
git rev-parse --is-inside-work-tree >nul 2>&1
if errorlevel 1 (
    echo   -> 不是有效的 Git 仓库，已跳过
    popd >nul
    goto :EOF
)

git remote update --prune
if errorlevel 1 (
    echo   -> git remote update 失败，已跳过后续操作
    popd >nul
    goto :EOF
)

git pull --ff-only
if errorlevel 1 (
    echo   -> git pull 失败，请检查是否有本地修改
) else (
    echo   -> 已更新
)
popd >nul
goto :EOF