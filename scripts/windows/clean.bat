@echo off

call config.bat

if exist %KBHELL_BIN_DIR% rmdir /s /q %KBHELL_BIN_DIR% & @echo "removed %KBHELL_BIN_DIR%"
if exist %KBHELL_BUILD_DIR% rmdir /s /q %KBHELL_BUILD_DIR% & @echo "removed %KBHELL_BUILD_DIR%"
