"""Render the independent image-data summary in README.md."""

from __future__ import annotations

import csv
import datetime
import json
from collections import Counter

from vostok.core.paths import DATA_GATE, DATA_REPORT, DATA_STATE, README

START = "<!-- data-match:start -->"
END = "<!-- data-match:end -->"


def render() -> str:
    if not DATA_STATE.is_file() or not DATA_REPORT.is_file():
        raise RuntimeError("data ledger/report unavailable")
    with DATA_STATE.open(newline="", encoding="utf-8") as source:
        rows = list(csv.DictReader(source, delimiter="\t"))
    counts = Counter(row["status"] for row in rows)
    metrics = json.loads(DATA_REPORT.read_text(encoding="utf-8"))["coverage"]
    paired = sum(counts[name] for name in (
        "EXACT", "BYTES", "RELOCS", "RELOC_TOPOLOGY", "SIZE"
    ))
    gate_armed = False
    if DATA_GATE.is_file():
        gate_armed = any(
            line.strip() and not line.lstrip().startswith("#")
            and not line.startswith("metric\t")
            for line in DATA_GATE.read_text(encoding="utf-8").splitlines()
        )
    return "\n".join((
        START,
        "## Data match status",
        "",
        "_Auto-generated from the independent PDB/image data lane; it does not "
        "feed the function score._",
        "",
        "**Retail extent coverage: {:.2f}% ({:,} / {:,} virtual bytes) &middot; "
        "fidelity: {:.2f}% ({:,} / {:,} compared bytes) &middot; definitions exact: "
        "{:,} / {:,} paired.**".format(
            metrics["gross_coverage_percent"], metrics["claimed_bytes"],
            metrics["gross_bytes"], metrics["fidelity_percent"],
            metrics["exact_bytes"], metrics["compared_bytes"],
            counts["EXACT"], paired,
        ),
        "",
        "_EXACT requires relocation-normalized bytes and resolved pointer targets "
        "to agree. Gate: {}. Inspect with `python3 -m vostok data report`._".format(
            "armed" if gate_armed else "shadow calibration"
        ),
        "",
        f"_Updated {datetime.date.today().isoformat()}._",
        END,
    ))


def write_readme(block: str) -> None:
    text = README.read_text(encoding="utf-8")
    if START in text and END in text:
        pre = text[:text.index(START)]
        post = text[text.index(END) + len(END):]
        README.write_text(pre + block + post, encoding="utf-8")
        return
    marker = "<!-- match-score:end -->"
    index = text.index(marker) + len(marker)
    README.write_text(text[:index] + "\n\n" + block + text[index:], encoding="utf-8")
