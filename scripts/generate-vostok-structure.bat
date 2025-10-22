@echo off

if not defined ROOT_DIR       set "ROOT_DIR=%~dp0..\.."
for %%I in ("%ROOT_DIR%")  do set "ROOT_DIR=%%~fI"

if not defined VOSTOK_STUB_DIR set "VOSTOK_STUB_DIR=%ROOT_DIR%\vostok-structure"
if not defined SURVARIUM_BIN   set   "SURVARIUM_BIN=D:\Projects\Survarium\binaries\win32"

for %%I in ("%VOSTOK_STUB_DIR%") do set "VOSTOK_STUB_DIR=%%~fI"
for %%I in ("%SURVARIUM_BIN%")   do set "SURVARIUM_BIN=%%~fI"

echo This will permanently delete everything in %VOSTOK_STUB_DIR%

set /p confirm=Type YES to continue:
if /i "%confirm%"=="YES" (
    rmdir /s /q "%VOSTOK_STUB_DIR%\sources"
    rmdir /s /q "%VOSTOK_STUB_DIR%\headers"
    echo Cleaned %VOSTOK_STUB_DIR%

    cargo run --release -- --no-overwrites --output-path "%VOSTOK_STUB_DIR%" --pdb-path "%SURVARIUM_BIN%\survarium.pdb" --engine-path c:\survarium\sources\vostok\
) else (
    echo Operation cancelled.

    exit /b 0
)
