# SPDX-License-Identifier: GPL-3.0-or-later
# Native Windows build: the toolchain's ninja.exe over the synced graph, with the same
# environment vostok.tool.toolchain configures for Wine. Produces
# C:\survarium\binaries\Win32\survarium-dx11-win32-gold.exe (+ .pdb). This is a fast
# iteration build only; scores, the ledger and the README block come from `vostok build`.
#
# With vcproj2ninja installed (setup.ps1 -Native) the graph is regenerated first, as
# `vostok build` does: new #includes and .vcproj edits take effect without WSL. -NoRegen skips it.
#
# Watchdog: VS2008's LTCG code generator (c2.dll) sometimes waits forever on a stale handle
# (WaitForSingleObject on a handle value Windows has already reused for a thread-pool
# IoCompletion object) - link.exe sits at "Generating code" with 0 CPU. A link that makes no
# CPU progress for -StallSeconds is killed and ninja is rerun (only the link step re-runs).
#
#   powershell -NoProfile -ExecutionPolicy Bypass -File scripts\windows\build.ps1 [-Clean] [-NoRegen] [-Target <ninja target>]
param([string]$Target = '', [int]$StallSeconds = 90, [int]$Attempts = 3, [switch]$Clean, [switch]$NoRegen)
$ErrorActionPreference = 'Stop'
. "$PSScriptRoot\common.ps1"
if (-not $Target) { $Target = $ExeTarget }
Assert-BuildRoot -NoGraph
if (-not $NoRegen) { Update-NinjaGraph | Out-Null }
Assert-BuildRoot
Use-Toolchain
$ninjaExe = "$Toolchain\ninja\ninja.exe"
New-Item -ItemType Directory -Force $LogDir | Out-Null
$stamp = Get-Date -Format yyyyMMdd-HHmmss
$log = "$LogDir\build-$stamp.log"
if ($Clean) { & $ninjaExe -C $NinjaDir -t clean $Target | Out-Null }
$sw = [Diagnostics.Stopwatch]::StartNew()
$rc = -1; $stalls = 0
$ErrorActionPreference = 'Continue'
for ($attempt = 1; $attempt -le $Attempts; $attempt++) {
    $alog = "$LogDir\build-$stamp-a$attempt.log"
    $ninja = Start-Process $ninjaExe -ArgumentList '-C', $NinjaDir, '-v', '-k', '0', $Target -NoNewWindow -PassThru -RedirectStandardOutput $alog -RedirectStandardError "$alog.err"
    $null = $ninja.Handle   # cache the handle now, or ExitCode reads back empty after exit
    $seen = @{}   # link pid -> @(cpu seconds, time it last advanced)
    $stalled = $false
    while (-not $ninja.HasExited) {
        Start-Sleep 5
        foreach ($l in @(Get-Process link -ErrorAction SilentlyContinue | Where-Object { $_.Path -like "$Toolchain\*" })) {
            $cpu = $l.TotalProcessorTime.TotalSeconds
            if (-not $seen.ContainsKey($l.Id) -or $cpu -gt $seen[$l.Id][0] + 0.5) { $seen[$l.Id] = @($cpu, (Get-Date)) }
            elseif (((Get-Date) - $seen[$l.Id][1]).TotalSeconds -gt $StallSeconds) {
                "attempt ${attempt}: link pid $($l.Id) stalled at {0:N0}s CPU - killing" -f $cpu | Tee-Object -FilePath $log -Append
                Stop-Process -Id $l.Id -Force -ErrorAction SilentlyContinue
                $stalled = $true; $stalls++
            }
        }
    }
    $ninja.WaitForExit(); $rc = $ninja.ExitCode
    Get-Content $alog, "$alog.err" -ErrorAction SilentlyContinue | Add-Content $log
    Remove-Item $alog, "$alog.err" -ErrorAction SilentlyContinue
    if (-not $stalled) { break }
}
$sw.Stop()
Get-Process mspdbsrv -ErrorAction SilentlyContinue | Where-Object { $_.Path -like "$Toolchain\*" } | Stop-Process -Force
$steps = @(Select-String -Path $log -Pattern '^\[\d+/\d+\]').Count
$errors = @(Select-String -Path $log -Pattern ' error (C|LNK)\d+').Count
"native build: rc=$rc  {0:N1} min  steps=$steps  errors=$errors  link stalls=$stalls  log=$log" -f $sw.Elapsed.TotalMinutes
if ($rc -eq 0 -and $Target -eq $ExeTarget) { "exe: $ExeDir\survarium-dx11-win32-gold.exe" }
exit $rc
