# SPDX-License-Identifier: GPL-3.0-or-later

"""vostok data - image-level matching for globals, constants, BSS, and pointers.

Commands:
  init-target     export the immutable retail PDB/image inventory and access map
  missing         update data_symbols.tsv with referenced allocations absent from PDB
  missing-next    inspect the first datum which still needs manual review
  missing-report  summarize the non-PDB recovery queue
  missing-symbol PATTERN inspect a non-PDB symbol or xref
  project         regenerate consumer-owned target/base delink manifests
  refresh         refresh image audits and generated binaries/gen tables
  render-relocs [PATTERN] direct render relocation/data audit or inspection
  module-relocs MODULE [PATTERN] direct relocation/data audit for any module
  review PATTERN  record a source-and-diff-scoped datum wall review
  report          summarize exactness and divergence classes
  symbol PATTERN  inspect matching ledger rows
  access PATTERN  find retail/base code references to a data symbol or address
  relocs PATTERN  inspect pointer cells and their resolved targets
  function PATTERN inspect per-function data-reference equality
  coverage        show retail coverage, objdiff enrollment, and generated gaps
  check [--gate]  validate artifacts; optionally enforce the integrity ratchet

The image audit is not the objdiff data score. Its EXACT means
relocation-normalized linked bytes and resolved pointer targets both agree;
objdiff becomes authoritative only for data enrolled by the delink manifests.
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
from vostok.data import gate as data_gate
from vostok.data import missing, pipeline, render_relocs, reviews as data_reviews
from vostok.data.inventory import load
from vostok.data.pe import PEImage
from vostok.core import log as _log


def _percent(value: float) -> str:
    return f"{value:.2f}%"


def _print_report(module: str | None = None) -> None:
    report = pipeline.load_report()
    coverage = report["coverage"]
    print(
        "Data: reachable retail data {} ({:,}/{:,} unique bytes), paired {} "
        "({:,}/{:,}); objdiff matched {} ({:,}/{:,} projected bytes)".format(
            _percent(coverage["consumer_reachable_percent"]),
            coverage["consumer_reachable_bytes"], coverage["gross_bytes"],
            _percent(coverage["consumer_paired_percent"]),
            coverage["consumer_paired_bytes"], coverage["gross_bytes"],
            _percent(coverage["objdiff_match_percent"]),
            coverage["objdiff_matched_bytes"], coverage["objdiff_projected_bytes"],
        )
    )
    print(
        "  image audit: typed coverage {} ({:,}/{:,}), whole-image exact {} "
        "({:,}/{:,}); paired-only fidelity {}".format(
            _percent(coverage["gross_coverage_percent"]), coverage["claimed_bytes"],
            coverage["gross_bytes"], _percent(coverage["image_exact_percent"]),
            coverage["exact_bytes"], coverage["gross_bytes"],
            _percent(coverage["fidelity_percent"]),
        )
    )
    counts = report["modules"].get(module, {}) if module else report["counts"]
    print("  " + "  ".join(f"{name}={count:,}" for name, count in counts.items()))
    if not module:
        consumer = report.get("consumer_projection", {})
        strict = report.get("strict_referents", {})
        if consumer:
            print(
                "  consumer projection: {consumer_units:,} units, "
                "{paired_copies:,}/{allocation_copies:,} allocation copies paired, "
                "{blockers:,} blockers".format(**consumer)
            )
        if strict:
            print(
                "  strict referents: {referent_debt_functions:,} exact functions / "
                "{referent_debt_code_bytes:,} code bytes exposed as relocation debt".format(
                    **strict
                )
            )
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


def _ratchet_values() -> dict[str, float]:
    if not paths.DATA_INTEGRITY_RATCHET.is_file():
        return {}
    with paths.DATA_INTEGRITY_RATCHET.open(newline="", encoding="utf-8") as source:
        reader = csv.DictReader(
            (line for line in source if not line.lstrip().startswith("#")),
            delimiter="\t",
        )
        return {row["metric"]: float(row["maximum"]) for row in reader}


def _check(gate: bool) -> int:
    required = (
        paths.DATA_TARGET_INDEX, paths.DATA_BASE_INDEX,
        paths.DATA_TARGET_ACCESS, paths.DATA_BASE_ACCESS,
        paths.DATA_TARGET_RELOCS, paths.DATA_BASE_RELOCS,
        paths.DATA_REPORT, paths.DATA_COVERAGE, paths.DATA_STATE,
        paths.DATA_COVERAGE_GAPS, paths.RETAIL_DATA,
        paths.DATA_INTEGRITY_RATCHET,
        paths.DELINK_DATA_MANIFEST, paths.DELINK_DATA_SECTION_MANIFEST,
        paths.BASE_DELINK_DATA_MANIFEST, paths.BASE_DELINK_DATA_SECTION_MANIFEST,
        paths.DATA_CONSUMER_CLOSURE, paths.DATA_FUNCTION_STATE,
        paths.DATA_MANIFEST_BLOCKERS, paths.DATA_STRICT_REPORT,
        paths.DATA_OBJDIFF_REPORT, paths.DATA_MISSING_CANDIDATES,
        paths.DATA_MISSING_XREFS, paths.DATA_MISSING_REPORT,
        paths.RETAIL_DATA_SYMBOLS, paths.RETAIL_PDB_DATA_EXTENTS,
        paths.RETAIL_RELOC_REFERENTS,
        paths.DATA_RENDER_RELOC_AUDIT, paths.DATA_RENDER_EXTENTLESS,
        paths.DATA_RENDER_FUNCTION_DATA, paths.DATA_RENDER_RELOC_REPORT,
        paths.DATA_RENDER_PROBLEMS,
        paths.DATA_MODULE_RELOC_REPORT, paths.DATA_FUNCTION_OPEN,
        paths.CODEX_WALL_REVIEWS,
    )
    missing_paths = [path for path in required if not path.is_file()]
    if missing_paths:
        print("missing data artifacts: " + ", ".join(str(path) for path in missing_paths),
              file=sys.stderr)
        return 1
    if missing.check():
        return 1
    if data_reviews.check():
        return 1
    if data_gate.check(require_zero=gate):
        return 1
    report = pipeline.load_report()
    if report.get("schema") != 2:
        print("unsupported data report schema", file=sys.stderr)
        return 1
    if sum(report["counts"].values()) != len(report["rows"]):
        print("data report count mismatch", file=sys.stderr)
        return 1
    with paths.DATA_STATE.open(newline="", encoding="utf-8") as source:
        ledger_rows = list(csv.DictReader(source, delimiter="\t"))
    ledger_counts = Counter(row["status"] for row in ledger_rows)
    if dict(sorted(ledger_counts.items())) != report["counts"]:
        print("generated data image state does not match its report", file=sys.stderr)
        return 1
    report_rows = [
        {column: str(row[column]) for column in pipeline.LEDGER_COLUMNS}
        for row in report["rows"]
    ]
    if ledger_rows != report_rows:
        print("generated data image rows do not match their report", file=sys.stderr)
        return 1
    with paths.RETAIL_DATA.open(newline="", encoding="utf-8") as source:
        census_reader = csv.DictReader(
            (line for line in source if not line.lstrip().startswith("#")),
            delimiter="\t",
        )
        if census_reader.fieldnames != ["rva", "kind"]:
            print("retail data census has an unexpected schema", file=sys.stderr)
            return 1
        census = list(census_reader)
    allowed_kinds = {
        "datum", "string", "fppool", "vtable", "rtti", "ehtable", "guard",
        "common", "copy", "pad",
    }
    census_rvas = [int(row["rva"], 0) for row in census]
    if census_rvas != sorted(set(census_rvas)):
        print("retail data census RVAs are not unique and sorted", file=sys.stderr)
        return 1
    if any(row["kind"] not in allowed_kinds for row in census):
        print("retail data census contains an unsupported kind", file=sys.stderr)
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
        if side == "target":
            actual.update({
                "retail_data_sha256": pipeline._file_hash(paths.RETAIL_DATA),
                "coverage_gaps_sha256": pipeline._file_hash(paths.DATA_COVERAGE_GAPS),
                "consumer_closure_sha256": pipeline._file_hash(
                    paths.DATA_CONSUMER_CLOSURE
                ),
                "function_state_sha256": pipeline._file_hash(
                    paths.DATA_FUNCTION_STATE
                ),
                "manifest_blockers_sha256": pipeline._file_hash(
                    paths.DATA_MANIFEST_BLOCKERS
                ),
                "target_manifest_sha256": pipeline._file_hash(
                    paths.DELINK_DATA_MANIFEST
                ),
                "target_section_manifest_sha256": pipeline._file_hash(
                    paths.DELINK_DATA_SECTION_MANIFEST
                ),
                "base_manifest_sha256": pipeline._file_hash(
                    paths.BASE_DELINK_DATA_MANIFEST
                ),
                "base_section_manifest_sha256": pipeline._file_hash(
                    paths.BASE_DELINK_DATA_SECTION_MANIFEST
                ),
                "strict_report_sha256": pipeline._file_hash(
                    paths.DATA_STRICT_REPORT
                ),
                "data_objdiff_report_sha256": pipeline._file_hash(
                    paths.DATA_OBJDIFF_REPORT
                ),
            })
        if report.get("inputs", {}).get(side) != actual:
            print(f"data report is stale for {side} image/PDB", file=sys.stderr)
            return 1
    zlib = report["modules"].get("zlib", {})
    if not zlib or zlib.get("EXACT", 0) != sum(zlib.values()):
        print("zlib calibration is not fully exact", file=sys.stderr)
        return 1
    if not gate:
        print("data artifacts valid (shadow mode; integrity ratchet not enforced)")
        return 0
    maxima = _ratchet_values()
    if not maxima:
        print(f"no calibrated maxima in {paths.DATA_INTEGRITY_RATCHET}", file=sys.stderr)
        return 1
    values = {
        **report["coverage"],
        **report.get("consumer_projection", {}),
        **report.get("strict_referents", {}),
    }
    failed = []
    for metric, maximum in maxima.items():
        actual = float(values.get(metric, report["counts"].get(metric, 0)))
        if actual > maximum:
            failed.append(f"{metric}={actual} > {maximum}")
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
    missing_command = sub.add_parser("missing")
    missing_command.add_argument("--no-export", action="store_true")
    missing_command.add_argument("--check", action="store_true")
    sub.add_parser("missing-next")
    sub.add_parser("missing-report")
    sub.add_parser("refresh")
    sub.add_parser("project")
    render = sub.add_parser("render-relocs")
    render.add_argument("pattern", nargs="?")
    render.add_argument("--check", action="store_true")
    module_relocs = sub.add_parser("module-relocs")
    module_relocs.add_argument("module")
    module_relocs.add_argument("pattern", nargs="?")
    module_relocs.add_argument("--check", action="store_true")
    review = sub.add_parser("review")
    review.add_argument("target", nargs="?")
    review.add_argument("--status", choices=data_reviews.STATUSES)
    review.add_argument(
        "--class", dest="wall_class", choices=data_reviews.CLASSES
    )
    review.add_argument("--evidence")
    review.add_argument("--list", action="store_true")
    report = sub.add_parser("report")
    report.add_argument("--module")
    for name in ("symbol", "access", "relocs", "function", "missing-symbol"):
        command = sub.add_parser(name)
        command.add_argument("pattern")
    sub.add_parser("coverage")
    check = sub.add_parser("check")
    check.add_argument("--gate", action="store_true")
    args = parser.parse_args(argv)

    try:
        if args.command == "init-target":
            pipeline.init_target(force=args.force)
        elif args.command == "missing":
            if args.check:
                return missing.check()
            missing_report = missing.refresh(export=not args.no_export)
            missing.print_report(missing_report)
        elif args.command == "missing-next":
            return missing.inspect_next()
        elif args.command == "missing-report":
            missing.print_report()
        elif args.command == "missing-symbol":
            return missing.inspect(args.pattern)
        elif args.command == "refresh":
            pipeline.refresh()
            render_relocs.print_report(render_relocs.refresh())
        elif args.command == "project":
            pipeline.prepare_manifests()
        elif args.command == "render-relocs":
            if args.check:
                return render_relocs.check()
            if args.pattern:
                return render_relocs.inspect(args.pattern)
            render_relocs.print_report(render_relocs.refresh())
        elif args.command == "module-relocs":
            if args.check:
                if args.module == "all":
                    return data_gate.check(require_zero=True)
                return render_relocs.check(args.module)
            if args.pattern:
                return render_relocs.inspect(args.pattern, args.module)
            if args.module == "all":
                aggregate = data_gate.refresh()
                print(
                    "all-module datum use: "
                    f"{aggregate['summary']['modules']} module(s), "
                    f"OPEN={aggregate['summary']['open_function_data']:,}"
                )
                return 0
            module_report = render_relocs.refresh(args.module)
            render_relocs.print_report(module_report)
        elif args.command == "review":
            if args.list:
                return data_reviews.list_reviews()
            if not all((
                args.target, args.status, args.wall_class, args.evidence,
            )):
                parser.error(
                    "review requires TARGET, --status, --class, and --evidence"
                )
            return data_reviews.record(
                args.target, args.status, args.wall_class, args.evidence
            )
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
        elif args.command == "function":
            return 0 if _search_tsv(paths.DATA_FUNCTION_STATE, args.pattern) else 1
        elif args.command == "coverage":
            _coverage()
        elif args.command == "check":
            return _check(args.gate)
    except (OSError, ValueError, RuntimeError, subprocess.CalledProcessError) as error:
        print(f"vostok data: {error}", file=sys.stderr)
        return 2
    return 0


if __name__ == "__main__":
    raise SystemExit(_log.run("vostok.data.cli", main))
