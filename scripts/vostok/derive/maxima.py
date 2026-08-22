"""vostok.derive.maxima - the hash-scoped MAX, and what may reset it.

MAX is the campaign's correctness gate: the best score PROVEN for a given source
body. It is scoped to a hash of that body, so it survives compiler-state churn -
an ICF fold representative changing, an inline decision flipping in a sibling TU
- and resets exactly when the source it describes changes, because a rewritten
body has to prove itself again. Ordinary best-seen history (`hist`) is NOT
promoted into it; that distinction is the reason both columns exist.

The banked peak lives in the committed ledger, one `max` and one `hash` per
function, so this module is a pure fold: previous evidence in, this build's
observations folded on top, out.
"""

import hashlib
from functools import lru_cache
from pathlib import Path, PurePosixPath

from vostok.core.paths import SOURCES
from vostok.derive import log

EXACT = 99.995  # objdiff reports byte-exact as >= this
LEDGER_RESOLUTION = 1e-4  # the ledger stores percentages to 4 decimals


@lru_cache(maxsize=None)
def _source_file(source_root, relative):
    """Resolve a PDB-normalized source path against the caseful checkout."""
    parts = PurePosixPath(relative.replace("\\", "/")).parts
    path = Path(source_root).joinpath(*parts)
    if path.is_file():
        return path

    path = Path(source_root)
    for part in parts:
        try:
            matches = [entry for entry in path.iterdir()
                       if entry.name.casefold() == part.casefold()]
        except OSError:
            return None
        if len(matches) != 1:
            return None
        path = matches[0]
    return path if path.is_file() else None


def _source_extent(rec):
    """Return ``(relative path, first line, last line, source text)``."""
    if rec is None:
        return None
    path = _source_file(str(SOURCES), rec["file"])
    lines = [s["line"] for s in rec["statements"] if s.get("line")]
    if path is None:
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


def source_hash(text):
    """The 12 hex chars the ledger stores as `hash`."""
    return hashlib.sha1(text.encode("latin-1")).hexdigest()[:12]


def effective_source_hash(rec):
    """The source-body hash for a rich-index record, or None with no source.

    Deliberately the function's OWN extent and nothing else. Folding in the
    owning module's headers and .cpp - as an earlier scheme did - meant editing
    any sibling in a TU reset MAX for every function in it.
    """
    extent = _source_extent(rec)
    return None if extent is None else source_hash(extent[3])


def fold(pairing, artifacts, banked):
    """Fold this build's observations into the hash-scoped MAX evidence.

    `banked` is {mangled: (hash, max)} from the committed ledger. A function
    whose hash still holds accumulates; a function whose body changed restarts
    at what this build measured. A function this build did not observe at all is
    simply absent from the result - `ledger.store.project` keeps its banked pair,
    which is what makes an LTCG/ICF disappearance cost nothing.

    Returns {mangled: (hash, max)}.
    """
    rows = {}
    raised = reset = 0
    for mangled, pair in pairing.pairs.items():
        if pair.fuzzy is None:
            continue
        extent = _source_extent(pairing.base_record(mangled, artifacts))
        if extent is None:
            continue
        effective_hash = source_hash(extent[3])
        previous = banked.get(mangled)
        maximum = pair.fuzzy
        if previous is not None and previous[0] == effective_hash:
            maximum = max(previous[1], pair.fuzzy)
            # the ledger stores 4 decimals, so a smaller "gain" than that is the
            # rounding of the peak we already banked, not a new one
            if maximum > previous[1] + LEDGER_RESOLUTION:
                raised += 1
        elif previous is not None:
            reset += 1
        rows[mangled] = (effective_hash, maximum)
    if raised or reset:
        log(f"source MAX: {raised} raised, {reset} source bodies changed (reset)")
    return rows
