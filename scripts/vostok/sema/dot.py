# SPDX-License-Identifier: GPL-3.0-or-later

"""vostok.sema.dot - the CFG as graphviz, optionally with the diff painted on.

`--diff` colours every target block red when it has no content-equal partner on
the base side, which makes "where is the divergence" a picture instead of a
scroll.
"""

from __future__ import annotations

import difflib
import re
import sys

from vostok.sema import die
from vostok.sema.cfg import graphs_for, notes


def dot(graph, title, diff_idx=None):
    npred = {}
    for _, _, term, _ in graph:
        for m in re.finditer(r"B(\d+)", term or ""):
            npred[int(m.group(1))] = npred.get(int(m.group(1)), 0) + 1
    out = ["digraph cfg {",
           f'  graph [rankdir=TB, fontname="monospace", label="{title}", labelloc=t];',
           '  node [shape=box, fontname="monospace", fontsize=10];']
    for i, (off, body, term, _) in enumerate(graph):
        label = f"B{i} @{off:x}  ({len(body)}i)\\l" + (f"{body[0]}\\l" if body else "")
        if len(body) > 1:
            label += ("...\\l" if len(body) > 2 else "") + f"{body[-1]}\\l"
        style = []
        if term == "ret" and npred.get(i, 0) > 2:
            style.append("peripheries=2")
        if diff_idx is not None:
            style.append('style=filled, fillcolor="%s"'
                         % ("#ffcccc" if i in diff_idx else "#ccffcc"))
        out.append(f'  B{i} [label="{label}"'
                   + (", " + ", ".join(style) if style else "") + "];")
        for m in re.finditer(r"(jcc|jmp|fall) B(\d+)(\^?)", term or ""):
            attrs = []
            if m.group(1) == "fall":
                attrs.append("style=dashed")
            if m.group(3):
                attrs.append("color=red, constraint=false")
            out.append(f"  B{i} -> B{m.group(2)}"
                       + (" [" + ", ".join(attrs) + "]" if attrs else "") + ";")
    out.append("}")
    return "\n".join(out) + "\n"


def cmd_dot(args):
    tgt, base, g, cut = graphs_for(args.fn, need_both=args.diff)
    # stdout is a graphviz stream, so the contraction/trim notes go to stderr
    # rather than corrupting a `| dot -Tpng` pipe.
    for note in notes(cut if args.diff else {k: v for k, v in cut.items()
                                             if k == ("base" if args.base else "target")}):
        sys.stderr.write(note + "\n")
    if args.diff:
        bs = ["\n".join(x[1] + [x[2] or ""]) for x in g["base"]]
        ts = ["\n".join(x[1] + [x[2] or ""]) for x in g["target"]]
        sm = difflib.SequenceMatcher(a=bs, b=ts, autojunk=False)
        bad = set(range(len(ts)))
        for tag, i1, i2, j1, j2 in sm.get_opcodes():
            if tag == "equal":
                bad -= set(range(j1, j2))
        print(dot(g["target"], f"{tgt['name']} TARGET (red = differs)", bad), end="")
        return 0
    side = "base" if args.base else "target"
    rec = base if args.base else tgt
    if side not in g:
        die(f"no {side} side for '{args.fn}'")
    print(dot(g[side], f"{rec['name']} {side.upper()}"), end="")
    return 0
