# SPDX-License-Identifier: GPL-3.0-or-later

"""vostok.tool.geometry - one function's PDB line geometry, retail beside ours.

Each statement record carries a source line, so the relative lines between
records expose where retail had blank lines, split braces, labels and
statements that compiled to zero bytes. This view anchors both sides on the
function's first record and prints our source line at every relative line,
flagging lines where only one side has a record. Known gaps that no source
change can reproduce (#ifdef'd retail lines, unrecoverable comments) are listed
with reasons in config/geometry_ignores.tsv; they are applied, labelled and not
counted:

    python3 -m vostok tool geometry 'find_srgb_format'
    python3 -m vostok tool geometry --rva 0x550560      # pick one of several matches

Exit 0 when every record lands on the same relative line (GEOMETRY MATCH),
1 when they diverge, 2 when the function cannot be selected.
"""

from __future__ import annotations

import argparse
import json
import sqlite3
import sys
from collections import defaultdict
from pathlib import Path

from vostok.core import log as _log
from vostok.core import tsv
from vostok.core.paths import BASE_EVIDENCE, GEOMETRY_IGNORES, SOURCES, TARGET_EVIDENCE


def _records(path: Path, where: str, arg) -> list[dict]:
    if not path.is_file():
        sys.exit(f"geometry: {path} is missing - run `python3 -m vostok build` first")
    connection = sqlite3.connect(f"file:{path}?mode=ro", uri=True)
    connection.row_factory = sqlite3.Row
    rows = connection.execute(
        "SELECT f.name,f.mangled,f.rva,f.file,p.statements_json FROM functions f "
        f"JOIN function_payloads p ON p.function_id=f.id WHERE {where}", arg
    ).fetchall()
    connection.close()
    return [dict(row) | {"statements": json.loads(row["statements_json"])} for row in rows]


def _select(name: str | None, rva: int | None) -> tuple[dict, dict]:
    if rva is not None:
        targets = _records(TARGET_EVIDENCE, "f.rva=?", (rva,))
    else:
        targets = _records(TARGET_EVIDENCE, "f.mangled=?", (name,)) or _records(
            TARGET_EVIDENCE, "f.name LIKE ?", (f"%{name}%",))
    if len(targets) != 1:
        if not targets:
            sys.exit(f"geometry: no retail function matches {name or hex(rva)}")
        for t in targets:
            print(f"  --rva {t['rva']:#x}  {t['file']}  {t['name']}", file=sys.stderr)
        sys.exit(2)
    target = targets[0]
    bases = _records(BASE_EVIDENCE, "f.mangled=?", (target["mangled"],))
    if len(bases) > 1:
        bases = [b for b in bases if b["file"] == target["file"]] or bases
    if not bases:
        sys.exit(f"geometry: our build has no {target['name']}")
    return target, bases[0]


def _relative(function: dict) -> tuple[int, dict[int, list[int]], dict[str, int]]:
    """Anchor line, {relative line: [sizes]} in the function's file, other files."""
    own = [s for s in function["statements"] if s.get("file", function["file"]) == function["file"]]
    other: dict[str, int] = defaultdict(int)
    for s in function["statements"]:
        if s.get("file", function["file"]) != function["file"]:
            other[s["file"]] += 1
    if not own:
        return 0, {}, dict(other)
    anchor = min(own, key=lambda s: s["off"])["line"]
    lines: dict[int, list[int]] = defaultdict(list)
    for s in sorted(own, key=lambda s: s["off"]):
        lines[s["line"] - anchor].append(s["size"])
    return anchor, dict(lines), dict(other)


def load_ignores(path: Path, mangled: str) -> list[tuple[int, int, str]]:
    """This function's (at, lines, reason) gaps, sorted by `at`."""
    gaps = []
    if not Path(path).is_file():
        return gaps
    for number, fields in tsv.read(path):
        where = f"geometry: {path}:{number}"
        if len(fields) != 4:
            sys.exit(f"{where}: expected function, at, lines, reason")
        function, at, lines, reason = fields
        try:
            at, lines = int(at), int(lines)
        except ValueError:
            sys.exit(f"{where}: at and lines must be integers")
        if lines == 0 or not reason.strip():
            sys.exit(f"{where}: a gap needs non-zero lines and a reason")
        if function == mangled:
            gaps.append((at, lines, reason.strip()))
    return sorted(gaps)


def to_frame(lines: dict[int, list[int]], gaps: list[tuple[int, int, str]], span: range):
    """Map our relative lines into retail's frame through the known gaps.

    Returns ({frame line: sizes}, {frame line: our line}, {frame line: gap reason},
    [(our line, reason, sizes) skipped by a negative gap]).
    """
    framed, ours_at, reasons, skipped = {}, {}, {}, []
    for r in span:
        shift, skip = 0, None
        for at, n, reason in gaps:
            if n > 0 and r >= at:
                shift += n
            elif n < 0 and at <= r < at - n:
                skip = reason
            elif n < 0 and r >= at - n:
                shift += n
        if skip is not None:
            if r in lines:
                skipped.append((r, skip, lines[r]))
            continue
        ours_at[r + shift] = r
        if r in lines:
            framed[r + shift] = lines[r]
    for at, n, reason in gaps:
        if n > 0:
            start = at + sum(m for a, m, _ in gaps if a < at)
            for frame in range(start, start + n):
                reasons[frame] = reason
    return framed, ours_at, reasons, skipped


# Line flags are geometry divergences; "bytes" only marks the next step's work.
DIVERGENT = ("retail only", "ours only", "records")


def geometry_rows(target: dict[int, list[int]], base: dict[int, list[int]]) -> list[tuple]:
    """(relative line, retail sizes, our sizes, flag) for every line either side spans."""
    rows = []
    span = list(target) + list(base)
    for rel in range(min(span), max(span) + 1) if span else ():
        t, b = target.get(rel, []), base.get(rel, [])
        flag = ("retail only" if t and not b else "ours only" if b and not t
                else "records" if len(t) != len(b) else "bytes" if t != b else "")
        rows.append((rel, t, b, flag))
    return rows


def _sizes(sizes: list[int]) -> str:
    return "+".join(f"{s}B" for s in sizes) if sizes else "."


def overlay(target: dict, base: dict) -> dict:
    """Retail's records beside ours in retail's relative-line frame.

    Returns the rows (`geometry_rows`, gaps applied), the known-gap reasons and
    the map from a frame line back to our source line, so a caller can print
    the overlay or reduce it to a verdict without re-deriving either.
    """
    _, t_lines, t_other = _relative(target)
    b_anchor, b_lines, b_other = _relative(base)
    gaps = load_ignores(GEOMETRY_IGNORES, base["mangled"])
    span = range(min(b_lines, default=0), max(b_lines, default=0) + 1)
    framed, ours_at, reasons, skipped = to_frame(b_lines, gaps, span)
    rows = geometry_rows(t_lines, framed)
    divergent = [(rel, flag) for rel, _, _, flag in rows if flag in DIVERGENT]
    divergent += [(r, "record in a known gap") for r, _, _ in skipped]
    return {"anchor": b_anchor, "rows": rows, "ours_at": ours_at, "reasons": reasons,
            "skipped": skipped, "gaps": gaps, "other": {"retail": t_other, "ours": b_other},
            "records": sum(len(v) for v in t_lines.values()), "divergent": divergent,
            "bytes": sum(1 for row in rows if row[3] == "bytes")}


def our_line(view: dict, rel: int) -> int | None:
    """Our absolute source line at a frame line, or None inside a known gap."""
    return view["anchor"] + view["ours_at"][rel] if rel in view["ours_at"] else None


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    parser.add_argument("function", nargs="?", help="mangled name or demangled substring")
    parser.add_argument("--rva", type=lambda v: int(v, 0), help="retail rva, to pick one match")
    args = parser.parse_args()
    if not args.function and args.rva is None:
        parser.error("give a function or --rva")

    target, base = _select(args.function, args.rva)
    view = overlay(target, base)
    reasons = view["reasons"]
    source_path = SOURCES / base["file"]
    source = source_path.read_text(errors="replace").splitlines() if source_path.is_file() else []
    stale = source_path.is_file() and source_path.stat().st_mtime > BASE_EVIDENCE.stat().st_mtime

    print(f"{target['name']}\n  retail {target['file']}  ours {base['file']} "
          f"(line {view['anchor']} = +0)")
    if view["gaps"]:
        print(f"  {len(view['gaps'])} known gap(s) from {GEOMETRY_IGNORES.name} applied")
    if stale:
        print("  note: the source is newer than our build's evidence - rebuild for an exact overlay")
    print(f"\n{'rel':>5} | {'retail':>10} | {'ours':>10} | source")
    for rel, t, b, flag in view["rows"]:
        if rel in reasons and not t:
            text, flag = f"(known gap: {reasons[rel]})", ""
        else:
            number = our_line(view, rel)
            text = source[number - 1].rstrip() if number and 0 < number <= len(source) else ""
        marker = f"   <- {flag}" if flag else ""
        print(f"{rel:>+5} | {_sizes(t):>10} | {_sizes(b):>10} | {text}{marker}")
    for side, other in view["other"].items():
        for file, count in sorted(other.items()):
            print(f"  {side}: {count} record(s) attributed to {file}")
    for r, reason, sizes in view["skipped"]:
        print(f"  ours +{r}: {_sizes(sizes)} sits inside the known gap '{reason}'")

    divergent, differing = view["divergent"], view["bytes"]
    if not divergent:
        print(f"\nGEOMETRY MATCH ({view['records']} records"
              + (f"; {differing} line(s) differ in bytes" if differing else "") + ")")
        return 0
    rel, flag = divergent[0]
    print(f"\nfirst divergence at {rel:+d} ({flag}); {len(divergent)} divergent line(s)")
    return 1

if __name__ == "__main__":
    raise SystemExit(_log.run("vostok.tool.geometry", main))
