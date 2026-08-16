"""vostok.ledger - the committed record of the matching campaign.

`docs/binary_matching/match_state.tsv` is one text row per target function and
is the campaign's memory: what has been proven, what was attempted, what is
parked and why. It is TEXT because it is committed - a conflict is an ordinary
text conflict and `git diff` names exactly which functions moved. (The old
SQLite `match.db` re-serialised its pages on every write, so every matching
commit stored a fresh ~4 MB blob; `binaries/match.db` survives only as a
gitignored derivation cache.)

    store    the file format, and `hist` - the all-time peak it ratchets so a
             peak can never be lost. `max` is NOT decided here: the hash-scoped
             reset lives in `vostok.derive.maxima`/`roster`, and `export_from_db`
             projects it (plus cls/tries/note/status) out of the cache.
    cli      query and mutate: report / list / queue / tried / park / open.
             Needs no build: it reads the committed ledger directly. Its three
             mutating verbs are re-derived from the cache by the next build - see
             the CAVEAT in `cli`.
    readme   the README score block, rendered from match.db
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
