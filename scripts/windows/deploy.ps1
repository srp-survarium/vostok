# SPDX-License-Identifier: GPL-3.0-or-later
# Copy the native build's exe into a game install, beside the retail exe (it loads the game's
# DLLs and resources from there). The PDB keeps its linked name, which the exe's debug
# directory records, so crash reports symbolize.
#
#   powershell -NoProfile -ExecutionPolicy Bypass -File scripts\windows\deploy.ps1 -GameDir <game>\binaries\win32
param([Parameter(Mandatory)][string]$GameDir, [string]$Name = 'survarium_rebuilt.exe')
$ErrorActionPreference = 'Stop'
. "$PSScriptRoot\common.ps1"
if (-not (Test-Path (Join-Path $GameDir 'survarium.exe'))) { throw "$GameDir has no survarium.exe - pass the game's binaries\win32 folder" }
# a running instance locks the exe and the copy fails
Get-Process ([IO.Path]::GetFileNameWithoutExtension($Name)) -ErrorAction SilentlyContinue | Stop-Process -Force
Start-Sleep -Milliseconds 500
Copy-Item "$ExeDir\survarium-dx11-win32-gold.exe" (Join-Path $GameDir $Name) -Force
Copy-Item "$ExeDir\survarium-dx11-win32-gold.pdb" (Join-Path $GameDir 'survarium-dx11-win32-gold.pdb') -Force
"deployed $(Join-Path $GameDir $Name)"
