::
:: Post-build hook for VS2008
::
:: Hooks run from this directory  : :/binaries/Win32/intermediates/Master Gold/survarium - PC - Windows/
:: The hook path is set to        : ../../../../../scripts/post-build-hook.bat
:: The script current directory is: :/sources/vostok/survarium/pc/sources
::

if not defined REQUIRED_CLASS set "REQUIRED_CLASS=vostok::network_core"

if not defined ROOT_DIR       set "ROOT_DIR=%~dp0..\.."
for %%I in ("%ROOT_DIR%")  do set "ROOT_DIR=%%~fI"

if not defined VOSTOK_DIR    set    "VOSTOK_DIR=%ROOT_DIR%\vostok"
if not defined SRP_DIR       set       "SRP_DIR=%ROOT_DIR%\srp"
if not defined COFF_DIR      set      "COFF_DIR=%ROOT_DIR%\vostok-coff-delinker"
if not defined XRAY_STUB_DIR set "XRAY_STUB_DIR=%ROOT_DIR%\xray-structure"

:: Normalize paths in environment variables
for %%I in ("%VOSTOK_DIR%")     do set "VOSTOK_DIR=%%~fI"
for %%I in ("%SRP_DIR%")        do set "SRP_DIR=%%~fI"
for %%I in ("%COFF_DIR%")       do set "COFF_DIR=%%~fI"
for %%I in ("%XRAY_STUB_DIR%")  do set "XRAY_STUB_DIR=%%~fI"

set   "ENGINE_DIR=%VOSTOK_DIR%\sources\vostok"
set     "PDB_FILE=%VOSTOK_DIR%\binaries\Win32\survarium-dx11-win32-gold.pdb"
set     "EXE_FILE=%VOSTOK_DIR%\binaries\Win32\survarium-dx11-win32-gold.exe"

::
:: Rerun pdb-parser script
::

pushd "%SRP_DIR%"

echo Regenerating structure based on the compiled PDB file
cargo run --release --bin pdb-parser -- --pdb-path "%PDB_FILE%" --output-path "%XRAY_STUB_DIR%" --engine-path "%ENGINE_DIR%" --as-base

popd

::
:: Generate COFF files for a newly built executable
::


:: `-import`        - will import an executable into the project.
:: `-overwrite`     - will ensure that a new project is always created.
:: `-deleteproject` - will create temporary project, which will be delted after the analysis finished.
set "PROCESS_ARGS=-import %EXE_FILE% -overwrite -deleteproject"
set "OUTPUT_DIR=%COFF_DIR%\base"
set "CLASS_FILTER=%REQUIRED_CLASS%"
set "PROJECTS_DIR=%VOSTOK_DIR%\binaries\temp_ghidra_project_2"

echo Regenerating COFF object files
"%VOSTOK_DIR%\scripts\vostok-generate-coff.bat"
