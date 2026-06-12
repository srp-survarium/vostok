#!/usr/bin/env python3
"""match_db - sqlite queue/report database over the delink/diff artifacts.

Design: docs/binary_matching/match_db_design.md. The DB answers BULK questions
(queues, per-TU reports, unpaired functions); pdb_fetch stays the authoritative
per-function view. Derived tables are rebuilt by `refresh` from:

  binaries/objdiff/report.json          per-TU roster + fuzzy %s
  binaries/rich/target/index.jsonl      exe-level target inventory + statements
  binaries/rich/base/index.jsonl        same for our build
  binaries/rich/target/declarations.jsonl   PDB declaration records (optional)

Names are interned (symbols/units/files tables, ids assigned in sorted order)
so the committed DB stays small and refreshes are byte-deterministic. The
persistent tables (history, flags) are keyed by mangled TEXT - ids are NOT
stable across refreshes, mangled names are.

Usage:
  python3 scripts/match_db.py refresh
  python3 scripts/match_db.py list --module game_core --max-size 0x80 [--json]
  python3 scripts/match_db.py list --module network_core --presence TARGET_ONLY
  python3 scripts/match_db.py report [--module game_core] [--per-unit]
  python3 scripts/match_db.py sql "SELECT ... "          # read-only
"""

import argparse
import difflib
import hashlib
import json
import os
import sqlite3
import sys
from pathlib import Path

VOSTOK = Path(__file__).resolve().parent.parent
DB_PATH = VOSTOK / "docs" / "binary_matching" / "match.db"
REPORT = VOSTOK / "binaries" / "objdiff" / "report.json"
TARGET_IDX = VOSTOK / "binaries" / "rich" / "target" / "index.jsonl"
BASE_IDX = VOSTOK / "binaries" / "rich" / "base" / "index.jsonl"
DECLARATIONS = VOSTOK / "binaries" / "rich" / "target" / "declarations.jsonl"

SCHEMA = """
CREATE TABLE meta(key TEXT PRIMARY KEY, value TEXT);

-- interned names (ids dense, assigned in sorted order; NOT stable across refreshes)
CREATE TABLE symbols(id INTEGER PRIMARY KEY, mangled TEXT UNIQUE, demangled TEXT);
CREATE TABLE units(id INTEGER PRIMARY KEY, name TEXT UNIQUE, module TEXT);
CREATE TABLE files(id INTEGER PRIMARY KEY, path TEXT UNIQUE);

-- inventories (fully derived)
CREATE TABLE target_functions(
  rva INTEGER PRIMARY KEY, sym INTEGER REFERENCES symbols(id),
  unit INTEGER REFERENCES units(id), file INTEGER REFERENCES files(id),
  line INTEGER, size INTEGER, n_stmts INTEGER);
CREATE TABLE base_functions(
  rva INTEGER PRIMARY KEY, sym INTEGER REFERENCES symbols(id),
  unit INTEGER REFERENCES units(id), file INTEGER REFERENCES files(id),
  line INTEGER, size INTEGER, n_stmts INTEGER);
CREATE TABLE unit_functions(
  unit INTEGER REFERENCES units(id), sym INTEGER REFERENCES symbols(id),
  fuzzy_pct REAL, PRIMARY KEY(unit, sym)) WITHOUT ROWID;
CREATE TABLE declared_functions(
  class TEXT, name TEXT, signature TEXT, access TEXT,
  is_virtual INTEGER, is_static INTEGER, is_const INTEGER, kind TEXT);

-- pairing + structure classification (derived)
CREATE TABLE pairs(
  sym INTEGER PRIMARY KEY REFERENCES symbols(id),
  target_rva INTEGER REFERENCES target_functions(rva),
  base_rva   INTEGER REFERENCES base_functions(rva),
  fuzzy_pct REAL,
  struct_class TEXT,   -- MATCH | SIZE | SPLIT | QUANTITY
  t_stmts INTEGER, b_stmts INTEGER,
  n_size_rows INTEGER, n_trgt_only INTEGER, n_base_only INTEGER) WITHOUT ROWID;

-- persistent (carried across refreshes; keyed by mangled TEXT for stability)
CREATE TABLE history(
  mangled TEXT PRIMARY KEY,
  last_paired_at TEXT, best_fuzzy_pct REAL, last_fuzzy_pct REAL,
  last_struct_class TEXT, src_fingerprint TEXT);
CREATE TABLE flags(
  mangled TEXT, flag TEXT, cause TEXT, set_at TEXT,
  PRIMARY KEY(mangled, flag));

CREATE INDEX idx_target_sym ON target_functions(sym);
CREATE INDEX idx_base_sym   ON base_functions(sym);
CREATE INDEX idx_target_unit ON target_functions(unit);
CREATE INDEX idx_base_unit   ON base_functions(unit);

CREATE VIEW target_only AS
  SELECT s.mangled, s.demangled, u.name AS unit, u.module, f.path AS file,
         t.rva, t.line, t.size, t.n_stmts
  FROM target_functions t
  JOIN symbols s ON s.id = t.sym
  LEFT JOIN units u ON u.id = t.unit
  LEFT JOIN files f ON f.id = t.file
  LEFT JOIN pairs p ON p.sym = t.sym WHERE p.sym IS NULL;
CREATE VIEW base_only AS
  SELECT s.mangled, s.demangled, u.name AS unit, u.module, f.path AS file,
         b.rva, b.line, b.size, b.n_stmts
  FROM base_functions b
  JOIN symbols s ON s.id = b.sym
  LEFT JOIN units u ON u.id = b.unit
  LEFT JOIN files f ON f.id = b.file
  LEFT JOIN pairs p ON p.sym = b.sym WHERE p.sym IS NULL;
CREATE VIEW paired AS
  SELECT s.mangled, s.demangled, u.name AS unit, u.module,
         p.fuzzy_pct, p.struct_class, p.t_stmts, p.b_stmts,
         p.n_size_rows, p.n_trgt_only, p.n_base_only,
         t.rva AS target_rva, t.size AS target_size,
         b.rva AS base_rva,   b.size AS base_size
  FROM pairs p
  JOIN symbols s ON s.id = p.sym
  JOIN target_functions t ON t.rva = p.target_rva
  JOIN base_functions   b ON b.rva = p.base_rva
  LEFT JOIN units u ON u.id = t.unit;
"""


def log(msg):
    print(f"[match_db] {msg}", file=sys.stderr)


def module_of(unit_or_file):
    parts = unit_or_file.split("/")
    if parts[0] == "vostok" and len(parts) > 2:
        return parts[1]
    return parts[0]  # third-party root, or a shared vostok/*.h -> "vostok"


def load_index(path):
    """rich index.jsonl -> {mangled: record} (lowest rva wins on duplicates)."""
    out = {}
    with open(path, encoding="utf-8") as f:
        for line in f:
            rec = json.loads(line)
            key = rec["mangled"]
            prev = out.get(key)
            if prev is None or rec["rva"] < prev["rva"]:
                out[key] = rec
    return out


def stmt_seq(rec):
    """Alignment alphabet: statement sizes only. Source LINE numbers (and their
    deltas) legitimately differ between target and reconstructed base
    (blank lines, comments), so they must not enter the comparison."""
    return [s["size"] for s in rec["statements"]]


def classify(t_rec, b_rec):
    """Approximate structure classification for queue ranking (the authoritative
    per-function verdict stays `pdb_fetch --view structure-diff`)."""
    t_seq, b_seq = stmt_seq(t_rec), stmt_seq(b_rec)
    t_n, b_n = len(t_seq), len(b_seq)
    if t_seq == b_seq:
        return "MATCH", t_n, b_n, 0, 0, 0

    sm = difflib.SequenceMatcher(a=t_seq, b=b_seq, autojunk=False)
    n_size = n_trgt_only = n_base_only = 0
    for op, i1, i2, j1, j2 in sm.get_opcodes():
        if op == "replace":
            paired = min(i2 - i1, j2 - j1)
            n_size += paired
            n_trgt_only += (i2 - i1) - paired
            n_base_only += (j2 - j1) - paired
        elif op == "delete":
            n_trgt_only += i2 - i1
        elif op == "insert":
            n_base_only += j2 - j1

    if t_n != b_n:
        cls = "QUANTITY"
    elif n_trgt_only or n_base_only:
        cls = "SPLIT"  # equal counts, but alignment left unpairable rows
    else:
        cls = "SIZE"
    return cls, t_n, b_n, n_size, n_trgt_only, n_base_only


def src_fingerprint(rec):
    """Hash of the function's source extent (file + statement line range text).

    Hashes SOURCE TEXT, not bytes: a matcher edit changes it; a header/other-unit
    change that only shifts codegen does not.
    """
    path = VOSTOK / "sources" / rec["file"]
    lines = [s["line"] for s in rec["statements"] if s.get("line")]
    if not lines or not path.is_file():
        return None
    lo, hi = min(lines), max(lines)
    try:
        with open(path, encoding="latin-1") as f:
            text = "".join(f.readlines()[lo - 1 : hi])
    except OSError:
        return None
    return hashlib.sha1(f"{rec['file']}:{lo}:{text}".encode("latin-1")).hexdigest()


def file_mtime_iso(path):
    import datetime

    ts = os.stat(path).st_mtime
    return datetime.datetime.fromtimestamp(ts, datetime.timezone.utc).strftime(
        "%Y-%m-%dT%H:%M:%SZ"
    )


def open_db(path=DB_PATH, must_exist=True):
    if must_exist and not Path(path).is_file():
        sys.exit(f"[match_db] no database at {path} - run `match_db.py refresh` first")
    con = sqlite3.connect(path)
    con.row_factory = sqlite3.Row
    return con


class Interner:
    def __init__(self):
        self.map = {}

    def __call__(self, key):
        if key is None:
            return None
        return self.map.setdefault(key, None)  # id assigned later, sorted

    def freeze(self):
        for i, key in enumerate(sorted(self.map), start=1):
            self.map[key] = i
        return self.map


def cmd_refresh(args):
    for required in (REPORT, TARGET_IDX, BASE_IDX):
        if not required.is_file():
            sys.exit(f"[match_db] missing {required} - run rebuild.py / the delink first")

    # stale-target guard: the target index should not predate the base one by ages
    t_mtime, b_mtime = os.stat(TARGET_IDX).st_mtime, os.stat(BASE_IDX).st_mtime
    if b_mtime - t_mtime > 7 * 86400:
        log(
            "WARNING: target index is >7 days older than base - a stale target "
            "side smears scores across modules; consider "
            "`generate_delink.py target` (see per-worktree staleness notes)"
        )

    log("loading rich indexes ...")
    target = load_index(TARGET_IDX)
    base = load_index(BASE_IDX)
    log(f"  target: {len(target)} functions, base: {len(base)} functions")

    log("loading report.json ...")
    report = json.loads(REPORT.read_text())
    report_fns = []  # (unit, mangled, fuzzy)
    units_by_mangled = {}
    for unit in report["units"]:
        uname = unit["name"]
        for fn in unit["functions"]:
            report_fns.append((uname, fn["name"], fn.get("fuzzy_match_percent")))
            units_by_mangled.setdefault(fn["name"], []).append(uname)
    for units in units_by_mangled.values():
        units.sort()

    # interners: collect every name, then assign ids in sorted order
    syms, units_i, files_i = Interner(), Interner(), Interner()
    demangled_by_mangled = {}
    for side in (target, base):
        for mangled, rec in side.items():
            syms(mangled)
            demangled_by_mangled[mangled] = rec["name"]
            files_i(rec["file"])
    for uname, mangled, _f in report_fns:
        syms(mangled)
        units_i(uname)
        demangled_by_mangled.setdefault(mangled, None)
    for rec in list(target.values()) + list(base.values()):
        units = units_by_mangled.get(rec["mangled"])
        unit = rec["file"] if units and rec["file"] in units else (units[0] if units else None)
        if unit:
            units_i(unit)
    sym_id = syms.freeze()
    unit_id = units_i.freeze()
    file_id = files_i.freeze()

    def side_rows(side):
        rows = []
        for mangled, rec in side.items():
            units = units_by_mangled.get(mangled)
            unit = rec["file"] if units and rec["file"] in units else (units[0] if units else None)
            stmts = rec["statements"]
            line = min((s["line"] for s in stmts if s.get("line")), default=None)
            rows.append(
                (
                    rec["rva"],
                    sym_id[mangled],
                    unit_id.get(unit),
                    file_id[rec["file"]],
                    line,
                    rec["size"],
                    len(stmts),
                )
            )
        rows.sort()
        return rows

    log("classifying structure for paired functions ...")
    fuzzy_by_mangled = {}
    for _u, mangled, fuzzy in report_fns:
        if fuzzy is not None:
            prev = fuzzy_by_mangled.get(mangled)
            fuzzy_by_mangled[mangled] = fuzzy if prev is None else max(prev, fuzzy)
    pair_rows = []
    for mangled in sorted(set(target) & set(base)):
        cls, t_n, b_n, n_size, n_tonly, n_bonly = classify(target[mangled], base[mangled])
        pair_rows.append(
            (
                sym_id[mangled],
                target[mangled]["rva"],
                base[mangled]["rva"],
                fuzzy_by_mangled.get(mangled),
                cls,
                t_n,
                b_n,
                n_size,
                n_tonly,
                n_bonly,
            )
        )
    pair_rows.sort()

    unit_rows = sorted(
        {
            (unit_id[u], sym_id[m]): f
            for u, m, f in sorted(report_fns, key=lambda r: (r[0], r[1], r[2] is not None, r[2] or 0))
        }.items()
    )
    unit_rows = [(u, s, f) for (u, s), f in unit_rows]

    decl_rows = []
    if DECLARATIONS.is_file():
        log("loading declaration records ...")
        with open(DECLARATIONS, encoding="utf-8") as f:
            for line in f:
                d = json.loads(line)
                decl_rows.append(
                    (
                        d.get("class"),
                        d["name"],
                        d.get("signature"),
                        d.get("access"),
                        int(bool(d.get("is_virtual"))),
                        int(bool(d.get("is_static"))),
                        int(bool(d.get("is_const"))),
                        d.get("kind", "method"),
                    )
                )
        decl_rows.sort(key=lambda r: (r[0] or "", r[1], r[2] or ""))
    else:
        log("no declarations.jsonl - declared_functions left empty (parser dump pending)")

    # carry persistent tables forward from the existing DB
    old_history, old_flags = [], []
    if DB_PATH.is_file():
        old = open_db()
        old_history = [tuple(r) for r in old.execute("SELECT * FROM history ORDER BY mangled")]
        old_flags = [tuple(r) for r in old.execute("SELECT * FROM flags ORDER BY mangled, flag")]
        old.close()

    # build fresh in a temp file, then atomically replace (deterministic bytes)
    tmp = DB_PATH.with_suffix(".db.tmp")
    tmp.unlink(missing_ok=True)
    con = sqlite3.connect(tmp)
    con.executescript("PRAGMA page_size=4096;" + SCHEMA)
    con.executemany(
        "INSERT INTO symbols VALUES (?,?,?)",
        sorted((i, m, demangled_by_mangled.get(m)) for m, i in sym_id.items()),
    )
    con.executemany(
        "INSERT INTO units VALUES (?,?,?)",
        sorted((i, u, module_of(u)) for u, i in unit_id.items()),
    )
    con.executemany("INSERT INTO files VALUES (?,?)", sorted((i, p) for p, i in file_id.items()))
    con.executemany("INSERT INTO target_functions VALUES (?,?,?,?,?,?,?)", side_rows(target))
    con.executemany("INSERT INTO base_functions VALUES (?,?,?,?,?,?,?)", side_rows(base))
    con.executemany("INSERT INTO unit_functions VALUES (?,?,?)", unit_rows)
    con.executemany("INSERT INTO pairs VALUES (?,?,?,?,?,?,?,?,?,?)", pair_rows)
    con.executemany("INSERT INTO declared_functions VALUES (?,?,?,?,?,?,?,?)", decl_rows)
    con.executemany("INSERT INTO history VALUES (?,?,?,?,?,?)", old_history)
    con.executemany("INSERT INTO flags VALUES (?,?,?,?)", old_flags)
    # deterministic meta only (artifact mtimes, not wall clock)
    con.executemany(
        "INSERT INTO meta VALUES (?,?)",
        [
            ("target_index_mtime", file_mtime_iso(TARGET_IDX)),
            ("base_index_mtime", file_mtime_iso(BASE_IDX)),
            ("report_mtime", file_mtime_iso(REPORT)),
            ("schema_version", "1"),
        ],
    )
    con.commit()
    con.execute("VACUUM")
    con.close()
    os.replace(tmp, DB_PATH)

    log(
        f"refreshed {DB_PATH.relative_to(VOSTOK)}: "
        f"{len(target)} target / {len(base)} base / {len(pair_rows)} paired / "
        f"{len(decl_rows)} declared"
    )


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
    con = open_db()
    where, params = [], []
    if args.presence == "PAIRED" or args.presence is None:
        base = """
          SELECT s.demangled, s.mangled, u.name AS unit, u.module,
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
    elif args.presence == "TARGET_ONLY":
        base = "SELECT demangled, mangled, unit, module, size, n_stmts FROM target_only"
        size_col = "size"
    elif args.presence == "BASE_ONLY":
        base = "SELECT demangled, mangled, unit, module, size, n_stmts FROM base_only"
        size_col = "size"
    else:
        sys.exit(f"[match_db] unknown --presence {args.presence}")

    if args.module:
        where.append("module = ?")
        params.append(args.module)
    if args.unit:
        where.append("unit = ?")
        params.append(args.unit)
    if args.max_size is not None:
        where.append(f"{size_col} <= ?")
        params.append(args.max_size)
    if args.struct_class:
        classes = [c.strip().upper() for c in args.struct_class.split(",")]
        where.append(f"struct_class IN ({','.join('?' * len(classes))})")
        params.extend(classes)
    q = base + ((" WHERE " + " AND ".join(where)) if where else "")
    q += f" ORDER BY {size_col}, mangled"
    rows = [dict(r) for r in con.execute(q, params)]
    emit(rows, args.json)


def cmd_report(args):
    con = open_db()
    scope = "unit_name" if args.per_unit else "module"
    where, params = "", []
    if args.module:
        where = "WHERE tf.module = ?"
        params = [args.module]
    q = f"""
      WITH tf AS (
        SELECT t.rva, t.sym, t.size,
               coalesce(un.module, '(no unit)') AS module,
               coalesce(un.name, '(no unit)')   AS unit_name
        FROM target_functions t LEFT JOIN units un ON un.id = t.unit)
      SELECT tf.{scope} AS scope,
             count(*)                                            AS target_fns,
             coalesce(sum(p.sym IS NOT NULL), 0)                 AS paired,
             coalesce(sum(p.fuzzy_pct >= 100), 0)                AS fuzzy_100,
             coalesce(sum(p.struct_class = 'MATCH'), 0)          AS struct_match,
             coalesce(sum(p.sym IS NULL), 0)                     AS target_only,
             printf('%.2f', sum(coalesce(p.fuzzy_pct, 0) * tf.size) /
                            sum(tf.size))                        AS weighted_pct
      FROM tf
      LEFT JOIN pairs p ON p.sym = tf.sym
      {where}
      GROUP BY scope ORDER BY scope
    """
    rows = [dict(r) for r in con.execute(q, params)]
    # base-only lint per scope
    bq = """SELECT coalesce({col}, '(no unit)') AS scope, count(*) AS base_only
            FROM base_only {where} GROUP BY scope""".format(
        col="unit" if args.per_unit else "module",
        where=("WHERE module = ?" if args.module else ""),
    )
    bonly = {r["scope"]: r["base_only"] for r in con.execute(bq, params)}
    for r in rows:
        r["base_only"] = bonly.get(r["scope"], 0)
    emit(rows, args.json)


def cmd_sql(args):
    con = sqlite3.connect(f"file:{DB_PATH}?mode=ro", uri=True)
    con.row_factory = sqlite3.Row
    rows = [dict(r) for r in con.execute(args.query)]
    emit(rows, args.json)


def main():
    ap = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter
    )
    sub = ap.add_subparsers(dest="cmd", required=True)
    sub.add_parser("refresh", help="rebuild derived tables from the delink/diff artifacts")

    p = sub.add_parser("list", help="list functions with filters")
    p.add_argument("--module")
    p.add_argument("--unit", help="TU path, e.g. vostok/game_core/sources/weapon_core.cpp")
    p.add_argument("--max-size", type=parse_size, help="max target size (0x.. ok)")
    p.add_argument("--class", dest="struct_class", help="csv of MATCH,SIZE,SPLIT,QUANTITY")
    p.add_argument(
        "--presence",
        choices=["PAIRED", "TARGET_ONLY", "BASE_ONLY"],
        help="default PAIRED",
    )
    p.add_argument("--json", action="store_true")

    p = sub.add_parser("report", help="per-module/TU rollup")
    p.add_argument("--module")
    p.add_argument("--per-unit", action="store_true")
    p.add_argument("--json", action="store_true")

    p = sub.add_parser("sql", help="read-only SQL escape hatch")
    p.add_argument("query")
    p.add_argument("--json", action="store_true")

    args = ap.parse_args()
    {"refresh": cmd_refresh, "list": cmd_list, "report": cmd_report, "sql": cmd_sql}[args.cmd](args)


if __name__ == "__main__":
    main()
