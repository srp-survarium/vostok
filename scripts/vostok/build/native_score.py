#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""vostok.build.native_score - fast scoring preview of a native Windows build.

Runs the base-side steps of `vostok build` (PDB evidence, code COFF + objdiff
report, structure) on the exe scripts/windows/build.ps1 produced, re-derives the
ledger exactly as `vostok build` does, and prints how it differs from the
committed config/match_state.tsv. Nothing committed is written: the working
ledger is restored afterwards and the preview is kept in binaries/windows/preview.

A PREVIEW, not a measurement: the native linker folds identical COMDATs a little
differently from the Wine link the ledger is measured with (about 2% of functions
score differently between the two, both deterministic), so commits stay measured
by `python3 -m vostok build`. Use this to see quickly what an edit does.

The target side (the original game's COFF, structure and PDB evidence) is
generated once from SURVARIUM_BIN, as `vostok tool toolchain` does.

  python -m vostok.build.native_score [--top N]   (scripts/windows/score.ps1 sets the tools up)
"""

import argparse
import os
import shutil
import sys
import time

from vostok.core import paths
from vostok.core.log import logger

log = logger("native-score")
PREVIEW_DIR = paths.BINARIES / "windows" / "preview"


def _rows(path):
    from vostok.ledger import store
    return store.load(str(path))


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--top", type=int, default=25, help="changed functions to list")
    args = ap.parse_args()
    if os.name != "nt":
        sys.exit("[native-score] Windows only - on Linux run python3 -m vostok build")
    if not paths.BASE_EXE.is_file():
        sys.exit(f"[native-score] no native build at {paths.BASE_EXE} - run scripts/windows/build.ps1")

    from vostok.build import generate_delink, generate_pdb, generate_structure
    from vostok.derive import roster
    from vostok.tool.toolchain import ensure_target_side

    t0 = time.monotonic()
    ensure_target_side()
    for name, step in (("base PDB evidence", lambda: generate_pdb.generate("base")),
                       ("base code COFF + report", lambda: generate_delink.generate("base")),
                       ("base structure", lambda: generate_structure.generate("base"))):
        t = time.monotonic()
        step()
        log(f"{name}: OK ({time.monotonic() - t:.0f}s)")

    # derive into the working ledger, keep a copy, and put the committed state back
    ledger = paths.MATCH_STATE
    original = ledger.read_bytes()
    PREVIEW_DIR.mkdir(parents=True, exist_ok=True)
    preview, previous = PREVIEW_DIR / "match_state.tsv", PREVIEW_DIR / "match_state.previous.tsv"
    if preview.is_file():
        shutil.copyfile(preview, previous)
    try:
        roster.regen()
        shutil.copyfile(ledger, preview)
    finally:
        ledger.write_bytes(original)

    after = _rows(preview)
    committed = _rows(ledger)

    def exact(rows):
        return sum(1 for r in rows.values() if (r["cur"] or 0) >= 100)

    log(f"preview ledger: {preview}")
    log(f"functions exact (cur): committed (Wine) {exact(committed):,}, native preview {exact(after):,}")
    # native builds are deterministic, so preview-to-preview isolates the edit; the first
    # preview can only be compared with the Wine-measured ledger (~2% folding noise)
    if previous.is_file():
        before, against = _rows(previous), "the previous native preview"
    else:
        before, against = committed, "the committed (Wine) ledger - includes folding noise"
    changed = []
    for mangled, row in after.items():
        old = before.get(mangled)
        cur_old = old["cur"] if old else None
        if row["cur"] != cur_old:
            changed.append((mangled, cur_old, row["cur"], committed.get(mangled, {}).get("max")))
    changed.sort(key=lambda c: abs((c[2] or 0) - (c[1] or 0)), reverse=True)
    log(f"{len(changed):,} functions changed against {against}")
    for mangled, old, new, mx in changed[:args.top]:
        fmt = lambda v: "-" if v is None else f"{v:.2f}"  # noqa: E731
        log(f"  {fmt(old):>6} -> {fmt(new):>6}  (committed max {fmt(mx)})  {mangled[:110]}")
    log(f"done in {time.monotonic() - t0:.0f}s")


if __name__ == "__main__":
    from vostok.core import log as _log
    raise SystemExit(_log.run("vostok.build.native_score", main))
