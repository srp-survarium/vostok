# Building the vostok-toolchain release (Linux / Wine)

The `vostok-toolchain` tarball (VS2008 SP1 compiler + WinSDK + DXSDK + ninja) is
produced on Linux from the original installation media:

```
nix-shell scripts/create-toolchain-release.nix
```

This fetches the VS2008 Professional ISO, the VS2008 SP1 ISO, and the DXSDK
installer, then runs `vostok.tool.toolchain_release`, which does a Wine
`msiexec` administrative install and stages the files into
`binaries/vostok-toolchain-v0.100b.tar.xz`. Publish it to this repo's
build-environment release:

```sh
gh release upload v0.100b binaries/vostok-toolchain-v0.100b.tar.xz --repo srp-survarium/vostok --clobber
```

Then update the `vostok-toolchain` `sha256` in `flake.nix`.

## What goes into the tarball (and what doesn't)

Three media are unpacked; only the **x86 native-build** pieces are kept. Final
layout: `msvc/VC/`, `winsdk/`, `dxsdk/`, `ninja/`.

### 1. VS2008 Professional ISO (`en_visual_studio_2008_professional_x86_dvd`)

```
Setup/vs_setup.msi          -> admin install (msiexec /a, with PATCH= below)
    VC/  (bin, lib, include, atlmfc, ...)   KEPT  -> whole VC/ tree -> msvc/VC/
    Common7/IDE/mspdb*.*, msobj*.dll        KEPT  -> copied next to cl.exe
    the IDE, .NET, samples, redist          SKIP  (not copied)
WCU/WinSDK/WinSDK_Build.exe (self-extracting)
    VistaClientHeadersLibs-x86.msi
        Include + Lib (x86)                 KEPT  -> winsdk/
        Lib x64                             SKIP
```

Note: the *entire* `VC/` tree is copied, so the unused amd64 / x86_amd64 cross
bins+libs ride along; only the x86 `cl.exe` is verified and used.

### 2. VS2008 SP1 ISO (`VS2008SP1ENUX1512962.iso`) - 13 `.msp` patches

Two patches contribute; the other eleven are skipped.

```
VS90sp1-KB945140-X86-ENU.msp    APPLIED as PATCH= during the admin install.
                                Bumps the compiler PE files (cl/c1/c2/mspdb)
                                RTM 15.00.21022 -> SP1 15.00.30729.
                                Does NOT update the static CRT -> hence the overlay.

VC90sp1-KB947888-x86-enu.msp    OVERLAID, but ONLY these whole-file members
                                (matched by their FL_<name>_<ext>_ MSI File key):
                                  libcmt.lib  libcmtd.lib  libcpmt.lib  libcpmtd.lib
                                  msvcrt.lib  msvcrtd.lib  msvcprt.lib  msvcprtd.lib
                                  msvcmrt.lib msvcmrtd.lib msvcurt.lib msvcurtd.lib
                                  crtassem.h   (-> _CRT_ASSEMBLY_VERSION 9.0.30729)
                                Everything else in this patch is SKIPPED:
                                  mfc90*.lib / atl*.lib + their headers/sources,
                                  the other CRT/MFC/ATL headers, redist DLLs.

SKIPPED entirely:
  VS90sp1-KB945140-{X64,IA64}-ENU.msp    other-arch compiler/IDE
  VC90sp1-KB948484-x86_x64-enu.msp       x64 VC
  VC90sp1-KB948560-x86_IA64-enu.msp      IA64 VC
  WinSDK-KB946729 / KB946733 / KB950424  WinSDK SP1 (our WinSDK stays 6.0A base)
  WebDesignerCore_KB945140 / KB950278    web designer
  vstor30sp1-KB949258-x86                VS Tools for Office runtime
  DTE90SP1-KB950425-ENU                  DTE / automation
```

### 3. DirectX SDK June 2010 (`DXSDK_Jun10.exe`)

```
Include + Lib   KEPT  -> dxsdk/
docs, samples, utilities, redist   SKIP  (not extracted)
```

### Deliberate SP1 gaps

Only the **static CRT libs** and **`crtassem.h`** are taken from the SP1 VC
patch. The other CRT headers (`crtdefs.h`, ...), MFC, and ATL therefore stay at
the RTM state the admin install produced. This has not affected matching (engine
objects link `/MT`, which suppresses the `crtassem` manifest, and objdiff does
not score the CRT; the compiler itself must still be SP1, because the retail PDB
records the SP1 compiler backend build). If an inlined CRT header or MFC/ATL ever
needs to match, it would be overlaid the same way as the CRT libs.

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
the only check was on `cl.exe`. The toolchain has to be SP1 throughout because
the retail PDB records the SP1 compiler backend build. Consequences:

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
