#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""tu_order - per-TU function definition-order diff (TARGET vs BASE).

Emits the ordered list of a translation unit's function signatures - one
`signature();` per line, in DEFINITION ORDER - for both the TARGET and the BASE,
as two plain-text blocks plus a unified diff between them.

Definition order == PDB SOURCE-LINE order: we order each side's functions by the
line-table `line` column the PDB records for the function's source definition (the
DB's {target,base}_functions.line), with the side's RVA only as a stable
tiebreaker. We deliberately do NOT order by the final-image RVA: the linker (with
/OPT, ICF and, on the optimized targets, LTCG) freely permutes COMDATs in the
image, so RVA order is link-scrambled on BOTH sides and reports a flood of FALSE
reorders (hundreds of vostok .cpp TUs) that do not correspond to any source move.
The PDB source line is what the .cpp actually declares, so a line-order mismatch
IS a real, source-editable definition-order divergence. (Functions with no source
line - compiler-emitted thunks, `dynamic initializer/atexit` records - carry
line 0/NULL and sort last; they are never genuine source reorders.)

Diffing the two blocks reveals:

  (a) TARGET-ONLY functions   (in target block, absent from base)  -> `+ ...`
      BASE-ONLY functions     (in base block, absent from target)  -> `- ...`
  (b) REARRANGEMENTS - functions present in BOTH but DECLARED at a different
      relative source line. Function definition order within a TU is structure, so
      a reordering is a directly actionable source fix: move the definition in the
      .cpp to the target's order.

DATA SOURCES, and each answers exactly one question:

  binaries/objdiff/report.json     WHICH TU. A unit is an object roster entry -
                                   exactly the functions emitted into that one
                                   .obj, i.e. the COMDAT membership. Nothing
                                   else knows this; the rich index records a
                                   function's SOURCE FILE, which is a different
                                   question (a header-inlined method's file is
                                   the header, not the .obj).
  binaries/rich/{target,base}/     WHICH FUNCTIONS, at which RVA, defined at
    index.jsonl                    which source line, under which signature.
  vostok.sema.pairing              WHETHER IT IS PAIRED. A target function whose
                                   RVA is nobody's pair is TARGET_ONLY, and
                                   likewise BASE_ONLY.

This used to read all three out of `binaries/match.db`, which held a derived
copy of each and could therefore be stale against them in ways nothing checked.

USAGE
  # list the TUs the DB knows (optionally filter by module / substring)
  python3 -m vostok diff tu-order --list [--module game] [PATTERN]

  # dump the two ordered blocks + unified diff for one TU
  python3 -m vostok diff tu-order vostok/game/sources/network_client_processing.cpp

  # short signatures (drop leading return type + vostok::/survarium:: noise)
  python3 -m vostok diff tu-order --short vostok/game/sources/object.cpp

  # only the reordering verdict (skip the full blocks)
  python3 -m vostok diff tu-order --reorder-only vostok/game/sources/human_npc.cpp

A TU argument may be a full unit path or a unique trailing substring (e.g. just
`network_client_processing.cpp`); ambiguous substrings list the candidates. Unit
paths are relative to the `sources/` root, exactly as the DB stores them.

The reordering count is the minimal set (longest-common-subsequence) of paired
functions that must move to make the source-line orders agree, annotated
`[t<target_pos> b<base_pos>]` (positions among the shared, line-ordered functions)
so the direction of the move is explicit. tu_order EXITS 1 when a TU has any
reordering (0 otherwise), so it can gate a structure-pass loop. Refresh the DB
(`python3 -m vostok build`) before trusting it after source moves.

Scope note: across all 1995 TUs the line-order signal flags ~9 genuine reorders
in vostok-own .cpp (mostly the not-yet-matched render module + one animation TU);
the old RVA-order key flagged ~300 vostok .cpp TUs - almost all link-scramble
noise, not source moves.
"""

import argparse
import difflib
import json
import re
import sys

from vostok.core.paths import REPORT

from vostok.derive.modules import module_of
from vostok.derive.scores import report_fuzzy_scores

from vostok.sema.pairing import pairing


class Roster:
    """report.json's object roster: which TU each emitted function belongs to."""

    def __init__(self):
        if not REPORT.is_file():
            sys.exit(f"[tu_order] no {REPORT} - run `python3 -m vostok build` first")
        report = json.loads(REPORT.read_text())
        self.units = sorted({unit["name"] for unit in report["units"]})
        _, self.units_by_mangled = report_fuzzy_scores(report)

    def unit_of(self, key, rec):
        """The TU a function is filed under.

        The record's OWN file wins when objdiff compared the symbol there, so a
        header-inlined method stays in its header instead of being filed under
        whichever .cpp won the fold; otherwise the first unit that compared it.
        """
        units = self.units_by_mangled.get(key)
        if units and rec["file"] in units:
            return rec["file"]
        return units[0] if units else None


def resolve_unit(roster, tu):
    """Map a user-supplied TU (full path or unique trailing substring) to the
    canonical unit name. Exits with the candidate list if ambiguous/missing."""
    if tu in roster.units:
        return tu
    hits = [name for name in roster.units if name.endswith(tu)]
    # also try a loose contains-match if the trailing-substring match found nothing
    if not hits:
        hits = [name for name in roster.units if tu in name]
    if len(hits) == 1:
        return hits[0]
    if not hits:
        sys.exit(f"[tu_order] no unit matches {tu!r} (try --list)")
    sys.stderr.write(f"[tu_order] {tu!r} is ambiguous - candidates:\n")
    for h in hits:
        sys.stderr.write(f"  {h}\n")
    sys.exit(1)


# A demangled signature is the identity we diff on. To keep diffs clean we offer
# a --short rendering that drops the leading return type and the ubiquitous
# vostok::/survarium:: namespace prefixes, while leaving everything that
# distinguishes overloads (the parameter list, const, etc.) intact.
_RET_PREFIXES = ("public:", "private:", "protected:", "static", "virtual")


def shorten(sig):
    s = sig
    # strip access/storage keywords MSVC sometimes leaves on
    changed = True
    while changed:
        changed = False
        for p in _RET_PREFIXES:
            if s.startswith(p + " "):
                s = s[len(p) + 1:]
                changed = True
    # drop a leading return type: everything up to the first identifier that is
    # immediately followed (eventually) by '('. Heuristic: if there is a space
    # before the first '(' that is not inside <...>, cut at the last top-level
    # space preceding the qualified name. Simplest robust rule: remove a single
    # leading "word ... " return-type run when the signature has a '(' and the
    # text before '(' contains a space at depth 0.
    depth = 0
    paren = None
    for i, ch in enumerate(s):
        if ch in "<([":
            depth += 1
        elif ch in ">)]":
            depth -= 1
        elif ch == "(" and depth == 1 and paren is None:
            paren = i
            break
    # find a depth-0 space in the qualified-name region [0, paren)
    if paren is not None:
        head = s[:paren]
        depth = 0
        cut = -1
        for i, ch in enumerate(head):
            if ch in "<([":
                depth += 1
            elif ch in ">)]":
                depth -= 1
            elif ch == " " and depth == 0:
                cut = i
        if cut != -1:
            s = s[cut + 1:]
    s = s.replace("vostok::", "").replace("survarium::", "")
    s = re.sub(r"\s+", " ", s).strip()
    return s


def fetch_side(roster, unit, side):
    """Ordered (rva, sym, line, demangled, paired) rows for one side of a TU.

    Ordered by the PDB SOURCE LINE the function is defined at (the line-table
    `line` column), NOT the final-image RVA - the linker scrambles RVA order on
    both sides (see module docstring). RVA is only a stable tiebreaker for two
    functions on the same line. Functions with no recorded line (0/NULL: thunks,
    dynamic initializer/atexit records) sort last, since they have no source
    position to be "out of order".

    `paired` is per-RVA, not per-name: ICF can fold two symbols onto one body, and
    the body is what got matched."""
    pair = pairing()
    inventory = pair.target if side == "target" else pair.base
    partnered = (pair.base_rva_by_target_rva if side == "target"
                 else pair.target_rva_by_base_rva)
    rows = []
    for key, rec in inventory.items():
        if roster.unit_of(key, rec) != unit:
            continue
        rows.append({
            "rva": rec["rva"],
            "sym": key,
            "line": min((s["line"] for s in rec["statements"] if s.get("line")),
                        default=None),
            "demangled": pair.demangled(key),
            "paired": rec["rva"] in partnered,
        })
    rows.sort(key=lambda r: (r["line"] is None, r["line"] or 0, r["rva"]))
    return rows


def render_block(rows, short):
    lines = []
    for r in rows:
        sig = shorten(r["demangled"]) if short else r["demangled"]
        lines.append(f"{sig};")
    return lines


def reorder_report(target_rows, base_rows):
    """Find functions present in BOTH sides whose relative order differs.

    We compare the sequence of *shared* symbols on each side. Using difflib's
    LCS we mark the symbols NOT on the longest common subsequence: those are the
    ones that would have to move to make the orders agree - i.e. the source
    definitions emitted out of target order in our build."""
    t_syms = [r["sym"] for r in target_rows if r["paired"]]
    b_syms = [r["sym"] for r in base_rows if r["paired"]]
    shared = set(t_syms) & set(b_syms)
    t_seq = [s for s in t_syms if s in shared]
    b_seq = [s for s in b_syms if s in shared]
    if t_seq == b_seq:
        return []  # same order, nothing to do
    sm = difflib.SequenceMatcher(a=t_seq, b=b_seq, autojunk=False)
    common = set()
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag == "equal":
            common.update(t_seq[i1:i2])
    moved = [s for s in t_seq if s not in common]
    sym_name = {r["sym"]: r["demangled"] for r in target_rows}
    # target/base relative positions among the shared sequence, so the verdict
    # tells you which way a definition has to move.
    t_pos = {s: i for i, s in enumerate(t_seq)}
    b_pos = {s: i for i, s in enumerate(b_seq)}
    return [(s, sym_name.get(s, f"sym#{s}"), t_pos[s], b_pos[s]) for s in moved]


def cmd_list(roster, args):
    rows = [(module_of(name), name) for name in roster.units]
    if args.module:
        rows = [r for r in rows if r[0] == args.module]
    if args.pattern:
        rows = [r for r in rows if args.pattern in r[1]]
    for module, name in sorted(rows):
        print(f"{module:<14} {name}")


def cmd_show(roster, args):
    unit = resolve_unit(roster, args.tu)
    t_rows = fetch_side(roster, unit, "target")
    b_rows = fetch_side(roster, unit, "base")
    if not t_rows and not b_rows:
        sys.exit(f"[tu_order] unit {unit!r} has no functions on either side")

    t_block = render_block(t_rows, args.short)
    b_block = render_block(b_rows, args.short)

    if not args.reorder_only:
        print(f"# UNIT: {unit}")
        print(f"# TARGET functions: {len(t_block)}   BASE functions: {len(b_block)}")
        print()
        print("===== TARGET (definition/source-line order) =====")
        print("\n".join(t_block) if t_block else "(none)")
        print()
        print("===== BASE (definition/source-line order) =====")
        print("\n".join(b_block) if b_block else "(none)")
        print()
        print("===== UNIFIED DIFF (target -> base)  [+ = target-only, - = base-only] =====")
        diff = difflib.unified_diff(
            t_block, b_block,
            fromfile=f"{unit} [TARGET]", tofile=f"{unit} [BASE]", lineterm="")
        any_diff = False
        for line in diff:
            any_diff = True
            print(line)
        if not any_diff:
            print("(blocks identical)")
        print()

    # presence summary + reorderings (the actionable structure output)
    t_paired = {r["sym"] for r in t_rows if r["paired"]}
    b_paired = {r["sym"] for r in b_rows if r["paired"]}
    target_only = [r for r in t_rows if not r["paired"]]
    base_only = [r for r in b_rows if not r["paired"]]
    moved = reorder_report(t_rows, b_rows)

    print("===== PRESENCE =====")
    print(f"paired:       {len(t_paired & b_paired)}")
    print(f"TARGET_ONLY:  {len(target_only)}")
    for r in target_only:
        print(f"  + {shorten(r['demangled']) if args.short else r['demangled']}")
    print(f"BASE_ONLY:    {len(base_only)}")
    for r in base_only:
        print(f"  - {shorten(r['demangled']) if args.short else r['demangled']}")
    print()
    print("===== REORDERINGS (paired fns out of target definition order) =====")
    if not moved:
        print("(none - paired functions are in the same relative order)")
    else:
        print(f"{len(moved)} function(s) emitted out of target order in our source")
        print("  (target_pos -> base_pos among shared fns):")
        for _sym, name, tp, bp in moved:
            disp = shorten(name) if args.short else name
            print(f"  ~ [t{tp:>3} b{bp:>3}] {disp}")

    return 1 if moved else 0


def main():
    ap = argparse.ArgumentParser(
        description="per-TU function definition-order diff (target vs base)")
    ap.add_argument("tu", nargs="?", help="TU path or unique trailing substring")
    ap.add_argument("--list", action="store_true",
                    help="list known units (optionally filtered by --module / pattern)")
    ap.add_argument("--module", help="filter --list by module")
    ap.add_argument("--pattern", help="substring filter for --list (or pass tu)")
    ap.add_argument("--short", action="store_true",
                    help="shorten signatures (drop return type + vostok::/survarium::)")
    ap.add_argument("--reorder-only", action="store_true",
                    help="print only presence + reordering verdict")
    args = ap.parse_args()

    roster = Roster()
    if args.list:
        # when --list is given, a positional tu acts as the substring filter
        if args.tu and not args.pattern:
            args.pattern = args.tu
        cmd_list(roster, args)
        return
    if not args.tu:
        ap.error("a TU is required (or pass --list)")
    sys.exit(cmd_show(roster, args))


if __name__ == "__main__":
    main()
