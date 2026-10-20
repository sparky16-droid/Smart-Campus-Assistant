@echo off
title Push Smart Campus Assistant to GitHub
color 0B

echo.
echo ===========================================================
echo   DEPLOYING SMART CAMPUS ASSISTANT TO GITHUB
echo ===========================================================
echo Target Repository: https://github.com/sparky16-droid/Smart-Campus-Assistant.git
echo.

set "PATH=%LOCALAPPDATA%\Programs\Git\cmd;%PATH%"

echo [1/3] Ensuring branch is main...
git branch -M main

echo [2/3] Setting remote origin...
git remote remove origin 2>nul
git remote add origin https://github.com/sparky16-droid/Smart-Campus-Assistant.git

echo [3/3] Pushing to GitHub...
echo (If a browser window opens, please click 'Sign in with your browser' / 'Authorize')
echo.
git push -u origin main

echo.
if %ERRORLEVEL% EQU 0 (
    echo ===========================================================
    echo   [SUCCESS] Code successfully pushed to GitHub!
    echo   View at: https://github.com/sparky16-droid/Smart-Campus-Assistant
    echo ===========================================================
) else (
    echo [ERROR] Push failed. Please check your GitHub permissions or credentials.
)
echo.
pause
