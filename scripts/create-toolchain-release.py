#!/usr/bin/env python3
"""
create-toolchain-release.py

Produces a self-contained vostok-toolchain tarball with VS2008 SP1 correctly
applied, ready to publish as a GitHub release. Once published, update
flake.nix sha256 to fetch that release instead of building from ISOs.

Entry point:
  nix-shell scripts/create-toolchain-release.nix
  (downloads ISOs, sets env vars, and runs this script automatically)

Output:
  binaries/vostok-toolchain-v0.100b.tar.xz
    msvc/VC/   — VS2008 SP1 compiler (cl.exe, headers, x86 libs)
    winsdk/    — Windows SDK 6.0A headers + x86 libs
    dxsdk/     — DirectX SDK June 2010 headers + libs
    ninja/     — ninja.exe v1.12.1
"""

import hashlib
import os
import shutil
import subprocess
from pathlib import Path

# VOSTOK_DIR is set by the nix-shell shellHook to $PWD (the repo root).
# Fallback for manual invocations where the script is run from its own directory.
VOSTOK_DIR = Path(os.environ.get("VOSTOK_DIR", str(Path(__file__).resolve().parent.parent)))


def log(msg: str) -> None:
    print(f"[release] {msg}", flush=True)


def run(cmd: list, *, check: bool = True, **kwargs) -> subprocess.CompletedProcess:
    return subprocess.run(cmd, check=check, **kwargs)


def winepath_w(p: Path) -> str:
    return subprocess.check_output(["winepath", "-w", str(p)], text=True).strip()


def xvfb_prefix() -> list[str]:
    """Return [xvfb-run, -a] when no display is available, else empty."""
    return ["xvfb-run", "-a"] if not os.environ.get("DISPLAY") else []


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
    run(["7z", "x", str(vs2008_iso), "-tUDF", f"-o{iso_dir}", "-y"],
        stdout=subprocess.DEVNULL, check=False)

    vs_msi = None
    for candidate in [iso_dir / "Setup" / "vs_setup.msi", iso_dir / "vs_setup.msi"]:
        if candidate.exists():
            vs_msi = candidate
            break
    if not vs_msi:
        raise SystemExit("ERROR: vs_setup.msi not found in VS2008 ISO")

    log("Initialising Wine prefix ...")
    Path(os.environ["WINEPREFIX"]).mkdir(parents=True, exist_ok=True)
    run(xvfb_prefix() + ["wineboot", "--init"])
    run(["wineserver", "--wait"], check=False, stderr=subprocess.DEVNULL)

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
        log("WARNING: SP1 MSP not found — packaging base VS2008 (RTM) without SP1.")

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
    run(["wineserver", "--wait"], check=False, stderr=subprocess.DEVNULL)

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
        log("cl.exe is SP1 (15.00.30729) ✓")
    elif sp1_msp:
        log("WARNING: SP1 requested via PATCH= but cl.exe is still RTM "
            "(15.00.21022) — the patch did not apply.")
    else:
        log("cl.exe is RTM (15.00.21022) — built without SP1.")

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
            "— cl.exe will fail to load")
    for src in sorted(helpers.values()):
        shutil.copy2(str(src), str(stage_vc / "bin" / src.name))
        log(f"  bundled {src.name} from {src}")


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
    log(f"Packaging → {output} ...")
    output.parent.mkdir(parents=True, exist_ok=True)
    run([
        "tar", "-C", str(stage),
        "--transform", r"s|^\.|vostok-toolchain-v0.100b|",
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
