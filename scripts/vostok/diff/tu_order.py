#!/usr/bin/env python3
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

Data source: docs/binary_matching/match.db (the committed delink/diff DB). A TU
is the report.json object roster (the `units` table), which is exactly the set of
functions emitted into that one .obj - i.e. the COMDAT membership. Pairing
(present in both sides) comes from the `pairs` table; a target function whose RVA
is not a `pairs.target_rva` is TARGET_ONLY, and likewise for BASE_ONLY.

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
import re
import sqlite3
import sys

from vostok.core.paths import MATCH_DB as DB_PATH


def open_db(path=DB_PATH):
    if not path.is_file():
        sys.exit(f"[tu_order] no database at {path} - run `python3 -m vostok build` first")
    con = sqlite3.connect(path)
    con.row_factory = sqlite3.Row
    return con


def resolve_unit(con, tu):
    """Map a user-supplied TU (full path or unique trailing substring) to the
    canonical `units.name`. Exits with the candidate list if ambiguous/missing."""
    row = con.execute("SELECT name FROM units WHERE name = ?", (tu,)).fetchone()
    if row:
        return row["name"]
    like = f"%{tu}"
    hits = [r["name"] for r in con.execute(
        "SELECT name FROM units WHERE name LIKE ? ORDER BY name", (like,))]
    # also try a loose contains-match if the trailing-substring match found nothing
    if not hits:
        hits = [r["name"] for r in con.execute(
            "SELECT name FROM units WHERE name LIKE ? ORDER BY name", (f"%{tu}%",))]
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


def fetch_side(con, unit, side):
    """Ordered (rva, sym, line, demangled, paired) rows for one side of a TU.

    Ordered by the PDB SOURCE LINE the function is defined at (the line-table
    `line` column), NOT the final-image RVA - the linker scrambles RVA order on
    both sides (see module docstring). RVA is only a stable tiebreaker for two
    functions on the same line. Functions with no recorded line (0/NULL: thunks,
    dynamic initializer/atexit records) sort last, since they have no source
    position to be "out of order".

    `paired` is True when this function is matched to a function on the other
    side (its rva appears in pairs as this side's rva)."""
    rva_col = "target_rva" if side == "target" else "base_rva"
    table = f"{side}_functions"
    rows = con.execute(f"""
        SELECT fn.rva   AS rva,
               fn.sym   AS sym,
               fn.line  AS line,
               s.demangled AS demangled,
               (p.{rva_col} IS NOT NULL) AS paired
        FROM {table} fn
        JOIN units   u ON fn.unit = u.id
        JOIN symbols s ON fn.sym  = s.id
        LEFT JOIN pairs p ON p.{rva_col} = fn.rva
        WHERE u.name = ?
        ORDER BY (fn.line IS NULL OR fn.line = 0), fn.line, fn.rva
    """, (unit,)).fetchall()
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


def cmd_list(con, args):
    q = "SELECT name, module FROM units"
    conds, params = [], []
    if args.module:
        conds.append("module = ?")
        params.append(args.module)
    if args.pattern:
        conds.append("name LIKE ?")
        params.append(f"%{args.pattern}%")
    if conds:
        q += " WHERE " + " AND ".join(conds)
    q += " ORDER BY module, name"
    for r in con.execute(q, params):
        print(f"{r['module']:<14} {r['name']}")


def cmd_show(con, args):
    unit = resolve_unit(con, args.tu)
    t_rows = fetch_side(con, unit, "target")
    b_rows = fetch_side(con, unit, "base")
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

    con = open_db()
    if args.list:
        # when --list is given, a positional tu acts as the substring filter
        if args.tu and not args.pattern:
            args.pattern = args.tu
        cmd_list(con, args)
        return
    if not args.tu:
        ap.error("a TU is required (or pass --list)")
    sys.exit(cmd_show(con, args))


if __name__ == "__main__":
    main()
