@echo off
:: Smart Campus Assistant - One-Click Launcher
:: Developed for College PBL (Project-Based Learning)

title Smart Campus Assistant - Launcher
color 0A

echo.
echo ===========================================================
echo   SMART CAMPUS ASSISTANT - AI Campus Navigator ^& Portal
echo   Built in C++ with Qt 6 ^| College PBL Project
echo ===========================================================
echo.

:: Setup Qt runtime paths
SET "QT_PATH=C:\Qt\6.12.0\mingw_64\bin"
SET "MINGW_PATH=C:\Qt\Tools\mingw1310_64\bin"
SET "APP_DIR=%~dp0build"

:: Add Qt DLLs to PATH so EXE works standalone
SET "PATH=%QT_PATH%;%MINGW_PATH%;%PATH%"

echo [INFO] Starting Smart Campus Assistant GUI...
echo [INFO] Web portal will be available at: http://localhost:8080
echo [INFO] Press any key inside the console window to close.
echo.

cd /d "%~dp0"
start "" "%APP_DIR%\SmartCampusAssistant.exe"

echo [OK] Application launched successfully!
echo [TIP] You can also open http://localhost:8080 in your browser.
echo.
pause
