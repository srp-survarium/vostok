"""vostok.sema.xref - the direct caller/callee graph, per side.

`--callees` reads one function's call sites; the default direction scans the
whole side for functions that call it. Both work off the rendered instruction
text, so an operand is matched against a mangled name OR the demangled
qualified name, whichever the disassembly used.
"""

from __future__ import annotations

import re

from vostok.sema.index import _records, _side_record


RE_CALL = re.compile(r"^call\s+(.+?)\s*$", re.IGNORECASE)


def _qualified_name(name):
    """Drop a demangled return type and argument list, preserving scoped names."""
    head = name.split("(", 1)[0].strip()
    marker = head.find("operator ")
    if marker >= 0:
        scope = head[:marker].rsplit(" ", 1)[-1]
        return scope + head[marker:]
    return head.rsplit(" ", 1)[-1]


def _call_operand(text):
    match = RE_CALL.match(text.strip())
    return match.group(1).strip() if match else None


def _callee_rows(rec):
    rows = []
    for insn in rec.get("instructions", []):
        operand = _call_operand(insn.get("text", ""))
        if operand:
            rows.append((insn.get("off", 0), operand))
    return rows


def _matches_operand(rec, operand):
    return operand in (rec["mangled"], _qualified_name(rec["name"]))


def cmd_xref(args):
    side, selected = _side_record(args)
    if args.callees:
        rows = _callee_rows(selected)
        if not args.raw:
            counts = {}
            first = {}
            for off, operand in rows:
                counts[operand] = counts.get(operand, 0) + 1
                first.setdefault(operand, off)
            rows = [(first[name], name, counts[name]) for name in sorted(counts)]
        else:
            rows = [(off, name, 1) for off, name in rows]
        print(f"{side} callees of {selected['name']} (rva={selected['rva']:#x})")
        for off, operand, count in rows:
            suffix = f" x{count}" if count > 1 else ""
            print(f"  +0x{off:04x}  {operand}{suffix}")
        return 0

    hits = []
    for rec in _records(side):
        sites = [(off, operand) for off, operand in _callee_rows(rec)
                 if _matches_operand(selected, operand)]
        if sites:
            hits.append((rec, sites))
    print(f"{side} callers of {selected['name']} (rva={selected['rva']:#x})")
    for rec, sites in hits:
        if args.raw:
            for off, _ in sites:
                print(f"  rva={rec['rva']:#010x}+0x{off:04x}  {rec['file']}  {rec['name']}")
        else:
            print(f"  rva={rec['rva']:#010x}  calls={len(sites):<3}  {rec['file']}  {rec['name']}")
    return 0
