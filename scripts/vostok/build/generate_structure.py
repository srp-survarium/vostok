#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""
vostok.build.generate_structure - run pdb-parser to (re)generate the annotated C++ "structure"
stubs used for objdiff/IDA comparison, for one side:

  base    from the freshly compiled PDB
          binaries/Win32/survarium-dx11-win32-gold.pdb
          -> binaries/structure/base   (pdb_parser --as-base --skip-non-engine-headers)

  target  from the original game PDB
          $SURVARIUM_BIN/survarium.pdb
          -> binaries/structure/target

Both sides live under binaries/structure/, mirroring the COFF layout in
binaries/objdiff/{base,target}.

Usage:
  python3 -m vostok.build.generate_structure base
  python3 -m vostok.build.generate_structure target

Env vars (set automatically by flake.nix devShell):
  SURVARIUM_BIN - directory containing the original survarium.pdb (target side)
  PDB_PARSER    - pdb-parser binary to invoke (default: pdb_parser on PATH)
"""

import argparse
import os
import subprocess
import sys

from vostok.core.paths import (BASE_PDB, RETAIL_SOURCE_PREFIX, STRUCTURE_DIR,
                               survarium_bin)


def log(msg: str) -> None:
    print(f"[structure] {msg}", flush=True)


def _pdb_parser() -> str:
    return os.environ.get("PDB_PARSER", "pdb_parser")


def generate(side: str) -> None:
    """Regenerate binaries/structure/<side> from the matching PDB.

    Raises RuntimeError if the source PDB is missing and CalledProcessError if
    pdb-parser fails - callers (e.g. vostok.build.rebuild) handle/report these.
    """
    out = STRUCTURE_DIR / side

    if side == "base":
        pdb = BASE_PDB
        # pdb-parser strips this prefix from every source path recorded in the
        # PDB. Engine includes are compiled through the retail virtual root, so
        # both PDBs reduce to the same `vostok\...` tree.
        engine = RETAIL_SOURCE_PREFIX + "\\"
        extra = ["--as-base", "--skip-non-engine-headers"]
        if not pdb.is_file():
            raise RuntimeError(
                f"compiled PDB not found: {pdb} - build first "
                "(python3 -m vostok build, or -m vostok.build.ninja)"
            )
    elif side == "target":
        survarium = survarium_bin()
        pdb = survarium / "survarium.pdb"
        engine = RETAIL_SOURCE_PREFIX
        extra = []
        if not pdb.is_file():
            raise RuntimeError(
                f"original survarium.pdb not found at {survarium} - set "
                "SURVARIUM_BIN or run inside `nix develop` (provides survarium-game)"
            )
    else:  # pragma: no cover - argparse restricts choices
        raise RuntimeError(f"unknown side {side!r} (expected 'base' or 'target')")

    out.mkdir(parents=True, exist_ok=True)
    log(f"Generating {side} structure from {pdb.name} -> {out}")
    try:
        proc = subprocess.run(
            [
                _pdb_parser(),
                "--output-path", str(out),
                "--pdb-path",     str(pdb),
                "--engine-path",  engine,
                *extra,
            ],
            stderr=subprocess.PIPE,
            text=True,
        )
    except FileNotFoundError:
        raise RuntimeError(
            f"pdb-parser binary {_pdb_parser()!r} not found on PATH - run inside "
            "`nix develop`, or set PDB_PARSER"
        )
    # pdb-parser (since the all-enums/-unions extraction bump) prints one warning per
    # enum it sees recorded with differing enumerator counts across compilands (a
    # forward-decl in one TU vs the full definition in another). It keeps the first and
    # the extracted enums are reference-only (gitignored, zero match risk), so this is
    # benign noise - collapse the flood to a single count line, pass everything else
    # through so real diagnostics still surface.
    _ENUM_DEDUP = "seen with differing enumerator counts across namespaces"
    suppressed = 0
    for line in (proc.stderr or "").splitlines():
        if _ENUM_DEDUP in line:
            suppressed += 1
        else:
            print(line, file=sys.stderr)
    if suppressed:
        log(f"({suppressed} benign 'enum differing enumerator counts' notes from "
            "pdb-parser suppressed)")
    if proc.returncode != 0:
        raise subprocess.CalledProcessError(proc.returncode, proc.args)
    log(f"Done: {out}")


def main() -> None:
    ap = argparse.ArgumentParser(
        description="Generate base/target C++ structure stubs via pdb-parser."
    )
    ap.add_argument("side", choices=["base", "target"])
    try:
        generate(ap.parse_args().side)
    except (RuntimeError, subprocess.CalledProcessError) as e:
        print(f"[structure] ERROR: {e}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
