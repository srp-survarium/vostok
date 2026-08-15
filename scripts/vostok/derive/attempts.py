"""vostok.derive.attempts - the two manual writes.

`tried` records that a dispatch included these functions, which is what lets the
queue demote work already attempted; `flag` parks a function with a cause, or
requeues it by forgetting its history. Everything else in this package is
derived - these two are the human's edits, and they live in the persistent
tables that survive a refresh.
"""

import sys

from vostok.derive import log
from vostok.derive.db import open_db


def cmd_tried(args):
    """Record that a dispatch included these functions (orchestrator, after
    each worker returns). Increments per-function attempt counts; the queue
    demotes fully-tried TUs so they come back later instead of being retried
    first. --unit marks EVERY function of a TU - even ones already at 100% - so a
    later `diff` shows the whole TU was touched, not just the functions that moved."""
    import datetime

    con = open_db(check_schema=True)
    today = datetime.date.today().isoformat()
    mangleds = list(args.mangled)
    if args.unit:
        q = """SELECT s.mangled FROM target_functions t JOIN symbols s ON s.id = t.sym
               LEFT JOIN units u ON u.id = t.unit WHERE u.name = ?"""
        mangleds += [r["mangled"] for r in con.execute(q, (args.unit,))]
    if args.done:
        # every fn that EVER reached 100% (by max) and isn't tried yet -> tries=1, so
        # the done set drops out of the queue until everything else is exhausted.
        q = """SELECT s.mangled FROM target_functions t JOIN symbols s ON s.id = t.sym
               LEFT JOIN pairs p ON p.sym = t.sym
               LEFT JOIN history h ON h.mangled = s.mangled
               LEFT JOIN attempts a ON a.mangled = s.mangled
               WHERE coalesce(h.best_fuzzy_pct, p.fuzzy_pct, 0) >= 100 AND a.mangled IS NULL"""
        mangleds += [r["mangled"] for r in con.execute(q)]
    if not mangleds:
        sys.exit("[match_db] nothing to mark - pass mangled names and/or --unit / --done")
    for m in mangleds:
        con.execute(
            """INSERT INTO attempts VALUES (?, 1, ?, ?)
               ON CONFLICT(mangled) DO UPDATE SET
                 n = n + 1, last_at = excluded.last_at,
                 note = coalesce(excluded.note, note)""",
            (m, today, args.note),
        )
    con.commit()
    log(f"marked {len(mangleds)} function(s) tried")


def cmd_flag(args):
    import datetime

    con = open_db()
    today = datetime.date.today().isoformat()
    if args.requeue:
        # manual override of the history-derived out-of-scope/matched-before
        # skip: forget the function's history + flags so queues offer it again
        n = con.execute("DELETE FROM history WHERE mangled = ?", (args.mangled,)).rowcount
        m = con.execute("DELETE FROM flags WHERE mangled = ?", (args.mangled,)).rowcount
        log(f"requeued {args.mangled}: dropped {n} history row(s), {m} flag(s)")
    else:
        if not args.cause:
            sys.exit("[match_db] --cause is required when setting a flag")
        con.execute(
            "INSERT OR REPLACE INTO flags VALUES (?,?,?,?)",
            (args.mangled, args.flag, args.cause, today),
        )
        log(f"flagged {args.mangled} {args.flag}")
    con.commit()
