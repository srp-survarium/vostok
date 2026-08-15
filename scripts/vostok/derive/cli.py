"""match_db - sqlite queue/report database over the delink/diff artifacts.

Design: docs/binary_matching/match_db_design.md. The DB answers BULK questions
(queues, per-TU reports, unpaired functions); pdb_fetch stays the authoritative
per-function view. Derived tables are regenerated from the already-built diff
artifacts by `rebuild.py` (at the end of every build) or, regen-only, by
`refresh` (run rebuild.py first if sources moved):

  binaries/objdiff/report.json          per-TU roster + fuzzy %s
  binaries/rich/target/index.jsonl      exe-level target inventory + statements
  binaries/rich/base/index.jsonl        same for our build
  binaries/rich/target/declarations.jsonl   PDB declaration records (optional)

Names are interned (symbols/units/files tables, ids assigned in sorted order)
so the committed DB stays small and refreshes are byte-deterministic. The
persistent tables (history, flags) are keyed by mangled TEXT - ids are NOT
stable across refreshes, mangled names are.

Usage:
  python3 scripts/match_db.py refresh    # regen-only (rebuild.py first if sources moved)
  python3 scripts/match_db.py list --module game_core --max-size 0x80 [--json]
  python3 scripts/match_db.py list --module network_core --presence TARGET_ONLY
  python3 scripts/match_db.py report [--module game_core] [--per-unit]
  python3 scripts/match_db.py sql "SELECT ... "          # read-only
"""

import argparse
import sys

from vostok.core.paths import BINARIES, MATCH_DB_LOG
from vostok.core.paths import REPO as VOSTOK

from vostok.derive.attempts import cmd_flag, cmd_tried
from vostok.derive.diff import cmd_diff
from vostok.derive.query import cmd_list, cmd_max, cmd_sql, parse_size
from vostok.derive.queue import cmd_queue
from vostok.derive.report import cmd_report
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
            f.write(f"[{ts}][{branch}]: match_db.py {' '.join(sys.argv[1:])}\n")
    except OSError:
        pass


def main():
    audit_log()
    ap = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter
    )
    sub = ap.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser(
        "refresh",
        help="regen-only: rebuild derived tables from the already-built report.json "
        "(rebuild.py is the canonical build and regenerates the DB itself; run it "
        "first if sources moved)",
    )

    p = sub.add_parser("list", help="list functions with filters")
    p.add_argument("--module")
    p.add_argument("--unit", help="TU path, e.g. vostok/game_core/sources/weapon_core.cpp")
    p.add_argument("--max-size", type=parse_size, help="max target size (0x.. ok)")
    p.add_argument("--class", dest="struct_class", help="csv of MATCH,SIZE,SPLIT,QUANTITY")
    p.add_argument(
        "--presence",
        choices=["PAIRED", "TARGET_ONLY", "BASE_ONLY"],
        help="default PAIRED",
    )
    p.add_argument(
        "--status",
        help="BASE_ONLY taxonomy filter: NEAR_MISS|JITTER|INLINED_IN_TARGET|UNEXPLAINED|COMPILER",
    )
    p.add_argument("--json", action="store_true")

    p = sub.add_parser(
        "max",
        help="list effective-source-hash-scoped MAX evidence (not ordinary history)",
    )
    p.add_argument("--module")
    p.add_argument("--below", type=float, help="only rows whose MAX is below this percent")
    p.add_argument("--json", action="store_true")





    p = sub.add_parser("report", help="per-module/TU rollup")
    p.add_argument("--module")
    p.add_argument(
        "--unit",
        help="filter to ONE unit by name or substring (e.g. 'medkit'); --module "
        "optional. Refuses with paste-ready full names if the substring is ambiguous",
    )
    p.add_argument("--per-unit", action="store_true")
    p.add_argument(
        "--per-function",
        action="store_true",
        help="list every function of ONE --unit (pct/cls/size/flag), 100%%->0%%",
    )
    p.add_argument(
        "--function",
        help="list every function whose demangled name CONTAINS this substring "
        "(e.g. 'medkit::'), across units; --module optional",
    )
    p.add_argument(
        "--lite",
        action="store_true",
        help="lean view: drop any '(no unit)' catch-all row and the custom_conv/"
        "out_of_scope/suspicious columns",
    )
    p.add_argument(
        "--verbose",
        action="store_true",
        help="fn column = the full (capped) demangled signature instead of the "
        "mangled-derived scope::name",
    )
    p.add_argument("--json", action="store_true")

    p = sub.add_parser("queue", help="one batch per TU (all its open functions), small-first")
    p.add_argument("--module", required=True)
    p.add_argument("--limit", type=int, help="show only the first N TUs")
    p.add_argument(
        "--include-frameless",
        action="store_true",
        help="also queue LTCG-customized (frameless) leaves - normally pointless",
    )
    p.add_argument(
        "--stale-ok",
        action="store_true",
        help="emit the queue even when the DB predates source changes (rows may be stale)",
    )
    p.add_argument("--json", action="store_true")

    p = sub.add_parser("tried", help="record a dispatch attempt; queue demotes tried work")
    p.add_argument("mangled", nargs="*")
    p.add_argument("--unit", help="mark every function of this TU (even fully matched)")
    p.add_argument(
        "--done",
        action="store_true",
        help="mark every fn that reached a full match (by max) and isn't tried yet -> "
        "keeps the done set out of the queue until everything else is exhausted",
    )
    p.add_argument("--note", help="optional context, e.g. the worker's park causes")

    p = sub.add_parser("sql", help="read-only SQL escape hatch")
    p.add_argument("query")
    p.add_argument("--json", action="store_true")

    p = sub.add_parser("flag", help="manual override: set a flag or requeue")
    p.add_argument("mangled")
    g = p.add_mutually_exclusive_group(required=True)
    g.add_argument("--flag", choices=["OUT_OF_SCOPE", "SKIP"])
    g.add_argument(
        "--requeue",
        action="store_true",
        help="forget history+flags so queues offer the function again",
    )
    p.add_argument("--cause")


    p = sub.add_parser(
        "diff", help="function-level diff between two committed match.db revisions")
    p.add_argument("spec", help="<hash> (vs working tree) or <hash>..<hash>")
    p.add_argument("--module")
    p.add_argument(
        "--verbose",
        action="store_true",
        help="fn column = the full (capped) demangled signature instead of scope::name",
    )
    p.add_argument("--json", action="store_true")

    args = ap.parse_args()
    {
        "refresh": cmd_refresh,
        "list": cmd_list,
        "max": cmd_max,
        "report": cmd_report,
        "queue": cmd_queue,
        "sql": cmd_sql,
        "tried": cmd_tried,
        "flag": cmd_flag,
        "diff": cmd_diff,
    }[args.cmd](args)
