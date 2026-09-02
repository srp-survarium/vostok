#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""vostok.ledger.queue - project the derivation's per-function structure
classification into docs/binary_matching/structure_mismatch_queue.md, a
PERSISTENT work queue of every structurally-mismatched paired function across the
NON-RENDER engine modules (render is matched last; excluded).

Modeled on vostok.diff.enums + docs/binary_matching/enum_queue.md: it
re-derives the live defect set from the artifacts on every run, DROPS rows now
handled, and PRESERVES human-authored BLOCKED status + cause across regen.
Idempotent (running twice produces the same file).

SOURCE OF TRUTH: `vostok.derive` itself, run live (~40 s over report.json and
the two rich indexes) - there is no database to query, so this projection cannot
drift from a stale one. It classifies every paired function's `struct_class`
from the two statement tables:

  QUANTITY  statement COUNTS differ - real missing/extra source statements
  SPLIT     equal counts + total bytes, alignment left unpairable rows
            (line-attribution split)
  SIZE      same count, >=1 per-statement byte mismatch (historically the
            LTCG/opt-level residual on otherwise-faithful structure)
  MATCH     structurally identical (dropped from the queue)

QUANTITY + SPLIT are the genuine STEERABLE structural work and lead the queue.
SIZE is split into a SEPARATE section: per sushi's hypothesis it is the
re-test-after-core-fix backlog (wrong core sizes/layouts/types change inline +
codegen decisions downstream; see the [ltcg-walls-may-be-core-structure-
downstream] memory), so it is tracked but NOT mixed with QUANTITY.

LOCALS: the classifier primarily aligns on statement sizes, with an exact
normalized-PDB-line fallback for equal-count rows. It does NOT independently surface
named-local divergence. "Locals are structure" (sushi); that work shows up inside
QUANTITY/SPLIT/SIZE rows and the authoritative per-function local check stays
`pdb_fetch --view structure-diff`. There is no separate LOCALS section because the
derivation carries no LOCALS class - see the doc's note.

TARGET_ONLY: unpaired real bodies (framed, >=1 statement) - genuine MISSING
structure - get their own section.

vostok diff layout CAVEAT: it OVER-reports size/field mismatches (blind to
MASTER_GOLD-guarded members + union aliases; Phase A proved 4 of 5 "resources size
mismatches" were source-parse false positives). This queue therefore reads
`struct_class` + (per function) `pdb_fetch --view structure-diff`, the
authoritative oracles, NOT layout_diff.

BLOCKED semantics (persistent, like enum_queue):
  * A row's struct_class flips to MATCH -> the row DROPS on the next regen.
  * A row the ledger says is `parked` is rendered `BLOCKED:parked` with the
    ledger's note as its cause (re-read each run - authoritative).
  * A human-authored BLOCKED row in the queue file (Status starting `BLOCKED`)
    PERSISTS across regen even if it would otherwise drop, carrying its cause -
    we iterate until the live set is empty.
  * The /Od frame-wall (game_core / game compile /Od, target Master Gold is /O1)
    is pre-seeded as module-level BLOCKED entries citing the memory note; those
    modules are outside the 20 in-scope ones, so they appear only in that block.

Re-derive the live set anytime:

    python3 -m vostok ledger mismatch-queue                # human-readable summary
    python3 -m vostok ledger mismatch-queue --write-queue  # reconcile the queue file
"""

import argparse
import re
import sys
from pathlib import Path

from vostok.core.paths import STRUCTURE_MISMATCH_QUEUE as QUEUE_FILE
# Reuse the derive layer's short-name renderer so rows read like its reports.
from vostok.derive.names import fn_from_mangled
from vostok.derive.roster import derive
from vostok.ledger import store

# The 20 in-scope NON-RENDER engine modules (render matched last; game / game_core
# are tracked only as the pre-seeded /Od BLOCKED block below).
IN_SCOPE_MODULES = (
    "ai_navigation", "ai", "animation", "collision", "core", "debug", "engine",
    "fs", "input", "logging", "network_core", "network", "particle", "physics",
    "scaleform", "sound", "survarium", "ui", "vfs", "vostok",
)

PRIMARY_CLASSES = ("QUANTITY", "SPLIT")  # genuine steerable structural work
SIZE_CLASS = "SIZE"                      # re-test-after-core-fix backlog

# Pre-seeded /Od frame-wall BLOCKED block (game_core + game). See memory:
# game-core-compiled-Od-target-is-O1-frame-wall - both compile /Od while the
# shipped Master Gold module is /O1, emitting a framed prologue/epilogue where the
# target is frameless. That caps MANY of their functions module-wide regardless of
# source faithfulness; it is a BUILD-CONFIG mismatch, not steerable per function.
OD_FRAME_WALL = [
    (
        "game_core",
        "BLOCKED:/Od-frame-wall",
        "game_core .vcproj Optimization=Disabled(/Od); target Master Gold is "
        "/O1. Framed prologue/epilogue + framed locals cap many fns module-wide "
        "regardless of source faithfulness (e.g. tick_active_object is "
        "structure-MATCH yet caps ~41%). Build-config mismatch, not per-fn "
        "steerable - resolved by an opt-level flip experiment, not source. "
        "See memory game-core-compiled-Od-target-is-O1-frame-wall.",
    ),
    (
        "game",
        "BLOCKED:/Od-frame-wall",
        "game .vcproj is also /Od where the shipped module is /O1 "
        "(object_*::load, player/player_input::deserialize all blamed "
        "/Od-vs-/Ox). Same build-config frame wall as game_core; module-wide, "
        "not per-fn steerable. See memory "
        "game-core-compiled-Od-target-is-O1-frame-wall.",
    ),
]

# ---------------------------------------------------------------------------
# Re-derive the live structure-mismatch set
# ---------------------------------------------------------------------------

def live_set(declarations=False):
    """(primary, size, target_only) rows, re-derived from the artifacts.

    There is no cache to query: the structure classification comes from the
    derivation itself (report.json + the two rich indexes, ~40 s) and the park
    state from the committed ledger. That is the point - a projection cannot
    drift from a source of truth it recomputes.
    """
    roster = derive(declarations=declarations)
    parked = _parked(store.load())
    return (
        _mismatches(roster, parked, PRIMARY_CLASSES),
        _mismatches(roster, parked, (SIZE_CLASS,)),
        _target_only(roster, parked),
    )


def _parked(ledger):
    """{mangled: cause} for every function the ledger says is parked.

    The ledger has one park state where the cache had two flags (OUT_OF_SCOPE
    and SKIP), so rows render `BLOCKED:parked` rather than naming a flag; the
    cause - the ledger's `note` - is what actually says why.
    """
    return {
        mangled: row.get("note") or ""
        for mangled, row in ledger.items()
        if row.get("status") == "parked"
    }


def _order(row):
    """module / file / line / mangled, with line-less rows first (as SQL sorted
    NULLs) so a header's inline bodies stay grouped."""
    return (row["module"], row["file"] or "", row["line"] or -1, row["mangled"])


def _short_loc(file_path):
    """Strip the leading 'vostok/' so file paths read short, like the reports."""
    if file_path and file_path.startswith("vostok/"):
        return file_path[len("vostok/"):]
    return file_path or "(no file)"


def _mismatches(roster, parked, classes):
    """Paired functions in the in-scope modules whose struct_class is in
    `classes`, carrying the ledger's park verdict as the BLOCKED signal."""
    rows = []
    for mangled, function in roster.target.items():
        pair = roster.pairing.pairs.get(mangled)
        if pair is None or pair.cls not in classes:
            continue
        if function.module not in IN_SCOPE_MODULES:
            continue
        rows.append({
            "mangled": mangled, "demangled": function.demangled,
            "module": function.module, "file": function.file, "line": function.line,
            "struct_class": pair.cls, "fuzzy_pct": pair.fuzzy,
            "t_stmts": pair.t_stmts, "b_stmts": pair.b_stmts,
            "flag": "parked" if mangled in parked else None,
            "flag_cause": parked.get(mangled),
        })
    rows.sort(key=_order)
    return rows


def _target_only(roster, parked):
    """Unpaired REAL bodies (framed prologue, >=1 statement) in the in-scope
    modules - genuine missing structure. Frameless unpaired symbols are
    custom-conv leaves that only pair inlined into callers (out of scope as a
    standalone match), so they are excluded."""
    rows = []
    for mangled, function in roster.target.items():
        if mangled in roster.pairing.pairs or function.frameless or not function.n_stmts:
            continue
        if function.module not in IN_SCOPE_MODULES:
            continue
        rows.append({
            "mangled": mangled, "demangled": function.demangled,
            "module": function.module, "file": function.file, "line": function.line,
            "size": function.size, "n_stmts": function.n_stmts,
            "flag": "parked" if mangled in parked else None,
            "flag_cause": parked.get(mangled),
        })
    rows.sort(key=_order)
    return rows


# ---------------------------------------------------------------------------
# Existing-queue parsing (preserve human-authored BLOCKED rows)
# ---------------------------------------------------------------------------

# Row shape: | module | fn | file:line | class | % | cause |   (mangled in HTML comment)
_ROW_RE = re.compile(r"^\|\s*`?[^|]*`?\s*\|")
_MANGLED_RE = re.compile(r"<!--\s*m:(\S+)\s*-->")


def read_existing_blocked(path):
    """{key: {status, cause}} for rows the human marked BLOCKED, so a reconcile
    preserves them. key = the hidden `<!-- m:MANGLED -->` marker when present,
    else the visible function cell (module-level pre-seeds carry no mangled)."""
    blocked = {}
    if not path.is_file():
        return blocked
    for line in path.read_text().splitlines():
        if not _ROW_RE.match(line):
            continue
        cells = [c.strip() for c in line.strip().strip("|").split("|")]
        if len(cells) < 6:
            continue
        module, fn_cell, _loc, status, _pct, cause = cells[:6]
        if not status.upper().startswith("BLOCKED"):
            continue
        m = _MANGLED_RE.search(line)
        key = m.group(1) if m else f"{module}|{fn_cell}"
        blocked[key] = {"status": status, "cause": cause}
    return blocked


# ---------------------------------------------------------------------------
# Rendering
# ---------------------------------------------------------------------------

def _fmt_pct(v):
    return f"{v:.2f}%" if v is not None else "-"


def _esc(text):
    return (text or "").replace("|", r"\|").replace("\n", " ")


def _fn_cell(row):
    return _esc(fn_from_mangled(row["mangled"], row.get("demangled") or ""))


def _loc_cell(row):
    line = row.get("line")
    return f"{_short_loc(row.get('file'))}:{line}" if line else _short_loc(row.get("file"))


def _status_and_cause(row, existing_blocked):
    """Resolve a row's Status + cause, honouring (1) the ledger's live park
    verdict, then (2) a human-authored BLOCKED row preserved from the file."""
    flag = row.get("flag")
    if flag:
        return f"BLOCKED:{flag}", _esc(row.get("flag_cause") or "")
    prev = existing_blocked.get(row["mangled"])
    if prev:
        return prev["status"], prev["cause"]
    return row["struct_class"], ""


def _mismatch_rows(rows, existing_blocked):
    out = []
    for r in rows:
        status, cause = _status_and_cause(r, existing_blocked)
        qd = f"{r.get('t_stmts')}->{r.get('b_stmts')}" if r["struct_class"] == "QUANTITY" else ""
        detail = "; ".join(x for x in (qd, cause) if x)
        out.append(
            f"| {r['module']} | {_fn_cell(r)} | {_loc_cell(r)} | {status} "
            f"| {_fmt_pct(r['fuzzy_pct'])} | {detail} | <!-- m:{r['mangled']} -->"
        )
    return out


def _target_only_rows(rows, existing_blocked):
    out = []
    for r in rows:
        prev = existing_blocked.get(r["mangled"])
        status = prev["status"] if prev else "TARGET_ONLY"
        cause = prev["cause"] if prev else f"{r['n_stmts']} stmts, size {r['size']}"
        out.append(
            f"| {r['module']} | {_fn_cell(r)} | {_loc_cell(r)} | {status} "
            f"| - | {_esc(cause)} | <!-- m:{r['mangled']} -->"
        )
    return out


def _table(rows):
    head = ["| Module | Function | File:line | Status | Current % | Cause / detail |",
            "|---|---|---|---|---|---|"]
    return head + (rows if rows else ["| _(none)_ |||||  |"])


_HEADER = """# Structure-mismatch work queue (target vs base)

Generated/reconciled by `python3 -m vostok ledger mismatch-queue --write-queue`.

A PERSISTENT, idempotent queue of every STRUCTURALLY MISMATCHED paired function
across the 20 NON-RENDER engine modules (render is matched last - EXCLUDED):
`ai_navigation, ai, animation, collision, core, debug, engine, fs, input,
logging, network_core, network, particle, physics, scaleform, sound, survarium,
ui, vfs, vostok`.

Source of truth: the derivation itself (`scripts/vostok/derive/`,
ledger_design.md), re-run live over report.json and the rich indexes, which
classifies each paired function's `struct_class` from the two statement tables.
This queue PROJECTS that classification - the authoritative per-function verdict
stays `pdb_fetch --view structure-diff`.

## Sections

1. **QUANTITY + SPLIT (primary, steerable):** statement-COUNT mismatch (real
   missing/extra source statements) and equal-count-but-line-split. This is the
   genuine structural work to drive to zero.
2. **SIZE (re-test after core fixes):** same statement count, >=1 per-statement
   byte mismatch - historically the LTCG/opt-level residual on otherwise-faithful
   structure. Tracked SEPARATELY: per sushi's hypothesis these may dissolve once
   the flagged CORE structure is fixed (wrong core sizes/layouts/types change
   inline + codegen downstream; memory `ltcg-walls-may-be-core-structure-
   downstream`). Re-test this backlog after each core fix.
3. **TARGET_ONLY (missing structure):** unpaired REAL bodies (framed, >=1 stmt) -
   structure that exists in the target but not yet in base. Frameless unpaired
   leaves (custom-conv, pair only inlined into callers) are excluded.

LOCALS note: "locals are structure" (sushi), but the classifier aligns on
statement sizes/PDB line geometry and does not independently surface named-local
divergence, so there is no standalone LOCALS section - that work rides inside the
QUANTITY / SPLIT / SIZE rows and is checked per function with
`pdb_fetch --view structure-diff`.

vostok diff layout caveat: it OVER-reports size/field mismatches (blind to
MASTER_GOLD-guarded members + union aliases - Phase A proved 4 of 5 "resources
size mismatches" were source-parse false positives). This queue therefore trusts
the derived struct_class + `pdb_fetch --view structure-diff`, not layout_diff.

## Persistence / BLOCKED semantics (like enum_queue)

* A row DROPS when its `struct_class` becomes `MATCH` (handled) - re-run
  `--write-queue` and it falls out.
* A row the committed ledger marks `parked` is rendered `BLOCKED:parked` with
  the ledger's note as its cause (re-read each run).
* A human-authored `BLOCKED:...` row in this file PERSISTS across regen even if it
  would otherwise drop (keyed by the hidden `<!-- m:MANGLED -->` marker), carrying
  its cause. Add a real block by setting Status to `BLOCKED:<cause>`.
* Idempotent: running `--write-queue` twice produces the same file.

The `<!-- m:MANGLED -->` trailing cell is the stable key for preservation - leave
it on the row when editing a Status.

Re-derive the live set anytime:

    python3 -m vostok ledger mismatch-queue                # human-readable summary
    python3 -m vostok ledger mismatch-queue --write-queue  # reconcile this file
"""


def _od_block_rows(existing_blocked):
    """Pre-seeded /Od frame-wall BLOCKED entries (module-level; game_core + game).
    Preserves a human-edited cause if one is already in the file."""
    rows = []
    for module, status, cause in OD_FRAME_WALL:
        prev = existing_blocked.get(f"{module}|_(whole module)_")
        if prev:
            status, cause = prev["status"], prev["cause"]
        rows.append(
            f"| {module} | _(whole module)_ | {module}/*.vcproj | {status} "
            f"| - | {_esc(cause)} |"
        )
    return rows


def write_queue(path):
    existing_blocked = read_existing_blocked(path)
    primary, size, target_only = live_set()

    # carry forward any human BLOCKED row whose function is no longer in the live
    # set (its struct_class flipped, but the human pinned it) - keep it visible.
    live_keys = {r["mangled"] for r in primary + size + target_only}
    orphan_blocked = [
        f"| {k.split('|', 1)[0] if '|' in k else '?'} "
        f"| {k.split('|', 1)[1] if '|' in k else k} | (pinned) | {v['status']} "
        f"| - | {_esc(v['cause'])} | <!-- m:{k} -->"
        for k, v in sorted(existing_blocked.items())
        if k not in live_keys and "|" not in k
        and not k.startswith(("game_core|", "game|"))
    ]

    primary_rows = _mismatch_rows(primary, existing_blocked)
    size_rows = _mismatch_rows(size, existing_blocked)
    to_rows = _target_only_rows(target_only, existing_blocked)
    od_rows = _od_block_rows(existing_blocked)

    n_blocked = sum(1 for r in primary_rows + size_rows if "| BLOCKED" in r)

    out = [_HEADER]
    out.append(
        f"\n_Live rows: {len(primary)} QUANTITY+SPLIT (primary), {len(size)} SIZE "
        f"(re-test backlog), {len(target_only)} TARGET_ONLY (missing). "
        f"{n_blocked} flagged BLOCKED._\n"
    )

    out.append("## QUANTITY + SPLIT (primary, steerable structural work)\n")
    out.extend(_table(primary_rows))
    if orphan_blocked:
        out.append("\n### Pinned BLOCKED (struct_class now clean, human-held)\n")
        out.extend(_table(orphan_blocked))

    out.append("\n## SIZE (re-test after core fixes - LTCG/opt-level residual)\n")
    out.extend(_table(size_rows))

    out.append("\n## TARGET_ONLY (missing structure - unpaired real bodies)\n")
    out.extend(_table(to_rows))

    out.append("\n## Pre-seeded BLOCKED: /Od frame-wall (game_core / game)\n")
    out.append(
        "These modules are OUTSIDE the 20 in-scope ones, pre-seeded here because "
        "the frame wall is a known module-wide structural ceiling. Both compile "
        "`/Od` while the shipped Master Gold module is `/O1`, emitting a framed "
        "prologue/epilogue + framed locals where the target is frameless - capping "
        "many of their functions regardless of source faithfulness. Build-config "
        "mismatch, resolved by an opt-level-flip experiment, not per-function "
        "source steering (memory `game-core-compiled-Od-target-is-O1-frame-wall`).\n"
    )
    out.extend(_table(od_rows))

    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text("\n".join(out) + "\n")
    return len(primary), len(size), len(target_only), n_blocked


# ---------------------------------------------------------------------------

def _summary():
    primary, size, target_only = live_set()
    print("structure-mismatch queue - derived live (non-render modules)")
    print(f"  QUANTITY+SPLIT (primary): {len(primary)}")
    print(f"  SIZE (re-test backlog):   {len(size)}")
    print(f"  TARGET_ONLY (missing):    {len(target_only)}")
    by_mod = {}
    for r in primary:
        if r["struct_class"] == "QUANTITY":
            by_mod[r["module"]] = by_mod.get(r["module"], 0) + 1
    print("\n  biggest QUANTITY clusters per module:")
    for mod, n in sorted(by_mod.items(), key=lambda kv: -kv[1])[:10]:
        print(f"    {mod:<14} {n}")


def main():
    ap = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter
    )
    ap.add_argument("--write-queue", action="store_true",
                    help="reconcile docs/binary_matching/structure_mismatch_queue.md")
    ap.add_argument("--queue-file", type=Path, default=QUEUE_FILE)
    args = ap.parse_args()

    if args.write_queue:
        n_p, n_s, n_t, n_b = write_queue(args.queue_file)
        print(f"wrote {args.queue_file} ({n_p} primary, {n_s} SIZE, {n_t} "
              f"TARGET_ONLY; {n_b} BLOCKED)")
        return 0

    _summary()
    return 0


if __name__ == "__main__":
    sys.exit(main())
