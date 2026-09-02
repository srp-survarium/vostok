#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""
vostok.diff.enums_compare - compare enum definitions between the BASE and TARGET
pdb-parser structure dumps.

The pdb-parser (see vostok.build.generate_structure) emits, for each enum, a block
of the form

    enum survarium::affect_event_type_enum
    {
        affect_applying     = 0x0,
        affect_recalling    = 0x1,
        affect_canceling    = 0x2,
    };

inside a `namespace survarium { ... }` (or, for class-nested enums, with a
class-qualified name like `survarium::game::debug_window_enum`). Forward
declarations end in `;` on the `enum ...` line and carry no body - they are
skipped.

This script parses every such block from both dumps and reports:

  (a) MISSING    - enums present in TARGET but absent from BASE.
  (b) MISMATCHED - enums present in BOTH whose enumerator names or values differ.

For each mismatch it prints a per-enumerator diff line:

    enum_name::ENUMERATOR  base=<v>  target=<v>

TARGET is the original game PDB (ground truth); BASE is our compiled source.

By default it reports across ALL namespaces. Restrict to one or more qualified
prefixes with --prefix (repeatable). Convenience presets:

  --game-core   -> survarium:: and vostok::  (game_core sources live under both;
                   there is no vostok::game_core:: namespace)
  --network     -> vostok::network_core:: and vostok::network::

CRITICAL: the parser emits each enum in several forms across different headers -
a fully-populated body, an empty `{ }` name-only reference, and (in the BASE PDB
only) occasionally a STALE older copy with fewer enumerators. parse_enums()
collapses to the MOST-POPULATED body per qualified name before comparing, so the
empty/stale forms do not produce bogus mismatches.

Usage:
  python3 -m vostok diff enums-compare
  python3 -m vostok diff enums-compare --prefix vostok::network_core:: --prefix vostok::network::
  python3 -m vostok diff enums-compare --network
  python3 -m vostok diff enums-compare --base DIR --target DIR
"""

import argparse
import re
import sys
from pathlib import Path

from vostok.core.paths import STRUCTURE_DIR

# `enum NS::foo` or `enum NS::cls::foo` opening a definition. A trailing `;`
# means a forward declaration (no body) - excluded by the regex.
ENUM_OPEN_RE = re.compile(r"^enum\s+([\w:]+::[\w:]+)\s*$")
# An enumerator line inside the body: `name = 0x1,` (value may be negative hex).
ENUMERATOR_RE = re.compile(r"^\s*(\w+)\s*=\s*(-?0[xX][0-9a-fA-F]+|-?\d+)\s*,?\s*$")


def _parse_value(text: str) -> int:
    text = text.strip()
    neg = text.startswith("-")
    if neg:
        text = text[1:]
    base = 16 if text.lower().startswith("0x") else 10
    val = int(text, base)
    return -val if neg else val


def parse_enums(headers_dir: Path) -> dict[str, list[tuple[str, int]]]:
    """Return {qualified_enum_name: [(enumerator_name, value), ...]} for every
    enum DEFINITION (with a body) found under headers_dir.

    A name may appear in many headers (the parser duplicates declarations across
    the files that need them). The real bodies are identical, but the parser also
    emits an EMPTY `enum X { };` form in headers that only need the type by name,
    and (base only) a STALE older copy via a compiland that includes the real
    header; so for a given enum we keep the definition with the MOST enumerators
    (the most-populated body), not merely the first one seen.
    """
    enums: dict[str, list[tuple[str, int]]] = {}

    for path in sorted(headers_dir.rglob("*.h")):
        try:
            lines = path.read_text(errors="replace").splitlines()
        except OSError:
            continue

        i = 0
        n = len(lines)
        while i < n:
            m = ENUM_OPEN_RE.match(lines[i])
            if not m:
                i += 1
                continue
            name = m.group(1)
            # The body opens with a `{` line and closes with a `};` line.
            j = i + 1
            if j >= n or lines[j].strip() != "{":
                i += 1
                continue
            members: list[tuple[str, int]] = []
            j += 1
            while j < n and lines[j].strip() not in ("};", "}"):
                em = ENUMERATOR_RE.match(lines[j])
                if em:
                    members.append((em.group(1), _parse_value(em.group(2))))
                j += 1
            # Keep the most-populated body (empty `{ }` forms are name-only refs).
            if name not in enums or len(members) > len(enums[name]):
                enums[name] = members
            i = j + 1

    return enums


def _match_prefix(name: str, prefixes: list[str] | None) -> bool:
    if not prefixes:
        return True
    return any(name.startswith(p) for p in prefixes)


def diff_enums(
    base: dict[str, list[tuple[str, int]]],
    target: dict[str, list[tuple[str, int]]],
    prefixes: list[str] | None = None,
) -> tuple[list[str], list[tuple[str, list[str]]]]:
    """Return (missing, mismatched), restricted to qualified names matching one
    of `prefixes` (all names when prefixes is None/empty).

    missing    - sorted target-only enum names.
    mismatched - [(enum_name, [per-enumerator diff lines]), ...] for enums in
                 both whose member name/value sequence differs.
    """
    missing = sorted(
        name for name in target if name not in base and _match_prefix(name, prefixes)
    )

    mismatched: list[tuple[str, list[str]]] = []
    for name in sorted(target):
        if name not in base or not _match_prefix(name, prefixes):
            continue
        b_members = base[name]
        t_members = target[name]
        if b_members == t_members:
            continue

        b_map = dict(b_members)
        t_map = dict(t_members)
        all_names = list(
            dict.fromkeys([m for m, _ in t_members] + [m for m, _ in b_members])
        )

        diff_lines: list[str] = []
        for em in all_names:
            in_b = em in b_map
            in_t = em in t_map
            if in_b and in_t:
                if b_map[em] != t_map[em]:
                    diff_lines.append(
                        f"  {name}::{em}  base={_fmt(b_map[em])}  target={_fmt(t_map[em])}"
                    )
            elif in_t:
                diff_lines.append(
                    f"  {name}::{em}  base=<absent>  target={_fmt(t_map[em])}"
                )
            else:
                diff_lines.append(
                    f"  {name}::{em}  base={_fmt(b_map[em])}  target=<absent>"
                )
        if diff_lines:
            mismatched.append((name, diff_lines))

    return missing, mismatched


def _fmt(v: int) -> str:
    return f"-0x{-v:x}" if v < 0 else f"0x{v:x}"


def main() -> int:
    ap = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter
    )
    ap.add_argument("--base", type=Path, default=STRUCTURE_DIR / "base" / "headers")
    ap.add_argument("--target", type=Path, default=STRUCTURE_DIR / "target" / "headers")
    ap.add_argument(
        "--prefix",
        action="append",
        default=[],
        metavar="QUALIFIED::",
        help="restrict to enums whose qualified name starts with this prefix "
        "(repeatable); default is all namespaces",
    )
    ap.add_argument(
        "--game-core",
        action="store_true",
        help="preset: survarium:: + vostok:: (game_core sources span both)",
    )
    ap.add_argument(
        "--network",
        action="store_true",
        help="preset: vostok::network_core:: + vostok::network::",
    )
    args = ap.parse_args()

    prefixes = list(args.prefix)
    if args.game_core:
        prefixes += ["survarium::", "vostok::"]
    if args.network:
        prefixes += ["vostok::network_core::", "vostok::network::"]

    for side, d in (("base", args.base), ("target", args.target)):
        if not d.is_dir():
            print(
                f"ERROR: {side} headers dir not found: {d}\n"
                f"  run: python3 -m vostok.build.generate_structure {side}",
                file=sys.stderr,
            )
            return 1

    base = parse_enums(args.base)
    target = parse_enums(args.target)

    missing, mismatched = diff_enums(base, target, prefixes)

    scope = ", ".join(prefixes) if prefixes else "(all namespaces)"
    print(
        f"enum compare - scope: {scope}\n"
        f"  total enums parsed: base={len(base)} target={len(target)}\n"
    )

    print(f"=== MISMATCHED ({len(mismatched)}) ===")
    if not mismatched:
        print("(none)")
    for name, diff_lines in mismatched:
        print(f"enum {name}")
        for line in diff_lines:
            print(line)
    print()

    print(f"=== MISSING (target-only) ({len(missing)}) ===")
    if not missing:
        print("(none)")
    for name in missing:
        members = target[name]
        print(f"enum {name}  ({len(members)} enumerators)")

    return 0


if __name__ == "__main__":
    sys.exit(main())
