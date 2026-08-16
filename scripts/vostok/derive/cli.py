"""vostok derive - re-derive the matching ledger from the built artifacts.

One verb, because there is one thing to do. The derivation reads

  binaries/objdiff/report.json          per-TU roster + fuzzy %s
  binaries/rich/target/index.jsonl      exe-level target inventory + statements
  binaries/rich/base/index.jsonl        same for our build
  binaries/rich/target/declarations.jsonl   PDB declaration records (optional)

and writes docs/binary_matching/match_state.tsv (the committed record) plus
binaries/base_only.tsv (the diagnostic for symbols only WE emit). `vostok build`
is the canonical build and runs this at the end of every build; run it by hand
only to re-derive from an artifact set that is already on disk.

Queries live with the record they read - `vostok ledger report|list|queue`,
which needs no build and no database. The verbs that only made sense over the
retired sqlite cache are gone: `sql` (the escape hatch for an unreadable
schema), `diff` (`git diff docs/binary_matching/match_state.tsv` is line
oriented and strictly better), `max` (`max` is a ledger column, and
`ledger list --headroom` covers the interesting case), and `list` / `report` /
`queue` / `tried` / `flag`, which `vostok ledger` answers from the committed
record - durably, since nothing re-projects over them any more.

Usage:
  python3 -m vostok derive refresh
"""

import argparse
import sys

from vostok.core.paths import BINARIES, MATCH_DB_LOG
from vostok.core.paths import REPO as VOSTOK
from vostok.derive.roster import cmd_refresh


def audit_log():
    """Append this invocation to binaries/match_db.log (same format as
    pdb_fetch.log: [timestamp][branch]: command) so a run's command history
    is reconstructable. Never breaks the tool; skipped when binaries/ is absent."""
    try:
        log_dir = BINARIES
        if not log_dir.is_dir():
            return
        import datetime
        import subprocess

        ts = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")[:-4]
        try:
            branch = (
                subprocess.run(
                    ["git", "-C", str(VOSTOK), "rev-parse", "--abbrev-ref", "HEAD"],
                    capture_output=True,
                    text=True,
                    timeout=5,
                ).stdout.strip()
                or "?"
            )
        except Exception:
            branch = "?"
        with open(MATCH_DB_LOG, "a", encoding="utf-8") as f:
            f.write(f"[{ts}][{branch}]: vostok derive {' '.join(sys.argv[1:])}\n")
    except OSError:
        pass


def main():
    audit_log()
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
