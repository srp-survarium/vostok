"""vostok.derive.roster - the derivation, end to end.

Five steps, one per module, and the order between them is the whole design:

    artifacts   load report.json + the two rich indexes, and say so when they
                are stale
    inventory   one record per function on each side: unit, module, size, frame
    pairing     which base function IS this target function, through every alias
                the two linkers spell differently
    maxima      fold this build's scores into the hash-scoped MAX evidence,
                on top of the peaks the committed ledger banked
    baseonly    and explain everything WE emit that the target does not

The result goes straight into `docs/binary_matching/match_state.tsv` through
`ledger.store.project`. There is no database in between: everything above is a
pure function of the artifacts, and everything a build must not destroy - hist,
notes, dispatch counts, banked peaks - already lives in the committed ledger.

Nothing here compiles anything: `vostok build` is the canonical build and calls
`regen()` at the end of it; `refresh` is the regen-only path for a report that
is already on disk.
"""

from dataclasses import dataclass
from pathlib import Path

from vostok.derive import artifacts as artifacts_module
from vostok.derive import baseonly, inventory, log, maxima, pairing
from vostok.derive.modules import module_of
from vostok.ledger import store as match_state


def cmd_refresh(args):
    regen()


@dataclass
class Roster:
    """What the artifacts say, before any campaign memory is folded in."""

    artifacts: artifacts_module.Artifacts
    target: dict            # {mangled: inventory.Function}, the retail roster
    base: dict              # {mangled: inventory.Function}, ours
    pairing: pairing.Pairing


def derive(declarations=True):
    """The pure derivation: artifacts -> inventories -> pairing.

    No previous state is read and nothing is written, so anything that wants the
    per-function structure verdict can call this instead of keeping a cache
    around to be queried. It re-reads ~190 MB of index, which is why the callers
    that do not need the declaration dump say so.
    """
    artifacts = artifacts_module.load(declarations=declarations)
    target = inventory.functions(artifacts.target, artifacts)
    base = inventory.functions(artifacts.base, artifacts)
    log("classifying structure for paired functions ...")
    return Roster(artifacts, target, base, pairing.pair(artifacts))


def observations(roster, maxima_rows):
    """One ledger observation per TARGET function: what this build measured.

    The target roster is the ledger's roster - a base-only symbol is a defect
    (see `baseonly`), not a row of the campaign's worklist.
    """
    for mangled, function in roster.target.items():
        pair = roster.pairing.pairs.get(mangled)
        banked = maxima_rows.get(mangled)
        yield {
            "mangled": mangled,
            "unit": function.unit,
            "module": function.module,
            "size": function.size,
            "frameless": function.frameless,
            "cls": pair.cls if pair else None,
            "cur": pair.fuzzy if pair else None,
            "hash": banked[0] if banked else "",
            "max": banked[1] if banked else None,
        }


def enclosing_function_mangled(mangled):
    """Return the source function enclosing an MSVC local-static thunk.

    A function-local ``descriptor_object`` atexit destructor, for example, is
    emitted as ``??__Fdescriptor_object@?1?<enclosing function>@YAXXZ``.  The
    retail PDB can omit the thunk while objdiff still measures its COFF symbol;
    the nested function identity is the authoritative source owner in that
    case.
    """
    _prefix, marker, tail = mangled.partition("@?1?")
    if not marker or not tail.endswith("@YAXXZ"):
        return None
    parent = tail[: -len("@YAXXZ")]
    return parent if parent.startswith("?") and parent.endswith("Z") else None


def report_only_observations(roster, maxima_rows, previous):
    """Recover measured compiler thunks omitted from the current rich index.

    These rows already belong to the committed target roster.  Keeping them as
    permanently unpaired rows discarded current 100% object evidence and left
    their unit/module/source hash blank.  A direct report identity plus the
    mangled enclosing function supplies all of those without guessing a source
    owner.
    """
    report_by_name = {}
    for unit, mangled, fuzzy, size in roster.artifacts.report_fns:
        report_by_name.setdefault(mangled, []).append((unit, fuzzy, size))

    for mangled, old in previous.items():
        if mangled in roster.target or mangled not in report_by_name:
            continue

        entries = report_by_name[mangled]
        scored = [entry for entry in entries if entry[1] is not None]
        if not scored:
            continue
        unit, fuzzy, size = max(scored, key=lambda entry: entry[1])

        parent_mangled = enclosing_function_mangled(mangled)
        parent = roster.target.get(parent_mangled)
        banked = maxima_rows.get(parent_mangled)
        if parent is not None:
            unit = parent.unit
            module = parent.module
        else:
            # `_msvc_internal/*` is a delinker bucket, not source ownership.
            # Preserve its direct measurement, but leave ownership blank until
            # a real enclosing source identity is available.
            if unit.startswith("_msvc_internal/") and old.get("module") in {
                None,
                "",
                "_msvc_internal",
            }:
                unit = ""
                module = ""
            else:
                module = old.get("module") or module_of(unit)

        body_hash = banked[0] if banked else old.get("hash", "")
        yield {
            "mangled": mangled,
            "unit": unit,
            "module": module,
            "size": size or old.get("size") or 0,
            "frameless": old.get("flags") == "f",
            # There is no PDB statement record to classify. Exact object bytes
            # remain exact evidence without inventing a statement verdict.
            "cls": old.get("cls") or None,
            "cur": fuzzy,
            "hash": body_hash,
            "max": fuzzy,
        }


def regen():
    """Re-derive everything from the already-built diff artifacts.

    REGEN-ONLY: it does NOT build. `vostok build` is the canonical build step
    and calls this at the end of its run; invoke `vostok derive refresh` by hand
    only to re-derive from an artifact set that is already on disk (run
    `vostok build` first if sources moved).
    """
    roster = derive()
    previous = match_state.load()
    committed = match_state.load_committed()
    proof = previous if committed is None else committed
    banked = {
        mangled: (row["hash"], row["max"])
        for mangled, row in proof.items()
        if row["hash"] and row["max"] is not None
    }
    maxima_rows = maxima.fold(roster.pairing, roster.artifacts, banked)

    base_only = baseonly.classify(
        roster.artifacts, roster.pairing, roster.base, seen=set(previous)
    )
    n = baseonly.write_report(base_only)
    log(f"base-only report: {n} rows")

    measured = list(observations(roster, maxima_rows))
    measured.extend(report_only_observations(roster, maxima_rows, previous))
    written = match_state.project(
        measured,
        banked_previous=proof,
        authoritative_roster={row["mangled"] for row in measured},
    )
    log(
        f"ledger {Path(match_state.STATE_PATH).name}: {written} rows "
        f"({len(roster.target)} target / {len(roster.base)} base / "
        f"{len(roster.pairing.pairs)} paired)"
    )
