"""vostok data - image-level matching for globals, constants, BSS, and pointers.

Commands:
  init-target     export the immutable retail PDB/image inventory and access map
  refresh         export base, compare both images, and regenerate data_state.tsv
  report          summarize exactness and divergence classes
  symbol PATTERN  inspect matching ledger rows
  access PATTERN  find retail/base code references to a data symbol or address
  relocs PATTERN  inspect pointer cells and their resolved targets
  coverage        show gross/reconstructable coverage, fidelity, and largest gaps
  check [--gate]  validate the data artifacts; optionally enforce calibrated floors

This lane is independent of objdiff/report.json and match_state.tsv.  Its EXACT
means relocation-normalized bytes and resolved pointer targets both agree.
"""

from __future__ import annotations

import argparse
import csv
import json
import subprocess
import sys
from collections import Counter
from pathlib import Path

from vostok.core import paths
from vostok.data import pipeline
from vostok.data.inventory import load
from vostok.data.pe import PEImage


def _percent(value: float) -> str:
    return f"{value:.2f}%"


def _print_report(module: str | None = None) -> None:
    report = pipeline.load_report()
    coverage = report["coverage"]
    print(
        "Data: coverage {} ({:,}/{:,} bytes), fidelity {} ({:,}/{:,} compared bytes)".format(
            _percent(coverage["gross_coverage_percent"]),
            coverage["claimed_bytes"], coverage["gross_bytes"],
            _percent(coverage["fidelity_percent"]),
            coverage["exact_bytes"], coverage["compared_bytes"],
        )
    )
    counts = report["modules"].get(module, {}) if module else report["counts"]
    print("  " + "  ".join(f"{name}={count:,}" for name, count in counts.items()))
    if not module:
        for name, values in report["modules"].items():
            exact = values.get("EXACT", 0)
            total = sum(values.values())
            if total:
                print(f"  {name:<18} {exact:>6,}/{total:<6,} exact")


def _search_tsv(path: Path, pattern: str) -> int:
    if not path.is_file():
        return 0
    needle = pattern.casefold()
    matches = 0
    for line in path.read_text(encoding="utf-8", errors="replace").splitlines():
        if needle in line.casefold():
            print(f"{path.parent.name}: {line}")
            matches += 1
    return matches


def _symbol(pattern: str) -> int:
    if not paths.DATA_STATE.is_file():
        raise RuntimeError("no data ledger; run `python3 -m vostok data refresh`")
    needle = pattern.casefold()
    with paths.DATA_STATE.open(newline="", encoding="utf-8") as source:
        rows = list(csv.DictReader(source, delimiter="\t"))
    matches = [row for row in rows if needle in "\t".join(row.values()).casefold()]
    for row in matches:
        print(
            f"{row['status']:<15} {row['module']:<16} {row['target_rva']:>10} -> "
            f"{row['base_rva']:<10} {row['target_size']:>8}/{row['base_size']:<8} "
            f"{row['identity']}"
        )
        if row["note"] not in ("", "-"):
            print(f"  {row['note']}")
    return 0 if matches else 1


def _covered_intervals(symbols, image: PEImage) -> list[tuple[int, int]]:
    intervals = []
    for symbol in symbols:
        if symbol.size is None or symbol.size <= 0:
            continue
        try:
            if image.section(symbol.section).contains(symbol.rva, symbol.size):
                intervals.append((symbol.rva, symbol.rva + symbol.size))
        except KeyError:
            pass
    merged = []
    for start, end in sorted(intervals):
        if not merged or start > merged[-1][1]:
            merged.append([start, end])
        else:
            merged[-1][1] = max(merged[-1][1], end)
    return [(start, end) for start, end in merged]


def _coverage() -> None:
    report = pipeline.load_report()
    values = report["coverage"]
    print(json.dumps(values, indent=2))
    symbols = load(paths.DATA_TARGET_INDEX)
    image = PEImage(pipeline.image_paths("target")[0])
    claimed = _covered_intervals(symbols, image)
    gaps = []
    for section_name in (".rdata", ".data"):
        section = image.section(section_name)
        cursor = section.rva
        for start, end in claimed:
            if end <= section.rva or start >= section.end:
                continue
            if start > cursor:
                gaps.append((start - cursor, cursor, start, section_name))
            cursor = max(cursor, min(end, section.end))
        if cursor < section.end:
            gaps.append((section.end - cursor, cursor, section.end, section_name))
    print("Largest uncovered target ranges:")
    for size, start, end, section in sorted(gaps, reverse=True)[:20]:
        print(f"  {section:<7} {start:#010x}..{end:#010x}  {size:>10,} bytes")


def _gate_values() -> dict[str, float]:
    if not paths.DATA_GATE.is_file():
        return {}
    with paths.DATA_GATE.open(newline="", encoding="utf-8") as source:
        reader = csv.DictReader(
            (line for line in source if not line.lstrip().startswith("#")),
            delimiter="\t",
        )
        return {row["metric"]: float(row["minimum"]) for row in reader}


def _check(gate: bool) -> int:
    required = (
        paths.DATA_TARGET_INDEX, paths.DATA_BASE_INDEX,
        paths.DATA_TARGET_ACCESS, paths.DATA_BASE_ACCESS,
        paths.DATA_TARGET_RELOCS, paths.DATA_BASE_RELOCS,
        paths.DATA_REPORT, paths.DATA_COVERAGE, paths.DATA_STATE,
    )
    missing = [path for path in required if not path.is_file()]
    if missing:
        print("missing data artifacts: " + ", ".join(str(path) for path in missing),
              file=sys.stderr)
        return 1
    report = pipeline.load_report()
    if report.get("schema") != 1:
        print("unsupported data report schema", file=sys.stderr)
        return 1
    if sum(report["counts"].values()) != len(report["rows"]):
        print("data report count mismatch", file=sys.stderr)
        return 1
    with paths.DATA_STATE.open(newline="", encoding="utf-8") as source:
        ledger_rows = list(csv.DictReader(source, delimiter="\t"))
    ledger_counts = Counter(row["status"] for row in ledger_rows)
    if dict(sorted(ledger_counts.items())) != report["counts"]:
        print("tracked data ledger does not match data report", file=sys.stderr)
        return 1
    report_rows = [
        {column: str(row[column]) for column in pipeline.LEDGER_COLUMNS}
        for row in report["rows"]
    ]
    if ledger_rows != report_rows:
        print("tracked data ledger rows do not match data report", file=sys.stderr)
        return 1
    coverage = json.loads(paths.DATA_COVERAGE.read_text(encoding="utf-8"))
    if coverage != report["coverage"]:
        print("standalone coverage report does not match data report", file=sys.stderr)
        return 1
    for side in ("target", "base"):
        exe, pdb = pipeline.image_paths(side)
        actual = {
            "exe_sha256": pipeline._file_hash(exe),
            "pdb_sha256": pipeline._file_hash(pdb),
            "index_sha256": pipeline._file_hash(pipeline.index_path(side)),
            "access_sha256": pipeline._file_hash(pipeline.access_path(side)),
            "relocations_sha256": pipeline._file_hash(pipeline.reloc_path(side)),
        }
        if report.get("inputs", {}).get(side) != actual:
            print(f"data report is stale for {side} image/PDB", file=sys.stderr)
            return 1
    zlib = report["modules"].get("zlib", {})
    if not zlib or zlib.get("EXACT", 0) != sum(zlib.values()):
        print("zlib calibration is not fully exact", file=sys.stderr)
        return 1
    if not gate:
        print("data artifacts valid (shadow mode; calibrated floors not enforced)")
        return 0
    floors = _gate_values()
    if not floors:
        print(f"no calibrated floors in {paths.DATA_GATE}", file=sys.stderr)
        return 1
    values = report["coverage"]
    failed = []
    for metric, minimum in floors.items():
        actual = float(values.get(metric, report["counts"].get(metric, 0)))
        if actual < minimum:
            failed.append(f"{metric}={actual} < {minimum}")
    if failed:
        print("data gate failed: " + "; ".join(failed), file=sys.stderr)
        return 1
    print("data gate passed")
    return 0


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    sub = parser.add_subparsers(dest="command", required=True)
    init = sub.add_parser("init-target")
    init.add_argument("--force", action="store_true")
    sub.add_parser("refresh")
    report = sub.add_parser("report")
    report.add_argument("--module")
    for name in ("symbol", "access", "relocs"):
        command = sub.add_parser(name)
        command.add_argument("pattern")
    sub.add_parser("coverage")
    check = sub.add_parser("check")
    check.add_argument("--gate", action="store_true")
    args = parser.parse_args(argv)

    try:
        if args.command == "init-target":
            pipeline.init_target(force=args.force)
        elif args.command == "refresh":
            pipeline.refresh()
        elif args.command == "report":
            _print_report(args.module)
        elif args.command == "symbol":
            return _symbol(args.pattern)
        elif args.command == "access":
            return 0 if sum(_search_tsv(path, args.pattern) for path in (
                paths.DATA_TARGET_ACCESS, paths.DATA_BASE_ACCESS
            )) else 1
        elif args.command == "relocs":
            return 0 if sum(_search_tsv(path, args.pattern) for path in (
                paths.DATA_TARGET_RELOCS, paths.DATA_BASE_RELOCS
            )) else 1
        elif args.command == "coverage":
            _coverage()
        elif args.command == "check":
            return _check(args.gate)
    except (OSError, ValueError, RuntimeError, subprocess.CalledProcessError) as error:
        print(f"vostok data: {error}", file=sys.stderr)
        return 2
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
