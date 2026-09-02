# SPDX-License-Identifier: GPL-3.0-or-later

"""vostok.sema.pairing - which base function IS this target function.

sema used to ask `binaries/match.db` for the pairing. That was a category
error: the pairing is a BUILD FACT, and every fact it rests on already lives in
the two files sema reads for disassembly - `binaries/rich/{target,base}/
index.jsonl`. The cache only held a derived copy, so a sema that needed it could
not answer on a tree that had never run `vostok derive`.

The passes are `vostok.derive.pairing`'s, run here over sema's inputs, because
two pairings that disagree are worse than one that is slow:

  1. same mangled spelling                            17,969
  2. an exact compiler-side name (`??__E`/`??__F`, audited aliases)   8
  3. a unique owner-compatible ICF alias of a SCORED symbol         203
  4. a unique same-source alias with a byte-identical body            1
  5. the same, sharing an already-claimed base RVA                   21
  6. dynamic-init/atexit thunks, on their canonical owner           589
                                                                 ------
                                                                 18,791

which is every pair the ledger records, at the same two RVAs, with nothing
extra. Measured against the cache on 2026-08-16: agree 18,791, disagree 0,
missing 0, extra 0.

Pass 3 is the one that needs evidence from outside the indexes: an ICF alias
earns a pair only because objdiff scored that target symbol. `derive` reads that
from `report.json`; here it comes from the committed ledger, where a row with a
`cur` percentage IS a symbol the report scored. So the whole pairing needs no
build artifact beyond the indexes themselves.

`derive` also seeds its `compiler_alias` from `symbols.rich_pdb_aliases`, which
re-reads both indexes (1.7 s) to derive 22 render template spellings. The
adapter below leaves that map empty and falls back to the plain MSVC name:
pass 3 recovers exactly those 22 anyway, and the pair set is identical with and
without it.

ONE implementation, in `vostok.derive.pairing`. This module owned a second copy
for exactly as long as the two were written in parallel; it now supplies the
inputs and lets `derive.pairing.pair()` run the passes, so a change to a pass
cannot land in one pairing and miss the other.

The only thing that ever really differed is where the "objdiff scored this"
evidence comes from - `report.json` for the derivation, the ledger here - and
that is now an argument rather than a fork.

Loading is lazy and cached: nothing here runs until a caller actually needs the
pairing (`sema rva` and the block/branch views resolve by name and never do).
`index` therefore imports THIS module inside the functions that need it, which
is what keeps the module-level dependency one-directional.
"""

from __future__ import annotations

from dataclasses import dataclass, field

from vostok.core import symbols as normalize_objdiff_symbols

from vostok.derive import set_quiet
from vostok.derive.index import index_by_mangled, load_index_records, overload_key
from vostok.derive.pairing import pair as derive_pair

from vostok.ledger import store

from vostok.sema.index import _index_path


@dataclass
class _Inputs:
    """The six things `derive.pairing.pair()` reads, sourced without a build.

    `derive` hands it an `Artifacts` built from report.json; sema has no
    report.json to offer, and needs none - `fuzzy` is the only field that ever
    came from there, and the ledger records the same measurement.
    """

    target: dict
    base: dict
    target_records: list
    base_records: list
    fuzzy: dict
    folded_fuzzy: dict = field(default_factory=dict)
    rich_pdb_aliases: dict = field(default_factory=dict)

    def compiler_alias(self, mangled):
        return (
            self.rich_pdb_aliases.get(mangled)
            or normalize_objdiff_symbols.compiler_name(mangled)
        )


class Pairing:
    """Both sides' function inventories, and the map between them.

    `target`/`base` are keyed the way the ledger keys a function: the mangled
    spelling, except for the PDB placeholder overloads that `index_by_mangled`
    disambiguates by signature. `pairs[key] = (target_rva, base_rva)`.
    """

    def __init__(self):
        target_records = load_index_records(_index_path("target"))
        base_records = load_index_records(_index_path("base"))
        target = index_by_mangled(target_records)
        # The base side inherits the target's owner and signature choices, so a
        # symbol the two PDBs spell identically also keys identically.
        base = index_by_mangled(
            base_records,
            {(r["mangled"], r["name"]): r["file"] for r in target.values()},
            {m: r["name"] for m, r in target.items() if m == r["mangled"]},
        )
        self.target_records = target_records
        self.base_records = base_records
        self.target = target
        self.base = base
        # A ledger row carries a `cur` percentage exactly when objdiff scored
        # that symbol in the build the ledger was written from - the evidence
        # pass 3 needs, and the only input here that is not an index.
        fuzzy = {m: row["cur"] for m, row in ledger().items()
                 if row["cur"] is not None}
        set_quiet()  # answering one question, not narrating a derivation
        self.pairs = {
            key: (p.target_rva, p.base_rva)
            for key, p in derive_pair(
                _Inputs(target, base, target_records, base_records, fuzzy)
            ).pairs.items()
        }

        self.base_rva_by_target_rva = {}
        self.target_rva_by_base_rva = {}
        for key in sorted(self.pairs):
            t_rva, b_rva = self.pairs[key]
            self.base_rva_by_target_rva.setdefault(t_rva, b_rva)
            self.target_rva_by_base_rva.setdefault(b_rva, t_rva)

        self.key_by_target_rva = {}
        self.key_by_base_rva = {}
        for key in sorted(target):
            self.key_by_target_rva.setdefault(target[key]["rva"], key)
        for key in sorted(base):
            self.key_by_base_rva.setdefault(base[key]["rva"], key)

    def partner_rva(self, side, rva):
        """The opposite side's RVA for a function, or None if it is unpaired."""
        if side == "target":
            return self.base_rva_by_target_rva.get(rva)
        return self.target_rva_by_base_rva.get(rva)

    def key_at(self, side, rva):
        """The ledger/roster key of the function at one side's RVA."""
        if side == "target":
            return self.key_by_target_rva.get(rva)
        return self.key_by_base_rva.get(rva)

    def demangled(self, key):
        """The retail PDB's signature, falling back to the base PDB's."""
        rec = self.target.get(key) or self.base.get(key)
        return rec["name"] if rec else key


_LOADED = None
_LEDGER = None


def pairing():
    """The pairing, built once per process (~3 s: both indexes are ~85 MB)."""
    global _LOADED
    if _LOADED is None:
        _LOADED = Pairing()
    return _LOADED


def ledger():
    """`config/match_state.tsv`, {key: row}, read once.

    The campaign's committed memory - `cur`/`max`/`cls`/`status`/`note`/module
    and TU ownership. It is in git, so unlike the indexes it needs no build at
    all, and unlike a database copy it cannot be stale in a way git will not show.
    """
    global _LEDGER
    if _LEDGER is None:
        _LEDGER = store.load()
    return _LEDGER


def ledger_row(rec, rows=None):
    """The ledger row for one rich-index record, or None.

    `index_by_mangled` renames every PDB placeholder overload after the first
    to a signature-derived key, so a record whose signature has such a key owns
    that row and anything else owns the plain mangled row. Doing it from the
    record keeps `sema rva` off the full pairing, which it would otherwise pay
    ~3 s for to learn one percentage.
    """
    rows = ledger() if rows is None else rows
    signature = overload_key(rec["mangled"], rec["name"])
    return rows.get(signature) or rows.get(rec["mangled"])
