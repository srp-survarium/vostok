"""vostok.sema.index - which function, on which side, at which RVA.

Everything else here takes a resolved record. One selector - a mangled name, a
demangled substring, or a hex RVA/VA on either side - is answered by ONE pass
over each side's `binaries/rich/<side>/index.jsonl` (~85 MB, ~0.05 s), then
cross-checked against the pairing the match cache recorded, so naming a target
function also finds its base twin (and the reverse) even when the two sides
spell it differently. Ambiguity is reported, never guessed at.
"""

from __future__ import annotations

import json
import re
import sqlite3
import sys

from vostok.core.paths import MATCH_DB as DB_PATH
from vostok.core.paths import RICH_DIR as RICH

from vostok.sema import die


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


def _paired_rva(side, rva):
    """Return the opposite-side RVA recorded by match.db, if paired."""
    if not DB_PATH.is_file():
        return None
    column, opposite = (
        ("target_rva", "base_rva") if side == "target" else ("base_rva", "target_rva")
    )
    try:
        con = sqlite3.connect(f"file:{DB_PATH}?mode=ro", uri=True)
        row = con.execute(
            f"SELECT {opposite} FROM pairs WHERE {column} = ?", (rva,)
        ).fetchone()
        con.close()
        return row[0] if row else None
    except sqlite3.Error:
        return None


def _record_at_rva(side, rva):
    hits = _scan_index(
        side,
        lambda line: (
            json.loads(line) if f'"rva":{rva}' in line else None
        ),
    )
    return hits[0] if len(hits) == 1 else None


def resolve(sel):
    """(target_record | None, base_record | None) for one selector.

    Ambiguity is resolved by preferring an EXACT mangled hit; anything still
    ambiguous is reported rather than guessed at."""
    want = _matcher(sel)
    tgt = _scan_index("target", want)
    base = _scan_index("base", want)
    if len(tgt) == 1:
        paired = _paired_rva("target", tgt[0]["rva"])
        if paired is not None:
            partner = _record_at_rva("base", paired)
            if partner is not None:
                base = [partner]
    elif len(base) == 1:
        paired = _paired_rva("base", base[0]["rva"])
        if paired is not None:
            partner = _record_at_rva("target", paired)
            if partner is not None:
                tgt = [partner]
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


def _records(side):
    with open(_index_path(side), encoding="utf-8", errors="replace") as fh:
        for line in fh:
            yield json.loads(line)


def _side_record(args):
    target, base = resolve(args.fn)
    side = "base" if getattr(args, "base", False) else "target"
    rec = base if side == "base" else target
    if rec is None:
        die(f"'{args.fn}' has no {side} function")
    return side, rec


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


def _load_side(side):
    """{mangled: record} for one whole side (one 85 MB pass)."""
    out = {}
    with open(_index_path(side), encoding="utf-8", errors="replace") as fh:
        for line in fh:
            rec = json.loads(line)
            out.setdefault(rec["mangled"], rec)
    return out
