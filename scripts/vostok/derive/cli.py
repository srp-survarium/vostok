# SPDX-License-Identifier: GPL-3.0-or-later

"""vostok derive - re-derive the matching ledger from the built artifacts.

One verb, because there is one thing to do. The derivation reads

  binaries/objdiff/report.json          per-TU roster + fuzzy %s
  binaries/rich/target/index.jsonl      exe-level target inventory + statements
  binaries/rich/base/index.jsonl        same for our build
  binaries/rich/target/declarations.jsonl   PDB declaration records (optional)

and writes config/match_state.tsv (the committed record) plus
binaries/base_only.tsv (the diagnostic for symbols only WE emit). `vostok build`
is the canonical build and runs this at the end of every build; run it by hand
only to re-derive from an artifact set that is already on disk.

Queries live with the record they read - `vostok ledger report|list|queue`,
which needs no build and no database. The verbs that only made sense over the
retired sqlite cache are gone: `sql` (the escape hatch for an unreadable
schema), `diff` (`git diff config/match_state.tsv` is line
oriented and strictly better), `max` (`max` is a ledger column, and
`ledger list --headroom` covers the interesting case), and `list` / `report` /
`queue` / `tried` / `flag`, which `vostok ledger` answers from the committed
record - durably, since nothing re-projects over them any more.

Usage:
  python3 -m vostok derive refresh
"""

import argparse

from vostok.derive.roster import cmd_refresh


def main():
    ap = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter
    )
    sub = ap.add_subparsers(dest="cmd", required=True)
    sub.add_parser(
        "refresh",
        help="re-derive the ledger from the already-built report.json "
        "(`vostok build` runs this itself; run it by hand only when the "
        "artifacts are already on disk)",
    )
    cmd_refresh(ap.parse_args())
