# SPDX-License-Identifier: GPL-3.0-or-later

"""vostok evidence - one function's matching evidence, reduced to what needs action.

    python3 -m vostok evidence FUNCTION... [--full] [--json]
                               [--only SECTION[,...]] [--skip SECTION[,...]]

FUNCTION is a mangled name, a signature, a qualified-name tail
(`weapon_core::tick`), a substring, or a hex retail/our RVA or VA. A selector
that names several functions lists them and exits 2.

In one process, from the build's PDB evidence databases and the committed
ledger (no rebuild, no subprocess), it reads:

    ledger     status, cur/max/hist, `tried` count and note
    structure  the statement partition, as `vostok pdb inspect --view
               structure-diff` reports it (an in-process port of that view)
    geometry   the relative-line check of `vostok tool geometry`, reduced to
               the statements where retail's line spacing departs from ours
    flow       the `vostok sema blocks --diff` verdict
    calls      callees one side makes and the other does not (inline-vs-call);
               `--full` adds the retail callers, which scans every payload

The first line is the verdict:

    STRUCTURE MATCH, 2 SIZE residuals; cur 97.30, max 98.10 (held)
    STRUCTURE DIFFERS: 3 TRGT_ONLY (+1 SIZE), geometry off (2 breaks); cur 81.20
    MATCH 100%: structure match

Below it comes only what needs action: diverging statements with their line and
source, SIZE residuals with byte deltas (ours minus retail), geometry breaks,
the flow divergence or the blocks whose bodies differ, unmatched calls, and the
ledger note. Everything that matches is left out; `--full` shows it, `--json`
gives agents every field.

`--only`/`--skip` take section names (aliases: blocks/sema -> flow, xref ->
calls) and repeat or take comma lists.

Exit status: 0 when nothing needs action (structure, geometry and flow match
and the ledger max is 100, or no ledger row), 1 when something does, 2 when a
selector is ambiguous or unknown, or the evidence is missing.
"""

from __future__ import annotations

import argparse
import json
import sys
import time

from vostok.core.paths import BASE_EVIDENCE, TARGET_EVIDENCE
from vostok.evidence import gather as _gather
from vostok.evidence import render as _render
from vostok.evidence import select as _select

AMBIGUITY_LIST = 12


def sections(only, skip) -> list[str]:
    def names(values):
        chosen = set()
        for value in values or []:
            for name in filter(None, (part.strip().lower() for part in value.split(","))):
                name = _gather.ALIASES.get(name, name)
                if name not in _gather.SECTIONS:
                    raise SystemExit(
                        f"vostok evidence: unknown section {name!r}; choose from "
                        + ", ".join([*_gather.SECTIONS, *sorted(_gather.ALIASES)]))
                chosen.add(name)
        return chosen

    wanted, unwanted = names(only), names(skip)
    return [s for s in _gather.SECTIONS
            if (not wanted or s in wanted) and s not in unwanted]


def _public(rec):
    if rec is None:
        return None
    keep = ("name", "mangled", "file", "rva", "image_base", "size", "aliases")
    return {key: rec[key] for key in keep if key in rec}


def needs_action(report) -> bool:
    if report["target"] is None or report["base"] is None:
        return True
    st, geo, flow = report.get("structure"), report.get("geometry"), report.get("flow")
    if st and (st["verdict"] != "MATCH"):
        return True
    if geo and not geo["match"]:
        return True
    if flow and (not flow["same"] or any(not d["calls_only"] for d in flow["differing"])):
        return True
    score = report.get("score") or {}
    return bool(score) and not score["exact"]


def evidence(selector, wanted, full=False, target_db=TARGET_EVIDENCE,
             base_db=BASE_EVIDENCE) -> dict:
    """The whole report for one selector, as data (what `--json` prints)."""
    started = time.monotonic()
    report = {"selector": selector, "schema": "vostok.evidence.v1"}
    try:
        target, base, target_con, base_con = _select.resolve(selector, target_db, base_db)
    except _select.Ambiguous as exc:
        report.update(rc=2, target=None, base=None,
                      error=f"AMBIGUOUS: '{selector}' names {len(exc.candidates)} functions; "
                            "pass a mangled name or an rva",
                      candidates=[{"side": c.get("side", "retail"), **_public(c)}
                                  for c in exc.candidates[:AMBIGUITY_LIST]],
                      more=max(0, len(exc.candidates) - AMBIGUITY_LIST))
        report["verdict"] = report["error"]
        return report
    except (LookupError, FileNotFoundError) as exc:
        report.update(rc=2, target=None, base=None, error=f"ERROR: {exc}")
        report["verdict"] = report["error"]
        return report
    row = _select.ledger_row(target) if target else None
    for rec, con in ((target, target_con), (base, base_con)):
        if rec is not None:
            _select.payload(con, rec)
    report.update(_gather.gather(target, base, row, wanted, target_con, full))
    report["score"] = _gather.score(row)
    report["target"], report["base"] = _public(target), _public(base)
    report["rc"] = 1 if needs_action(report) else 0
    report["verdict"] = _render.verdict(report)
    report["seconds"]["total"] = round(time.monotonic() - started, 3)
    target_con.close()
    base_con.close()
    return report


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(prog="vostok evidence", description=__doc__.split("\n\n")[0],
                                     epilog=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("functions", nargs="+", metavar="FUNCTION",
                        help="mangled name, signature, name substring, or hex rva/va")
    parser.add_argument("--full", action="store_true",
                        help="the complete detail, matched rows and callers included")
    parser.add_argument("--json", action="store_true", help="one JSON document, every field")
    parser.add_argument("--only", action="append", metavar="SECTION[,...]",
                        help="run only these sections: " + ", ".join(_gather.SECTIONS))
    parser.add_argument("--skip", action="append", metavar="SECTION[,...]",
                        help="leave these sections out")
    args = parser.parse_args(sys.argv[1:] if argv is None else argv)
    wanted = sections(args.only, args.skip)
    if not wanted:
        parser.error("--only/--skip left no sections")
    reports = [evidence(selector, wanted, args.full) for selector in args.functions]
    if args.json:
        json.dump(reports[0] if len(reports) == 1 else reports, sys.stdout, indent=1,
                  default=str)
        print()
    else:
        sys.stdout.write("\n".join(_render.render(r, args.full) for r in reports))
    return max(r["rc"] for r in reports)
