"""vostok.derive.roster - the whole derivation, in one pass.

`regen()` rebuilds every derived table from artifacts that already exist:
report.json for the per-TU roster and fuzzy %s, the two rich indexes for the
target/base inventories and their statements. It pairs the two sides (through
`aliases` where the spellings differ), classifies each pair's structure, decides
logical module ownership, folds the observation into the hash-scoped maxima, and
finally projects the result into the committed ledger.

It does NOT compile anything: rebuild.py is the canonical build and calls this
at the end; `refresh` is the regen-only path for an already-built report.
"""

import json
import os
import sqlite3
import sys
from pathlib import Path

from vostok.core import symbols as normalize_objdiff_symbols
from vostok.ledger import store as match_state
from vostok.core.paths import (BASE_IDX, CROSS_UNIT_REPORT, DECLARATIONS,
                               MATCH_DB as DB_PATH, REPORT, TARGET_IDX)
from vostok.core.paths import REPO as VOSTOK

from vostok.derive import log
from vostok.derive.aliases import (dyn_canon_base, dyn_canon_rich, dyn_owner_compatible, dynamic_pair_score, exact_paired_source_alias, instruction_stream_exact, load_exact_fold_aliases, report_source_alias_candidates, strict_source_alias_candidates)
from vostok.derive.classify import classify
from vostok.derive.db import (Interner, SCHEMA, SCHEMA_VERSION, _git, file_mtime_iso, git_head, open_db)
from vostok.derive.index import (authoritative_demangled_names, index_by_mangled, is_framed, legacy_overload_keys, load_index_records)
from vostok.derive.maxima import (_source_extent, compiled_state_id, effective_source_hash, maximum_for_effective_hash, maximum_needs_epoch_archive, merge_maximum_epoch, retained_max_effective_hash, src_fingerprint)
from vostok.derive.modules import (dynamic_local_owner_modules, load_module_ownership_overrides, logical_module, module_of)
from vostok.derive.names import mangled_name_part, norm_name, qualified_name
from vostok.derive.scores import report_fuzzy_scores, report_score_for_target


def cmd_refresh(args):
    regen()


def regen():
    """Regenerate match.db from the already-built diff artifacts (report.json +
    rich indexes). This is REGEN-ONLY: it does NOT run rebuild.py. rebuild.py is
    the canonical build step and calls this at the end of its run; invoke
    `match_db.py refresh` by hand only to re-derive the DB from an artifact set
    that is already on disk (run rebuild.py first if sources moved)."""
    for required in (REPORT, TARGET_IDX, BASE_IDX):
        if not required.is_file():
            sys.exit(f"[match_db] missing {required} - run rebuild.py / the delink first")

    # stale-target guard: the target index should not predate the base one by ages
    t_mtime, b_mtime = os.stat(TARGET_IDX).st_mtime, os.stat(BASE_IDX).st_mtime
    if b_mtime - t_mtime > 7 * 86400:
        log(
            "WARNING: target index is >7 days older than base - a stale target "
            "side smears scores across modules; consider "
            "`generate_delink.py target` (see per-worktree staleness notes)"
        )

    # soft staleness note: refresh is a deliberate regen-only step, so it proceeds
    # even when report.json predates the current sources - but warn so a hand-run
    # refresh doesn't silently re-derive the DB from a stale build (the same
    # build-leg the queue staleness_check enforces; here we only warn).
    head_marker = REPORT.parent / "report.head"
    if head_marker.is_file():
        report_head = head_marker.read_text().strip()
        bh = report_head[: -len("+dirty")] if report_head.endswith("+dirty") else report_head
        if bh and not bh.startswith("?") and bh != git_head():
            rc, _ = _git("diff", "--quiet", bh, "HEAD", "--", "sources/")
            if rc != 0:
                log("STALE: report.json is stale (sources/ moved since it was "
                    "built) - run rebuild.py first; refreshing anyway")

    log("loading rich indexes ...")
    target_records = load_index_records(TARGET_IDX)
    target = index_by_mangled(target_records)
    base_records = load_index_records(BASE_IDX)
    target_primary_signatures = {
        mangled: rec["name"]
        for mangled, rec in target.items()
        if mangled == rec["mangled"]
    }
    target_owners = {
        (rec["mangled"], rec["name"]): rec["file"] for rec in target.values()
    }
    base = index_by_mangled(
        base_records,
        target_owners,
        target_primary_signatures,
    )
    dynamic_owners = dynamic_local_owner_modules(target_records)
    module_overrides = load_module_ownership_overrides()
    rich_pdb_aliases = normalize_objdiff_symbols.rich_pdb_aliases(
        TARGET_IDX,
        BASE_IDX,
        source_prefix="vostok/render/",
    )

    def compiler_alias(mangled):
        return (
            rich_pdb_aliases.get(mangled)
            or normalize_objdiff_symbols.compiler_name(mangled)
        )

    log(f"  target: {len(target)} functions, base: {len(base)} functions")
    if rich_pdb_aliases:
        log(f"  render rich-PDB aliases: {len(rich_pdb_aliases)}")

    log("loading report.json ...")
    report = json.loads(REPORT.read_text())
    report_scores, units_by_mangled = report_fuzzy_scores(report)
    report_fns = []  # (unit, mangled, fuzzy)
    for unit in report["units"]:
        uname = unit["name"]
        for fn in unit["functions"]:
            report_fns.append((uname, fn["name"], fn.get("fuzzy_match_percent")))

    cross_unit_fuzzy = {}
    if CROSS_UNIT_REPORT.is_file():
        cross_report = json.loads(CROSS_UNIT_REPORT.read_text())
        for function in cross_report.get("functions", []):
            fuzzy = function.get("fuzzy_match_percent")
            if fuzzy is not None:
                cross_unit_fuzzy[function["name"]] = fuzzy
        if cross_unit_fuzzy:
            log(f"loaded {len(cross_unit_fuzzy)} cross-unit COMDAT scores")

    # interners: collect every name, then assign ids in sorted order
    syms, units_i, files_i = Interner(), Interner(), Interner()
    demangled_by_mangled = authoritative_demangled_names(target, base)
    for side in (target, base):
        for mangled, rec in side.items():
            syms(mangled)
            files_i(rec["file"])
    for uname, mangled, _f in report_fns:
        syms(mangled)
        units_i(uname)
        demangled_by_mangled.setdefault(mangled, None)
    for rec in list(target.values()) + list(base.values()):
        units = units_by_mangled.get(rec["mangled"])
        unit = rec["file"] if units and rec["file"] in units else (units[0] if units else None)
        if unit:
            units_i(unit)
    sym_id = syms.freeze()
    unit_id = units_i.freeze()
    file_id = files_i.freeze()

    def side_rows(side):
        rows = []
        for mangled, rec in side.items():
            units = units_by_mangled.get(mangled)
            unit = rec["file"] if units and rec["file"] in units else (units[0] if units else None)
            stmts = rec["statements"]
            line = min((s["line"] for s in stmts if s.get("line")), default=None)
            rows.append(
                (
                    rec["rva"],
                    sym_id[mangled],
                    unit_id.get(unit),
                    file_id[rec["file"]],
                    logical_module(
                        mangled, rec, units, dynamic_owners, module_overrides
                    ),
                    line,
                    rec["size"],
                    len(stmts),
                    0 if is_framed(rec) else 1,
                )
            )
        rows.sort()
        return rows

    log("classifying structure for paired functions ...")
    fuzzy_by_mangled = dict(report_scores)
    for mangled, fuzzy in cross_unit_fuzzy.items():
        fuzzy_by_mangled.setdefault(mangled, fuzzy)
    # The disposable target COFF tree normalizes safe retail PDB backtick names
    # to MSVC's ??__E/??__F spelling so objdiff can pair them with candidate
    # objects. The rich indexes retain the authoritative PDB names; reflect each
    # normalized report score back onto that identity before building pair rows.
    for mangled in target:
        fuzzy = report_score_for_target(mangled, fuzzy_by_mangled)
        if fuzzy is not None:
            fuzzy_by_mangled[mangled] = fuzzy
        compiler = compiler_alias(mangled)
        if compiler in fuzzy_by_mangled and mangled not in fuzzy_by_mangled:
            fuzzy_by_mangled[mangled] = fuzzy_by_mangled[compiler]
    pair_rows = []
    rich_exact = 0
    for mangled in sorted(set(target) & set(base)):
        cls, t_n, b_n, n_size, n_tonly, n_bonly = classify(target[mangled], base[mangled])
        fuzzy = fuzzy_by_mangled.get(mangled)
        if fuzzy is None and instruction_stream_exact(target[mangled], base[mangled]):
            fuzzy = 100.0
            rich_exact += 1
        pair_rows.append(
            (
                sym_id[mangled],
                target[mangled]["rva"],
                base[mangled]["rva"],
                fuzzy,
                cls,
                t_n,
                b_n,
                n_size,
                n_tonly,
                n_bonly,
            )
        )
    if rich_exact:
        log(
            f"function-scoped rich-index exact attribution recovered "
            f"{rich_exact} objdiff score gap(s)"
        )
    # Cross-name pairing first recovers PDB aliases whose base rich-index record
    # carries another folded symbol's mangled name.  The delink report proves
    # that the target name was emitted and compared; additionally require one
    # free base RVA, an exact demangled signature, and compatible source ownership.
    paired_primary = set(target) & set(base)
    cross_paired_mangled = set()  # base AND target names paired across a name gap
    used_target_rvas = {row[1] for row in pair_rows}
    used_base_rvas = {row[2] for row in pair_rows}
    n_compiler_alias = 0
    for tm in sorted(set(target) - paired_primary):
        bm = compiler_alias(tm)
        if (
            bm not in base
            or tm not in fuzzy_by_mangled
            or target[tm]["rva"] in used_target_rvas
            or base[bm]["rva"] in used_base_rvas
        ):
            continue
        cls, t_n, b_n, n_size, n_tonly, n_bonly = classify(target[tm], base[bm])
        pair_rows.append(
            (
                sym_id[tm], target[tm]["rva"], base[bm]["rva"],
                fuzzy_by_mangled[tm], cls, t_n, b_n, n_size, n_tonly, n_bonly,
            )
        )
        used_target_rvas.add(target[tm]["rva"])
        used_base_rvas.add(base[bm]["rva"])
        cross_paired_mangled.update((tm, bm))
        n_compiler_alias += 1
    if n_compiler_alias:
        log(f"cross-name paired {n_compiler_alias} exact compiler aliases")

    target_aliases_by_name = {}
    for rec in target_records:
        target_aliases_by_name.setdefault(rec["name"], {})[rec["rva"]] = rec
    base_aliases_by_name = {}
    for rec in base_records:
        base_aliases_by_name.setdefault(rec["name"], {})[rec["rva"]] = rec
    exact_fold_aliases = load_exact_fold_aliases()
    base_aliases_by_mangled = {}
    for rec in base_records:
        base_aliases_by_mangled.setdefault(rec["mangled"], {})[rec["rva"]] = rec
    target_alias_names_by_rva = {}
    for rec in target_records:
        target_alias_names_by_rva.setdefault(rec["rva"], set()).add(rec["name"])
    base_alias_names_by_rva = {}
    for rec in base_records:
        base_alias_names_by_rva.setdefault(rec["rva"], set()).add(rec["name"])
    n_alias = 0
    for tm in sorted(set(target) - paired_primary):
        if tm not in fuzzy_by_mangled:
            continue
        trec = target[tm]
        candidates = report_source_alias_candidates(
            trec,
            base_aliases_by_name,
            used_base_rvas,
            target_alias_names_by_rva=target_alias_names_by_rva,
            base_alias_names_by_rva=base_alias_names_by_rva,
        )
        if trec["rva"] in used_target_rvas or len(candidates) != 1:
            continue
        brec = candidates[0]
        bm = brec["mangled"]
        cls, t_n, b_n, n_size, n_tonly, n_bonly = classify(trec, brec)
        pair_rows.append(
            (
                sym_id[tm], trec["rva"], brec["rva"], fuzzy_by_mangled[tm], cls,
                t_n, b_n, n_size, n_tonly, n_bonly,
            )
        )
        used_target_rvas.add(trec["rva"])
        used_base_rvas.add(brec["rva"])
        cross_paired_mangled.update((tm, bm))
        n_alias += 1
    if n_alias:
        log(f"cross-name paired {n_alias} report-grounded folded PDB aliases")

    # Some highly-COMDAT functions never reach objdiff because target and base
    # choose different canonical mangled names for the same folded RVA.  Recover
    # only unique, same-source aliases whose complete rich instruction streams
    # are identical; unlike the report-grounded pass above, this also covers
    # target-only score gaps.
    n_rich_alias = 0
    for tm in sorted(set(target) - paired_primary - cross_paired_mangled):
        trec = target[tm]
        if trec["rva"] in used_target_rvas:
            continue
        candidates = strict_source_alias_candidates(
            trec,
            base_aliases_by_name,
            used_base_rvas,
            target_alias_names_by_rva=target_alias_names_by_rva,
            base_alias_names_by_rva=base_alias_names_by_rva,
            exact_fold_aliases=exact_fold_aliases,
            base_aliases_by_mangled=base_aliases_by_mangled,
        )
        if len(candidates) != 1:
            continue
        brec = candidates[0]
        bm = brec["mangled"]
        cls, t_n, b_n, n_size, n_tonly, n_bonly = classify(trec, brec)
        pair_rows.append(
            (
                sym_id[tm], trec["rva"], brec["rva"], 100.0, cls,
                t_n, b_n, n_size, n_tonly, n_bonly,
            )
        )
        used_target_rvas.add(trec["rva"])
        used_base_rvas.add(brec["rva"])
        cross_paired_mangled.update((tm, bm))
        n_rich_alias += 1
    if n_rich_alias:
        log(f"cross-name paired {n_rich_alias} strict same-source rich aliases")

    # A single ICF-selected body can legitimately own several PDB aliases.  If
    # the base RVA was already consumed by another target symbol, retain a
    # second target pairing only when the base PDB still records this exact
    # demangled signature in the same source file and the normalized rich
    # instruction stream is byte-exact.  This recovers genuine header islands
    # without treating an arbitrary same-body fold as source ownership.
    n_shared_rich_alias = 0
    for tm in sorted(set(target) - paired_primary - cross_paired_mangled):
        trec = target[tm]
        if trec["rva"] in used_target_rvas:
            continue
        candidates = strict_source_alias_candidates(
            trec,
            base_aliases_by_name,
            used_base_rvas,
            allow_used=True,
            target_alias_names_by_rva=target_alias_names_by_rva,
            base_alias_names_by_rva=base_alias_names_by_rva,
            exact_fold_aliases=exact_fold_aliases,
            base_aliases_by_mangled=base_aliases_by_mangled,
        )
        if len(candidates) != 1 or candidates[0]["rva"] not in used_base_rvas:
            continue
        brec = candidates[0]
        bm = brec["mangled"]
        cls, t_n, b_n, n_size, n_tonly, n_bonly = classify(trec, brec)
        pair_rows.append(
            (
                sym_id[tm], trec["rva"], brec["rva"], 100.0, cls,
                t_n, b_n, n_size, n_tonly, n_bonly,
            )
        )
        used_target_rvas.add(trec["rva"])
        cross_paired_mangled.update((tm, bm))
        n_shared_rich_alias += 1
    if n_shared_rich_alias:
        log(
            "cross-name paired "
            f"{n_shared_rich_alias} shared-RVA same-source rich aliases"
        )

    # Then pair dynamic-init/atexit thunks across their several rich/raw name
    # spellings. The canonical identity must be unique on each side; emitted
    # size may legitimately differ and is what the fuzzy matcher measures.
    t_canon, b_canon = {}, {}     # (kind, fqn) -> [mangled, ...]
    for m in set(target) - paired_primary:
        c = dyn_canon_rich(m)
        if c:
            t_canon.setdefault(c, []).append(m)
    for m in set(base) - paired_primary:
        c = dyn_canon_base(m)
        if c:
            b_canon.setdefault(c, []).append(m)
    n_cross = 0
    for c in t_canon.keys() & b_canon.keys():
        tm_list, bm_list = t_canon[c], b_canon[c]
        if len(tm_list) != 1 or len(bm_list) != 1:
            continue  # ambiguous - leave for the Rust-side demangler
        tm, bm = tm_list[0], bm_list[0]
        if target[tm]["rva"] in used_target_rvas or base[bm]["rva"] in used_base_rvas:
            continue
        if not dyn_owner_compatible(target[tm], base[bm], c):
            continue  # same-named statics in different known owners are not proven identical
        cls, t_n, b_n, n_size, n_tonly, n_bonly = classify(target[tm], base[bm])
        pair_rows.append(
            (
                sym_id[tm],
                target[tm]["rva"],
                base[bm]["rva"],
                dynamic_pair_score(
                    tm, bm, target[tm], base[bm], fuzzy_by_mangled
                ),
                cls,
                t_n,
                b_n,
                n_size,
                n_tonly,
                n_bonly,
            )
        )
        cross_paired_mangled.update((tm, bm))
        used_target_rvas.add(target[tm]["rva"])
        used_base_rvas.add(base[bm]["rva"])
        n_cross += 1
    if n_cross:
        log(f"cross-name paired {n_cross} dynamic-init/atexit thunks (??__E/??__F <-> demangled)")

    # The retail and candidate PDBs render a small audited set of template type
    # spellings differently (for example, an explicit ``enum`` keyword).  The
    # disposable target COFF normalizer gives objdiff the candidate-side name;
    # pair the unchanged rich-index identities through that exact same map.
    n_alias = 0
    for tm in sorted(set(target) - paired_primary - cross_paired_mangled):
        bm = normalize_objdiff_symbols.pdb_alias_name(tm)
        if not bm or bm not in base:
            continue
        if bm in paired_primary or bm in cross_paired_mangled:
            continue
        if target[tm]["file"] != base[bm]["file"]:
            continue
        cls, t_n, b_n, n_size, n_tonly, n_bonly = classify(target[tm], base[bm])
        pair_rows.append(
            (
                sym_id[tm],
                target[tm]["rva"],
                base[bm]["rva"],
                fuzzy_by_mangled.get(tm, fuzzy_by_mangled.get(bm)),
                cls,
                t_n,
                b_n,
                n_size,
                n_tonly,
                n_bonly,
            )
        )
        cross_paired_mangled.update((tm, bm))
        n_alias += 1
    if n_alias:
        log(f"cross-name paired {n_alias} exact PDB spelling aliases")
    pair_rows.sort()

    unit_rows = sorted(
        {
            (unit_id[u], sym_id[m]): f
            for u, m, f in sorted(report_fns, key=lambda r: (r[0], r[1], r[2] is not None, r[2] or 0))
        }.items()
    )
    unit_rows = [(u, s, f) for (u, s), f in unit_rows]

    # declaration records: loaded TRANSIENTLY (222k rows / 85MB - never stored
    # in the committed DB; only the per-function base_only_status verdict is)
    declared_methods, declared_free = set(), set()
    if DECLARATIONS.is_file():
        log("loading declaration records (transient) ...")
        with open(DECLARATIONS, encoding="utf-8") as f:
            for line in f:
                d = json.loads(line)
                if d.get("class"):
                    declared_methods.add((norm_name(d["class"]), norm_name(d["name"])))
                else:
                    declared_free.add(norm_name(d["name"]))
    else:
        log("no declarations.jsonl - BASE_ONLY legitimacy check degraded (parser dump pending)")

    # carry persistent tables forward from the existing DB
    old_history, old_maxima, old_maxima_epochs = {}, {}, {}
    old_flags, old_attempts = [], []
    if not DB_PATH.is_file() and Path(match_state.STATE_PATH).is_file():
        # no cache (fresh clone / wiped binaries): the committed ledger holds
        # the only copy of the campaign's memory, so start from it
        old_history, old_maxima, old_attempts_t, old_flags_t = (
            match_state.seed_db_tables()
        )
        old_attempts = list(old_attempts_t)
        old_flags = list(old_flags_t)
        log(f"seeded persistent state from {Path(match_state.STATE_PATH).name}: "
            f"{len(old_history)} history / {len(old_maxima)} maxima / "
            f"{len(old_flags)} flags")
    if DB_PATH.is_file():
        old = open_db()
        old_history = {r["mangled"]: tuple(r) for r in old.execute("SELECT * FROM history")}
        try:
            old_maxima = {
                r["mangled"]: tuple(r)
                for r in old.execute("SELECT * FROM source_maxima")
            }
        except sqlite3.OperationalError:
            old_maxima = {}  # schema 3: start MAX from current observations
        try:
            old_maxima_epochs = {
                (r["mangled"], r["effective_hash"]): tuple(r)
                for r in old.execute("SELECT * FROM source_maxima_epochs")
            }
        except sqlite3.OperationalError:
            old_maxima_epochs = {}  # schema 4: seed from valuable active rows
        old_current_fuzzy = {
            r["mangled"]: r["fuzzy_pct"]
            for r in old.execute("SELECT mangled, fuzzy_pct FROM paired")
        }
        for mangled, row in old_maxima.items():
            if not maximum_needs_epoch_archive(
                row, old_current_fuzzy.get(mangled)
            ):
                continue
            key = (mangled, row[1])
            old_maxima_epochs[key] = merge_maximum_epoch(
                old_maxima_epochs.get(key), row
            )
        old_flags = [tuple(r) for r in old.execute("SELECT * FROM flags ORDER BY mangled, flag")]
        try:
            old_attempts = [
                tuple(r) for r in old.execute("SELECT * FROM attempts ORDER BY mangled")
            ]
        except sqlite3.OperationalError:
            old_attempts = []  # pre-schema-3 DB
        old.close()

    # Schema 4 originally disambiguated placeholder overloads with target RVAs.
    # Carry persistent evidence onto the signature-derived keys introduced here.
    legacy_aliases = legacy_overload_keys(target_records)
    old_history = {
        legacy_aliases.get(key, key): (legacy_aliases.get(key, key), *row[1:])
        for key, row in old_history.items()
    }
    old_maxima = {
        legacy_aliases.get(key, key): (legacy_aliases.get(key, key), *row[1:])
        for key, row in old_maxima.items()
    }
    remapped_maxima_epochs = {}
    for (key, effective_hash), row in old_maxima_epochs.items():
        mapped = legacy_aliases.get(key, key)
        candidate = (mapped, *row[1:])
        epoch_key = (mapped, effective_hash)
        remapped_maxima_epochs[epoch_key] = merge_maximum_epoch(
            remapped_maxima_epochs.get(epoch_key), candidate
        )
    old_maxima_epochs = remapped_maxima_epochs
    old_flags = [
        (legacy_aliases.get(row[0], row[0]), *row[1:]) for row in old_flags
    ]
    old_attempts = [
        (legacy_aliases.get(row[0], row[0]), *row[1:]) for row in old_attempts
    ]

    # reconcile history: upsert every CURRENT pairing; reset rows whose source
    # extent changed ("touched" - requirement: the seen-flag dies on any edit)
    paired_at = file_mtime_iso(BASE_IDX)  # artifact time, not wall clock (determinism)
    sym_by_id = {}
    for mangled, i in sym_id.items():
        sym_by_id[i] = mangled
    touched = dropped = 0
    # paired = primary same-name pairs PLUS the cross-name dynamic-init/atexit pairs
    # (so a cross-paired base ??__E / target thunk is not re-classified as *_only).
    paired_mangled = paired_primary | cross_paired_mangled
    # cross-name pairs are keyed by the TARGET name, which is absent from `base`;
    # fall back to that pair's matched base record (located by base RVA) for the
    # source fingerprint.
    base_by_rva = {rec["rva"]: rec for rec in base.values()}
    base_rec_for = {
        sym_by_id[r[0]]: base_by_rva.get(r[2])
        for r in pair_rows
        if sym_by_id[r[0]] not in base
    }
    for row in pair_rows:
        mangled = sym_by_id[row[0]]
        fuzzy, cls = row[3], row[4]
        brec = base.get(mangled) or base_rec_for.get(mangled)
        fp = src_fingerprint(brec)
        prev = old_history.get(mangled)
        best = fuzzy
        if prev is not None and prev[5] == fp and prev[2] is not None:
            best = prev[2] if fuzzy is None else max(prev[2], fuzzy)
        elif prev is not None and prev[5] != fp:
            touched += 1  # source edited: history restarts at the current state
        old_history[mangled] = (mangled, paired_at, best, fuzzy, cls, fp)
    # rows for functions that are NOT currently paired: keep only while their
    # source extent is unchanged - a touched-then-vanished function re-queues
    for mangled in list(old_history):
        if mangled in base and mangled not in paired_mangled:
            if old_history[mangled][5] != src_fingerprint(base[mangled]):
                del old_history[mangled]
                dropped += 1
    history_rows = sorted(old_history.values())
    if touched or dropped:
        log(f"history: {touched} touched (reset), {dropped} touched-and-vanished (re-queued)")

    # Correctness-facing MAX is separate from ordinary `history`: only
    # observations in the same effective-source epoch accumulate. A first
    # A first schema refresh seeds it from the current build, never from best-seen.
    maxima_rows = {}
    current_fuzzy_by_mangled = {}
    old_epoch_symbols = {key[0] for key in old_maxima_epochs}
    maxima_reset = maxima_raised = 0
    for row in pair_rows:
        mangled = sym_by_id[row[0]]
        fuzzy, cls = row[3], row[4]
        if fuzzy is None:
            continue
        brec = base.get(mangled) or base_rec_for.get(mangled)
        extent = _source_extent(brec)
        if extent is None:
            continue
        source_file, lo, hi, _text = extent
        units = units_by_mangled.get(mangled)
        if not units and brec is not None:
            units = units_by_mangled.get(brec["mangled"])
        module = logical_module(
            mangled, brec, units, dynamic_owners, module_overrides
        )
        effective_hash = effective_source_hash(brec, module)
        state_id = compiled_state_id(brec)
        current_exact = int(fuzzy >= 99.995)
        current_fuzzy_by_mangled[mangled] = fuzzy
        active_previous = old_maxima.get(mangled)
        previous = maximum_for_effective_hash(
            mangled, effective_hash, old_maxima, old_maxima_epochs
        )

        maximum = fuzzy
        exact_proven = current_exact
        origin, evidence = "rebuild", None
        if previous is not None and previous[1] == effective_hash:
            maximum = max(previous[2], fuzzy)
            exact_proven = int(bool(previous[3]) or current_exact)
            current_improves = maximum > previous[2] or exact_proven > previous[3]
            if not current_improves:
                state_id, origin, evidence = previous[4], previous[9], previous[10]
            if current_improves:
                maxima_raised += 1
        elif active_previous is not None or mangled in old_epoch_symbols:
            maxima_reset += 1

        maxima_rows[mangled] = (
            mangled, effective_hash, maximum, exact_proven, state_id, module,
            source_file, lo, hi, origin, evidence,
        )

    # A same-hash maximum remains valid when LTCG/ICF makes the function
    # temporarily disappear. Re-hash the retained source locator before keeping
    # it; edits or context changes retire the old epoch.
    retained_candidates = list(old_maxima.values()) + list(old_maxima_epochs.values())
    for previous in retained_candidates:
        mangled = previous[0]
        if mangled in maxima_rows:
            continue
        brec = base.get(mangled)
        module, source_file, lo, hi = previous[5:9]
        effective_hash = retained_max_effective_hash(previous, brec)
        if effective_hash == previous[1]:
            candidate = (
                *previous[:5], module, source_file, lo, hi, previous[9], previous[10]
            )
            maxima_rows[mangled] = merge_maximum_epoch(
                maxima_rows.get(mangled), candidate
            )
    maxima_rows = sorted(maxima_rows.values())
    for row in maxima_rows:
        if not maximum_needs_epoch_archive(
            row, current_fuzzy_by_mangled.get(row[0])
        ):
            continue
        key = (row[0], row[1])
        old_maxima_epochs[key] = merge_maximum_epoch(
            old_maxima_epochs.get(key), row
        )
    maxima_epoch_rows = sorted(old_maxima_epochs.values())
    if maxima_reset or maxima_raised:
        log(f"source MAX: {maxima_raised} raised, {maxima_reset} source epochs reset")

    # BASE_ONLY taxonomy (design: declaration-grounded)
    log("classifying base-only symbols ...")
    target_only_parts = {}
    for mangled in target:
        if mangled not in paired_mangled:
            target_only_parts.setdefault(mangled_name_part(mangled), mangled)
    declared_stems = {
        (cls.split("<", 1)[0], name.split("<", 1)[0]) for cls, name in declared_methods
    }
    bos_rows = []
    counts = {}
    for mangled in sorted(set(base) - paired_mangled):
        rec = base[mangled]
        near = target_only_parts.get(mangled_name_part(mangled))
        qn = qualified_name(rec["name"])
        folded_target = exact_paired_source_alias(
            rec,
            target_aliases_by_name,
            used_target_rvas,
            source_alias_names_by_rva=base_alias_names_by_rva,
            candidate_alias_names_by_rva=target_alias_names_by_rva,
        )
        if qn is None or mangled.startswith("??__") or "?A0x" in mangled:
            # thunks, backtick names, dynamic initializers/finalizers, anon-ns
            status, detail = "COMPILER", None
        elif rec["file"].endswith("temp_include_all.cpp"):
            status, detail = "ANCHOR", None  # our reachability scaffolding
        elif near is not None:
            status, detail = "NEAR_MISS", near
        elif folded_target is not None:
            status = "JITTER"
            detail = (
                f"exact ICF alias of {folded_target['mangled']} "
                f"at target RVA 0x{folded_target['rva']:x}"
            )
        elif mangled in old_history:
            status, detail = "JITTER", None
        elif declared_methods or declared_free:
            cls, name = qn
            cls_n, name_n = norm_name(cls), norm_name(name)
            if (cls and (cls_n, name_n) in declared_methods) or (
                not cls and name_n in declared_free
            ):
                status, detail = "INLINED_IN_TARGET", None
            elif cls and (cls_n.split("<", 1)[0], name_n.split("<", 1)[0]) in declared_stems:
                # instantiation of a declared template (dump and demangler
                # render template args differently; the stem is the signal)
                status, detail = "TEMPLATE", None
            else:
                status, detail = "UNEXPLAINED", None
        else:
            status, detail = "UNEXPLAINED", None  # no declarations dump to consult
        bos_rows.append((mangled, status, detail))
        counts[status] = counts.get(status, 0) + 1
    log(f"  base-only: {counts}")

    # which source state was report.json BUILT from? rebuild.py records it in
    # report.head. Without the marker (older rebuild.py): CARRY FORWARD the
    # previous DB's build_head - re-assuming the current HEAD on every refresh
    # would let refresh-without-rebuild launder staleness. Only a virgin DB
    # gets the one-time current-HEAD assumption.
    head_marker = REPORT.parent / "report.head"
    if head_marker.is_file():
        build_head = head_marker.read_text().strip()
    else:
        prev = None
        if DB_PATH.is_file():
            try:
                prev_row = open_db().execute(
                    "SELECT value FROM meta WHERE key='build_head'"
                ).fetchone()
                prev = prev_row[0] if prev_row else None
            except sqlite3.OperationalError:
                prev = None
        if prev and not prev.startswith("?"):
            build_head = prev
            log("no report.head marker - carrying forward the previous build_head")
        else:
            build_head = git_head()
            log("no report.head marker (older rebuild.py) - assuming report was built at current HEAD")

    # build fresh in a temp file, then atomically replace (deterministic bytes)
    tmp = DB_PATH.with_suffix(".db.tmp")
    tmp.unlink(missing_ok=True)
    con = sqlite3.connect(tmp)
    con.executescript("PRAGMA page_size=4096;" + SCHEMA)
    con.executemany(
        "INSERT INTO symbols VALUES (?,?,?)",
        sorted((i, m, demangled_by_mangled.get(m)) for m, i in sym_id.items()),
    )
    con.executemany(
        "INSERT INTO units VALUES (?,?,?)",
        sorted((i, u, module_of(u)) for u, i in unit_id.items()),
    )
    con.executemany("INSERT INTO files VALUES (?,?)", sorted((i, p) for p, i in file_id.items()))
    con.executemany("INSERT INTO target_functions VALUES (?,?,?,?,?,?,?,?,?)", side_rows(target))
    con.executemany("INSERT INTO base_functions VALUES (?,?,?,?,?,?,?,?,?)", side_rows(base))
    con.executemany("INSERT INTO unit_functions VALUES (?,?,?)", unit_rows)
    con.executemany("INSERT INTO pairs VALUES (?,?,?,?,?,?,?,?,?,?)", pair_rows)
    con.executemany("INSERT INTO base_only_status VALUES (?,?,?)", bos_rows)
    con.executemany("INSERT INTO history VALUES (?,?,?,?,?,?)", history_rows)
    con.executemany("INSERT INTO source_maxima VALUES (?,?,?,?,?,?,?,?,?,?,?)", maxima_rows)
    con.executemany(
        "INSERT INTO source_maxima_epochs VALUES (?,?,?,?,?,?,?,?,?,?,?)",
        maxima_epoch_rows,
    )
    con.executemany("INSERT INTO flags VALUES (?,?,?,?)", old_flags)
    con.executemany("INSERT INTO attempts VALUES (?,?,?,?)", old_attempts)
    # deterministic meta only (artifact mtimes, not wall clock)
    con.executemany(
        "INSERT INTO meta VALUES (?,?)",
        [
            ("target_index_mtime", file_mtime_iso(TARGET_IDX)),
            ("base_index_mtime", file_mtime_iso(BASE_IDX)),
            ("report_mtime", file_mtime_iso(REPORT)),
            ("refresh_head", git_head()),
            ("build_head", build_head),
            ("declarations_loaded", "1" if (declared_methods or declared_free) else "0"),
            ("schema_version", SCHEMA_VERSION),
        ],
    )
    con.commit()
    con.execute("VACUUM")
    con.close()
    os.replace(tmp, DB_PATH)

    log(
        f"refreshed {DB_PATH.relative_to(VOSTOK)}: "
        f"{len(target)} target / {len(base)} base / {len(pair_rows)} paired / "
        f"{len(bos_rows)} base-only classified"
    )

    written = match_state.export_from_db(DB_PATH)
    log(f"ledger {Path(match_state.STATE_PATH).name}: {written} rows")
