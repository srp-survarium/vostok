# SPDX-License-Identifier: GPL-3.0-or-later
# Rerun only the final exe link, with error reporting off so a linker crash prints its message
# instead of hanging on the invisible "send error report?" prompt. Starts mspdbsrv first and
# prints link.exe CPU every 30 s, so a stall (blocked, not working) is visible. Diagnostic
# companion to build.ps1; the objects must already be built.
#
#   powershell -NoProfile -ExecutionPolicy Bypass -File scripts\windows\relink.ps1 [-TimeoutMinutes 8]
param([int]$TimeoutMinutes = 8)
$ErrorActionPreference = 'Stop'
. "$PSScriptRoot\common.ps1"
Assert-BuildRoot
Use-Toolchain
New-Item -ItemType Directory -Force $LogDir | Out-Null
$out = "$LogDir\relink.out"; $err = "$LogDir\relink.err"
# the link rsp's relative paths resolve from the exe project's directory, as under ninja
Set-Location (Join-Path $BuildRoot 'sources\vostok\survarium\pc\sources')
Start-Process "$Toolchain\msvc\VC\bin\mspdbsrv.exe" -ArgumentList '-start', '-spawn', '-shutdowntime', '600' -WindowStyle Hidden
Start-Sleep 1
$sw = [Diagnostics.Stopwatch]::StartNew()
$job = Start-Process "$Toolchain\msvc\VC\bin\link.exe" -ArgumentList "@$NinjaDir\rsp\${ExeTarget}_link.rsp", '/NOLOGO', '/ERRORREPORT:NONE' -NoNewWindow -PassThru -RedirectStandardOutput $out -RedirectStandardError $err
$null = $job.Handle
while (-not $job.HasExited -and $sw.Elapsed.TotalMinutes -lt $TimeoutMinutes) {
    Start-Sleep 30
    try { $job.Refresh(); "{0,5:N1} min  link cpu {1:N0}s  threads {2}" -f $sw.Elapsed.TotalMinutes, $job.TotalProcessorTime.TotalSeconds, $job.Threads.Count } catch {}
}
if (-not $job.HasExited) { "link still running after $TimeoutMinutes min - killing"; Stop-Process $job -Force; $job.WaitForExit() }
"link rc=$($job.ExitCode) after {0:N1} min" -f $sw.Elapsed.TotalMinutes
Get-Content $out, $err | Select-Object -Last 40
