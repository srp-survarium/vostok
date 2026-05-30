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

### 2b. PATCH= does *not* update the static CRT — overlay it from the MSP

`PATCH=` bumps the **versioned** compiler PE files (`cl`/`c1`/`c2`/`mspdb`) to
SP1, but Wine's `msiexec` does **not** lay down the patched, **unversioned**
static CRT. The result is a toolchain whose compiler is SP1 yet whose
`VC/lib/libcmt.lib`, `libcpmt.lib` (and the `*d`/`msvcrt*` variants) and CRT
headers stay at **RTM `9.0.21022`**. This shipped silently for a while because
the only check was on `cl.exe`. Consequences (see
[compiler-sp1-rtm.md](compiler-sp1-rtm.md)):

* the linked static-CRT objects are RTM and never byte-match the game's SP1 CRT;
* `VC/include/crtassem.h` (`_CRT_ASSEMBLY_VERSION "9.0.21022.8"`) is inlined into
  **every** object we compile as its manifest dependency — an embedded RTM-vs-SP1
  difference in our own engine objects.

The script now fixes this in `step1_vs2008`:

* **`find_crt_msps()`** locates the patch that actually carries the static CRT.
  It is **not** the umbrella `VS90sp1-KB945140` IDE MSP that `PATCH=` consumes
  (that one bumps the compiler, not the CRT) — the CRT ships in the **Visual C++**
  patch `VC90sp1-*-x86-*.msp` (e.g. `VC90sp1-KB947888-x86-enu.msp`). Reading the
  wrong MSP is exactly why the first cut of the overlay replaced **0** files.
* **`overlay_sp1_crt()`** 7z-unpacks the VC MSP (the members come straight out as
  whole files keyed by their MSI File-table name, e.g.
  `FL_libcmt_lib_7051_x86_ln.<GUID>` → `libcmt.lib`), matches each CRT target by
  its `FL_<stem>_<ext>_` key prefix, confirms the member is genuinely SP1
  (`@comp.id` 30729, no 21022), and copies it over the RTM file in the staged
  `VC/`. (No `msiinfo`/File-table lookup is needed — the key prefix is the name.)
* **`verify_crt_sp1(fatal=True)`** then reads `@comp.id` from `libcmt.lib` (low
  16 bits = build: `30729` SP1 vs `21022` RTM) and the `crtassem.h` version, and
  **aborts the build** if the CRT is still RTM — so a wrong-CRT toolchain can no
  longer ship unnoticed.

> The `@comp.id`/`crtassem.h` checks are pure-Python (no Wine) and are unit-safe.
> The matcher was validated against the real `VC90sp1-KB947888-x86-enu.msp`
> payload: all 12 CRT libs + `crtassem.h` resolve and verify SP1. If
> `verify_crt_sp1` aborts on other media, check that `find_crt_msps` matched a VC
> patch and that the `FL_<stem>_<ext>_` member keys follow the same convention.

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
