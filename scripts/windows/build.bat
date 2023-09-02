@echo off

call config.bat

REM Supported build types are "RELEASE" and "DEBUG".
set BUILD_TYPE="RELEASE"
if not "%1"=="" set BUILD_TYPE=%1

if not exist %KBHELL_BIN_DIR% mkdir %KBHELL_BIN_DIR%

REM Generate the kbhell Visual Studio 2022 solution.
cmake ^
    -G "Visual Studio 17 2022" ^
    -DBUILD_DOCS=ON ^
    -DPACKAGE_TESTS=ON ^
    -DCMAKE_INSTALL_PREFIX=%KBHELL_BIN_DIR% ^
    -B %KBHELL_BUILD_DIR% ^
    -S %KBHELL_PROJECT_PATH%

cmake ^
    --build %KBHELL_BUILD_DIR% ^
    --config %BUILD_TYPE%

cmake ^
    --install %KBHELL_BUILD_DIR% ^
    --config %BUILD_TYPE%
