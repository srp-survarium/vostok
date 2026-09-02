# SPDX-License-Identifier: GPL-3.0-or-later

"""vostok.derive.index - the rich PDB index, keyed for pairing.

`binaries/rich/<side>/index.jsonl` is one JSON record per emitted function. The
job here is turning that stream into a {mangled: record} map that survives the
two ways a PDB lies about identity: several overloads sharing one mangled
placeholder (disambiguated by signature, with the target's choice preferred so
both sides agree), and several names folded onto one RVA by ICF.
"""

import hashlib
import json


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
        # Prefer the target signature before collapsing: an ICF RVA can carry
        # the same mangled spelling on both the intended record and an unrelated
        # display alias, and collapsing first permanently discards the intended
        # PDB identity.
        primary = preferred_signatures.get(mangled)
        if primary is not None:
            selected.sort(key=lambda rec: rec["name"] != primary)
        selected_by_rva = {}
        for rec in selected:
            selected_by_rva.setdefault(rec["rva"], rec)
        ordered = sorted(selected_by_rva.values(), key=lambda rec: rec["rva"])
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


def body_statements(rec):
    """The record's REAL source statements, without the synthetic frame braces.

    A rich record's first and last entries are the `{` and `}` the compiler
    emits for the frame itself, not source. pdb_fetch, gen_sources and the
    structure-diff all read `statements[1:-1]`; counting the raw list instead
    says a function has two more statements than it does, and since statement
    COUNT is the whole basis of the QUANTITY verdict, that phantom pair reads
    as a real structural divergence. A body-less function carries only the
    braces (sometimes just one), so the floor is an empty body, never negative.
    """
    statements = rec.get("statements") or []
    return statements[1:-1] if len(statements) > 2 else []


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
