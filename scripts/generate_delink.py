#!/usr/bin/env python3
"""
generate_delink.py - run vostok-delinker to split an EXE into per-unit COFF .obj files
for objdiff comparison, for one side:

  base    the freshly compiled game
          binaries/Win32/survarium-dx11-win32-gold.{exe,pdb}
          -> binaries/objdiff/base

  target  the original game
          $SURVARIUM_BIN/survarium.{exe,pdb}
          -> binaries/objdiff/target

After delinking it refreshes binaries/objdiff/objdiff.json via
generate_objdiff_config.py. This mirrors vostok-delinker/build_{base,target}.sh
but is driven from Python so the whole build/diff loop stays in this repo.

Usage:
  python3 scripts/generate_delink.py base
  python3 scripts/generate_delink.py target

Env vars (set automatically by flake.nix devShell):
  SURVARIUM_BIN   - directory with the original survarium.{exe,pdb} (target side)
  VOSTOK_DELINKER - vostok-delinker binary to invoke (default: vostok-delinker on PATH)
"""

import argparse
import os
import shutil
import subprocess
import sys
from pathlib import Path


SCRIPT_DIR  = Path(__file__).resolve().parent
VOSTOK_DIR  = SCRIPT_DIR.parent
OBJDIFF_DIR = VOSTOK_DIR / "binaries" / "objdiff"
WIN32_DIR   = VOSTOK_DIR / "binaries" / "Win32"


def log(msg: str) -> None:
    print(f"[delink] {msg}", flush=True)


def _delinker_bin() -> str:
    return os.environ.get("VOSTOK_DELINKER", "vostok-delinker")


def _wine_path(p: Path) -> str:
    r"""Render a native absolute path the way MSVC-under-Wine records it in a PDB:
    on the Z: drive (Wine maps ``/`` -> ``Z:``), lowercased, ``\``-separated.
    e.g. /home/u/Proj/vostok/sources -> z:\home\u\proj\vostok\sources
    """
    return "z:" + str(p).replace("/", "\\").lower()


def generate(side: str) -> None:
    """Delink <side> into binaries/objdiff/<side> and refresh objdiff.json.

    Raises RuntimeError if inputs are missing and CalledProcessError if the
    delinker / config generator fail - callers handle/report these.
    """
    delinker = _delinker_bin()

    if side == "base":
        exe = WIN32_DIR / "survarium-dx11-win32-gold.exe"
        pdb = WIN32_DIR / "survarium-dx11-win32-gold.pdb"
        # The base PDB is MSVC-built under Wine, which records source paths under
        # the Z: drive (Wine maps `/` -> Z:), e.g. z:\home\...\vostok\sources\....
        # Pass the Wine form of <repo>/sources with a trailing separator (the
        # delinker strips this prefix off each recorded path), mirroring target's
        # bare `c:/survarium/sources`.
        engine = ["--engine-path", _wine_path(VOSTOK_DIR / "sources") + "\\"]
        hint = "build first (python3 scripts/rebuild.py)"
    elif side == "target":
        survarium_bin = Path(
            os.environ.get("SURVARIUM_BIN", VOSTOK_DIR / "binaries" / "nix-store" / "survarium-game")
        )
        exe = survarium_bin / "survarium.exe"
        pdb = survarium_bin / "survarium.pdb"
        engine = ["--engine-path", "c:/survarium/sources"]
        hint = "set SURVARIUM_BIN or run inside `nix develop` (provides survarium-game)"
    else:  # pragma: no cover - argparse restricts choices
        raise RuntimeError(f"unknown side {side!r} (expected 'base' or 'target')")

    for f in (exe, pdb):
        if not f.is_file():
            raise RuntimeError(f"{f} not found - {hint}")

    # Check the delinker is present before wiping the output directory, so a
    # missing binary can't destroy a previously-good delink.
    if shutil.which(delinker) is None:
        raise RuntimeError(
            f"{delinker!r} not found on PATH - run inside `nix develop` "
            "(provides vostok-delinker), or set VOSTOK_DELINKER"
        )

    out = OBJDIFF_DIR / side
    if out.exists():
        shutil.rmtree(out)
    out.mkdir(parents=True, exist_ok=True)

    log(f"Delinking {side} ({exe.name}) -> {out}")
    subprocess.run(
        [
            delinker,
            "--pdb-path",    str(pdb),
            "--exe-path",    str(exe),
            "--output-path", str(out),
            *engine,
        ],
        check=True,
    )

    log("Refreshing objdiff config ...")
    subprocess.run(
        [sys.executable, str(SCRIPT_DIR / "generate_objdiff_config.py")],
        check=True,
    )
    log(f"Done: {out}")


def main() -> None:
    ap = argparse.ArgumentParser(
        description="Delink base/target EXE into COFF objs via vostok-delinker."
    )
    ap.add_argument("side", choices=["base", "target"])
    try:
        generate(ap.parse_args().side)
    except (RuntimeError, subprocess.CalledProcessError) as e:
        print(f"[delink] ERROR: {e}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
