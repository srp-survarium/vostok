#!/usr/bin/env python3
"""structure_mismatch_queue.py - project match.db's per-function structure
classification into docs/binary_matching/structure_mismatch_queue.md, a
PERSISTENT work queue of every structurally-mismatched paired function across the
NON-RENDER engine modules (render is matched last; excluded).

Modeled on scripts/enum_diff.py + docs/binary_matching/enum_queue.md: it
re-derives the live defect set from the source of truth (match.db) on every run,
DROPS rows now handled, and PRESERVES human-authored BLOCKED status + cause across
regen. Idempotent (running twice produces the same file).

SOURCE OF TRUTH: match.db (scripts/match_db.py; design in match_db_design.md). It
classifies every paired function's `struct_class` from the two statement tables:

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

LOCALS: match.db's classifier primarily aligns on statement sizes, with an exact
normalized-PDB-line fallback for equal-count rows. It does NOT independently surface
named-local divergence. "Locals are structure" (sushi); that work shows up inside
QUANTITY/SPLIT/SIZE rows and the authoritative per-function local check stays
`pdb_fetch --view structure-diff`. There is no separate LOCALS section because the
DB carries no LOCALS class - see the doc's note.

TARGET_ONLY: unpaired real bodies (framed, >=1 statement) - genuine MISSING
structure - get their own section.

layout_diff.py CAVEAT: it OVER-reports size/field mismatches (blind to
MASTER_GOLD-guarded members + union aliases; Phase A proved 4 of 5 "resources size
mismatches" were source-parse false positives). This queue therefore reads
match.db's struct_class + (per function) `pdb_fetch --view structure-diff`, the
authoritative oracles, NOT layout_diff.

BLOCKED semantics (persistent, like enum_queue):
  * A row's struct_class flips to MATCH -> the row DROPS on the next regen.
  * A row carrying a match.db OUT_OF_SCOPE / SKIP flag is rendered BLOCKED with
    that flag's cause (regenerated from the DB each run - authoritative).
  * A human-authored BLOCKED row in the queue file (Status starting `BLOCKED`)
    PERSISTS across regen even if it would otherwise drop, carrying its cause -
    we iterate until the live set is empty.
  * The /Od frame-wall (game_core / game compile /Od, target Master Gold is /O1)
    is pre-seeded as module-level BLOCKED entries citing the memory note; those
    modules are outside the 20 in-scope ones, so they appear only in that block.

Re-derive the live set anytime:

    python3 scripts/structure_mismatch_queue.py                # human-readable summary
    python3 scripts/structure_mismatch_queue.py --write-queue  # reconcile the queue file
"""

import argparse
import re
import sqlite3
import sys
from pathlib import Path

from vostok.core.paths import MATCH_DB as DB_PATH
from vostok.core.paths import STRUCTURE_MISMATCH_QUEUE as QUEUE_FILE

# Reuse match_db's short-name renderer so rows read like its `report` output.
from match_db import fn_from_mangled

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
# Query match.db for the live structure-mismatch set
# ---------------------------------------------------------------------------

def _open_db():
    if not DB_PATH.is_file():
        sys.exit(f"[structure_mismatch_queue] no match.db at {DB_PATH} - run "
                 "rebuild.py / `match_db.py refresh` first")
    con = sqlite3.connect(f"file:{DB_PATH}?mode=ro", uri=True)
    con.row_factory = sqlite3.Row
    return con


def _scope_placeholders():
    return ",".join("?" * len(IN_SCOPE_MODULES))


def _short_loc(file_path):
    """Strip the leading 'vostok/' so file paths read short, like match_db."""
    if file_path and file_path.startswith("vostok/"):
        return file_path[len("vostok/"):]
    return file_path or "(no file)"


def query_mismatches(con, classes):
    """Paired functions in the in-scope modules whose struct_class is in `classes`.
    Joins the flags table so an OUT_OF_SCOPE / SKIP verdict (with its cause) rides
    along as the BLOCKED signal. One row per function (lowest target rva wins; the
    DB already keys pairs by mangled)."""
    ph = _scope_placeholders()
    cls_ph = ",".join("?" * len(classes))
    q = f"""
      SELECT s.mangled                                       AS mangled,
             s.demangled                                     AS demangled,
             t.module                                        AS module,
             fl.path                                         AS file,
             t.line                                          AS line,
             p.struct_class                                  AS struct_class,
             p.fuzzy_pct                                     AS fuzzy_pct,
             p.t_stmts                                       AS t_stmts,
             p.b_stmts                                       AS b_stmts,
             (SELECT group_concat(flag, '+') FROM flags
              WHERE mangled = s.mangled
                AND flag IN ('OUT_OF_SCOPE', 'SKIP'))        AS flag,
             (SELECT cause FROM flags
              WHERE mangled = s.mangled
                AND flag IN ('OUT_OF_SCOPE', 'SKIP')
              ORDER BY flag LIMIT 1)                         AS flag_cause
      FROM pairs p
      JOIN symbols s ON s.id = p.sym
      JOIN target_functions t ON t.rva = p.target_rva
      LEFT JOIN files fl ON fl.id = t.file
      WHERE t.module IN ({ph}) AND p.struct_class IN ({cls_ph})
      ORDER BY t.module, fl.path, t.line, s.mangled
    """
    return [dict(r) for r in con.execute(q, (*IN_SCOPE_MODULES, *classes))]


def query_target_only(con):
    """Unpaired REAL bodies (framed prologue, >=1 statement) in the in-scope
    modules - genuine missing structure. Frameless unpaired symbols are
    custom-conv leaves that only pair inlined into callers (out of scope as a
    standalone match), so they are excluded."""
    ph = _scope_placeholders()
    q = f"""
      SELECT s.mangled         AS mangled,
             s.demangled       AS demangled,
             t.module          AS module,
             fl.path           AS file,
             t.line            AS line,
             t.size            AS size,
             t.n_stmts         AS n_stmts
      FROM target_functions t
      JOIN symbols s ON s.id = t.sym
      LEFT JOIN files fl ON fl.id = t.file
      LEFT JOIN pairs p ON p.sym = t.sym
      WHERE p.sym IS NULL AND t.frameless = 0 AND t.n_stmts > 0
        AND t.module IN ({ph})
      ORDER BY t.module, fl.path, t.line, s.mangled
    """
    return [dict(r) for r in con.execute(q, IN_SCOPE_MODULES)]


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
    """Resolve a row's Status + cause, honouring (1) a live match.db OUT_OF_SCOPE /
    SKIP flag, then (2) a human-authored BLOCKED row preserved from the file."""
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

Generated/reconciled by `python3 scripts/structure_mismatch_queue.py --write-queue`.

A PERSISTENT, idempotent queue of every STRUCTURALLY MISMATCHED paired function
across the 20 NON-RENDER engine modules (render is matched last - EXCLUDED):
`ai_navigation, ai, animation, collision, core, debug, engine, fs, input,
logging, network_core, network, particle, physics, scaleform, sound, survarium,
ui, vfs, vostok`.

Source of truth: `docs/binary_matching/match.db` (scripts/match_db.py,
match_db_design.md), which classifies each paired function's `struct_class` from
the two statement tables. This queue PROJECTS that classification - the
authoritative per-function verdict stays `pdb_fetch --view structure-diff`.

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

LOCALS note: "locals are structure" (sushi), but match.db's classifier aligns on
statement sizes/PDB line geometry and does not independently surface named-local
divergence, so there is no standalone LOCALS section - that work rides inside the
QUANTITY / SPLIT / SIZE rows and is checked per function with
`pdb_fetch --view structure-diff`.

layout_diff.py caveat: it OVER-reports size/field mismatches (blind to
MASTER_GOLD-guarded members + union aliases - Phase A proved 4 of 5 "resources
size mismatches" were source-parse false positives). This queue therefore trusts
match.db's struct_class + `pdb_fetch --view structure-diff`, not layout_diff.

## Persistence / BLOCKED semantics (like enum_queue)

* A row DROPS when its `struct_class` becomes `MATCH` (handled) - re-run
  `--write-queue` and it falls out.
* A row carrying a match.db `OUT_OF_SCOPE` / `SKIP` flag is rendered
  `BLOCKED:<flag>` with that flag's cause (regenerated from the DB each run).
* A human-authored `BLOCKED:...` row in this file PERSISTS across regen even if it
  would otherwise drop (keyed by the hidden `<!-- m:MANGLED -->` marker), carrying
  its cause. Add a real block by setting Status to `BLOCKED:<cause>`.
* Idempotent: running `--write-queue` twice produces the same file.

The `<!-- m:MANGLED -->` trailing cell is the stable key for preservation - leave
it on the row when editing a Status.

Re-derive the live set anytime:

    python3 scripts/structure_mismatch_queue.py                # human-readable summary
    python3 scripts/structure_mismatch_queue.py --write-queue  # reconcile this file
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
    con = _open_db()
    primary = query_mismatches(con, PRIMARY_CLASSES)
    size = query_mismatches(con, (SIZE_CLASS,))
    target_only = query_target_only(con)
    con.close()

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

def _summary(con):
    primary = query_mismatches(con, PRIMARY_CLASSES)
    size = query_mismatches(con, (SIZE_CLASS,))
    target_only = query_target_only(con)
    print("structure-mismatch queue - match.db projection (non-render modules)")
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

    con = _open_db()
    _summary(con)
    con.close()
    return 0


if __name__ == "__main__":
    sys.exit(main())
