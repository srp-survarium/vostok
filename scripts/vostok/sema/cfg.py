"""vostok.sema.cfg - the control-flow graph, and the two things it is for.

`cfg()` turns one side's instruction list into basic blocks whose branch
destinations are named by BLOCK INDEX - the step that makes the two sides
comparable at all, because a uniform displacement shift then compares EQUAL
while a genuine retarget does not.

`contract()` is the other half, and the reason the view is readable: the leader
rule starts a block after every branch, so an alignment pad or a spill reload
becomes a block that carries no control flow. One such block on one side would
otherwise rename every later block and print as a cascade of retargets.
"""

from __future__ import annotations

import bisect
import difflib
import re

from vostok.sema import die
from vostok.sema.disasm import disasm, parse
from vostok.sema.index import resolve


RE_TARGET = re.compile(r"(?:^|[\s,\[])(\.\d+)\s*$")


def mnem(text):
    return text.split(" ", 1)[0].lower()


def is_branch(m):
    return m.startswith("j") or m.startswith("loop")


def is_ret(m):
    return m.startswith("ret") or m == "iret"


def cfg(insns, labels, stmts=None, stats=None):
    """[(off, [masked insns], term, src)] with every branch destination named by
    BLOCK INDEX, which is what makes the two sides comparable.

    Leaders = block starts = every label target PLUS every post-branch/post-ret
    instruction. That second rule matters: without it a jump-table's dispatch
    arms (which carry no label) get absorbed into the preceding block and their
    terminators vanish. It also manufactures blocks that carry no control flow at
    all, which `contract` then removes - see there."""
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
    out, n = contract(trim_tail(out))
    if stats is not None:
        stats["contracted"] = n
    return out


def succs(term):
    return {int(x) for x in re.findall(r"B(\d+)", term or "")}


def fall_of(term):
    """The index a terminator falls through to, or None."""
    m = re.search(r"fall B(\d+)", term or "")
    return int(m.group(1)) if m else None


def contract(blocks):
    """Splice out blocks that carry NO control flow, and say how many.

    The leader rule starts a new block after every branch, so the instruction
    following a `jcc` always begins one - even when it is an alignment `nop`, a
    `lea ecx,[ecx]` pad, a spill reload or a re-materialised zero. Such a block
    has ONE predecessor (reached by that fall-through), ONE successor, and no
    branch of its own: it contributes nothing to the shape. When one side has it
    and the other does not - or has it somewhere else - the two CFGs are still
    isomorphic, but every index-named destination after it shifts and the whole
    comparison reads as a cascade of retargeted branches. That cost batch B7 real
    time on `effect_options_descriptor::operator[]` (271 bytes and 23 blocks on
    BOTH sides; the "missing early-out" was our own next block, displaced by a
    one-byte `nop`).

    A block is spliced out only when

      * its exit is a bare `fall` to the next block,
      * exactly one block reaches it, and reaches it by FALL-THROUGH, and
      * that predecessor's other successor is not the same block we would merge
        into (which would leave a terminator with two edges to one block).

    The fall-through requirement is what keeps real structure: an `if`/`else`
    body entered by the TAKEN edge of its guard is never contracted, so a missing
    branch arm still shows. Contraction can never change the branch COUNT - it
    only ever removes blocks that have no branch - so it cannot mask a missing
    guard either.

    The elided instructions are prepended to the successor, which is where they
    physically sit, so the block listing stays a correct linear disassembly."""
    n = 0
    for _ in range(len(blocks)):
        for i in range(1, len(blocks) - 1):
            if (blocks[i][2] or "") != f"fall B{i + 1}":
                continue
            preds = [j for j, b in enumerate(blocks) if i in succs(b[2])]
            if len(preds) != 1 or fall_of(blocks[preds[0]][2]) != i:
                continue
            if i + 1 in succs(blocks[preds[0]][2]):
                continue                        # would double an edge
            off, body, _, src = blocks[i]
            _, nbody, nterm, nsrc = blocks[i + 1]
            blocks[i + 1] = (off, body + nbody, nterm, src or nsrc)
            del blocks[i]
            remap = {j: (j if j <= i else j - 1) for j in range(len(blocks) + 1)}
            blocks = [(o, bd, _renumber(t, remap, k), s)
                      for k, (o, bd, t, s) in enumerate(blocks)]
            n += 1
            break
        else:
            break
    return blocks, n


def _renumber(term, remap, own):
    """Rewrite a terminator's block indices through `remap`, recomputing the `^`
    back-edge marker against the block's new index."""
    def repl(m):
        j = remap[int(m.group(1))]
        return f"B{j}" + ("^" if j <= own else "")
    return re.sub(r"B(\d+)\^?", repl, term or "")


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


INVERSE = {"je": "jne", "jz": "jnz", "ja": "jbe", "jae": "jb", "jg": "jle",
           "jge": "jl", "js": "jns", "jo": "jno", "jp": "jnp"}


SIGNED_TWIN = {"jl": "jb", "jle": "jbe", "jg": "ja", "jge": "jae"}


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


def graphs_for(sel, need_both=True):
    tgt, base = resolve(sel)
    if need_both and not (tgt and base):
        missing = "base" if tgt else "target"
        die(f"'{sel}' has no {missing} side "
            f"({'TARGET_ONLY - nothing compiled yet' if tgt else 'BASE_ONLY - not in the original'})")
    g, cut = {}, {}
    for side, rec in (("target", tgt), ("base", base)):
        if rec:
            insns, labels, stmts = parse(disasm(side, rec["rva"]))
            st = {}
            g[side] = cfg(insns, labels, stmts, st)
            cut[side] = st.get("contracted", 0)
    return tgt, base, g, cut


def _contract_note(cut):
    if not any(cut.values()):
        return None
    return ("[contracted %s flow-free block(s) (alignment pad / spill reload / "
            "re-materialised zero) - see `contract` in vostok/sema/cfg.py]"
            % " / ".join(f"{n} {s}" for s, n in sorted(cut.items()) if n))


INVERSE.update({v: k for k, v in list(INVERSE.items())})


SIGNED_TWIN.update({v: k for k, v in list(SIGNED_TWIN.items())})
