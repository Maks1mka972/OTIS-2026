@echo off
setlocal
cd /d "%~dp0"

set "TASK_DIR=%~dp0trunk\ii02907\task_02"
set "BUILD_DIR=%TASK_DIR%\build"

echo ==========================================
echo OTIS Lab 2 - configure, build, test, cover
echo ==========================================

where cmake >nul 2>nul
if errorlevel 1 (
    echo ERROR: CMake was not found in PATH.
    exit /b 1
)

where g++ >nul 2>nul
if errorlevel 1 (
    echo ERROR: GCC/MinGW was not found in PATH.
    exit /b 1
)

cmake -S "%TASK_DIR%" -B "%BUILD_DIR%" -G "MinGW Makefiles" -DENABLE_COVERAGE=ON -DBUILD_TESTING=ON
if errorlevel 1 exit /b 1

cmake --build "%BUILD_DIR%" --parallel
if errorlevel 1 exit /b 1

ctest --test-dir "%BUILD_DIR%" --output-on-failure
if errorlevel 1 exit /b 1

python -m pip install --user gcovr
if errorlevel 1 (
    echo ERROR: Could not install gcovr. Install it with: python -m pip install --user gcovr
    exit /b 1
)

python -m gcovr --root "%TASK_DIR%" --filter "%TASK_DIR%/src/" --exclude "%TASK_DIR%/test/" --html-details "%TASK_DIR%/coverage.html" --print-summary
if errorlevel 1 exit /b 1

echo.
echo All tests passed. Coverage report: %TASK_DIR%\coverage.html
pause
