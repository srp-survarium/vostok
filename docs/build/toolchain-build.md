# Building the vostok-toolchain release (Linux / Wine)

The `vostok-toolchain` tarball (VS2008 SP1 compiler + WinSDK + DXSDK + ninja) is
produced on Linux from the original installation media:

```
nix-shell scripts/create-toolchain-release.nix
```

This fetches the VS2008 Professional ISO, the VS2008 SP1 ISO, and the DXSDK
installer, then runs `scripts/create-toolchain-release.py`, which does a Wine
`msiexec` administrative install and stages the files into
`binaries/vostok-toolchain-v0.100b.tar.xz`. To publish: `gh release upload` the
tarball and update the `vostok-toolchain` `sha256` in `flake.nix`.

## Two non-obvious requirements (both handled by the script)

### 1. The PDB-writer DLLs must sit next to `cl.exe`

`cl.exe` imports `mspdb80.dll` (the *program database manager*) at runtime,
along with `mspdbcore.dll`, `mspdbsrv.exe`, and `msobj80.dll`. **These ship in
`Common7/IDE`, not `VC/bin`.** If they are absent, `cl.exe` fails to load with
Wine status `c0000135` ("Library mspdb80.dll … not found").

The script searches the admin-install tree for all `mspdb*.*` and `msobj*.dll`
and copies them next to `cl.exe` (the itsmattkc/MSVC420 portable-toolchain
convention). It **prefers the `Common7/IDE` copies** — identical-named copies
also live under `Common Files/Microsoft Shared/VSA/9.0/VsaEnv`, but only the
`Common7/IDE` build is guaranteed to match the compiler.

> Historical bug: the old code looked in `vc_dir.parent.parent / Common7 / IDE`,
> one directory too high, so it found nothing and silently shipped a toolchain
> without these DLLs.

### 2. SP1 must be applied *during* the admin install (`PATCH=`)

The game was built with **VS2008 SP1** (compiler `15.00.30729`, `mspdb*`
`9.00.30729`). Wine's `msiexec` **cannot patch an existing administrative
image** — `msiexec /p <msp> /a <admin.msi>` returns
`ERROR_CALL_NOT_IMPLEMENTED` (exit 120) and silently leaves the toolchain at
RTM (`…21022`).

Instead the patch is applied in one step via the `PATCH` property during the
admin install:

```
msiexec /a vs_setup.msi TARGETDIR=… PATCH=VS90sp1-KB945140-X86-ENU.msp /qn
```

The script verifies afterward that `cl.exe` reports `15.00.30729`.

## Wine must be staging (≥ 10.20) — the `cl /Zi` → C1902 saga

Compiling with `/Zi` (separate `.pdb` debug info — what the game uses) fails
under **wine-10.0 stable** with:

```
fatal error C1902: Program database manager mismatch; please check your installation
```

### Root cause

`cl /Zi` writes the PDB through `mspdb80.dll`, which hands off to a
**`mspdbsrv.exe`** server process reached over **RPC** (even for a single
source). On wine-10.0, RPC is broken — every run logs
`err:ole:start_rpcss Failed to start RpcSs service`, so `mspdbsrv.exe` is
**never spawned** and cl can't reach the PDB manager → it reports the failure
as a "manager mismatch" (C1902). It is *not* a version mismatch.

**Fix:** use **`wineWowPackages.staging`** (wine-staging ≥ 10.20; nixpkgs
currently provides 11.8), which starts RpcSs and spawns `mspdbsrv.exe`
correctly. The flake's devShell pins this.

Ruled out before finding the wine-version cause: toolchain completeness and
versions (cl/`mspdb*`/`msobj*` are all the matched SP1 `…30729` build, correct
`mspdb80.dll` loads), RTM vs SP1, the `mspdb` source dir (the `Common7/IDE` and
`VSA/9.0/VsaEnv` copies are byte-identical), prefix arch (`wow64` and `win32`
both fail), and wow64-vs-multilib (both `stable` flavors fail at 10.0). It is
purely the wine *version*: 10.0's RPC/`mspdbsrv` path is broken, staging fixes it.
