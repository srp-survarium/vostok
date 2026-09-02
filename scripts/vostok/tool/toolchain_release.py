#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""
vostok.tool.toolchain_release

Produces a self-contained vostok-toolchain tarball with VS2008 SP1 correctly
applied, ready to publish as a GitHub release. Once published, update
flake.nix sha256 to fetch that release instead of building from ISOs.

Entry point:
  nix-shell scripts/create-toolchain-release.nix
  (downloads ISOs, sets env vars, and runs this script automatically)

Output:
  binaries/vostok-toolchain-v0.100b.tar.xz
    msvc/VC/   - VS2008 SP1 compiler (cl.exe, headers, x86 libs)
    winsdk/    - Windows SDK 6.0A headers + x86 libs
    dxsdk/     - DirectX SDK June 2010 headers + libs
    ninja/     - ninja.exe v1.12.1

Headless Wine:
  wineboot and `msiexec /a` create windows (the "updating Wine configuration"
  dialog, installer progress) via Wine's default X11 graphics driver - even
  though the build is non-interactive (/qn). On a desktop those pop up on
  screen. xvfb_prefix() runs them under a throwaway virtual X server (xvfb-run)
  so they render invisibly. See xvfb_prefix() below.
"""

import hashlib
import os
import shutil
import subprocess
from pathlib import Path

from vostok.core.paths import REPO

# VOSTOK_DIR is set by the nix-shell shellHook to $PWD (the repo root).
# Fallback for manual invocations where the script is run from its own directory.
VOSTOK_DIR = Path(os.environ.get("VOSTOK_DIR", str(REPO)))

# Fixed mtime stamped on every tar entry for reproducible packaging.
# Survarium v0.100b release date: 2013-05-09 12:00:00 UTC.
RELEASE_EPOCH = 1368100800


def log(msg: str) -> None:
    print(f"[release] {msg}", flush=True)


def run(cmd: list, *, check: bool = True, **kwargs) -> subprocess.CompletedProcess:
    return subprocess.run(cmd, check=check, **kwargs)


def winepath_w(p: Path) -> str:
    return subprocess.check_output(["winepath", "-w", str(p)], text=True).strip()


def xvfb_prefix() -> list[str]:
    """Always run Wine GUI steps under a throwaway virtual X server (xvfb-run -a).

    The toolchain build is fully unattended (msiexec /qn), so Wine needs no real
    display. Forcing xvfb even when DISPLAY is set keeps the "updating the Wine
    configuration" / prefix-creation dialog off the user's screen. xvfb-run is
    provided by create-toolchain-release.nix.
    """
    return ["xvfb-run", "-a"]


def wineserver_settle(timeout: int = 120) -> None:
    """Wait for Wine activity to drain, then guarantee an idle prefix.

    `wineserver --wait` blocks until *every* process in the prefix exits, but
    wineboot leaves persistent services (winedevice.exe) running that never exit
    on their own under wine-wow64 10.0 - so a bare --wait deadlocks (it hung the
    build at "Initialising Wine prefix" indefinitely). Cap the wait and force the
    server down on timeout: the prefix is already initialised on disk and any
    msiexec /a has already returned, so killing the idle server is safe and the
    next `wine` call just starts a fresh one.
    """
    try:
        run(["wineserver", "--wait"], check=False,
            stderr=subprocess.DEVNULL, timeout=timeout)
    except subprocess.TimeoutExpired:
        log(f"wineserver --wait exceeded {timeout}s; forcing the server down (-k)")
        run(["wineserver", "-k"], check=False, stderr=subprocess.DEVNULL)


# ---------------------------------------------------------------------------
# SP1 CRT overlay
#
# The PATCH= admin install bumps the *versioned* compiler PE files (cl/c1/c2/
# mspdb) to SP1, but Wine's msiexec does not lay down the patched, *unversioned*
# static CRT (libcmt.lib etc.) or the CRT headers - they stay at RTM 9.0.21022.
# The shipped game linked the SP1 (9.0.30729) CRT, so an RTM CRT will never
# byte-match; worse, RTM crtassem.h stamps every one of our SP1-compiled objects
# with a 9.0.21022 manifest dependency. So we overlay the SP1 CRT straight out
# of the SP1 MSP payload and then verify it. See docs/build/compiler-sp1-rtm.md.
# ---------------------------------------------------------------------------

SP1_BUILD = 30729
RTM_BUILD = 21022

# Static-link CRT archives that SP1 updates and that we may pull in via /MT(d).
CRT_LIBS = [
    "libcmt.lib", "libcmtd.lib", "libcpmt.lib", "libcpmtd.lib",
    "msvcrt.lib", "msvcrtd.lib", "msvcprt.lib", "msvcprtd.lib",
    "msvcmrt.lib", "msvcmrtd.lib", "msvcurt.lib", "msvcurtd.lib",
]


def comp_id_builds(data: bytes) -> set[int]:
    """Every @comp.id build number (low 16 bits) embedded in an obj/.lib blob.

    Scans for the literal "@comp.id" symbol name and reads the 4-byte Value that
    follows it: this works for plain COFF and for LTCG "anonymous object" files
    alike, without parsing the (format-specific) COFF symbol table. The low 16
    bits are the compiler build: 30729 = SP1, 21022 = RTM.
    """
    builds: set[int] = set()
    i = data.find(b"@comp.id")
    while i != -1:
        if i + 12 <= len(data):
            builds.add(int.from_bytes(data[i + 8:i + 12], "little") & 0xFFFF)
        i = data.find(b"@comp.id", i + 1)
    return builds


def lib_is_sp1(path: Path) -> bool:
    """True if a .lib archive's objects are SP1 (30729) and carry no RTM (21022)."""
    try:
        builds = comp_id_builds(path.read_bytes())
    except OSError:
        return False
    return SP1_BUILD in builds and RTM_BUILD not in builds


def _extract_archive(src: Path, dst: Path) -> None:
    dst.mkdir(parents=True, exist_ok=True)
    run(["7z", "x", "-y", f"-o{dst}", str(src)],
        stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, check=False)


def find_crt_msps(sp1_dir: Path) -> list[Path]:
    """SP1 MSP(s) that carry the x86 static CRT (libcmt.lib etc.).

    The static CRT ships - as *whole files* - in the Visual C++ SP1 patch,
    VC90sp1-*-x86-*.msp, NOT the umbrella VS90sp1 IDE MSP that PATCH= consumes.
    (The first cut of this overlay read VS90sp1 and so replaced nothing.) Glob
    rather than hardcode the KB number, and prefer the focused English x86 VC
    patch so we don't unpack the much larger x86_x64 / x86_IA64 cross MSPs.
    """
    msps = sorted(
        p for p in sp1_dir.rglob("VC90sp1-*.msp") if "x86" in p.name.lower()
    )
    enu = [p for p in msps if "x86-enu" in p.name.lower()]
    return enu or msps


def overlay_sp1_crt(crt_msps: list[Path], stage_vc: Path, work: Path) -> int:
    """Overlay the SP1 static CRT (libs + crtassem.h) from the VC SP1 MSP(s).

    Wine's `msiexec /a PATCH=` updates the versioned compiler but leaves the
    static CRT at RTM. The SP1 CRT ships as whole files in the VC SP1 MSP, stored
    under their MSI File-table key, e.g. `FL_libcmt_lib_7051_x86_ln.<GUID>` ->
    libcmt.lib (the key is the name with '.' -> '_'). 7z unpacks the members
    straight out; we match each CRT target by its `FL_<stem>_<ext>_` key prefix,
    confirm the member is genuinely SP1 (@comp.id 30729, no 21022), and copy it
    over the RTM file staged in VC/. Returns the number of files replaced.
    """
    log("Overlaying SP1 CRT from the VC SP1 MSP(s) ...")
    payload = work / "sp1-crt"
    if payload.exists():
        shutil.rmtree(payload, ignore_errors=True)
    for msp in crt_msps:
        out = payload / msp.stem
        _extract_archive(msp, out)
        # Most media expose the files directly; some nest them in a patch.cab.
        # Unpack any cab we find so both layouts work.
        for f in list(out.iterdir()):
            if f.is_file() and f.suffix.lower() == ".cab":
                _extract_archive(f, out / "cab")
    files = [p for p in payload.rglob("*") if p.is_file()]

    replaced = 0

    # 1) static CRT libs -> VC/lib (only replace ones we actually staged).
    lib_dir = stage_vc / "lib"
    for target in CRT_LIBS:
        staged = lib_dir / target
        if not staged.exists():
            continue
        stem, ext = target.rsplit(".", 1)
        prefix = f"fl_{stem}_{ext}_"
        cand = next(
            (p for p in files
             if p.name.lower().startswith(prefix) and lib_is_sp1(p)),
            None,
        )
        if cand:
            shutil.copy2(str(cand), str(staged))
            replaced += 1
            log(f"  CRT lib  {target}  <- SP1  ({cand.name})")

    # 2) crtassem.h carries _CRT_ASSEMBLY_VERSION, which the compiler embeds into
    #    every object as a manifest dependency (RTM 9.0.21022.8 vs SP1 9.0.30729).
    for staged in (stage_vc / "include" / "crtassem.h",
                   stage_vc / "crt" / "src" / "crtassem.h"):
        if not staged.exists():
            continue
        cand = next(
            (p for p in files
             if p.name.lower().startswith("fl_crtassem_h_")
             and "_x86_" in p.name.lower()
             and b"9.0.30729" in p.read_bytes()),
            None,
        )
        if cand:
            shutil.copy2(str(cand), str(staged))
            replaced += 1
            log(f"  CRT hdr  crtassem.h ({staged.parent.name})  <- SP1")

    log(f"SP1 CRT overlay: replaced {replaced} file(s).")
    return replaced


def verify_crt_sp1(stage_vc: Path, *, fatal: bool) -> None:
    """Check the staged static CRT (and crtassem.h) are SP1, not RTM.

    This is the gate that would have caught the RTM CRT silently shipping: the
    old script only checked cl.exe. Run after overlay_sp1_crt().

    The static `libcmt.lib` is the hard requirement (its objects link straight
    into the EXE), so a still-RTM lib aborts when `fatal`. `crtassem.h` only ever
    reaches our objects via the manifest-dependency pragma (which `/MT` static
    linking suppresses), so a still-RTM header is a warning, not a failure.
    """
    libcmt = stage_vc / "lib" / "libcmt.lib"
    lib_ok = False
    if libcmt.exists():
        builds = comp_id_builds(libcmt.read_bytes())
        lib_ok = SP1_BUILD in builds and RTM_BUILD not in builds
        if lib_ok:
            log("libcmt.lib is SP1 (30729) OK")
        else:
            log(f"CRT CHECK: libcmt.lib is not SP1 (@comp.id builds={sorted(builds)})")
    else:
        log("CRT CHECK: libcmt.lib missing from staged VC/lib")

    crtassem = stage_vc / "include" / "crtassem.h"
    if crtassem.exists():
        ver = crtassem.read_text(errors="ignore")
        if "9.0.30729" in ver:
            log("crtassem.h is SP1 (9.0.30729) OK")
        elif "9.0.21022" in ver:
            log("CRT CHECK: crtassem.h still RTM (9.0.21022.8) - warning only "
                "(suppressed by /MT, but inlined CRT code may still differ)")

    if not lib_ok and fatal:
        raise SystemExit(
            "Toolchain static CRT is RTM, not SP1 - the SP1 overlay did not take. "
            "libcmt.lib must be 9.0.30729 to match the game; see "
            "docs/build/compiler-sp1-rtm.md. Aborting rather than ship a "
            "toolchain that links the wrong CRT."
        )


# ---------------------------------------------------------------------------
# Step 1 helpers
# ---------------------------------------------------------------------------

def find_vc_bin(root: Path) -> Path | None:
    """
    Find the x86 VC bin directory (the one containing cl.exe) inside a VS2008
    install tree. Excludes the amd64, x86_amd64, and CE cross-compiler bins.
    Returns the bin/ directory path (matching find -printf "%h\n" semantics).
    """
    for cl in root.rglob("cl.exe"):
        if cl.parent.name != "bin":
            continue
        if any(x in cl.parts for x in ("amd64", "x86_amd64", "ce")):
            continue
        return cl.parent
    return None


# ---------------------------------------------------------------------------
# Step 1: VS2008 administrative install + SP1 patch
# ---------------------------------------------------------------------------

def step1_vs2008(work: Path, stage: Path) -> None:
    vs2008_iso = Path(os.environ["VS2008_ISO"])
    sp1_iso    = Path(os.environ["VS2008_SP1_ISO"])

    log("Extracting VS2008 ISO ...")
    iso_dir = work / "vs2008-iso"
    iso_dir.mkdir(parents=True, exist_ok=True)
    # -tUDF: the .iso is a UDF (DVD) filesystem image.
    run(["7z", "x", str(vs2008_iso), "-tUDF", f"-o{iso_dir}", "-y"],
        stdout=subprocess.DEVNULL, check=False)

    vs_msi = None
    for candidate in [iso_dir / "Setup" / "vs_setup.msi", iso_dir / "vs_setup.msi"]:
        if candidate.exists():
            vs_msi = candidate
            break
    if not vs_msi:
        raise SystemExit("ERROR: vs_setup.msi not found in VS2008 ISO")

    # VS2008 SP1: wine's msiexec cannot patch an administrative image after the
    # fact (`/p msp /a msi` returns ERROR_CALL_NOT_IMPLEMENTED=120). The supported
    # path is to apply the patch *during* the admin install via the PATCH
    # property. Without SP1 the compiler/mspdb stay at RTM (15.00/9.00.21022),
    # which mismatches the game's SP1 build and triggers C1902 under wine.
    log("Extracting VS2008 SP1 ISO ...")
    sp1_dir = work / "sp1-iso"
    sp1_dir.mkdir(parents=True, exist_ok=True)
    run(["7z", "x", str(sp1_iso), "-tUDF", f"-o{sp1_dir}", "-y"],
        stdout=subprocess.DEVNULL, check=False)
    sp1_msp = next(
        (p for p in [
            sp1_dir / "vs90sp1" / "VS90sp1-KB945140-X86-ENU.msp",
            sp1_dir / "VS90sp1-KB945140-X86-ENU.msp",
        ] if p.exists()),
        None,
    )
    if not sp1_msp:
        log("WARNING: SP1 MSP not found - packaging base VS2008 (RTM) without SP1.")

    # The static CRT lives in a *different* MSP (the VC patch) than the umbrella
    # VS90sp1 IDE MSP used for PATCH= above; locate it for the post-install CRT
    # overlay (PATCH= bumps the compiler but not the unversioned CRT under wine).
    crt_msps = find_crt_msps(sp1_dir)
    if not crt_msps:
        log("WARNING: VC SP1 MSP (VC90sp1-*-x86-*.msp) not found - "
            "static CRT overlay will be skipped (CRT stays RTM).")

    # Both ISOs are extracted; wine is only needed from here on (the admin install).
    log("Initialising Wine prefix ...")
    Path(os.environ["WINEPREFIX"]).mkdir(parents=True, exist_ok=True)
    run(xvfb_prefix() + ["wineboot", "--init"])
    wineserver_settle()

    # Not a real install: `msiexec /a` just unpacks the MSI files (with SP1 folded
    # in via PATCH=) into TARGETDIR - no registry/system changes.
    log("Running VS2008 administrative install (this may take a while) ...")
    admin_dir = work / "vs-admin"
    admin_dir.mkdir(parents=True, exist_ok=True)
    cmd = [
        "wine", "msiexec", "/a", winepath_w(vs_msi),
        f"TARGETDIR={winepath_w(admin_dir)}", "/qn",
    ]
    if sp1_msp:
        log(f"Applying SP1 during admin install via PATCH={sp1_msp.name}")
        cmd.append(f"PATCH={winepath_w(sp1_msp)}")
    result = run(xvfb_prefix() + cmd, stderr=subprocess.DEVNULL, check=False)
    if result.returncode != 0:
        log("WARNING: msiexec /a exited non-zero. Checking for files ...")
    wineserver_settle()

    vc_bin_dir = find_vc_bin(admin_dir)
    if not vc_bin_dir:
        log("ERROR: cl.exe not found after administrative install.")
        log("  Possible causes:")
        log("    - Wine msiexec /a requires a display. Try setting DISPLAY=:99")
        log("    - The ISO extraction was incomplete.")
        for d in list(admin_dir.rglob("*"))[:20]:
            if d.is_dir():
                print(f"  {d}")
        raise SystemExit(1)
    vc_dir = vc_bin_dir.parent
    log(f"Found VC at: {vc_dir}")

    # Verify SP1 landed: RTM cl/mspdb are 15.00/9.00.21022, SP1 is .30729.
    cl_ver = subprocess.run(
        ["strings", "-el", str(vc_bin_dir / "cl.exe")],
        capture_output=True, text=True, check=False,
    ).stdout
    if "15.00.30729" in cl_ver:
        log("cl.exe is SP1 (15.00.30729) OK")
    elif sp1_msp:
        log("WARNING: SP1 requested via PATCH= but cl.exe is still RTM "
            "(15.00.21022) - the patch did not apply.")
    else:
        log("cl.exe is RTM (15.00.21022) - built without SP1.")

    stage_vc = stage / "msvc" / "VC"
    stage_vc.mkdir(parents=True, exist_ok=True)
    shutil.copytree(str(vc_dir), str(stage_vc), dirs_exist_ok=True)

    # cl.exe imports PDB-writer libraries at runtime (mspdb80.dll, mspdbcore.dll,
    # mspdbsrv.exe, msobj80.dll, ...). Following the portable MSVC toolchain
    # convention (itsmattkc/MSVC420), copy *all* mspdb*.* and msobj*.dll next to
    # cl.exe so they're found on $PATH. Without them cl fails to load (c0000135).
    #
    # These MUST come from Common7/IDE: it ships the build that matches the VC
    # compiler. Other copies exist in the install (e.g. Common Files/Microsoft
    # Shared/VSA/9.0/VsaEnv) but are a different build, and mixing them with
    # cl/c1/c2 yields "C1902: Program database manager mismatch". So prefer
    # Common7/IDE and only fall back to a tree-wide search if it's absent.
    ide_dir = vc_dir.parent / "Common7" / "IDE"
    helpers: dict[str, Path] = {}
    for root in (ide_dir, admin_dir):
        if not root.is_dir():
            continue
        for pattern in ("mspdb*.*", "msobj*.dll"):
            for src in sorted(root.rglob(pattern)):
                if src.is_file():
                    helpers.setdefault(src.name.lower(), src)  # first (IDE) wins
    if not helpers:
        log(f"WARNING: no mspdb*/msobj* helpers found under {admin_dir} "
            "- cl.exe will fail to load")
    for src in sorted(helpers.values()):
        shutil.copy2(str(src), str(stage_vc / "bin" / src.name))
        log(f"  bundled {src.name} from {src}")

    # SP1 patched cl/c1/c2 above, but Wine's msiexec left the static CRT and its
    # headers at RTM. Overlay the SP1 CRT from the MSP, then verify (the old
    # script only checked cl.exe, so an RTM CRT shipped silently).
    if crt_msps:
        overlay_sp1_crt(crt_msps, stage_vc, work)
        verify_crt_sp1(stage_vc, fatal=True)
    else:
        verify_crt_sp1(stage_vc, fatal=False)


# ---------------------------------------------------------------------------
# Step 2: Windows SDK 6.0A
# ---------------------------------------------------------------------------

def step2_winsdk(work: Path, stage: Path) -> None:
    log("Extracting Windows SDK ...")
    vs2008_iso = Path(os.environ["VS2008_ISO"])
    iso_dir    = work / "vs2008-iso"

    sdk_build = iso_dir / "WCU" / "WinSDK" / "WinSDK_Build.exe"
    if not sdk_build.exists():
        # VS2008 ISO may already be extracted above; if not, grab just this file.
        run(["7z", "x", str(vs2008_iso), "-tUDF", "WCU/WinSDK/WinSDK_Build.exe",
             f"-o{iso_dir}", "-y"],
            stdout=subprocess.DEVNULL, check=False)
    if not sdk_build.exists():
        raise SystemExit("ERROR: WinSDK_Build.exe not found")

    sfx_dir      = work / "sdk-sfx"
    sdk_extracted = work / "sdk-extracted"
    sfx_dir.mkdir(parents=True, exist_ok=True)
    sdk_extracted.mkdir(parents=True, exist_ok=True)

    # WinSDK_Build.exe is a self-extracting archive; 7z unpacks it to the inner MSI.
    run(["7z", "x", str(sdk_build), f"-o{sfx_dir}", "-y"],
        stdout=subprocess.DEVNULL)

    msi = sfx_dir / "VistaClientHeadersLibs-x86.msi"
    if not msi.exists():
        raise SystemExit("ERROR: VistaClientHeadersLibs-x86.msi not found")

    run(["msiextract", "-C", str(sdk_extracted), msi.name], cwd=str(sfx_dir))

    sdk_inc = next((p for p in sdk_extracted.rglob("Include") if p.is_dir()), None)
    sdk_lib = next(
        (p for p in sdk_extracted.rglob("Lib")
         if p.is_dir() and "x64" not in str(p)),
        None,
    )
    if not sdk_inc:
        raise SystemExit("ERROR: WinSDK Include not found")
    if not sdk_lib:
        raise SystemExit("ERROR: WinSDK Lib not found")

    stage_sdk = stage / "winsdk"
    stage_sdk.mkdir(parents=True, exist_ok=True)
    shutil.copytree(str(sdk_inc), str(stage_sdk / "Include"))
    shutil.copytree(str(sdk_lib), str(stage_sdk / "Lib"))
    log(f"WinSDK: {len(list((stage_sdk / 'Include').iterdir()))} headers.")


# ---------------------------------------------------------------------------
# Step 3: DirectX SDK June 2010
# ---------------------------------------------------------------------------

def step3_dxsdk(work: Path, stage: Path) -> None:
    log("Extracting DirectX SDK ...")
    dxsdk_exe = Path(os.environ["DXSDK_EXE"])

    dx_work = work / "dxsdk"
    dx_work.mkdir(parents=True, exist_ok=True)
    run(["7z", "x", str(dxsdk_exe), f"-o{dx_work}", "-y"],
        stdout=subprocess.DEVNULL)

    src_root = None
    for candidate in [dx_work / "DXSDK", dx_work]:
        if (candidate / "Include").is_dir():
            src_root = candidate
            break
    if not src_root:
        raise SystemExit("ERROR: DXSDK Include not found")

    stage_dx = stage / "dxsdk"
    shutil.copytree(str(src_root / "Include"), str(stage_dx / "Include"))
    shutil.copytree(str(src_root / "Lib"),     str(stage_dx / "Lib"))
    log(f"DXSDK: {len(list((stage_dx / 'Include').iterdir()))} headers.")


# ---------------------------------------------------------------------------
# Step 4: ninja.exe
# ---------------------------------------------------------------------------

def step4_ninja(work: Path, stage: Path) -> None:
    log("Extracting ninja.exe ...")
    ninja_zip = Path(os.environ["NINJA_WIN_ZIP"])

    ninja_dir = stage / "ninja"
    ninja_dir.mkdir(parents=True, exist_ok=True)
    run(["7z", "e", str(ninja_zip), "ninja.exe", f"-o{ninja_dir}", "-y"],
        stdout=subprocess.DEVNULL)
    if not (ninja_dir / "ninja.exe").exists():
        raise SystemExit("ERROR: ninja.exe not found in zip")


# ---------------------------------------------------------------------------
# Step 5: Package
# ---------------------------------------------------------------------------

def step5_package(work: Path, stage: Path, output: Path) -> None:
    log(f"Packaging -> {output} ...")
    output.parent.mkdir(parents=True, exist_ok=True)
    # Reproducibility: the staged file *contents* are already deterministic - a
    # fresh rebuild yields byte-identical files (verified file-by-file by sha256
    # against the published release). The only nondeterminism is tar metadata,
    # so normalise all of it:
    #   --sort=name       stable entry order, independent of filesystem readdir
    #   --mtime=@...      fix every entry's timestamp to RELEASE_EPOCH instead of
    #                     build time / extraction time (msiextract and created
    #                     directories otherwise stamp "now")
    #   --owner/--group/--numeric-owner  drop the building user's identity
    #   --format=gnu      avoid pax atime/ctime extended headers
    # With these, the same media + pinned tooling produce a byte-identical
    # tarball on any machine.
    run([
        "tar",
        "--sort=name",
        "--format=gnu",
        "--owner=0", "--group=0", "--numeric-owner",
        f"--mtime=@{RELEASE_EPOCH}",
        "--transform", r"s|^\.|vostok-toolchain-v0.100b|",
        "-C", str(stage),
        "-cJf", str(output), ".",
    ])

    h = hashlib.sha256()
    with output.open("rb") as f:
        for chunk in iter(lambda: f.read(65536), b""):
            h.update(chunk)
    digest  = h.hexdigest()
    size_gb = output.stat().st_size / (1024 ** 3)

    print()
    log("Done!")
    print(f"  Output: {output}  ({size_gb:.1f} GB)")
    print(f"  SHA256: {digest}")
    print()
    print("Next steps:")
    print(f"  1. gh release upload v0.100b {output} --repo srp-survarium/vostok-build-env")
    print( "  2. In flake.nix, replace the vostok-toolchain sha256 with:")
    print(f'       sha256 = "{digest}";')


# ---------------------------------------------------------------------------
# Entry point
# ---------------------------------------------------------------------------

def main() -> None:
    output = Path(os.environ.get(
        "OUTPUT",
        str(VOSTOK_DIR / "binaries" / "vostok-toolchain-v0.100b.tar.xz"),
    ))
    work  = Path(os.environ.get(
        "WORK_DIR",
        str(VOSTOK_DIR / "binaries" / ".release-work"),
    ))
    stage = work / "stage"

    work.mkdir(parents=True, exist_ok=True)
    stage.mkdir(parents=True, exist_ok=True)

    os.environ.setdefault("WINEPREFIX", str(work / ".wineprefix"))
    os.environ["WINEDEBUG"]        = "-all"
    os.environ["WINEDLLOVERRIDES"] = "mscoree,mshtml="

    log(f"Work dir: {work}")
    log(f"Output:   {output}")
    print()

    try:
        step1_vs2008(work, stage)
        step2_winsdk(work, stage)
        step3_dxsdk(work, stage)
        step4_ninja(work, stage)
        step5_package(work, stage, output)
    finally:
        log("Cleaning up work dir ...")
        shutil.rmtree(work, ignore_errors=True)


if __name__ == "__main__":
    main()
