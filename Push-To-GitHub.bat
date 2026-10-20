@echo off
title Push Smart Campus Assistant to GitHub
color 0B

echo.
echo ===========================================================
echo   DEPLOY SMART CAMPUS ASSISTANT TO GITHUB
echo ===========================================================
echo.
echo Make sure you have created an empty repository on GitHub first!
echo (e.g. at https://github.com/new named Smart-Campus-Assistant)
echo.

set /p REPO_URL="Enter your GitHub Repository URL (e.g., https://github.com/YOUR_USERNAME/Smart-Campus-Assistant.git): "

if "%REPO_URL%"=="" (
    echo [ERROR] No URL entered. Aborting.
    pause
    exit /b
)

:: Set PATH to include Git
set "PATH=%LOCALAPPDATA%\Programs\Git\cmd;%PATH%"

echo.
echo [1/3] Setting branch to main...
git branch -M main

echo [2/3] Linking remote origin...
git remote remove origin 2>nul
git remote add origin %REPO_URL%

echo [3/3] Pushing code to GitHub...
echo (A GitHub login window may pop up to authorize)
git push -u origin main

echo.
if %ERRORLEVEL% EQU 0 (
    echo ===========================================================
    echo   [SUCCESS] Code successfully pushed to GitHub!
    echo ===========================================================
) else (
    echo [NOTE] If push failed, check your repo URL and GitHub credentials.
)
echo.
pause
