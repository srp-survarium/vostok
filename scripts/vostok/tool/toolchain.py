#!/usr/bin/env python3
"""
setup-toolchain.py

Installs the Vostok build environment so that later `wine ninja.exe` invocations
in binaries/ninja/ can build the project. Steps:

  1. Stage vostok-libs (prebuilt blobs) into binaries.prebuilt/
  2. Initialise the Wine prefix
  3. Set PATH, INCLUDE, LIB in the Wine registry
  4. Run vcproj2ninja.exe under Wine to (re)generate binaries/ninja/build.ninja
  5. Generate the target-side diff inputs once (binaries/{objdiff,structure}/target)
     from the original game - these don't change between recompiles.

Does not run ninja itself. Steps 1-4 short-circuit if every input still hashes to
the previously recorded fingerprint AND all expected outputs exist; step 5 is
independently idempotent (skips whatever target output already exists).

The fingerprint tracks the toolchain paths and the .sln, not the individual
.vcproj files - so after editing a .vcproj, re-run with `--force ninja` to
regenerate the build graph. `--force {libs,wine,registry,ninja,target,all}`
re-runs the named stage(s) regardless of the fingerprint.

Run from inside `nix develop` after all Nix packages are built:
  nix build .#vostok-toolchain --out-link binaries/nix-store/vostok-toolchain
  nix build .#vostok-libs      --out-link binaries/nix-store/vostok-libs
  python3 scripts/setup-toolchain.py
  python3 scripts/setup-toolchain.py --force ninja   # after editing a .vcproj

Required env vars (set automatically by flake.nix devShell):
  MSVC_DIR         - VC compiler directory (has VC/bin/cl.exe)
  WINSDK_DIR       - Windows SDK directory (has Include/ and Lib/)
  DXSDK_DIR        - DirectX SDK directory (has Include/ and Lib/)
  NINJA_DIR        - directory containing ninja.exe
  WINEPREFIX       - Wine prefix directory
  VOSTOK_LIBS_DIR  - path to extracted vostok-libs Nix package
  VCPROJ2NINJA_EXE - path to vcproj2ninja.exe (run under Wine)
"""

import argparse
import os
import subprocess
import sys
from pathlib import Path

from vostok.build import ninja_regen

from vostok.core.paths import REPO as VOSTOK_DIR
from vostok.core.paths import SCRIPTS as SCRIPT_DIR

from vostok.core.paths import (OBJDIFF_DIR, PREBUILT, RICH_DIR, SETUP_STAMP,
                               STRUCTURE_DIR, WINEPREFIX)
from vostok.core.paths import NINJA_DIR as BUILD_DIR
from vostok.core.paths import SLN as SLN_PATH

# Setup stages, in run order. Each can be forced via `--force <stage>` even when
# the fingerprint says setup is already complete.
STAGES = ("libs", "wine", "registry", "ninja", "target")


def log(msg: str) -> None:
    print(f"[setup] {msg}", flush=True)


def die(msg: str, *hints: str) -> None:
    log(f"ERROR: {msg}")
    for h in hints:
        print(f"  {h}", file=sys.stderr)
    sys.exit(1)


def require_env(name: str) -> str:
    v = os.environ.get(name)
    if not v:
        die(f"{name} not set - run this from inside `nix develop`")
    return v


def winepath_w(p: Path) -> str:
    return subprocess.check_output(["winepath", "-w", str(p)], text=True).strip()


def wine_reg(*args: str, capture: bool = False) -> subprocess.CompletedProcess:
    kwargs = {"check": False, "text": True}
    if capture:
        kwargs["stdout"] = subprocess.PIPE
        kwargs["stderr"] = subprocess.PIPE
    else:
        kwargs["stdout"] = subprocess.DEVNULL
        kwargs["stderr"] = subprocess.DEVNULL
    return subprocess.run(["wine", "reg", *args], **kwargs)


def configure_registry(msvc_dir: Path, winsdk_dir: Path, dxsdk_dir: Path) -> None:
    vc_bin     = winepath_w(msvc_dir / "VC" / "bin")
    vc_inc     = winepath_w(msvc_dir / "VC" / "include")
    vc_lib     = winepath_w(msvc_dir / "VC" / "lib")
    winsdk_inc = winepath_w(winsdk_dir / "Include")
    winsdk_lib = winepath_w(winsdk_dir / "Lib")
    dxsdk_inc  = winepath_w(dxsdk_dir / "Include")
    dxsdk_lib  = winepath_w(dxsdk_dir / "Lib" / "x86")

    # DXSDK must precede WinSDK: d3d9types.h in WinSDK 6.0A is an older version
    # without DX9Ex types (D3DAUTHENTICATEDCHANNELTYPE etc.); the include guard
    # would shadow the DXSDK version if WinSDK came first.
    include_val = f"{vc_inc};{dxsdk_inc};{winsdk_inc}"
    lib_val     = f"{vc_lib};{dxsdk_lib};{winsdk_lib}"

    reg_env = (
        r"HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control"
        r"\Session Manager\Environment"
    )

    cur = wine_reg("query", reg_env, "/v", "PATH", capture=True)
    current_path = ""
    for line in cur.stdout.splitlines():
        if "REG_" in line:
            current_path = line.split()[-1]
            break

    if not current_path:
        new_path = f"{vc_bin};%SystemRoot%\\system32;%SystemRoot%"
    elif vc_bin not in current_path:
        new_path = f"{vc_bin};{current_path}"
    else:
        new_path = None

    if new_path is not None:
        wine_reg("add", reg_env, "/v", "PATH",
                 "/t", "REG_EXPAND_SZ", "/d", new_path, "/f")

    wine_reg("add", reg_env, "/v", "INCLUDE",
             "/t", "REG_SZ", "/d", include_val, "/f")
    wine_reg("add", reg_env, "/v", "LIB",
             "/t", "REG_SZ", "/d", lib_val, "/f")

    log("Wine environment configured.")
    log(f"  PATH+=   {vc_bin}")
    log(f"  INCLUDE= {include_val}")
    log(f"  LIB=     {lib_val}")


def compute_fingerprint(
    msvc_dir: Path, winsdk_dir: Path, dxsdk_dir: Path, ninja_dir: Path,
    libs_dir: Path, vcproj_exe: Path, wineprefix: Path,
) -> str:
    sln_stat = SLN_PATH.stat()
    lines = [
        f"msvc={msvc_dir.resolve()}",
        f"winsdk={winsdk_dir.resolve()}",
        f"dxsdk={dxsdk_dir.resolve()}",
        f"ninja={ninja_dir.resolve()}",
        f"libs={libs_dir.resolve()}",
        f"vcproj={vcproj_exe.resolve()}",
        f"wineprefix={wineprefix.resolve()}",
        f"sln_mtime={sln_stat.st_mtime_ns}",
        f"sln_size={sln_stat.st_size}",
    ]
    return "\n".join(lines) + "\n"


def _nonempty_dir(p: Path) -> bool:
    return p.is_dir() and any(p.iterdir())


def ensure_target_side(force: bool = False) -> None:
    """Generate the target-side diff inputs (the original game never changes):
    binaries/objdiff/target (COFF), binaries/structure/target (pdb-parser stubs),
    and binaries/rich/target (pdb_rich_context index for `pdb_fetch`).

    Idempotent - skips whichever output already exists (unless `force`), so this is
    cheap to call on every `nix develop`. Fatal: a failure here aborts setup so it
    doesn't go unnoticed.
    """
    import generate_delink
    import generate_rich
    import generate_structure

    objdiff_target   = OBJDIFF_DIR / "target"
    structure_target = STRUCTURE_DIR / "target"
    rich_target      = RICH_DIR / "target"
    if (
        not force
        and _nonempty_dir(objdiff_target)
        and _nonempty_dir(structure_target)
        and _nonempty_dir(rich_target)
    ):
        return  # already generated

    log("Generating target diff inputs (original game COFF + structure + rich index) ...")
    try:
        if force or not _nonempty_dir(objdiff_target):
            generate_delink.generate("target")
        if force or not _nonempty_dir(structure_target):
            generate_structure.generate("target")
        if force or not _nonempty_dir(rich_target):
            generate_rich.generate("target")
    except (RuntimeError, subprocess.CalledProcessError) as e:
        die(f"could not generate target diff inputs: {e}")
    log("Target diff inputs ready.")


def copy_libs(libs_dir: Path) -> None:
    log("Staging vostok-libs -> binaries.prebuilt/ ...")
    subprocess.check_call([
        sys.executable, str(SCRIPT_DIR / "copy_lib_files.py"),
        str(libs_dir / "sources"), str(PREBUILT),
    ])
    log("Library files staged.")


def init_wine_prefix(wineprefix: Path, force: bool = False) -> None:
    if not force and (wineprefix / "drive_c").is_dir():
        log("Wine prefix already initialised.")
        return
    log(f"Initialising Wine prefix at {wineprefix} ...")
    wineprefix.mkdir(parents=True, exist_ok=True)
    subprocess.run(["wineboot", "--init"], check=True)
    subprocess.run(["wineserver", "--wait"], check=False, stderr=subprocess.DEVNULL)
    log("Wine prefix ready.")


def generate_ninja(vcproj_exe: Path) -> None:
    # Pass native Linux paths for I/O (vcproj2ninja reads/writes them directly),
    # and --wine so the *emitted* build graph uses the drive-rooted `Z:\...` form
    # that ninja.exe/cl.exe resolve under Wine. The .exe sometimes exits non-zero
    # under wine even on success, so we trust build.ninja's presence over the code.
    BUILD_DIR.mkdir(parents=True, exist_ok=True)
    log(f"Generating ninja files in {BUILD_DIR} via vcproj2ninja ...")
    subprocess.run([
        "wine", str(vcproj_exe),
        "--wine",
        "--sln-path", str(SLN_PATH),
        "--configuration-platform", "Master Gold|Win32",
        "--output-dir", str(BUILD_DIR),
        "--project-name", "survarium - PC - DirectX 11",
    ], check=False)
    if not (BUILD_DIR / "build.ninja").is_file():
        die(f"vcproj2ninja did not produce {BUILD_DIR}/build.ninja")
    log("Ninja files generated.")


def ensure_compdb(force: bool = False) -> None:
    """clangd inputs at the repo root (gitignored, so absent on fresh clones
    and worktrees). Generated here so editors/clangd_query work before the
    first rebuild; rebuild.py keeps them fresh afterwards."""
    if not force and all(
        (VOSTOK_DIR / n).is_file() for n in ninja_regen.COMPDB_FILES
    ):
        return
    log("Generating clangd inputs (compile_commands.json + clangd-vfs.yaml) ...")
    ninja_regen.regenerate_compdb()


def parse_force(argv) -> set:
    ap = argparse.ArgumentParser(
        description="Set up the Vostok Wine/ninja build environment."
    )
    ap.add_argument(
        "--force", nargs="+", choices=(*STAGES, "all"), default=[], metavar="STAGE",
        help="Re-run these setup stages even when setup is already complete - e.g. "
             "`--force ninja` after editing a .vcproj (the fingerprint tracks only "
             "the .sln, so vcproj edits don't auto-regenerate the build graph). "
             f"Stages: {', '.join(STAGES)}, all.",
    )
    forced = ap.parse_args(argv).force
    return set(STAGES) if "all" in forced else set(forced)


def main() -> None:
    force = parse_force(sys.argv[1:])
    msvc_dir   = Path(require_env("MSVC_DIR"))
    winsdk_dir = Path(require_env("WINSDK_DIR"))
    dxsdk_dir  = Path(require_env("DXSDK_DIR"))
    ninja_dir  = Path(require_env("NINJA_DIR"))
    libs_dir   = Path(os.environ.get(
        "VOSTOK_LIBS_DIR", str(VOSTOK_DIR.parent / "vostok-libs"),
    ))

    wineprefix = Path(os.environ.get(
        "WINEPREFIX", str(WINEPREFIX),
    ))
    os.environ["WINEPREFIX"] = str(wineprefix)
    # Silence unactionable Wine debug spam (fixme stubs + kerberos err) while
    # keeping genuine errors visible. setdefault so `nix develop` can override.
    os.environ.setdefault("WINEDEBUG", "fixme-all,err-kerberos")
    os.environ.setdefault("WINEDLLOVERRIDES", "mscoree,mshtml=")

    if not (msvc_dir / "VC" / "bin" / "cl.exe").exists():
        die(
            f"cl.exe not found under {msvc_dir}/VC/bin",
            "Build it: nix build .#vostok-toolchain --out-link binaries/nix-store/vostok-toolchain",
        )
    if not (libs_dir / "sources").is_dir():
        die(
            f"vostok-libs not found at {libs_dir}",
            "Build it: nix build .#vostok-libs --out-link binaries/nix-store/vostok-libs",
        )

    vcproj_exe_env = os.environ.get("VCPROJ2NINJA_EXE")
    if not vcproj_exe_env:
        die(
            "VCPROJ2NINJA_EXE not set",
            "Build it: nix build .#vcproj2ninja --out-link binaries/nix-store/vcproj2ninja",
        )
    vcproj_exe = Path(vcproj_exe_env)

    if not SLN_PATH.is_file():
        die(f"Solution file not found: {SLN_PATH}")

    fingerprint = compute_fingerprint(
        msvc_dir, winsdk_dir, dxsdk_dir, ninja_dir,
        libs_dir, vcproj_exe, wineprefix,
    )

    # setup_current: every input still matches AND every output still exists.
    outputs = [wineprefix / "drive_c", BUILD_DIR / "build.ninja"]
    setup_current = (
        SETUP_STAMP.is_file()
        and SETUP_STAMP.read_text() == fingerprint
        and all(p.exists() for p in outputs)
    )

    if setup_current and not force:
        log("Wine/ninja setup already complete.")
        ensure_compdb()
        ensure_target_side()
        return
    if setup_current:
        log(f"Setup is current; forcing stage(s): {', '.join(sorted(force))}")

    # Each stage runs on a full (re)setup, or when explicitly forced.
    if not setup_current or "libs" in force:
        copy_libs(libs_dir)
    if not setup_current or "wine" in force:
        init_wine_prefix(wineprefix, force="wine" in force)
    if not setup_current or "registry" in force:
        log("Configuring Wine environment (PATH, INCLUDE, LIB) ...")
        configure_registry(msvc_dir, winsdk_dir, dxsdk_dir)
    if not setup_current or "ninja" in force:
        generate_ninja(vcproj_exe)
        ensure_compdb(force=True)

    # Record the fingerprint so the next plain run short-circuits.
    SETUP_STAMP.parent.mkdir(parents=True, exist_ok=True)
    SETUP_STAMP.write_text(fingerprint)

    ensure_target_side(force="target" in force)

    if not setup_current:
        log("")
        log("Toolchain layout:")
        log(f"  MSVC:    {msvc_dir}/VC/")
        log(f"  WinSDK:  {winsdk_dir}/")
        log(f"  DXSDK:   {dxsdk_dir}/")
        log(f"  Ninja:   {ninja_dir}/ninja.exe")
        log(f"  Wine:    {wineprefix}/")
        log(f"  Build:   {BUILD_DIR}/")
        log("")
        log(f"To build:  (cd {BUILD_DIR} && wine {ninja_dir}/ninja.exe)")


if __name__ == "__main__":
    main()
