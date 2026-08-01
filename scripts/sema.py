#!/usr/bin/env python3
"""sema - CONTROL-FLOW views over the delinked base<->target function pair.

The layer vostok did not have. `pdb_fetch --view structure-diff` compares SOURCE
STATEMENTS and `pdb_fetch --view diff` compares INSTRUCTIONS (objdiff-core,
operand/reloc aware); neither says anything about the shape of the control-flow
graph. A branch that lands on a different basic block is, to both of those views,
either invisible or an uninterpretable pair of absolute addresses:

    ~ 0x2b: je short 0000029Eh    -> je short 000003C1h

Two numbers from two different address spaces. Nothing tells you whether that is
a real retarget (an `if` we got wrong) or the harmless consequence of an upstream
size drift. `sema blocks --diff` and `sema branches --diff` answer exactly that,
by naming every branch destination with a BLOCK INDEX derived per side, so the
two sides become comparable and a uniform displacement shift compares EQUAL while
a genuine retarget does not. Design + evidence: docs/binary_matching/sema_tools.md.

Both sides come out of the same delinker and the same disassembler
(`pdb_fetch --view target` / `--view base` over `binaries/rich/{target,base}`),
so - unlike the two-disassembler setups this idea comes from - almost no
instruction-spelling normalization is needed: only the branch operand is masked.
Every other textual difference between the two sides is a real byte difference.

    python3 scripts/sema.py blocks   <fn>            # target-side CFG
    python3 scripts/sema.py blocks   <fn> --base     # base-side CFG
    python3 scripts/sema.py blocks   <fn> --diff     # aligned CFG diff  (the main view)
    python3 scripts/sema.py blocks   <fn> --diff --lite    # skeleton only
    python3 scripts/sema.py branches <fn> --diff     # ordered branch sequence, diffed
    python3 scripts/sema.py dot      <fn> [--diff]   # graphviz
    python3 scripts/sema.py sweep --module render [--unit U] [--max N]

<fn> is a mangled name, a demangled substring, or a hex RVA/VA on either side.

rc: 0 = answered YES/clean, 1 = answered NO (flow or branches differ), 2 = error.
"""

import argparse
import bisect
import difflib
import json
import os
import re
import shutil
import sqlite3
import subprocess
import sys
from pathlib import Path

VOSTOK = Path(__file__).resolve().parent.parent
RICH = VOSTOK / "binaries" / "rich"
DB_PATH = VOSTOK / "docs" / "binary_matching" / "match.db"
OBJDIFF = VOSTOK / "binaries" / "objdiff"


def die(msg):
    sys.stderr.write(f"sema: {msg}\n")
    sys.exit(2)


# --------------------------------------------------------------------------
# side selection: which function, on which side, at which RVA
# --------------------------------------------------------------------------

def _index_path(side):
    p = RICH / side / "index.jsonl"
    if not p.is_file():
        die(f"{p} missing - run scripts/rebuild.py first")
    return p


def _scan_index(side, want):
    """One pass over binaries/rich/<side>/index.jsonl (~85 MB, ~0.05 s).

    `want(record_line, record_or_None)` is called with the raw line first so the
    common case never pays for json.loads."""
    hits = []
    with open(_index_path(side), encoding="utf-8", errors="replace") as fh:
        for line in fh:
            rec = want(line)
            if rec is not None:
                hits.append(rec)
    return hits


def _matcher(sel):
    """Build the line predicate for a selector: exact mangled > mangled
    substring > demangled substring > hex RVA/VA."""
    hexval = None
    if re.fullmatch(r"0[xX][0-9a-fA-F]+", sel):
        hexval = int(sel, 16)
    low = sel.lower()

    def want(line):
        if hexval is not None:
            # cheap pre-filter before json.loads
            if f'"rva":{hexval}' not in line and f'"rva":{hexval - 0x10000}' not in line:
                return None
            rec = json.loads(line)
            if rec["rva"] in (hexval, hexval - rec.get("image_base", 0)):
                return rec
            return None
        if sel in line:
            rec = json.loads(line)
            if rec["mangled"] == sel or sel in rec["mangled"]:
                return rec
        if low in line.lower():
            rec = json.loads(line)
            if low in rec["name"].lower():
                return rec
        return None

    return want


def resolve(sel):
    """(target_record | None, base_record | None) for one selector.

    Ambiguity is resolved by preferring an EXACT mangled hit; anything still
    ambiguous is reported rather than guessed at."""
    want = _matcher(sel)
    tgt = _scan_index("target", want)
    base = _scan_index("base", want)
    exact = [r for r in tgt if r["mangled"] == sel] or \
            [r for r in base if r["mangled"] == sel]
    if exact:
        m = exact[0]["mangled"]
        tgt = [r for r in tgt if r["mangled"] == m]
        base = [r for r in base if r["mangled"] == m]
    if len(tgt) > 1 or (not tgt and len(base) > 1):
        for r in (tgt or base):
            sys.stderr.write(f"  {r['mangled']}\n      {r['name']}\n")
        die(f"'{sel}' is ambiguous ({len(tgt or base)} hits) - pass a mangled name or RVA")
    if tgt and base:
        # pair strictly by mangled name
        base = [r for r in base if r["mangled"] == tgt[0]["mangled"]]
    return (tgt[0] if tgt else None), (base[0] if base else None)


def fuzzy_of(mangled):
    """match.db's fuzzy % for a mangled name (max across units), or None.
    Read-only, and only consulted for the < 100 hint."""
    if not DB_PATH.is_file():
        return None
    try:
        con = sqlite3.connect(f"file:{DB_PATH}?mode=ro", uri=True)
        row = con.execute(
            "SELECT MAX(fuzzy_pct) FROM paired WHERE mangled = ?", (mangled,)
        ).fetchone()
        con.close()
        return row[0] if row else None
    except sqlite3.Error:
        return None


# --------------------------------------------------------------------------
# disassembly: pdb_fetch renders it, we parse it
# --------------------------------------------------------------------------

RE_STMT = re.compile(r"^\[0x([0-9a-fA-F]+)\]:(?:\s*(.*))?$")
RE_LABEL = re.compile(r"^\.(\d+):\s*$")
RE_INSN = re.compile(r"^0x([0-9a-fA-F]+):\s+(\S.*?)\s*$")


def _pdb_fetch():
    exe = os.environ.get("PDB_FETCH") or shutil.which("pdb_fetch")
    if not exe:
        die("pdb_fetch not on PATH - run inside `nix develop`")
    return exe


def disasm(side, rva):
    """One side's rendered disassembly (offsets, `.N` labels, symbolized calls,
    and - base side only - the source statement headers)."""
    cmd = [_pdb_fetch(), f"--{side}-index", str(_index_path(side)),
           "--rva", hex(rva), "--view", side]
    res = subprocess.run(cmd, cwd=str(VOSTOK), capture_output=True, text=True)
    if res.returncode != 0:
        die(f"pdb_fetch failed: {res.stderr.strip()}")
    if "no function matched" in res.stdout:
        die(f"pdb_fetch found no {side} function at {rva:#x}")
    return res.stdout


def parse(text):
    """-> (insns, labels, stmts): [(off, text)], {'.1': off}, {off: source}."""
    insns, labels, stmts, pending = [], {}, {}, []
    for line in text.splitlines():
        line = line.rstrip()
        m = RE_LABEL.match(line)
        if m:
            pending.append("." + m.group(1))
            continue
        m = RE_INSN.match(line)
        if m:
            off = int(m.group(1), 16)
            for lab in pending:
                labels[lab] = off
            pending = []
            insns.append((off, re.sub(r"\s+", " ", m.group(2)).strip()))
            continue
        m = RE_STMT.match(line)
        if m:
            stmts[int(m.group(1), 16)] = (m.group(2) or "").strip()
    return insns, labels, stmts


# --------------------------------------------------------------------------
# CFG
# --------------------------------------------------------------------------

RE_TARGET = re.compile(r"(?:^|[\s,\[])(\.\d+)\s*$")


def mnem(text):
    return text.split(" ", 1)[0].lower()


def is_branch(m):
    return m.startswith("j") or m.startswith("loop")


def is_ret(m):
    return m.startswith("ret") or m == "iret"


def cfg(insns, labels, stmts=None):
    """[(off, [masked insns], term, src)] with every branch destination named by
    BLOCK INDEX, which is what makes the two sides comparable.

    Leaders = block starts = every label target PLUS every post-branch/post-ret
    instruction. That second rule matters: without it a jump-table's dispatch
    arms (which carry no label) get absorbed into the preceding block and their
    terminators vanish."""
    if not insns:
        return []
    leaders = {insns[0][0]} | set(labels.values())
    for i, (off, text) in enumerate(insns):
        m = mnem(text)
        if i + 1 < len(insns) and (is_branch(m) or is_ret(m)):
            leaders.add(insns[i + 1][0])
    order = sorted(leaders)

    def blk_of(off):
        return bisect.bisect_right(order, off) - 1

    blocks = [[off, [], None, (stmts or {}).get(off, "")] for off in order]
    for i, (off, text) in enumerate(insns):
        b = blocks[blk_of(off)]
        m = mnem(text)
        tm = RE_TARGET.search(text)
        tgt = labels.get(tm.group(1)) if tm else None
        # the body keeps the branch MNEMONIC but not its destination; the
        # destination lives in the terminator, in block-index form
        b[1].append(RE_TARGET.sub(" <tgt>", text) if tgt is not None else text)
        nxt = insns[i + 1][0] if i + 1 < len(insns) else None
        if nxt is not None and blk_of(nxt) == blk_of(off):
            continue                                    # not the block's last insn
        if is_branch(m):
            # a destination outside the function (tail call) or through a jump
            # table stays <ext> - unknown, but still a branch, never a fall
            d = (f"B{blk_of(tgt)}" + ("^" if tgt <= off else "")) if tgt is not None else "<ext>"
            if m == "jmp":
                b[2] = f"jmp {d}"
            elif nxt is not None:
                b[2] = f"jcc {d} | fall B{blk_of(nxt)}"
            else:
                b[2] = f"jcc {d}"
        elif is_ret(m):
            b[2] = "ret"
        elif nxt is not None:
            b[2] = f"fall B{blk_of(nxt)}"
        else:
            b[2] = "end"
    out = [tuple(b) for b in blocks]
    while out and all(x.startswith(("nop", "int3")) for x in out[-1][1]):
        out.pop()                                       # alignment padding
    return trim_tail(out)


def succs(term):
    return {int(x) for x in re.findall(r"B(\d+)", term or "")}


def trim_tail(blocks):
    """Drop the trailing blocks that are DATA, not code.

    A switch's jump TABLE lives in `.text` immediately after the function, and
    the delinker's symbol size covers it - so the disassembler happily decodes
    the table bytes into plausible-looking instructions ('mov dl,0CAh / jns').
    Left in, that phantom tail differs on every switch function and reads as a
    control-flow divergence that is not one.

    Only a TRAILING block is ever dropped, and only when it is unreachable from
    the entry AND does not branch back into the kept prefix. A jump-table ARM is
    also unreachable under this edge model (its dispatch is a computed `jmp`),
    but it jumps forward to the merge block, which the second condition keeps."""
    reach, stack = set(), [0]
    while stack:
        i = stack.pop()
        if i in reach or i >= len(blocks):
            continue
        reach.add(i)
        stack += list(succs(blocks[i][2]))
    while len(blocks) > 1:
        i = len(blocks) - 1
        if i in reach or succs(blocks[i][2]) & set(range(i)):
            break
        blocks.pop()
    return blocks


def kind(term, at):
    """The branch SKELETON of one terminator: kinds + directions, no absolute
    indices - so a single inserted block does not cascade into every later row."""
    parts = []
    for k, _, idx, back in re.findall(r"(jcc|jmp|ret|fall|end)( B(\d+)(\^?))?", term or ""):
        if idx:
            parts.append(k + ("^" if back else ">" if int(idx) > at else "<"))
        else:
            parts.append(k)
    return " ".join(parts)


# --------------------------------------------------------------------------
# views
# --------------------------------------------------------------------------

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


def canon(block):
    """One block's LAYOUT-FREE exit: (canonical condition, ordered successors).

    A conditional branch is spelled two ways depending on which side the
    compiler chose to fall through to (`ja X / fall Y` == `jbe Y / fall X`), so
    the mnemonic is canonicalised to one of the inverse pair and the successors
    are swapped to match. Two blocks that mean the same thing then compare equal
    whatever the block ORDER is."""
    body, term = block[1], block[2] or ""
    m = re.match(r"jcc B(\d+)\^? \| fall B(\d+)$", term)
    if m:
        mn = mnem(body[-1]) if body else "?"
        inv = INVERSE.get(mn)
        x, y = int(m.group(1)), int(m.group(2))
        if inv and inv < mn:
            return inv, [y, x]
        return mn, [x, y]
    m = re.match(r"jmp B(\d+)", term)
    if m:
        return "jmp", [int(m.group(1))]
    m = re.match(r"jcc <ext> \| fall B(\d+)$", term)
    if m:
        return "jcc<ext>", [int(m.group(1))]
    m = re.match(r"fall B(\d+)$", term)
    if m:
        return "fall", [int(m.group(1))]
    return term, []


def iso_map(b, t):
    """base block index -> target block index, if the two CFGs are ISOMORPHIC
    (same blocks, same edges) - or None.

    This is the distinction that decides what kind of work a function needs. An
    isomorphic pair whose mapping is NOT the identity differs only in block
    LAYOUT: the compiler placed a merged `return` block at a different point in
    the linear order, which flips the polarity of every branch that reaches it.
    That reads as several 'inverted conditions' and 'retargeted branches' in any
    positional comparison, and it is ONE fact, not several.

    Bodies are deliberately NOT compared here - this answers 'same shape?', and
    `blocks --diff` answers 'same instructions?'."""
    if len(b) != len(t):
        return None
    fwd, rev, stack = {}, {}, [(0, 0)]
    while stack:
        bi, tj = stack.pop()
        if bi in fwd:
            if fwd[bi] != tj:
                return None
            continue
        if tj in rev:
            return None
        fwd[bi], rev[tj] = tj, bi
        cb, sb = canon(b[bi])
        ct, st = canon(t[tj])
        if cb != ct or len(sb) != len(st):
            return None
        stack += list(zip(sb, st))
    return fwd if len(fwd) == len(b) else None


def align(b, t):
    """Pair base blocks to target blocks: [(bi|None, tj|None)].

    Equal whole blocks (body + terminator) anchor the alignment; a replace-run
    pairs index-wise inside the run and the overhang becomes a one-sided row. So
    ONE inserted block shows up as one insertion, not as "everything after this
    differs" - the cascade that makes an instruction-level diff unreadable."""
    bs = ["\n".join(x[1] + [x[2] or ""]) for x in b]
    ts = ["\n".join(x[1] + [x[2] or ""]) for x in t]
    pairs = []
    for tag, i1, i2, j1, j2 in difflib.SequenceMatcher(a=bs, b=ts, autojunk=False).get_opcodes():
        if tag == "equal":
            pairs += [(i1 + k, j1 + k) for k in range(i2 - i1)]
        else:
            for k in range(max(i2 - i1, j2 - j1)):
                pairs.append((i1 + k if i1 + k < i2 else None,
                              j1 + k if j1 + k < j2 else None))
    return pairs


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


# --------------------------------------------------------------------------
# branch sequence
# --------------------------------------------------------------------------

INVERSE = {"je": "jne", "jz": "jnz", "ja": "jbe", "jae": "jb", "jg": "jle",
           "jge": "jl", "js": "jns", "jo": "jno", "jp": "jnp"}
INVERSE.update({v: k for k, v in list(INVERSE.items())})
SIGNED_TWIN = {"jl": "jb", "jle": "jbe", "jg": "ja", "jge": "jae"}
SIGNED_TWIN.update({v: k for k, v in list(SIGNED_TWIN.items())})


def branch_rows(graph):
    """[(idx, block, mnemonic, dest_block|None)] in address order - the ordered
    branch sequence, with every destination named by BLOCK INDEX."""
    rows = []
    for i, (off, body, term, _) in enumerate(graph):
        if not term or term.startswith(("fall", "ret", "end")):
            continue
        m = re.search(r"(?:jcc|jmp) B(\d+)", term)
        rows.append((len(rows), i, mnem(body[-1]) if body else "?",
                     int(m.group(1)) if m else None))
    return rows


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
        if tag == "SIGNEDNESS":
            out.append("        a signed/unsigned twin is nearly always a REAL source "
                       "type bug - a member/local/param that wants the other signedness")
        elif tag == "POLARITY":
            out.append("        inverted condition: read where each side GOES, not just "
                       "the mnemonic - the `if` body and `else` body may be swapped")
    return "\n".join(out) + "\n", 1


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


# --------------------------------------------------------------------------
# the hint (why this tool exists)
# --------------------------------------------------------------------------

def hint(mangled, clean_flow, clean_branches):
    """When a view comes out clean but the function is NOT matched, SAY where the
    remaining signal is. The failure mode this guards against: a matcher runs the
    first-look command, sees nothing, and concludes 'regalloc only' - which is a
    verdict, not an observation."""
    pct = fuzzy_of(mangled)
    if pct is None or pct >= 100.0:
        return
    if clean_flow and clean_branches:
        print(f"[this function is {pct:.2f}%, not 100, yet base and target have the SAME "
              "control-flow graph and the SAME branch destinations. The residual is "
              "instruction selection / register allocation / operand values - run "
              "`pdb_fetch --view diff` (operand-aware) for the byte cause, and "
              "`--view structure-diff` for the statement shape.]")
    elif clean_branches:
        print(f"[this function is {pct:.2f}%, not 100. The branch SEQUENCE agrees but "
              "block bodies do not - `sema blocks --diff` names the divergent blocks "
              "and their source statements.]")
    else:
        print(f"[this function is {pct:.2f}%, not 100 - and the control flow itself "
              "differs. Fix the SHAPE first: an instruction-level diff of two "
              "differently-shaped functions is noise.]")


# --------------------------------------------------------------------------
# commands
# --------------------------------------------------------------------------

def graphs_for(sel, need_both=True):
    tgt, base = resolve(sel)
    if need_both and not (tgt and base):
        missing = "base" if tgt else "target"
        die(f"'{sel}' has no {missing} side "
            f"({'TARGET_ONLY - nothing compiled yet' if tgt else 'BASE_ONLY - not in the original'})")
    g = {}
    for side, rec in (("target", tgt), ("base", base)):
        if rec:
            insns, labels, stmts = parse(disasm(side, rec["rva"]))
            g[side] = cfg(insns, labels, stmts)
    return tgt, base, g


def cmd_blocks(args):
    tgt, base, g = graphs_for(args.fn, need_both=args.diff)
    if args.diff:
        text, rc = blocks_diff(g["base"], g["target"], args.lite)
        print(f"[{tgt['name']}]")
        print(f"[base {base['file']} @ {base['rva']:#x} ({base['size']}B)  vs  "
              f"target {tgt['file']} @ {tgt['rva']:#x} ({tgt['size']}B)]")
        print(text, end="")
        _, brc = branch_diff(g["base"], g["target"])
        if rc == 0:
            hint(tgt["mangled"], True, brc == 0)
        return rc
    side = "base" if args.base else "target"
    rec = base if args.base else tgt
    if side not in g:
        die(f"no {side} side for '{args.fn}'")
    print(f"[basic blocks: {side.upper()} - {rec['name']}]")
    print(f"[{rec['file']} @ {rec['rva']:#x} ({rec['size']}B)]")
    print(show_blocks(g[side], args.lite), end="")
    return 0


def cmd_branches(args):
    tgt, base, g = graphs_for(args.fn, need_both=args.diff)
    if args.diff:
        print(f"[{tgt['name']}]")
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


def cmd_dot(args):
    tgt, base, g = graphs_for(args.fn, need_both=args.diff)
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


def _load_side(side):
    """{mangled: record} for one whole side (one 85 MB pass)."""
    out = {}
    with open(_index_path(side), encoding="utf-8", errors="replace") as fh:
        for line in fh:
            rec = json.loads(line)
            out.setdefault(rec["mangled"], rec)
    return out


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
    print("[BRANCH-COUNT/TOPOLOGY/COND-FLIP = steerable SHAPE work: a missing guard, "
          "an inverted condition, a folded `if`.]")
    return 0


def main():
    ap = argparse.ArgumentParser(
        description=__doc__.split("\n\n")[0],
        formatter_class=argparse.RawDescriptionHelpFormatter, epilog=__doc__)
    sub = ap.add_subparsers(dest="cmd", required=True)
    for name, fn in (("blocks", cmd_blocks), ("branches", cmd_branches), ("dot", cmd_dot)):
        p = sub.add_parser(name)
        p.add_argument("fn", help="mangled name, demangled substring, or hex RVA/VA")
        p.add_argument("--base", action="store_true", help="show the base side (default: target)")
        p.add_argument("--diff", action="store_true", help="compare base against target")
        p.add_argument("--lite", action="store_true", help="skeleton only, no bodies")
        p.set_defaults(func=fn)
    p = sub.add_parser("sweep")
    p.add_argument("--module")
    p.add_argument("--unit")
    p.add_argument("--min-pct", type=float)
    p.add_argument("--max", type=int)
    p.set_defaults(func=cmd_sweep)
    args = ap.parse_args()
    sys.exit(args.func(args))


if __name__ == "__main__":
    main()
