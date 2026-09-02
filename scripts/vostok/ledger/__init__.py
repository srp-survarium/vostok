# SPDX-License-Identifier: GPL-3.0-or-later

"""vostok.ledger - the committed record of the matching campaign.

`config/match_state.tsv` is one text row per target function and
is the campaign's memory: what has been proven, what was attempted, what is
parked and why. It is TEXT because it is committed - a conflict is an ordinary
text conflict and `git diff` names exactly which functions moved. (The old
SQLite `match.db` re-serialised its pages on every write, so every matching
commit stored a fresh ~4 MB blob; it then survived a while as a gitignored
derivation cache, and is now gone entirely - the derivation writes here.)

    store    the file format, `project` - the build path that folds a fresh
             derivation onto this record - and `hist`, the all-time peak it
             ratchets so a peak can never be lost. `max` is NOT decided here:
             the hash-scoped reset lives in `vostok.derive.maxima`.
    cli      query and mutate: report / list / queue / tried / park / open.
             Needs no build: it reads the committed ledger directly, and its
             mutating verbs are durable because nothing re-projects over them.
    readme   the README score block, rolled up from this record
    queue    the structure-mismatch worklist, projected into a markdown queue

    python3 -m vostok.ledger report --module render
    python3 -m vostok.ledger list --module render --class QUANTITY,SPLIT
    python3 -m vostok.ledger readme [--write-readme]
    python3 -m vostok.ledger mismatch-queue [--write-queue]

`vostok ledger <verb>` on the umbrella CLI is the same dispatch; `store` has no
verb of its own and runs as `python3 -m vostok.ledger.store`.
"""

from __future__ import annotations

import sys


def main(argv: list[str] | None = None) -> int:
    """Dispatch one ledger verb. Unknown verbs fall through to `cli`, whose
    argparse names the valid set."""
    argv = list(sys.argv[1:] if argv is None else argv)
    if argv and argv[0] == "readme":
        from vostok.ledger import readme
        sys.argv = ["vostok.ledger readme", *argv[1:]]
        readme.main()
        return 0
    if argv and argv[0] == "mismatch-queue":
        from vostok.ledger import queue
        sys.argv = ["vostok.ledger mismatch-queue", *argv[1:]]
        return queue.main()
    from vostok.ledger import cli
    cli.main()
    return 0
