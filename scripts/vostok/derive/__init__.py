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

and writes the answer to `binaries/match.db`, a gitignored cache, then projects
it into the committed ledger (`vostok.ledger`). Nothing here compiles anything:
`rebuild.py` is the canonical build and calls `roster.regen()` at the end of it;
`refresh` is the regen-only path for an already-built report.

    python3 -m vostok.derive report --module render
    python3 -m vostok.derive list --module game_core --max-size 0x80
    python3 -m vostok.derive queue --module render --limit 3
    python3 -m vostok.derive refresh

Design: docs/binary_matching/match_db_design.md. `scripts/match_db.py` remains
as a shim. The DB answers BULK questions (queues, per-TU reports, unpaired
functions); `pdb_fetch` stays the authoritative per-function view.
"""

import sys


def log(msg):
    print(f"[match_db] {msg}", file=sys.stderr)
