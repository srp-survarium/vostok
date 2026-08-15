"""vostok.sema.rva - the address/source/match dossier for one function.

The first command of an investigation: where each side lives, how big it is,
how many statements it carries, and what the match cache knows about it
(current %, hash-scoped max, structure class, attempts, flags).
"""

from __future__ import annotations

import sqlite3

from vostok.core.paths import MATCH_DB as DB_PATH

from vostok.sema import die
from vostok.sema.index import resolve


def _print_record(side, rec):
    print(f"{side:6}  rva={rec['rva']:#x}  size={rec.get('size', 0):#x}  "
          f"stmts={len(rec.get('statements', []))}")
    print(f"        {rec.get('file', '(unknown file)')}")
    print(f"        {rec['name']}")
    print(f"        {rec['mangled']}")


def cmd_rva(args):
    target, base = resolve(args.fn)
    if target is None and base is None:
        die(f"no function matches '{args.fn}'")
    if target:
        _print_record("target", target)
    if base:
        _print_record("base", base)
    if not DB_PATH.is_file():
        return 0
    con = sqlite3.connect(f"file:{DB_PATH}?mode=ro", uri=True)
    con.row_factory = sqlite3.Row
    rows = con.execute(
        """
        SELECT t.module, u.name AS unit, f.path AS file, p.fuzzy_pct,
               p.struct_class, p.t_stmts, p.b_stmts,
               (SELECT group_concat(flag || coalesce(':' || cause, ''), '; ')
                  FROM flags WHERE mangled = s.mangled) AS flags,
               a.n AS attempts, m.max_fuzzy_pct, m.exact_proven
          FROM target_functions t
          JOIN symbols s ON s.id = t.sym
          LEFT JOIN units u ON u.id = t.unit
          LEFT JOIN files f ON f.id = t.file
          LEFT JOIN pairs p ON p.target_rva = t.rva
          LEFT JOIN attempts a ON a.mangled = s.mangled
          LEFT JOIN source_maxima m ON m.mangled = s.mangled
         WHERE t.rva = ? OR p.base_rva = ? ORDER BY t.rva
        """,
        (
            target["rva"] if target else -1,
            base["rva"] if base else -1,
        ),
    ).fetchall()
    con.close()
    for row in rows:
        pct = "-" if row["fuzzy_pct"] is None else f"{row['fuzzy_pct']:.2f}%"
        maximum = "-" if row["max_fuzzy_pct"] is None else f"{row['max_fuzzy_pct']:.2f}%"
        print(f"match   module={row['module']}  unit={row['unit'] or '-'}")
        print(f"        current={pct}  max={maximum}  class={row['struct_class'] or '-'}  "
              f"statements={row['t_stmts'] or '-'}:{row['b_stmts'] or '-'}  "
              f"attempts={row['attempts'] or 0}")
        if row["flags"]:
            print(f"        flags={row['flags']}")
    return 0
