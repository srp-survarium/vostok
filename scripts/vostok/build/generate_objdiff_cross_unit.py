#!/usr/bin/env python3
"""Recover strict scores hidden by COFF/PDB attribution.

The linker can select a COMDAT body from different translation units in the
retail and reconstructed executables.  The normal objdiff project compares one
target object with its same-path base object, so such a function is present on
both sides but receives no score.  This helper creates a disposable project for
the unique cross-unit object pairs and writes only the recovered function
scores.  It also records same-path source objects that are byte-identical as
strict evidence for functions whose PDB names do not pair.  It never changes
the main report's unit totals.
"""

from __future__ import annotations

import filecmp
import json
import shutil
import subprocess
import tempfile
from collections import defaultdict
from pathlib import Path

from vostok.core import tsv
from vostok.core.paths import EFFECTIVE_SYMBOL_MAP, MEASURED_FOLD_ALIASES


def _load_pairs(path: Path) -> dict[str, str]:
    """Load reviewed target-to-base-PDB identities."""
    pairs: dict[str, str] = {}
    for line, fields in tsv.read(path):
        if len(fields) != 2 or not all(fields):
            raise RuntimeError(f"{path}:{line}: expected target<TAB>base symbol")
        target, base = fields
        if target in pairs:
            raise RuntimeError(f"{path}:{line}: duplicate target symbol {target}")
        pairs[target] = base
    return pairs


def _load_symbol_choices(path: Path) -> dict[str, str]:
    """Load the delinker's generated PDB-to-COFF symbol choices."""
    choices: dict[str, str] = {}
    for line, fields in tsv.read(path):
        if len(fields) != 2 or not all(fields):
            raise RuntimeError(f"{path}:{line}: expected PDB<TAB>COFF symbol")
        pdb_name, coff_name = fields
        prior = choices.get(pdb_name)
        if prior is not None and prior != coff_name:
            raise RuntimeError(f"{path}:{line}: conflicting choice for {pdb_name}")
        choices[pdb_name] = coff_name
    return choices


def _resolve_reviewed_aliases(
    pairs: dict[str, str], choices: dict[str, str]
) -> dict[str, str]:
    """Resolve reviewed base PDB names to symbols present in delinked COFF."""
    return {target: choices.get(base, base) for target, base in pairs.items()}


def _identical_units(report: dict, objdiff_dir: Path) -> list[str]:
    """Return source units whose complete delinked objects are exact."""
    units = []
    for unit in report["units"]:
        name = unit["name"]
        target = objdiff_dir / "target" / f"{name}.obj"
        base = objdiff_dir / "base" / f"{name}.obj"
        if (
            target.is_file()
            and base.is_file()
            and target.stat().st_size == base.stat().st_size
            and filecmp.cmp(target, base, shallow=False)
        ):
            units.append(name)
    return units


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
            if name in names and Path(path_text).resolve() not in owners[name]:
                owners[name].append(Path(path_text).resolve())
    return owners


def generate(
    objdiff_dir: Path, *, objdiff_cli: str = "objdiff-cli"
) -> tuple[int, int, int]:
    """Write strict supplementary evidence and return its three counts."""
    objdiff_dir = objdiff_dir.resolve()
    report_path = objdiff_dir / "report.json"
    output_path = objdiff_dir / "report-cross-unit.json"
    report = json.loads(report_path.read_text())
    exact_units = _identical_units(report, objdiff_dir)

    missing: set[str] = {
        function["name"]
        for unit in report["units"]
        for function in unit["functions"]
        if function.get("fuzzy_match_percent") is None
    }
    nm = shutil.which("llvm-nm")
    if not nm:
        raise RuntimeError("llvm-nm is required for cross-unit COMDAT matching")

    reviewed = _resolve_reviewed_aliases(
        _load_pairs(MEASURED_FOLD_ALIASES),
        _load_symbol_choices(EFFECTIVE_SYMBOL_MAP),
    )
    target_owners = _defined_owners(objdiff_dir / "target", missing, nm)
    base_owners = _defined_owners(
        objdiff_dir / "base", missing | set(reviewed.values()), nm
    )
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

    reviewed_jobs: list[tuple[str, str, Path, Path]] = []
    for target, base_coff in sorted(reviewed.items()):
        if target not in missing:
            continue
        target_paths = target_owners.get(target, [])
        base_paths = base_owners.get(base_coff, [])
        if base_owners.get(target):
            raise RuntimeError(
                f"reviewed fold alias {target} already has a same-name base owner"
            )
        if len(target_paths) != 1 or len(base_paths) != 1:
            raise RuntimeError(
                "reviewed fold alias must have one target and one base owner: "
                f"{target} -> {base_coff} "
                f"({len(target_paths)} target, {len(base_paths)} base)"
            )
        reviewed_jobs.append((target, base_coff, target_paths[0], base_paths[0]))

    candidate_count = sum(map(len, candidates.values())) + len(reviewed_jobs)
    if not candidate_count:
        output_path.write_text(
            json.dumps(
                {"version": 3, "functions": [], "exact_units": exact_units},
                separators=(",", ":"),
            )
            + "\n"
        )
        return 0, 0, len(exact_units)

    ordered = sorted(candidates.items(), key=lambda item: tuple(map(str, item[0])))
    with tempfile.TemporaryDirectory(prefix="objdiff-cross-", dir=objdiff_dir) as temp:
        project = Path(temp)
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

        objcopy = shutil.which("llvm-objcopy")
        if reviewed_jobs and not objcopy:
            raise RuntimeError(
                "llvm-objcopy is required for reviewed folded-COMDAT matching"
            )
        for index, (target, base_coff, target_path, base_path) in enumerate(
            reviewed_jobs
        ):
            base_copy = project / f"reviewed-base-{index:04d}.obj"
            shutil.copy2(base_path, base_copy)
            subprocess.run(
                [objcopy, "--redefine-sym", f"{base_coff}={target}", str(base_copy)],
                check=True,
            )
            unit_name = f"reviewed-fold/{index:04d}"
            units.append(
                {
                    "name": unit_name,
                    "target_path": str(target_path),
                    "base_path": str(base_copy),
                    "scratch": {"platform": "win32", "compiler": "msvc8.0"},
                }
            )
            expected[unit_name] = {target}

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
    output_path.write_text(
        json.dumps(
            {"version": 3, "functions": rows, "exact_units": exact_units},
            separators=(",", ":"),
        )
        + "\n"
    )
    return candidate_count, len(rows), len(exact_units)
