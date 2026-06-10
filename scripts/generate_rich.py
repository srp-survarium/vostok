#!/usr/bin/env python3
"""
generate_rich.py - run pdb_rich_context to (re)build the "rich" function index
(disassembly paired with source-level statements) used by `pdb_fetch`, per side:

  base    from the freshly compiled PDB+EXE
          binaries/Win32/survarium-dx11-win32-gold.{pdb,exe}
          -> binaries/rich/base   (index.jsonl + sources/ tree, --mode base)

  target  from the original game PDB+EXE
          $SURVARIUM_BIN/survarium.{pdb,exe}
          -> binaries/rich/target (--mode target)

Both sides live under binaries/rich/, mirroring binaries/objdiff/{base,target}
and binaries/structure/{base,target}. `pdb_fetch` joins base<->target by
signature name; the objdiff backend additionally reads binaries/objdiff/.

Usage:
  python3 scripts/generate_rich.py base
  python3 scripts/generate_rich.py target

Env vars (set automatically by flake.nix devShell):
  SURVARIUM_BIN - directory containing the original survarium.{pdb,exe} (target)
  PDB_RICH      - pdb_rich_context binary to invoke (default: pdb_rich_context on PATH)
"""

import argparse
import os
import subprocess
import sys

from _common import (
    VOSTOK_DIR, begin_output_dir, commit_output_dir, make_log,
    survarium_bin_dir, wine_pdb_path,
)


ENGINE_DIR = VOSTOK_DIR / "sources" / "vostok"
RICH_DIR   = VOSTOK_DIR / "binaries" / "rich"
WIN32_DIR  = VOSTOK_DIR / "binaries" / "Win32"
BASE_PDB   = WIN32_DIR / "survarium-dx11-win32-gold.pdb"
BASE_EXE   = WIN32_DIR / "survarium-dx11-win32-gold.exe"

log = make_log("rich")


def _pdb_rich() -> str:
    return os.environ.get("PDB_RICH", "pdb_rich_context")


def generate(side: str) -> None:
    """Regenerate binaries/rich/<side> from the matching PDB+EXE.

    Raises RuntimeError if an input is missing and CalledProcessError if
    pdb_rich_context fails - callers (e.g. rebuild.py) handle/report these.
    """
    out = RICH_DIR / side

    if side == "base":
        pdb, exe = BASE_PDB, BASE_EXE
        # Same prefix logic as generate_structure.py: the base PDB is MSVC-built
        # under Wine, so recorded paths are z:\...\vostok\sources\vostok\<module>.
        # Strip the Wine form of <repo>/sources (trailing sep) so file paths in the
        # index are rooted at vostok\..., matching the binaries/objdiff/*.obj tree.
        engine = wine_pdb_path(ENGINE_DIR.parent) + "\\"
        # base mode reads the real source line for each statement from here.
        extra = ["--source-root", str(ENGINE_DIR.parent)]
        if not pdb.is_file() or not exe.is_file():
            raise RuntimeError(
                f"compiled PDB/EXE not found in {WIN32_DIR} - build first "
                "(python3 scripts/rebuild.py, or scripts/ninja_build.py)"
            )
    elif side == "target":
        survarium_bin = survarium_bin_dir()
        pdb = survarium_bin / "survarium.pdb"
        exe = survarium_bin / "survarium.exe"
        engine = "c:/survarium/sources"  # pdb_rich_context normalizes to c:\...\
        extra = []  # target has no local sources; statements carry line placeholders
        if not pdb.is_file() or not exe.is_file():
            raise RuntimeError(
                f"original survarium.{{pdb,exe}} not found at {survarium_bin} - set "
                "SURVARIUM_BIN or run inside `nix develop` (provides survarium-game)"
            )
    else:  # pragma: no cover - argparse restricts choices
        raise RuntimeError(f"unknown side {side!r} (expected 'base' or 'target')")

    # Generate into <out>.tmp and swap into place on success, so a crash
    # mid-write can't leave a half-written index for the next pdb_fetch.
    tmp = begin_output_dir(out)
    log(f"Building {side} rich index from {pdb.name}+{exe.name} -> {out}")
    try:
        subprocess.run(
            [
                _pdb_rich(),
                "--pdb",         str(pdb),
                "--exe",         str(exe),
                "--engine-path", engine,
                "--mode",        side,
                "--out",         str(tmp),
                *extra,
            ],
            check=True,
        )
    except FileNotFoundError:
        raise RuntimeError(
            f"pdb_rich_context binary {_pdb_rich()!r} not found on PATH - run inside "
            "`nix develop`, or set PDB_RICH"
        )
    commit_output_dir(tmp, out)
    log(f"Done: {out}/index.jsonl")


def main() -> None:
    ap = argparse.ArgumentParser(
        description="Build the base/target rich function index via pdb_rich_context."
    )
    ap.add_argument("side", choices=["base", "target"])
    try:
        generate(ap.parse_args().side)
    except (RuntimeError, subprocess.CalledProcessError) as e:
        print(f"[rich] ERROR: {e}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
