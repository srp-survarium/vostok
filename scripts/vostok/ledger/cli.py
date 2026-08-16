"""match - query and update the matching ledger.

Replaced vostok derive's query surface. That tool answered every question with
SQL over an interned schema - `list` alone joined symbols/units/pairs/history
to rebuild what is now one row of docs/binary_matching/match_state.tsv. The
ledger is 19,645 rows: loading it costs ~100 ms and answering costs under a
millisecond, so nothing here needs a database.

It also needs no build: the ledger is committed, so a fresh clone can answer
every question below before it has compiled anything.

  vostok ledger report  [--module M] [--per-unit]   rollup, byte-weighted
  vostok ledger list    [--module M] [--unit U] [--class C] [--status S]
                        [--max-size N] [--headroom] [--json]
  vostok ledger queue   --module M [--limit N]      one batch per TU, worst first
  vostok ledger tried   <mangled>... [--note ...]   record a dispatch attempt
  vostok ledger park    <mangled> --cause "..."     stop working it, with a reason
  vostok ledger open    <mangled>                   undo a park

Two more verbs live beside these and are dispatched by `vostok.ledger.__init__`,
so argparse below does not list them:

  vostok ledger readme         [--write-readme] [--max-code]   the README block
  vostok ledger mismatch-queue [--write-queue]                 the structure queue

tried/park/open are durable: they edit the committed record, and the next
`vostok build` folds its measurements ONTO that record (`ledger.store.project`)
rather than re-projecting it out of a cache, so a mark stays until someone
removes it.

Dropped from the old tool, deliberately:
  max     `max` is a column now; `list --headroom` covers the interesting case
  diff    `git diff docs/binary_matching/match_state.tsv` is line-oriented and
          strictly better than comparing two committed database revisions
  sql     the escape hatch existed because the schema was unreadable
"""

import argparse
import json
import sys

from vostok.ledger import store

EXACT = store.EXACT


def held(row):
    """'(held)' when this build dipped below the banked peak for the SAME body.

    Under LTCG/ICF a function's score moves without its source moving - a fold
    representative changes, an inline decision flips in a sibling TU. That is
    compiler noise, not a regression: the peak is still proven for this exact
    source. Say so loudly, so nobody spends an afternoon "fixing" it.
    """
    cur, mx = row.get("cur"), row.get("max")
    if cur is None or mx is None:
        return ""
    return f"  (held {mx:.1f})" if cur < mx - 0.01 else ""


def _rows(args):
    rows = list(store.load().values())
    if getattr(args, "module", None):
        rows = [r for r in rows if r["module"] == args.module]
    if getattr(args, "unit", None):
        rows = [r for r in rows if args.unit in (r["unit"] or "")]
    if getattr(args, "struct_class", None):
        wanted = {c.strip().upper() for c in args.struct_class.split(",")}
        rows = [r for r in rows if (r["cls"] or "") in wanted]
    if getattr(args, "status", None):
        wanted = {s.strip() for s in args.status.split(",")}
        rows = [r for r in rows if r["status"] in wanted]
    if getattr(args, "max_size", None):
        rows = [r for r in rows if r["size"] and r["size"] <= args.max_size]
    if getattr(args, "headroom", False):
        rows = [r for r in rows
                if (r["hist"] or 0) > (r["max"] or 0) + 0.01]
    return rows


def _pct(part, whole):
    return 100.0 * part / whole if whole else 0.0


def cmd_report(args):
    """Rollup per module (or per TU). The headline is byte-weighted: a 2 KB
    function at 50% is worth more than twenty 30-byte leaves at 100%."""
    rows = _rows(args)
    key = (lambda r: r["unit"] or "(no unit)") if args.per_unit else (
        lambda r: r["module"] or "(none)")
    groups = {}
    for row in rows:
        groups.setdefault(key(row), []).append(row)

    out = []
    for name, group in sorted(groups.items()):
        size = sum(r["size"] or 0 for r in group)
        weighted = sum((r["cur"] or 0) * (r["size"] or 0) for r in group)
        out.append({
            "scope": name,
            "fns": len(group),
            "done": sum(1 for r in group if r["status"] == "done"),
            "struct_match": sum(1 for r in group if r["cls"] == "MATCH"),
            "blocked": sum(1 for r in group if r["status"] == "blocked"),
            "parked": sum(1 for r in group if r["status"] == "parked"),
            "open": sum(1 for r in group if r["status"] == "inprogress"),
            "headroom": sum(1 for r in group
                            if (r["hist"] or 0) > (r["max"] or 0) + 0.01),
            "held": sum(1 for r in group if held(r)),
            "bytes": size,
            "weighted_pct": round(weighted / size, 2) if size else 0.0,
            "max_pct": round(
                sum((r["max"] or 0) * (r["size"] or 0) for r in group) / size, 2
            ) if size else 0.0,
        })
    if args.json:
        print(json.dumps(out, indent=1))
        return
    if not out:
        print("(no rows)", file=sys.stderr)
        return
    head = f"{'scope':52s} {'fns':>6s} {'done':>6s} {'open':>6s} {'park':>5s} {'blkd':>5s} {'held':>5s} {'head':>5s} {'cur%':>7s} {'max%':>7s}"
    print(head)
    for r in out:
        print(f"{r['scope'][:52]:52s} {r['fns']:6d} {r['done']:6d} {r['open']:6d} "
              f"{r['parked']:5d} {r['blocked']:5d} {r['held']:5d} {r['headroom']:5d} "
              f"{r['weighted_pct']:7.2f} {r['max_pct']:7.2f}")


def cmd_list(args):
    rows = sorted(_rows(args), key=lambda r: (-(r["size"] or 0), r["mangled"]))
    if args.limit:
        rows = rows[: args.limit]
    if args.json:
        print(json.dumps(rows, indent=1))
        return
    if not rows:
        print("(no rows)", file=sys.stderr)
        return
    for r in rows:
        cur = f"{r['cur']:.1f}" if r["cur"] is not None else "-"
        mx = f"{r['max']:.1f}" if r["max"] is not None else "-"
        print(f"{r['size'] or 0:6d} {cur:>6s} {mx:>6s} {r['cls'] or '':9s} "
              f"{r['status']:10s} {r['mangled'][:88]}{held(r)}")


def cmd_queue(args):
    """One batch per TU: a matcher owns a whole TU, so its small helpers are
    matched in their real inlining context instead of as cross-TU churn.

    Frameless (LTCG-customised) leaves are skipped - they have no stable frame
    to match against. TUs are ordered worst-first by byte-weighted match level,
    so real unmatched code outranks 99% polish, with size breaking ties.
    """
    rows = [r for r in _rows(args)
            if r["status"] not in ("done", "parked")
            and "f" not in (r["flags"] or "")
            and (r["unit"] or "")]
    tus = {}
    for row in rows:
        tus.setdefault(row["unit"], []).append(row)

    def rank(item):
        _unit, group = item
        size = sum(r["size"] or 0 for r in group) or 1
        weighted = sum((r["cur"] or 0) * (r["size"] or 0) for r in group) / size
        return (weighted, -size)

    for unit, group in sorted(tus.items(), key=rank)[: args.limit or None]:
        size = sum(r["size"] or 0 for r in group)
        pct = (sum((r["cur"] or 0) * (r["size"] or 0) for r in group) / size
               if size else 0.0)
        print(f"=== {unit}: {len(group)} functions, 0x{size:x} bytes, "
              f"{pct:.1f}% matched")
        for r in sorted(group, key=lambda r: -(r["size"] or 0)):
            cur = f"{r['cur']:.1f}" if r["cur"] is not None else "-"
            tried = f"  tried:{r['tries']}" if r["tries"] else ""
            print(f"  {r['size'] or 0:6d} {cur:>6s}  {r['cls'] or 'TARGET_ONLY':11s} "
                  f"{r['mangled'][:80]}{tried}{held(r)}")


def _update(mangleds, mutate):
    rows = store.load()
    touched = 0
    for mangled in mangleds:
        row = rows.get(mangled)
        if row is None:
            print(f"[match] not in ledger: {mangled[:70]}", file=sys.stderr)
            continue
        mutate(row)
        touched += 1
    if touched:
        store.save(rows)
    print(f"[match] updated {touched} function(s)")


def cmd_tried(args):
    def mutate(row):
        row["tries"] = (row["tries"] or 0) + 1
        if args.note:
            row["note"] = args.note
    _update(args.mangled, mutate)


def cmd_park(args):
    def mutate(row):
        row["note"] = args.cause
        if row["status"] != "done":
            row["status"] = "parked"
    _update(args.mangled, mutate)


def cmd_open(args):
    def mutate(row):
        if row["status"] == "parked":
            row["status"] = "inprogress"
    _update(args.mangled, mutate)


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)

    def filters(p, with_limit=True):
        p.add_argument("--module")
        p.add_argument("--unit", help="substring of the TU path")
        p.add_argument("--class", dest="struct_class",
                       help="csv of MATCH,SIZE,QUANTITY,SPLIT")
        p.add_argument("--status", help="csv of done,inprogress,blocked,parked")
        p.add_argument("--max-size", type=lambda v: int(v, 0))
        p.add_argument("--headroom", action="store_true",
                       help="only rows where hist > max (we had it better once)")
        if with_limit:
            p.add_argument("--limit", type=int)
        p.add_argument("--json", action="store_true")

    p = sub.add_parser("report", help="rollup per module or TU (byte-weighted)")
    filters(p, with_limit=False)
    p.add_argument("--per-unit", action="store_true")
    p.set_defaults(func=cmd_report)

    p = sub.add_parser("list", help="list functions, largest first")
    filters(p)
    p.set_defaults(func=cmd_list)

    p = sub.add_parser("queue", help="one batch per TU, worst-matched first")
    filters(p)
    p.set_defaults(func=cmd_queue)

    p = sub.add_parser("tried", help="record a dispatch attempt")
    p.add_argument("mangled", nargs="+")
    p.add_argument("--note")
    p.set_defaults(func=cmd_tried)

    p = sub.add_parser("park", help="stop working a function, with a reason")
    p.add_argument("mangled", nargs="+")
    p.add_argument("--cause", required=True)
    p.set_defaults(func=cmd_park)

    p = sub.add_parser("open", help="undo a park")
    p.add_argument("mangled", nargs="+")
    p.set_defaults(func=cmd_open)

    args = ap.parse_args()
    args.func(args)


if __name__ == "__main__":
    main()
