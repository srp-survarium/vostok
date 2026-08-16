"""vostok.sema.pairing - which base function IS this target function.

sema used to ask `binaries/match.db` for the pairing. That was a category
error: the pairing is a BUILD FACT, and every fact it rests on already lives in
the two files sema reads for disassembly - `binaries/rich/{target,base}/
index.jsonl`. The cache only held a derived copy, so a sema that needed it could
not answer on a tree that had never run `vostok derive`.

The passes below are the same ones `vostok.derive.pairing` runs, in the same
order, over the same helpers - deliberately, because two pairings that disagree
are worse than one that is slow:

  1. same mangled spelling                            17,969
  2. an exact compiler-side name (`??__E`/`??__F`, audited aliases)   8
  3. a unique owner-compatible ICF alias of a SCORED symbol         203
  4. a unique same-source alias with a byte-identical body            1
  5. the same, sharing an already-claimed base RVA                   21
  6. dynamic-init/atexit thunks, on their canonical owner           589
                                                                 ------
                                                                 18,791

which is every pair `match.db` records, at the same two RVAs, with nothing
extra. Measured against the cache on 2026-08-16: agree 18,791, disagree 0,
missing 0, extra 0.

Pass 3 is the one that needs evidence from outside the indexes: an ICF alias
earns a pair only because objdiff scored that target symbol. `derive` reads that
from `report.json`; here it comes from the committed ledger, where a row with a
`cur` percentage IS a symbol the report scored. So the whole pairing needs no
build artifact beyond the indexes themselves.

`derive` also seeds pass 2 from `symbols.rich_pdb_aliases`, which re-reads both
indexes (1.7 s) to derive 22 render template spellings. It is omitted here
because pass 3 recovers exactly those 22 anyway - the pair count is identical
with and without it.

TWO IMPLEMENTATIONS OF ONE THING, and that is not meant to last.
`vostok.derive.pairing.pair()` runs these passes for the derivation; this module
runs them for `sema` and `diff tu-order`. They were written at the same time in
one worktree and neither could import the other yet. The difference is only
where the "objdiff scored this" evidence comes from - `report.json` there, the
ledger here - so the merge is to give `derive.pairing.pair()` that evidence as
an argument and call it from here. Until then, a change to one pass belongs in
both.

Loading is lazy and cached: nothing here runs until a caller actually needs the
pairing (`sema rva` and the block/branch views resolve by name and never do).
`index` therefore imports THIS module inside the functions that need it, which
is what keeps the module-level dependency one-directional.
"""

from __future__ import annotations

import collections

from vostok.core import symbols as msvc_names

from vostok.derive.aliases import (dyn_canon_base, dyn_canon_rich,
                                   dyn_owner_compatible,
                                   load_exact_fold_aliases,
                                   report_source_alias_candidates,
                                   strict_source_alias_candidates)
from vostok.derive.index import index_by_mangled, load_index_records, overload_key

from vostok.ledger import store

from vostok.sema.index import _index_path


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
        scored = {m for m, row in ledger().items() if row["cur"] is not None}
        self.pairs = _pair(target_records, target, base_records, base, scored)

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

    def record_at(self, side, rva):
        key = self.key_at(side, rva)
        source = self.target if side == "target" else self.base
        return source.get(key) if key else None

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
    """`docs/binary_matching/match_state.tsv`, {key: row}, read once.

    The campaign's committed memory - `cur`/`max`/`cls`/`status`/`note`/module
    and TU ownership. It is in git, so unlike the indexes it needs no build at
    all, and unlike `match.db` it cannot be stale in a way git will not show.
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


def _pair(target_records, target, base_records, base, scored):
    """Run the passes in order. Returns {key: (target_rva, base_rva)}.

    `scored` is the set of keys objdiff measured - the evidence the ICF-alias
    pass needs, and the caller's to supply so this stays a pure function of its
    arguments."""
    pairs = {key: (target[key]["rva"], base[key]["rva"])
             for key in target.keys() & base.keys()}
    primary = set(pairs)
    claimed_target = {t for t, _ in pairs.values()}
    claimed_base = {b for _, b in pairs.values()}
    cross = set()

    def claim(key, t_rec, b_rec, *, share_base=False):
        pairs[key] = (t_rec["rva"], b_rec["rva"])
        claimed_target.add(t_rec["rva"])
        if not share_base:
            claimed_base.add(b_rec["rva"])
        cross.update((key, b_rec["mangled"]))

    by_name = collections.defaultdict(dict)
    by_mangled = collections.defaultdict(dict)
    base_alias_names = collections.defaultdict(set)
    target_alias_names = collections.defaultdict(set)
    for rec in base_records:
        by_name[rec["name"]][rec["rva"]] = rec
        by_mangled[rec["mangled"]][rec["rva"]] = rec
        base_alias_names[rec["rva"]].add(rec["name"])
    for rec in target_records:
        target_alias_names[rec["rva"]].add(rec["name"])
    fold_aliases = load_exact_fold_aliases()

    # 2. the retail PDB's readable spelling of a name MSVC 8 decorates
    for key in sorted(set(target) - primary):
        alias = msvc_names.compiler_name(key)
        if alias not in base or key not in scored:
            continue
        if target[key]["rva"] in claimed_target or base[alias]["rva"] in claimed_base:
            continue
        claim(key, target[key], base[alias])

    # 3. a scored symbol whose base body wears another ICF alias's name. The
    #    report already proved WHICH target symbol was compared, so - unlike
    #    pass 4 - the bodies are allowed to differ; requiring them to be equal
    #    would hide the very mismatch the score is reporting.
    for key in sorted(set(target) - primary):
        if key not in scored or target[key]["rva"] in claimed_target:
            continue
        candidates = report_source_alias_candidates(
            target[key], by_name, claimed_base,
            target_alias_names_by_rva=target_alias_names,
            base_alias_names_by_rva=base_alias_names)
        if len(candidates) == 1:
            claim(key, target[key], candidates[0])

    # 4/5. an unscored fold: admitted only on a byte-identical instruction
    #      stream, first onto a free base RVA and then onto a claimed one (one
    #      folded body legitimately owns several PDB aliases).
    for share_base in (False, True):
        for key in sorted(set(target) - primary - cross):
            if target[key]["rva"] in claimed_target:
                continue
            candidates = strict_source_alias_candidates(
                target[key], by_name, claimed_base, allow_used=share_base,
                target_alias_names_by_rva=target_alias_names,
                base_alias_names_by_rva=base_alias_names,
                exact_fold_aliases=fold_aliases,
                base_aliases_by_mangled=by_mangled)
            if len(candidates) != 1:
                continue
            if share_base and candidates[0]["rva"] not in claimed_base:
                continue
            claim(key, target[key], candidates[0], share_base=share_base)

    # 6. `vostok::x::`dynamic initializer for 'y''` on one side is
    #    ``dynamic initializer for 'vostok::x::y''` on the other. Same owner,
    #    different place to put the namespace.
    canon_target, canon_base = collections.defaultdict(list), collections.defaultdict(list)
    for key in set(target) - primary:
        canon = dyn_canon_rich(key)
        if canon:
            canon_target[canon].append(key)
    for key in set(base) - primary:
        canon = dyn_canon_base(key)
        if canon:
            canon_base[canon].append(key)
    for canon in sorted(canon_target.keys() & canon_base.keys()):
        keys, base_keys = canon_target[canon], canon_base[canon]
        if len(keys) != 1 or len(base_keys) != 1:
            continue  # ambiguous: two same-named statics, no proof of identity
        key, base_key = keys[0], base_keys[0]
        if target[key]["rva"] in claimed_target or base[base_key]["rva"] in claimed_base:
            continue
        if not dyn_owner_compatible(target[key], base[base_key], canon):
            continue
        claim(key, target[key], base[base_key])

    # 7. an audited template spelling the two PDBs render differently. Zero
    #    rows today (pass 3 gets there first) - kept so the pass list is the
    #    roster's, not a subset that silently drifts from it.
    for key in sorted(set(target) - primary - cross):
        alias = msvc_names.pdb_alias_name(key)
        if not alias or alias not in base or alias in primary or alias in cross:
            continue
        if target[key]["file"] != base[alias]["file"]:
            continue
        claim(key, target[key], base[alias])

    return pairs
