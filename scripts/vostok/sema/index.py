"""vostok.sema.index - which function, on which side, at which RVA.

Everything else here takes a resolved record. One selector - a mangled name, a
demangled substring, or a hex RVA/VA on either side - is answered by ONE pass
over each side's `binaries/rich/<side>/index.jsonl` (~85 MB), then completed
from `sema.pairing`, which is how naming ONE side's address also finds the
other side's twin (the two sides sit at different addresses).

Ambiguity is reported, never guessed at - with two things that are NOT
ambiguities: several records at one RVA are ICF aliases of one function
(`_fold_aliases`), and a hex selector's four readings are enumerated rather than
raced (`_hex_readings`).

Addresses here are RVAs, and every one printed says so; `image_base` on the
record turns it into the VA that IDA shows. See the convention in `sema`.
"""

from __future__ import annotations

import json
import re
import sys

from vostok.core.paths import RICH_DIR as RICH
from vostok.derive.aliases import dyn_canon_base

from vostok.sema import die


def va_of(rec):
    """The virtual address IDA shows: the record's RVA plus its image base."""
    return rec["rva"] + rec.get("image_base", 0)


def _index_path(side):
    p = RICH / side / "index.jsonl"
    if not p.is_file():
        die(f"{p} missing - run `python3 -m vostok build` first")
    return p


def _scan_index(side, want):
    """One pass over binaries/rich/<side>/index.jsonl (~85 MB, ~0.05 s).

    `want(record_line, record_or_None)` is called with the raw line first so the
    common case never pays for json.loads."""
    hits = []
    with open(_index_path(side), encoding="utf-8", errors="replace") as fh:
        for line in fh:
            rec = want(line)
            if rec is not None:
                hits.append(rec)
    return hits


HEX = re.compile(r"0[xX][0-9a-fA-F]+\Z")


# `sema rva render` matches 4,673 symbols; dumping two lines each is not a help.
_AMBIGUITY_LIST = 20


def _matcher(sel):
    """Build the line predicate for a NAME selector: exact mangled > mangled
    substring > demangled substring > dynamic-init canonical form. Hex goes
    through `_hex_readings`."""
    low = sel.lower()
    canon = dyn_canon_base(sel)

    def want(line):
        if sel in line:
            rec = json.loads(line)
            if rec["mangled"] == sel or sel in rec["mangled"]:
                return rec
        if low in line.lower():
            rec = json.loads(line)
            if low in rec["name"].lower():
                return rec
        # The two sides spell a dynamic initializer differently, and neither
        # spelling contains the other: report.json and our base PDB say
        # `??__Es_cc@survarium@@YAXXZ`, the retail PDB has no mangled name at all
        # and stores ``survarium::`dynamic initializer for 's_cc'''``. So the
        # name you copy out of report.json never found the target record - the
        # tool answered "no function matches" for a function it holds. Compare
        # the canonical identity instead. The cheap guard keeps the common path
        # off json.loads.
        if canon and ("dynamic " in line or "??__" in line):
            rec = json.loads(line)
            if dyn_canon_base(rec["mangled"]) == canon:
                return rec
        return None

    return want


def _hex_readings(sel):
    """Every function a bare hex number can mean: [(side, "rva"|"va", record)].

    A hex selector is FOUR questions - target RVA, target VA, base RVA, base VA -
    and the old code answered whichever it found first, target side before base
    and with no note that it had chosen. 1,391 base RVAs are also (base RVA -
    image_base) of a real target function, so asking about a base address
    silently returned an unrelated target one:

        $ python3 -m vostok.sema rva 0xe9fa0     # base: PrimitiveFillData::RequiresBlend
        target  rva=0xd9fa0 ... functor_manager_common<...>::manage_small

    Every reading is collected here and the caller reports a tie instead."""
    h = int(sel, 16)
    out = []
    for side in ("target", "base"):
        hits = {}
        with open(_index_path(side), encoding="utf-8", errors="replace") as fh:
            for line in fh:
                if f'"rva":{h}' not in line and f'"rva":{h - 0x10000}' not in line:
                    continue                       # cheap prefilter before parsing
                rec = json.loads(line)
                base_addr = rec.get("image_base", 0)
                if rec["rva"] == h:
                    hits.setdefault("rva", []).append(rec)
                elif base_addr and rec["rva"] == h - base_addr:
                    hits.setdefault("va", []).append(rec)
        for how, recs in hits.items():
            for rec in _fold_aliases(side, recs, sel):
                out.append((side, how, rec))
    return out


def _paired_rva(side, rva):
    """The opposite side's RVA for a function, or None if it is unpaired.

    Derived from the two indexes (`sema.pairing`), not from a cache. It is only
    ever reached when the name scan found ONE side, which costs ~3 s of index
    parsing - so a selector both sides answer, i.e. almost every selector, never
    pays for it. Import is deliberately local: `pairing` imports this module.
    """
    from vostok.sema.pairing import pairing

    return pairing().partner_rva(side, rva)


def _record_at_rva(side, rva):
    """The one record at an exact RVA, or None.

    The `"rva":N` substring is only a prefilter: N is a decimal prefix of every
    longer RVA (`"rva":123` is in `"rva":1234`), so the loaded record is checked
    for equality. Several records at one RVA are ICF aliases of one function, so
    the first is as good as any."""
    needle = f'"rva":{rva}'

    def want(line):
        if needle not in line:
            return None
        rec = json.loads(line)
        return rec if rec["rva"] == rva else None

    hits = _scan_index(side, want)
    return hits[0] if hits else None


def _fold_aliases(side, hits, sel):
    """Collapse hits that share an RVA: they are ONE function under several names.

    The delinker emits one index record per symbol and the linker's ICF folds
    identical bodies onto one address - 1042 target RVAs carry 2..8 records. Those
    are not competing candidates, they are aliases of the same code, so treating
    them as an ambiguity made 912 paired functions unreachable by ANY selector:
    the error told the reader to 'pass a mangled name or RVA' when both were
    already exact and both produced that same error."""
    by_rva, counts = {}, {}
    for rec in hits:
        rva = rec["rva"]
        counts[rva] = counts.get(rva, 0) + 1
        keep = by_rva.get(rva)
        if keep is None or (keep["mangled"] != sel and rec["mangled"] == sel):
            by_rva[rva] = rec
    for rva in sorted(by_rva):
        if counts[rva] > 1:
            sys.stderr.write(
                f"[{counts[rva]} symbols share {side} rva={rva:#x} (ICF fold) - "
                f"reading it as {by_rva[rva]['mangled']}]\n")
    return [by_rva[k] for k in sorted(by_rva)]


def _same_function(a, b):
    """Same symbol on the two sides.

    The mangled name is the primary key, but the two indexes do not always carry
    the same spelling: where the PDB records no mangled name, pdb_parser falls
    back to the demangled one, so the target can say
    `vostok::render::copy_destroyer<binary_config_value, custom_config_value>`
    where the base says `??$copy_destroyer@Vbinary_config_value@configs@...`.
    Both still render the same demangled signature, so accept that too - 820 of
    the 18,788 paired functions are named by only one of the two indexes, and
    dropping the partner made every one of them answer `--diff` with a false
    "TARGET_ONLY - nothing compiled yet"."""
    return a["mangled"] == b["mangled"] or a["name"] == b["name"]


def _resolve_hex(sel):
    """(target_record | None, base_record | None) for a hex address selector."""
    readings = _hex_readings(sel)
    if len(readings) > 1:
        for side, how, rec in readings:
            # `how` is which READING hit, so it must not label the number: a VA
            # reading still sits at the record's RVA, and printing that RVA
            # under "va" is exactly the 0x10000 confusion this listing exists
            # to prevent. Both forms, both labelled.
            sys.stderr.write(
                f"  {side} as {how}: rva={rec['rva']:#x} va={va_of(rec):#x}  "
                f"{rec['mangled']}\n      {rec['name']}\n")
        die(f"'{sel}' reads as {len(readings)} different functions (listed above): a bare "
            f"hex is a target/base RVA and a target/base VA at once, and sema has no side "
            f"flag for it. Pass the mangled name printed above, or ask pdb_fetch, which "
            f"takes the side and the kind explicitly (--target-index/--base-index with "
            f"--rva/--va)")
    if not readings:
        return None, None
    side, _, rec = readings[0]
    other = _paired_rva(side, rec["rva"])
    partner = _record_at_rva("base" if side == "target" else "target", other) \
        if other is not None else None
    return (rec, partner) if side == "target" else (partner, rec)


def resolve(sel):
    """(target_record | None, base_record | None) for one selector.

    Ambiguity is resolved by preferring an EXACT mangled hit; anything still
    ambiguous is reported rather than guessed at. Hits that share an RVA are ONE
    function under several names (ICF), not an ambiguity - see `_fold_aliases`."""
    if HEX.match(sel):
        return _resolve_hex(sel)
    want = _matcher(sel)
    tgt = _fold_aliases("target", _scan_index("target", want), sel)
    base = _fold_aliases("base", _scan_index("base", want), sel)
    # A partner found through the match.db pairing is authoritative and is NOT
    # re-checked by name below. The two indexes really do spell some symbols
    # differently: where the PDB carries no mangled name, pdb_parser falls back
    # to the demangled one, so `copy_destroyer<binary_config_value,
    # custom_config_value>` is the target's `mangled` and `??$copy_destroyer@V...`
    # is the base's. Filtering the partner by name dropped it and the view then
    # announced "TARGET_ONLY - nothing compiled yet" for a 100% matched pair.
    # ...and it is only consulted when the name scan did NOT find that side, so a
    # stale RVA in the cache can never displace a record the indexes agree on.
    from_pairing = False
    if not base and len(tgt) == 1:
        paired = _paired_rva("target", tgt[0]["rva"])
        if paired is not None:
            partner = _record_at_rva("base", paired)
            if partner is not None:
                base, from_pairing = [partner], True
    elif not tgt and len(base) == 1:
        paired = _paired_rva("base", base[0]["rva"])
        if paired is not None:
            partner = _record_at_rva("target", paired)
            if partner is not None:
                tgt, from_pairing = [partner], True
    exact = [r for r in tgt if r["mangled"] == sel] or \
            [r for r in base if r["mangled"] == sel]
    if exact and not from_pairing:
        tgt = [r for r in tgt if _same_function(r, exact[0])]
        base = [r for r in base if _same_function(r, exact[0])]
    if len(tgt) > 1 or (not tgt and len(base) > 1):
        hits = tgt or base
        for r in hits[:_AMBIGUITY_LIST]:
            sys.stderr.write(f"  {r['mangled']}\n      {r['name']}\n")
        if len(hits) > _AMBIGUITY_LIST:
            sys.stderr.write(f"  ... and {len(hits) - _AMBIGUITY_LIST} more\n")
        die(f"'{sel}' is ambiguous ({len(hits)} hits) - copy one of the spellings above, "
            f"or narrow the substring (`vostok ledger list --unit <tu>` and "
            f"`pdb_rich_query --list --function <substring>` enumerate candidates)")
    if tgt and base and not from_pairing:
        base = [r for r in base if _same_function(r, tgt[0])]
    return (tgt[0] if tgt else None), (base[0] if base else None)


def _records(side):
    with open(_index_path(side), encoding="utf-8", errors="replace") as fh:
        for line in fh:
            yield json.loads(line)


def _side_record(args):
    target, base = resolve(args.fn)
    side = "base" if getattr(args, "base", False) else "target"
    rec = base if side == "base" else target
    if rec is None:
        if target is None and base is None:
            die(f"no function matches '{args.fn}'")
        die(f"'{args.fn}' has no {side} function (it exists only on the "
            f"{'target' if target else 'base'} side)")
    return side, rec


def fuzzy_of(rec):
    """This build's fuzzy % for a function, from the ledger, or None.

    The ledger's `cur` IS `report.json`'s score for the build that wrote it, so
    this is the same number `match.db` used to answer with - and it is committed,
    so the hint still fires on a tree that has never run a derivation."""
    from vostok.sema.pairing import ledger_row

    row = ledger_row(rec)
    return row["cur"] if row else None


def hint(rec, clean_flow, clean_branches):
    """When a view comes out clean but the function is NOT matched, SAY where the
    remaining signal is. The failure mode this guards against: a matcher runs the
    first-look command, sees nothing, and concludes 'regalloc only' - which is a
    verdict, not an observation."""
    pct = fuzzy_of(rec)
    if pct is None or pct >= 100.0:
        return
    if clean_flow and clean_branches:
        print(f"[this function is {pct:.2f}%, not 100, yet base and target have the SAME "
              "control-flow graph and the SAME branch destinations. The residual is "
              "instruction selection / register allocation / operand values - run "
              "`pdb_fetch --view diff` (operand-aware) for the byte cause, and "
              "`--view structure-diff` for the statement shape.]")
    elif clean_branches:
        print(f"[this function is {pct:.2f}%, not 100. The branch SEQUENCE agrees but "
              "block bodies do not - `sema blocks --diff` names the divergent blocks "
              "and their source statements.]")
    else:
        print(f"[this function is {pct:.2f}%, not 100 - and the control flow itself "
              "differs. Fix the SHAPE first: an instruction-level diff of two "
              "differently-shaped functions is noise.]")
