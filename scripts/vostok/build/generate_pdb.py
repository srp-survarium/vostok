#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""
vostok.build.generate_pdb - build the canonical PDB/PE evidence database per side:

  base    from the freshly compiled PDB+EXE
          binaries/Win32/survarium-dx11-win32-gold.{pdb,exe}
          -> binaries/pdb/base/evidence.sqlite

  target  from the original game PDB+EXE
          $SURVARIUM_BIN/survarium.{pdb,exe}
          -> binaries/pdb/target/evidence.sqlite

Both sides live under binaries/pdb/, mirroring binaries/objdiff/{base,target}
and binaries/structure/{base,target}. `vostok-pdb inspect` joins the sides by
decorated identity and exposes JSON views on demand.

Usage:
  python3 -m vostok.build.generate_pdb base
  python3 -m vostok.build.generate_pdb target
  python3 -m vostok.build.generate_pdb all     # both sides (e.g. after a parser bump)

Env vars (set automatically by flake.nix devShell):
  SURVARIUM_BIN - directory containing the original survarium.{pdb,exe} (target)
  PDB_TOOL      - vostok-pdb binary to invoke (default: vostok-pdb on PATH)
"""

import argparse
import os
import subprocess
import sys

from vostok.core.paths import (
    BASE_EXE,
    BASE_PDB,
    GFX_BUILD_TREE,
    GFX_TARGET_PREFIX,
    RETAIL_SOURCE_PREFIX,
    PDB_DIR,
    SCALEFORM_SDK,
    WIN32_DIR,
    gfx_release_prefixes,
    survarium_bin,
)
from vostok.core.paths import ENGINE as ENGINE_DIR
from vostok.core.wine import pdb_path
from vostok.core.log import logger
from vostok.core import log as _log


log = logger("pdb")


def _pdb_tool() -> str:
    return os.environ.get("PDB_TOOL", "vostok-pdb")


def generate(side: str) -> None:
    """Regenerate binaries/pdb/<side> from the matching PDB+EXE.

    Raises RuntimeError if an input is missing and CalledProcessError if
    vostok-pdb fails - callers (e.g. vostok.build.rebuild) handle/report these.
    """
    out = PDB_DIR / side

    if side == "base":
        pdb, exe = BASE_PDB, BASE_EXE
        # New objects record the retail C: source root.  Keep the old worktree
        # prefix during the one-build migration and for any unstaged archive
        # member that predates source-root reproduction.
        engine = [RETAIL_SOURCE_PREFIX + "\\",
                  pdb_path(ENGINE_DIR.parent) + "\\",
                  pdb_path(GFX_BUILD_TREE) + "\\",
                  *(prefix + "\\" for prefix in gfx_release_prefixes()),
                  pdb_path(SCALEFORM_SDK) + "\\"]
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
        engine = [RETAIL_SOURCE_PREFIX,             # normalized to c:\...\
                  GFX_TARGET_PREFIX + "\\"]
        extra = []  # target has no local sources; statements carry line placeholders
        if not pdb.is_file() or not exe.is_file():
            raise RuntimeError(
                f"original survarium.{{pdb,exe}} not found at {survarium} - set "
                "SURVARIUM_BIN or run inside `nix develop` (provides survarium-game)"
            )
    else:  # pragma: no cover - argparse restricts choices
        raise RuntimeError(f"unknown side {side!r} (expected 'base' or 'target')")

    out.mkdir(parents=True, exist_ok=True)
    log(f"Building {side} PDB evidence from {pdb.name}+{exe.name} -> {out}")
    try:
        subprocess.run(
            [
                _pdb_tool(), "index", "pdb",
                "--pdb",         str(pdb),
                "--exe",         str(exe),
                "--database",    str(out / "evidence.sqlite"),
                "--side",        side,
                *[a for path in engine for a in ("--engine-path", path)],
                *extra,
            ],
            check=True,
        )
    except FileNotFoundError:
        raise RuntimeError(
            f"vostok-pdb binary {_pdb_tool()!r} not found on PATH - run inside "
            "`nix develop`, or set PDB_TOOL"
        )
    log(f"Done: {out}/evidence.sqlite")


def main() -> None:
    ap = argparse.ArgumentParser(
        description="Build the base/target PDB/PE evidence database via vostok-pdb."
    )
    # `all` regenerates BOTH sides after changing the in-repo Rust extractor;
    # the one-time target database then picks up the new evidence schema.
    # A normal build only refreshes `base` (the target retail binary never changes
    # between recompiles, so rebuild reuses it); `all`/`target` is the manual
    # path to refresh it on a parser change.
    ap.add_argument("side", choices=["base", "target", "all"])
    try:
        side = ap.parse_args().side
        for s in (["base", "target"] if side == "all" else [side]):
            generate(s)
    except (RuntimeError, subprocess.CalledProcessError) as e:
        print(f"[pdb] ERROR: {e}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    raise SystemExit(_log.run("vostok.build.generate_pdb", main))
