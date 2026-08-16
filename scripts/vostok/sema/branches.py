"""vostok.sema.branches - read an established difference, branch by branch.

This view pairs branches POSITIONALLY, which is only meaningful once `blocks`
has established that the two sides have the same shape. It classifies each
mismatch as SIGNEDNESS (nearly always a real source type bug), POLARITY, or
TOPOLOGY - and says so loudly when the block counts differ, because then the
positional pairing is evidence, not a defect count.
"""

from __future__ import annotations

from vostok.sema import die
from vostok.sema.cfg import (INVERSE, SIGNED_TWIN, _contract_note, branch_rows,
                             graphs_for, iso_map)
from vostok.sema.index import hint


def show_branches(graph, label):
    rows = branch_rows(graph)
    out = [f"[branch sequence: {label}]"]
    for idx, blk, mn, dst in rows:
        d = f"B{dst}" if dst is not None else "<ext/table>"
        out.append(f"  #{idx:<3} B{blk:<3} @{graph[blk][0]:<6x} {mn:<6} -> {d}")
    nret = sum(1 for x in graph if x[2] == "ret")
    out.append(f"  {len(rows)} branch(es), {nret} ret(s), {len(graph)} block(s)")
    return "\n".join(out) + "\n"


def branch_diff(b, t):
    br, tr = branch_rows(b), branch_rows(t)
    nbret = sum(1 for x in b if x[2] == "ret")
    ntret = sum(1 for x in t if x[2] == "ret")
    out = ["[branch diff: BASE (compiled) vs TARGET (original)]",
           "[destinations are named by BLOCK INDEX, so a uniform displacement shift "
           "compares EQUAL and a genuine retarget does not]",
           f"  base {len(br)} branch(es), {nbret} ret(s)   |   "
           f"target {len(tr)} branch(es), {ntret} ret(s)"]
    # Branches are paired POSITIONALLY. That is only meaningful when the two
    # sides have the same number of blocks; otherwise ONE structural difference
    # shifts every later pair and prints as a run of POLARITY/TOPOLOGY rows that
    # are not independent defects. Say so instead of letting a reader count them.
    skewed = len(b) != len(t)
    if skewed:
        out.append(f"  BLOCK COUNTS DIFFER (base {len(b)} vs target {len(t)}) - this view "
                   "pairs branches BY POSITION, so one structural difference shifts every "
                   "later pair. Take the verdict from `blocks --diff --lite` (content-"
                   "aligned); read the rows below as evidence, NOT as a defect count.")
    if len(br) != len(tr):
        out.append("  BRANCH COUNTS DIFFER - structural: a block we did not reconstruct, "
                   "an `if` the optimizer folded, or a different inlining decision. "
                   "Reconstruct, do not re-spell.  (`sema blocks --diff` shows where)")
        return "\n".join(out) + "\n", 1
    if not br:
        out.append("  no branches on either side.")
        return "\n".join(out) + "\n", 0
    flips = [(i, br[i], tr[i]) for i in range(len(br))
             if br[i][2] != tr[i][2] or br[i][3] != tr[i][3]]
    if not flips:
        out.append("  branch sequences AGREE (mnemonics and block destinations). "
                   "Whatever is left is instruction selection / regalloc / operands, "
                   "NOT control flow.")
        return "\n".join(out) + "\n", 0
    iso = iso_map(b, t)
    if iso is not None and any(k != v for k, v in iso.items()):
        out.append("  ORDER-ONLY: the CFGs are ISOMORPHIC (same blocks, same edges); only "
                   "the linear block ORDER differs, so the rows below are ONE fact - a "
                   "merged exit block placed elsewhere - not several retargets:")
        out.append("    moved: " + ", ".join(f"base B{k}->target B{v}"
                                             for k, v in sorted(iso.items()) if k != v))
    topo = [f for f in flips if f[1][2] == f[2][2]]
    mn = [f for f in flips if f[1][2] != f[2][2]]
    if topo:
        out.append("  TOPOLOGY - same mnemonic, different destination block "
                   "(the shape an instruction diff cannot show you):")
        for i, x, y in topo:
            out.append(f"    #{i:<3} B{x[1]:<3} @{b[x[1]][0]:<6x} {x[2]:<6} "
                       f"target lands on B{y[3]}, we land on B{x[3]}"
                       + (f"   ; {b[x[1]][3]}" if b[x[1]][3] else ""))
    for i, x, y in mn:
        tag = ("SIGNEDNESS" if SIGNED_TWIN.get(x[2]) == y[2] else
               "POLARITY" if INVERSE.get(x[2]) == y[2] else "OTHER")
        out.append(f"  {tag}  #{i:<3} B{x[1]:<3} @{b[x[1]][0]:<6x}  base {x[2]:<6} -> "
                   f"target {y[2]}"
                   + (f"   ; {b[x[1]][3]}" if b[x[1]][3] else ""))
        if skewed:
            continue            # positional pairing is unreliable - no interpretation
        if tag == "SIGNEDNESS":
            out.append("        a signed/unsigned twin is nearly always a REAL source "
                       "type bug - a member/local/param that wants the other signedness")
        elif tag == "POLARITY":
            out.append("        inverted condition: read where each side GOES, not just "
                       "the mnemonic - the `if` body and `else` body may be swapped")
    return "\n".join(out) + "\n", 1


def cmd_branches(args):
    tgt, base, g, cut = graphs_for(args.fn, need_both=args.diff)
    if args.diff:
        print(f"[{tgt['name']}]")
        note = _contract_note(cut)
        if note:
            print(note)
        text, rc = branch_diff(g["base"], g["target"])
        print(text, end="")
        if rc == 0:
            flow = [x[2] for x in g["base"]] == [x[2] for x in g["target"]]
            hint(tgt["mangled"], flow, True)
        return rc
    side = "base" if args.base else "target"
    rec = base if args.base else tgt
    if side not in g:
        die(f"no {side} side for '{args.fn}'")
    print(show_branches(g[side], f"{side.upper()} - {rec['name']}"), end="")
    return 0
