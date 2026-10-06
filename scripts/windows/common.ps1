# SPDX-License-Identifier: GPL-3.0-or-later
# Shared settings for the native Windows build scripts (dot-sourced, not run directly).
#
# The tree must be reachable as C:\survarium: the retail objects record c:\survarium\sources,
# and the ninja graph is rooted there (paths.NATIVE_BUILD_ROOT). setup.ps1 makes it a junction
# to this checkout.

$BuildRoot  = 'C:\survarium'
$RepoRoot   = (Resolve-Path "$PSScriptRoot\..\..").Path
if ($RepoRoot.TrimEnd('\') -eq $BuildRoot) {
    # invoked through the junction: work with the real checkout path
    $RepoRoot = @((Get-Item $BuildRoot).Target)[0]
}
$NativeDir  = Join-Path $RepoRoot 'binaries\windows'
$Toolchain  = if ($env:VOSTOK_WIN_TOOLCHAIN) { $env:VOSTOK_WIN_TOOLCHAIN } else { Join-Path $NativeDir 'toolchain' }
$LogDir     = Join-Path $NativeDir 'logs'
$Vcproj2NinjaDir = Join-Path $NativeDir 'vcproj2ninja'
$Vcproj2Ninja    = Join-Path $Vcproj2NinjaDir 'bin\vcproj2ninja.exe'
# native scoring preview (setup.ps1 -Scoring, score.ps1)
$ScoringBin = Join-Path $NativeDir 'scoring\bin'
$LlvmMingw  = Join-Path $NativeDir 'llvm-mingw'
$LlvmMingwRelease = '20260922'
$LlvmMingwSha256  = 'E3AD77D117A4BEA19A7A3B333341824D79A5A371004A10E25B8504E7B3047666'
$NinjaDir   = Join-Path $BuildRoot 'binaries\ninja'
$ExeTarget  = 'survarium_-_PC_-_DirectX_11'
$ExeDir     = Join-Path $BuildRoot 'binaries\Win32'

function Use-Toolchain {
    # the same PATH/INCLUDE/LIB that vostok.tool.toolchain writes into the Wine registry
    foreach ($d in 'msvc\VC\bin\cl.exe', 'ninja\ninja.exe', 'winsdk\Include', 'dxsdk\Include') {
        if (-not (Test-Path (Join-Path $Toolchain $d))) { throw "toolchain incomplete: $Toolchain\$d missing - run setup.ps1" }
    }
    $env:PATH    = "$Toolchain\msvc\VC\bin;$env:SystemRoot\system32;$env:SystemRoot"
    $env:INCLUDE = "$Toolchain\msvc\VC\include;$Toolchain\dxsdk\Include;$Toolchain\winsdk\Include"
    $env:LIB     = "$Toolchain\msvc\VC\lib;$Toolchain\dxsdk\Lib\x86;$Toolchain\winsdk\Lib"
}

function Assert-BuildRoot([switch]$NoGraph) {
    $item = Get-Item $BuildRoot -ErrorAction SilentlyContinue
    if (-not $item) { throw "$BuildRoot does not exist - run setup.ps1" }
    $target = if ($item.LinkType) { @($item.Target)[0] } else { $item.FullName }
    if ((Resolve-Path $target).Path.TrimEnd('\') -ne $RepoRoot.TrimEnd('\')) {
        throw "$BuildRoot points at $target, not this checkout ($RepoRoot) - rerun setup.ps1 -Force"
    }
    if (-not $NoGraph -and -not (Test-Path (Join-Path $NinjaDir 'build.ninja'))) {
        throw "no ninja graph in $NinjaDir - run setup.ps1 -Native (or sync.ps1 from WSL)"
    }
}

function Get-Python {
    # the repo's python tooling (vostok.build.ninja_regen, vostok.tool.libs) needs 3.11+
    foreach ($c in @('python', 'py')) {
        $cmd = Get-Command $c -ErrorAction SilentlyContinue
        if (-not $cmd) { continue }
        $ok = & $cmd.Source -c 'import sys; print(int(sys.version_info >= (3, 11)))' 2>$null
        if ("$ok".Trim() -eq '1') { return $cmd.Source }
    }
    throw 'Python 3.11+ not found on PATH - install it from python.org'
}

function Invoke-Vostok([string]$Module, [string[]]$Arguments) {
    # python -m vostok.<module> from this checkout's scripts/
    $python = Get-Python
    $env:PYTHONPATH = Join-Path $RepoRoot 'scripts'
    # the tooling logs to stderr, which Windows PowerShell would turn into error records
    $ErrorActionPreference = 'Continue'
    & $python -m $Module @Arguments 2>&1 | ForEach-Object { Write-Host "$_" }
    $rc = $LASTEXITCODE
    Remove-Item Env:\PYTHONPATH
    if ($rc -ne 0) { throw "python -m $Module failed ($rc)" }
}

function Get-FlakePin([string]$Name) {
    # url + sha256 of a release download, as flake.nix pins it (`<Name> = pkgs.runCommand ...`)
    $text = [IO.File]::ReadAllText((Join-Path $RepoRoot 'flake.nix'))
    $m = [regex]::Match($text, "(?s)\b$([regex]::Escape($Name)) = pkgs\.runCommand.*?url = `"([^`"]+)`";\s*sha256 = `"([0-9a-f]{64})`"")
    if (-not $m.Success) { throw "flake.nix has no release pin for $Name" }
    @{ Url = $m.Groups[1].Value; Sha256 = $m.Groups[2].Value }
}

function Get-LockedRev([string]$Name) {
    # the git rev flake.lock pins for a flake input
    $lock = Get-Content (Join-Path $RepoRoot 'flake.lock') -Raw | ConvertFrom-Json
    $locked = $lock.nodes.$Name.locked
    if (-not $locked) { throw "flake.lock has no input $Name" }
    @{ Url = "https://github.com/$($locked.owner)/$($locked.repo)"; Rev = $locked.rev }
}

function Update-NinjaGraph {
    # natively regenerated graph (vostok.build.ninja_regen): only the files whose contents changed
    # are rewritten, so a no-op regen dirties nothing
    if (-not (Test-Path $Vcproj2Ninja)) { return $false }
    $pin = Get-LockedRev 'vcproj2ninja-src'
    $built = Get-Content (Join-Path $Vcproj2NinjaDir 'rev') -ErrorAction SilentlyContinue
    if ($built -ne $pin.Rev) { Write-Warning "vcproj2ninja was built at $built but flake.lock pins $($pin.Rev) - rerun setup.ps1 -Native -Force" }
    $env:VCPROJ2NINJA_EXE = $Vcproj2Ninja
    Invoke-Vostok 'vostok.build.ninja_regen' @()
    Remove-Item Env:\VCPROJ2NINJA_EXE
    $true
}

function Get-WslPath([string]$Distro, [string]$Path) {
    # absolute Linux path of a WSL directory (expands ~)
    $p = (& wsl.exe -d $Distro -- bash -c "cd $Path && pwd -P" | Out-String).Trim()
    if ($LASTEXITCODE -ne 0 -or -not $p) { throw "cannot resolve $Path in WSL distro $Distro" }
    $p
}
