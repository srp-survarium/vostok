"""vostok.derive.db - the cache file itself: schema, opening, staleness.

`binaries/match.db` is a REGENERABLE cache, not truth (the committed ledger is
`docs/binary_matching/match_state.tsv`). Everything here treats it that way: the
schema is recreated wholesale on every refresh, the interned ids are dense and
sorted so a refresh is byte-deterministic, and only the persistent tables
(history, flags, attempts, source_maxima) are carried across.

`staleness_check` is the guard that matters day to day: it compares the git HEAD
recorded when report.json was BUILT against the current sources/, so a queue can
refuse to dispatch already-done work instead of quietly serving stale rows.
"""

import os
import sqlite3
import sys
from pathlib import Path

from vostok.core.paths import MATCH_DB as DB_PATH
from vostok.core.paths import REPO as VOSTOK
from vostok.core.paths import REPORT
from vostok.derive import log


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
CREATE TABLE source_maxima(
  mangled TEXT PRIMARY KEY,
  effective_hash TEXT NOT NULL,
  max_fuzzy_pct REAL NOT NULL,
  exact_proven INTEGER NOT NULL,
  state_id TEXT,
  module TEXT,
  source_file TEXT, source_lo INTEGER, source_hi INTEGER,
  origin TEXT NOT NULL, evidence TEXT);
-- Valuable observations from inactive source/compiler epochs.  The active
-- source_maxima row remains the only row consumed by score queries; refresh
-- resurrects an archived row only when its effective hash becomes current
-- again.
CREATE TABLE source_maxima_epochs(
  mangled TEXT,
  effective_hash TEXT,
  max_fuzzy_pct REAL NOT NULL,
  exact_proven INTEGER NOT NULL,
  state_id TEXT,
  module TEXT,
  source_file TEXT, source_lo INTEGER, source_hi INTEGER,
  origin TEXT NOT NULL, evidence TEXT,
  PRIMARY KEY(mangled, effective_hash)) WITHOUT ROWID;
CREATE TABLE flags(
  mangled TEXT, flag TEXT, cause TEXT, set_at TEXT,
  PRIMARY KEY(mangled, flag));
CREATE TABLE attempts(
  mangled TEXT PRIMARY KEY,
  n INTEGER,            -- how many matcher dispatches included this function
  last_at TEXT, note TEXT);

CREATE INDEX idx_target_sym ON target_functions(sym);
CREATE INDEX idx_base_sym   ON base_functions(sym);
CREATE INDEX idx_target_unit ON target_functions(unit);
CREATE INDEX idx_base_unit   ON base_functions(unit);
CREATE INDEX idx_pairs_target_rva ON pairs(target_rva);
CREATE INDEX idx_pairs_base_rva   ON pairs(base_rva);

-- Unpaired = the function's RVA is not the target_rva / base_rva of any pair.
-- Excluding by RVA (not by sym) also catches the cross-name dynamic-init/atexit
-- pairs, whose pair row is keyed by the TARGET sym while the base side carries a
-- different (??__E/??__F) sym - a by-sym check would leave the base twin in
-- base_only and double-count it.
CREATE VIEW target_only AS
  SELECT s.mangled, s.demangled, u.name AS unit, t.module, f.path AS file,
         t.rva, t.line, t.size, t.n_stmts
  FROM target_functions t
  JOIN symbols s ON s.id = t.sym
  LEFT JOIN units u ON u.id = t.unit
  LEFT JOIN files f ON f.id = t.file
  LEFT JOIN pairs p ON p.target_rva = t.rva WHERE p.target_rva IS NULL;
CREATE VIEW base_only AS
  SELECT s.mangled, s.demangled, u.name AS unit, b.module, f.path AS file,
         b.rva, b.line, b.size, b.n_stmts
  FROM base_functions b
  JOIN symbols s ON s.id = b.sym
  LEFT JOIN units u ON u.id = b.unit
  LEFT JOIN files f ON f.id = b.file
  LEFT JOIN pairs p ON p.base_rva = b.rva WHERE p.base_rva IS NULL;
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


def file_mtime_iso(path):
    import datetime

    ts = os.stat(path).st_mtime
    return datetime.datetime.fromtimestamp(ts, datetime.timezone.utc).strftime(
        "%Y-%m-%dT%H:%M:%SZ"
    )


SCHEMA_VERSION = "5"


def _git(*args):
    import subprocess

    try:
        out = subprocess.run(
            ["git", "-C", str(VOSTOK), *args], capture_output=True, text=True, timeout=10
        )
        return out.returncode, out.stdout.strip()
    except Exception:
        return 1, ""


def git_head():
    _, head = _git("rev-parse", "HEAD")
    return head or "?"


def staleness_check(con, strict=False):
    """Freshness has TWO legs: report.json must be BUILT from the current
    sources (`vostok build` records the HEAD it built at in report.head), and the
    DB must have ingested that report (`vostok build` regenerates it at the end of
    every build; `refresh` re-derives it regen-only). Under lowest-match-first
    ordering, stale rows for freshly-landed work look like 0% and get
    dispatched FIRST - so queue refuses on either stale leg (--stale-ok
    overrides); other commands warn. Cadence: land -> `vostok build` (regenerates
    the DB); run `refresh` by hand only to re-ingest an already-built report."""
    hard, soft = [], []
    row = con.execute("SELECT value FROM meta WHERE key='build_head'").fetchone()
    build_head = row[0] if row else None
    if not build_head or build_head.startswith("?"):
        hard.append("DB predates the staleness guard - `vostok build` + refresh first")
    else:
        if build_head.endswith("+dirty"):
            build_head = build_head[: -len("+dirty")]
            soft.append("report.json was built from a DIRTY sources/ tree")
        if build_head != git_head():
            rc, _ = _git("diff", "--quiet", build_head, "HEAD", "--", "sources/")
            if rc != 0:
                _, last = _git("log", "-1", "--format=%h %s", "--", "sources/")
                hard.append(
                    f"sources/ changed since report.json was BUILT ({build_head[:8]}..HEAD,"
                    f" last: {last}) - `vostok build` + refresh first"
                )
    if REPORT.is_file():
        row = con.execute("SELECT value FROM meta WHERE key='report_mtime'").fetchone()
        if row and row[0] != file_mtime_iso(REPORT):
            hard.append("report.json changed since the DB was refreshed - run refresh")
    _, dirty = _git("status", "--porcelain", "--", "sources/")
    if dirty:
        soft.append("uncommitted sources/ changes - rows may not reflect them")
    for m in hard + soft:
        log(f"STALE: {m}")
    if strict and hard:
        sys.exit("[match_db] stale DB - rows would dispatch already-done work (--stale-ok to override)")


def open_db(path=DB_PATH, must_exist=True, check_schema=False):
    if must_exist and not Path(path).is_file():
        sys.exit(f"[match_db] no database at {path} - run "
                 "`python3 -m vostok derive refresh` first")
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
                "run `python3 -m vostok derive refresh` first"
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


def _artifact_path(value):
    path = Path(value)
    return path if path.is_absolute() else VOSTOK / path
