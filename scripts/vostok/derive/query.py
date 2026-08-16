"""vostok.derive.query - the flat listing views.

`list` answers "which functions, filtered how", over the three presence classes
(paired, target-only, base-only); `max` lists the hash-scoped MAX evidence
specifically, kept separate from ordinary history on purpose; `sql` is the
read-only escape hatch. All three print through one column-aligned renderer.
"""

import json
import sqlite3
import sys

from vostok.core.paths import MATCH_DB as DB_PATH
from vostok.derive.db import open_db, staleness_check


def parse_size(text):
    return int(text, 0)


def emit(rows, as_json):
    """rows: list of dicts (insertion-ordered keys)."""
    if as_json:
        print(json.dumps(rows, indent=1))
        return
    if not rows:
        print("(no rows)", file=sys.stderr)
        return
    cols = list(rows[0].keys())
    widths = [max(len(c), *(len(str(r[c] if r[c] is not None else "-")) for r in rows)) for c in cols]
    print("  ".join(c.ljust(w) for c, w in zip(cols, widths)))
    for r in rows:
        print("  ".join(str(r[c] if r[c] is not None else "-").ljust(w) for c, w in zip(cols, widths)))


def cmd_list(args):
    con = open_db(check_schema=True)
    staleness_check(con)
    where, params = [], []
    if args.presence == "PAIRED" or args.presence is None:
        base = """
          SELECT s.demangled, s.mangled, u.name AS unit, t.module,
                 t.size AS t_size, b.size AS b_size,
                 p.fuzzy_pct, p.struct_class,
                 p.t_stmts, p.b_stmts, p.n_size_rows, p.n_trgt_only, p.n_base_only,
                 printf('0x%x', t.rva) AS target_va_hint
          FROM pairs p
          JOIN symbols s ON s.id = p.sym
          JOIN target_functions t ON t.rva = p.target_rva
          JOIN base_functions   b ON b.rva = p.base_rva
          LEFT JOIN units u ON u.id = t.unit"""
        size_col = "t.size"
        name_col = "s.mangled"
        # module/unit exist on t, b AND u here - qualify or SQLite errors "ambiguous"
        module_col, unit_col = "t.module", "u.name"
    elif args.presence == "TARGET_ONLY":
        base = "SELECT demangled, mangled, unit, module, size, n_stmts FROM target_only"
        size_col = "size"
        name_col = "mangled"
        module_col, unit_col = "module", "unit"
    elif args.presence == "BASE_ONLY":
        base = """SELECT b.demangled, b.mangled, b.unit, b.module, b.size, b.n_stmts,
                         coalesce(fl.flag, st.status) AS status,
                         coalesce(fl.cause, st.detail) AS detail
                  FROM base_only b
                  LEFT JOIN base_only_status st ON st.mangled = b.mangled
                  LEFT JOIN flags fl ON fl.mangled = b.mangled
                                    AND fl.flag = 'OUT_OF_SCOPE'"""
        size_col = "b.size"
        name_col = "b.mangled"
        module_col, unit_col = "b.module", "b.unit"
    else:
        sys.exit(f"[match_db] unknown --presence {args.presence}")

    if args.module:
        where.append(f"{module_col} = ?")
        params.append(args.module)
    if args.unit:
        where.append(f"{unit_col} = ?")
        params.append(args.unit)
    if args.max_size is not None:
        where.append(f"{size_col} <= ?")
        params.append(args.max_size)
    if args.struct_class:
        classes = [c.strip().upper() for c in args.struct_class.split(",")]
        where.append(f"struct_class IN ({','.join('?' * len(classes))})")
        params.extend(classes)
    if args.status:
        if args.presence != "BASE_ONLY":
            sys.exit("[match_db] --status only applies to --presence BASE_ONLY")
        where.append("st.status = ?")
        params.append(args.status.upper())
    q = base + ((" WHERE " + " AND ".join(where)) if where else "")
    q += f" ORDER BY {size_col}, {name_col}"
    rows = [dict(r) for r in con.execute(q, params)]
    emit(rows, args.json)


def cmd_max(args):
    """List correctness-facing MAX rows, separate from ordinary history."""
    con = open_db(check_schema=True)
    staleness_check(con)
    where, params = [], []
    if args.module:
        where.append("m.module = ?")
        params.append(args.module)
    if args.below is not None:
        where.append("m.max_fuzzy_pct < ?")
        params.append(args.below)
    q = """
      SELECT s.demangled, m.mangled, m.module,
             round(m.max_fuzzy_pct, 4) AS max_fuzzy_pct,
             m.exact_proven, m.effective_hash, m.state_id, m.origin, m.evidence
      FROM source_maxima m
      LEFT JOIN symbols s ON s.mangled = m.mangled
    """
    if where:
        q += " WHERE " + " AND ".join(where)
    q += " ORDER BY m.max_fuzzy_pct, m.module, m.mangled"
    rows = [dict(row) for row in con.execute(q, params)]
    con.close()
    emit(rows, args.json)


def cmd_sql(args):
    con = sqlite3.connect(f"file:{DB_PATH}?mode=ro", uri=True)
    con.row_factory = sqlite3.Row
    rows = [dict(r) for r in con.execute(args.query)]
    emit(rows, args.json)
