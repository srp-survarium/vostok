@echo off
rem ===================================================================
rem  vostok toolchain build - runs in the XP guest at first logon.
rem  Lives on the FAT data disk (hdb); %~d0 is that disk's drive letter.
rem  It also writes results + logs back to that same disk so the host can
rem  read them with mtools even if the build fails.  Powers off when done.
rem ===================================================================
setlocal enableextensions

set DATA=%~d0
set LOG=%DATA%\build.log

echo === vostok VM toolchain build ===> "%LOG%"
echo DATA=%DATA%>> "%LOG%"
ver >> "%LOG%"
date /t >> "%LOG%"
time /t >> "%LOG%"

rem --- locate the VS2008 DVD (has vs_setup.msi at root or under Setup\) ---
set VSMSI=
for %%D in (D E F G H I) do if exist %%D:\Setup\vs_setup.msi set VSMSI=%%D:\Setup\vs_setup.msi
for %%D in (D E F G H I) do if exist %%D:\vs_setup.msi set VSMSI=%%D:\vs_setup.msi
echo VSMSI=%VSMSI%>> "%LOG%"
if "%VSMSI%"=="" goto :fail

rem --- SP1 MSP + DXSDK live next to this script on the data disk ---
set SP1=%DATA%\sp1\VS90sp1-KB945140-X86-ENU.msp
set DXSDK=%DATA%\DXSDK_Jun10.exe
echo SP1=%SP1%>> "%LOG%"
echo DXSDK=%DXSDK%>> "%LOG%"

rem --- 1) VS2008 admin install (unpack) with SP1 applied via PATCH ---
rem     The whole reason for a real Windows guest: this should patch the static
rem     CRT (libcmt.lib) to SP1 9.0.30729, which wine's msiexec does not.
echo [1] msiexec /a vs_setup.msi PATCH=sp1 ...>> "%LOG%"
msiexec /a "%VSMSI%" TARGETDIR=C:\vsout PATCH="%SP1%" /qn /L*v "%DATA%\msi-vs.log"
echo   msiexec exit=%errorlevel%>> "%LOG%"

rem --- 2) DirectX SDK June 2010 (silent) ---
echo [2] DXSDK /U ...>> "%LOG%"
"%DXSDK%" /U
echo   dxsdk exit=%errorlevel%>> "%LOG%"

rem --- 3) copy the trees back onto the data disk ---
echo [3] copy out VC / DXSDK ...>> "%LOG%"
set VCDIR=
for /d /r C:\vsout %%V in (VC) do if exist "%%V\bin\cl.exe" set VCDIR=%%V
echo   VCDIR=%VCDIR%>> "%LOG%"
if not "%VCDIR%"=="" xcopy /e /i /y /q "%VCDIR%" "%DATA%\out\msvc\VC" >> "%LOG%" 2>&1

set DXDIR=C:\Program Files\Microsoft DirectX SDK (June 2010)
if exist "%DXDIR%\Include" xcopy /e /i /y /q "%DXDIR%\Include" "%DATA%\out\dxsdk\Include" >> "%LOG%" 2>&1
if exist "%DXDIR%\Lib" xcopy /e /i /y /q "%DXDIR%\Lib" "%DATA%\out\dxsdk\Lib" >> "%LOG%" 2>&1

rem TODO(winsdk): the full Windows SDK 6.0A is a separate install (the Wine path
rem pulls it from the VS2008 ISO's WCU\WinSDK\WinSDK_Build.exe). Add it here once
rem the VC path is confirmed; the CRT (the point of this experiment) is in VC.

echo DONE> "%DATA%\out\DONE"
echo === finished ===>> "%LOG%"
time /t >> "%LOG%"
shutdown -s -t 0 -f
goto :eof

:fail
echo ORCHESTRATE FAILED - see above>> "%LOG%"
shutdown -s -t 0 -f
