"""vostok.derive.artifacts - the inputs, loaded once, checked before use.

Everything the derivation knows comes from four generated files:

    binaries/objdiff/report.json            per-TU roster + objdiff's fuzzy %s
    binaries/rich/target/index.jsonl        the retail inventory + statements
    binaries/rich/base/index.jsonl          the same for our build
    binaries/rich/target/declarations.jsonl PDB declarations (optional)

This module owns reading them and the two freshness questions worth asking
before anyone believes a number: is the TARGET side ancient relative to the base
(a stale target smears scores across every module), and was report.json built
from the sources that are checked out now. Both only warn - a regen is a
deliberate act, and refusing here would just hide the warning behind a rerun.
"""

import json
import os
import subprocess
import sys
from dataclasses import dataclass, field

from vostok.core import symbols as normalize_objdiff_symbols
from vostok.core import tsv
from vostok.core.paths import (BASE_IDX, CROSS_UNIT_REPORT, DECLARATIONS,
                               EFFECTIVE_SYMBOL_MAP, REPORT, REPORT_HEAD,
                               SYMBOL_MAP, TARGET_IDX)
from vostok.core.paths import REPO as VOSTOK
from vostok.derive import log
from vostok.derive.index import (authoritative_demangled_names,
                                 index_by_mangled, load_index_records)
from vostok.derive.modules import (dynamic_local_owner_modules,
                                   load_module_ownership_overrides,
                                   load_source_ownership_overrides)
from vostok.derive.names import norm_name
from vostok.derive.scores import (report_fuzzy_scores, report_overload_scores,
                                  report_score_for_target)

STALE_TARGET_SECONDS = 7 * 86400


def _git(*args):
    try:
        out = subprocess.run(
            ["git", "-C", str(VOSTOK), *args], capture_output=True, text=True, timeout=10
        )
        return out.returncode, out.stdout.strip()
    except Exception:  # noqa: BLE001 - freshness reporting must never break a regen
        return 1, ""


def git_head():
    _, head = _git("rev-parse", "HEAD")
    return head or "?"


def _require_present():
    for required in (REPORT, TARGET_IDX, BASE_IDX):
        if not required.is_file():
            sys.exit(f"[derive] missing {required} - run `vostok build` / the delink first")


def _warn_if_stale():
    """Two soft warnings: an ancient target side, and a report built before the
    sources moved. Neither refuses - `refresh` is a deliberate regen-only step -
    but a silent re-derive off a stale build is how a queue dispatches work that
    is already done."""
    t_mtime, b_mtime = os.stat(TARGET_IDX).st_mtime, os.stat(BASE_IDX).st_mtime
    if b_mtime - t_mtime > STALE_TARGET_SECONDS:
        log(
            "WARNING: target index is >7 days older than base - a stale target "
            "side smears scores across modules; consider "
            "`vostok.build.generate_delink target` (see per-worktree staleness notes)"
        )
    if not REPORT_HEAD.is_file():
        return
    report_head = REPORT_HEAD.read_text().strip()
    head = report_head[: -len("+dirty")] if report_head.endswith("+dirty") else report_head
    if not head or head.startswith("?") or head == git_head():
        return
    rc, _ = _git("diff", "--quiet", head, "HEAD", "--", "sources/")
    if rc != 0:
        log("STALE: report.json is stale (sources/ moved since it was "
            "built) - run `vostok build` first; refreshing anyway")


@dataclass
class Artifacts:
    """One loaded, cross-referenced view of the four input files."""

    target: dict                  # {mangled: rich record}, retail side
    base: dict                    # {mangled: rich record}, our build
    target_records: list          # every raw record, same-RVA PDB aliases included
    base_records: list
    units_by_mangled: dict        # {mangled: [report.json TU, ...]}, sorted
    report_units: list            # every TU report.json compared, in file order
    report_fns: list              # (unit, mangled, objdiff %, target size) from report
    fuzzy: dict                   # {mangled: objdiff %}, target identities folded in
    demangled: dict               # {mangled: display name}, retail spelling wins
    dynamic_owners: dict
    module_overrides: dict
    source_overrides: dict
    folded_symbol_aliases: dict
    folded_fuzzy: dict = field(default_factory=dict)
    cross_unit_fuzzy: dict = field(default_factory=dict)
    rich_pdb_aliases: dict = field(default_factory=dict)
    declared_methods: set = field(default_factory=set)
    declared_free: set = field(default_factory=set)

    def compiler_alias(self, mangled):
        """The base-side spelling MSVC would emit for a retail PDB name."""
        return (
            self.rich_pdb_aliases.get(mangled)
            or normalize_objdiff_symbols.compiler_name(mangled)
        )

    @property
    def has_declarations(self):
        return bool(self.declared_methods or self.declared_free)


def load(declarations=True):
    """Read every input, warn about staleness, and cross-reference the two sides.

    `declarations=False` skips the 85 MB declaration dump, which answers exactly
    one question (is a base-only symbol legitimate) - so only that consumer pays
    the ~10 s it costs.
    """
    _require_present()
    _warn_if_stale()

    log("loading rich indexes ...")
    target_records = load_index_records(TARGET_IDX)
    target = index_by_mangled(target_records)
    base_records = load_index_records(BASE_IDX)
    # Pairing prefers the other side's owner and primary signature, so a static
    # helper compiled into several TUs resolves to the same record on both sides.
    target_primary_signatures = {
        mangled: rec["name"]
        for mangled, rec in target.items()
        if mangled == rec["mangled"]
    }
    target_owners = {
        (rec["mangled"], rec["name"]): rec["file"] for rec in target.values()
    }
    base = index_by_mangled(base_records, target_owners, target_primary_signatures)
    rich_pdb_aliases = normalize_objdiff_symbols.rich_pdb_aliases(
        TARGET_IDX, BASE_IDX, source_prefix="vostok/",
    )
    folded_symbol_aliases = _load_folded_symbol_aliases()
    log(f"  target: {len(target)} functions, base: {len(base)} functions")
    if rich_pdb_aliases:
        log(f"  Vostok rich-PDB aliases: {len(rich_pdb_aliases)}")
    if folded_symbol_aliases:
        log(f"  target folded-symbol aliases: {len(folded_symbol_aliases)}")

    artifacts = Artifacts(
        target=target,
        base=base,
        target_records=target_records,
        base_records=base_records,
        units_by_mangled={},
        report_units=[],
        report_fns=[],
        fuzzy={},
        demangled=authoritative_demangled_names(target, base),
        dynamic_owners=dynamic_local_owner_modules(target_records),
        module_overrides=load_module_ownership_overrides(),
        source_overrides=load_source_ownership_overrides(),
        folded_symbol_aliases=folded_symbol_aliases,
        rich_pdb_aliases=rich_pdb_aliases,
    )
    _load_report(artifacts)
    if declarations:
        _load_declarations(artifacts)
    return artifacts


def _load_folded_symbol_aliases():
    """Read the identity map used to normalize highly-COMDAT target objects.

    Report functions carry the delinker's selected COFF representative, while
    the rich target index retains the retail PDB identity.  Keep that exact
    generated mapping available so an unpaired target identity can recover its
    measured report score before the same-signature rich alias pass selects the
    corresponding base record.
    """
    path = EFFECTIVE_SYMBOL_MAP if EFFECTIVE_SYMBOL_MAP.is_file() else SYMBOL_MAP
    if not path.is_file():
        return {}
    aliases = {}
    for line_number, fields in tsv.read(path):
        if len(fields) != 2 or not all(fields):
            raise RuntimeError(
                f"{path}:{line_number}: expected <alias>\t<target choice>"
            )
        alias, choice = fields
        previous = aliases.setdefault(alias, choice)
        if previous != choice:
            raise RuntimeError(
                f"{path}:{line_number}: conflicting choices for {alias!r}: "
                f"{previous!r} vs {choice!r}"
            )
    return aliases


def _load_report(artifacts):
    """report.json's per-unit roster, reduced to one score per identity.

    The disposable target COFF tree normalizes safe retail PDB backtick names to
    MSVC's ??__E/??__F spelling so objdiff can pair them with candidate objects.
    The rich indexes retain the authoritative PDB names, so every normalized
    score is reflected back onto that identity here, before anything reads it.
    """
    log("loading report.json ...")
    report = json.loads(REPORT.read_text())
    scores, artifacts.units_by_mangled = report_fuzzy_scores(report)
    for unit in report["units"]:
        if not unit["functions"]:
            continue
        artifacts.report_units.append(unit["name"])
        for fn in unit["functions"]:
            artifacts.report_fns.append(
                (
                    unit["name"],
                    fn["name"],
                    fn.get("fuzzy_match_percent"),
                    int(fn.get("size") or 0),
                )
            )

    fuzzy = dict(scores)
    overload_scores = report_overload_scores(artifacts.target, report)
    fuzzy.update(overload_scores)
    for mangled in overload_scores:
        artifacts.units_by_mangled[mangled] = [artifacts.target[mangled]["file"]]
    if overload_scores:
        log(f"attributed {len(overload_scores)} unit-scoped PDB overload score(s)")
    if CROSS_UNIT_REPORT.is_file():
        cross = json.loads(CROSS_UNIT_REPORT.read_text())
        cross_unit_fuzzy = {}
        n = 0
        for function in cross.get("functions", []):
            score = function.get("fuzzy_match_percent")
            if score is not None:
                mangled = function["name"]
                previous = cross_unit_fuzzy.get(mangled)
                cross_unit_fuzzy[mangled] = (
                    score if previous is None else max(previous, score)
                )
                fuzzy.setdefault(mangled, score)
                n += 1
        artifacts.cross_unit_fuzzy = cross_unit_fuzzy
        if n:
            log(f"loaded {n} cross-unit COMDAT scores")
        exact_units = set(cross.get("exact_units", []))
        exact_functions = 0
        for mangled, record in artifacts.target.items():
            if record.get("file") not in exact_units or fuzzy.get(mangled) == 100.0:
                continue
            fuzzy[mangled] = 100.0
            exact_functions += 1
        if exact_functions:
            log(
                "attributed "
                f"{exact_functions} score gap(s) from exact source-object units"
            )

    for mangled in artifacts.target:
        score = report_score_for_target(mangled, fuzzy)
        if score is not None:
            fuzzy[mangled] = score
        compiler = artifacts.compiler_alias(mangled)
        if compiler in fuzzy and mangled not in fuzzy:
            fuzzy[mangled] = fuzzy[compiler]

    # Do not merge these representative scores into the ordinary report map:
    # strict same-name and same-signature rich evidence must pair first.  A
    # separate late pairing pass consumes this weaker evidence only for target
    # identities that remain unclaimed after those function-scoped proofs.
    artifacts.folded_fuzzy = {}
    for mangled in set(artifacts.target) - set(artifacts.base):
        if report_score_for_target(mangled, fuzzy) is not None:
            continue
        score = report_score_for_target(
            mangled,
            fuzzy,
            artifacts.folded_symbol_aliases,
        )
        if score is not None:
            artifacts.folded_fuzzy[mangled] = score
    if artifacts.folded_fuzzy:
        log(
            f"made {len(artifacts.folded_fuzzy)} report score gap(s) "
            "available through the target folded-symbol map"
        )
    artifacts.fuzzy = fuzzy


def _load_declarations(artifacts):
    """PDB declaration records, read TRANSIENTLY (222k rows / 85 MB).

    They exist to answer one question - was this base-only symbol ever DECLARED
    in the retail PDB - so only the per-function verdict outlives the load.
    """
    if not DECLARATIONS.is_file():
        log("no declarations.jsonl - BASE_ONLY legitimacy check degraded (parser dump pending)")
        return
    log("loading declaration records (transient) ...")
    with open(DECLARATIONS, encoding="utf-8") as f:
        for line in f:
            declaration = json.loads(line)
            if declaration.get("class"):
                artifacts.declared_methods.add(
                    (norm_name(declaration["class"]), norm_name(declaration["name"]))
                )
            else:
                artifacts.declared_free.add(norm_name(declaration["name"]))
