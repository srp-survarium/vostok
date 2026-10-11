# SPDX-License-Identifier: GPL-3.0-or-later

"""eh - compare MSVC x86 C++ exception-handling tables, retail vs our build.

Every function that must unwind (destructible locals or temporaries, or a
try/catch) gets an `__ehhandler$` stub, a FuncInfo, an unwind map whose
states each name a `.text$x` funclet that destroys one object, and a try-block
map with the caught types. Together they record WHICH objects need
destruction, in WHAT construction order and scope nesting, and the try/catch
shape - source shape the byte diff blurs. None of it has a PDB function
record, so the report, ledger and structure-diff never see it.

The engine itself compiles with ExceptionHandling=0, so its C++ EH tables all
belong to vendored and prebuilt code (STLport, SpeedTree, Boost, the CRT). Its
`__try` blocks still register SEH frames, so their scope tables (nesting,
__except vs __finally, filter/handler offsets) are compared the same way and
reported as kind `seh`.

    python3 -m vostok eh                    # whole-image summary
    python3 -m vostok eh --list             # one verdict line per divergent registration
    python3 -m vostok eh --list --class unwind_frame
    python3 -m vostok eh <fn>               # verdict, then both sides' tables
    python3 -m vostok eh ... --json

<fn> is a mangled name or a substring of either side's parent names.

Verdicts: MATCH, DIFFERS [classes], TARGET_ONLY (retail has a FuncInfo, our
function has none), BASE_ONLY (the reverse), UNPAIRED (the other side has no
function of that name), NONE (the function exists but neither side unwinds).
The classes are listed in `vostok.eh.compare`.

Inputs: the shipped exe+pdb (`$SURVARIUM_BIN`, else the pinned survarium-game)
and our `binaries/Win32` exe+pdb, read directly - the tables live in the image,
so this needs no delink, report or evidence database. Override either side with
--target-exe/--target-pdb/--base-exe/--base-pdb.

rc: summary and --list 0; <fn> 0 for MATCH/NONE, 1 for any divergence, 2 error.
"""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

from vostok.core import log as _log
from vostok.core import paths


def die(msg: str) -> None:
    sys.stderr.write(f"eh: {msg}\n")
    sys.exit(2)


def _load(args):
    from vostok.eh.compare import pair_registrations
    from vostok.eh.tables import Image
    from vostok.ledger import store

    for label, path in (("target exe", args.target_exe), ("base exe", args.base_exe)):
        if not path.is_file():
            die(f"{label} {path} missing - run `python3 -m vostok build` (base) or enter "
                "`nix develop` (target)")
    target = Image("target", args.target_exe,
                   args.target_pdb if args.target_pdb.is_file() else None)
    base = Image("base", args.base_exe, args.base_pdb if args.base_pdb.is_file() else None)
    ledger = store.load()
    pairing = None
    scored = any(name in ledger for reg in target.registrations for name in reg.parent_names)
    if scored and paths.TARGET_EVIDENCE.is_file() and paths.BASE_EVIDENCE.is_file():
        # a scored parent pairs the way the ledger pairs it; the rest by name
        from vostok.sema.pairing import pairing as sema_pairing

        loaded = sema_pairing()

        def pairing(rva):
            return loaded.partner_rva("target", rva)
    return target, base, ledger, pair_registrations(target, base, pairing)


def _verdict(pair) -> str:
    classes = pair.classes()
    tag = pair.verdict + (f" [{', '.join(classes)}]" if pair.verdict == "DIFFERS" else "")
    return f"{pair.kind:<4} {tag:<40} {pair.name}"


def _ledger_note(pair, ledger) -> str:
    from vostok.eh.compare import ledger_row

    row = ledger_row(pair, ledger)
    if row is None:
        return "not in the ledger"

    def num(value):
        return "-" if value is None else f"{value:g}"
    return (f"ledger cur={num(row.get('cur'))} max={num(row.get('max'))} "
            f"cls={row.get('cls') or '-'} status={row.get('status') or '-'}")


def _pair_json(pair, ledger) -> dict:
    from vostok.eh.compare import ledger_row

    def side(reg):
        if reg is None:
            return None
        if pair.kind == "seh":
            return {
                "parent_rva": reg.parent_rva, "names": reg.parent_names,
                "site_rva": reg.site_rva, "flavor": reg.flavor, "table_rva": reg.table_rva,
                "cookies": reg.cookies,
                "scopes": [{"enclosing": sc.enclosing, "kind": sc.kind,
                            "filter_rva": sc.filter_rva, "handler_rva": sc.handler_rva}
                           for sc in reg.scopes],
            }
        info = reg.info
        return {
            "parent_rva": reg.parent_rva, "names": reg.parent_names,
            "site_rva": reg.site_rva, "stub_rva": reg.stub_rva,
            "stub": reg.stub.text, "funcinfo_rva": info.rva, "magic": info.magic,
            "max_state": info.max_state, "eh_flags": info.eh_flags,
            "es_types": info.es_types,
            "unwind": [{"to_state": to, "action_rva": action,
                        "funclet": reg.funclets[action].text if action else None,
                        "callee": reg.funclets[action].callee if action else None,
                        "frame": reg.funclets[action].frame if action else None}
                       for to, action in info.unwind],
            "try_blocks": [{"low": tb.low, "high": tb.high, "catch_high": tb.catch_high,
                            "handlers": [{"type": h.type_name, "adjectives": h.adjectives,
                                          "disp_catch_obj": h.disp_catch_obj,
                                          "handler_rva": h.handler_rva}
                                         for h in tb.handlers]}
                           for tb in info.try_blocks],
        }
    row = ledger_row(pair, ledger)
    return {
        "kind": pair.kind, "name": pair.name, "mangled": pair.mangled,
        "verdict": pair.verdict, "paired_by": pair.paired_by,
        "classes": pair.classes(),
        "divergences": [{"class": d.kind, "detail": d.detail} for d in pair.divergences],
        "funclets": [{"target_rva": fp.target.rva, "base_rva": fp.base.rva,
                      "size": [fp.target.size, fp.base.size], "exact": fp.exact}
                     for fp in pair.funclets],
        "ledger": None if row is None else {k: row.get(k) for k in
                                            ("cur", "max", "hist", "cls", "status")},
        "target": side(pair.target), "base": side(pair.base),
    }


def cmd_summary(args) -> int:
    from vostok.eh.compare import summarize

    target, base, ledger, pairs = _load(args)
    summary = summarize(target, base, pairs, ledger)
    if args.json:
        json.dump({"counts": summary.counts, "classes": dict(summary.classes),
                   "hidden_mismatches": [_pair_json(p, ledger) for p in summary.hidden]},
                  sys.stdout, indent=1)
        print()
        return 0
    c = summary.counts
    print(f"C++ EH tables   target {target.exe.name}   base {base.exe.name}")
    rows = (("FuncInfo tables", "funcinfo"), ("__ehhandler stubs", "stubs"),
            ("registrations (parent functions)", "registrations"),
            ("  parents the PDB names", "parents_named"),
            ("  parents in the ledger", "in_ledger"),
            ("unwind states", "unwind_states"), ("try blocks", "try_blocks"),
            ("unwind funclets", "funclets"), ("  funclet bytes", "funclet_bytes"),
            ("catch handlers outside the parent", "handlers_outside_parent"),
            ("funclets that failed to decode", "decode_errors"))
    rows += (("SEH frames (__try)", "seh_frames"), ("  in the ledger", "seh_in_ledger"),
             ("  scopes", "seh_scopes"))
    print(f"  {'':36} {'target':>8} {'base':>8}")
    for label, key in rows:
        print(f"  {label:36} {c['target'][key]:>8} {c['base'][key]:>8}")
    for kind, title in (("c++", "C++ EH registrations"), ("seh", "SEH frames")):
        v = c["verdicts"][kind]
        paired = v.get("MATCH", 0) + v.get("DIFFERS", 0)
        print(f"\n{title}: paired {paired} (MATCH {v.get('MATCH', 0)}, DIFFERS "
              f"{v.get('DIFFERS', 0)}), target only {v.get('TARGET_ONLY', 0)}, base only "
              f"{v.get('BASE_ONLY', 0)}, unpaired {v.get('UNPAIRED', 0)}")
    fb, fe = c["funclet_bytes_paired"], c["funclet_bytes_exact"]
    pct = 100.0 * fe / fb if fb else 100.0
    print(f"funclets paired {c['funclet_pairs']}: exact {c['funclet_pairs_exact']} "
          f"({fe}/{fb} bytes, {pct:.2f}%)")
    print(f"ledger rows carrying EH or SEH tables: {c['ledger_rows_with_eh']}")
    if summary.classes:
        print("\ndivergence classes (registrations carrying each):")
        for kind, n in summary.classes.most_common():
            print(f"  {kind:14} {n}")
    print(f"\nledger rows at 100% whose EH diverges: {len(summary.hidden)}")
    for p in summary.hidden:
        print(f"  {_verdict(p)}")
    for side in ("target", "base"):
        for err in c[side]["scan_errors"]:
            print(f"[{side}] {err}", file=sys.stderr)
    return 0


def cmd_list(args) -> int:
    _, _, ledger, pairs = _load(args)
    if args.cls is not None:
        chosen = [p for p in pairs if args.cls in [*p.classes(), p.verdict]]
    else:
        chosen = [p for p in pairs if args.all or p.verdict != "MATCH"]
    if args.json:
        json.dump([_pair_json(p, ledger) for p in chosen], sys.stdout, indent=1)
        print()
        return 0
    order = {"DIFFERS": 0, "TARGET_ONLY": 1, "BASE_ONLY": 2, "UNPAIRED": 3, "MATCH": 4}
    for p in sorted(chosen, key=lambda p: (order[p.verdict], p.name)):
        print(_verdict(p))
    return 0


def _names(pair) -> list[str]:
    return [n for reg in (pair.target, pair.base) if reg for n in reg.parent_names]


def cmd_function(args) -> int:
    target, base, ledger, pairs = _load(args)
    low = args.fn.lower()
    hits = [p for p in pairs if any(low in n.lower() for n in _names(p))]
    hits = [p for p in hits if args.fn in _names(p)] or hits
    if not hits:
        known = any(args.fn == n or low in n.lower()
                    for img in (target, base) for n in img.symbols.by_name)
        if not known:
            die(f"no function matches {args.fn!r} on either side")
        if args.json:
            json.dump({"name": args.fn, "verdict": "NONE"}, sys.stdout)
            print()
        else:
            print(f"{'-':<4} {'NONE':<40} {args.fn}")
            print("  neither side registers an EH frame for it")
        return 0
    if len(hits) > 1 and not args.all:
        for p in hits[:20]:
            sys.stderr.write(f"  {_verdict(p)}\n")
        die(f"{args.fn!r} matches {len(hits)} registrations (listed above); pass a "
            "mangled name, or --all")
    if args.json:
        json.dump([_pair_json(p, ledger) for p in hits], sys.stdout, indent=1)
        print()
    else:
        for p in hits:
            _print_pair(p, ledger)
    return 0 if all(p.verdict == "MATCH" for p in hits) else 1


def _print_pair(pair, ledger) -> None:
    from vostok.eh.compare import seh_offsets

    print(_verdict(pair))
    note = _ledger_note(pair, ledger)
    if pair.paired_by and pair.paired_by != "name":
        note += f"; paired by {pair.paired_by}"
    print(f"  {note}")
    for d in pair.divergences:
        print(f"  {d.kind:14} {d.detail}")
    for label, reg in (("target", pair.target), ("base", pair.base)):
        if reg is None:
            continue
        parent = f"rva={reg.parent_rva:#x}" if reg.parent_rva is not None else "?"
        if pair.kind == "seh":
            print(f"\n  {label}: parent {parent}  {reg.flavor} scope table "
                  f"rva={reg.table_rva:#x}" + (f" cookies {reg.cookies}" if reg.cookies else ""))
            offsets = seh_offsets(reg) if reg.parent_rva is not None else []
            for k, sc in enumerate(reg.scopes):
                where = (f"filter {offsets[k][0]} handler {offsets[k][1]}" if offsets else
                         f"handler rva={sc.handler_rva:#x}")
                print(f"    scope {k:<3} in {sc.enclosing:<3} {sc.kind:10} {where}")
            continue
        info = reg.info
        print(f"\n  {label}: parent {parent}  FuncInfo rva={info.rva:#x} magic "
              f"{info.magic:#x} maxState {info.max_state} tries {len(info.try_blocks)} "
              f"EHFlags {info.eh_flags}")
        print(f"    stub  rva={reg.stub_rva:#x}  {reg.stub.summary()}")
        for state, (to, action) in enumerate(info.unwind):
            body = reg.funclets[action].summary() if action else "(no action)"
            print(f"    state {state:<3} -> {to:<3} {body}")
        for k, tb in enumerate(info.try_blocks):
            catches = ", ".join(
                f"catch({h.spelling()}) obj={h.disp_catch_obj:#x} handler rva={h.handler_rva:#x}"
                for h in tb.handlers)
            print(f"    try {k}: states {tb.low}..{tb.high} catchHigh {tb.catch_high}  "
                  f"{catches}")


def main() -> int:
    ap = argparse.ArgumentParser(
        prog="vostok eh", description=__doc__.split("\n\n")[0],
        formatter_class=argparse.RawDescriptionHelpFormatter, epilog=__doc__)
    ap.add_argument("fn", nargs="?", help="mangled name or substring of a parent name")
    ap.add_argument("--list", action="store_true", help="one verdict line per divergent "
                    "registration (with --all, every registration)")
    ap.add_argument("--class", dest="cls", metavar="CLASS",
                    help="list only registrations carrying this class or verdict")
    ap.add_argument("--all", action="store_true",
                    help="--list: include MATCH; <fn>: show every registration it matches")
    ap.add_argument("--json", action="store_true")
    ap.add_argument("--target-exe", type=Path, default=paths.target_exe())
    ap.add_argument("--target-pdb", type=Path, default=paths.target_pdb())
    ap.add_argument("--base-exe", type=Path, default=paths.BASE_EXE)
    ap.add_argument("--base-pdb", type=Path, default=paths.BASE_PDB)
    args = ap.parse_args()
    if args.fn:
        return cmd_function(args)
    if args.list or args.cls:
        return cmd_list(args)
    return cmd_summary(args)


if __name__ == "__main__":
    raise SystemExit(_log.run("vostok.eh", main))
