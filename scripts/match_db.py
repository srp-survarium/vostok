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
  module TEXT,  -- from the unit when report.json knows the symbol, else the file
  line INTEGER, size INTEGER, n_stmts INTEGER,
  frameless INTEGER);  -- no push ebp/mov ebp,esp prologue: LTCG-customized leaf
                       -- (custom calling conv, this-in-eax) - out of scope as a
                       -- standalone match, it only pairs inlined into callers
CREATE TABLE base_functions(
  rva INTEGER PRIMARY KEY, sym INTEGER REFERENCES symbols(id),
  unit INTEGER REFERENCES units(id), file INTEGER REFERENCES files(id),
  module TEXT,
  line INTEGER, size INTEGER, n_stmts INTEGER,
  frameless INTEGER);
CREATE TABLE unit_functions(
  unit INTEGER REFERENCES units(id), sym INTEGER REFERENCES symbols(id),
  fuzzy_pct REAL, PRIMARY KEY(unit, sym)) WITHOUT ROWID;

-- pairing + structure classification (derived)
CREATE TABLE pairs(
  sym INTEGER PRIMARY KEY REFERENCES symbols(id),
  target_rva INTEGER REFERENCES target_functions(rva),
  base_rva   INTEGER REFERENCES base_functions(rva),
  fuzzy_pct REAL,
  struct_class TEXT,   -- MATCH | SIZE | SPLIT | QUANTITY
  t_stmts INTEGER, b_stmts INTEGER,
  n_size_rows INTEGER, n_trgt_only INTEGER, n_base_only INTEGER) WITHOUT ROWID;

-- BASE_ONLY taxonomy (derived at refresh; design: declaration-grounded)
CREATE TABLE base_only_status(
  mangled TEXT PRIMARY KEY,
  status TEXT,            -- NEAR_MISS | JITTER | INLINED_IN_TARGET | UNEXPLAINED
  detail TEXT);           -- NEAR_MISS: the target-side mangled it likely is

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
  SELECT s.mangled, s.demangled, u.name AS unit, t.module, f.path AS file,
         t.rva, t.line, t.size, t.n_stmts
  FROM target_functions t
  JOIN symbols s ON s.id = t.sym
  LEFT JOIN units u ON u.id = t.unit
  LEFT JOIN files f ON f.id = t.file
  LEFT JOIN pairs p ON p.sym = t.sym WHERE p.sym IS NULL;
CREATE VIEW base_only AS
  SELECT s.mangled, s.demangled, u.name AS unit, b.module, f.path AS file,
         b.rva, b.line, b.size, b.n_stmts
  FROM base_functions b
  JOIN symbols s ON s.id = b.sym
  LEFT JOIN units u ON u.id = b.unit
  LEFT JOIN files f ON f.id = b.file
  LEFT JOIN pairs p ON p.sym = b.sym WHERE p.sym IS NULL;
CREATE VIEW paired AS
  SELECT s.mangled, s.demangled, u.name AS unit, t.module,
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


_OPERATOR_PLACEHOLDERS = [
    ("operator<<", "operator\x01"),
    ("operator>>", "operator\x02"),
    ("operator<=", "operator\x03"),
    ("operator>=", "operator\x04"),
    ("operator->", "operator\x05"),
    ("operator<", "operator\x06"),
    ("operator>", "operator\x07"),
    ("operator()", "operator\x08"),
]


def qualified_name(demangled):
    """Demangled signature -> (class_or_None, name); None for compiler-generated
    names (thunks, backtick names) that have no source declaration."""
    if demangled.startswith("[thunk]") or "`" in demangled:
        return None
    s = demangled
    for op, ph in _OPERATOR_PLACEHOLDERS:
        s = s.replace(op, ph)
    depth = 0
    cut = None
    for i, c in enumerate(s):
        if c == "<":
            depth += 1
        elif c == ">":
            depth -= 1
        elif c == "(" and depth == 0:
            cut = i
            break
    head = s[:cut] if cut is not None else s
    # qualified name = last space-separated token at angle-depth 0
    depth = 0
    token_start = 0
    for i, c in enumerate(head):
        if c == "<":
            depth += 1
        elif c == ">":
            depth -= 1
        elif c == " " and depth == 0:
            token_start = i + 1
    qual = head[token_start:]
    # split class::name at the last depth-0 '::'
    depth = 0
    split = None
    i = 0
    while i < len(qual):
        c = qual[i]
        if c == "<":
            depth += 1
        elif c == ">":
            depth -= 1
        elif c == ":" and depth == 0 and i + 1 < len(qual) and qual[i + 1] == ":":
            split = i
            i += 1
        i += 1
    def restore(t):
        for op, ph in _OPERATOR_PLACEHOLDERS:
            t = t.replace(ph, op)
        return t
    if split is None:
        return None, restore(qual)
    return restore(qual[:split]), restore(qual[split + 2 :])


def norm_name(text):
    return text.replace(" ", "") if text else text


def mangled_name_part(mangled):
    """The qualified-name portion of an MSVC-mangled symbol (everything before
    the first '@@', where the access/convention/type encoding starts)."""
    i = mangled.find("@@")
    return mangled[:i] if i > 0 else mangled


def is_framed(rec):
    """True when the function keeps the /Od `push ebp; mov ebp, esp` prologue.
    A frameless function in a matchable module is an LTCG-customized leaf
    (custom calling convention, e.g. this-in-eax) - never source-steerable as a
    standalone symbol; it only matters inlined into its callers."""
    ins = rec.get("instructions") or []
    return (
        len(ins) >= 2
        and ins[0]["text"].split()[:2] == ["push", "ebp"]
        and ins[1]["text"].replace(" ", "").startswith("movebp,esp")
    )


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


SCHEMA_VERSION = "2"


def open_db(path=DB_PATH, must_exist=True, check_schema=False):
    if must_exist and not Path(path).is_file():
        sys.exit(f"[match_db] no database at {path} - run `match_db.py refresh` first")
    con = sqlite3.connect(path)
    con.row_factory = sqlite3.Row
    if check_schema:
        try:
            v = con.execute("SELECT value FROM meta WHERE key='schema_version'").fetchone()
        except sqlite3.OperationalError:
            v = None
        if v is None or v[0] != SCHEMA_VERSION:
            sys.exit(
                f"[match_db] DB schema {v[0] if v else '?'} != {SCHEMA_VERSION} - "
                "run `match_db.py refresh` first"
            )
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
                    module_of(unit or rec["file"]),
                    line,
                    rec["size"],
                    len(stmts),
                    0 if is_framed(rec) else 1,
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

    # declaration records: loaded TRANSIENTLY (222k rows / 85MB - never stored
    # in the committed DB; only the per-function base_only_status verdict is)
    declared_methods, declared_free = set(), set()
    if DECLARATIONS.is_file():
        log("loading declaration records (transient) ...")
        with open(DECLARATIONS, encoding="utf-8") as f:
            for line in f:
                d = json.loads(line)
                if d.get("class"):
                    declared_methods.add((norm_name(d["class"]), norm_name(d["name"])))
                else:
                    declared_free.add(norm_name(d["name"]))
    else:
        log("no declarations.jsonl - BASE_ONLY legitimacy check degraded (parser dump pending)")

    # carry persistent tables forward from the existing DB
    old_history, old_flags = {}, []
    if DB_PATH.is_file():
        old = open_db()
        old_history = {r["mangled"]: tuple(r) for r in old.execute("SELECT * FROM history")}
        old_flags = [tuple(r) for r in old.execute("SELECT * FROM flags ORDER BY mangled, flag")]
        old.close()

    # reconcile history: upsert every CURRENT pairing; reset rows whose source
    # extent changed ("touched" - requirement: the seen-flag dies on any edit)
    paired_at = file_mtime_iso(BASE_IDX)  # artifact time, not wall clock (determinism)
    sym_by_id = {}
    for mangled, i in sym_id.items():
        sym_by_id[i] = mangled
    touched = dropped = 0
    paired_mangled = set(target) & set(base)
    for row in pair_rows:
        mangled = sym_by_id[row[0]]
        fuzzy, cls = row[3], row[4]
        fp = src_fingerprint(base[mangled])
        prev = old_history.get(mangled)
        best = fuzzy
        if prev is not None and prev[5] == fp and prev[2] is not None:
            best = prev[2] if fuzzy is None else max(prev[2], fuzzy)
        elif prev is not None and prev[5] != fp:
            touched += 1  # source edited: history restarts at the current state
        old_history[mangled] = (mangled, paired_at, best, fuzzy, cls, fp)
    # rows for functions that are NOT currently paired: keep only while their
    # source extent is unchanged - a touched-then-vanished function re-queues
    for mangled in list(old_history):
        if mangled in base and mangled not in paired_mangled:
            if old_history[mangled][5] != src_fingerprint(base[mangled]):
                del old_history[mangled]
                dropped += 1
    history_rows = sorted(old_history.values())
    if touched or dropped:
        log(f"history: {touched} touched (reset), {dropped} touched-and-vanished (re-queued)")

    # BASE_ONLY taxonomy (design: declaration-grounded)
    log("classifying base-only symbols ...")
    target_only_parts = {}
    for mangled in target:
        if mangled not in paired_mangled:
            target_only_parts.setdefault(mangled_name_part(mangled), mangled)
    declared_stems = {
        (cls.split("<", 1)[0], name.split("<", 1)[0]) for cls, name in declared_methods
    }
    bos_rows = []
    counts = {}
    for mangled in sorted(set(base) - paired_mangled):
        rec = base[mangled]
        near = target_only_parts.get(mangled_name_part(mangled))
        qn = qualified_name(rec["name"])
        if qn is None or mangled.startswith("??__") or "?A0x" in mangled:
            # thunks, backtick names, dynamic initializers/finalizers, anon-ns
            status, detail = "COMPILER", None
        elif rec["file"].endswith("temp_include_all.cpp"):
            status, detail = "ANCHOR", None  # our reachability scaffolding
        elif near is not None:
            status, detail = "NEAR_MISS", near
        elif mangled in old_history:
            status, detail = "JITTER", None
        elif declared_methods or declared_free:
            cls, name = qn
            cls_n, name_n = norm_name(cls), norm_name(name)
            if (cls and (cls_n, name_n) in declared_methods) or (
                not cls and name_n in declared_free
            ):
                status, detail = "INLINED_IN_TARGET", None
            elif cls and (cls_n.split("<", 1)[0], name_n.split("<", 1)[0]) in declared_stems:
                # instantiation of a declared template (dump and demangler
                # render template args differently; the stem is the signal)
                status, detail = "TEMPLATE", None
            else:
                status, detail = "UNEXPLAINED", None
        else:
            status, detail = "UNEXPLAINED", None  # no declarations dump to consult
        bos_rows.append((mangled, status, detail))
        counts[status] = counts.get(status, 0) + 1
    log(f"  base-only: {counts}")

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
    con.executemany("INSERT INTO target_functions VALUES (?,?,?,?,?,?,?,?,?)", side_rows(target))
    con.executemany("INSERT INTO base_functions VALUES (?,?,?,?,?,?,?,?,?)", side_rows(base))
    con.executemany("INSERT INTO unit_functions VALUES (?,?,?)", unit_rows)
    con.executemany("INSERT INTO pairs VALUES (?,?,?,?,?,?,?,?,?,?)", pair_rows)
    con.executemany("INSERT INTO base_only_status VALUES (?,?,?)", bos_rows)
    con.executemany("INSERT INTO history VALUES (?,?,?,?,?,?)", history_rows)
    con.executemany("INSERT INTO flags VALUES (?,?,?,?)", old_flags)
    # deterministic meta only (artifact mtimes, not wall clock)
    con.executemany(
        "INSERT INTO meta VALUES (?,?)",
        [
            ("target_index_mtime", file_mtime_iso(TARGET_IDX)),
            ("base_index_mtime", file_mtime_iso(BASE_IDX)),
            ("report_mtime", file_mtime_iso(REPORT)),
            ("declarations_loaded", "1" if (declared_methods or declared_free) else "0"),
            ("schema_version", "2"),
        ],
    )
    con.commit()
    con.execute("VACUUM")
    con.close()
    os.replace(tmp, DB_PATH)

    log(
        f"refreshed {DB_PATH.relative_to(VOSTOK)}: "
        f"{len(target)} target / {len(base)} base / {len(pair_rows)} paired / "
        f"{len(bos_rows)} base-only classified"
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
    con = open_db(check_schema=True)
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
        name_col = "s.mangled"
    elif args.presence == "TARGET_ONLY":
        base = "SELECT demangled, mangled, unit, module, size, n_stmts FROM target_only"
        size_col = "size"
        name_col = "mangled"
    elif args.presence == "BASE_ONLY":
        base = """SELECT b.demangled, b.mangled, b.unit, b.module, b.size, b.n_stmts,
                         st.status, st.detail
                  FROM base_only b LEFT JOIN base_only_status st ON st.mangled = b.mangled"""
        size_col = "b.size"
        name_col = "b.mangled"
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
    if args.status:
        if args.presence != "BASE_ONLY":
            sys.exit("[match_db] --status only applies to --presence BASE_ONLY")
        where.append("st.status = ?")
        params.append(args.status.upper())
    q = base + ((" WHERE " + " AND ".join(where)) if where else "")
    q += f" ORDER BY {size_col}, {name_col}"
    rows = [dict(r) for r in con.execute(q, params)]
    emit(rows, args.json)


def cmd_report(args):
    con = open_db(check_schema=True)
    scope = "unit_name" if args.per_unit else "module"
    where, params = "", []
    if args.module:
        where = "WHERE tf.module = ?"
        params = [args.module]
    q = f"""
      WITH tf AS (
        SELECT t.rva, t.sym, t.size, t.frameless,
               coalesce(t.module, '(no unit)') AS module,
               coalesce(un.name, '(no unit)')  AS unit_name
        FROM target_functions t LEFT JOIN units un ON un.id = t.unit)
      SELECT tf.{scope} AS scope,
             count(*)                                            AS target_fns,
             coalesce(sum(p.sym IS NOT NULL), 0)                 AS paired,
             coalesce(sum(p.fuzzy_pct >= 100), 0)                AS fuzzy_100,
             coalesce(sum(p.struct_class = 'MATCH'), 0)          AS struct_match,
             coalesce(sum(p.sym IS NULL), 0)                     AS target_only,
             coalesce(sum(tf.frameless), 0)                      AS custom_conv,
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
    # out_of_scope: target-only functions whose history row survived (paired
    # once, vanished without a source touch)
    oq = """
      SELECT coalesce({col}, '(no unit)') AS scope, count(*) AS n FROM target_only
      WHERE mangled IN (SELECT mangled FROM history) {extra} GROUP BY scope
    """.format(
        col="unit" if args.per_unit else "module",
        extra=("AND module = ?" if args.module else ""),
    )
    oos = {r["scope"]: r["n"] for r in con.execute(oq, params)}
    # the fabricated-symbol lint: base-only rows nothing explains
    uq = """
      SELECT coalesce({col}, '(no unit)') AS scope, count(*) AS n
      FROM base_only b JOIN base_only_status st ON st.mangled = b.mangled
      WHERE st.status IN ('UNEXPLAINED', 'NEAR_MISS') {extra} GROUP BY scope
    """.format(
        col="unit" if args.per_unit else "module",
        extra=("AND module = ?" if args.module else ""),
    )
    suspicious = {r["scope"]: r["n"] for r in con.execute(uq, params)}
    for r in rows:
        r["base_only"] = bonly.get(r["scope"], 0)
        r["suspicious"] = suspicious.get(r["scope"], 0)
        r["out_of_scope"] = oos.get(r["scope"], 0)
    emit(rows, args.json)


def queue_host_units(con, module):
    """Header pseudo-units are a delink artifact (the PDB attributes inline
    methods to the header), not real TUs - fold their functions into a HOST
    .cpp TU: stem match first (x.h / x_inline.h -> sources/x.cpp), then the
    .cpp unit owning most functions of the same class; genuinely header-only
    classes stay standalone, labeled."""
    cpps = {
        r["name"]
        for r in con.execute(
            "SELECT name FROM units WHERE module = ? AND name LIKE '%.cpp'", (module,)
        )
    }

    def stem(path):
        b = path.rsplit("/", 1)[-1].rsplit(".", 1)[0]
        return b[:-7] if b.endswith("_inline") else b

    stem_to_cpp = {}
    for c in sorted(cpps):
        stem_to_cpp.setdefault(stem(c), c)
    class_owner = {}
    q = """SELECT u.name AS unit, s.demangled FROM target_functions t
           JOIN symbols s ON s.id = t.sym JOIN units u ON u.id = t.unit
           WHERE t.module = ? AND u.name LIKE '%.cpp'"""
    for r in con.execute(q, (module,)):
        qn = qualified_name(r["demangled"] or "")
        if qn and qn[0]:
            counts = class_owner.setdefault(norm_name(qn[0]), {})
            counts[r["unit"]] = counts.get(r["unit"], 0) + 1

    def host(unit, demangled):
        if unit.endswith(".cpp"):
            return unit
        c = stem_to_cpp.get(stem(unit))
        if c:
            return c
        qn = qualified_name(demangled or "")
        if qn and qn[0]:
            counts = class_owner.get(norm_name(qn[0]))
            if counts:
                return max(sorted(counts), key=lambda u: counts[u])
        return f"{unit} (header-only)"

    return host


def cmd_queue(args):
    """One batch per TU: a matcher owns the WHOLE TU, so small helpers are
    matched in their real context (same inlining/LTCG environment as their
    callers) instead of as cross-TU small-function churn. Header pseudo-units
    fold into their host .cpp TU; frameless (LTCG-customized) leaves are
    skipped. TUs are ordered by total open bytes, smallest first."""
    con = open_db(check_schema=True)
    where, params = ["module = ?"], [args.module]
    if not args.include_frameless:
        where.append("frameless = 0")
    q = f"""
      WITH cand AS (
        SELECT s.demangled, s.mangled, coalesce(u.name,'(no unit)') AS unit,
               t.module, t.size, t.frameless, p.fuzzy_pct, p.struct_class,
               CASE WHEN p.sym IS NULL THEN 'TARGET_ONLY' ELSE 'PAIRED' END AS presence
        FROM target_functions t
        JOIN symbols s ON s.id = t.sym
        LEFT JOIN units u ON u.id = t.unit
        LEFT JOIN pairs p ON p.sym = t.sym
        LEFT JOIN history h ON h.mangled = s.mangled
        WHERE NOT (coalesce(p.fuzzy_pct, 0) >= 100 AND p.struct_class = 'MATCH')
          AND s.mangled NOT IN (SELECT mangled FROM flags WHERE flag IN ('SKIP','OUT_OF_SCOPE'))
          -- seen-before, vanished without a source touch: external inline/link
          -- decision, out of scope (design: history does the classifying)
          AND NOT (p.sym IS NULL AND h.mangled IS NOT NULL)
          -- matched at 100 before and untouched since: a later regression is
          -- outside this function (LTCG non-steerable) - skip
          AND NOT (coalesce(h.best_fuzzy_pct, 0) >= 100 AND coalesce(p.fuzzy_pct, 0) < 100)
      )
      SELECT * FROM cand WHERE {" AND ".join(where)}
      ORDER BY unit, size
    """
    host = queue_host_units(con, args.module)
    by_unit = {}
    for r in con.execute(q, params):
        by_unit.setdefault(host(r["unit"], r["demangled"]), []).append(dict(r))
    units = sorted(by_unit.items(), key=lambda kv: (sum(f["size"] for f in kv[1]), kv[0]))
    if args.limit:
        units = units[: args.limit]

    if args.json:
        out = [
            {
                "unit": unit,
                "total_size": sum(f["size"] for f in fns),
                "functions": [
                    {k: f[k] for k in ("demangled", "mangled", "unit", "size", "fuzzy_pct", "struct_class", "presence")}
                    for f in fns
                ],
            }
            for unit, fns in units
        ]
        print(json.dumps(out, indent=1))
        return
    for unit, fns in units:
        total = sum(f["size"] for f in fns)
        print(f"=== {unit}: {len(fns)} functions, {total:#x} bytes")
        for f in fns:
            pct = "-" if f["fuzzy_pct"] is None else f"{f['fuzzy_pct']:.1f}"
            via = "" if f["unit"] == unit else f"   [defined in {f['unit']}]"
            print(
                f"  {f['size']:>6}  {pct:>6}  {f['struct_class'] or f['presence']:<11}  "
                f"{f['demangled'][:110]}{via}"
            )


def cmd_sql(args):
    con = sqlite3.connect(f"file:{DB_PATH}?mode=ro", uri=True)
    con.row_factory = sqlite3.Row
    rows = [dict(r) for r in con.execute(args.query)]
    emit(rows, args.json)


def cmd_flag(args):
    import datetime

    con = open_db()
    today = datetime.date.today().isoformat()
    if args.requeue:
        # manual override of the history-derived out-of-scope/matched-before
        # skip: forget the function's history + flags so queues offer it again
        n = con.execute("DELETE FROM history WHERE mangled = ?", (args.mangled,)).rowcount
        m = con.execute("DELETE FROM flags WHERE mangled = ?", (args.mangled,)).rowcount
        log(f"requeued {args.mangled}: dropped {n} history row(s), {m} flag(s)")
    else:
        if not args.cause:
            sys.exit("[match_db] --cause is required when setting a flag")
        con.execute(
            "INSERT OR REPLACE INTO flags VALUES (?,?,?,?)",
            (args.mangled, args.flag, args.cause, today),
        )
        log(f"flagged {args.mangled} {args.flag}")
    con.commit()


def cmd_merge_flags(args):
    con = open_db()
    other = sqlite3.connect(f"file:{args.other}?mode=ro", uri=True)
    other.row_factory = sqlite3.Row
    nf = nh = 0
    for r in other.execute("SELECT * FROM flags"):
        nf += con.execute(
            "INSERT OR IGNORE INTO flags VALUES (?,?,?,?)",
            (r["mangled"], r["flag"], r["cause"], r["set_at"]),
        ).rowcount
    for r in other.execute("SELECT * FROM history"):
        cur = con.execute(
            "SELECT last_paired_at, best_fuzzy_pct FROM history WHERE mangled = ?",
            (r["mangled"],),
        ).fetchone()
        newer = cur is None or (r["last_paired_at"] or "") > (cur["last_paired_at"] or "")
        if newer:
            con.execute(
                "INSERT OR REPLACE INTO history VALUES (?,?,?,?,?,?)",
                tuple(r),
            )
            nh += 1
        elif cur and r["best_fuzzy_pct"] and (cur["best_fuzzy_pct"] or 0) < r["best_fuzzy_pct"]:
            con.execute(
                "UPDATE history SET best_fuzzy_pct = ? WHERE mangled = ?",
                (r["best_fuzzy_pct"], r["mangled"]),
            )
            nh += 1
    con.commit()
    log(f"merged from {args.other}: {nf} flags, {nh} history rows")


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
    p.add_argument(
        "--status",
        help="BASE_ONLY taxonomy filter: NEAR_MISS|JITTER|INLINED_IN_TARGET|UNEXPLAINED|COMPILER",
    )
    p.add_argument("--json", action="store_true")

    p = sub.add_parser("report", help="per-module/TU rollup")
    p.add_argument("--module")
    p.add_argument("--per-unit", action="store_true")
    p.add_argument("--json", action="store_true")

    p = sub.add_parser("queue", help="one batch per TU (all its open functions), small-first")
    p.add_argument("--module", required=True)
    p.add_argument("--limit", type=int, help="show only the first N TUs")
    p.add_argument(
        "--include-frameless",
        action="store_true",
        help="also queue LTCG-customized (frameless) leaves - normally pointless",
    )
    p.add_argument("--json", action="store_true")

    p = sub.add_parser("sql", help="read-only SQL escape hatch")
    p.add_argument("query")
    p.add_argument("--json", action="store_true")

    p = sub.add_parser("flag", help="manual override: set a flag or requeue")
    p.add_argument("mangled")
    g = p.add_mutually_exclusive_group(required=True)
    g.add_argument("--flag", choices=["OUT_OF_SCOPE", "SKIP"])
    g.add_argument(
        "--requeue",
        action="store_true",
        help="forget history+flags so queues offer the function again",
    )
    p.add_argument("--cause")

    p = sub.add_parser("merge-flags", help="union persistent tables from another match.db")
    p.add_argument("other")

    args = ap.parse_args()
    {
        "refresh": cmd_refresh,
        "list": cmd_list,
        "report": cmd_report,
        "queue": cmd_queue,
        "sql": cmd_sql,
        "flag": cmd_flag,
        "merge-flags": cmd_merge_flags,
    }[args.cmd](args)


if __name__ == "__main__":
    main()
