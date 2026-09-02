# SPDX-License-Identifier: GPL-3.0-or-later

"""Render the independent image-data summary in README.md."""

from __future__ import annotations

import csv
import datetime
import json
from collections import Counter

from vostok.core.paths import DATA_INTEGRITY_RATCHET, DATA_REPORT, DATA_STATE, README

START = "<!-- data-match:start -->"
END = "<!-- data-match:end -->"


def render() -> str:
    if not DATA_STATE.is_file() or not DATA_REPORT.is_file():
        raise RuntimeError("data ledger/report unavailable")
    with DATA_STATE.open(newline="", encoding="utf-8") as source:
        rows = list(csv.DictReader(source, delimiter="\t"))
    counts = Counter(row["status"] for row in rows)
    report = json.loads(DATA_REPORT.read_text(encoding="utf-8"))
    metrics = report["coverage"]
    consumer = report.get("consumer_projection", {})
    strict = report.get("strict_referents", {})
    paired = sum(counts[name] for name in (
        "EXACT", "BYTES", "RELOCS", "RELOC_TOPOLOGY", "SIZE"
    ))
    ratchet_armed = False
    if DATA_INTEGRITY_RATCHET.is_file():
        ratchet_armed = any(
            line.strip() and not line.lstrip().startswith("#")
            and not line.startswith("metric\t")
            for line in DATA_INTEGRITY_RATCHET.read_text(encoding="utf-8").splitlines()
        )
    return "\n".join((
        START,
        "## Data reconstruction status",
        "",
        "_Auto-generated from the consumer-owned objdiff projection plus the "
        "independent linked-image audit; it does not feed the function score._",
        "",
        "**Function-reachable retail data: {:.2f}% ({:,} / {:,} unique bytes) "
        "&middot; paired for comparison: {:.2f}% ({:,} / {:,}) &middot; matched projected "
        "copies: {:.2f}% ({:,} / {:,}).**".format(
            metrics["consumer_reachable_percent"],
            metrics["consumer_reachable_bytes"], metrics["gross_bytes"],
            metrics["consumer_paired_percent"], metrics["consumer_paired_bytes"],
            metrics["gross_bytes"], metrics["objdiff_match_percent"],
            metrics["objdiff_matched_bytes"], metrics["objdiff_projected_bytes"],
        ),
        "",
        "_The projection has {:,} paired copies across {:,} units and {:,} "
        "unresolved blockers. Strict relocation comparison exposes {:,} "
        "otherwise-exact functions ({:,} code bytes) with referent debt._".format(
            consumer.get("paired_copies", 0), consumer.get("consumer_units", 0),
            consumer.get("blockers", 0), strict.get("referent_debt_functions", 0),
            strict.get("referent_debt_code_bytes", 0),
        ),
        "",
        "_The linked-image audit is {:.2f}% exact and covers {:.2f}% by PDB type "
        "extent, with {:.2f}% paired-only fidelity ({} / {} definitions). "
        "Integrity ratchet: {}._".format(
            metrics["image_exact_percent"], metrics["gross_coverage_percent"],
            metrics["fidelity_percent"],
            counts["EXACT"], paired,
            "armed" if ratchet_armed else "shadow calibration",
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
