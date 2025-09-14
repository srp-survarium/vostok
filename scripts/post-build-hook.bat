::
:: Post-build hook for VS2008
::
:: Hooks run from this directory  : :/binaries/Win32/intermediates/Master Gold/survarium - PC - Windows/
:: The hook path is set to        : ../../../../../scripts/post-build-hook.bat
:: The script current directory is: :/sources/vostok/survarium/pc/sources
::

if not defined VOSTOK_DIR    set    "VOSTOK_DIR=%cd%\..\..\..\..\.."
if not defined SRP_DIR       set       "SRP_DIR=%VOSTOK_DIR%\..\srp"
if not defined COFF_DIR      set      "COFF_DIR=%VOSTOK_DIR%\..\vostok-coff-delinker"
if not defined XRAY_STUB_DIR set "XRAY_STUB_DIR=%VOSTOK_DIR%\..\xray-structure"

:: Normalize paths in environment variables
for %%I in ("%VOSTOK_DIR%")     do set "VOSTOK_DIR=%%~fI"
for %%I in ("%SRP_DIR%")        do set "SRP_DIR=%%~fI"
for %%I in ("%COFF_DIR%")       do set "COFF_DIR=%%~fI"
for %%I in ("%XRAY_STUB_DIR%")  do set "XRAY_STUB_DIR=%%~fI"

set "ENGINE_DIR=%VOSTOK_DIR%\sources\vostok"
set   "TEMP_DIR=%VOSTOK_DIR%\binaries\temp_ghidra_project"
set   "PDB_FILE=%VOSTOK_DIR%\binaries\Win32\survarium-dx11-win32-gold.pdb"
set   "EXE_FILE=%VOSTOK_DIR%\binaries\Win32\survarium-dx11-win32-gold.exe"

if not defined REQUIRED_CLASS set "REQUIRED_CLASS=vostok::collision"

if not exist "%TEMP_DIR%" (
    mkdir "%TEMP_DIR%"
)

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

set "PROJECTS_DIR=%TEMP_DIR%"

:: `-import`        - will import an executable into the project.
:: `-overwrite`     - will ensure that a new project is always created.
:: `-deleteproject` - will create temporary project, which will be delted after the analysis finished.
set "PROCESS_ARGS=-import %EXE_FILE% -overwrite -deleteproject"
set "OUTPUT_DIR=%COFF_DIR%\base"
set "CLASS_FILTER=%REQUIRED_CLASS%"

echo Regenerating COFF object files
"%SRP_DIR%\vostok-generate-coff.bat"
