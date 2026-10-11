# SPDX-License-Identifier: GPL-3.0-or-later

"""vostok.sema.ltcg - name the LTCG mechanism behind one function's residual.

Reads the target/base pair from the PDB evidence DBs and tags the difference
with the transformations catalogued in docs/binary_matching/ltcg.md:

    entry       the function's own register convention differs (T3)
    ret         `ret N` differs - stack cleanup moved to the caller or dropped (T3)
    marshal     a shared callee is called with different push/register setup (T4)
    calls       one side calls a callee the other inlined; the called copy's
                prologue says which optimisation class owns it (T1/T2)
    frame       stack frame size differs; never LTCG in an /Od body (T10)
    eh          one side has an fs:[0] frame, the other none (T9)
    regalloc    identical after renaming registers (T6 or scheduling noise)

and prints the function's fast-loop trust class: whether a per-TU compile
without /GL can be trusted for it (see "Fast non-LTCG loop" in ltcg.md).

    python3 -m vostok sema ltcg <fn>

ICF-folded callees are compared through their fold group (same RVA), so a call
the retail spells `finalize_impl` and we spell `~noncopyable` is one callee.
"""

from __future__ import annotations

import json
import re
import subprocess
from collections import Counter
from functools import lru_cache

from vostok.sema import die
from vostok.sema.index import _index_path, resolve

_GPR = {
    "eax": "eax", "ax": "eax", "al": "eax", "ah": "eax",
    "ebx": "ebx", "bx": "ebx", "bl": "ebx", "bh": "ebx",
    "ecx": "ecx", "cx": "ecx", "cl": "ecx", "ch": "ecx",
    "edx": "edx", "dx": "edx", "dl": "edx", "dh": "edx",
    "esi": "esi", "si": "esi", "edi": "edi", "di": "edi", "ebp": "ebp", "bp": "ebp",
}
_REG = re.compile(r"\b(e?[abcd]x|[abcd][lh]|e?[sd]i|e?bp)\b")
_WRITE_ONLY = {"mov", "lea", "movzx", "movsx", "pop", "cvttss2si", "cvtss2si",
               "cvttsd2si", "movd", "movss", "movsd", "movq"}
_NO_DEST = {"cmp", "test", "push", "comiss", "ucomiss", "comisd", "ucomisd", "bt"}
_CALLEE_SAVED = ("ebx", "esi", "edi", "ebp")
#: registers a standard convention may read at entry
STANDARD_LIVE = {"thiscall": {"ecx"}, "fastcall": {"ecx", "edx"},
                 "stdcall": set(), "cdecl": set()}
#: module directories by optimisation class (binaries/ninja/rsp, Master Gold)
OD_MODULES = {"game_core", "network_core", "logging"}
NO_OG_MODULES = {"ai", "debug", "fs", "vfs", "network", "particle"}
NO_GL_MODULES = {"sound"}


def _regs(text):
    return {_GPR[m] for m in _REG.findall(text)}


def entry_registers(texts):
    """Registers read before written, scanning straight-line from the entry.

    Stops at the first call or ret. A push of a callee-saved register that was
    not yet touched is a save, not a read."""
    written, live = set(), set()
    for text in texts:
        op, _, rest = text.strip().partition(" ")
        rest = rest.strip()
        if op in ("call", "ret", "retn"):
            break
        if op == "push" and rest in _CALLEE_SAVED and rest not in written and rest not in live:
            continue
        if op.startswith("j"):
            continue
        dst, _, src = rest.partition(",")
        dst, src = dst.strip(), src.strip()
        reads, writes = set(), set()
        if op == "xor" and dst == src and dst in _GPR:
            writes.add(_GPR[dst])
        elif op == "cdq":
            reads, writes = {"eax"}, {"edx"}
        elif op in ("mul", "imul", "div", "idiv") and not src:
            reads = _regs(dst) | {"eax"} | ({"edx"} if "div" in op else set())
            writes = {"eax", "edx"}
        elif op == "imul" and src.count(",") == 1:
            reads, writes = _regs(src), _regs(dst)
        elif op in _NO_DEST:
            reads = _regs(rest)
        elif op in _WRITE_ONLY:
            reads = _regs(src) | (_regs(dst) if "[" in dst else set())
            writes = set() if "[" in dst else _regs(dst)
        else:
            reads = _regs(rest)
            writes = set() if "[" in dst else _regs(dst)
        live |= reads - written
        written |= writes
    return live


def ret_bytes(texts):
    """Largest `ret N` immediate (0 for a bare ret), or None without a ret."""
    best = None
    for text in texts:
        op, _, rest = text.strip().partition(" ")
        if op in ("ret", "retn"):
            rest = rest.strip()
            try:
                n = int(rest.rstrip("h"), 16) if rest else 0
            except ValueError:
                continue
            best = n if best is None else max(best, n)
    return best


def call_sites(texts):
    """[(callee, pushes, registers set)] for every direct call, in order."""
    sites, window = [], []
    for text in texts:
        op, _, rest = text.strip().partition(" ")
        if op == "call":
            regs = set()
            for prev in window:
                pop, _, prest = prev.partition(" ")
                dst = prest.split(",")[0].strip()
                if pop in ("mov", "lea", "movzx", "movsx", "xor") and dst in _GPR:
                    regs.add(_GPR[dst])
                elif pop == "push" and prest.strip() in _GPR:
                    regs.discard(_GPR[prest.strip()])  # consumed as a stack argument
            sites.append((rest.strip(), sum(1 for w in window if w.startswith("push")), regs))
            window = []
        else:
            window = (window + [text.strip()])[-10:]
    return sites


def unmatched_calls(t_calls, b_calls, same):
    """Callees only one side calls, after pairing ICF-equal names via `same`."""
    t_left, b_left = Counter(t_calls) - Counter(b_calls), Counter(b_calls) - Counter(t_calls)
    for t in list(t_left):
        for b in list(b_left):
            if t_left[t] and b_left[b] and same(t, b):
                k = min(t_left[t], b_left[b])
                t_left[t] -= k
                b_left[b] -= k
    return +t_left, +b_left


def framed(texts):
    """An /Od-class body: `push ebp; mov ebp, esp` at entry (no /Og)."""
    head = [re.sub(r"\s+", " ", t.strip()) for t in texts[:2]]
    return head == ["push ebp", "mov ebp, esp"]


def owner_hint(caller_framed, kept):
    """Why a call survived on one side, from the kept copy's optimisation class.

    LTCG inlines a COMDAT only when the caller and the kept IL copy share a
    class ({/Od, no-/Og} vs /Og); see T2 in ltcg.md."""
    if not kept:
        return "callee outside the image index (CRT/import/prebuilt): never inlined"
    cross = [framed(_texts(k)) for k in kept if framed(_texts(k)) != caller_framed]
    if cross:
        cls = "/Od-class" if cross[0] else "/Og"
        return (f"kept copy is {cls}, the caller is not: owner-class barrier (T2) - "
                f"move the reference so a same-class TU owns the COMDAT")
    return ("kept copy is the caller's own class: not the owner barrier - the callee is "
            "not inline, over the inline budget (T1), or its body differs (source)")


def frame_size(texts):
    for text in texts[:12]:
        m = re.match(r"sub\s+esp,\s*([0-9A-Fa-f]+)h?$", text.strip())
        if m:
            return int(m.group(1), 16)
    return 0


def reg_canonical(texts):
    """Rename registers by first appearance (sub-register suffix kept)."""
    names = {}

    def sub(m):
        reg = _GPR[m.group(0)]
        names.setdefault(reg, f"R{len(names)}")
        return names[reg] + ("" if len(m.group(0)) == 3 else m.group(0)[-1])
    return [_REG.sub(sub, t) for t in texts]


def flag_class(path):
    """Optimisation class of the TU a function body comes from, by source path."""
    p = path.lower().replace("\\", "/")
    if re.search(r"\.(h|hpp|inl)$", p) or p.startswith(("stlport/", "boost/")):
        return "header"
    if p.startswith(("src/gfx", "src/kernel", "src/render", "zlib")):
        return "nogl"
    parts = p.split("/")
    if parts[0] == "vostok" and len(parts) > 2:
        if parts[1] in NO_GL_MODULES:
            return "nogl"
        if parts[1] in OD_MODULES:
            return "od"
        if parts[1] in NO_OG_MODULES:
            return "noog"
    return "ox"


@lru_cache(maxsize=None)
def declared_convention(mangled):
    if not mangled.startswith("?"):
        return "cdecl"
    out = subprocess.run(["llvm-undname"], input=mangled + "\n",
                         capture_output=True, text=True).stdout
    for cc in ("thiscall", "fastcall", "stdcall", "cdecl"):
        if f"__{cc}" in out:
            return cc
    return "cdecl"


def trust_class(rec, cc, callee_info):
    """(class, reasons) for a non-/GL per-TU compile of this function.

    callee_info(name) -> list of (custom_convention, header_defined) for the
    callee's fold group, or [] when it is outside LTCG (CRT, import, prebuilt)."""
    fc = flag_class(rec.get("file", ""))
    if fc == "nogl":
        return "exact (no LTCG in retail)", []
    texts = [i["text"] for i in rec.get("instructions", [])]
    reasons = set()
    if entry_registers(texts) - STANDARD_LIVE.get(cc, set()):
        reasons.add("own custom convention")
    for callee, _, _ in call_sites(texts):
        for custom, header in callee_info(callee):
            if custom:
                reasons.add("calls a custom-convention callee")
            if header:
                reasons.add("calls a header-defined function out of line (COMDAT owner class)")
    if reasons:
        return "unreliable", sorted(reasons)
    if fc in ("od", "noog"):
        return "exact", []
    return "approximate (cross-TU inlining is invisible)", []


def _qualified_name(name):
    """'ret ns::f<a *, b>(args) const' -> 'ns::f<a *, b>', the call-text spelling.

    Template-depth aware: a space inside <...> is not the return-type boundary."""
    depth, cut = 0, len(name)
    for i, ch in enumerate(name):
        depth += (ch == "<") - (ch == ">")
        if ch == "(" and depth == 0 and "operator" not in name[max(0, i - 9):i]:
            cut = i
            break
    head, depth, start = name[:cut], 0, 0
    for i, ch in enumerate(head):
        depth += (ch == "<") - (ch == ">")
        if head.startswith("operator", i) and depth == 0:
            break  # `operator bool`, `operator char const *`: the rest is the name
        if ch == " " and depth == 0 and not head[i + 1:].startswith(("*", "&")):
            start = i + 1
    return head[start:]


def _key(name):
    """Call text and PDB names space template arguments differently."""
    return name.replace(" ", "")


class _Side:
    """Fold groups and bodies of one image, keyed by call-text name.

    Names come from one SQL pass; instruction bodies are fetched per RVA on
    demand, so a lookup costs a query instead of loading every payload."""

    def __init__(self, side):
        import sqlite3
        self.db = sqlite3.connect(f"file:{_index_path(side)}?mode=ro", uri=True)
        self.by_name, self.by_rva = {}, {}
        for name, mangled, rva, file in self.db.execute(
                "SELECT name, mangled, rva, file FROM functions"):
            self.by_name.setdefault(_key(_qualified_name(name)), set()).add(rva)
            self.by_rva.setdefault(rva, []).append({"name": name, "mangled": mangled,
                                                    "rva": rva, "file": file})

    def group(self, name):
        return {_key(_qualified_name(r["name"])) for rva in self.by_name.get(_key(name), ())
                for r in self.by_rva[rva]}

    def bodies(self, name):
        out = []
        for rva in sorted(self.by_name.get(_key(name), ())):
            row = self.db.execute(
                "SELECT p.instructions_json FROM functions f JOIN function_payloads p "
                "ON p.function_id = f.id WHERE f.rva = ? LIMIT 1", (rva,)).fetchone()
            rec = dict(self.by_rva[rva][0])
            rec["instructions"] = json.loads(row[0]) if row else []
            out.append(rec)
        return out


def _texts(rec):
    return [i["text"].strip() for i in rec.get("instructions", [])]


def cmd_ltcg(args):
    target, base = resolve(args.fn)
    if target is None:
        die(f"no target function matches '{args.fn}'")
    tgt_side, base_side = _Side("target"), _Side("base")
    cc = declared_convention(target["mangled"])
    tt = _texts(target)
    print(f"target  {target['name']}\n        {target.get('file', '')}  declared __{cc}  "
          f"class={flag_class(target.get('file', ''))}")

    def callee_info(name):
        out = []
        for body in tgt_side.bodies(name):
            group = tgt_side.by_rva[body["rva"]]
            custom = bool(entry_registers(_texts(body)) -
                          STANDARD_LIVE.get(declared_convention(body["mangled"]), set()))
            header = any(flag_class(r.get("file", "")) == "header" for r in group)
            out.append((custom, header))
        return out

    if base is not None:
        bt = _texts(base)
        tags = []
        if tt == bt:
            tags.append(("identical", ""))
        else:
            tl, bl = entry_registers(tt), entry_registers(bt)
            if tl != bl:
                tags.append(("entry", f"target reads {'+'.join(sorted(tl)) or '-'}, "
                                      f"base {'+'.join(sorted(bl)) or '-'} (T3)"))
            if ret_bytes(tt) != ret_bytes(bt):
                tags.append(("ret", f"target ret {ret_bytes(tt)}, base ret {ret_bytes(bt)} (T3)"))
            ts, bs = call_sites(tt), call_sites(bt)
            t_only, b_only = unmatched_calls(
                [c for c, _, _ in ts], [c for c, _, _ in bs],
                lambda t, b: _key(b) in tgt_side.group(t) or _key(t) in base_side.group(b))
            for side, calls, img in (("target", t_only, tgt_side), ("base", b_only, base_side)):
                for callee in calls:
                    tags.append(("calls", f"only {side} calls {callee}: "
                                          f"{owner_hint(framed(tt), img.bodies(callee))}"))
            by_callee = {}
            for c, p, r in ts:
                by_callee.setdefault(c, []).append((p, r))
            for c, p, r in bs:
                if by_callee.get(c):
                    tp, tr = by_callee[c].pop(0)
                    if tp != p or (tr ^ r) - {"ecx"}:
                        tags.append(("marshal", f"{c}: target {tp} push(es) "
                                                f"{'+'.join(sorted(tr)) or '-'}, base {p} push(es) "
                                                f"{'+'.join(sorted(r)) or '-'} (T4)"))
            tf, bf = frame_size(tt), frame_size(bt)
            if tf != bf:
                why = ("an /Od frame: LTCG never pads it - extra locals/temporaries or an "
                       "inline-barrier cascade" if framed(tt) else "LTCG alignment padding "
                       "or different inlined locals")
                tags.append(("frame", f"target {tf:#x}, base {bf:#x}: {why} (T10)"))
            if any("fs:" in t for t in tt) != any("fs:" in t for t in bt):
                tags.append(("eh", "one side registers an fs:[0] frame (T9)"))
            if len(tt) == len(bt) and reg_canonical(tt) == reg_canonical(bt):
                tags.append(("regalloc", "identical after register renaming (T6 / allocation)"))
            if not tags:
                tags.append(("other", "operand, immediate or scheduling difference - "
                                      "not an LTCG transformation; check source shape"))
        for tag, text in tags:
            print(f"{tag:9} {text}")
    else:
        print("base      (no base function)")
    klass, reasons = trust_class(target, cc, callee_info)
    print(f"fastloop  {klass}" + (f": {'; '.join(reasons)}" if reasons else ""))
    return 0
