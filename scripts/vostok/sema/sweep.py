# SPDX-License-Identifier: GPL-3.0-or-later
"""vostok.sema.sweep - bulk classification over a module or TU.

For every paired, non-100% function in scope: is the residual a FLOW divergence
or not? That is the question `report.json` cannot answer, and the one that
decides whether a function is steerable structure work or a regalloc/operand
residual. BRANCH-COUNT is the highest-yield verdict; FLOW-SAME is not source
shape work at all.
"""

from __future__ import annotations

from vostok.sema.cfg import branch_rows, cfg, iso_map
from vostok.sema.disasm import disasm, parse
from vostok.sema.pairing import ledger, pairing


def _candidates(args):
    """(rows, unscored) for the scope: `rows` is [(demangled, %, t_rva, b_rva)].

    Module and TU ownership and the percentage come from the committed ledger;
    the two RVAs come from the index-derived pairing. A ledger row with no pair
    is target-only (or banked from a build that no longer emits it) and is not a
    candidate - `sweep` has nothing to disassemble on the base side."""
    pairs = pairing().pairs
    unit = args.unit.lower() if args.unit else None
    rows, unscored = [], 0
    for key, row in ledger().items():
        if args.module and row["module"] != args.module:
            continue
        if unit and unit not in (row["unit"] or "").lower():
            continue
        pair = pairs.get(key)
        if pair is None:
            continue
        pct = row["cur"]
        # Rows objdiff paired but could not score are NOT in the candidate set.
        # They used to vanish without a word; the footer counts them.
        if pct is None:
            unscored += 1
        elif pct < 100 and (args.min_pct is None or pct >= args.min_pct):
            rows.append((pairing().demangled(key), pct, *pair))
    # worst last, and ties broken by name so two runs agree (the SQL ORDER BY
    # left ties to the query plan).
    rows.sort(key=lambda r: (-r[1], r[0]))
    return rows, unscored


def cmd_sweep(args):
    """Bulk classification: for every paired, non-100% function in scope, is the
    residual a FLOW divergence or not? That is the question `report.json` cannot
    answer and the one that decides whether a function is steerable structure
    work or a regalloc/operand residual."""
    rows, unscored = _candidates(args)
    if args.max:
        rows = rows[:args.max]
    print(f"{'fuzzy':>7} {'blocks b/t':>11} {'branches b/t':>13}  {'verdict':<14} function")
    tally = {}
    undisassembled = 0
    for dem, pct, t_rva, b_rva in rows:
        ts, bs = {}, {}
        try:
            tg = cfg(*parse(disasm("target", t_rva))[:2], stats=ts)
            bg = cfg(*parse(disasm("base", b_rva))[:2], stats=bs)
        except SystemExit:
            undisassembled += 1
            continue
        tb, bb = branch_rows(tg), branch_rows(bg)
        flow_same = [x[2] for x in bg] == [x[2] for x in tg]
        if ts["trimmed"] > len(tg) or bs["trimmed"] > len(bg):
            # the trailing trim ate the function (see `trim_tail`); the graph is
            # a prefix, so NO verdict about it is honest - not even IDENTICAL,
            # which is what `model_factory::create_render_surface` used to get
            # after collapsing 30-vs-23 blocks to 1-vs-1.
            verdict = "TRIMMED"
        elif flow_same:
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
        # 3 decimals: every row here is BELOW 100 by construction, and %.2f
        # printed 99.997 as "100.00" in a list defined as "not 100".
        print(f"{pct:7.3f} {len(bg):>5}/{len(tg):<5} {len(bb):>6}/{len(tb):<6}  "
              f"{verdict:<14} {dem[:96]}")
    print(f"\n[{sum(tally.values())} of {len(rows)} candidate(s) classified"
          + (f"; {undisassembled} could not be disassembled" if undisassembled else "")
          + (f"; {unscored} more paired function(s) in scope carry no fuzzy score and are "
             "not candidates" if unscored else "") + "]")
    print("[verdicts] " + "  ".join(f"{k}={v}" for k, v in sorted(tally.items())))
    if tally.get("TRIMMED"):
        print("[TRIMMED = the trailing trim dropped more blocks than it kept, so the graph "
              "is a PREFIX of the function and no flow verdict about it is honest. Read "
              "those with `pdb_fetch --view target|base` - see sema_tools.md.]")
    print("[FLOW-SAME/IDENTICAL = not control flow: operands, regalloc, scheduling.]")
    print("[ORDER-ONLY = isomorphic CFG, different block LAYOUT - one merged exit placed "
          "elsewhere; usually downstream, not a per-branch bug.]")
    print("[BRANCH-COUNT first (measured: 5 real source bugs in 9 opened), then "
          "BLOCK-COUNT; TOPOLOGY was 0 for 6 - see docs/binary_matching/sema_tools.md.]")
    return 0
