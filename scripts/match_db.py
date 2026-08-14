#!/usr/bin/env python3
"""match_db - sqlite queue/report database over the delink/diff artifacts.

Design: docs/binary_matching/match_db_design.md. The DB answers BULK questions
(queues, per-TU reports, unpaired functions); pdb_fetch stays the authoritative
per-function view. Derived tables are regenerated from the already-built diff
artifacts by `rebuild.py` (at the end of every build) or, regen-only, by
`refresh` (run rebuild.py first if sources moved):

  binaries/objdiff/report.json          per-TU roster + fuzzy %s
  binaries/rich/target/index.jsonl      exe-level target inventory + statements
  binaries/rich/base/index.jsonl        same for our build
  binaries/rich/target/declarations.jsonl   PDB declaration records (optional)

Names are interned (symbols/units/files tables, ids assigned in sorted order)
so the committed DB stays small and refreshes are byte-deterministic. The
persistent tables (history, flags) are keyed by mangled TEXT - ids are NOT
stable across refreshes, mangled names are.

Usage:
  python3 scripts/match_db.py refresh    # regen-only (rebuild.py first if sources moved)
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
import re
import sqlite3
import sys
from pathlib import Path

import normalize_objdiff_symbols

VOSTOK = Path(__file__).resolve().parent.parent
DB_PATH = VOSTOK / "docs" / "binary_matching" / "match.db"
REPORT = VOSTOK / "binaries" / "objdiff" / "report.json"
CROSS_UNIT_REPORT = VOSTOK / "binaries" / "objdiff" / "report-cross-unit.json"
TARGET_IDX = VOSTOK / "binaries" / "rich" / "target" / "index.jsonl"
BASE_IDX = VOSTOK / "binaries" / "rich" / "base" / "index.jsonl"
DECLARATIONS = VOSTOK / "binaries" / "rich" / "target" / "declarations.jsonl"
EXACT_FOLD_ALIASES = (
    VOSTOK / "docs" / "binary_matching" / "exact_fold_aliases.tsv"
)
MODULE_OWNERSHIP_OVERRIDES = (
    VOSTOK / "docs" / "binary_matching" / "module_ownership_overrides.tsv"
)

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


def log(msg):
    print(f"[match_db] {msg}", file=sys.stderr)


def module_of(unit_or_file):
    parts = unit_or_file.split("/")
    if parts[0] == "vostok" and len(parts) > 2:
        return parts[1]
    return parts[0]  # third-party root, or a shared vostok/*.h -> "vostok"


def load_index(path, preferred_files=None):
    """Return one deterministic rich-index record per mangled spelling."""
    return index_by_mangled(load_index_records(path), preferred_files)


def load_index_records(path):
    """Load every rich-index record, including same-RVA PDB aliases."""
    with open(path, encoding="utf-8") as f:
        return [json.loads(line) for line in f]


def overload_key(mangled, name):
    """Return a side-independent key for a PDB placeholder overload."""
    digest = hashlib.sha256(name.encode("utf-8")).hexdigest()[:16]
    return f"{mangled}@@pdb-overload:{digest}"


def index_by_mangled(records, preferred_files=None, preferred_signatures=None):
    """Collapse rich records by PDB identity, preferring the other side's owner.

    Static helpers and COMDATs can have the same PDB spelling in several
    translation units. When the other side selected an owner, prefer that same
    source file; otherwise retain the historical lowest-RVA rule. Some retail
    PDB records lose the decorated signature for overloads and expose the same
    scope-qualified placeholder as ``mangled``. Preserve each distinct full
    demangled signature in that case instead of dropping target functions.
    """
    candidates = {}
    for rec in records:
        candidates.setdefault(rec["mangled"], {}).setdefault(rec["name"], []).append(
            rec
        )
    out = {}
    preferred_files = preferred_files or {}
    preferred_signatures = preferred_signatures or {}
    for mangled, signatures in candidates.items():
        selected = []
        for name, signature_records in signatures.items():
            preferred = preferred_files.get((mangled, name), preferred_files.get(mangled))
            same_owner = [
                rec for rec in signature_records if rec["file"] == preferred
            ]
            selected.append(
                min(same_owner or signature_records, key=lambda rec: rec["rva"])
            )

        # The schema intentionally stores one canonical symbol per RVA. Keep
        # same-RVA aliases collapsed, but never collapse distinct overload
        # bodies merely because the PDB gave them the same placeholder name.
        selected_by_rva = {}
        for rec in selected:
            selected_by_rva.setdefault(rec["rva"], rec)
        ordered = sorted(selected_by_rva.values(), key=lambda rec: rec["rva"])
        primary = preferred_signatures.get(mangled)
        if primary is not None:
            ordered.sort(key=lambda rec: rec["name"] != primary)
        for index, rec in enumerate(ordered):
            key = mangled if index == 0 else overload_key(mangled, rec["name"])
            out[key] = rec
    return out


def legacy_overload_keys(records):
    """Map the former target-RVA overload keys to stable signature keys."""
    indexed = index_by_mangled(records)
    aliases = {}
    for mangled, primary in indexed.items():
        if mangled != primary["mangled"]:
            continue
        siblings = sorted(
            (
                rec
                for key, rec in indexed.items()
                if rec["mangled"] == mangled and key != mangled
            ),
            key=lambda rec: rec["rva"],
        )
        for rec in siblings:
            aliases[f"{mangled}@@pdb-overload:{rec['rva']:x}"] = overload_key(
                mangled, rec["name"]
            )
    return aliases


def stmt_seq(rec):
    """Primary alignment alphabet: statement sizes."""
    return [s["size"] for s in rec["statements"]]


def normalized_stmt_lines(rec):
    """Return function-relative PDB line numbers when every row has one.

    Absolute lines legitimately move as files are reconstructed. Exact relative
    line geometry, however, is strong evidence that equal-count rows correspond
    in source order even when repeated statement sizes confuse SequenceMatcher.
    """
    lines = [s.get("line") for s in rec["statements"]]
    if not lines or any(not isinstance(line, int) for line in lines):
        return None
    first = lines[0]
    return [line - first for line in lines]


def classify(t_rec, b_rec):
    """Approximate structure classification for queue ranking (the authoritative
    per-function verdict stays `pdb_fetch --view structure-diff`)."""
    t_seq, b_seq = stmt_seq(t_rec), stmt_seq(b_rec)
    t_n, b_n = len(t_seq), len(b_seq)
    if t_seq == b_seq:
        return "MATCH", t_n, b_n, 0, 0, 0

    # Prefer exact PDB line geometry over a size-only fuzzy alignment. This is
    # deliberately an exact shortcut: differing relative lines still fall back
    # to the established size alignment below.
    t_lines = normalized_stmt_lines(t_rec)
    if (
        t_n == b_n
        and t_lines is not None
        and t_lines == normalized_stmt_lines(b_rec)
    ):
        n_size = sum(t_size != b_size for t_size, b_size in zip(t_seq, b_seq))
        return "SIZE", t_n, b_n, n_size, 0, 0

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


# --- dynamic-initializer / atexit-destructor cross-name pairing -------------
#
# The two sides label the same compiler-generated static-init thunk with
# DIFFERENT name strings, so the primary `set(target) & set(base)` pass misses
# them and they show up as both target_only AND base_only (a measurement bug -
# the functions are byte-identical, just unpaired):
#
#   TARGET (original game PDB): demangled form, emitted verbatim by
#     pdb_rich_context -   vostok::sound::`dynamic initializer for 's_debug_audio''
#   BASE   (our PDB): either the raw mangled form -
#     ??__Es_debug_audio@sound@vostok@@YAXXZ
#   or a demangled form with the namespace inside the quotes -
#     `dynamic initializer for 'vostok::sound::s_debug_audio''
#
# Rich indexes can place a local static's qualified scope either outside or
# inside the thunk's quotes. Canonicalize both forms to the exact
# (kind, fully-qualified-variable-name) identity and pair only unique 1:1 keys
# attributed to compatible source files. Rich spellings preserve complete local
# scopes and can therefore pair function-local statics directly. Raw mangled
# spellings remain limited to fully-qualified plain identifiers (no anonymous-
# namespace `?A0x...` hash or template/local scope, which need the real
# demangler). Body and statement differences remain visible as ordinary measured
# differences.

_DYN_RE = re.compile(r"^(.*?)`dynamic (initializer|atexit destructor) for '(.*)''$")
_IDENT_RE = re.compile(r"^[A-Za-z_][A-Za-z0-9_]*$")
_LOCAL_SCOPE_RE = re.compile(r"::`\d+'::")
_LOCAL_FUNCTION_SCOPE_RE = re.compile(r"^`([^'\r\n]+)'::`\d+'::")


def dyn_canon_rich(mangled):
    """Canonical identity for a rich-index dynamic-init thunk."""
    m = _DYN_RE.match(mangled)
    if not m:
        return None
    pfx, kind, inner = m.group(1), m.group(2), m.group(3)
    kc = "E" if kind == "initializer" else "F"
    # pfx is the namespace/local scope, or empty when it sits inside the quotes.
    if pfx:
        if not pfx.endswith("::"):
            return None
        fqn = pfx + inner
    else:
        fqn = inner
    if not fqn or "\n" in fqn or "\r" in fqn:
        return None
    return (kc, fqn)


def dyn_canon_base(mangled):
    """Canonical identity for either rich spelling or raw ??__E/??__F form."""
    rich = dyn_canon_rich(mangled)
    if rich:
        return rich
    if mangled.startswith("??__E"):
        kc = "E"
    elif mangled.startswith("??__F"):
        kc = "F"
    else:
        return None
    body = mangled[5:]
    if not body.endswith("@@YAXXZ"):
        return None
    inner = body[: -len("@@YAXXZ")]
    if "?" in inner or "$" in inner:  # local/anon/template scope - defer
        return None
    parts = [p for p in inner.split("@") if p]
    if not parts:
        return None
    var, scopes = parts[0], list(reversed(parts[1:]))  # mangled scopes are inner-first
    if not _IDENT_RE.match(var) or any(not _IDENT_RE.match(s) for s in scopes):
        return None
    return (kc, "::".join(scopes + [var]))


def dyn_owner_compatible(target_rec, base_rec, canon):
    """Whether source attribution can disambiguate a dynamic thunk pair.

    Compiler-generated local-static destructors can have no base-side source
    record even when the target PDB assigns them to an unrelated inline header.
    A unique canonical local scope is sufficient even when both PDBs report
    different owners: the enclosing function and compiler local-scope ordinal
    disambiguate it. Global/static names with two known owners still require an
    exact owner match.
    """
    target_file = target_rec.get("file")
    base_file = base_rec.get("file")
    return (
        not target_file
        or not base_file
        or target_file == base_file
        or bool(_LOCAL_SCOPE_RE.search(canon[1]))
    )


def dynamic_local_owner_modules(records):
    """Resolve function-local static thunks through their enclosing PDB owner.

    ICF may attribute the thunk body to an unrelated inline header. Its rich
    name still preserves the complete enclosing function scope and local-scope
    ordinal. When that enclosing function has one source owner in the target
    PDB, that source module is the thunk's logical owner.
    """
    scopes_by_canon = {}
    for rec in records:
        canon = dyn_canon_rich(rec["mangled"])
        if not canon:
            continue
        match = _LOCAL_FUNCTION_SCOPE_RE.match(canon[1])
        if match:
            scopes_by_canon[canon] = match.group(1)

    files_by_scope = {}
    for scope in set(scopes_by_canon.values()):
        marker = f"{scope}("
        files = {
            rec["file"] for rec in records
            if marker in (rec.get("name") or "") and rec.get("file")
        }
        if len(files) == 1:
            files_by_scope[scope] = next(iter(files))

    return {
        canon: module_of(files_by_scope[scope])
        for canon, scope in scopes_by_canon.items()
        if scope in files_by_scope
    }


def load_module_ownership_overrides(path=MODULE_OWNERSHIP_OVERRIDES):
    """Load reviewed mangled-name owners for PDB records defeated by ICF."""
    if not path.is_file():
        return {}
    overrides = {}
    for number, raw in enumerate(path.read_text(encoding="utf-8").splitlines(), 1):
        if not raw or raw.startswith("#"):
            continue
        fields = raw.split("\t")
        if len(fields) != 3:
            raise ValueError(f"{path}:{number}: expected symbol, module, source file")
        mangled, module, source_file = fields
        if not mangled or not module or module_of(source_file) != module:
            raise ValueError(f"{path}:{number}: inconsistent module ownership row")
        if mangled in overrides:
            raise ValueError(f"{path}:{number}: duplicate symbol {mangled}")
        overrides[mangled] = module
    return overrides


def logical_module(mangled, rec, units, dynamic_owners, overrides):
    """Return logical source ownership, independent of an ICF body owner."""
    override = overrides.get(mangled)
    if override:
        return override
    canon = dyn_canon_rich(mangled) or dyn_canon_base(mangled)
    if canon and canon in dynamic_owners:
        return dynamic_owners[canon]
    unit_or_file = rec["file"] if units and rec["file"] in units else (
        units[0] if units else rec["file"]
    )
    return module_of(unit_or_file)


def dynamic_pair_score(target_mangled, base_mangled, target_rec, base_rec, scores):
    """Use report score, or strict rich-stream exact evidence when unscored."""
    fuzzy = scores.get(target_mangled, scores.get(base_mangled))
    if fuzzy is None and instruction_stream_exact(target_rec, base_rec):
        return 100.0
    return fuzzy


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


def _source_extent(rec):
    """Return ``(relative path, first line, last line, source text)``."""
    if rec is None:
        return None
    path = VOSTOK / "sources" / rec["file"]
    lines = [s["line"] for s in rec["statements"] if s.get("line")]
    if not path.is_file():
        return None
    try:
        with open(path, encoding="latin-1") as f:
            source_lines = f.readlines()
    except OSError:
        return None
    if lines:
        lo, hi = min(lines), max(lines)
        text = "".join(source_lines[lo - 1 : hi])
    else:
        # Compiler-generated helpers and some ICF-selected header bodies carry
        # a real source file but no line records. Hash the whole owning file:
        # broader than a function extent, but conservative and source-scoped.
        lo, hi = 1, len(source_lines)
        text = "".join(source_lines)
    return rec["file"], lo, hi, text


def _whole_source_extent(rec):
    """Return a conservative whole-file extent for object-only evidence."""
    if rec is None or not rec.get("file"):
        return None
    path = VOSTOK / "sources" / rec["file"]
    if not path.is_file():
        return None
    try:
        with open(path, encoding="latin-1") as source:
            lines = source.readlines()
    except OSError:
        return None
    return rec["file"], 1, len(lines), "".join(lines)


def src_fingerprint(rec):
    """Hash of the function's source extent (file + statement line range text).

    Hashes SOURCE TEXT, not bytes: a matcher edit changes it; a header/other-unit
    change that only shifts codegen does not.
    """
    extent = _source_extent(rec)
    if extent is None:
        return None
    source_file, lo, _hi, text = extent
    return hashlib.sha1(f"{source_file}:{lo}:{text}".encode("latin-1")).hexdigest()


_MAX_CONTEXT_CACHE = {}
_MAX_CONTEXT_SUFFIXES = frozenset((".h", ".hh", ".hpp", ".inl", ".vcproj"))
_TRANSLATION_UNIT_SUFFIXES = frozenset((".c", ".cc", ".cpp", ".cxx"))


def _hash_paths(paths):
    digest = hashlib.sha256()
    for path in sorted(set(paths)):
        if not path.is_file():
            continue
        digest.update(path.relative_to(VOSTOK).as_posix().encode("utf-8"))
        digest.update(b"\0")
        digest.update(path.read_bytes())
        digest.update(b"\0")
    return digest.hexdigest()[:16]


def _max_context_hash(module):
    """Hash stable inputs that can change a function without changing its body.

    Vostok is LTCG-built, so this is deliberately conservative: all headers and
    project files in the owning module, shared top-level Vostok headers, anchor
    sources, and the pinned build/delink configuration form one module context.
    Compiler-state island probes may vary disposable declarations, but must
    restore these tracked inputs before recording evidence.
    """
    if module in _MAX_CONTEXT_CACHE:
        return _MAX_CONTEXT_CACHE[module]

    source_root = VOSTOK / "sources" / "vostok"
    paths = [
        VOSTOK / "flake.lock",
        VOSTOK / "flake.nix",
        VOSTOK / "scripts" / "generate_ninja.py",
        VOSTOK / "scripts" / "generate_delink.py",
    ]
    paths.extend(
        path for path in source_root.iterdir()
        if path.is_file() and path.suffix.lower() in _MAX_CONTEXT_SUFFIXES
    )
    module_root = source_root / module
    if module_root.is_dir():
        paths.extend(
            path for path in module_root.rglob("*")
            if path.is_file() and (
                path.suffix.lower() in _MAX_CONTEXT_SUFFIXES
                or path.name in {"anchor.cpp", "temp_include_all.cpp"}
            )
        )
    value = _hash_paths(paths)
    _MAX_CONTEXT_CACHE[module] = value
    return value


def _translation_unit_context_hash(source_file):
    """Hash the owning implementation file when the PDB identifies one.

    With LTCG, edits to another function in the same translation unit can alter
    inlining, register allocation, and retained statement rows without changing
    this function's source extent. Header-owned COMDATs do not identify one
    unique owning TU here, so they remain covered by the conservative module
    header/project context until unit-scoped attribution is available.
    """
    path = VOSTOK / "sources" / source_file
    if path.suffix.lower() not in _TRANSLATION_UNIT_SUFFIXES or not path.is_file():
        return None
    return _hash_paths([path])


def effective_source_hash(rec, module=None):
    """HoMM2-style effective-source epoch for a source-backed function.

    The body hash is scoped by a conservative module/compiler context. Unlike
    ``history.src_fingerprint``, this hash owns correctness-facing MAX evidence;
    ordinary best-seen/ICF history is never promoted into it.
    """
    extent = _source_extent(rec)
    if extent is None:
        return None
    source_file, _lo, _hi, text = extent
    owner = module or module_of(source_file)
    body = hashlib.sha1(text.encode("latin-1")).hexdigest()[:12]
    context = _max_context_hash(owner)
    unit_context = _translation_unit_context_hash(source_file)
    if unit_context:
        context = f"{context}.{unit_context}"
    return f"{body}.{context}"


def effective_source_hash_at(source_file, lo, hi, module):
    """Re-hash a retained source locator when its symbol is not in this build."""
    path = VOSTOK / "sources" / source_file
    if not path.is_file() or not lo or not hi:
        return None
    try:
        with open(path, encoding="latin-1") as f:
            text = "".join(f.readlines()[lo - 1 : hi])
    except OSError:
        return None
    body = hashlib.sha1(text.encode("latin-1")).hexdigest()[:12]
    context = _max_context_hash(module or module_of(source_file))
    unit_context = _translation_unit_context_hash(source_file)
    if unit_context:
        context = f"{context}.{unit_context}"
    return f"{body}.{context}"


def retained_max_effective_hash(previous, rec):
    """Re-hash a retained MAX without trusting a different folded owner.

    LTCG/ICF can leave the same mangled spelling attached only to an unrelated
    PDB alias in the canonical build. Island evidence pins its reviewed source
    locator, so a different current owner must not invalidate that epoch. A
    same-owner record still supplies the current statement extent, preserving
    normal edit/reset behavior.
    """
    module, source_file, lo, hi = previous[5:9]
    if rec is None or rec.get("file") != source_file:
        return effective_source_hash_at(source_file, lo, hi, module)
    extent = _source_extent(rec)
    if extent is None:
        return None
    return effective_source_hash(rec, module or module_of(source_file))


def merge_maximum_epoch(previous, candidate):
    """Merge two observations for one mangled/effective-hash epoch."""
    if previous is None:
        return candidate
    if candidate is None:
        return previous
    if previous[:2] != candidate[:2]:
        raise ValueError("cannot merge different source MAX epochs")

    preferred = max((previous, candidate), key=lambda row: (row[3], row[2]))
    return (
        preferred[0],
        preferred[1],
        max(previous[2], candidate[2]),
        int(bool(previous[3]) or bool(candidate[3])),
        *preferred[4:],
    )


def maximum_needs_epoch_archive(row, current_fuzzy):
    """Keep only evidence that a rebuild cannot trivially reproduce.

    Plain rebuild observations equal to the build that created the database do
    not need an inactive-epoch copy.  Island/cross-unit evidence, temporarily
    absent functions, and maxima above that build do.
    """
    if row[9] != "rebuild" or row[10]:
        return True
    if current_fuzzy is None:
        return True
    return row[2] > current_fuzzy + 0.0001 or (
        bool(row[3]) and current_fuzzy < 99.995
    )


def maximum_for_effective_hash(mangled, effective_hash, active, epochs):
    """Select only proof belonging to the source/compiler hash now active."""
    previous = epochs.get((mangled, effective_hash))
    active_previous = active.get(mangled)
    if active_previous is not None and active_previous[1] == effective_hash:
        previous = merge_maximum_epoch(previous, active_previous)
    return previous


def compiled_state_id(rec):
    """Identity for the observed candidate state (size + ordered instructions)."""
    if rec is None:
        return None
    state = {
        "size": rec.get("size"),
        "instructions": [
            (ins.get("off"), ins.get("len"), ins.get("text"))
            for ins in rec.get("instructions", [])
        ],
    }
    encoded = json.dumps(state, ensure_ascii=True, separators=(",", ":"))
    return hashlib.sha256(encoded.encode("utf-8")).hexdigest()[:24]


def report_fuzzy_scores(report):
    """Return the best measured objdiff score for each decorated name."""
    scores = {}
    units_by_mangled = {}
    for unit in report["units"]:
        uname = unit["name"]
        for fn in unit["functions"]:
            mangled = fn["name"]
            units_by_mangled.setdefault(mangled, []).append(uname)
            fuzzy = fn.get("fuzzy_match_percent")
            if fuzzy is None:
                continue
            previous = scores.get(mangled)
            scores[mangled] = fuzzy if previous is None else max(previous, fuzzy)
    for units in units_by_mangled.values():
        units.sort()
    return scores, units_by_mangled


def report_score_for_target(mangled, scores):
    """Map objdiff's normalized spellings back to the retail PDB identity."""
    fuzzy = scores.get(mangled)
    compiler = normalize_objdiff_symbols.compiler_name(mangled)
    if fuzzy is None and compiler:
        fuzzy = scores.get(compiler)
    if fuzzy is None and mangled.startswith("??_G"):
        fuzzy = scores.get(f"??_E{mangled[4:]}")
    return fuzzy


def cross_unit_exact_score(mangled, scores):
    """Return strict object-level exact evidence for a target PDB identity."""
    fuzzy = report_score_for_target(mangled, scores)
    return fuzzy if fuzzy is not None and fuzzy >= 99.995 else None


def island_report_score(expected, mangled, scores):
    """Read a manifested report identity, defaulting to the target spelling."""
    return report_score_for_target(expected.get("report_mangled", mangled), scores)


def sha256_file(path):
    digest = hashlib.sha256()
    with open(path, "rb") as source:
        for chunk in iter(lambda: source.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def instruction_stream_exact(target_rec, base_rec):
    """Prove exact code when objdiff omitted a function score.

    The rich-index producer has already normalized branch labels and relocation
    operands to symbolic instruction text. Equal size plus an identical,
    non-empty ordered instruction stream is therefore strict function-scoped
    exact evidence; an absent instruction stream is never evidence.
    """
    if target_rec is None or base_rec is None:
        return False
    target_instructions = target_rec.get("instructions") or []
    base_instructions = base_rec.get("instructions") or []
    if not target_instructions or target_rec.get("size") != base_rec.get("size"):
        return False

    def normalized_text(text):
        parts = text.split(None, 1)
        if len(parts) == 2 and parts[0] in {"call", "jmp"}:
            operand = {
                "operator delete": "??3@YAXPAX@Z",
            }.get(parts[1], parts[1])
            operand = re.sub(r"\s+>", ">", operand)
            return f"{parts[0]} {operand}"
        return text

    def identity(instructions):
        return [
            (ins.get("off"), ins.get("len"), normalized_text(ins.get("text", "")))
            for ins in instructions
        ]

    return identity(target_instructions) == identity(base_instructions)


def island_candidate_score(expected, mangled, scores, target_rec, candidate_rec):
    """Prefer strict function-scoped rich exactness over a stale report pair."""
    if instruction_stream_exact(target_rec, candidate_rec):
        return 100.0
    return island_report_score(expected, mangled, scores)


def load_exact_fold_aliases(path=EXACT_FOLD_ALIASES):
    """Load reviewed target-to-base identities for asymmetric ICF groups."""
    aliases = {}
    if not Path(path).is_file():
        return aliases
    for line_number, raw_line in enumerate(Path(path).read_text().splitlines(), 1):
        line = raw_line.strip()
        if not line or line.startswith("#"):
            continue
        fields = line.split("\t")
        if len(fields) != 2 or not all(fields):
            raise RuntimeError(
                f"{path}:{line_number}: expected <target>\\t<base alias>"
            )
        target, base = fields
        aliases[target] = base
    return aliases


def shared_icf_alias_owner_compatible(
    target_rec, base_rec, target_alias_names_by_rva, base_alias_names_by_rva
):
    """Prove an owner-file mismatch is only a shared ICF alias choice.

    A single matching demangled spelling is insufficient because unrelated
    internal helpers can have the same readable name in different TUs. When
    both RVAs expose the same two-or-more-name alias cluster, however, the PDBs
    identify the same folded body even if each linker selected a different
    source record as its owner.
    """
    if not target_alias_names_by_rva or not base_alias_names_by_rva:
        return False
    target_names = target_alias_names_by_rva.get(target_rec["rva"], set())
    base_names = base_alias_names_by_rva.get(base_rec["rva"], set())
    return (
        target_names == base_names
        and target_rec["name"] in target_names
        and len(target_names) >= 2
    )


def strict_source_alias_candidates(
    target_rec,
    base_aliases_by_name,
    used_base_rvas,
    allow_used=False,
    target_alias_names_by_rva=None,
    base_alias_names_by_rva=None,
    exact_fold_aliases=None,
    base_aliases_by_mangled=None,
):
    """Find exact same-source bodies hidden behind a different ICF name.

    A folded RVA may inherit another function's mangled identity independently
    on each side.  The PDB still records every alias's demangled signature and
    source owner, so a unique same-name, byte-exact rich record is sufficient
    when it either has the same source file or both RVAs expose the same
    multi-name ICF alias cluster. The latter proves differing owner files came
    from linker alias selection, not a same-named internal helper in another
    TU.
    """
    candidates = [
        rec
        for rva, rec in base_aliases_by_name.get(target_rec["name"], {}).items()
        if (allow_used or rva not in used_base_rvas)
        and (
            rec["file"] == target_rec["file"]
            or shared_icf_alias_owner_compatible(
                target_rec,
                rec,
                target_alias_names_by_rva,
                base_alias_names_by_rva,
            )
        )
        and instruction_stream_exact(target_rec, rec)
    ]

    # A reviewed asymmetric ICF identity may bridge disjoint readable PDB
    # aliases. It still earns a pair only when the complete body is identical;
    # uniqueness is enforced by the caller.
    exact_fold_aliases = exact_fold_aliases or {}
    base_aliases_by_mangled = base_aliases_by_mangled or {}
    base_mangled = exact_fold_aliases.get(target_rec.get("mangled"))
    if base_mangled:
        seen_rvas = {rec["rva"] for rec in candidates}
        for rva, rec in base_aliases_by_mangled.get(base_mangled, {}).items():
            if rva in seen_rvas or (not allow_used and rva in used_base_rvas):
                continue
            if instruction_stream_exact(target_rec, rec):
                candidates.append(rec)
                seen_rvas.add(rva)
    return candidates


def report_source_alias_candidates(
    target_rec,
    base_aliases_by_name,
    used_base_rvas,
    target_alias_names_by_rva=None,
    base_alias_names_by_rva=None,
):
    """Find a unique owner-compatible alias for an objdiff-measured symbol.

    The report score already proves which target symbol was compared.  Unlike
    an unmeasured rich-only alias, its source statements and instructions may
    legitimately differ; requiring either to be exact would hide the mismatch
    that the report is meant to expose.
    """
    return [
        rec
        for rva, rec in base_aliases_by_name.get(target_rec["name"], {}).items()
        if rva not in used_base_rvas
        and (
            rec["file"] == target_rec["file"]
            or shared_icf_alias_owner_compatible(
                target_rec,
                rec,
                target_alias_names_by_rva,
                base_alias_names_by_rva,
            )
        )
    ]


def exact_paired_source_alias(
    source_rec,
    candidate_aliases_by_name,
    paired_candidate_rvas,
    source_alias_names_by_rva=None,
    candidate_alias_names_by_rva=None,
):
    """Return one exact source alias already represented by a paired RVA.

    ICF may fold several target functions onto one RVA while the base keeps a
    separate body for every symbol (or vice versa).  The extra bodies are not
    unexplained source: a rich record with the same complete demangled name,
    compatible source ownership, and identical instructions proves that the
    paired side already represents the function.
    """
    candidates = strict_source_alias_candidates(
        source_rec,
        candidate_aliases_by_name,
        set(),
        allow_used=True,
        target_alias_names_by_rva=source_alias_names_by_rva,
        base_alias_names_by_rva=candidate_alias_names_by_rva,
    )
    candidates = [
        rec for rec in candidates if rec["rva"] in paired_candidate_rvas
    ]
    return candidates[0] if len(candidates) == 1 else None


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
    sources (rebuild.py records the HEAD it built at in report.head), and the
    DB must have ingested that report (rebuild.py regenerates it at the end of
    every build; `refresh` re-derives it regen-only). Under lowest-match-first
    ordering, stale rows for freshly-landed work look like 0% and get
    dispatched FIRST - so queue refuses on either stale leg (--stale-ok
    overrides); other commands warn. Cadence: land -> rebuild.py (regenerates
    the DB); run `refresh` by hand only to re-ingest an already-built report."""
    hard, soft = [], []
    row = con.execute("SELECT value FROM meta WHERE key='build_head'").fetchone()
    build_head = row[0] if row else None
    if not build_head or build_head.startswith("?"):
        hard.append("DB predates the staleness guard - rebuild.py + refresh first")
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
                    f" last: {last}) - rebuild.py + refresh first"
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


def authoritative_demangled_names(target, base):
    """Return one display name per mangled symbol, preferring retail PDB data.

    A folded base RVA can expose the target mangled spelling through a different
    PDB alias.  That base alias is useful for pairing, but it must not replace
    the target PDB's authoritative spelling in the canonical symbol inventory.
    """
    names = {mangled: rec["name"] for mangled, rec in target.items()}
    for mangled, rec in base.items():
        names.setdefault(mangled, rec["name"])
    return names


def cmd_refresh(args):
    regen()


def regen():
    """Regenerate match.db from the already-built diff artifacts (report.json +
    rich indexes). This is REGEN-ONLY: it does NOT run rebuild.py. rebuild.py is
    the canonical build step and calls this at the end of its run; invoke
    `match_db.py refresh` by hand only to re-derive the DB from an artifact set
    that is already on disk (run rebuild.py first if sources moved)."""
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

    # soft staleness note: refresh is a deliberate regen-only step, so it proceeds
    # even when report.json predates the current sources - but warn so a hand-run
    # refresh doesn't silently re-derive the DB from a stale build (the same
    # build-leg the queue staleness_check enforces; here we only warn).
    head_marker = REPORT.parent / "report.head"
    if head_marker.is_file():
        report_head = head_marker.read_text().strip()
        bh = report_head[: -len("+dirty")] if report_head.endswith("+dirty") else report_head
        if bh and not bh.startswith("?") and bh != git_head():
            rc, _ = _git("diff", "--quiet", bh, "HEAD", "--", "sources/")
            if rc != 0:
                log("STALE: report.json is stale (sources/ moved since it was "
                    "built) - run rebuild.py first; refreshing anyway")

    log("loading rich indexes ...")
    target_records = load_index_records(TARGET_IDX)
    target = index_by_mangled(target_records)
    base_records = load_index_records(BASE_IDX)
    target_primary_signatures = {
        mangled: rec["name"]
        for mangled, rec in target.items()
        if mangled == rec["mangled"]
    }
    target_owners = {
        (rec["mangled"], rec["name"]): rec["file"] for rec in target.values()
    }
    base = index_by_mangled(
        base_records,
        target_owners,
        target_primary_signatures,
    )
    dynamic_owners = dynamic_local_owner_modules(target_records)
    module_overrides = load_module_ownership_overrides()
    rich_pdb_aliases = normalize_objdiff_symbols.rich_pdb_aliases(
        TARGET_IDX,
        BASE_IDX,
        source_prefix="vostok/render/",
    )

    def compiler_alias(mangled):
        return (
            rich_pdb_aliases.get(mangled)
            or normalize_objdiff_symbols.compiler_name(mangled)
        )

    log(f"  target: {len(target)} functions, base: {len(base)} functions")
    if rich_pdb_aliases:
        log(f"  render rich-PDB aliases: {len(rich_pdb_aliases)}")

    log("loading report.json ...")
    report = json.loads(REPORT.read_text())
    report_scores, units_by_mangled = report_fuzzy_scores(report)
    report_fns = []  # (unit, mangled, fuzzy)
    for unit in report["units"]:
        uname = unit["name"]
        for fn in unit["functions"]:
            report_fns.append((uname, fn["name"], fn.get("fuzzy_match_percent")))

    cross_unit_fuzzy = {}
    if CROSS_UNIT_REPORT.is_file():
        cross_report = json.loads(CROSS_UNIT_REPORT.read_text())
        for function in cross_report.get("functions", []):
            fuzzy = function.get("fuzzy_match_percent")
            if fuzzy is not None:
                cross_unit_fuzzy[function["name"]] = fuzzy
        if cross_unit_fuzzy:
            log(f"loaded {len(cross_unit_fuzzy)} cross-unit COMDAT scores")

    # interners: collect every name, then assign ids in sorted order
    syms, units_i, files_i = Interner(), Interner(), Interner()
    demangled_by_mangled = authoritative_demangled_names(target, base)
    for side in (target, base):
        for mangled, rec in side.items():
            syms(mangled)
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
                    logical_module(
                        mangled, rec, units, dynamic_owners, module_overrides
                    ),
                    line,
                    rec["size"],
                    len(stmts),
                    0 if is_framed(rec) else 1,
                )
            )
        rows.sort()
        return rows

    log("classifying structure for paired functions ...")
    fuzzy_by_mangled = dict(report_scores)
    for mangled, fuzzy in cross_unit_fuzzy.items():
        fuzzy_by_mangled.setdefault(mangled, fuzzy)
    # The disposable target COFF tree normalizes safe retail PDB backtick names
    # to MSVC's ??__E/??__F spelling so objdiff can pair them with candidate
    # objects. The rich indexes retain the authoritative PDB names; reflect each
    # normalized report score back onto that identity before building pair rows.
    for mangled in target:
        fuzzy = report_score_for_target(mangled, fuzzy_by_mangled)
        if fuzzy is not None:
            fuzzy_by_mangled[mangled] = fuzzy
        compiler = compiler_alias(mangled)
        if compiler in fuzzy_by_mangled and mangled not in fuzzy_by_mangled:
            fuzzy_by_mangled[mangled] = fuzzy_by_mangled[compiler]
    pair_rows = []
    rich_exact = 0
    for mangled in sorted(set(target) & set(base)):
        cls, t_n, b_n, n_size, n_tonly, n_bonly = classify(target[mangled], base[mangled])
        fuzzy = fuzzy_by_mangled.get(mangled)
        if fuzzy is None and instruction_stream_exact(target[mangled], base[mangled]):
            fuzzy = 100.0
            rich_exact += 1
        pair_rows.append(
            (
                sym_id[mangled],
                target[mangled]["rva"],
                base[mangled]["rva"],
                fuzzy,
                cls,
                t_n,
                b_n,
                n_size,
                n_tonly,
                n_bonly,
            )
        )
    if rich_exact:
        log(
            f"function-scoped rich-index exact attribution recovered "
            f"{rich_exact} objdiff score gap(s)"
        )
    # Cross-name pairing first recovers PDB aliases whose base rich-index record
    # carries another folded symbol's mangled name.  The delink report proves
    # that the target name was emitted and compared; additionally require one
    # free base RVA, an exact demangled signature, and compatible source ownership.
    paired_primary = set(target) & set(base)
    cross_paired_mangled = set()  # base AND target names paired across a name gap
    used_target_rvas = {row[1] for row in pair_rows}
    used_base_rvas = {row[2] for row in pair_rows}
    n_compiler_alias = 0
    for tm in sorted(set(target) - paired_primary):
        bm = compiler_alias(tm)
        if (
            bm not in base
            or tm not in fuzzy_by_mangled
            or target[tm]["rva"] in used_target_rvas
            or base[bm]["rva"] in used_base_rvas
        ):
            continue
        cls, t_n, b_n, n_size, n_tonly, n_bonly = classify(target[tm], base[bm])
        pair_rows.append(
            (
                sym_id[tm], target[tm]["rva"], base[bm]["rva"],
                fuzzy_by_mangled[tm], cls, t_n, b_n, n_size, n_tonly, n_bonly,
            )
        )
        used_target_rvas.add(target[tm]["rva"])
        used_base_rvas.add(base[bm]["rva"])
        cross_paired_mangled.update((tm, bm))
        n_compiler_alias += 1
    if n_compiler_alias:
        log(f"cross-name paired {n_compiler_alias} exact compiler aliases")

    target_aliases_by_name = {}
    for rec in target_records:
        target_aliases_by_name.setdefault(rec["name"], {})[rec["rva"]] = rec
    base_aliases_by_name = {}
    for rec in base_records:
        base_aliases_by_name.setdefault(rec["name"], {})[rec["rva"]] = rec
    exact_fold_aliases = load_exact_fold_aliases()
    base_aliases_by_mangled = {}
    for rec in base_records:
        base_aliases_by_mangled.setdefault(rec["mangled"], {})[rec["rva"]] = rec
    target_alias_names_by_rva = {}
    for rec in target_records:
        target_alias_names_by_rva.setdefault(rec["rva"], set()).add(rec["name"])
    base_alias_names_by_rva = {}
    for rec in base_records:
        base_alias_names_by_rva.setdefault(rec["rva"], set()).add(rec["name"])
    n_alias = 0
    for tm in sorted(set(target) - paired_primary):
        if tm not in fuzzy_by_mangled:
            continue
        trec = target[tm]
        candidates = report_source_alias_candidates(
            trec,
            base_aliases_by_name,
            used_base_rvas,
            target_alias_names_by_rva=target_alias_names_by_rva,
            base_alias_names_by_rva=base_alias_names_by_rva,
        )
        if trec["rva"] in used_target_rvas or len(candidates) != 1:
            continue
        brec = candidates[0]
        bm = brec["mangled"]
        cls, t_n, b_n, n_size, n_tonly, n_bonly = classify(trec, brec)
        pair_rows.append(
            (
                sym_id[tm], trec["rva"], brec["rva"], fuzzy_by_mangled[tm], cls,
                t_n, b_n, n_size, n_tonly, n_bonly,
            )
        )
        used_target_rvas.add(trec["rva"])
        used_base_rvas.add(brec["rva"])
        cross_paired_mangled.update((tm, bm))
        n_alias += 1
    if n_alias:
        log(f"cross-name paired {n_alias} report-grounded folded PDB aliases")

    # Some highly-COMDAT functions never reach objdiff because target and base
    # choose different canonical mangled names for the same folded RVA.  Recover
    # only unique, same-source aliases whose complete rich instruction streams
    # are identical; unlike the report-grounded pass above, this also covers
    # target-only score gaps.
    n_rich_alias = 0
    for tm in sorted(set(target) - paired_primary - cross_paired_mangled):
        trec = target[tm]
        if trec["rva"] in used_target_rvas:
            continue
        candidates = strict_source_alias_candidates(
            trec,
            base_aliases_by_name,
            used_base_rvas,
            target_alias_names_by_rva=target_alias_names_by_rva,
            base_alias_names_by_rva=base_alias_names_by_rva,
            exact_fold_aliases=exact_fold_aliases,
            base_aliases_by_mangled=base_aliases_by_mangled,
        )
        if len(candidates) != 1:
            continue
        brec = candidates[0]
        bm = brec["mangled"]
        cls, t_n, b_n, n_size, n_tonly, n_bonly = classify(trec, brec)
        pair_rows.append(
            (
                sym_id[tm], trec["rva"], brec["rva"], 100.0, cls,
                t_n, b_n, n_size, n_tonly, n_bonly,
            )
        )
        used_target_rvas.add(trec["rva"])
        used_base_rvas.add(brec["rva"])
        cross_paired_mangled.update((tm, bm))
        n_rich_alias += 1
    if n_rich_alias:
        log(f"cross-name paired {n_rich_alias} strict same-source rich aliases")

    # A single ICF-selected body can legitimately own several PDB aliases.  If
    # the base RVA was already consumed by another target symbol, retain a
    # second target pairing only when the base PDB still records this exact
    # demangled signature in the same source file and the normalized rich
    # instruction stream is byte-exact.  This recovers genuine header islands
    # without treating an arbitrary same-body fold as source ownership.
    n_shared_rich_alias = 0
    for tm in sorted(set(target) - paired_primary - cross_paired_mangled):
        trec = target[tm]
        if trec["rva"] in used_target_rvas:
            continue
        candidates = strict_source_alias_candidates(
            trec,
            base_aliases_by_name,
            used_base_rvas,
            allow_used=True,
            target_alias_names_by_rva=target_alias_names_by_rva,
            base_alias_names_by_rva=base_alias_names_by_rva,
            exact_fold_aliases=exact_fold_aliases,
            base_aliases_by_mangled=base_aliases_by_mangled,
        )
        if len(candidates) != 1 or candidates[0]["rva"] not in used_base_rvas:
            continue
        brec = candidates[0]
        bm = brec["mangled"]
        cls, t_n, b_n, n_size, n_tonly, n_bonly = classify(trec, brec)
        pair_rows.append(
            (
                sym_id[tm], trec["rva"], brec["rva"], 100.0, cls,
                t_n, b_n, n_size, n_tonly, n_bonly,
            )
        )
        used_target_rvas.add(trec["rva"])
        cross_paired_mangled.update((tm, bm))
        n_shared_rich_alias += 1
    if n_shared_rich_alias:
        log(
            "cross-name paired "
            f"{n_shared_rich_alias} shared-RVA same-source rich aliases"
        )

    # Then pair dynamic-init/atexit thunks across their several rich/raw name
    # spellings. The canonical identity must be unique on each side; emitted
    # size may legitimately differ and is what the fuzzy matcher measures.
    t_canon, b_canon = {}, {}     # (kind, fqn) -> [mangled, ...]
    for m in set(target) - paired_primary:
        c = dyn_canon_rich(m)
        if c:
            t_canon.setdefault(c, []).append(m)
    for m in set(base) - paired_primary:
        c = dyn_canon_base(m)
        if c:
            b_canon.setdefault(c, []).append(m)
    n_cross = 0
    for c in t_canon.keys() & b_canon.keys():
        tm_list, bm_list = t_canon[c], b_canon[c]
        if len(tm_list) != 1 or len(bm_list) != 1:
            continue  # ambiguous - leave for the Rust-side demangler
        tm, bm = tm_list[0], bm_list[0]
        if target[tm]["rva"] in used_target_rvas or base[bm]["rva"] in used_base_rvas:
            continue
        if not dyn_owner_compatible(target[tm], base[bm], c):
            continue  # same-named statics in different known owners are not proven identical
        cls, t_n, b_n, n_size, n_tonly, n_bonly = classify(target[tm], base[bm])
        pair_rows.append(
            (
                sym_id[tm],
                target[tm]["rva"],
                base[bm]["rva"],
                dynamic_pair_score(
                    tm, bm, target[tm], base[bm], fuzzy_by_mangled
                ),
                cls,
                t_n,
                b_n,
                n_size,
                n_tonly,
                n_bonly,
            )
        )
        cross_paired_mangled.update((tm, bm))
        used_target_rvas.add(target[tm]["rva"])
        used_base_rvas.add(base[bm]["rva"])
        n_cross += 1
    if n_cross:
        log(f"cross-name paired {n_cross} dynamic-init/atexit thunks (??__E/??__F <-> demangled)")

    # The retail and candidate PDBs render a small audited set of template type
    # spellings differently (for example, an explicit ``enum`` keyword).  The
    # disposable target COFF normalizer gives objdiff the candidate-side name;
    # pair the unchanged rich-index identities through that exact same map.
    n_alias = 0
    for tm in sorted(set(target) - paired_primary - cross_paired_mangled):
        bm = normalize_objdiff_symbols.pdb_alias_name(tm)
        if not bm or bm not in base:
            continue
        if bm in paired_primary or bm in cross_paired_mangled:
            continue
        if target[tm]["file"] != base[bm]["file"]:
            continue
        cls, t_n, b_n, n_size, n_tonly, n_bonly = classify(target[tm], base[bm])
        pair_rows.append(
            (
                sym_id[tm],
                target[tm]["rva"],
                base[bm]["rva"],
                fuzzy_by_mangled.get(tm, fuzzy_by_mangled.get(bm)),
                cls,
                t_n,
                b_n,
                n_size,
                n_tonly,
                n_bonly,
            )
        )
        cross_paired_mangled.update((tm, bm))
        n_alias += 1
    if n_alias:
        log(f"cross-name paired {n_alias} exact PDB spelling aliases")
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
    old_history, old_maxima, old_maxima_epochs = {}, {}, {}
    old_flags, old_attempts = [], []
    if DB_PATH.is_file():
        old = open_db()
        old_history = {r["mangled"]: tuple(r) for r in old.execute("SELECT * FROM history")}
        try:
            old_maxima = {
                r["mangled"]: tuple(r)
                for r in old.execute("SELECT * FROM source_maxima")
            }
        except sqlite3.OperationalError:
            old_maxima = {}  # schema 3: start MAX from current observations
        try:
            old_maxima_epochs = {
                (r["mangled"], r["effective_hash"]): tuple(r)
                for r in old.execute("SELECT * FROM source_maxima_epochs")
            }
        except sqlite3.OperationalError:
            old_maxima_epochs = {}  # schema 4: seed from valuable active rows
        old_current_fuzzy = {
            r["mangled"]: r["fuzzy_pct"]
            for r in old.execute("SELECT mangled, fuzzy_pct FROM paired")
        }
        for mangled, row in old_maxima.items():
            if not maximum_needs_epoch_archive(
                row, old_current_fuzzy.get(mangled)
            ):
                continue
            key = (mangled, row[1])
            old_maxima_epochs[key] = merge_maximum_epoch(
                old_maxima_epochs.get(key), row
            )
        old_flags = [tuple(r) for r in old.execute("SELECT * FROM flags ORDER BY mangled, flag")]
        try:
            old_attempts = [
                tuple(r) for r in old.execute("SELECT * FROM attempts ORDER BY mangled")
            ]
        except sqlite3.OperationalError:
            old_attempts = []  # pre-schema-3 DB
        old.close()

    # Schema 4 originally disambiguated placeholder overloads with target RVAs.
    # Carry persistent evidence onto the signature-derived keys introduced here.
    legacy_aliases = legacy_overload_keys(target_records)
    old_history = {
        legacy_aliases.get(key, key): (legacy_aliases.get(key, key), *row[1:])
        for key, row in old_history.items()
    }
    old_maxima = {
        legacy_aliases.get(key, key): (legacy_aliases.get(key, key), *row[1:])
        for key, row in old_maxima.items()
    }
    remapped_maxima_epochs = {}
    for (key, effective_hash), row in old_maxima_epochs.items():
        mapped = legacy_aliases.get(key, key)
        candidate = (mapped, *row[1:])
        epoch_key = (mapped, effective_hash)
        remapped_maxima_epochs[epoch_key] = merge_maximum_epoch(
            remapped_maxima_epochs.get(epoch_key), candidate
        )
    old_maxima_epochs = remapped_maxima_epochs
    old_flags = [
        (legacy_aliases.get(row[0], row[0]), *row[1:]) for row in old_flags
    ]
    old_attempts = [
        (legacy_aliases.get(row[0], row[0]), *row[1:]) for row in old_attempts
    ]

    # reconcile history: upsert every CURRENT pairing; reset rows whose source
    # extent changed ("touched" - requirement: the seen-flag dies on any edit)
    paired_at = file_mtime_iso(BASE_IDX)  # artifact time, not wall clock (determinism)
    sym_by_id = {}
    for mangled, i in sym_id.items():
        sym_by_id[i] = mangled
    touched = dropped = 0
    # paired = primary same-name pairs PLUS the cross-name dynamic-init/atexit pairs
    # (so a cross-paired base ??__E / target thunk is not re-classified as *_only).
    paired_mangled = paired_primary | cross_paired_mangled
    # cross-name pairs are keyed by the TARGET name, which is absent from `base`;
    # fall back to that pair's matched base record (located by base RVA) for the
    # source fingerprint.
    base_by_rva = {rec["rva"]: rec for rec in base.values()}
    base_rec_for = {
        sym_by_id[r[0]]: base_by_rva.get(r[2])
        for r in pair_rows
        if sym_by_id[r[0]] not in base
    }
    for row in pair_rows:
        mangled = sym_by_id[row[0]]
        fuzzy, cls = row[3], row[4]
        brec = base.get(mangled) or base_rec_for.get(mangled)
        fp = src_fingerprint(brec)
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

    # Correctness-facing MAX is separate from ordinary `history`: only
    # observations in the same effective-source epoch accumulate. A first
    # A first schema refresh seeds it from the current build, never from best-seen.
    maxima_rows = {}
    current_fuzzy_by_mangled = {}
    old_epoch_symbols = {key[0] for key in old_maxima_epochs}
    maxima_reset = maxima_raised = 0
    for row in pair_rows:
        mangled = sym_by_id[row[0]]
        fuzzy, cls = row[3], row[4]
        if fuzzy is None:
            continue
        brec = base.get(mangled) or base_rec_for.get(mangled)
        extent = _source_extent(brec)
        if extent is None:
            continue
        source_file, lo, hi, _text = extent
        units = units_by_mangled.get(mangled)
        if not units and brec is not None:
            units = units_by_mangled.get(brec["mangled"])
        module = logical_module(
            mangled, brec, units, dynamic_owners, module_overrides
        )
        effective_hash = effective_source_hash(brec, module)
        state_id = compiled_state_id(brec)
        current_exact = int(fuzzy >= 99.995)
        current_fuzzy_by_mangled[mangled] = fuzzy
        active_previous = old_maxima.get(mangled)
        previous = maximum_for_effective_hash(
            mangled, effective_hash, old_maxima, old_maxima_epochs
        )

        maximum = fuzzy
        exact_proven = current_exact
        origin, evidence = "rebuild", None
        if previous is not None and previous[1] == effective_hash:
            maximum = max(previous[2], fuzzy)
            exact_proven = int(bool(previous[3]) or current_exact)
            current_improves = maximum > previous[2] or exact_proven > previous[3]
            if not current_improves:
                state_id, origin, evidence = previous[4], previous[9], previous[10]
            if current_improves:
                maxima_raised += 1
        elif active_previous is not None or mangled in old_epoch_symbols:
            maxima_reset += 1

        maxima_rows[mangled] = (
            mangled, effective_hash, maximum, exact_proven, state_id, module,
            source_file, lo, hi, origin, evidence,
        )

    # A same-hash maximum remains valid when LTCG/ICF makes the function
    # temporarily disappear. Re-hash the retained source locator before keeping
    # it; edits or context changes retire the old epoch.
    retained_candidates = list(old_maxima.values()) + list(old_maxima_epochs.values())
    for previous in retained_candidates:
        mangled = previous[0]
        if mangled in maxima_rows:
            continue
        brec = base.get(mangled)
        module, source_file, lo, hi = previous[5:9]
        effective_hash = retained_max_effective_hash(previous, brec)
        if effective_hash == previous[1]:
            candidate = (
                *previous[:5], module, source_file, lo, hi, previous[9], previous[10]
            )
            maxima_rows[mangled] = merge_maximum_epoch(
                maxima_rows.get(mangled), candidate
            )
    maxima_rows = sorted(maxima_rows.values())
    for row in maxima_rows:
        if not maximum_needs_epoch_archive(
            row, current_fuzzy_by_mangled.get(row[0])
        ):
            continue
        key = (row[0], row[1])
        old_maxima_epochs[key] = merge_maximum_epoch(
            old_maxima_epochs.get(key), row
        )
    maxima_epoch_rows = sorted(old_maxima_epochs.values())
    if maxima_reset or maxima_raised:
        log(f"source MAX: {maxima_raised} raised, {maxima_reset} source epochs reset")

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
        folded_target = exact_paired_source_alias(
            rec,
            target_aliases_by_name,
            used_target_rvas,
            source_alias_names_by_rva=base_alias_names_by_rva,
            candidate_alias_names_by_rva=target_alias_names_by_rva,
        )
        if qn is None or mangled.startswith("??__") or "?A0x" in mangled:
            # thunks, backtick names, dynamic initializers/finalizers, anon-ns
            status, detail = "COMPILER", None
        elif rec["file"].endswith("temp_include_all.cpp"):
            status, detail = "ANCHOR", None  # our reachability scaffolding
        elif near is not None:
            status, detail = "NEAR_MISS", near
        elif folded_target is not None:
            status = "JITTER"
            detail = (
                f"exact ICF alias of {folded_target['mangled']} "
                f"at target RVA 0x{folded_target['rva']:x}"
            )
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

    # which source state was report.json BUILT from? rebuild.py records it in
    # report.head. Without the marker (older rebuild.py): CARRY FORWARD the
    # previous DB's build_head - re-assuming the current HEAD on every refresh
    # would let refresh-without-rebuild launder staleness. Only a virgin DB
    # gets the one-time current-HEAD assumption.
    head_marker = REPORT.parent / "report.head"
    if head_marker.is_file():
        build_head = head_marker.read_text().strip()
    else:
        prev = None
        if DB_PATH.is_file():
            try:
                prev_row = open_db().execute(
                    "SELECT value FROM meta WHERE key='build_head'"
                ).fetchone()
                prev = prev_row[0] if prev_row else None
            except sqlite3.OperationalError:
                prev = None
        if prev and not prev.startswith("?"):
            build_head = prev
            log("no report.head marker - carrying forward the previous build_head")
        else:
            build_head = git_head()
            log("no report.head marker (older rebuild.py) - assuming report was built at current HEAD")

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
    con.executemany("INSERT INTO source_maxima VALUES (?,?,?,?,?,?,?,?,?,?,?)", maxima_rows)
    con.executemany(
        "INSERT INTO source_maxima_epochs VALUES (?,?,?,?,?,?,?,?,?,?,?)",
        maxima_epoch_rows,
    )
    con.executemany("INSERT INTO flags VALUES (?,?,?,?)", old_flags)
    con.executemany("INSERT INTO attempts VALUES (?,?,?,?)", old_attempts)
    # deterministic meta only (artifact mtimes, not wall clock)
    con.executemany(
        "INSERT INTO meta VALUES (?,?)",
        [
            ("target_index_mtime", file_mtime_iso(TARGET_IDX)),
            ("base_index_mtime", file_mtime_iso(BASE_IDX)),
            ("report_mtime", file_mtime_iso(REPORT)),
            ("refresh_head", git_head()),
            ("build_head", build_head),
            ("declarations_loaded", "1" if (declared_methods or declared_free) else "0"),
            ("schema_version", SCHEMA_VERSION),
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


def _artifact_path(value):
    path = Path(value)
    return path if path.is_absolute() else VOSTOK / path


def rank_island_delta(
    candidate_fuzzy,
    candidate_hash,
    previous=None,
    current_fuzzy=None,
):
    """Return a prospective MAX delta, rejecting a different source epoch.

    ``previous`` is a source_maxima row (or any mapping with the same fields).
    When correctness-facing MAX evidence does not yet exist, the canonical
    physical score is the floor. Target-only symbols therefore start at zero
    instead of disappearing from island discovery.
    """
    previous_fuzzy = previous["max_fuzzy_pct"] if previous is not None else None
    previous_exact = previous["exact_proven"] if previous is not None else 0
    baseline = previous_fuzzy
    if baseline is None:
        baseline = current_fuzzy if current_fuzzy is not None else 0.0
    exact = int(candidate_fuzzy >= 99.995)
    improves = (
        candidate_fuzzy > baseline + 0.000001
        or exact > previous_exact
    )
    if not improves:
        return None
    if not candidate_hash:
        raise ValueError("candidate has no effective source hash")
    if previous is not None and previous["effective_hash"] != candidate_hash:
        raise ValueError(
            "canonical source epoch disagrees: "
            f"{previous['effective_hash']} != {candidate_hash}"
        )
    return {
        "previous_fuzzy_pct": previous_fuzzy,
        "baseline_fuzzy_pct": baseline,
        "gain_pct": candidate_fuzzy - baseline,
        "exact_proven": max(previous_exact, exact),
    }


def cmd_rank_island(args):
    """Discover source-current MAX gains in an isolated link report.

    This command never mutates match.db. Its output is a review queue for
    ``import-island``, which still requires a hash-pinned tracked manifest.
    """
    report_path = _artifact_path(args.report)
    base_index_path = _artifact_path(args.base_index)
    for label, path in (("report", report_path), ("base index", base_index_path)):
        if not path.is_file():
            sys.exit(f"[match_db] missing island {label}: {path}")

    report = json.loads(report_path.read_text())
    scores, _units = report_fuzzy_scores(report)
    target_records = load_index_records(TARGET_IDX)
    target = index_by_mangled(target_records)
    candidate_records = load_index_records(base_index_path)
    candidate = index_by_mangled(
        candidate_records,
        {mangled: rec["file"] for mangled, rec in target.items()},
    )

    con = open_db(check_schema=True)
    staleness_check(con)
    query = """
      SELECT mangled, demangled, module, fuzzy_pct AS current_fuzzy_pct
      FROM paired
      UNION ALL
      SELECT mangled, demangled, module, NULL AS current_fuzzy_pct
      FROM target_only
    """
    inventory = [dict(row) for row in con.execute(query)]
    if args.module:
        inventory = [row for row in inventory if row["module"] == args.module]

    observations = []
    measured = 0
    for item in inventory:
        mangled = item["mangled"]
        target_rec = target.get(mangled)
        candidate_rec = candidate.get(mangled)
        if target_rec is None or candidate_rec is None:
            continue
        fuzzy = island_candidate_score(
            {}, mangled, scores, target_rec, candidate_rec
        )
        if fuzzy is None:
            continue
        measured += 1

        previous = con.execute(
            "SELECT * FROM source_maxima WHERE mangled = ?", (mangled,)
        ).fetchone()
        extent = _source_extent(candidate_rec)
        candidate_hash = (
            effective_source_hash(candidate_rec, item["module"])
            if extent is not None
            else None
        )
        try:
            delta = rank_island_delta(
                fuzzy,
                candidate_hash,
                previous=previous,
                current_fuzzy=item["current_fuzzy_pct"],
            )
        except ValueError as error:
            con.close()
            sys.exit(f"[match_db] stale island candidate for {mangled}: {error}")
        if delta is None:
            continue

        source_file, lo, hi, _text = extent
        observations.append(
            {
                "mangled": mangled,
                "demangled": item["demangled"],
                "module": item["module"],
                "candidate_fuzzy_pct": fuzzy,
                "current_max_fuzzy_pct": delta["previous_fuzzy_pct"],
                "gain_pct": delta["gain_pct"],
                "exact_proven": delta["exact_proven"],
                "effective_hash": candidate_hash,
                "state_id": compiled_state_id(candidate_rec),
                "source_file": source_file,
                "source_lo": lo,
                "source_hi": hi,
            }
        )

    con.close()
    observations.sort(key=lambda row: (-row["gain_pct"], row["mangled"]))
    log(
        f"ranked {measured} measured canonical function(s): "
        f"{len(observations)} source-current MAX gain(s)"
    )
    emit(observations, args.json)


def cmd_import_cross_unit(args):
    """Import reviewed exact source-tree COMDAT evidence for target-only rows."""
    evidence = Path(args.evidence)
    if evidence.is_absolute():
        try:
            evidence = evidence.relative_to(VOSTOK)
        except ValueError:
            sys.exit("[match_db] --evidence must be inside the repository worktree")
    evidence_path = VOSTOK / evidence
    if not evidence_path.is_file():
        sys.exit(f"[match_db] missing evidence manifest: {evidence}")

    report_path = _artifact_path(args.report)
    if not report_path.is_file():
        sys.exit(f"[match_db] missing cross-unit report: {report_path}")
    manifest = json.loads(evidence_path.read_text())
    if manifest.get("kind") != "cross-unit-source-tree":
        sys.exit("[match_db] evidence kind must be 'cross-unit-source-tree'")
    expected_report_hash = manifest.get("report_sha256")
    measured_report_hash = sha256_file(report_path)
    if not expected_report_hash or expected_report_hash != measured_report_hash:
        sys.exit(
            "[match_db] report_sha256 mismatch: "
            f"expected {expected_report_hash or '<missing>'}, "
            f"measured {measured_report_hash}"
        )

    requested = manifest.get("functions")
    if not isinstance(requested, list) or not requested:
        sys.exit("[match_db] evidence manifest has no functions")
    mangled_names = [row.get("mangled") for row in requested]
    if any(not name for name in mangled_names) or len(set(mangled_names)) != len(
        mangled_names
    ):
        sys.exit("[match_db] evidence functions need unique, non-empty mangled names")

    report = json.loads(report_path.read_text())
    scores = {
        row["name"]: row["fuzzy_match_percent"]
        for row in report.get("functions", [])
        if row.get("fuzzy_match_percent") is not None
    }
    target = index_by_mangled(load_index_records(TARGET_IDX))
    con = open_db(check_schema=True)
    observations = []
    for expected in requested:
        mangled = expected["mangled"]
        target_rec = target.get(mangled)
        if target_rec is None:
            con.close()
            sys.exit(f"[match_db] cross-unit symbol absent from target index: {mangled}")
        inventory = con.execute(
            "SELECT module FROM target_only WHERE mangled = ?", (mangled,)
        ).fetchone()
        if inventory is None:
            con.close()
            sys.exit(f"[match_db] cross-unit symbol is not canonical target-only: {mangled}")
        module = inventory["module"]
        if expected.get("module") and expected["module"] != module:
            con.close()
            sys.exit(
                f"[match_db] module changed for {mangled}: "
                f"expected {expected['module']}, measured {module}"
            )

        fuzzy = cross_unit_exact_score(mangled, scores)
        expected_fuzzy = expected.get("expected_fuzzy_pct")
        if fuzzy is None or expected_fuzzy is None or abs(expected_fuzzy - fuzzy) > 0.0001:
            con.close()
            sys.exit(
                f"[match_db] cross-unit exact score changed for {mangled}: "
                f"expected {expected_fuzzy}, measured {fuzzy}"
            )

        extent = _whole_source_extent(target_rec)
        if extent is None:
            con.close()
            sys.exit(f"[match_db] cross-unit symbol has no source file: {mangled}")
        source_file, lo, hi, _text = extent
        effective_hash = effective_source_hash_at(source_file, lo, hi, module)
        expected_hash = expected.get("expected_hash")
        if not args.dry_run and not expected_hash:
            con.close()
            sys.exit(f"[match_db] evidence lacks expected_hash for: {mangled}")
        if expected_hash and expected_hash != effective_hash:
            con.close()
            sys.exit(
                f"[match_db] effective hash changed for {mangled}: "
                f"expected {expected_hash}, measured {effective_hash}"
            )

        previous = con.execute(
            "SELECT * FROM source_maxima WHERE mangled = ?", (mangled,)
        ).fetchone()
        if previous is not None and previous["effective_hash"] != effective_hash:
            con.close()
            sys.exit(
                f"[match_db] canonical source epoch disagrees for {mangled}: "
                f"{previous['effective_hash']} != {effective_hash}"
            )
        if previous is not None and previous["exact_proven"]:
            con.close()
            sys.exit(f"[match_db] cross-unit observation is not a MAX gain: {mangled}")
        observations.append(
            {
                "mangled": mangled,
                "module": module,
                "fuzzy_pct": fuzzy,
                "exact_proven": 1,
                "effective_hash": effective_hash,
                "state_id": compiled_state_id(target_rec),
                "source_file": source_file,
                "source_lo": lo,
                "source_hi": hi,
                "previous_fuzzy_pct": (
                    previous["max_fuzzy_pct"] if previous is not None else None
                ),
            }
        )

    if args.dry_run:
        con.close()
        emit(observations, True)
        return

    for row in observations:
        con.execute(
            "INSERT INTO source_maxima VALUES (?,?,?,?,?,?,?,?,?,?,?) "
            "ON CONFLICT(mangled) DO UPDATE SET "
            "effective_hash=excluded.effective_hash, "
            "max_fuzzy_pct=excluded.max_fuzzy_pct, "
            "exact_proven=excluded.exact_proven, state_id=excluded.state_id, "
            "module=excluded.module, source_file=excluded.source_file, "
            "source_lo=excluded.source_lo, source_hi=excluded.source_hi, "
            "origin=excluded.origin, evidence=excluded.evidence",
            (
                row["mangled"],
                row["effective_hash"],
                row["fuzzy_pct"],
                row["exact_proven"],
                row["state_id"],
                row["module"],
                row["source_file"],
                row["source_lo"],
                row["source_hi"],
                "cross-unit",
                evidence.as_posix(),
            ),
        )
    con.commit()
    con.close()
    log(f"imported {len(observations)} exact source-tree COMDAT observation(s)")
    emit(observations, True)


def cmd_import_island(args):
    """Import explicitly manifested observations from isolated link artifacts.

    Unlike a wholesale refresh, this preserves the canonical current pairing
    and imports only reviewed functions.  The manifest pins both artifact
    hashes plus each expected score and effective source hash, so a stale or
    accidentally regenerated island cannot silently change correctness-facing
    MAX evidence.
    """
    evidence = Path(args.evidence)
    if evidence.is_absolute():
        try:
            evidence = evidence.relative_to(VOSTOK)
        except ValueError:
            sys.exit("[match_db] --evidence must be inside the repository worktree")
    evidence_path = VOSTOK / evidence
    if not evidence_path.is_file():
        sys.exit(f"[match_db] missing evidence manifest: {evidence}")

    report_path = _artifact_path(args.report)
    base_index_path = _artifact_path(args.base_index)
    for label, path in (("report", report_path), ("base index", base_index_path)):
        if not path.is_file():
            sys.exit(f"[match_db] missing island {label}: {path}")

    manifest = json.loads(evidence_path.read_text())
    if manifest.get("kind") != "compiler-state-island":
        sys.exit("[match_db] evidence kind must be 'compiler-state-island'")
    for key, path in (
        ("report_sha256", report_path),
        ("base_index_sha256", base_index_path),
    ):
        expected = manifest.get(key)
        measured = sha256_file(path)
        if not expected or expected != measured:
            sys.exit(
                f"[match_db] {key} mismatch: expected {expected or '<missing>'}, "
                f"measured {measured}"
            )

    requested = manifest.get("functions")
    if not isinstance(requested, list) or not requested:
        sys.exit("[match_db] evidence manifest has no functions")
    mangled_names = [row.get("mangled") for row in requested]
    if any(not name for name in mangled_names) or len(set(mangled_names)) != len(mangled_names):
        sys.exit("[match_db] evidence functions need unique, non-empty mangled names")

    report = json.loads(report_path.read_text())
    scores, _units = report_fuzzy_scores(report)
    target_records = load_index_records(TARGET_IDX)
    target = index_by_mangled(target_records)
    candidate_records = load_index_records(base_index_path)
    candidate = index_by_mangled(
        candidate_records,
        {mangled: rec["file"] for mangled, rec in target.items()},
    )

    con = open_db(check_schema=True)
    observations = []
    for expected in requested:
        mangled = expected["mangled"]
        target_rec = target.get(mangled)
        candidate_rec = candidate.get(mangled)
        if target_rec is None or candidate_rec is None:
            con.close()
            sys.exit(f"[match_db] island symbol absent from target/base rich index: {mangled}")

        fuzzy = island_candidate_score(
            expected, mangled, scores, target_rec, candidate_rec
        )
        if fuzzy is None:
            con.close()
            sys.exit(f"[match_db] island report has no measured score for: {mangled}")
        expected_fuzzy = expected.get("expected_fuzzy_pct")
        if expected_fuzzy is None or abs(float(expected_fuzzy) - fuzzy) > 0.0001:
            con.close()
            sys.exit(
                f"[match_db] island score changed for {mangled}: "
                f"expected {expected_fuzzy}, measured {fuzzy}"
            )

        inventory = con.execute(
            "SELECT module FROM paired WHERE mangled = ? "
            "UNION ALL SELECT module FROM target_only WHERE mangled = ? LIMIT 1",
            (mangled, mangled),
        ).fetchone()
        if inventory is None:
            con.close()
            sys.exit(f"[match_db] symbol is not in the canonical target inventory: {mangled}")
        module = inventory["module"]
        if expected.get("module") and expected["module"] != module:
            con.close()
            sys.exit(
                f"[match_db] module changed for {mangled}: "
                f"expected {expected['module']}, measured {module}"
            )

        extent = _source_extent(candidate_rec)
        if extent is None:
            con.close()
            sys.exit(f"[match_db] island symbol has no source extent: {mangled}")
        source_file, lo, hi, _text = extent
        effective_hash = effective_source_hash(candidate_rec, module)
        expected_hash = expected.get("expected_hash")
        if not args.dry_run and not expected_hash:
            con.close()
            sys.exit(f"[match_db] evidence lacks expected_hash for: {mangled}")
        if expected_hash and expected_hash != effective_hash:
            con.close()
            sys.exit(
                f"[match_db] effective hash changed for {mangled}: "
                f"expected {expected_hash}, measured {effective_hash}"
            )

        previous = con.execute(
            "SELECT * FROM source_maxima WHERE mangled = ?", (mangled,)
        ).fetchone()
        if previous is not None and previous["effective_hash"] != effective_hash:
            con.close()
            sys.exit(
                f"[match_db] canonical source epoch disagrees for {mangled}: "
                f"{previous['effective_hash']} != {effective_hash}"
            )
        previous_fuzzy = previous["max_fuzzy_pct"] if previous is not None else None
        previous_exact = previous["exact_proven"] if previous is not None else 0
        exact = int(fuzzy >= 99.995)
        improves = (
            previous is None
            or fuzzy > previous_fuzzy + 0.000001
            or exact > previous_exact
        )
        if not improves:
            con.close()
            sys.exit(
                f"[match_db] island is not a MAX improvement for {mangled}: "
                f"candidate {fuzzy:.6f}, current {previous_fuzzy:.6f}"
            )
        observations.append(
            {
                "mangled": mangled,
                "module": module,
                "fuzzy_pct": fuzzy,
                "exact_proven": max(previous_exact, exact),
                "effective_hash": effective_hash,
                "state_id": compiled_state_id(candidate_rec),
                "source_file": source_file,
                "source_lo": lo,
                "source_hi": hi,
                "previous_fuzzy_pct": previous_fuzzy,
            }
        )

    if args.dry_run:
        con.close()
        emit(observations, True)
        return

    for row in observations:
        con.execute(
            "INSERT INTO source_maxima VALUES (?,?,?,?,?,?,?,?,?,?,?) "
            "ON CONFLICT(mangled) DO UPDATE SET "
            "effective_hash=excluded.effective_hash, "
            "max_fuzzy_pct=excluded.max_fuzzy_pct, "
            "exact_proven=excluded.exact_proven, state_id=excluded.state_id, "
            "module=excluded.module, source_file=excluded.source_file, "
            "source_lo=excluded.source_lo, source_hi=excluded.source_hi, "
            "origin=excluded.origin, evidence=excluded.evidence",
            (
                row["mangled"], row["effective_hash"], row["fuzzy_pct"],
                row["exact_proven"], row["state_id"], row["module"],
                row["source_file"], row["source_lo"], row["source_hi"],
                "island", evidence.as_posix(),
            ),
        )
    con.commit()
    con.close()
    log(f"imported {len(observations)} function-scoped island MAX observation(s)")
    emit(observations, True)


def cmd_record_max(args):
    """Attach island provenance to an already measured MAX observation.

    The score, effective hash, exact proof, and state identity must first have
    been derived by `refresh` from real candidate artifacts. This command never
    accepts a score and therefore cannot manufacture a maximum.
    """
    con = open_db(check_schema=True)
    row = con.execute(
        "SELECT effective_hash, max_fuzzy_pct, state_id FROM source_maxima "
        "WHERE mangled = ?",
        (args.mangled,),
    ).fetchone()
    if row is None:
        con.close()
        sys.exit("[match_db] no measured source_maxima row for that symbol")
    if args.expected_hash and args.expected_hash != row["effective_hash"]:
        con.close()
        sys.exit(
            f"[match_db] effective hash changed: expected {args.expected_hash}, "
            f"measured {row['effective_hash']}"
        )
    evidence = Path(args.evidence)
    if evidence.is_absolute():
        try:
            evidence = evidence.relative_to(VOSTOK)
        except ValueError:
            con.close()
            sys.exit("[match_db] --evidence must be inside the repository worktree")
    evidence_path = VOSTOK / evidence
    if not evidence_path.exists():
        con.close()
        sys.exit(f"[match_db] missing evidence path: {evidence}")
    con.execute(
        "UPDATE source_maxima SET origin = 'island', evidence = ? WHERE mangled = ?",
        (evidence.as_posix(), args.mangled),
    )
    con.commit()
    con.close()
    log(
        f"recorded island provenance for {args.mangled}: "
        f"{row['max_fuzzy_pct']:.4f}% state={row['state_id']}"
    )


# Display caps for the function column: a demangled signature splits into a return
# type and name+args; cap each so a boost/asio template monster can't blow out the
# table. JSON output stays full (the raw name). Tune here.
RET_MAX = 24   # return type
SIG_MAX = 80   # name + args (+ trailing const, etc.)


def _split_return(dem):
    """(return_type, name+args) for a demangled signature. The arg list opens at the
    first '(' at angle-bracket depth 0; the return type ends at the nearest depth-0
    space to its left. Ctors/dtors (no return type) yield ('', dem)."""
    depth, paren = 0, -1
    for i, ch in enumerate(dem):
        if ch == "<":
            depth += 1
        elif ch == ">":
            depth -= 1
        elif ch == "(" and depth == 0:
            paren = i
            break
    if paren < 0:
        return "", dem
    depth = 0
    for i in range(paren - 1, -1, -1):
        ch = dem[i]
        if ch == ">":
            depth += 1
        elif ch == "<":
            depth -= 1
        elif ch == " " and depth == 0:
            return dem[:i], dem[i + 1:]
    return "", dem


def shorten_fn(dem):
    """Cap the return type and the name+args independently (RET_MAX / SIG_MAX)."""
    ret, rest = _split_return(dem)
    if len(ret) > RET_MAX:
        ret = ret[:RET_MAX - 1] + "…"
    if len(rest) > SIG_MAX:
        rest = rest[:SIG_MAX - 1] + "…"
    return f"{ret} {rest}".strip()


_MANGLED_OPS = {
    "2": "operator new", "3": "operator delete", "4": "operator=",
    "8": "operator==", "9": "operator!=", "A": "operator[]",
    "R": "operator()", "E": "operator++", "F": "operator--",
    "_7": "`vftable'", "_8": "`vbtable'",
}


def _scope_tokens(s):
    """`@`-separated scope identifiers up to the terminating `@@`."""
    toks = []
    for t in s.split("@"):
        if t == "":
            break
        toks.append(t)
    return toks


def _name_from_demangled(dem):
    """Clean 'scope::name' from a demangled signature: drop the return type, every
    <template-arg> block (any depth), and the parameter list. Robust where the
    mangled name has templated scopes the lightweight parser can't walk."""
    if not dem:
        return ""
    _, rest = _split_return(dem)
    out, depth = [], 0
    for ch in rest:                          # strip balanced <...>
        if ch == "<":
            depth += 1
        elif ch == ">":
            depth = max(0, depth - 1)
        elif depth == 0:
            out.append(ch)
    s = "".join(out)
    rp = s.rfind(")")                         # cut the parameter list
    if rp != -1:
        depth = 0
        for i in range(rp, -1, -1):
            if s[i] == ")":
                depth += 1
            elif s[i] == "(":
                depth -= 1
                if depth == 0:
                    s = s[:i]
                    break
    return s.strip()


def _looks_clean(name):
    """A parsed qualified name with no leftover mangling artifacts."""
    return bool(name) and not any(c in name for c in "?$@")


def fn_from_mangled(mangled, demangled=""):
    """A short 'scope::name' that reads far easier than the demangled signature.
    Parsed straight from the MSVC mangled name for the simple cases; for a template
    function or a templated scope (a '?$' - nested type encodings the lightweight
    parser can't skip) it falls back to stripping <...>+params off the demangled."""
    try:
        m = mangled
        if not m.startswith("?"):
            return m  # already a plain qualified name (free functions, etc.)
        body = m[1:]
        if "?$" in body:                      # template fn / templated scope
            return _name_from_demangled(demangled) or body
        if body.startswith("?"):              # ctor / dtor / operator
            body = body[1:]
            code = body[:2] if body[:1] == "_" else body[:1]
            toks = _scope_tokens(body[len(code):].lstrip("@"))
            if not toks:
                raise ValueError
            cls = toks[0]
            leaf = (cls if code == "0"
                    else "~" + cls if code in ("1", "_G", "_E")
                    else _MANGLED_OPS.get(code, "operator" + code))
            name = "::".join(reversed(toks)) + "::" + leaf
        else:                                 # normal function
            base, _, rest = body.partition("@")
            toks = _scope_tokens(rest)
            name = ("::".join(reversed(toks)) + "::" + base) if toks else base
        return name if _looks_clean(name) else (_name_from_demangled(demangled) or name)
    except Exception:
        return _name_from_demangled(demangled) or shorten_fn(demangled)


def resolve_units(con, partial, module=None):
    """Unit names whose path CONTAINS `partial` (case-insensitive), optionally
    scoped to a module. A full path is a unique substring, so it resolves to one."""
    q, params = "SELECT name FROM units WHERE name LIKE ?", [f"%{partial}%"]
    if module:
        q, params = q + " AND module = ?", params + [module]
    return [r["name"] for r in con.execute(q + " ORDER BY name", params)]


def cmd_report(args):
    con = open_db(check_schema=True)
    staleness_check(con)

    # --function: every target fn whose demangled name CONTAINS the substring
    # (e.g. 'medkit::'), across units; same columns as --per-function plus a unit
    # column. --module optional; lists ALL matches (no ambiguity refusal).
    if args.function:
        mod = "AND t.module = ?" if args.module else ""
        mparams = [args.module] if args.module else []
        like = f"%{args.function}%"
        hdr = con.execute(
            f"""SELECT count(*) AS n,
                       printf('%.2f', sum(coalesce(p.fuzzy_pct, 0) * t.size) /
                                      nullif(sum(t.size), 0))        AS w,
                       printf('%.2f', avg(coalesce(p.fuzzy_pct, 0))) AS a
                FROM target_functions t JOIN symbols s ON s.id = t.sym
                LEFT JOIN pairs p ON p.sym = t.sym
                WHERE s.demangled LIKE ? {mod}""",
            [like] + mparams).fetchone()
        if not hdr["n"]:
            sys.exit(f"[match_db] no function matches '{args.function}'"
                     + (f" in module {args.module}" if args.module else ""))
        if not args.json:
            print(f"[match_db] {hdr['n']} fn(s) matching '{args.function}': "
                  f"weighted {hdr['w']}%  avg {hdr['a']}%", file=sys.stderr)
        ffq = f"""
          SELECT CASE WHEN p.fuzzy_pct IS NULL THEN NULL
                      ELSE printf('%.2f', p.fuzzy_pct) END        AS pct,
                 CASE WHEN h.best_fuzzy_pct IS NULL THEN NULL
                      ELSE printf('%.2f', h.best_fuzzy_pct) END   AS best,
                 coalesce(a.n, 0)                                 AS tries,
                 coalesce(p.struct_class, '-')                    AS cls,
                 t.size                                           AS size,
                 coalesce((SELECT group_concat(flag, '+') FROM flags
                           WHERE mangled = s.mangled), '-')       AS flag,
                 -- the source FILE (== the .cpp for a TU fn, the header for an
                 -- inline one); prefer it over the TU, strip the 'vostok/' prefix
                 CASE WHEN coalesce(fl.path, u.name) LIKE 'vostok/%'
                      THEN substr(coalesce(fl.path, u.name), 8)
                      ELSE coalesce(fl.path, u.name, '(no unit)') END AS file,
                 s.demangled                                      AS fn,
                 s.mangled                                        AS m
          FROM target_functions t
          JOIN symbols s ON s.id = t.sym
          LEFT JOIN units u ON u.id = t.unit
          LEFT JOIN files fl ON fl.id = t.file
          LEFT JOIN pairs p ON p.sym = t.sym
          LEFT JOIN history h ON h.mangled = s.mangled
          LEFT JOIN attempts a ON a.mangled = s.mangled
          WHERE s.demangled LIKE ? {mod}
          ORDER BY p.fuzzy_pct DESC, t.size DESC
        """
        rows = [dict(r) for r in con.execute(ffq, [like] + mparams)]
        for r in rows:
            m = r.pop("m")
            if not args.json:
                r["fn"] = (shorten_fn(r["fn"]) if args.verbose
                           else fn_from_mangled(m, r["fn"]))
        emit(rows, args.json)
        return

    # --unit: fuzzy-resolve to exactly ONE unit; refuse (with paste-ready full
    # names) on ambiguity. --module is optional - a unit substring stands alone.
    unit_full = None
    if args.unit:
        matches = resolve_units(con, args.unit, args.module)
        if not matches:
            sys.exit(f"[match_db] no unit matches '{args.unit}'"
                     + (f" in module {args.module}" if args.module else ""))
        if len(matches) > 1:
            sys.exit("Multiple units found:\n"
                     + "\n".join(f"  --unit {m}" for m in matches))
        unit_full = matches[0]

    # --per-function: list every function of ONE unit (pct / cls / size / flag),
    # sorted 100%->0% like the unit rollup; NULL pct = unpaired/open.
    if args.per_function:
        if not unit_full:
            sys.exit("[match_db] --per-function needs --unit (one unit)")
        if not args.json:  # unit-level weighted + avg % as a header (terminal only)
            s = con.execute(
                """SELECT printf('%.2f', sum(coalesce(p.fuzzy_pct, 0) * t.size) /
                                         sum(t.size))          AS w,
                          printf('%.2f', avg(coalesce(p.fuzzy_pct, 0))) AS a
                   FROM target_functions t LEFT JOIN pairs p ON p.sym = t.sym
                   JOIN units u ON u.id = t.unit WHERE u.name = ?""",
                [unit_full]).fetchone()
            print(f"[match_db] {unit_full}: weighted {s['w']}%  avg {s['a']}%",
                  file=sys.stderr)
        # best = best-ever fuzzy (history); best==100 with pct<100 is a TRANSIENT
        # (regressed) match. tries = how many matcher dispatches included this fn.
        fq = """
          SELECT CASE WHEN p.fuzzy_pct IS NULL THEN NULL
                      ELSE printf('%.2f', p.fuzzy_pct) END        AS pct,
                 CASE WHEN h.best_fuzzy_pct IS NULL THEN NULL
                      ELSE printf('%.2f', h.best_fuzzy_pct) END   AS best,
                 coalesce(a.n, 0)                                 AS tries,
                 coalesce(p.struct_class, '-')                    AS cls,
                 t.size                                           AS size,
                 coalesce((SELECT group_concat(flag, '+') FROM flags
                           WHERE mangled = s.mangled), '-')       AS flag,
                 s.demangled                                      AS fn,
                 s.mangled                                        AS m
          FROM target_functions t
          JOIN symbols s ON s.id = t.sym
          JOIN units u ON u.id = t.unit
          LEFT JOIN pairs p ON p.sym = t.sym
          LEFT JOIN history h ON h.mangled = s.mangled
          LEFT JOIN attempts a ON a.mangled = s.mangled
          WHERE u.name = ?
          ORDER BY p.fuzzy_pct DESC, t.size DESC
        """
        rows = [dict(r) for r in con.execute(fq, [unit_full])]
        for r in rows:
            m = r.pop("m")
            if not args.json:
                r["fn"] = (shorten_fn(r["fn"]) if args.verbose
                           else fn_from_mangled(m, r["fn"]))
        emit(rows, args.json)
        return

    per_unit = args.per_unit or unit_full is not None
    lite = args.lite or unit_full is not None  # one unit -> always the lean view
    scope = "unit_name" if per_unit else "module"
    if unit_full:
        where, params = "WHERE tf.unit_name = ?", [unit_full]
    elif args.module:
        where, params = "WHERE tf.module = ?", [args.module]
    else:
        where, params = "", []
    q = f"""
      WITH tf AS (
        SELECT t.rva, t.sym, t.size, t.frameless,
               coalesce(t.module, '(no unit)') AS module,
               coalesce(fl.path, un.name, '(no unit)') AS unit_name
        FROM target_functions t
        LEFT JOIN units un ON un.id = t.unit
        LEFT JOIN files fl ON fl.id = t.file)
      SELECT tf.{scope} AS scope,
             count(*)                                            AS target_fns,
             coalesce(sum(p.sym IS NOT NULL), 0)                 AS paired,
             coalesce(sum(p.fuzzy_pct >= 100), 0)                AS fuzzy_100,
             coalesce(sum(p.struct_class = 'MATCH'), 0)          AS struct_match,
             coalesce(sum(p.sym IS NULL), 0)                     AS target_only,
             coalesce(sum(tf.frameless), 0)                      AS custom_conv,
             printf('%.2f', sum(coalesce(p.fuzzy_pct, 0) * tf.size) /
                            sum(tf.size))                        AS weighted_pct,
             printf('%.2f', avg(coalesce(p.fuzzy_pct, 0)))       AS avg_pct
      FROM tf
      LEFT JOIN pairs p ON p.sym = tf.sym
      {where}
      GROUP BY scope
      ORDER BY sum(coalesce(p.fuzzy_pct, 0) * tf.size) / sum(tf.size) DESC, scope
    """
    rows = [dict(r) for r in con.execute(q, params)]
    # The base-only/out_of_scope/suspicious lints only ever scope BY MODULE; for a
    # single --unit they compute module-wide (or repo-wide) and the per-scope merge
    # below picks the row - and the lean --unit/--lite view drops them anyway.
    # per-unit scope prefers the source FILE (so header inlines show their .h, not
    # one '(no unit)' lump); module scope stays the module. The base_only/target_only
    # views carry both `file` and `unit`, so the counts merge against the same key.
    scope_expr = ("coalesce(file, unit, '(no unit)')" if per_unit
                  else "coalesce(module, '(no unit)')")
    sub_where = "WHERE module = ?" if args.module else ""
    sub_extra = "AND module = ?" if args.module else ""
    sub_params = [args.module] if args.module else []
    # base-only lint per scope
    bq = """SELECT {scope} AS scope, count(*) AS base_only
            FROM base_only {where} GROUP BY scope""".format(
        scope=scope_expr, where=sub_where)
    bonly = {r["scope"]: r["base_only"] for r in con.execute(bq, sub_params)}
    # out_of_scope: target-only functions whose history row survived (paired
    # once, vanished without a source touch)
    oq = """
      SELECT {scope} AS scope, count(*) AS n FROM target_only
      WHERE mangled IN (SELECT mangled FROM history) {extra} GROUP BY scope
    """.format(scope=scope_expr, extra=sub_extra)
    oos = {r["scope"]: r["n"] for r in con.execute(oq, sub_params)}
    # the fabricated-symbol lint: base-only rows nothing explains
    uq = """
      SELECT {scope} AS scope, count(*) AS n
      FROM base_only b JOIN base_only_status st ON st.mangled = b.mangled
      WHERE st.status IN ('UNEXPLAINED', 'NEAR_MISS')
        AND b.mangled NOT IN (
          SELECT mangled FROM flags WHERE flag = 'OUT_OF_SCOPE'
        ) {extra} GROUP BY scope
    """.format(scope=scope_expr, extra=sub_extra)
    suspicious = {r["scope"]: r["n"] for r in con.execute(uq, sub_params)}
    for r in rows:
        r["base_only"] = bonly.get(r["scope"], 0)
        r["suspicious"] = suspicious.get(r["scope"], 0)
        r["out_of_scope"] = oos.get(r["scope"], 0)
    if lite:
        rows = [r for r in rows if r["scope"] != "(no unit)"]
        for r in rows:
            for c in ("custom_conv", "out_of_scope", "suspicious"):
                r.pop(c, None)
    emit(rows, args.json)


def _db_blob_at(rev):
    """(path, cleanup_path) for a readable match.db at git `rev`. Empty rev = the
    on-disk current DB (cleanup None). Else extract the committed blob to a tempfile."""
    if not rev:
        return str(DB_PATH), None
    import subprocess
    import tempfile

    rel = "docs/binary_matching/match.db"
    fd, tmp = tempfile.mkstemp(prefix="match_db_", suffix=".db")
    os.close(fd)
    with open(tmp, "wb") as fh:
        r = subprocess.run(
            ["git", "-C", str(VOSTOK), "show", f"{rev}:{rel}"],
            stdout=fh, stderr=subprocess.PIPE,
        )
    if r.returncode != 0:
        os.unlink(tmp)
        sys.exit(f"[match_db] cannot read {rel} at '{rev}': "
                 f"{r.stderr.decode(errors='replace').strip()}")
    return tmp, tmp


def _fn_state(db_path, module):
    """{mangled: row(dem, pct, cls, loc, best, tries)} over a match.db's target fns.
    Tolerates older schemas that lack the history/attempts tables (best=NULL,tries=0)."""
    con = sqlite3.connect(f"file:{db_path}?mode=ro", uri=True)
    con.row_factory = sqlite3.Row
    have = {r[0] for r in con.execute(
        "SELECT name FROM sqlite_master WHERE type='table'")}
    best = "h.best_fuzzy_pct" if "history" in have else "NULL"
    tries = "coalesce(a.n, 0)" if "attempts" in have else "0"
    hist = "LEFT JOIN history h ON h.mangled = s.mangled" if "history" in have else ""
    att = "LEFT JOIN attempts a ON a.mangled = s.mangled" if "attempts" in have else ""
    where = "WHERE t.module = ?" if module else ""
    rows = con.execute(
        f"""SELECT s.mangled AS m, s.demangled AS dem, p.fuzzy_pct AS pct,
                   p.struct_class AS cls, {best} AS best, {tries} AS tries,
                   coalesce(fl.path, un.name, '(no unit)') AS loc
            FROM symbols s
            JOIN target_functions t ON t.sym = s.id
            LEFT JOIN units un ON un.id = t.unit
            LEFT JOIN files fl ON fl.id = t.file
            LEFT JOIN pairs p ON p.sym = s.id
            {hist}
            {att}
            {where}""",
        ([module] if module else []),
    ).fetchall()
    con.close()
    return {r["m"]: r for r in rows}


def cmd_diff(args):
    """Function-level diff between two committed match.db revisions (or a rev vs the
    working tree): what each target function's fuzzy_pct / struct_class did."""
    spec = args.spec
    a_rev, b_rev = spec.split("..", 1) if ".." in spec else (spec, "")
    if not a_rev:
        sys.exit("[match_db] diff needs a base rev: <hash> or <hash>..<hash>")
    a_path, a_tmp = _db_blob_at(a_rev)
    b_path, b_tmp = _db_blob_at(b_rev)
    try:
        a_state = _fn_state(a_path, args.module)
        b_state = _fn_state(b_path, args.module)
    finally:
        for t in (a_tmp, b_tmp):
            if t:
                os.unlink(t)

    eps = 0.01
    # ORDER drives both the printed summary and row grouping.
    ORDER = ["REGRESS", "LOST", "NEW", "IMPROVE", "TOUCHED", "RECLASS"]
    cats = {k: [] for k in ORDER}
    for m in set(a_state) | set(b_state):
        a, b = a_state.get(m), b_state.get(m)
        ap, bp = (a["pct"] if a else None), (b["pct"] if b else None)
        ac, bc = (a["cls"] if a else None), (b["cls"] if b else None)
        at, bt = (a["tries"] if a else 0), (b["tries"] if b else 0)
        amx, bmx = (a["best"] if a else None), (b["best"] if b else None)
        rec = {"m": m, "dem": (b or a)["dem"], "loc": (b or a)["loc"],
               "ap": ap, "bp": bp, "ac": ac, "bc": bc, "amx": amx, "bmx": bmx,
               "at": at, "bt": bt}
        if ap is None and bp is None:
            kind, srt = ("TOUCHED", bt - at) if bt > at else (None, 0)
        elif ap is None:
            kind, srt = "NEW", bp
        elif bp is None:
            kind, srt = "LOST", ap
        elif bp - ap > eps:
            kind, srt = "IMPROVE", bp - ap
        elif ap - bp > eps:
            kind, srt = "REGRESS", bp - ap
        elif ac != bc:
            kind, srt = "RECLASS", ap or 0
        elif bt > at:                       # worked but % + structure unchanged
            kind, srt = "TOUCHED", bt - at
        else:
            kind = None
        if kind:
            rec["srt"] = srt
            cats[kind].append(rec)
    for k in ORDER:                          # REGRESS most-negative first; rest desc
        cats[k].sort(key=lambda r: r["srt"], reverse=(k != "REGRESS"))

    b_label = b_rev or "WORKTREE"
    if args.json:
        def js(r):
            return {"from": r["ap"], "to": r["bp"],
                    "max_from": r["amx"], "max_to": r["bmx"],
                    "from_cls": r["ac"], "to_cls": r["bc"],
                    "tries_from": r["at"], "tries_to": r["bt"],
                    "fn": r["dem"], "file": r["loc"]}
        print(json.dumps({"a": a_rev, "b": b_label, "module": args.module,
                          **{k.lower(): [js(r) for r in cats[k]] for k in ORDER}},
                         indent=1))
        return

    def strip(loc):
        return loc[7:] if loc.startswith("vostok/") else loc

    def clsfmt(ac, bc):
        return (ac or "-") if ac == bc else f"{ac or ''}->{bc or ''}"

    def pf(x):
        return f"{x:.1f}" if x is not None else "-"

    print(f"[match_db] diff {a_rev} -> {b_label}"
          + (f"  (module {args.module})" if args.module else ""))
    print("  " + "  ".join(f"{k.lower()} {len(cats[k])}" for k in ORDER))

    DTAG = {"LOST": "gone", "NEW": "new", "TOUCHED": "~tries", "RECLASS": "~cls"}
    rows = []
    for k in ORDER:
        for r in cats[k]:
            d = (f"{r['bp'] - r['ap']:+.2f}" if k in ("REGRESS", "IMPROVE")
                 else DTAG[k])
            tries = (f"{r['at']}->{r['bt']}" if r["bt"] != r["at"] else str(r["bt"]))
            # max DROP (amx > bmx) = the fn was RE-WORKED (best reset to current); show
            # it as from->to so a re-match is unmistakable vs a non-steerable LTO drop.
            mx = (f"{pf(r['amx'])}->{pf(r['bmx'])}"
                  if r["amx"] is not None and r["bmx"] is not None and r["amx"] > r["bmx"] + 0.01
                  else pf(r["bmx"] if r["bmx"] is not None else r["amx"]))
            rows.append({
                "kind": k, "d": d, "from": pf(r["ap"]), "to": pf(r["bp"]),
                "max": mx, "cls": clsfmt(r["ac"], r["bc"]), "tries": tries,
                "file": strip(r["loc"]),
                "fn": (shorten_fn(r["dem"]) if args.verbose
                       else fn_from_mangled(r["m"], r["dem"])),
            })
    if not rows:
        print("  (no function-level differences)")
    else:
        emit(rows, False)


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
    skipped. TUs are ordered LOWEST match level first (real unmatched code
    before 99% polish - that tail is verifier work), size breaking ties."""
    con = open_db(check_schema=True)
    staleness_check(con, strict=not args.stale_ok)
    where, params = ["module = ?"], [args.module]
    if not args.include_frameless:
        where.append("frameless = 0")
    q = f"""
      WITH cand AS (
        SELECT s.demangled, s.mangled, coalesce(u.name,'(no unit)') AS unit,
               t.module, t.size, t.frameless, p.fuzzy_pct, p.struct_class,
               h.best_fuzzy_pct AS best,
               coalesce(a.n, 0) AS attempts,
               fl.path AS def_file,
               bos.mangled AS near_miss_base,
               CASE WHEN p.sym IS NULL AND bos.mangled IS NOT NULL THEN 'NEAR_MISS'
                    WHEN p.sym IS NULL THEN 'TARGET_ONLY'
                    ELSE 'PAIRED' END AS presence
        FROM target_functions t
        JOIN symbols s ON s.id = t.sym
        LEFT JOIN units u ON u.id = t.unit
        LEFT JOIN pairs p ON p.sym = t.sym
        LEFT JOIN history h ON h.mangled = s.mangled
        LEFT JOIN attempts a ON a.mangled = s.mangled
        LEFT JOIN files fl ON fl.id = t.file
        -- a base-only NEAR_MISS row's detail IS this target symbol: the body
        -- exists under a different mangling (access/const) - a header fix
        LEFT JOIN base_only_status bos
               ON bos.detail = s.mangled AND bos.status = 'NEAR_MISS'
        -- skip near-ceiling fns whose STRUCTURE already matches (MATCH/SIZE/SPLIT all
        -- have the target's statement count; the residual is non-steerable LTCG -
        -- inline-vs-call / reg-alloc). Don't hand an effectively-done fn to a matcher.
        -- Keyed on MAX (best-ever %, not current): an LTO drop below the peak is itself
        -- non-steerable, so a fn that ONCE reached >=95 structure-matched stays skipped -
        -- UNLESS its source was edited, which resets best to current (refresh), re-queuing
        -- it. QUANTITY (wrong statement count, incl. the high-%-over-wrong-structure trap)
        -- STAYS at any %, as do all fns whose max is below 95%.
        WHERE NOT (coalesce(h.best_fuzzy_pct, p.fuzzy_pct, 0) >= 95 AND coalesce(p.struct_class, '') != 'QUANTITY')
          AND s.mangled NOT IN (SELECT mangled FROM flags WHERE flag IN ('SKIP','OUT_OF_SCOPE'))
          -- compiler-generated machinery is not source-steerable standalone:
          -- deleting dtors / dynamic initializers (backtick names), thunks,
          -- anonymous-namespace symbols
          AND s.demangled NOT LIKE '%`%'
          AND s.demangled NOT LIKE '[thunk]%'
          AND s.mangled NOT LIKE '%?A0x%'
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
    src_cache = {}

    def body_in_source(row):
        """Cheap dispatch hint for unpaired rows: does the defining file
        already name this function with an argument list? Distinguishes
        'write it' from 'it exists - check the anchor / mangling first'."""
        if row["presence"] == "PAIRED" or not row["def_file"]:
            return None
        path = VOSTOK / "sources" / row["def_file"]
        if path not in src_cache:
            try:
                src_cache[path] = path.read_text(encoding="latin-1")
            except OSError:
                src_cache[path] = ""
        qn = qualified_name(row["demangled"] or "")
        if not qn:
            return None
        short = qn[1].split("<", 1)[0]
        return f"{short}(" in src_cache[path] or f"{short} (" in src_cache[path]

    by_unit = {}
    for r in con.execute(q, params):
        row = dict(r)
        row["body_in_source"] = body_in_source(row)
        by_unit.setdefault(host(row["unit"], row["demangled"]), []).append(row)

    def matched_pct(fns):
        """Size-weighted match level by MAX (best-ever %), NOT current: an LTO drop below
        a fn's peak is non-steerable, so it must NOT make its TU look urgent. A genuine
        re-match resets best to current (refresh), so real regressions do re-prioritize.
        TARGET_ONLY counts as 0%."""
        total = sum(f["size"] for f in fns)
        return sum((f["best"] or f["fuzzy_pct"] or 0) * f["size"] for f in fns) / total if total else 0.0

    # UNTRIED work first: a TU whose open functions were ALL dispatched before
    # is demoted by its least-tried function's attempt count, so parked walls
    # stop jumping to the front but still come back later (sushi, 2026-06-13).
    # Then LOWEST match level (real unmatched code beats polishing 99% TUs),
    # real .cpp TUs before header-only batches, size ascending.
    units = sorted(
        by_unit.items(),
        key=lambda kv: (
            min(f["attempts"] for f in kv[1]),
            matched_pct(kv[1]),
            0 if kv[0].endswith(".cpp") else 1,
            sum(f["size"] for f in kv[1]),
            kv[0],
        ),
    )
    if args.limit:
        units = units[: args.limit]

    if args.json:
        out = [
            {
                "unit": unit,
                "total_size": sum(f["size"] for f in fns),
                "matched_pct": round(matched_pct(fns), 2),
                "functions": [
                    {
                        k: f[k]
                        for k in (
                            "demangled", "mangled", "unit", "size", "fuzzy_pct",
                            "struct_class", "presence", "attempts",
                            "near_miss_base", "body_in_source",
                        )
                    }
                    for f in fns
                ],
            }
            for unit, fns in units
        ]
        print(json.dumps(out, indent=1))
        return
    for unit, fns in units:
        total = sum(f["size"] for f in fns)
        print(f"=== {unit}: {len(fns)} functions, {total:#x} bytes, {matched_pct(fns):.1f}% matched")
        for f in fns:
            pct = "-" if f["fuzzy_pct"] is None else f"{f['fuzzy_pct']:.1f}"
            via = "" if f["unit"] == unit else f"   [defined in {f['unit']}]"
            tried = f"  tried:{f['attempts']}" if f["attempts"] else ""
            hints = ""
            if f["near_miss_base"]:
                b = f["near_miss_base"]
                tgt = f["mangled"]
                cut = b.find("@@")
                hints = (
                    f"   [NEAR_MISS: body exists as @@{b[cut + 2:]} - change access/const "
                    f"to match target @@{tgt[tgt.find('@@') + 2:]}]"
                )
            elif f["body_in_source"]:
                hints = "   [body already in source - check anchor/mangling first]"
            print(
                f"  {f['size']:>6}  {pct:>6}  {f['struct_class'] or f['presence']:<11}  "
                f"{f['demangled'][:110]}{via}{tried}{hints}"
            )


def cmd_tried(args):
    """Record that a dispatch included these functions (orchestrator, after
    each worker returns). Increments per-function attempt counts; the queue
    demotes fully-tried TUs so they come back later instead of being retried
    first. --unit marks EVERY function of a TU - even ones already at 100% - so a
    later `diff` shows the whole TU was touched, not just the functions that moved."""
    import datetime

    con = open_db(check_schema=True)
    today = datetime.date.today().isoformat()
    mangleds = list(args.mangled)
    if args.unit:
        q = """SELECT s.mangled FROM target_functions t JOIN symbols s ON s.id = t.sym
               LEFT JOIN units u ON u.id = t.unit WHERE u.name = ?"""
        mangleds += [r["mangled"] for r in con.execute(q, (args.unit,))]
    if args.done:
        # every fn that EVER reached 100% (by max) and isn't tried yet -> tries=1, so
        # the done set drops out of the queue until everything else is exhausted.
        q = """SELECT s.mangled FROM target_functions t JOIN symbols s ON s.id = t.sym
               LEFT JOIN pairs p ON p.sym = t.sym
               LEFT JOIN history h ON h.mangled = s.mangled
               LEFT JOIN attempts a ON a.mangled = s.mangled
               WHERE coalesce(h.best_fuzzy_pct, p.fuzzy_pct, 0) >= 100 AND a.mangled IS NULL"""
        mangleds += [r["mangled"] for r in con.execute(q)]
    if not mangleds:
        sys.exit("[match_db] nothing to mark - pass mangled names and/or --unit / --done")
    for m in mangleds:
        con.execute(
            """INSERT INTO attempts VALUES (?, 1, ?, ?)
               ON CONFLICT(mangled) DO UPDATE SET
                 n = n + 1, last_at = excluded.last_at,
                 note = coalesce(excluded.note, note)""",
            (m, today, args.note),
        )
    con.commit()
    log(f"marked {len(mangleds)} function(s) tried")


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


def merge_persistent_maxima(con, other):
    """Union active and archived hash-scoped MAX observations.

    The receiving database keeps its active row when the incoming row belongs
    to another source epoch; that incoming proof is archived so the next
    refresh can reactivate it if its effective hash is current.
    """
    try:
        con.execute("SELECT 1 FROM source_maxima_epochs LIMIT 1")
        other.execute("SELECT 1 FROM source_maxima LIMIT 1")
    except sqlite3.OperationalError:
        return 0

    merged_count = 0

    def merge_epoch(incoming):
        nonlocal merged_count
        key = incoming[0], incoming[1]
        current = con.execute(
            "SELECT * FROM source_maxima_epochs "
            "WHERE mangled = ? AND effective_hash = ?",
            key,
        ).fetchone()
        merged = merge_maximum_epoch(tuple(current) if current else None, incoming)
        if current is None or tuple(current) != merged:
            con.execute(
                "INSERT OR REPLACE INTO source_maxima_epochs VALUES "
                "(?,?,?,?,?,?,?,?,?,?,?)",
                merged,
            )
            merged_count += 1

    try:
        incoming_epochs = other.execute("SELECT * FROM source_maxima_epochs")
        for row in incoming_epochs:
            merge_epoch(tuple(row))
    except sqlite3.OperationalError:
        pass  # schema 4: active rows below carry its only MAX evidence

    for row in other.execute("SELECT * FROM source_maxima"):
        incoming = tuple(row)
        current = con.execute(
            "SELECT * FROM source_maxima WHERE mangled = ?",
            (row["mangled"],),
        ).fetchone()
        if current is None:
            con.execute(
                "INSERT INTO source_maxima VALUES (?,?,?,?,?,?,?,?,?,?,?)",
                incoming,
            )
            merged_count += 1
        elif current["effective_hash"] == row["effective_hash"]:
            merged = merge_maximum_epoch(tuple(current), incoming)
            if tuple(current) != merged:
                con.execute(
                    "INSERT OR REPLACE INTO source_maxima VALUES "
                    "(?,?,?,?,?,?,?,?,?,?,?)",
                    merged,
                )
                merged_count += 1
        else:
            merge_epoch(incoming)

    return merged_count


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
    na = 0
    try:
        for r in other.execute("SELECT * FROM attempts"):
            na += con.execute(
                """INSERT INTO attempts VALUES (?,?,?,?)
                   ON CONFLICT(mangled) DO UPDATE SET
                     n = max(n, excluded.n),
                     last_at = max(last_at, excluded.last_at),
                     note = coalesce(excluded.note, note)""",
                tuple(r),
            ).rowcount
    except sqlite3.OperationalError:
        pass  # other DB predates attempts
    nm = merge_persistent_maxima(con, other)
    con.commit()
    log(
        f"merged from {args.other}: {nf} flags, {nh} history rows, "
        f"{na} attempts, {nm} MAX rows"
    )


def audit_log():
    """Append this invocation to binaries/match_db.log (same format as
    pdb_fetch.log: [timestamp][branch]: command) so a run's command history
    is reconstructable. Never breaks the tool; skipped when binaries/ is absent."""
    try:
        log_dir = VOSTOK / "binaries"
        if not log_dir.is_dir():
            return
        import datetime
        import subprocess

        ts = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")[:-4]
        try:
            branch = (
                subprocess.run(
                    ["git", "-C", str(VOSTOK), "rev-parse", "--abbrev-ref", "HEAD"],
                    capture_output=True,
                    text=True,
                    timeout=5,
                ).stdout.strip()
                or "?"
            )
        except Exception:
            branch = "?"
        with open(log_dir / "match_db.log", "a", encoding="utf-8") as f:
            f.write(f"[{ts}][{branch}]: match_db.py {' '.join(sys.argv[1:])}\n")
    except OSError:
        pass


def main():
    audit_log()
    ap = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter
    )
    sub = ap.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser(
        "refresh",
        help="regen-only: rebuild derived tables from the already-built report.json "
        "(rebuild.py is the canonical build and regenerates the DB itself; run it "
        "first if sources moved)",
    )

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

    p = sub.add_parser(
        "max",
        help="list effective-source-hash-scoped MAX evidence (not ordinary history)",
    )
    p.add_argument("--module")
    p.add_argument("--below", type=float, help="only rows whose MAX is below this percent")
    p.add_argument("--json", action="store_true")

    p = sub.add_parser(
        "record-max",
        help="mark an already measured MAX row as compiler-state island evidence",
    )
    p.add_argument("mangled")
    p.add_argument("--evidence", required=True, help="candidate manifest/path in this worktree")
    p.add_argument("--expected-hash", help="refuse if the effective source epoch changed")

    p = sub.add_parser(
        "import-island",
        help="import explicitly manifested function MAX from isolated artifacts",
    )
    p.add_argument("--report", required=True, help="isolated objdiff report.json")
    p.add_argument("--base-index", required=True, help="isolated base rich index.jsonl")
    p.add_argument("--evidence", required=True, help="tracked JSON evidence manifest")
    p.add_argument(
        "--dry-run",
        action="store_true",
        help="validate and print observations without changing match.db",
    )

    p = sub.add_parser(
        "rank-island",
        help="rank source-current MAX gains in isolated artifacts (read-only)",
    )
    p.add_argument("--report", required=True, help="isolated objdiff report.json")
    p.add_argument("--base-index", required=True, help="isolated base rich index.jsonl")
    p.add_argument("--module", help="only rank canonical functions in this module")
    p.add_argument("--json", action="store_true")

    p = sub.add_parser(
        "import-cross-unit",
        help="import reviewed exact target-only source-tree COMDAT evidence",
    )
    p.add_argument(
        "--report",
        default=str(CROSS_UNIT_REPORT.relative_to(VOSTOK)),
        help="cross-unit report (default: binaries/objdiff/report-cross-unit.json)",
    )
    p.add_argument("--evidence", required=True, help="tracked JSON evidence manifest")
    p.add_argument(
        "--dry-run",
        action="store_true",
        help="validate and print observations without changing match.db",
    )

    p = sub.add_parser("report", help="per-module/TU rollup")
    p.add_argument("--module")
    p.add_argument(
        "--unit",
        help="filter to ONE unit by name or substring (e.g. 'medkit'); --module "
        "optional. Refuses with paste-ready full names if the substring is ambiguous",
    )
    p.add_argument("--per-unit", action="store_true")
    p.add_argument(
        "--per-function",
        action="store_true",
        help="list every function of ONE --unit (pct/cls/size/flag), 100%%->0%%",
    )
    p.add_argument(
        "--function",
        help="list every function whose demangled name CONTAINS this substring "
        "(e.g. 'medkit::'), across units; --module optional",
    )
    p.add_argument(
        "--lite",
        action="store_true",
        help="lean view: drop any '(no unit)' catch-all row and the custom_conv/"
        "out_of_scope/suspicious columns",
    )
    p.add_argument(
        "--verbose",
        action="store_true",
        help="fn column = the full (capped) demangled signature instead of the "
        "mangled-derived scope::name",
    )
    p.add_argument("--json", action="store_true")

    p = sub.add_parser("queue", help="one batch per TU (all its open functions), small-first")
    p.add_argument("--module", required=True)
    p.add_argument("--limit", type=int, help="show only the first N TUs")
    p.add_argument(
        "--include-frameless",
        action="store_true",
        help="also queue LTCG-customized (frameless) leaves - normally pointless",
    )
    p.add_argument(
        "--stale-ok",
        action="store_true",
        help="emit the queue even when the DB predates source changes (rows may be stale)",
    )
    p.add_argument("--json", action="store_true")

    p = sub.add_parser("tried", help="record a dispatch attempt; queue demotes tried work")
    p.add_argument("mangled", nargs="*")
    p.add_argument("--unit", help="mark every function of this TU (even fully matched)")
    p.add_argument(
        "--done",
        action="store_true",
        help="mark every fn that reached a full match (by max) and isn't tried yet -> "
        "keeps the done set out of the queue until everything else is exhausted",
    )
    p.add_argument("--note", help="optional context, e.g. the worker's park causes")

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

    p = sub.add_parser(
        "diff", help="function-level diff between two committed match.db revisions")
    p.add_argument("spec", help="<hash> (vs working tree) or <hash>..<hash>")
    p.add_argument("--module")
    p.add_argument(
        "--verbose",
        action="store_true",
        help="fn column = the full (capped) demangled signature instead of scope::name",
    )
    p.add_argument("--json", action="store_true")

    args = ap.parse_args()
    {
        "refresh": cmd_refresh,
        "list": cmd_list,
        "max": cmd_max,
        "record-max": cmd_record_max,
        "import-island": cmd_import_island,
        "rank-island": cmd_rank_island,
        "import-cross-unit": cmd_import_cross_unit,
        "report": cmd_report,
        "queue": cmd_queue,
        "sql": cmd_sql,
        "tried": cmd_tried,
        "flag": cmd_flag,
        "merge-flags": cmd_merge_flags,
        "diff": cmd_diff,
    }[args.cmd](args)


if __name__ == "__main__":
    main()
