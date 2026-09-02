:: SPDX-License-Identifier: GPL-3.0-or-later
@echo off

if not defined ROOT_DIR       set "ROOT_DIR=%~dp0..\..\.."
for %%I in ("%ROOT_DIR%")  do set "ROOT_DIR=%%~fI"

if not defined VOSTOK_STUB_DIR set "VOSTOK_STUB_DIR=%ROOT_DIR%\vostok-structure"
if not defined PDB_PARSER_DIR  set  "PDB_PARSER_DIR=%ROOT_DIR%\pdb-parser"
if not defined SURVARIUM_BIN   set   "SURVARIUM_BIN=D:\Projects\Survarium\binaries\win32"

for %%I in ("%VOSTOK_STUB_DIR%") do set "VOSTOK_STUB_DIR=%%~fI"
for %%I in ("%PDB_PARSER_DIR%")  do set "PDB_PARSER_DIR=%%~fI"
for %%I in ("%SURVARIUM_BIN%")   do set "SURVARIUM_BIN=%%~fI"

pushd "%PDB_PARSER_DIR%"

cargo run --release --                              ^
    --output-path   "%VOSTOK_STUB_DIR%"             ^
    --pdb-path      "%SURVARIUM_BIN%\survarium.pdb" ^
    --engine-path   c:\survarium\sources\vostok\

popd
