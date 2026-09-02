:: SPDX-License-Identifier: GPL-3.0-or-later

::
:: Generate COFF files for all classes in the executable
::
:: By default this script generates COFF files for the target executable
::

::
:: Repository paths can be overwritten in case they are defined in unrelated locations
::

if not defined ROOT_DIR       set "ROOT_DIR=%~dp0..\..\.."
for %%I in ("%ROOT_DIR%")  do set "ROOT_DIR=%%~fI"

if not defined VOSTOK_DIR      set      "VOSTOK_DIR=%ROOT_DIR%\vostok"
if not defined XRAY_STUB_DIR   set   "XRAY_STUB_DIR=%ROOT_DIR%\xray-structure"
if not defined VOSTOK_STUB_DIR set "VOSTOK_STUB_DIR=%ROOT_DIR%\vostok-structure"

if not defined IDA_HOME        set "IDA_HOME=C:\Program Files\IDA Free 9.1"
if not defined OBJDIFF_HOME    set "OBJDIFF_HOME=C:\Program Files\scripts"

:: Normalize paths in environment variables
for %%I in ("%IDA_HOME%")      do set "IDA_HOME=%%~fI"

set   "VS2008_PATH=C:\Program Files (x86)\Microsoft Visual Studio 9.0\Common7\IDE\devenv.exe"
set  "SOLUTION_PATH=%VOSTOK_DIR%\sources\vostok v2.0.sln"
set "VOSTOK_SRC_DIR=%VOSTOK_DIR%\sources\vostok"


tasklist /FI "IMAGENAME eq ida.exe" | find /I "ida.exe" >nul
if errorlevel 1 (
    start "" "%IDA_HOME%\ida.exe"
)

tasklist /FI "IMAGENAME eq devenv.exe" | find /I "devenv.exe" >nul
if errorlevel 1 (
    start "Vostok Engine" "%VS2008_PATH%" "%SOLUTION_PATH%"

    start "Vostok Structure" "%VS2008_PATH%" "%VOSTOK_STUB_DIR%\vostok_structure.sln"
    start "Xray Structure"   "%VS2008_PATH%"   "%XRAY_STUB_DIR%\xray_structure.sln"
)

tasklist /FI "IMAGENAME eq objdiff.exe" | find /I "objdiff.exe" >nul
if errorlevel 1 (
    start "" "%OBJDIFF_HOME%\objdiff.exe"
)

wt ^
new-tab -d "%VOSTOK_DIR%"      --title "nvim"         powershell -NoExit -Command "nvim ./scripts"             ; ^
new-tab -d "%VOSTOK_DIR%"      --title "cargo build"  powershell -NoExit -Command "$c='cargo check' \; iex $c" ; ^
new-tab -d "%VOSTOK_SRC_DIR%"  --title "vostok"       powershell -NoExit -Command "ls"                         ; ^
new-tab -d "%VOSTOK_STUB_DIR%" --title "vostok stubs" powershell -NoExit -Command "ls"                         ; ^
new-tab -d "%XRAY_STUB_DIR%"   --title "xray stubs"   powershell -NoExit -Command "ls"                         ; ^
focus-tab -t 0
