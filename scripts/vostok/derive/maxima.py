# SPDX-License-Identifier: GPL-3.0-or-later

# SPDX-License-Identifier: GPL-3.0-or-later
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

import re
import hashlib
from functools import lru_cache
from pathlib import Path, PurePosixPath

from vostok.core.paths import SOURCES
from vostok.derive import log

EXACT = 99.995  # objdiff reports byte-exact as >= this
LEDGER_RESOLUTION = 1e-4  # the ledger stores percentages to 4 decimals


def canonical_peak(score):
    """Store exact objdiff evidence as the campaign's literal 100% gate."""
    return 100.0 if score is not None and score >= EXACT else score


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
        text = _extent_text(source_lines, lo, hi)
        legacy = "".join(source_lines[lo - 1 : hi])
    else:
        # Compiler-generated helpers and some ICF-selected header bodies carry
        # a real source file but no line records. Hash the whole owning file:
        # broader than a function extent, but conservative and source-scoped.
        lo, hi = 1, len(source_lines)
        text = legacy = "".join(source_lines)
    return rec["file"], lo, hi, text, legacy


_LINE_DIRECTIVE = re.compile(r"^\s*#\s*line\s+(\d+)")


def _extent_text(source_lines, lo, hi):
    """The physical lines the PDB's `lo..hi` line numbers denote.

    `#line N` renumbers everything after it (matchers pin `__LINE__` geometry
    with it), so a PDB line number is the compiler's virtual number, not a
    physical index. Directive lines themselves are not source body.
    """
    number = 1
    selected = []
    renumbered = False
    for line in source_lines:
        directive = _LINE_DIRECTIVE.match(line)
        if directive:
            number = int(directive.group(1))
            renumbered = True
            continue
        if lo <= number <= hi:
            selected.append(line)
        number += 1
    if not renumbered:
        return "".join(source_lines[lo - 1 : hi])
    return "".join(selected)


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
    raised = reset = rekeyed = 0
    for mangled, pair in pairing.pairs.items():
        if pair.fuzzy is None:
            continue
        extent = _source_extent(pairing.base_record(mangled, artifacts))
        if extent is None:
            continue
        effective_hash = source_hash(extent[3])
        if (previous is not None and previous[0] != effective_hash
                and previous[0] == source_hash(extent[4])):
            # the same body, banked before extents followed `#line`: re-key
            # the evidence under the hash that now describes it, no reset
            previous = (effective_hash, previous[1])
            rekeyed += 1
        previous = banked.get(mangled)
        maximum = canonical_peak(pair.fuzzy)
        if previous is not None and previous[0] == effective_hash:
            previous_maximum = canonical_peak(previous[1])
            maximum = max(previous_maximum, maximum)
            # the ledger stores 4 decimals, so a smaller "gain" than that is the
            # rounding of the peak we already banked, not a new one
            if maximum > previous_maximum + LEDGER_RESOLUTION:
                raised += 1
        elif previous is not None:
            reset += 1
        rows[mangled] = (effective_hash, maximum)
    if raised or reset or rekeyed:
        log(f"source MAX: {raised} raised, {reset} source bodies changed (reset), "
            f"{rekeyed} re-keyed under #line-aware extents")
    return rows
