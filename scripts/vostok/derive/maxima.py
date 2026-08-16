"""vostok.derive.maxima - the hash-scoped MAX, and what may reset it.

MAX is the campaign's correctness gate: the best score PROVEN for a given source
body. It is scoped to an effective source hash, so it survives compiler-state
churn and resets exactly when the source it describes changes. Ordinary
best-seen history is NOT promoted into it - that is the whole distinction, and
the reason both exist.

An observation from an inactive source epoch is archived rather than dropped, so
that reverting to an earlier body resurrects its proven peak instead of
forgetting it.
"""

import hashlib
import json
import sqlite3

from vostok.core.paths import REPO as VOSTOK
from vostok.core.paths import SOURCES
from vostok.derive import log
from vostok.derive.modules import logical_module, module_of

EXACT = 99.995  # objdiff reports byte-exact as >= this


def _source_extent(rec):
    """Return ``(relative path, first line, last line, source text)``."""
    if rec is None:
        return None
    path = SOURCES / rec["file"]
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
    path = SOURCES / rec["file"]
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


def effective_source_hash(rec, module=None):
    """HoMM2-style effective-source epoch for a source-backed function.

    The body hash is scoped by a conservative module/compiler context. Unlike
    ``history.src_fingerprint``, this hash owns correctness-facing MAX evidence;
    ordinary best-seen/ICF history is never promoted into it.
    """
    extent = _source_extent(rec)
    if extent is None:
        return None
    _source_file, _lo, _hi, text = extent
    return hashlib.sha1(text.encode("latin-1")).hexdigest()[:12]


def effective_source_hash_at(source_file, lo, hi, module):
    """Re-hash a retained source locator when its symbol is not in this build."""
    path = SOURCES / source_file
    if not path.is_file() or not lo or not hi:
        return None
    try:
        with open(path, encoding="latin-1") as f:
            text = "".join(f.readlines()[lo - 1 : hi])
    except OSError:
        return None
    return hashlib.sha1(text.encode("latin-1")).hexdigest()[:12]


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


def fold(pairing, artifacts, active, epochs):
    """Fold this build's observations into the hash-scoped MAX evidence.

    One row per source-backed pairing: the observed score accumulates while its
    effective source hash holds and restarts when the hash changes, because a
    rewritten body has to prove itself again. A row whose function LTCG/ICF made
    disappear is retained on its pinned source locator, and a valuable
    observation from a hash that is no longer current is archived rather than
    dropped, so reverting a body resurrects its proof.

    Returns ``(rows, epoch_rows, raised, reset)``.
    """
    rows = {}
    current_fuzzy = {}
    epoch_symbols = {key[0] for key in epochs}
    raised = reset = 0
    for mangled, pair in pairing.pairs.items():
        if pair.fuzzy is None:
            continue
        brec = pairing.base_record(mangled, artifacts)
        extent = _source_extent(brec)
        if extent is None:
            continue
        source_file, lo, hi, _text = extent
        units = artifacts.units_by_mangled.get(mangled)
        if not units and brec is not None:
            units = artifacts.units_by_mangled.get(brec["mangled"])
        module = logical_module(
            mangled, brec, units, artifacts.dynamic_owners, artifacts.module_overrides
        )
        effective_hash = effective_source_hash(brec, module)
        state_id = compiled_state_id(brec)
        current_exact = int(pair.fuzzy >= EXACT)
        current_fuzzy[mangled] = pair.fuzzy
        active_previous = active.get(mangled)
        previous = maximum_for_effective_hash(mangled, effective_hash, active, epochs)

        maximum = pair.fuzzy
        exact_proven = current_exact
        origin, evidence = "rebuild", None
        if previous is not None and previous[1] == effective_hash:
            maximum = max(previous[2], pair.fuzzy)
            exact_proven = int(bool(previous[3]) or current_exact)
            if maximum > previous[2] or exact_proven > previous[3]:
                raised += 1
            else:
                state_id, origin, evidence = previous[4], previous[9], previous[10]
        elif active_previous is not None or mangled in epoch_symbols:
            reset += 1

        rows[mangled] = (
            mangled, effective_hash, maximum, exact_proven, state_id, module,
            source_file, lo, hi, origin, evidence,
        )

    # A same-hash maximum stays valid when LTCG/ICF makes the function
    # temporarily disappear. Re-hash the retained source locator before keeping
    # it; edits or context changes retire the old epoch.
    for previous in list(active.values()) + list(epochs.values()):
        mangled = previous[0]
        if mangled in rows:
            continue
        brec = artifacts.base.get(mangled)
        module, source_file, lo, hi = previous[5:9]
        if retained_max_effective_hash(previous, brec) != previous[1]:
            continue
        candidate = (
            *previous[:5], module, source_file, lo, hi, previous[9], previous[10]
        )
        rows[mangled] = merge_maximum_epoch(rows.get(mangled), candidate)

    ordered = sorted(rows.values())
    for row in ordered:
        if not maximum_needs_epoch_archive(row, current_fuzzy.get(row[0])):
            continue
        key = (row[0], row[1])
        epochs[key] = merge_maximum_epoch(epochs.get(key), row)
    if reset or raised:
        log(f"source MAX: {raised} raised, {reset} source epochs reset")
    return ordered, sorted(epochs.values()), raised, reset


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
