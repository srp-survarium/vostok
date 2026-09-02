# SPDX-License-Identifier: GPL-3.0-or-later

"""vostok.sema.rva - the address/source/match dossier for one function.

The first command of an investigation: where each side lives, how big it is,
how many statements it carries, and what the campaign knows about it (current
%, hash-scoped max, structure class, attempts, status).

Two sources, and the split is the point. The two rich indexes own the BUILD
facts - address, size, statements, owning file, both spellings of the name. The
committed ledger (`config/match_state.tsv`) owns the CAMPAIGN's
memory - the percentages, the structure verdict, how many matchers have tried,
and why it is parked. Neither is `match.db`, so this answers on a tree that has
never run a derivation.

Both address forms are printed. RVA is what the indexes, the ledger and every
`--rva` flag hold; VA is what IDA shows and what carcass comments carry. They
differ by the 0x10000 image base, which is exactly the size of the mistake made
by pasting one where the other was wanted.
"""

from __future__ import annotations

from vostok.derive.index import body_statements
from vostok.sema import die
from vostok.sema.index import resolve, va_of
from vostok.sema.pairing import ledger_row


def _print_record(side, rec):
    print(f"{side:6}  rva={rec['rva']:#x}  va={va_of(rec):#x}  "
          f"size={rec.get('size', 0):#x}  stmts={len(body_statements(rec))}")
    print(f"        {rec.get('file', '(unknown file)')}")
    print(f"        {rec['name']}")
    print(f"        {rec['mangled']}")


def _pct(value):
    return "-" if value is None else f"{value:.2f}%"


def cmd_rva(args):
    target, base = resolve(args.fn)
    if target is None and base is None:
        die(f"no function matches '{args.fn}'")
    if target:
        _print_record("target", target)
    if base:
        _print_record("base", base)
    row = ledger_row(target) if target else None
    if row is None:
        return 0
    t_stmts = len(body_statements(target))
    b_stmts = len(body_statements(base)) if base else None
    print(f"match   module={row['module'] or '-'}  unit={row['unit'] or '-'}")
    print(f"        current={_pct(row['cur'])}  max={_pct(row['max'])}  "
          f"class={row['cls'] or '-'}  "
          f"statements={t_stmts or '-'}:{b_stmts if b_stmts else '-'}  "
          f"attempts={row['tries'] or 0}  status={row['status'] or '-'}")
    if row.get("note"):
        print(f"        note={row['note']}")
    return 0
