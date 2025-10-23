::
:: Generate COFF files for all classes in the executable
::
:: By default this script generates COFF files for the target executable
::

::
:: Repository paths can be overwritten in case they are defined in unrelated locations
::

if not defined ROOT_DIR       set "ROOT_DIR=%~dp0..\.."
for %%I in ("%ROOT_DIR%")  do set "ROOT_DIR=%%~fI"

if not defined COFF_DIR       set      "COFF_DIR=%ROOT_DIR%\vostok-coff-delinker"
if not defined VOSTOK_DIR     set    "VOSTOK_DIR=%ROOT_DIR%\vostok"
if not defined SURVARIUM_BIN  set "SURVARIUM_BIN=D:\Projects\Survarium\binaries\win32"

if not defined GHIDRA_HOME    set   "GHIDRA_HOME=C:\Program Files\ghidra_11.4_PUBLIC"
if not defined PROJECTS_DIR   set  "PROJECTS_DIR=%VOSTOK_DIR%\binaries\temp_ghidra_project"
if not defined SCRIPTS_DIR    set   "SCRIPTS_DIR=%VOSTOK_DIR%\scripts\ghidra"
if not defined OUTPUT_DIR     set    "OUTPUT_DIR=%COFF_DIR%\target"


:: Normalize paths in environment variables
for %%I in ("%GHIDRA_HOME%")   do set "GHIDRA_HOME=%%~fI"
for %%I in ("%PROJECTS_DIR%")  do set "PROJECTS_DIR=%%~fI"
for %%I in ("%SCRIPTS_DIR%")   do set "SCRIPTS_DIR=%%~fI"
for %%I in ("%OUTPUT_DIR%")    do set "OUTPUT_DIR=%%~fI"
for %%I in ("%SURVARIUM_BIN%") do set "SURVARIUM_BIN=%%~fI"


set "EXE_FILE=%SURVARIUM_BIN%\survarium.exe"
if not defined PROCESS_ARGS   set "PROCESS_ARGS=-import %EXE_FILE% -overwrite -deleteproject"
if not defined CLASS_FILTER   set "CLASS_FILTER="

if not exist "%PROJECTS_DIR%" (
    mkdir "%PROJECTS_DIR%"
)

"%GHIDRA_HOME%\support\analyzeHeadless.bat"                       ^
  "%PROJECTS_DIR%" vostok                                         ^
  %PROCESS_ARGS%                                                  ^
  -scriptPath "%SCRIPTS_DIR%"                                     ^
  -preScript  TurnOffNonCoffAnalyzers.java                        ^
  -postScript DelinkProgram.java "%OUTPUT_DIR%" "%CLASS_FILTER%"  ^
    | rg -v                                                       ^
      -e "PDB issue dmangling type name:"                         ^
      -e "reconstruction failed to align"                         ^
      -e "No vfTable found for RTTICompleteObjectLocator"         ^
      -e "Could not create Data"                                  ^
      -e "Failed to create pointer"                               ^
      -e "at java.base"                                           ^
      -e "at ghidra.program"                                      ^
      -e "at ghidra.app"

