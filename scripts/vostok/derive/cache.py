"""vostok.derive.cache - the sqlite side: what it remembers, and how it is written.

`binaries/match.db` is a regenerable cache; the committed record is
`docs/binary_matching/match_state.tsv`. Two responsibilities live here, and
nothing else in the package needs to know sqlite exists:

    carry_forward   read the PREVIOUS state the derivation cannot recompute -
                    history, hash-scoped maxima and their archived epochs,
                    flags, attempts - out of the cache, or out of the committed
                    ledger when there is no cache (a fresh clone).
    write           rebuild every table from this run's model, in a temp file
                    replaced atomically, with dense sorted ids so the bytes are
                    deterministic.

`reconcile_history` sits between them: it folds the current observations into
the carried history and drops rows whose source moved, which is what makes a
touched-then-vanished function re-queue instead of looking permanently done.
"""

import os
import sqlite3
from dataclasses import dataclass, field
from pathlib import Path

from vostok.core.paths import BASE_IDX, MATCH_DB as DB_PATH, REPORT, REPORT_HEAD, TARGET_IDX
from vostok.derive import log
from vostok.derive.artifacts import git_head
from vostok.derive.db import (Interner, SCHEMA, SCHEMA_VERSION, file_mtime_iso,
                              open_db)
from vostok.derive.index import legacy_overload_keys
from vostok.derive.maxima import (maximum_needs_epoch_archive,
                                  merge_maximum_epoch, src_fingerprint)
from vostok.derive.modules import module_of
from vostok.ledger import store as match_state


@dataclass
class Previous:
    """The state a refresh must not lose."""

    history: dict = field(default_factory=dict)
    maxima: dict = field(default_factory=dict)
    epochs: dict = field(default_factory=dict)
    flags: list = field(default_factory=list)
    attempts: list = field(default_factory=list)


def carry_forward(artifacts):
    previous = Previous()
    if not DB_PATH.is_file() and Path(match_state.STATE_PATH).is_file():
        # No cache (fresh clone / wiped binaries): the committed ledger holds the
        # only copy of the campaign's memory, so the regen starts from it.
        history, maxima, attempts, flags = match_state.seed_db_tables()
        previous = Previous(history=history, maxima=maxima,
                            attempts=list(attempts), flags=list(flags))
        log(f"seeded persistent state from {Path(match_state.STATE_PATH).name}: "
            f"{len(history)} history / {len(maxima)} maxima / {len(flags)} flags")
    if DB_PATH.is_file():
        previous = _read_cache()
    return _remap_legacy_keys(previous, artifacts)


def _read_cache():
    old = open_db()
    previous = Previous(
        history={r["mangled"]: tuple(r) for r in old.execute("SELECT * FROM history")}
    )
    try:
        previous.maxima = {
            r["mangled"]: tuple(r) for r in old.execute("SELECT * FROM source_maxima")
        }
    except sqlite3.OperationalError:
        pass  # schema 3: start MAX from current observations
    try:
        previous.epochs = {
            (r["mangled"], r["effective_hash"]): tuple(r)
            for r in old.execute("SELECT * FROM source_maxima_epochs")
        }
    except sqlite3.OperationalError:
        pass  # schema 4: seed from valuable active rows
    current_fuzzy = {
        r["mangled"]: r["fuzzy_pct"]
        for r in old.execute("SELECT mangled, fuzzy_pct FROM paired")
    }
    for mangled, row in previous.maxima.items():
        if not maximum_needs_epoch_archive(row, current_fuzzy.get(mangled)):
            continue
        key = (mangled, row[1])
        previous.epochs[key] = merge_maximum_epoch(previous.epochs.get(key), row)
    previous.flags = [
        tuple(r) for r in old.execute("SELECT * FROM flags ORDER BY mangled, flag")
    ]
    try:
        previous.attempts = [
            tuple(r) for r in old.execute("SELECT * FROM attempts ORDER BY mangled")
        ]
    except sqlite3.OperationalError:
        pass  # pre-schema-3 DB
    old.close()
    return previous


def _remap_legacy_keys(previous, artifacts):
    """Schema 4 disambiguated placeholder overloads with target RVAs. Carry the
    persistent evidence onto the signature-derived keys used since."""
    aliases = legacy_overload_keys(artifacts.target_records)
    previous.history = {
        aliases.get(key, key): (aliases.get(key, key), *row[1:])
        for key, row in previous.history.items()
    }
    previous.maxima = {
        aliases.get(key, key): (aliases.get(key, key), *row[1:])
        for key, row in previous.maxima.items()
    }
    epochs = {}
    for (key, effective_hash), row in previous.epochs.items():
        mapped = aliases.get(key, key)
        epoch_key = (mapped, effective_hash)
        epochs[epoch_key] = merge_maximum_epoch(
            epochs.get(epoch_key), (mapped, *row[1:])
        )
    previous.epochs = epochs
    previous.flags = [(aliases.get(row[0], row[0]), *row[1:]) for row in previous.flags]
    previous.attempts = [
        (aliases.get(row[0], row[0]), *row[1:]) for row in previous.attempts
    ]
    return previous


def reconcile_history(history, pairing, artifacts):
    """Upsert every CURRENT pairing, and reset a row whose source extent moved.

    The seen-flag has to die on any edit: a function that was paired, was then
    edited, and has now vanished must come back to the queue rather than count
    as settled. `paired_at` is the artifact's mtime, not wall clock, so a refresh
    is deterministic.
    """
    paired_at = file_mtime_iso(BASE_IDX)
    touched = dropped = 0
    for mangled, pair in pairing.pairs.items():
        brec = pairing.base_record(mangled, artifacts)
        fingerprint = src_fingerprint(brec)
        previous = history.get(mangled)
        best = pair.fuzzy
        if previous is not None and previous[5] == fingerprint and previous[2] is not None:
            best = previous[2] if pair.fuzzy is None else max(previous[2], pair.fuzzy)
        elif previous is not None and previous[5] != fingerprint:
            touched += 1  # source edited: history restarts at the current state
        history[mangled] = (mangled, paired_at, best, pair.fuzzy, pair.cls, fingerprint)
    for mangled in list(history):
        if mangled in artifacts.base and mangled not in pairing.paired:
            if history[mangled][5] != src_fingerprint(artifacts.base[mangled]):
                del history[mangled]
                dropped += 1
    if touched or dropped:
        log(f"history: {touched} touched (reset), {dropped} touched-and-vanished (re-queued)")
    return history


def _build_head():
    """WHICH source state was report.json built from.

    `vostok build` records it in report.head. Without the marker (an older
    build), CARRY FORWARD the previous build_head: re-assuming the current HEAD
    on every refresh would let a refresh-without-rebuild launder staleness. Only
    a virgin cache gets the one-time current-HEAD assumption.
    """
    if REPORT_HEAD.is_file():
        return REPORT_HEAD.read_text().strip()
    previous = None
    if DB_PATH.is_file():
        try:
            row = open_db().execute(
                "SELECT value FROM meta WHERE key='build_head'"
            ).fetchone()
            previous = row[0] if row else None
        except sqlite3.OperationalError:
            previous = None
    if previous and not previous.startswith("?"):
        log("no report.head marker - carrying forward the previous build_head")
        return previous
    log("no report.head marker (older build) - assuming report was built at current HEAD")
    return git_head()


def write(artifacts, target, base, pairing, history, maxima, epochs, base_only, previous):
    """Rebuild every table from this run's model."""
    symbols, units, files = Interner(), Interner(), Interner()
    for side in (target, base):
        for function in side.values():
            symbols(function.mangled)
            files(function.file)
            units(function.unit)
    for unit, mangled, _fuzzy in artifacts.report_fns:
        symbols(mangled)
        units(unit)
    sym_id, unit_id, file_id = symbols.freeze(), units.freeze(), files.freeze()

    def side_rows(side):
        return sorted(
            (
                fn.rva, sym_id[fn.mangled], unit_id.get(fn.unit), file_id[fn.file],
                fn.module, fn.line, fn.size, fn.n_stmts, int(fn.frameless),
            )
            for fn in side.values()
        )

    # one row per (unit, symbol): report.json can score a COMDAT in several TUs
    unit_rows = sorted({
        (unit_id[u], sym_id[m]): f
        for u, m, f in sorted(
            artifacts.report_fns, key=lambda r: (r[0], r[1], r[2] is not None, r[2] or 0)
        )
    }.items())
    unit_rows = [(u, s, f) for (u, s), f in unit_rows]

    pair_rows = sorted(
        (
            sym_id[p.mangled], p.target_rva, p.base_rva, p.fuzzy, p.cls,
            p.t_stmts, p.b_stmts, p.n_size_rows, p.n_trgt_only, p.n_base_only,
        )
        for p in pairing.pairs.values()
    )

    # build fresh in a temp file, then atomically replace (deterministic bytes)
    tmp = DB_PATH.with_suffix(".db.tmp")
    tmp.unlink(missing_ok=True)
    con = sqlite3.connect(tmp)
    con.executescript("PRAGMA page_size=4096;" + SCHEMA)
    con.executemany(
        "INSERT INTO symbols VALUES (?,?,?)",
        sorted((i, m, artifacts.demangled.get(m)) for m, i in sym_id.items()),
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
    con.executemany(
        "INSERT INTO base_only_status VALUES (?,?,?)",
        [(row.mangled, row.status, row.detail) for row in base_only],
    )
    con.executemany("INSERT INTO history VALUES (?,?,?,?,?,?)", sorted(history.values()))
    con.executemany("INSERT INTO source_maxima VALUES (?,?,?,?,?,?,?,?,?,?,?)", maxima)
    con.executemany(
        "INSERT INTO source_maxima_epochs VALUES (?,?,?,?,?,?,?,?,?,?,?)", epochs
    )
    con.executemany("INSERT INTO flags VALUES (?,?,?,?)", previous.flags)
    con.executemany("INSERT INTO attempts VALUES (?,?,?,?)", previous.attempts)
    # deterministic meta only (artifact mtimes, not wall clock)
    con.executemany(
        "INSERT INTO meta VALUES (?,?)",
        [
            ("target_index_mtime", file_mtime_iso(TARGET_IDX)),
            ("base_index_mtime", file_mtime_iso(BASE_IDX)),
            ("report_mtime", file_mtime_iso(REPORT)),
            ("refresh_head", git_head()),
            ("build_head", _build_head()),
            ("declarations_loaded", "1" if artifacts.has_declarations else "0"),
            ("schema_version", SCHEMA_VERSION),
        ],
    )
    con.commit()
    con.execute("VACUUM")
    con.close()
    os.replace(tmp, DB_PATH)
