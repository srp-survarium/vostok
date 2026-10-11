# SPDX-License-Identifier: GPL-3.0-or-later

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


def _text(block):
    return "\n".join(block[1] + [block[2] or ""])


def flow_verdict(b, t):
    """The facts `blocks_diff` prints, as data. `b`/`t` are cfg() outputs.

    `same_flow`: identical terminator sequence. `moved`: for an ISOMORPHIC pair
    that differs only in block layout, the (base, target) indices that moved
    (`iso` is the full map, None when not isomorphic). `skeleton`: the first
    block whose branch KIND differs. `extra`: (side, n) when the skeletons agree
    over the shared blocks and one side has more. `differing`: the aligned
    (base, target) pairs whose bodies or terminators differ, None for a
    one-sided block."""
    bflow = [x[2] or "" for x in b]
    tflow = [x[2] or "" for x in t]
    same_flow = bflow == tflow
    pairs = align(b, t)
    iso = None if same_flow else iso_map(b, t)
    skeleton = extra = None
    if not same_flow:
        bk = [kind(x, i) for i, x in enumerate(bflow)]
        tk = [kind(x, i) for i, x in enumerate(tflow)]
        k = next((i for i, (x, y) in enumerate(zip(bk, tk)) if x != y), None)
        if k is not None:
            skeleton = {"block": k, "base": bflow[k], "target": tflow[k],
                        "source": b[k][3] or t[k][3] or ""}
        elif len(bk) != len(tk):
            extra = ("base" if len(bk) > len(tk) else "target", abs(len(bk) - len(tk)),
                     min(len(bk), len(tk)))
    differing = [(bi, tj) for bi, tj in pairs
                 if bi is None or tj is None or _text(b[bi]) != _text(t[tj])]
    return {"same_flow": same_flow, "iso": iso,
            "moved": [(k, v) for k, v in sorted((iso or {}).items()) if k != v],
            "skeleton": skeleton, "extra": extra, "pairs": pairs, "differing": differing}


def blocks_diff(b, t, lite):
    """Aligned CFG diff. `b`/`t` are cfg() outputs for base/target."""
    verdict = flow_verdict(b, t)
    same_flow, pairs = verdict["same_flow"], verdict["pairs"]
    out = [f"[block diff: base {len(b)} blocks vs target {len(t)} blocks; "
           f"flow {'SAME' if same_flow else 'DIFFERS'}]"]
    if verdict["iso"] is not None:
        moved = [f"base B{k}->target B{j}" for k, j in verdict["moved"]]
        out.append("[ORDER-ONLY: the two CFGs are ISOMORPHIC - same blocks, same edges. "
                   "Only the linear BLOCK ORDER differs, which flips the polarity of "
                   "every branch that reaches a moved block. One fact, not several.]")
        out.append("[  moved: " + ", ".join(moved) + "]")
    if not same_flow:
        if verdict["skeleton"]:
            k = verdict["skeleton"]["block"]
            out.append(f"[skeleton diverges at B{k}: base [{verdict['skeleton']['base']}]  vs  "
                       f"target [{verdict['skeleton']['target']}]  (branch kinds identical before)]")
            if b[k][3] or t[k][3]:
                out.append(f"[  base block source: {b[k][3] or '-'}]")
        elif verdict["extra"]:
            side, n, shared = verdict["extra"]
            out.append(f"[same branch skeleton for the shared {shared} "
                       f"blocks; {side} has {n} extra block(s)]")
        else:
            out.append("[same branch-kind skeleton; only block-index targets differ "
                       "(an inserted/moved block shifts later indices)]")
    ndiff = len(verdict["differing"])
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
        print(f"[base {base['file']} @ rva={base['rva']:#x} ({base['size']}B)  vs  "
              f"target {tgt['file']} @ rva={tgt['rva']:#x} ({tgt['size']}B)]")
        for note in notes(cut):
            print(note)
        print(text, end="")
        # the hint is about SHAPE, so it fires whenever the flow matches - a
        # byte-dirty diff over a matching CFG is exactly the case worth naming
        if not starved(cut) and \
                [x[2] for x in g["base"]] == [x[2] for x in g["target"]]:
            _, brc = branch_diff(g["base"], g["target"])
            hint(tgt, True, brc == 0)
        return rc
    side = "base" if args.base else "target"
    rec = base if args.base else tgt
    if side not in g:
        die(f"no {side} side for '{args.fn}'")
    print(f"[basic blocks: {side.upper()} - {rec['name']}]")
    print(f"[{rec['file']} @ rva={rec['rva']:#x} ({rec['size']}B)]")
    for note in notes({side: cut[side]}):
        print(note)
    print(show_blocks(g[side], args.lite), end="")
    return 0
