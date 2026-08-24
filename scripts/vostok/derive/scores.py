"""vostok.derive.scores - what objdiff measured, per function.

report.json is per-UNIT; these read a function's score out of it, including the
two cases a naive lookup gets wrong: a symbol scored in several units (take the
best) and the cross-unit COMDAT report, which is the only place an island's
exact score appears.
"""

import hashlib

from vostok.core import symbols as normalize_objdiff_symbols


def report_fuzzy_scores(report):
    """Return the best measured objdiff score for each decorated name."""
    scores = {}
    units_by_mangled = {}
    for unit in report["units"]:
        uname = unit["name"]
        for fn in unit["functions"]:
            mangled = fn["name"]
            units_by_mangled.setdefault(mangled, []).append(uname)
            fuzzy = fn.get("fuzzy_match_percent")
            if fuzzy is None:
                continue
            previous = scores.get(mangled)
            scores[mangled] = fuzzy if previous is None else max(previous, fuzzy)
    for units in units_by_mangled.values():
        units.sort()
    return scores, units_by_mangled


def report_overload_scores(indexed_target, report):
    """Recover per-unit scores for stable PDB overload identities.

    Some retail PDB procedures expose only a scope-qualified placeholder as
    their mangled identity. ``index_by_mangled`` keeps the distinct signatures
    under stable overload keys, while objdiff still reports the shared raw
    spelling. Attribute a report row only when its source unit names exactly
    one such target overload; same-file ambiguity remains unpaired.
    """
    candidates = {}
    for key, record in indexed_target.items():
        identity = record["file"], record["mangled"]
        candidates.setdefault(identity, []).append(key)

    report_rows = {}
    for unit in report["units"]:
        for function in unit["functions"]:
            identity = unit["name"], function["name"]
            report_rows.setdefault(identity, []).append(
                function.get("fuzzy_match_percent")
            )

    scores = {}
    for identity, keys in candidates.items():
        rows = report_rows.get(identity, [])
        if len(keys) != 1 or len(rows) != 1 or rows[0] is None:
            continue
        key = keys[0]
        if key != indexed_target[key]["mangled"]:
            scores[key] = rows[0]
    return scores


def report_score_for_target(mangled, scores, folded_aliases=None):
    """Map objdiff's normalized spellings back to the retail PDB identity.

    ``folded_aliases`` is the delinker's generated target-identity -> selected
    COFF-representative map.  Callers admit it only for identities absent from
    the base rich index, after same-name function-scoped evidence has had first
    refusal.
    """
    fuzzy = scores.get(mangled)
    compiler = normalize_objdiff_symbols.compiler_name(mangled)
    if fuzzy is None and compiler:
        fuzzy = scores.get(compiler)
    if fuzzy is None and mangled.startswith("??_G"):
        fuzzy = scores.get(f"??_E{mangled[4:]}")
    if fuzzy is None and folded_aliases:
        folded = folded_aliases.get(mangled)
        if folded:
            fuzzy = scores.get(folded)
    return fuzzy


def cross_unit_exact_score(mangled, scores):
    """Return strict object-level exact evidence for a target PDB identity."""
    fuzzy = report_score_for_target(mangled, scores)
    return fuzzy if fuzzy is not None and fuzzy >= 99.995 else None


def island_report_score(expected, mangled, scores):
    """Read a manifested report identity, defaulting to the target spelling."""
    return report_score_for_target(expected.get("report_mangled", mangled), scores)


def sha256_file(path):
    digest = hashlib.sha256()
    with open(path, "rb") as source:
        for chunk in iter(lambda: source.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def rank_island_delta(
    candidate_fuzzy,
    candidate_hash,
    previous=None,
    current_fuzzy=None,
):
    """Return a prospective MAX delta, rejecting a different source epoch.

    ``previous`` is a source_maxima row (or any mapping with the same fields).
    When correctness-facing MAX evidence does not yet exist, the canonical
    physical score is the floor. Target-only symbols therefore start at zero
    instead of disappearing from island discovery.
    """
    previous_fuzzy = previous["max_fuzzy_pct"] if previous is not None else None
    previous_exact = previous["exact_proven"] if previous is not None else 0
    baseline = previous_fuzzy
    if baseline is None:
        baseline = current_fuzzy if current_fuzzy is not None else 0.0
    exact = int(candidate_fuzzy >= 99.995)
    improves = (
        candidate_fuzzy > baseline + 0.000001
        or exact > previous_exact
    )
    if not improves:
        return None
    if not candidate_hash:
        raise ValueError("candidate has no effective source hash")
    if previous is not None and previous["effective_hash"] != candidate_hash:
        raise ValueError(
            "canonical source epoch disagrees: "
            f"{previous['effective_hash']} != {candidate_hash}"
        )
    return {
        "previous_fuzzy_pct": previous_fuzzy,
        "baseline_fuzzy_pct": baseline,
        "gain_pct": candidate_fuzzy - baseline,
        "exact_proven": max(previous_exact, exact),
    }
