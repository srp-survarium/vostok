"""vostok.sema.blocks - THE verdict view: same shape or not?

`blocks --diff` aligns the two CFGs by CONTENT (equal whole blocks anchor the
alignment), so ONE inserted block reads as one insertion instead of "everything
after this differs". It also names the three answers that are one fact rather
than several: identical flow, an ISOMORPHIC pair that differs only in block
LAYOUT, and a genuine skeleton divergence at a named block.
"""

from __future__ import annotations

import difflib
import re

from vostok.sema import die
from vostok.sema.branches import branch_diff
from vostok.sema.cfg import align, graphs_for, iso_map, kind, notes, starved
from vostok.sema.index import hint


def show_blocks(graph, lite):
    out = []
    npred = {}
    for _, _, term, _ in graph:
        for m in re.finditer(r"B(\d+)", term or ""):
            npred[int(m.group(1))] = npred.get(int(m.group(1)), 0) + 1
    for i, (off, body, term, src) in enumerate(graph):
        loop = "   LOOP" if "^" in (term or "") else ""
        tail = "   <== tail" if term == "ret" and npred.get(i, 0) > 2 else ""
        if lite:
            first = body[0].split(" ")[0] if body else "?"
            out.append(f"  B{i:<3} @{off:<6x} {len(body):>3}i  [{term}]{loop}{tail}   ({first}..)")
            continue
        out.append("")
        out.append(f"B{i} @{off:x}   {len(body)} insn   [{term}]{loop}{tail}"
                   + (f"\n        ; {src}" if src else ""))
        for ins in body:
            out.append(f"    {ins}")
    return "\n".join(out) + "\n"


def blocks_diff(b, t, lite):
    """Aligned CFG diff. `b`/`t` are cfg() outputs for base/target."""
    bflow = [x[2] or "" for x in b]
    tflow = [x[2] or "" for x in t]
    same_flow = bflow == tflow
    pairs = align(b, t)
    out = [f"[block diff: base {len(b)} blocks vs target {len(t)} blocks; "
           f"flow {'SAME' if same_flow else 'DIFFERS'}]"]
    iso = None if same_flow else iso_map(b, t)
    if iso is not None:
        moved = [f"base B{k}->target B{v}" for k, v in sorted(iso.items()) if k != v]
        out.append("[ORDER-ONLY: the two CFGs are ISOMORPHIC - same blocks, same edges. "
                   "Only the linear BLOCK ORDER differs, which flips the polarity of "
                   "every branch that reaches a moved block. One fact, not several.]")
        out.append("[  moved: " + ", ".join(moved) + "]")
    if not same_flow:
        bk = [kind(x, i) for i, x in enumerate(bflow)]
        tk = [kind(x, i) for i, x in enumerate(tflow)]
        k = next((i for i, (x, y) in enumerate(zip(bk, tk)) if x != y), None)
        if k is not None:
            out.append(f"[skeleton diverges at B{k}: base [{bflow[k]}]  vs  "
                       f"target [{tflow[k]}]  (branch kinds identical before)]")
            if b[k][3] or t[k][3]:
                out.append(f"[  base block source: {b[k][3] or '-'}]")
        elif len(bk) != len(tk):
            side = "base" if len(bk) > len(tk) else "target"
            out.append(f"[same branch skeleton for the shared {min(len(bk), len(tk))} "
                       f"blocks; {side} has {abs(len(bk) - len(tk))} extra block(s)]")
        else:
            out.append("[same branch-kind skeleton; only block-index targets differ "
                       "(an inserted/moved block shifts later indices)]")
    ndiff = sum(1 for bi, tj in pairs
                if bi is None or tj is None
                or "\n".join(b[bi][1] + [b[bi][2] or ""]) !=
                   "\n".join(t[tj][1] + [t[tj][2] or ""]))
    if lite:
        out.append(f"       {'BASE':30}    TARGET    "
                   "(== exact, ~= same kind/shifted target, ## size drift, !! kind mismatch)")
        first_bad = None
        for bi, tj in pairs:
            bl = f"B{bi:<3} {len(b[bi][1]):>3}i [{b[bi][2]}]" if bi is not None else "-"
            tl = f"B{tj:<3} {len(t[tj][1]):>3}i [{t[tj][2]}]" if tj is not None else "-"
            if bi is None or tj is None:
                mark = "!!"
            elif b[bi][2] == t[tj][2]:
                mark = "==" if len(b[bi][1]) == len(t[tj][1]) else "##"
            elif kind(b[bi][2], bi) == kind(t[tj][2], tj):
                mark = "~="
            else:
                mark = "!!"
            if mark == "!!" and first_bad is None:
                first_bad = tj if tj is not None else bi
            out.append(f"  {bl:34} {mark} {tl}")
        if first_bad is not None:
            out.append(f"[first true skeleton divergence at B{first_bad}]")
        return "\n".join(out) + "\n", (0 if same_flow and not ndiff else 1)

    for bi, tj in pairs:
        if bi is not None and tj is not None:
            bstr = "\n".join(b[bi][1] + [b[bi][2] or ""])
            tstr = "\n".join(t[tj][1] + [t[tj][2] or ""])
            if bstr == tstr:
                out.append(f"  B{tj} @{b[bi][0]:x}/@{t[tj][0]:x}  == "
                           f"({len(t[tj][1])} insns)  [{t[tj][2]}]")
                continue
            src = b[bi][3] or t[tj][3]
            out.append(f"  B{tj} @{b[bi][0]:x}/@{t[tj][0]:x}  DIFFERS:"
                       + (f"   ; {src}" if src else ""))
            for ln in difflib.unified_diff(b[bi][1] + [b[bi][2] or ""],
                                           t[tj][1] + [t[tj][2] or ""],
                                           lineterm="", n=2):
                if not ln.startswith(("---", "+++", "@@")):
                    out.append("      " + ln)
        elif bi is not None:
            out.append(f"  --  @{b[bi][0]:x}  BASE-ONLY block "
                       f"({len(b[bi][1])} insns)  [{b[bi][2]}]"
                       + (f"   ; {b[bi][3]}" if b[bi][3] else "") + ":")
            for ln in b[bi][1][:8]:
                out.append(f"      -{ln}")
        else:
            out.append(f"  B{tj} @{t[tj][0]:x}  TARGET-ONLY block "
                       f"({len(t[tj][1])} insns)  [{t[tj][2]}]:")
            for ln in t[tj][1][:8]:
                out.append(f"      +{ln}")
    out.append(f"[{ndiff} block(s) differ]" if ndiff else "[all aligned blocks identical]")
    return "\n".join(out) + "\n", (0 if same_flow and not ndiff else 1)


def cmd_blocks(args):
    tgt, base, g, cut = graphs_for(args.fn, need_both=args.diff)
    if args.diff:
        text, rc = blocks_diff(g["base"], g["target"], args.lite)
        print(f"[{tgt['name']}]")
        print(f"[base {base['file']} @ {base['rva']:#x} ({base['size']}B)  vs  "
              f"target {tgt['file']} @ {tgt['rva']:#x} ({tgt['size']}B)]")
        for note in notes(cut):
            print(note)
        print(text, end="")
        # the hint is about SHAPE, so it fires whenever the flow matches - a
        # byte-dirty diff over a matching CFG is exactly the case worth naming
        if not starved(cut) and \
                [x[2] for x in g["base"]] == [x[2] for x in g["target"]]:
            _, brc = branch_diff(g["base"], g["target"])
            hint(tgt["mangled"], True, brc == 0)
        return rc
    side = "base" if args.base else "target"
    rec = base if args.base else tgt
    if side not in g:
        die(f"no {side} side for '{args.fn}'")
    print(f"[basic blocks: {side.upper()} - {rec['name']}]")
    print(f"[{rec['file']} @ {rec['rva']:#x} ({rec['size']}B)]")
    for note in notes({side: cut[side]}):
        print(note)
    print(show_blocks(g[side], args.lite), end="")
    return 0
