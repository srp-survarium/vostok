:: SPDX-License-Identifier: GPL-3.0-or-later
::
:: Post-build hook for VS2008
::
:: Hooks run from this directory  : :/binaries/Win32/intermediates/Master Gold/survarium - PC - Windows/
:: The hook path is set to        : ../../../../../scripts/vs/post-build-hook.bat
:: The script current directory is: :/sources/vostok/survarium/pc/sources
::

if not defined ROOT_DIR       set "ROOT_DIR=%~dp0..\..\.."
for %%I in ("%ROOT_DIR%")  do set "ROOT_DIR=%%~fI"

if not defined VOSTOK_DIR           set          "VOSTOK_DIR=%ROOT_DIR%\vostok"
if not defined XRAY_STUB_DIR        set       "XRAY_STUB_DIR=%ROOT_DIR%\xray-structure"
if not defined VOSTOK_DELINKER_DIR  set "VOSTOK_DELINKER_DIR=%ROOT_DIR%\vostok-delinker"
if not defined PDB_PARSER_DIR       set      "PDB_PARSER_DIR=%ROOT_DIR%\pdb-parser"

:: Normalize paths in environment variables
for %%I in ("%VOSTOK_DIR%")          do set "VOSTOK_DIR=%%~fI"
for %%I in ("%XRAY_STUB_DIR%")       do set "XRAY_STUB_DIR=%%~fI"
for %%I in ("%VOSTOK_DELINKER_DIR%") do set "VOSTOK_DELINKER_DIR=%%~fI"
for %%I in ("%PDB_PARSER_DIR%")      do set "PDB_PARSER_DIR=%%~fI"

set   "ENGINE_DIR=%VOSTOK_DIR%\sources\vostok"
set     "PDB_FILE=%VOSTOK_DIR%\binaries\Win32\survarium-dx11-win32-gold.pdb"
set     "EXE_FILE=%VOSTOK_DIR%\binaries\Win32\survarium-dx11-win32-gold.exe"

::
:: Generate COFF files for a newly built executable
::

echo Regenerating COFF object files
call "%VOSTOK_DELINKER_DIR%\build_base.bat"

::
:: Rerun pdb-parser
::

pushd "%PDB_PARSER_DIR%"

echo Regenerating structure based on the compiled PDB file
cargo run --release -- ^
    --output-path   "%XRAY_STUB_DIR%"   ^
    --pdb-path      "%PDB_FILE%"        ^
    --engine-path   "%ENGINE_DIR%"      ^
    --as-base                           ^
    --skip-non-engine-headers

popd
