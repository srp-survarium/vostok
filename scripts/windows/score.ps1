# SPDX-License-Identifier: GPL-3.0-or-later
# Fast scoring preview of the native build (vostok.build.native_score): PDB evidence, objdiff
# report and structure for the exe build.ps1 produced, the ledger re-derived and compared with the
# committed one - in minutes, without touching config/match_state.tsv or the README.
#
# A preview only: the native link folds identical functions a little differently from the Wine
# link the ledger is measured with, so commits stay measured by `python3 -m vostok build` in WSL.
# Compare preview to preview (each run keeps the previous one) to see what an edit changed.
#
# Needs setup.ps1 -Scoring (the tools) and the original survarium.exe/.pdb, from the game
# install (-GameDir, or SURVARIUM_BIN). The first run also generates the target side from them.
#
#   powershell -NoProfile -ExecutionPolicy Bypass -File scripts\windows\score.ps1 -GameDir <game>\binaries\win32 [-Top 40]
param([string]$GameDir = $env:SURVARIUM_BIN, [int]$Top = 25)
$ErrorActionPreference = 'Stop'
. "$PSScriptRoot\common.ps1"
foreach ($t in 'vostok-pdb', 'vostok-delinker', 'vostok-data-delinker', 'objdiff-cli') {
    if (-not (Test-Path (Join-Path $ScoringBin "$t.exe"))) { throw "$t.exe missing in $ScoringBin - run setup.ps1 -Scoring" }
}
if (-not $GameDir -or -not (Test-Path (Join-Path $GameDir 'survarium.pdb'))) {
    throw 'pass -GameDir <game>\binaries\win32 (the folder with the original survarium.exe and survarium.pdb)'
}
Assert-BuildRoot
$env:PDB_TOOL             = Join-Path $ScoringBin 'vostok-pdb.exe'
$env:VOSTOK_DELINKER      = Join-Path $ScoringBin 'vostok-delinker.exe'
$env:VOSTOK_DATA_DELINKER = Join-Path $ScoringBin 'vostok-data-delinker.exe'
$env:OBJDIFF_CLI          = Join-Path $ScoringBin 'objdiff-cli.exe'
$env:SURVARIUM_BIN        = (Resolve-Path $GameDir).Path
# llvm-nm / llvm-objcopy for the cross-unit COMDAT pass (appended: nothing here shadows them)
$env:PATH = "$env:PATH;$LlvmMingw\bin"
Push-Location $RepoRoot
try { Invoke-Vostok 'vostok.build.native_score' @('--top', "$Top") } finally { Pop-Location }
