# SPDX-License-Identifier: GPL-3.0-or-later

"""vostok.eh.compare - pair the two images' EH registrations and diff them.

A registration pairs with the other side's by the parent function: first
through the ledger's pairing (`sema.pairing`) when the parent is a scored
function, then by any shared PDB name (mangled publics compare across builds;
procedure names cover statics). A parent with EH on one side whose function
exists without EH on the other is a presence divergence; one whose function
the other side does not have at all is unpaired.

Divergence classes, in the order a reader should fix them:

    presence      one side has a FuncInfo, the other's function has none
    max_state     number of unwind states: destructible objects + try states
    unwind_shape  the toState chain: scope nesting of those objects
    unwind_order  same destructors, constructed in a different order
    unwind_dtor   a state destroys a different thing (or nothing)
    unwind_frame  same destructor, different frame slot for the object
    try_ranges    try-block state ranges and their nesting
    catch_types   caught types, their adjectives, their order
    catch_object  where the caught object lands (dispCatchObj)
    funclet_bytes same callee and slot, other funclet bytes differ
    handler_bytes a catch handler outside the parent differs
    stub_frame    the __ehhandler stub's cookie/frame offsets differ
    flags         FuncInfo magic, EHFlags or the exception-spec list

SEH frames (`__try`, compiled with or without C++ EH) pair the same way:

    seh_scopes    number of __try scopes or their nesting (EnclosingLevel)
    seh_kind      __except vs __finally per scope
    seh_offsets   filter/handler entry offsets inside the parent
    seh_frame     SEH3 vs SEH4, or the SEH4 cookie offsets
"""

from __future__ import annotations

import dataclasses
from collections import Counter

from vostok.eh.tables import Funclet, Image, Registration, SehFrame
from vostok.ledger.store import EXACT

CLASSES = ("presence", "max_state", "unwind_shape", "unwind_order", "unwind_dtor",
           "unwind_frame", "try_ranges", "catch_types", "catch_object",
           "funclet_bytes", "handler_bytes", "stub_frame", "flags",
           "seh_scopes", "seh_kind", "seh_offsets", "seh_frame")


@dataclasses.dataclass
class Divergence:
    kind: str
    detail: str


@dataclasses.dataclass
class FuncletPair:
    target: Funclet
    base: Funclet

    @property
    def exact(self) -> bool:
        return (self.target.masked == self.base.masked
                and _ref_names(self.target) == _ref_names(self.base))


@dataclasses.dataclass
class Pair:
    name: str
    target: Registration | SehFrame | None
    base: Registration | SehFrame | None
    verdict: str  # MATCH DIFFERS TARGET_ONLY BASE_ONLY UNPAIRED
    divergences: list[Divergence]
    funclets: list[FuncletPair]
    mangled: list[str]
    kind: str = "c++"  # or "seh"
    paired_by: str | None = None  # name / ledger / shape

    def classes(self) -> list[str]:
        seen = []
        for d in self.divergences:
            if d.kind not in seen:
                seen.append(d.kind)
        return seen


def _ref_names(f: Funclet) -> list[str]:
    return [f"{r.kind}:{r.name or '?'}" for r in f.refs]


def pair_registrations(target: Image, base: Image, ledger_pairing=None) -> list[Pair]:
    """Every C++ EH registration and SEH frame on either side, paired where
    its parent pairs."""
    return (_pair(target.registrations, base.registrations, target, base, ledger_pairing,
                  compare, "c++")
            + _pair(target.seh_frames, base.seh_frames, target, base, ledger_pairing,
                    compare_seh, "seh"))


def _pair(t_items, b_items, target: Image, base: Image, ledger_pairing, cmp, kind):
    b_by_parent: dict[int, list] = {}
    b_by_name: dict[str, list] = {}
    for item in b_items:
        if item.parent_rva is not None:
            b_by_parent.setdefault(item.parent_rva, []).append(item)
        for name in item.parent_names:
            b_by_name.setdefault(name, []).append(item)
    used: set[int] = set()
    matched: list[tuple[object, object, str]] = []
    left: list = []
    for item in t_items:
        votes = Counter()
        for name in item.parent_names:
            for cand in b_by_name.get(name, ()):
                if id(cand) not in used:
                    votes[id(cand)] += 1
        partner, how = None, "name"
        if votes:
            best = max(votes.values())
            partner = next(c for c in b_items if votes.get(id(c)) == best)
        elif ledger_pairing is not None and item.parent_rva is not None:
            b_rva = ledger_pairing(item.parent_rva)
            cands = [c for c in b_by_parent.get(b_rva, ()) if id(c) not in used]
            partner, how = (cands[0] if cands else None), "ledger"
        if partner is None:
            left.append(item)
            continue
        used.add(id(partner))
        matched.append((item, partner, how))
    # A parent the PDB cannot name on one side (a CRT static our PDB has no
    # procedure record for lands on the preceding public) still pairs when its
    # tables are unique and identical on both sides' leftovers.
    b_left = [c for c in b_items if id(c) not in used]
    t_prints = Counter(_fingerprint(i) for i in left)
    b_prints = Counter(_fingerprint(i) for i in b_left)
    b_by_print = {_fingerprint(i): i for i in b_left}
    rest = []
    for item in left:
        fp = _fingerprint(item)
        if t_prints[fp] == 1 and b_prints.get(fp) == 1:
            partner = b_by_print[fp]
            used.add(id(partner))
            matched.append((item, partner, "shape"))
        else:
            rest.append(item)
    pairs = []
    for t, b, how in matched:
        pair = cmp(t, b)
        pair.paired_by = how
        if not t.parent_names and b.parent_names:
            pair.name = b.display
        pairs.append(pair)
    for item in rest:
        other = _function_on(base, item.parent_names, ledger_pairing, item.parent_rva)
        pairs.append(_one_sided(item, None, "TARGET_ONLY" if other else "UNPAIRED", kind))
    for item in b_items:
        if id(item) in used:
            continue
        other = _function_on(target, item.parent_names, None, None)
        pairs.append(_one_sided(None, item, "BASE_ONLY" if other else "UNPAIRED", kind))
    return pairs


def _fingerprint(item) -> tuple:
    """Everything a comparison looks at, independent of addresses and names."""
    if isinstance(item, SehFrame):
        return ("seh", item.flavor, item.cookies, tuple(
            (sc.enclosing, sc.kind, off) for sc, off in zip(item.scopes, _site_offsets(item))))
    info = item.info
    return ("c++", info.magic, info.eh_flags, tuple(
        (to, item.funclets[a].callee if a else None, item.funclets[a].masked if a else None)
        for to, a in info.unwind),
        tuple((tb.low, tb.high, tb.catch_high, tuple(h.spelling() for h in tb.handlers))
              for tb in info.try_blocks), item.stub.masked)


def _function_on(image: Image, names: list[str], ledger_pairing, parent_rva) -> bool:
    if ledger_pairing is not None and parent_rva is not None \
            and ledger_pairing(parent_rva) is not None:
        return True
    return any(name in image.symbols.by_name for name in names)


def _one_sided(t, b, verdict: str, kind: str) -> Pair:
    reg = t or b
    side = "target" if t else "base"
    other = "base" if t else "target"
    if kind == "seh":
        what = f"an SEH frame ({len(reg.scopes)} scopes)"
    else:
        what = f"a FuncInfo (maxState {reg.info.max_state}, {len(reg.info.try_blocks)} try)"
    detail = (f"{side} has {what} and the {other} function has none"
              if verdict != "UNPAIRED" else f"no {other} function of this name")
    divs = [Divergence("presence", detail)] if verdict != "UNPAIRED" else []
    return Pair(reg.display, t, b, verdict, divs, [], _mangled(reg), kind)


def compare_seh(t: SehFrame, b: SehFrame) -> Pair:
    divs: list[Divergence] = []
    if (t.flavor, t.cookies) != (b.flavor, b.cookies):
        divs.append(Divergence("seh_frame", f"{t.flavor} {t.cookies} vs {b.flavor} {b.cookies}"))
    t_nest = [sc.enclosing for sc in t.scopes]
    b_nest = [sc.enclosing for sc in b.scopes]
    if t_nest != b_nest:
        divs.append(Divergence("seh_scopes", f"EnclosingLevel target {t_nest} base {b_nest}"))
    t_kind = [sc.kind for sc in t.scopes]
    b_kind = [sc.kind for sc in b.scopes]
    if t_kind != b_kind:
        divs.append(Divergence("seh_kind", f"target {t_kind} base {b_kind}"))
    elif _site_offsets(t) != _site_offsets(b):
        divs.append(Divergence("seh_offsets", "(filter, handler) from the registration: "
                                              f"target {_site_offsets(t)} base "
                                              f"{_site_offsets(b)}"))
    divs.sort(key=lambda d: CLASSES.index(d.kind))
    return Pair(t.display, t, b, "DIFFERS" if divs else "MATCH", divs, [],
                _mangled(t) or _mangled(b), "seh")


def _site_offsets(frame: SehFrame) -> list[tuple[int | None, int]]:
    """Filter/handler entries relative to the frame's registration site: the
    parent attribution can be wrong on a side whose PDB lacks the procedure,
    the registration site cannot."""
    return [(None if sc.filter_rva is None else sc.filter_rva - frame.site_rva,
             sc.handler_rva - frame.site_rva) for sc in frame.scopes]


def seh_offsets(frame: SehFrame) -> list[tuple[str, str]]:
    return [(f"+{sc.filter_rva - frame.parent_rva:#x}" if sc.filter_rva is not None else "-",
             f"+{sc.handler_rva - frame.parent_rva:#x}") for sc in frame.scopes]



def _mangled(reg) -> list[str]:
    return [n for n in reg.parent_names if n.startswith("?")]


def _slot(f: Funclet) -> str:
    return f.frame or "-"


def compare(t: Registration, b: Registration) -> Pair:
    divs: list[Divergence] = []
    ti, bi = t.info, b.info
    if (ti.magic, ti.eh_flags, ti.es_types) != (bi.magic, bi.eh_flags, bi.es_types):
        divs.append(Divergence("flags", f"magic {ti.magic:#x}/{bi.magic:#x} EHFlags "
                                        f"{ti.eh_flags}/{bi.eh_flags} es {ti.es_types}/{bi.es_types}"))
    if ti.max_state != bi.max_state:
        divs.append(Divergence("max_state", f"target {ti.max_state}, base {bi.max_state}"))
    t_chain = [s for s, _ in ti.unwind]
    b_chain = [s for s, _ in bi.unwind]
    if t_chain != b_chain:
        divs.append(Divergence("unwind_shape", f"toState target {t_chain} base {b_chain}"))

    def callee(reg, action):
        return reg.funclets[action].callee if action is not None else None

    t_callees = [callee(t, a) for _, a in ti.unwind]
    b_callees = [callee(b, a) for _, a in bi.unwind]
    if t_callees != b_callees and Counter(t_callees) == Counter(b_callees):
        divs.append(Divergence("unwind_order", "same destructors, other order: target "
                                               f"{_short(t_callees)} base {_short(b_callees)}"))
    funclets: list[FuncletPair] = []
    for state in range(min(ti.max_state, bi.max_state)):
        ta, ba = ti.unwind[state][1], bi.unwind[state][1]
        if ta is None or ba is None:
            if (ta is None) != (ba is None):
                divs.append(Divergence("unwind_dtor", f"state {state}: "
                                       f"{'no action' if ta is None else callee(t, ta)} vs "
                                       f"{'no action' if ba is None else callee(b, ba)}"))
            continue
        tf, bf = t.funclets[ta], b.funclets[ba]
        funclets.append(FuncletPair(tf, bf))
        if tf.callee != bf.callee:
            if Counter(t_callees) != Counter(b_callees):
                divs.append(Divergence("unwind_dtor",
                                       f"state {state}: {tf.callee} vs {bf.callee}"))
        elif _slot(tf) != _slot(bf):
            divs.append(Divergence("unwind_frame", f"state {state} {tf.callee}: "
                                                   f"{_slot(tf)} vs {_slot(bf)}"))
        elif tf.shape != bf.shape or _ref_names(tf) != _ref_names(bf):
            divs.append(Divergence("funclet_bytes", f"state {state}: {tf.summary()} | "
                                                    f"{bf.summary()}"))
    t_try = [(tb.low, tb.high, tb.catch_high) for tb in ti.try_blocks]
    b_try = [(tb.low, tb.high, tb.catch_high) for tb in bi.try_blocks]
    if t_try != b_try:
        divs.append(Divergence("try_ranges", f"target {t_try} base {b_try}"))
    t_types = [[h.spelling() for h in tb.handlers] for tb in ti.try_blocks]
    b_types = [[h.spelling() for h in tb.handlers] for tb in bi.try_blocks]
    if t_types != b_types:
        divs.append(Divergence("catch_types", f"target {t_types} base {b_types}"))
    t_obj = [[h.disp_catch_obj for h in tb.handlers] for tb in ti.try_blocks]
    b_obj = [[h.disp_catch_obj for h in tb.handlers] for tb in bi.try_blocks]
    if t_obj != b_obj:
        divs.append(Divergence("catch_object", f"dispCatchObj target {t_obj} base {b_obj}"))
    t_out = [t.handlers.get(h.handler_rva) for tb in ti.try_blocks for h in tb.handlers]
    b_out = [b.handlers.get(h.handler_rva) for tb in bi.try_blocks for h in tb.handlers]
    for k, (tf, bf) in enumerate(zip(t_out, b_out)):
        if tf is None and bf is None:
            continue
        if tf is None or bf is None:
            divs.append(Divergence("handler_bytes", f"handler {k}: in the parent on one "
                                                    "side only"))
            continue
        funclets.append(FuncletPair(tf, bf))
        if tf.shape != bf.shape or _ref_names(tf) != _ref_names(bf):
            divs.append(Divergence("handler_bytes", f"handler {k} differs"))
    if t.stub.masked != b.stub.masked:
        divs.append(Divergence("stub_frame", f"{t.stub.summary()} | {b.stub.summary()}"))
    verdict = "DIFFERS" if divs else "MATCH"
    divs.sort(key=lambda d: CLASSES.index(d.kind))
    return Pair(t.display, t, b, verdict, divs, funclets, _mangled(t) or _mangled(b))


def _short(callees: list[str | None]) -> str:
    return "[" + ", ".join(_demangle_hint(c) for c in callees) + "]"


def _demangle_hint(name: str | None) -> str:
    """`??1foo@bar@@QAE@XZ` -> `~foo`; anything else unchanged (or `-`)."""
    if name is None:
        return "-"
    if name.startswith("??1"):
        return "~" + name[3:].split("@", 1)[0]
    if name.startswith("??3@"):
        return "delete"
    return name


@dataclasses.dataclass
class Summary:
    counts: dict
    classes: Counter
    hidden: list[Pair]


def summarize(target: Image, base: Image, pairs: list[Pair], ledger: dict) -> Summary:
    def side(image: Image) -> dict:
        regs = image.registrations
        funclets = {rva: f for r in regs for rva, f in r.funclets.items()}
        handlers = {rva: f for r in regs for rva, f in r.handlers.items()}
        return {
            "funcinfo": len(image.funcinfos),
            "stubs": len(image.stubs),
            "registrations": len(regs),
            "parents_named": sum(1 for r in regs if r.parent_names),
            "in_ledger": sum(1 for r in regs if _ledger_row(r, ledger)),
            "unwind_states": sum(i.max_state for i in image.funcinfos.values()),
            "try_blocks": sum(len(i.try_blocks) for i in image.funcinfos.values()),
            "funclets": len(funclets),
            "funclet_bytes": sum(f.size for f in funclets.values()),
            "handlers_outside_parent": len(handlers),
            "decode_errors": sum(1 for f in funclets.values() if f.error),
            "seh_frames": len(image.seh_frames),
            "seh_in_ledger": sum(1 for f in image.seh_frames if _ledger_row(f, ledger)),
            "seh_scopes": sum(len(f.scopes) for f in image.seh_frames),
            "scan_errors": list(image.errors),
        }
    verdicts = {kind: dict(Counter(p.verdict for p in pairs if p.kind == kind))
                for kind in ("c++", "seh")}
    classes = Counter(k for p in pairs for k in p.classes())
    funclet_pairs = {}
    for p in pairs:
        for fp in p.funclets:
            funclet_pairs[(fp.target.rva, fp.base.rva)] = fp
    exact = [fp for fp in funclet_pairs.values() if fp.exact]
    hidden = [p for p in pairs if p.verdict != "MATCH"
              and _peak(ledger_row(p, ledger)) >= EXACT]
    counts = {
        "target": side(target),
        "base": side(base),
        "verdicts": verdicts,
        "funclet_pairs": len(funclet_pairs),
        "funclet_pairs_exact": len(exact),
        "funclet_bytes_paired": sum(fp.target.size for fp in funclet_pairs.values()),
        "funclet_bytes_exact": sum(fp.target.size for fp in exact),
        "ledger_rows_with_eh": len({id(r) for p in pairs
                                    for r in [ledger_row(p, ledger)] if r is not None}),
        "hidden_mismatches": len(hidden),
    }
    return Summary(counts, classes, hidden)


def _ledger_row(reg, ledger: dict):
    if reg is None:
        return None
    for name in reg.parent_names:
        row = ledger.get(name)
        if row is not None:
            return row
    return None


def _peak(row) -> float:
    if not row:
        return -1.0
    return max(v for v in (row.get("cur"), row.get("max"), -1.0) if v is not None)


def ledger_row(pair: Pair, ledger: dict):
    return _ledger_row(pair.target, ledger) or _ledger_row(pair.base, ledger)
