#!/usr/bin/env python3
"""Measure uniquely-owned functions whose COFF body moved between units.

The linker can select a COMDAT body from different translation units in the
retail and reconstructed executables.  The normal objdiff project compares one
target object with its same-path base object, so such a function is present on
both sides but receives no score.  This helper creates a disposable project for
the unique cross-unit object pairs and writes only the recovered function
scores.  It never changes the main report's unit totals.
"""

from __future__ import annotations

import json
import shutil
import subprocess
import tempfile
from collections import defaultdict
from pathlib import Path


def _defined_owners(root: Path, names: set[str], nm: str) -> dict[str, list[Path]]:
    owners: dict[str, list[Path]] = defaultdict(list)
    objects = sorted(root.rglob("*.obj"))
    for start in range(0, len(objects), 256):
        batch = objects[start : start + 256]
        result = subprocess.run(
            [nm, "-A", "--defined-only", *map(str, batch)],
            check=True,
            capture_output=True,
            text=True,
        )
        for line in result.stdout.splitlines():
            try:
                path_text, symbol_record = line.split(": ", 1)
                name = symbol_record.split(None, 2)[2]
            except (IndexError, ValueError):
                continue
            if name in names:
                owners[name].append(Path(path_text).resolve())
    return owners


def generate(objdiff_dir: Path, *, objdiff_cli: str = "objdiff-cli") -> tuple[int, int]:
    """Write report-cross-unit.json; return (candidate, scored) counts."""
    objdiff_dir = objdiff_dir.resolve()
    report_path = objdiff_dir / "report.json"
    output_path = objdiff_dir / "report-cross-unit.json"
    report = json.loads(report_path.read_text())

    missing: set[str] = {
        function["name"]
        for unit in report["units"]
        for function in unit["functions"]
        if function.get("fuzzy_match_percent") is None
    }
    nm = shutil.which("llvm-nm")
    if not nm:
        raise RuntimeError("llvm-nm is required for cross-unit COMDAT matching")

    target_owners = _defined_owners(objdiff_dir / "target", missing, nm)
    base_owners = _defined_owners(objdiff_dir / "base", missing, nm)
    candidates: dict[tuple[Path, Path], set[str]] = defaultdict(set)
    for name in sorted(missing):
        target_paths = target_owners.get(name, [])
        base_paths = base_owners.get(name, [])
        if len(target_paths) != 1 or len(base_paths) != 1:
            continue
        target_path, base_path = target_paths[0], base_paths[0]
        direct_base = objdiff_dir / "base" / target_path.relative_to(
            objdiff_dir / "target"
        )
        if base_path == direct_base.resolve():
            continue
        candidates[(target_path, base_path)].add(name)

    if not candidates:
        output_path.write_text('{"version":1,"functions":[]}\n')
        return 0, 0

    ordered = sorted(candidates.items(), key=lambda item: tuple(map(str, item[0])))
    units = []
    expected: dict[str, set[str]] = {}
    for index, ((target_path, base_path), names) in enumerate(ordered):
        unit_name = f"cross-unit/{index:04d}"
        units.append(
            {
                "name": unit_name,
                "target_path": str(target_path),
                "base_path": str(base_path),
                "scratch": {"platform": "win32", "compiler": "msvc8.0"},
            }
        )
        expected[unit_name] = names

    with tempfile.TemporaryDirectory(prefix="objdiff-cross-", dir=objdiff_dir) as temp:
        project = Path(temp)
        (project / "objdiff.json").write_text(
            json.dumps(
                {"build_base": False, "build_target": False, "units": units},
                indent=2,
            )
            + "\n"
        )
        raw_report = project / "report.json"
        subprocess.run(
            [objdiff_cli, "report", "generate", "-p", str(project), "-o", str(raw_report)],
            check=True,
        )
        cross_report = json.loads(raw_report.read_text())

    scores: dict[str, float] = {}
    for unit in cross_report["units"]:
        wanted = expected[unit["name"]]
        for function in unit["functions"]:
            name = function["name"]
            fuzzy = function.get("fuzzy_match_percent")
            if name in wanted and fuzzy is not None:
                scores[name] = max(scores.get(name, 0.0), fuzzy)

    rows = [
        {"name": name, "fuzzy_match_percent": scores[name]}
        for name in sorted(scores)
    ]
    output_path.write_text(json.dumps({"version": 1, "functions": rows}, separators=(",", ":")) + "\n")
    return sum(map(len, candidates.values())), len(rows)
