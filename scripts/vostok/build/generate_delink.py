#!/usr/bin/env python3
"""
vostok.build.generate_delink - run vostok-delinker to split an EXE into per-unit COFF .obj files
for objdiff comparison, for one side:

  base    the freshly compiled game
          binaries/Win32/survarium-dx11-win32-gold.{exe,pdb}
          -> binaries/objdiff/base

  target  the original game
          $SURVARIUM_BIN/survarium.{exe,pdb}
          -> binaries/objdiff/target

After delinking it refreshes binaries/objdiff/objdiff.json via
vostok.build.generate_objdiff_config. This mirrors vostok-delinker/build_{base,target}.sh
but is driven from Python so the whole build/diff loop stays in this repo.

Usage:
  python3 -m vostok.build.generate_delink base
  python3 -m vostok.build.generate_delink target

Env vars (set automatically by flake.nix devShell):
  SURVARIUM_BIN   - directory with the original survarium.{exe,pdb} (target side)
  VOSTOK_DELINKER - vostok-delinker binary to invoke (default: vostok-delinker on PATH)
"""

import argparse
import datetime
import json
import os
import re
import shutil
import subprocess
import sys
from pathlib import Path

from vostok.build import generate_objdiff_cross_unit
from vostok.core import paths
from vostok.core import symbols as normalize_objdiff_symbols
from vostok.core import tsv
from vostok.core.paths import (EFFECTIVE_SYMBOL_MAP, GFX_TARGET_PREFIX,
                               OBJDIFF_DIR, RICH_DIR, SCALEFORM_SDK,
                               SOURCES, SYMBOL_MAP, SYMBOL_MAP_OVERRIDES,
                               WIN32_DIR, survarium_bin)
from vostok.derive.aliases import (instruction_stream_exact,
                                   load_exact_fold_aliases,
                                   strict_source_alias_candidates)
from vostok.derive.index import index_by_mangled, load_index_records
from vostok.derive.names import qualified_name

# The MSVC linker folds identical functions/data to one location, so a single
# address can carry several mangled names. target and base may pick different
# names for the same body, which breaks objdiff matching. The target delink
# records its choice per folded group here (SYMBOL_MAP); the base delink reads
# it back and reproduces target's names where it can.


def _effective_symbol_map() -> Path:
    """Merge reviewed alias choices over the delinker's generated target map.

    The target map only contains addresses that are folded in the target.  A
    target singleton can nevertheless be folded with another symbol in the
    base image, in which case the generated map has no preference to apply.
    Reviewed overrides cover those asymmetric fold groups without changing
    source merely to steer /OPT:ICF.
    """
    choices: dict[str, str] = {}
    order: list[str] = []
    for path in (SYMBOL_MAP, SYMBOL_MAP_OVERRIDES):
        if not path.is_file():
            continue
        for line_number, fields in tsv.read(path):
            if len(fields) != 2 or not all(fields):
                raise RuntimeError(
                    f"{path}:{line_number}: expected <alias>\\t<target choice>"
                )
            alias, choice = fields
            if alias not in choices:
                order.append(alias)
            choices[alias] = choice

    if not choices:
        return SYMBOL_MAP

    EFFECTIVE_SYMBOL_MAP.write_text(
        "".join(f"{alias}\t{choices[alias]}\n" for alias in order)
    )
    return EFFECTIVE_SYMBOL_MAP


def log(msg: str) -> None:
    print(f"[delink] {msg}", flush=True)


def _delinker_bin() -> str:
    return os.environ.get("VOSTOK_DELINKER", "vostok-delinker")


def _delinker_supports(delinker: str, flag: str) -> bool:
    """Whether `delinker` advertises `flag` in its --help.

    The folded-symbol map flags are newer than some installed delinker builds;
    gating on this keeps an older binary on PATH working unchanged (it simply
    skips reconciliation) instead of erroring on an unknown argument.
    """
    try:
        out = subprocess.run(
            [delinker, "--help"], capture_output=True, text=True, check=False
        )
    except OSError:
        return False
    return flag in (out.stdout + out.stderr)


def _wine_path(p: Path) -> str:
    r"""Render a native absolute path the way MSVC-under-Wine records it in a PDB:
    on the Z: drive (Wine maps ``/`` -> ``Z:``), lowercased, ``\``-separated.
    e.g. /home/u/Proj/vostok/sources -> z:\home\u\proj\vostok\sources
    """
    return "z:" + str(p).replace("/", "\\").lower()


def _nonempty_dir(p: Path) -> bool:
    return p.is_dir() and any(p.iterdir())


def _rich_pdb_aliases() -> dict[str, str]:
    target_index = RICH_DIR / "target" / "index.jsonl"
    base_index = RICH_DIR / "base" / "index.jsonl"
    if not target_index.is_file() or not base_index.is_file():
        return {}
    return normalize_objdiff_symbols.rich_pdb_aliases(
        target_index,
        base_index,
        source_prefix="vostok/",
    )


def _call_operand_aliases(records):
    """Index the printable call/jump aliases contributed by each rich RVA."""
    rvas_by_operand = {}
    operands_by_rva = {}
    for rec in records:
        qualified = qualified_name(rec.get("name", ""))
        if qualified is None:
            continue
        owner, leaf = qualified
        operand = f"{owner}::{leaf}" if owner else leaf
        operand = re.sub(r"\s+>", ">", operand)
        rvas_by_operand.setdefault(operand, set()).add(rec["rva"])
        operands_by_rva.setdefault(rec["rva"], set()).add(operand)
    return rvas_by_operand, operands_by_rva


def _strict_current_exact_symbols() -> set[str]:
    """Return same-identity functions proven exact by current rich streams.

    Raw objdiff can drop a COMDAT from 100 to unscored when LTCG selects the
    same body from a different header owner.  The rich indexes still carry the
    target and base identities and their normalized instruction streams, so an
    equal stream is strict current-build evidence that this is attribution
    churn rather than a source regression.
    """
    target_index = RICH_DIR / "target" / "index.jsonl"
    base_index = RICH_DIR / "base" / "index.jsonl"
    if not target_index.is_file() or not base_index.is_file():
        return set()

    target_records = load_index_records(target_index)
    base_records = load_index_records(base_index)
    target = index_by_mangled(target_records)
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

    target_rvas, target_operands = _call_operand_aliases(target_records)
    base_rvas, base_operands = _call_operand_aliases(base_records)
    base_aliases_by_name = {}
    base_aliases_by_mangled = {}
    target_alias_names_by_rva = {}
    base_alias_names_by_rva = {}
    for rec in target_records:
        target_alias_names_by_rva.setdefault(rec["rva"], set()).add(rec["name"])
    for rec in base_records:
        base_aliases_by_name.setdefault(rec["name"], {})[rec["rva"]] = rec
        base_aliases_by_mangled.setdefault(rec["mangled"], {})[rec["rva"]] = rec
        base_alias_names_by_rva.setdefault(rec["rva"], set()).add(rec["name"])

    def call_alias_equivalent(target_operand, base_operand):
        target_candidates = target_rvas.get(target_operand, set())
        base_candidates = base_rvas.get(base_operand, set())
        if len(target_candidates) != 1 or len(base_candidates) != 1:
            return False
        target_rva = next(iter(target_candidates))
        base_rva = next(iter(base_candidates))
        return bool(
            target_operands.get(target_rva, set())
            & base_operands.get(base_rva, set())
        )

    exact = set()
    for mangled in target.keys() & base.keys():
        if not instruction_stream_exact(
            target[mangled],
            base[mangled],
            call_alias_equivalent,
        ):
            continue
        exact.add(mangled)
        compiler_name = normalize_objdiff_symbols.compiler_name(mangled)
        if compiler_name:
            exact.add(compiler_name)

    exact_fold_aliases = load_exact_fold_aliases()
    for mangled, target_rec in target.items():
        candidates = strict_source_alias_candidates(
            target_rec,
            base_aliases_by_name,
            set(),
            allow_used=True,
            target_alias_names_by_rva=target_alias_names_by_rva,
            base_alias_names_by_rva=base_alias_names_by_rva,
            exact_fold_aliases=exact_fold_aliases,
            base_aliases_by_mangled=base_aliases_by_mangled,
        )
        if len(candidates) != 1:
            continue
        exact.add(mangled)
        compiler_name = normalize_objdiff_symbols.compiler_name(mangled)
        if compiler_name:
            exact.add(compiler_name)
    return exact


# Archived reports are ~14 MB each and one is written per build. Unbounded, that
# reached 824 files / 11 GB in a fortnight - the bulk of `binaries/`, and larger
# than everything the build actually needs. Only the immediately-previous report
# is ever read (`_report_changes`); the rest are there so a human can diff across
# a few builds. Keep a small ring and let the kernel have the disk back.
KEEP_REPORTS = int(os.environ.get("VOSTOK_KEEP_REPORTS", "10"))


def _prune_reports(archive_dir: Path) -> None:
    """Keep only the newest KEEP_REPORTS archived reports (0 = keep all)."""
    if KEEP_REPORTS <= 0:
        return
    # Names are report-YYYYmmdd-HHMMSS.json, so lexical order IS chronological.
    archived = sorted(archive_dir.glob("report-*.json"))
    stale = archived[:-KEEP_REPORTS]
    if not stale:
        return
    freed = 0
    for path in stale:
        try:
            freed += path.stat().st_size
            path.unlink()
        except OSError as e:  # noqa: PERF203 - a prune failure must never fail a build
            log(f"could not prune {path.name}: {e}")
    log(f"Pruned {len(stale)} archived report(s), {freed / 1e9:.2f} GB freed "
        f"(keeping {KEEP_REPORTS}; set VOSTOK_KEEP_REPORTS=0 to keep all)")


def _generate_report() -> None:
    """Write the objdiff match report (base vs target) to binaries/objdiff/report.json.

    If a previous report exists it is archived (timestamped) under
    binaries/objdiff/reports/ and diffed against the new one (see _report_changes),
    so improvements and regressions between builds are visible. Skips quietly if
    objdiff-cli is unavailable or either side has not been delinked yet (e.g.
    during the one-time target setup, before any base build).
    """
    objdiff_cli = os.environ.get("OBJDIFF_CLI", "objdiff-cli")
    if shutil.which(objdiff_cli) is None:
        log(f"objdiff-cli not found ({objdiff_cli!r}); skipping report")
        return
    if not (_nonempty_dir(OBJDIFF_DIR / "base") and _nonempty_dir(OBJDIFF_DIR / "target")):
        log("Report skipped (base and target not both delinked yet).")
        return

    report = OBJDIFF_DIR / "report.json"

    # Keep history: archive the existing report (timestamped) before regenerating,
    # so we can diff the new one against it instead of overwriting it.
    previous = None
    if report.exists():
        archive_dir = OBJDIFF_DIR / "reports"
        archive_dir.mkdir(parents=True, exist_ok=True)
        ts = datetime.datetime.now().strftime("%Y%m%d-%H%M%S")
        previous = archive_dir / f"report-{ts}.json"
        report.rename(previous)
        _prune_reports(archive_dir)

    log("Generating objdiff report ...")
    subprocess.run(
        [objdiff_cli, "report", "generate", "-p", str(OBJDIFF_DIR), "-o", str(report)],
        check=True,
    )
    try:
        m = json.loads(report.read_text()).get("measures", {})
        log("Match: code {:.2f}% / functions {:.2f}%".format(
            m.get("matched_code_percent", 0.0),
            m.get("matched_functions_percent", 0.0),
        ))
    except (OSError, ValueError):
        pass
    log(f"Report: {report}")

    if previous is not None:
        _report_changes(previous, report)

    candidates, scored, exact_unit_count = generate_objdiff_cross_unit.generate(
        OBJDIFF_DIR, objdiff_cli=objdiff_cli
    )
    log(f"Cross-unit COMDAT scores: {scored}/{candidates}")
    log(f"Exact source-object units: {exact_unit_count}")


def _report_changes(previous: Path, current: Path) -> None:
    """Log the net deltas plus the per-function changes between the previous and
    current reports: regressions, improvements, and functions that disappeared
    (removed) or appeared (added) - e.g. a signature change drops the old mangled
    name and adds a new one.

    The per-function diff is computed directly from the two reports, keyed by
    (unit, function) - which is precise. objdiff-cli's own `report changes`
    matches functions by name across the whole archive and adds a lot of noise
    (phantom entries, same-name/different-size mismatches). A clean per-function
    list is written to binaries/objdiff/report-changes.json.

    A symbol emitted into several TUs and folded by the linker is scored 100 in
    one unit and 0 in the others, and the winner moves between builds on its own.
    Those are bucketed into `fold_churn` rather than `regressed`, so the headline
    counts work someone actually broke.
    """
    try:
        prev = json.loads(previous.read_text())
        cur = json.loads(current.read_text())
    except (OSError, ValueError):
        return

    def fuzzy_by_function(report):
        """Best score per (unit, symbol).

        BEST, not last-wins. A symbol the linker folded appears SEVERAL times in
        one unit's function list - scored 100 for the copy objdiff picked as the
        representative and 0 for the rest - and the order they arrive in is not
        stable between builds. Overwriting on each hit therefore recorded 100 or
        0 depending on which copy happened to come last, and the value flipped
        with no source change: 1,207 keys are duplicated today and 159 of those
        have copies that disagree, which is where the phantom regressions came
        from. Taking the maximum is both stable and the true answer - the symbol
        IS matched, once, under its representative.
        """
        out = {}
        for unit in report.get("units", []):
            for fn in unit.get("functions", []):
                name = fn.get("metadata", {}).get("demangled_name") or fn.get("name", "?")
                key = (unit.get("name"), fn.get("name"))
                pct = fn.get("fuzzy_match_percent")
                if pct is None:
                    pct = 0.0
                if key not in out or pct > out[key][0]:
                    out[key] = (pct, name)
        return out

    before, after = fuzzy_by_function(prev), fuzzy_by_function(cur)
    strict_current_exact = None

    def best_by_symbol(by_function):
        """The best score each SYMBOL reaches over every unit that emitted it."""
        best = {}
        for (_unit, sym), (pct, _name) in by_function.items():
            if pct > best.get(sym, -1.0):
                best[sym] = pct
        return best

    best_before, best_after = best_by_symbol(before), best_by_symbol(after)

    regressed, improved, fold_churn = [], [], []
    for key in before.keys() & after.keys():
        was, name = before[key]
        now, _ = after[key]
        if now < was - 1e-6:
            # The linker folds identical COMDATs emitted into several TUs, and
            # objdiff scores ONE unit's copy at 100 with the rest at 0. Which
            # copy wins is arbitrary and moves between builds with no source
            # change, so a swap reads as a regression here AND an improvement
            # two lines down. If the symbol's BEST score across its units did
            # not drop, nothing regressed - the representative merely moved.
            # 101 symbols sit in that state today (??_G/??1 dtors, boost asio
            # ops, btHashMap), and a reader who trusts the headline concludes
            # they broke every one of them.
            sym = key[1]
            best_did_not_drop = (
                best_after.get(sym, 0.0) >= best_before.get(sym, 0.0) - 1e-6
            )
            if not best_did_not_drop and strict_current_exact is None:
                strict_current_exact = _strict_current_exact_symbols()
            if best_did_not_drop or sym in strict_current_exact:
                fold_churn.append((was, now, name))
            else:
                regressed.append((was, now, name))
        elif now > was + 1e-6:
            improved.append((was, now, name))

    # Keys in only one report (signature change, inlining, deletion). A matched
    # function that vanished is effectively a regression; a new low-match one
    # drags the score down.
    removed = sorted((before[k] for k in before.keys() - after.keys()), reverse=True)
    added = sorted(after[k] for k in after.keys() - before.keys())

    pm, cm = prev.get("measures", {}), cur.get("measures", {})

    def code(m):
        return m.get("matched_code_percent", 0.0)

    def matched(m):
        return int(m.get("matched_functions", 0))

    total = int(cm.get("total_functions", 0))
    log("Changes vs previous: code {:.2f}% -> {:.2f}% ({:+.2f}), "
        "functions {}/{} -> {}/{} ({:+d})".format(
            code(pm), code(cm), code(cm) - code(pm),
            matched(pm), total, matched(cm), total, matched(cm) - matched(pm),
        ))

    regressed.sort(key=lambda r: r[1] - r[0])  # worst (most negative) first
    improved.sort(key=lambda r: r[0] - r[1])   # biggest gain first
    fold_churn.sort(key=lambda r: r[1] - r[0])
    log("  {} regressed, {} improved, {} removed, {} added{}".format(
        len(regressed), len(improved), len(removed), len(added),
        f", {len(fold_churn)} fold-churn (not regressions)" if fold_churn else ""))
    limit = 10
    for was, now, name in regressed[:limit]:
        log(f"  regressed {was:6.2f}% -> {now:6.2f}%  {name}")
    if len(regressed) > limit:
        log(f"  ... and {len(regressed) - limit} more regressed")
    for was, now, name in improved[:limit]:
        log(f"  improved  {was:6.2f}% -> {now:6.2f}%  {name}")
    if len(improved) > limit:
        log(f"  ... and {len(improved) - limit} more improved")
    for pct, name in removed[:limit]:
        log(f"  removed   (was {pct:6.2f}%)  {name}")
    if len(removed) > limit:
        log(f"  ... and {len(removed) - limit} more removed")
    for pct, name in added[:limit]:
        log(f"  added     (now {pct:6.2f}%)  {name}")
    if len(added) > limit:
        log(f"  ... and {len(added) - limit} more added")

    changes = OBJDIFF_DIR / "report-changes.json"
    changes.write_text(json.dumps({
        "regressed": [{"function": n, "from": a, "to": b} for a, b, n in regressed],
        "improved": [{"function": n, "from": a, "to": b} for a, b, n in improved],
        "removed": [{"function": n, "from": p} for p, n in removed],
        "added": [{"function": n, "to": p} for p, n in added],
        # Bucketed OUT of `regressed` on purpose: either the same symbol's best
        # score merely moved between units, or current rich streams prove the
        # raw unscored copy remains instruction-exact after an ICF owner swap.
        "fold_churn": [{"function": n, "from": a, "to": b} for a, b, n in fold_churn],
    }, indent=2) + "\n")
    log(f"Changes: {changes} (previous report kept at {previous})")


def generate(side: str) -> None:
    """Delink <side> into binaries/objdiff/<side> and refresh objdiff.json.

    Raises RuntimeError if inputs are missing and CalledProcessError if the
    delinker / config generator fail - callers handle/report these.
    """
    delinker = _delinker_bin()

    if side == "base":
        exe = WIN32_DIR / "survarium-dx11-win32-gold.exe"
        pdb = WIN32_DIR / "survarium-dx11-win32-gold.pdb"
        # The base PDB is MSVC-built under Wine, which records source paths under
        # the Z: drive (Wine maps `/` -> Z:), e.g. z:\home\...\vostok\sources\....
        # Pass the Wine form of <repo>/sources with a trailing separator (the
        # delinker strips this prefix off each recorded path), mirroring target's
        # bare `c:/survarium/sources`.
        engine = ["--engine-path", _wine_path(SOURCES) + "\\"]
        # ...plus the GFx SDK, which is compiled in but lives outside sources/.
        engine += ["--engine-path", _wine_path(SCALEFORM_SDK) + "\\"]
        # Reproduce target's folded-symbol name choices (tolerant if target has
        # not been delinked yet, i.e. the map is missing).
        symbol_map = ["--read-symbol-map", str(_effective_symbol_map())]
        hint = "build first (python3 -m vostok build)"
    elif side == "target":
        survarium = survarium_bin()
        exe = survarium / "survarium.exe"
        pdb = survarium / "survarium.pdb"
        engine = ["--engine-path", "c:/survarium/sources",
                  "--engine-path", GFX_TARGET_PREFIX + "\\"]
        # Record target's choice for each folded symbol group so the base delink
        # can reproduce it.
        symbol_map = ["--write-symbol-map", str(SYMBOL_MAP)]
        hint = "set SURVARIUM_BIN or run inside `nix develop` (provides survarium-game)"
    else:  # pragma: no cover - argparse restricts choices
        raise RuntimeError(f"unknown side {side!r} (expected 'base' or 'target')")

    for f in (exe, pdb):
        if not f.is_file():
            raise RuntimeError(f"{f} not found - {hint}")

    # Check the delinker is present before wiping the output directory, so a
    # missing binary can't destroy a previously-good delink.
    if shutil.which(delinker) is None:
        raise RuntimeError(
            f"{delinker!r} not found on PATH - run inside `nix develop` "
            "(provides vostok-delinker), or set VOSTOK_DELINKER"
        )

    # Drop the map flags if this delinker predates them (keeps an older PATH
    # binary working; reconciliation auto-enables once the new one is deployed).
    if symbol_map and not _delinker_supports(delinker, symbol_map[0]):
        log(f"delinker has no {symbol_map[0]}; skipping folded-symbol reconciliation")
        symbol_map = []

    out = OBJDIFF_DIR / side
    if out.exists():
        shutil.rmtree(out)
    out.mkdir(parents=True, exist_ok=True)

    log(f"Delinking {side} ({exe.name}) -> {out}")
    subprocess.run(
        [
            delinker,
            "--pdb-path",    str(pdb),
            "--exe-path",    str(exe),
            "--output-path", str(out),
            *engine,
            *symbol_map,
        ],
        check=True,
    )

    if side == "target":
        nm = shutil.which("llvm-nm")
        objcopy = shutil.which("llvm-objcopy")
        if not nm or not objcopy:
            raise RuntimeError(
                "llvm-nm and llvm-objcopy are required to normalize retail "
                "static-init thunk names"
            )
        aliases = _rich_pdb_aliases()
        objects, symbols = normalize_objdiff_symbols.normalize_tree(
            out, nm=nm, objcopy=objcopy, aliases=aliases
        )
        log(
            f"Normalized {symbols} project-specific static-init symbols "
            f"in {objects} target objects ({len(aliases)} unique rich-PDB aliases)"
        )
    elif (OBJDIFF_DIR / "target").is_dir():
        # The target tree is persistent, but normalization rules can improve.
        # Reapply the idempotent transform before every comparison so a script
        # update does not require regenerating the retail delink by hand.
        nm = shutil.which("llvm-nm")
        objcopy = shutil.which("llvm-objcopy")
        if not nm or not objcopy:
            raise RuntimeError(
                "llvm-nm and llvm-objcopy are required to normalize retail "
                "static-init thunk names"
            )
        aliases = _rich_pdb_aliases()
        objects, symbols = normalize_objdiff_symbols.normalize_tree(
            OBJDIFF_DIR / "target", nm=nm, objcopy=objcopy, aliases=aliases
        )
        log(
            f"Updated {symbols} project-specific static-init symbols "
            f"in {objects} persistent target objects "
            f"({len(aliases)} unique rich-PDB aliases)"
        )

    log("Refreshing objdiff config ...")
    subprocess.run(
        [sys.executable, "-m", "vostok.build.generate_objdiff_config"],
        check=True, env=paths.child_env(),
    )
    log(f"Done: {out}")
    _generate_report()


def main() -> None:
    ap = argparse.ArgumentParser(
        description="Delink base/target EXE into COFF objs via vostok-delinker."
    )
    ap.add_argument("side", choices=["base", "target"])
    try:
        generate(ap.parse_args().side)
    except (RuntimeError, subprocess.CalledProcessError) as e:
        print(f"[delink] ERROR: {e}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
