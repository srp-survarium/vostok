"""vostok.sema.sweep - bulk classification over a module or TU.

For every paired, non-100% function in scope: is the residual a FLOW divergence
or not? That is the question `report.json` cannot answer, and the one that
decides whether a function is steerable structure work or a regalloc/operand
residual. BRANCH-COUNT is the highest-yield verdict; FLOW-SAME is not source
shape work at all.
"""

from __future__ import annotations

import sqlite3

from vostok.core.paths import MATCH_DB as DB_PATH

from vostok.sema import die
from vostok.sema.cfg import branch_rows, cfg, iso_map
from vostok.sema.disasm import disasm, parse
from vostok.sema.index import _load_side


def cmd_sweep(args):
    """Bulk classification: for every paired, non-100% function in scope, is the
    residual a FLOW divergence or not? That is the question `report.json` cannot
    answer and the one that decides whether a function is steerable structure
    work or a regalloc/operand residual."""
    if not DB_PATH.is_file():
        die(f"{DB_PATH} missing")
    con = sqlite3.connect(f"file:{DB_PATH}?mode=ro", uri=True)
    q = ("SELECT mangled, demangled, unit, fuzzy_pct, struct_class FROM paired "
         "WHERE fuzzy_pct < 100")
    p = []
    if args.module:
        q += " AND module = ?"
        p.append(args.module)
    if args.unit:
        q += " AND unit LIKE ?"
        p.append(f"%{args.unit}%")
    if args.min_pct is not None:
        q += " AND fuzzy_pct >= ?"
        p.append(args.min_pct)
    q += " ORDER BY fuzzy_pct DESC"
    rows = con.execute(q, p).fetchall()
    con.close()
    if args.max:
        rows = rows[:args.max]
    tgt_all, base_all = _load_side("target"), _load_side("base")
    print(f"{'fuzzy':>7} {'blocks b/t':>11} {'branches b/t':>13}  {'verdict':<14} function")
    tally = {}
    for mangled, dem, unit, pct, sclass in rows:
        t, b = tgt_all.get(mangled), base_all.get(mangled)
        if not t or not b:
            continue
        try:
            tg = cfg(*parse(disasm("target", t["rva"]))[:2])
            bg = cfg(*parse(disasm("base", b["rva"]))[:2])
        except SystemExit:
            continue
        tb, bb = branch_rows(tg), branch_rows(bg)
        flow_same = [x[2] for x in bg] == [x[2] for x in tg]
        if flow_same:
            bodies = ["\n".join(x[1]) for x in bg] == ["\n".join(x[1]) for x in tg]
            verdict = "IDENTICAL" if bodies else "FLOW-SAME"
        elif iso_map(bg, tg) is not None:
            verdict = "ORDER-ONLY"
        elif len(bb) == len(tb) and len(bg) != len(tg):
            # equal branch count, unequal block count: one side has an extra
            # block no contraction could remove. Positional branch pairing is
            # meaningless here, so do NOT call the resulting mnemonic mismatches
            # COND-FLIP - that label sent batch B7 after two phantom bugs.
            verdict = "BLOCK-COUNT"
        elif len(bb) != len(tb):
            verdict = "BRANCH-COUNT"
        elif [x[2] for x in bb] != [x[2] for x in tb]:
            verdict = "COND-FLIP"
        elif [x[3] for x in bb] != [x[3] for x in tb]:
            verdict = "TOPOLOGY"
        else:
            verdict = "BLOCK-SPLIT"
        tally[verdict] = tally.get(verdict, 0) + 1
        print(f"{pct:7.2f} {len(bg):>5}/{len(tg):<5} {len(bb):>6}/{len(tb):<6}  "
              f"{verdict:<14} {dem[:96]}")
    print("\n[verdicts] " + "  ".join(f"{k}={v}" for k, v in sorted(tally.items())))
    print("[FLOW-SAME/IDENTICAL = not control flow: operands, regalloc, scheduling.]")
    print("[ORDER-ONLY = isomorphic CFG, different block LAYOUT - one merged exit placed "
          "elsewhere; usually downstream, not a per-branch bug.]")
    print("[BRANCH-COUNT first (measured: 5 real source bugs in 9 opened), then "
          "BLOCK-COUNT; TOPOLOGY was 0 for 6 - see docs/binary_matching/sema_tools.md.]")
    return 0
