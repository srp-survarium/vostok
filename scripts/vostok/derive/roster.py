"""vostok.derive.roster - the derivation, end to end.

Six steps, one per module, and the order between them is the whole design:

    artifacts   load report.json + the two rich indexes, and say so when they
                are stale
    inventory   one record per function on each side: unit, module, size, frame
    pairing     which base function IS this target function, through every alias
                the two linkers spell differently
    cache       the previous state a rebuild cannot recompute
    maxima      fold this build's scores into the hash-scoped MAX evidence
    baseonly    and explain everything WE emit that the target does not

Nothing here compiles anything: `vostok build` is the canonical build and calls
`regen()` at the end of it; `refresh` is the regen-only path for a report that
is already on disk.
"""

from dataclasses import dataclass
from pathlib import Path

from vostok.core.paths import MATCH_DB as DB_PATH
from vostok.core.paths import REPO as VOSTOK
from vostok.derive import artifacts as artifacts_module
from vostok.derive import baseonly, cache, inventory, log, maxima, pairing
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


def regen():
    """Re-derive everything from the already-built diff artifacts.

    REGEN-ONLY: it does NOT build. `vostok build` is the canonical build step
    and calls this at the end of its run; invoke `vostok derive refresh` by hand
    only to re-derive from an artifact set that is already on disk (run
    `vostok build` first if sources moved).
    """
    roster = derive()
    artifacts, target, base = roster.artifacts, roster.target, roster.base
    pairs = roster.pairing

    previous = cache.carry_forward(artifacts)
    history = cache.reconcile_history(previous.history, pairs, artifacts)
    maxima_rows, epoch_rows, _raised, _reset = maxima.fold(
        pairs, artifacts, previous.maxima, previous.epochs
    )
    base_only = baseonly.classify(artifacts, pairs, base, history)

    cache.write(artifacts, target, base, pairs, history, maxima_rows, epoch_rows,
                base_only, previous)
    log(
        f"refreshed {DB_PATH.relative_to(VOSTOK)}: "
        f"{len(target)} target / {len(base)} base / {len(pairs.pairs)} paired / "
        f"{len(base_only)} base-only classified"
    )

    written = match_state.export_from_db(DB_PATH)
    log(f"ledger {Path(match_state.STATE_PATH).name}: {written} rows")
