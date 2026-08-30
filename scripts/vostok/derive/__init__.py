"""vostok.derive - report.json + the rich indexes -> the match roster.

The derivation nobody else may duplicate. Given artifacts that already exist -
objdiff's `report.json`, the two `binaries/rich/<side>/index.jsonl` inventories,
the reviewed override tables - this decides, for every target function:

    which base function it IS          (aliases: ICF folds and ??__E/??__F pairs
                                        spell the same body two different ways)
    which module OWNS it               (modules: ICF makes the PDB's owner and
                                        the source's owner disagree)
    whether its STRUCTURE matches      (classify: MATCH/SIZE/QUANTITY/SPLIT)
    what its proven MAX is             (maxima: scoped to a source hash, so it
                                        survives compiler churn and resets when
                                        the source it describes changes)

and writes the answer straight into the committed ledger
(`config/match_state.tsv`, owned by `vostok.ledger`), plus one
regenerable diagnostic - `binaries/base_only.tsv`, for the symbols only WE emit.
Nothing is cached in between: `roster.derive()` is a pure function of the
artifacts, so anything wanting the per-function verdict re-runs it (~7 s)
instead of querying a copy that can go stale.

Nothing here compiles anything: `vostok build` is the canonical build and calls
`roster.regen()` at the end of it; `refresh` is the regen-only path for an
already-built report.

    python3 -m vostok derive refresh          # re-derive from the built report
    python3 -m vostok ledger report --module render     # ask the record

Design: docs/binary_matching/match_db_design.md. The ledger answers BULK
questions (queues, per-TU reports, unpaired functions); `pdb_fetch` stays the
authoritative per-function view.
"""

import sys

_QUIET = False


def set_quiet(quiet=True):
    """Silence the progress commentary.

    `sema` and `diff tu-order` borrow the passes below to answer ONE question
    about ONE function. The derivation's running tally is noise there, and
    worse than noise: a pass count nobody asked for reads as though a rebuild
    just happened.
    """
    global _QUIET
    _QUIET = quiet


def log(msg):
    if not _QUIET:
        print(f"[derive] {msg}", file=sys.stderr)
