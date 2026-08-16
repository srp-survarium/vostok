#!/usr/bin/env python3
"""
vostok.build.generate_rich - run pdb_rich_context to (re)build the "rich" function index
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
  python3 -m vostok.build.generate_rich base
  python3 -m vostok.build.generate_rich target
  python3 -m vostok.build.generate_rich all     # both sides (e.g. after a parser bump)

Env vars (set automatically by flake.nix devShell):
  SURVARIUM_BIN - directory containing the original survarium.{pdb,exe} (target)
  PDB_RICH      - pdb_rich_context binary to invoke (default: pdb_rich_context on PATH)
"""

import argparse
import os
import subprocess
import sys
from pathlib import Path

from vostok.core.paths import (BASE_EXE, BASE_PDB, RICH_DIR, WIN32_DIR,
                               survarium_bin)
from vostok.core.paths import ENGINE as ENGINE_DIR


def log(msg: str) -> None:
    print(f"[rich] {msg}", flush=True)


def _pdb_rich() -> str:
    return os.environ.get("PDB_RICH", "pdb_rich_context")


def _wine_path(p: Path) -> str:
    r"""Render a native absolute path as MSVC-under-Wine records it in a PDB:
    on the Z: drive (Wine maps ``/`` -> ``Z:``), lowercased, ``\``-separated.
    e.g. /home/u/Proj/vostok/sources -> z:\home\u\proj\vostok\sources
    """
    return "z:" + str(p).replace("/", "\\").lower()


def generate(side: str) -> None:
    """Regenerate binaries/rich/<side> from the matching PDB+EXE.

    Raises RuntimeError if an input is missing and CalledProcessError if
    pdb_rich_context fails - callers (e.g. vostok.build.rebuild) handle/report these.
    """
    out = RICH_DIR / side

    if side == "base":
        pdb, exe = BASE_PDB, BASE_EXE
        # Same prefix logic as generate_structure: the base PDB is MSVC-built
        # under Wine, so recorded paths are z:\...\vostok\sources\vostok\<module>.
        # Strip the Wine form of <repo>/sources (trailing sep) so file paths in the
        # index are rooted at vostok\..., matching the binaries/objdiff/*.obj tree.
        engine = _wine_path(ENGINE_DIR.parent) + "\\"
        # base mode reads the real source line for each statement from here.
        extra = ["--source-root", str(ENGINE_DIR.parent)]
        if not pdb.is_file() or not exe.is_file():
            raise RuntimeError(
                f"compiled PDB/EXE not found in {WIN32_DIR} - build first "
                "(python3 -m vostok build, or -m vostok.build.ninja)"
            )
    elif side == "target":
        survarium = survarium_bin()
        pdb = survarium / "survarium.pdb"
        exe = survarium / "survarium.exe"
        engine = "c:/survarium/sources"  # pdb_rich_context normalizes to c:\...\
        extra = []  # target has no local sources; statements carry line placeholders
        if not pdb.is_file() or not exe.is_file():
            raise RuntimeError(
                f"original survarium.{{pdb,exe}} not found at {survarium} - set "
                "SURVARIUM_BIN or run inside `nix develop` (provides survarium-game)"
            )
    else:  # pragma: no cover - argparse restricts choices
        raise RuntimeError(f"unknown side {side!r} (expected 'base' or 'target')")

    out.mkdir(parents=True, exist_ok=True)
    log(f"Building {side} rich index from {pdb.name}+{exe.name} -> {out}")
    try:
        subprocess.run(
            [
                _pdb_rich(),
                "--pdb",         str(pdb),
                "--exe",         str(exe),
                "--engine-path", engine,
                "--mode",        side,
                "--out",         str(out),
                *extra,
            ],
            check=True,
        )
    except FileNotFoundError:
        raise RuntimeError(
            f"pdb_rich_context binary {_pdb_rich()!r} not found on PATH - run inside "
            "`nix develop`, or set PDB_RICH"
        )
    log(f"Done: {out}/index.jsonl")


def main() -> None:
    ap = argparse.ArgumentParser(
        description="Build the base/target rich function index via pdb_rich_context."
    )
    # `all` regenerates BOTH sides - use it after bumping vostok-pdb-parser, when
    # the one-time target index needs to pick up new extraction (e.g. local scope).
    # A normal build only refreshes `base` (the target retail binary never changes
    # between recompiles, so rebuild reuses it); `all`/`target` is the manual
    # path to refresh it on a parser change.
    ap.add_argument("side", choices=["base", "target", "all"])
    try:
        side = ap.parse_args().side
        for s in (["base", "target"] if side == "all" else [side]):
            generate(s)
    except (RuntimeError, subprocess.CalledProcessError) as e:
        print(f"[rich] ERROR: {e}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
