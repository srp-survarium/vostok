# SPDX-License-Identifier: GPL-3.0-or-later

"""
joined.py - list source lines that carry more than one statement.

A retail PDB line record that spans what we spell as two statements is evidence
about the original shape (an inlined helper, a macro, a different expression),
never a licence to glue statements onto one line: real code is not written
that way, and a joined line reproduces the record count for the wrong reason.
This scanner finds every such line in the engine sources so they can be
unwound and the real divergence investigated.

    python3 -m vostok tool joined                   # summary per module
    python3 -m vostok tool joined --list            # every line, TSV
    python3 -m vostok tool joined --module game_core --blame
                                                    # with the commit date of each line
    python3 -m vostok tool joined --since 2026-06-01 --blame
                                                    # only lines touched after that date

Scans sources/vostok; vendored subtrees (bullet, stlport, ...) and the tool programs
(editor, maya, tests) are skipped, `--all` includes them.
"""

from __future__ import annotations

import argparse
import os
import re
import subprocess
from collections import Counter, defaultdict
from pathlib import Path

from vostok.core import log as _log
from vostok.core.paths import BASE_IDX, REPO, SOURCES, TARGET_IDX

# Subtrees whose joined lines are not ours to unwind: third-party code kept in
# tree, and tool programs that are not binary-matched.
SKIPPED_SUBTREES = (
    "bullet", "ode", "opcode", "ogg", "vorbis", "vorbisfile", "freeimage", "rtp",
    "stlport", "boost", "editor", "maya", "maya_animation", "test", "test_animations",
    "graphics_benchmark", "finger_printer", "sample_rate_test", "shader_generator",
    "login_server", "client", "core/sources/compressor_ppmd", "core/sources/ptmalloc3",
)
ENGINE = SOURCES / "vostok"

_STRING = re.compile(r'"(?:[^"\\]|\\.)*"')
_CHAR = re.compile(r"'(?:[^'\\]|\\.)'")
_LINE_COMMENT = re.compile(r"//.*$")
_BLOCK_COMMENT = re.compile(r"/\*.*?\*/")
_CASE_ONE_LINER = re.compile(r"^(case\b[^:]*|default)\s*:[^;]*;\s*(break|return[^;]*);\s*}?$")
_DECL_START = re.compile(
    r"^(typedef|using|enum|class|struct|union|template|namespace|friend|public|"
    r"private|protected|extern|static_assert|STATIC_SIZE_ASSERT|COMPILE_ASSERT)\b"
)
# a ';' followed by more code on the same line (not just closing brackets)
_JOINED = re.compile(r";\s*(?![\s\}\)]*$)\S")


def is_joined(line: str) -> bool:
    """True when `line` holds a statement terminator followed by more code."""
    s = line.strip()
    if not s or s.startswith(("//", "#", "*", "/*")):
        return False
    if s.startswith("for") or re.search(r"\bfor\s*\(", s):
        return False  # for-headers legitimately carry two semicolons
    code = _LINE_COMMENT.sub("", _BLOCK_COMMENT.sub("", _CHAR.sub("''", _STRING.sub('""', s))))
    if _DECL_START.match(code):
        return False
    if code.endswith("\\"):
        return False  # macro continuation lines are one logical line
    if _CASE_ONE_LINER.match(code):
        return False  # `case x: stmt; break;` is an idiom, not a join
    if code.count(";") == 2 and code.count(")") > code.count("("):
        return False  # the middle line of a wrapped for-header
    return bool(_JOINED.search(code))


def scan(root: Path, include_all: bool = False):
    """Yield (path, line_number, text) for every joined line under `root`."""
    for dirpath, dirnames, filenames in os.walk(root):
        rel = Path(dirpath).relative_to(root).as_posix()
        if not include_all and any(rel == s or rel.startswith(s + "/") for s in SKIPPED_SUBTREES):
            dirnames[:] = []
            continue
        for name in sorted(filenames):
            if not name.endswith((".cpp", ".h", ".inl")):
                continue
            stem = f"{rel}/{name}" if rel != "." else name
            if not include_all and any(stem.startswith(s) for s in SKIPPED_SUBTREES):
                continue  # a single vendored file (compressor_ppmd.cpp, ptmalloc3.c)
            path = Path(dirpath) / name
            try:
                text = path.read_text(encoding="utf-8", errors="replace")
            except OSError:
                continue
            in_comment = False
            for number, line in enumerate(text.split("\n"), 1):
                code = line
                if in_comment:
                    end = code.find("*/")
                    if end < 0:
                        continue
                    code = code[end + 2:]
                    in_comment = False
                if is_joined(code):
                    yield path, number, line.strip()
                stripped = _LINE_COMMENT.sub("", _CHAR.sub("''", _STRING.sub('""', code)))
                if stripped.rfind("/*") > stripped.rfind("*/"):
                    in_comment = True


def verdicts(rows):
    """Map each (rel_path, line) to what the retail record says about the join.

    `retail-one-liner`: the base and retail functions have the same statement count -
    the joined line is retail's own shape (a one-line header body). `retail-split`:
    retail records more statements - unwind it. `no-record`: no base statement starts
    on that line (inlined everywhere), `no-target`: base-only function.
    """
    by_loc = {}
    import json  # local: the indexes are large, keep the import cost off the plain scan

    def load(path):
        out = {}
        for raw in path.read_text(errors="replace").split("\n"):
            if raw:
                rec = json.loads(raw)
                out.setdefault(rec["name"], rec)
        return out

    target = load(TARGET_IDX) if TARGET_IDX.exists() else {}
    base = load(BASE_IDX) if BASE_IDX.exists() else {}
    for name, rec in base.items():
        for stmt in rec.get("statements", []):
            by_loc.setdefault((rec.get("file") or "", stmt["line"]), []).append(name)
    result = {}
    for rel, number in rows:
        names = by_loc.get((rel.removeprefix("sources/"), number), [])
        if not names:
            result[(rel, number)] = "no-record"
            continue
        verdict = "no-target"
        for name in names:
            trec = target.get(name)
            if trec is None:
                continue
            if len(trec["statements"]) > len(base[name]["statements"]):
                verdict = "retail-split"
                break
            verdict = "retail-one-liner"
        result[(rel, number)] = verdict
    return result


def blame_dates(path: Path) -> list[str]:
    """Commit date (YYYY-MM-DD) per line of `path`, or [] when git is unavailable."""
    try:
        out = subprocess.run(
            ["git", "blame", "-l", "--date=short", str(path)],
            capture_output=True, text=True, errors="replace", cwd=REPO, check=False,
        ).stdout
    except OSError:
        return []
    dates = []
    for row in out.split("\n"):
        m = re.search(r"(\d{4}-\d{2}-\d{2})", row)
        dates.append(m.group(1) if m else "?")
    return dates


def module_of(path: Path, root: Path) -> str:
    parts = path.relative_to(root).parts
    return parts[0] if len(parts) > 1 else "."


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.split("\n\n")[1])
    parser.add_argument("--list", action="store_true", help="print every line as TSV")
    parser.add_argument("--module", help="restrict to one module directory (e.g. game_core)")
    parser.add_argument("--blame", action="store_true", help="add the git commit date of each line")
    parser.add_argument("--since", metavar="YYYY-MM-DD", help="only lines committed on/after this date (implies --blame)")
    parser.add_argument("--all", action="store_true", help="scan vendored subtrees and tool programs too")
    parser.add_argument("--verdict", action="store_true",
                        help="look each line up in the rich indexes: retail-one-liner / retail-split / no-record")
    parser.add_argument("root", nargs="?", type=Path, default=ENGINE)
    args = parser.parse_args()
    root = args.root.resolve()
    want_blame = args.blame or bool(args.since)

    hits = defaultdict(list)
    for path, number, text in scan(root, args.all):
        module = module_of(path, root)
        if args.module and module != args.module:
            continue
        hits[path].append((number, text))

    rows = []
    for path, items in hits.items():
        dates = blame_dates(path) if want_blame else []
        for number, text in items:
            date = dates[number - 1] if number - 1 < len(dates) else "?"
            if args.since and date != "?" and date < args.since:
                continue
            rows.append((module_of(path, root), path.relative_to(REPO).as_posix(), number, date, text))

    said = verdicts([(rel, number) for _, rel, number, _, _ in rows]) if args.verdict else {}

    if args.list:
        for module, rel, number, date, text in sorted(rows):
            stamp = f"\t{date}" if want_blame else ""
            tag = f"\t{said[(rel, number)]}" if args.verdict else ""
            print(f"{rel}\t{number}{stamp}{tag}\t{text}")
        return 0

    per_module = Counter(module for module, *_ in rows)
    print(f"{len(rows)} joined lines" + (f" since {args.since}" if args.since else ""))
    for module, count in sorted(per_module.items(), key=lambda kv: -kv[1]):
        print(f"  {module:22s} {count}")
    if want_blame:
        per_month = Counter(date[:7] for *_, date, _ in rows)
        print("by month: " + ", ".join(f"{m} {c}" for m, c in sorted(per_month.items())))
    if args.verdict:
        per_verdict = Counter(said.values())
        print("verdicts: " + ", ".join(f"{v} {c}" for v, c in sorted(per_verdict.items())))
    return 0


if __name__ == "__main__":
    raise SystemExit(_log.run("vostok.tool.joined", main))
