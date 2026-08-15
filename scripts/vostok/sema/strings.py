"""vostok.sema.strings - the literals a function reaches, in both directions.

`strings <fn>` lists what one function references; `--find <text>` is the
reverse lookup that turns a remembered message into a set of candidate
functions. MSVC truncates long literals in the `??_C@` symbol, so a decoded
prefix is a hint, never a claim about the whole string.
"""

from __future__ import annotations

import re

from vostok.sema import die
from vostok.sema.index import _records, _side_record


RE_LITERAL = re.compile(r"\?\?_C@[^\s,\]\)]+")


def _literal_symbols(rec):
    result = []
    for insn in rec.get("instructions", []):
        result.extend(RE_LITERAL.findall(insn.get("text", "")))
    return result


def _decode_literal(symbol):
    """Decode the readable prefix carried by an MSVC ``??_C@`` symbol.

    Long literals are truncated by the compiler, so this is deliberately a hint,
    not a claim that the returned text is complete.
    """
    fields = symbol.split("@")
    if len(fields) < 3:
        return symbol
    payload = fields[-2] if fields[-1] == "" else fields[-1]
    out = []
    i = 0
    digit_escapes = {
        "0": ",", "1": "/", "2": "\\", "3": ":", "4": ".",
        "5": " ", "6": "\n", "7": "\t", "8": "'", "9": "-",
    }
    while i < len(payload):
        if payload.startswith("?$", i) and i + 3 < len(payload):
            hi, lo = payload[i + 2], payload[i + 3]
            if "A" <= hi <= "P" and "A" <= lo <= "P":
                value = (ord(hi) - ord("A")) * 16 + ord(lo) - ord("A")
                if value:
                    out.append(chr(value))
                i += 4
                continue
        if payload[i] == "?" and i + 1 < len(payload):
            decoded = digit_escapes.get(payload[i + 1])
            if decoded is not None:
                out.append(decoded)
                i += 2
                continue
        out.append(payload[i])
        i += 1
    return "".join(out)


def cmd_strings(args):
    side = "base" if args.base else "target"
    if args.find is not None:
        needle = args.find.casefold()
        hits = []
        for rec in _records(side):
            literals = sorted(set(_literal_symbols(rec)))
            matched = [s for s in literals
                       if needle in _decode_literal(s).casefold() or needle in s.casefold()]
            if matched:
                hits.append((rec, matched))
        print(f"{side} functions referencing string '{args.find}'")
        for rec, literals in hits:
            hints = ", ".join(repr(_decode_literal(s)) for s in literals)
            print(f"  {rec['rva']:#010x}  {rec['file']}  {rec['name']}  [{hints}]")
        return 0
    if not args.fn:
        die("strings requires <fn> or --find <text>")
    _, rec = _side_record(args)
    literals = sorted(set(_literal_symbols(rec)))
    print(f"{side} strings of {rec['name']} ({rec['rva']:#x})")
    for symbol in literals:
        print(f"  {repr(_decode_literal(symbol)):<38}  {symbol}")
    return 0
