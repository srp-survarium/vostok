# SPDX-License-Identifier: GPL-3.0-or-later

"""Aggregate every module's direct datum-use audit into one zero gate."""

from __future__ import annotations

import csv
import json

from vostok.core import paths
from vostok.core.tsv import write_if_changed
from vostok.data import pipeline, render_relocs
from vostok.ledger import store


OPEN_COLUMNS = ("module", *render_relocs.FUNCTION_DATA_COLUMNS)


def modules() -> tuple[str, ...]:
    """Every module in the current function ledger, with no silent allowlist."""
    return tuple(sorted({
        str(row.get("module") or "").strip()
        for row in store.load().values()
        if str(row.get("module") or "").strip()
    }))


def _inputs() -> dict[str, str]:
    return {
        "match_state_sha256": pipeline._file_hash(paths.MATCH_STATE),
        "target_exe_sha256": pipeline._file_hash(
            pipeline.image_paths("target")[0]
        ),
        "base_exe_sha256": pipeline._file_hash(
            pipeline.image_paths("base")[0]
        ),
    }


def _module_row(module: str, report: dict) -> dict:
    artifacts = render_relocs._artifacts(module)
    return {
        "report_sha256": pipeline._file_hash(artifacts.report),
        "target_relocation_sites": report["target_relocation_sites"],
        "base_relocation_sites": report["base_relocation_sites"],
        "function_data_status": report["function_data_status"],
        "function_data_resolution": report["function_data_resolution"],
        "open_function_data": report["function_data_resolution"].get(
            "OPEN", 0
        ),
    }


def _summary(module_rows: dict[str, dict]) -> dict[str, int]:
    return {
        "modules": len(module_rows),
        "target_relocation_sites": sum(
            row["target_relocation_sites"] for row in module_rows.values()
        ),
        "base_relocation_sites": sum(
            row["base_relocation_sites"] for row in module_rows.values()
        ),
        "open_function_data": sum(
            row["open_function_data"] for row in module_rows.values()
        ),
    }


def _open_rows(module: str) -> list[dict[str, str]]:
    path = render_relocs._artifacts(module).function_data
    with path.open(newline="", encoding="utf-8") as source:
        reader = csv.DictReader(source, delimiter="\t")
        if tuple(reader.fieldnames or ()) != render_relocs.FUNCTION_DATA_COLUMNS:
            raise ValueError(f"{path}: unexpected function-data schema")
        return [
            {"module": module, **row}
            for row in reader
            if row["resolution"] == "OPEN"
        ]


def refresh() -> dict:
    """Refresh every ledger module and write the aggregate denominator."""
    context = render_relocs.build_audit_context()
    module_rows = {}
    open_rows = []
    for module in modules():
        report = render_relocs.refresh(module, context)
        module_rows[module] = _module_row(module, report)
        open_rows.extend(_open_rows(module))
        print(
            f"[data-gate] {module}: "
            f"OPEN={module_rows[module]['open_function_data']:,}",
            flush=True,
        )
    render_relocs._write_tsv(paths.DATA_FUNCTION_OPEN, OPEN_COLUMNS, open_rows)
    report = {
        "schema": 3,
        "inputs": _inputs(),
        "summary": _summary(module_rows),
        "modules": module_rows,
        "outputs": {
            "function_open_sha256": pipeline._file_hash(
                paths.DATA_FUNCTION_OPEN
            ),
        },
    }
    write_if_changed(
        paths.DATA_MODULE_RELOC_REPORT,
        json.dumps(report, indent=2, sort_keys=True) + "\n",
    )
    return report


def load_report() -> dict:
    return json.loads(
        paths.DATA_MODULE_RELOC_REPORT.read_text(encoding="utf-8")
    )


def check(*, require_zero: bool = False) -> int:
    """Validate the complete module set; optionally require zero OPEN rows."""
    missing = [
        path for path in (
            paths.DATA_MODULE_RELOC_REPORT, paths.DATA_FUNCTION_OPEN,
        ) if not path.is_file()
    ]
    if missing:
        print(
            "missing aggregate datum-use artifact(s): "
            + ", ".join(map(str, missing))
        )
        return 1
    report = load_report()
    if report.get("schema") != 3:
        print("unsupported aggregate datum-use report schema")
        return 1
    if report.get("inputs") != _inputs():
        print("aggregate datum-use report inputs are stale")
        return 1
    expected_modules = modules()
    if tuple(sorted(report.get("modules", {}))) != expected_modules:
        print("aggregate datum-use report does not cover every ledger module")
        return 1

    context = render_relocs.build_audit_context()
    module_inputs = render_relocs._inputs()
    module_rows = {}
    open_rows = []
    for module in expected_modules:
        if render_relocs.check(module, context, module_inputs):
            return 1
        module_report = render_relocs._load_report(module)
        module_rows[module] = _module_row(module, module_report)
        open_rows.extend(_open_rows(module))
    if report["modules"] != module_rows:
        print("aggregate datum-use module rows are stale or inconsistent")
        return 1
    summary = _summary(module_rows)
    if report.get("summary") != summary:
        print("aggregate datum-use summary is inconsistent")
        return 1
    if summary["open_function_data"] != len(open_rows):
        print("aggregate datum-use open-row count is inconsistent")
        return 1
    with paths.DATA_FUNCTION_OPEN.open(newline="", encoding="utf-8") as source:
        reader = csv.DictReader(source, delimiter="\t")
        if tuple(reader.fieldnames or ()) != OPEN_COLUMNS:
            print("aggregate datum-use open queue has an unexpected schema")
            return 1
        if list(reader) != open_rows:
            print("aggregate datum-use open queue is stale or inconsistent")
            return 1
    outputs = {
        "function_open_sha256": pipeline._file_hash(paths.DATA_FUNCTION_OPEN),
    }
    if report.get("outputs") != outputs:
        print("aggregate datum-use output hashes are stale")
        return 1
    if require_zero and summary["open_function_data"]:
        debt = ", ".join(
            f"{module}={row['open_function_data']}"
            for module, row in module_rows.items()
            if row["open_function_data"]
        )
        print(
            "datum-use zero gate failed: "
            f"OPEN={summary['open_function_data']} ({debt})"
        )
        return 1
    print(
        "datum-use aggregate valid: "
        f"{summary['modules']} module(s), "
        f"OPEN={summary['open_function_data']:,}"
    )
    return 0
