:: Generate COFF files for a newly built executable

if not defined SRP_DIR set "SRP_DIR=E:\Projects\srp"

set "PROJECTS_DIR=E:\temp"
set "OUTPUT_DIR=E:\Projects\vostok-coff-delinker\base"
set "PROCESS_ARGS=-import E:\Projects\vostok\binaries\Win32\survarium-win32-gold.exe -overwrite -deleteproject"
set "CLASS_FILTER=vostok::collision"

"%SRP_DIR%\vostok-generate-coff.bat"

