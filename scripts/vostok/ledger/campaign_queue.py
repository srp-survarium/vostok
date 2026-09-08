# SPDX-License-Identifier: GPL-3.0-or-later
"""Full-engine evidence queue, separate from the historical filtered queue.

This is a projection, never a score writer. Approximate statement classes,
recorded-local differences and unpaired procedures are triage evidence, not a
license to delete source omitted by optimization or to fabricate missing bodies.
"""

from collections import Counter
import hashlib
import json

from vostok.core import paths
from vostok.derive.artifacts import git_head
from vostok.derive.roster import derive
from vostok.ledger import store
from vostok.ledger.readme import _NON_ENGINE


SECTIONS = (
    ("structure", "Below 100%: statement/local candidates"),
    ("presence", "Unpaired/unscored: establish presence and measurement"),
    ("bytes", "Below 100%: per-statement SIZE and byte-only residuals"),
    ("exact-last", "Last: structure/presence candidates with current-source max=100%"),
)


def recorded_locals(record):
    """Compare named records as a multiset; retain CV spelling and scope.

    PDB register/stack allocation is intentionally not compared here. Formatting
    whitespace is not a type difference, but const qualification and names are.
    """
    return Counter(
        (local.get("name", ""), " ".join(local.get("ty", "").split()),
         str(local.get("scope", "")))
        for local in record.get("locals") or []
    )


def _local_details(delta):
    return [
        {"name": name, "type": ty, "scope": scope, "count": count}
        for (name, ty, scope), count in sorted(delta.items())
    ]


def ledger_only_rows(roster, ledger, coverage):
    """Keep compiler-generated ledger identities lacking rich-PDB procedures.

    Object bytes can still be measured without a procedure record. Do not
    invent statement counts, addresses or a structural verdict for these rows.
    """
    report_scores = {}
    for _unit, mangled, fuzzy, _size in roster.artifacts.report_fns:
        report_scores.setdefault(mangled, [])
        if fuzzy is not None:
            report_scores[mangled].append(fuzzy)
    for mangled, previous in ledger.items():
        module = previous.get("module")
        if mangled in roster.target or not module or module in _NON_ENGINE:
            continue
        coverage["engine_ledger_only_records"] += 1
        scores = report_scores.get(mangled)
        current = max(scores) if scores else None
        evidence = ["RICH_COVERAGE"]
        if scores is None:
            evidence.append("LEDGER_ONLY")
        elif current is None:
            evidence.append("UNSCORED")
        maximum = previous.get("max")
        exact = maximum is not None and maximum >= store.EXACT
        yield {
            "section": "exact-last" if exact else "presence",
            "mangled": mangled,
            "function": roster.artifacts.demangled.get(mangled, mangled),
            "module": module, "unit": previous.get("unit", ""),
            "file": None, "line": None, "target_rva": None, "base_rva": None,
            "target_bytes": previous.get("size"),
            "frameless": previous.get("flags") == "f",
            "evidence": evidence, "ledger_class": None,
            "current": current, "max": maximum, "hist": previous.get("hist"),
            "status": previous.get("status", "unreviewed"),
            "note": previous.get("note", ""),
            "target_statements": None, "base_statements": None,
            "target_locals_only": [], "base_locals_only": [],
        }


def collect(roster, ledger):
    rows = []
    coverage = Counter()
    for mangled, function in roster.target.items():
        if not function.module:
            coverage["unknown_module_owner"] += 1
            continue
        if function.module in _NON_ENGINE:
            coverage["excluded_vendor_or_internal"] += 1
            continue
        coverage["engine_target_records"] += 1
        previous = ledger.get(mangled, {})
        pair = roster.pairing.pairs.get(mangled)
        target = roster.artifacts.target[mangled]
        base = roster.pairing.base_record(mangled, roster.artifacts) if pair else None
        maximum = previous.get("max")
        current = pair.fuzzy if pair else None
        exact = maximum is not None and maximum >= store.EXACT
        evidence = []
        target_locals_only = []
        base_locals_only = []
        if pair is None:
            evidence.append("PRESENCE")
            coverage["unpaired"] += 1
        else:
            coverage["paired"] += 1
            if pair.cls != "MATCH":
                evidence.append(pair.cls)
            if base is not None:
                coverage["locals_compared"] += 1
                target_locals = recorded_locals(target)
                base_locals = recorded_locals(base)
                target_locals_only = _local_details(target_locals - base_locals)
                base_locals_only = _local_details(base_locals - target_locals)
                if target_locals_only or base_locals_only:
                    evidence.append("LOCALS")
            else:
                coverage["paired_without_base_record"] += 1
                evidence.append("LOCAL_COVERAGE")
            if current is None and not exact:
                evidence.append("UNSCORED")
            if not evidence and not exact:
                evidence.append("BYTE_ONLY")
        if not evidence:
            coverage["exact_without_queue_evidence"] += 1
            continue
        if exact:
            section = "exact-last"
        elif any(item in evidence for item in ("QUANTITY", "SPLIT", "LOCALS")):
            section = "structure"
        elif any(item in evidence for item in ("PRESENCE", "UNSCORED", "LOCAL_COVERAGE")):
            section = "presence"
        else:
            section = "bytes"
        rows.append({
            "section": section, "mangled": mangled,
            "function": function.demangled, "module": function.module,
            "unit": function.unit or function.file, "file": function.file,
            "line": function.line, "target_rva": function.rva,
            "base_rva": pair.base_rva if pair else None,
            "target_bytes": function.size, "frameless": function.frameless,
            "evidence": evidence, "ledger_class": pair.cls if pair else None,
            "current": current, "max": maximum, "hist": previous.get("hist"),
            "status": previous.get("status", "unreviewed"),
            "note": previous.get("note", ""),
            "target_statements": pair.t_stmts if pair else function.n_stmts,
            "base_statements": pair.b_stmts if pair else None,
            "target_locals_only": target_locals_only,
            "base_locals_only": base_locals_only,
        })
    rows.extend(ledger_only_rows(roster, ledger, coverage))
    ranks = {name: index for index, (name, _) in enumerate(SECTIONS)}
    rows.sort(key=lambda row: (ranks[row["section"]], row["module"],
                               row["unit"], row["line"] or -1, row["mangled"]))
    return rows, dict(sorted(coverage.items()))


def _sha256(path):
    with path.open("rb") as source:
        return hashlib.file_digest(source, "sha256").hexdigest()


def snapshot():
    return {
        "head": git_head(),
        "report_head": (paths.REPORT_HEAD.read_text().strip()
                        if paths.REPORT_HEAD.is_file() else None),
        "sha256": {str(path.relative_to(paths.REPO)): _sha256(path)
                   for path in (paths.TARGET_IDX, paths.BASE_IDX, paths.REPORT,
                                paths.MATCH_STATE)},
        "excluded_modules": sorted(_NON_ENGINE),
    }


def _escape(value):
    return str(value or "").replace("|", "&#124;").replace("\n", " ")


def _percent(value):
    return "unscored" if value is None else f"{value:.4f}%"


def _locals_text(row):
    sides = []
    for key, label in (("target_locals_only", "target only"),
                       ("base_locals_only", "base only")):
        if row[key]:
            locals_text = "; ".join(
                f"{item['type']} {item['name']}"
                + (f" (scope {item['scope']})" if item["scope"] else "")
                + (f" x{item['count']}" if item["count"] > 1 else "")
                for item in row[key]
            )
            sides.append(f"{label}: {locals_text}")
    return "; ".join(sides)


def render(payload):
    rows = payload["rows"]
    snap = payload["snapshot"]
    output = [
        "<!-- SPDX-License-Identifier: GPL-3.0-or-later -->",
        "# Full-engine matching campaign queue", "",
        "Generated by `python3 -m vostok ledger mismatch-queue --all-engine --write-queue`.",
        "", f"Source HEAD: `{snap['head']}`. Report HEAD: `{snap['report_head']}`.",
        "",
        "Work one complete translation unit at a time. Functions whose current-source",
        "`max` is 100% go last, even if `cur` dipped. No scores or ledger notes are",
        "changed by this command. Existing parks remain visible, not permanent exclusions.",
        "",
        "QUANTITY/SPLIT/SIZE are approximate candidates; validate with the two-sided",
        "`pdb_fetch --view structure-diff` before changing source. LOCALS compares the",
        "named records, including type/const spelling and exposed block identity, not",
        "register/stack placement. Optimized omissions and ICF aliases require review.",
        "PRESENCE means no established pair, not proof that source is missing.",
        "Frameless and zero-statement target procedures are included, not silently dropped.",
        "BYTE_ONLY means no statement/local difference was detected, but max is below 100%.",
        "RICH_COVERAGE identifies ledger functions with no rich-PDB procedure record;",
        "object scores remain usable, but statement/local structure cannot be assessed.",
        "LEDGER_ONLY additionally means there is no direct current object-report identity.",
        "",
        "Class layout/access/declaration order, enum variants and source definition order",
        "are separate whole-PDB evidence channels: `pdb_topology --classes --json` and",
        "`pdb_divergence`. This function queue is not a claim those channels are clean.",
        "",
        "Excluded vendor/internal modules: " + ", ".join(snap["excluded_modules"]) + ".",
        "Unknown module ownership is counted separately in coverage, not called matched.",
        "", "## Coverage", "", "```json",
        json.dumps(payload["coverage"], indent=2), "```", "",
        "| Module | Statement/local | Presence/unscored | SIZE/bytes | 100%-last |",
        "|---|---:|---:|---:|---:|",
    ]
    counts = Counter((row["module"], row["section"]) for row in rows)
    for module in sorted({row["module"] for row in rows}):
        output.append("| " + module + " | " + " | ".join(
            str(counts[module, section]) for section, _ in SECTIONS) + " |")
    for section, title in SECTIONS:
        members = [row for row in rows if row["section"] == section]
        output.extend(["", f"## {title} ({len(members)})", "",
                       "| Unit | Function (target RVA) | Evidence | Statements T/B | Cur / max | Status / previous attempt | Local differences |",
                       "|---|---|---|---|---|---|---|"])
        for row in members:
            address = (f"0x{row['target_rva']:x}" if row["target_rva"] is not None
                       else "no PDB RVA")
            statements = (f"{row['target_statements']}/{row['base_statements']}"
                          if row["target_statements"] is not None else "not recorded")
            cells = [
                row["unit"], f"{row['function']} ({address})",
                ", ".join(row["evidence"]),
                statements,
                f"{_percent(row['current'])} / {_percent(row['max'])}",
                f"{row['status']}: {row['note']}", _locals_text(row),
            ]
            output.append("| " + " | ".join(_escape(cell) for cell in cells) + " |")
    return "\n".join(output) + "\n"


def run(*, write, destination):
    roster = derive(declarations=False)
    rows, coverage = collect(roster, store.load())
    payload = {"snapshot": snapshot(), "coverage": coverage, "rows": rows}
    for section, title in SECTIONS:
        print(f"{title}: {sum(row['section'] == section for row in rows)}")
    if write:
        destination.parent.mkdir(parents=True, exist_ok=True)
        destination.write_text(render(payload), encoding="utf-8")
        paths.STRUCTURE_CAMPAIGN_JSON.parent.mkdir(parents=True, exist_ok=True)
        paths.STRUCTURE_CAMPAIGN_JSON.write_text(
            json.dumps(payload, indent=2) + "\n", encoding="utf-8")
        print(f"Wrote {destination} and {paths.STRUCTURE_CAMPAIGN_JSON}")
    return 0
