@echo off
setlocal EnableExtensions

set "BUILD_SCRIPT=%~dp0build.ps1"

if not exist "%BUILD_SCRIPT%" (
    echo [FAIL] PowerShell build script not found: %BUILD_SCRIPT%
    exit /b 2
)

echo [INFO] Starting iEWP Keil5 Headless Build...
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%BUILD_SCRIPT%" %*
set "BUILD_EXIT_CODE=%ERRORLEVEL%"

if not "%BUILD_EXIT_CODE%"=="0" (
    echo [FAIL] iEWP build failed. Exit code: %BUILD_EXIT_CODE%
    exit /b %BUILD_EXIT_CODE%
)

echo [PASS] iEWP build completed successfully.
exit /b 0
