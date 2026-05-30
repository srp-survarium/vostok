#!/usr/bin/env python3
"""
generate_delink.py - run vostok-delinker to split an EXE into per-unit COFF .obj files
for objdiff comparison, for one side:

  base    the freshly compiled game
          binaries/Win32/survarium-dx11-win32-gold.{exe,pdb}
          -> binaries/objdiff/base

  target  the original game
          $SURVARIUM_BIN/survarium.{exe,pdb}
          -> binaries/objdiff/target

After delinking it refreshes binaries/objdiff/objdiff.json via
generate_objdiff_config.py. This mirrors vostok-delinker/build_{base,target}.sh
but is driven from Python so the whole build/diff loop stays in this repo.

Usage:
  python3 scripts/generate_delink.py base
  python3 scripts/generate_delink.py target

Env vars (set automatically by flake.nix devShell):
  SURVARIUM_BIN   - directory with the original survarium.{exe,pdb} (target side)
  VOSTOK_DELINKER - vostok-delinker binary to invoke (default: vostok-delinker on PATH)
"""

import argparse
import datetime
import json
import os
import shutil
import subprocess
import sys
from pathlib import Path


SCRIPT_DIR  = Path(__file__).resolve().parent
VOSTOK_DIR  = SCRIPT_DIR.parent
OBJDIFF_DIR = VOSTOK_DIR / "binaries" / "objdiff"
WIN32_DIR   = VOSTOK_DIR / "binaries" / "Win32"


def log(msg: str) -> None:
    print(f"[delink] {msg}", flush=True)


def _delinker_bin() -> str:
    return os.environ.get("VOSTOK_DELINKER", "vostok-delinker")


def _wine_path(p: Path) -> str:
    r"""Render a native absolute path the way MSVC-under-Wine records it in a PDB:
    on the Z: drive (Wine maps ``/`` -> ``Z:``), lowercased, ``\``-separated.
    e.g. /home/u/Proj/vostok/sources -> z:\home\u\proj\vostok\sources
    """
    return "z:" + str(p).replace("/", "\\").lower()


def _nonempty_dir(p: Path) -> bool:
    return p.is_dir() and any(p.iterdir())


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


def _report_changes(previous: Path, current: Path) -> None:
    """Log the net deltas plus the per-function regressions and improvements
    between the previous and current reports.

    The per-function diff is computed directly from the two reports, keyed by
    (unit, function) - which is precise. objdiff-cli's own `report changes`
    matches functions by name across the whole archive and adds a lot of noise
    (phantom entries, same-name/different-size mismatches). A clean per-function
    list is written to binaries/objdiff/report-changes.json.
    """
    try:
        prev = json.loads(previous.read_text())
        cur = json.loads(current.read_text())
    except (OSError, ValueError):
        return

    def fuzzy_by_function(report):
        out = {}
        for unit in report.get("units", []):
            for fn in unit.get("functions", []):
                name = fn.get("metadata", {}).get("demangled_name") or fn.get("name", "?")
                out[(unit.get("name"), fn.get("name"))] = (fn.get("fuzzy_match_percent", 0.0), name)
        return out

    before, after = fuzzy_by_function(prev), fuzzy_by_function(cur)
    regressed, improved = [], []
    for key in before.keys() & after.keys():
        was, name = before[key]
        now, _ = after[key]
        if now < was - 1e-6:
            regressed.append((was, now, name))
        elif now > was + 1e-6:
            improved.append((was, now, name))

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
    log(f"  {len(regressed)} function(s) regressed, {len(improved)} improved")
    limit = 10
    for was, now, name in regressed[:limit]:
        log(f"  regressed {was:6.2f}% -> {now:6.2f}%  {name}")
    if len(regressed) > limit:
        log(f"  ... and {len(regressed) - limit} more regressed")
    for was, now, name in improved[:limit]:
        log(f"  improved  {was:6.2f}% -> {now:6.2f}%  {name}")
    if len(improved) > limit:
        log(f"  ... and {len(improved) - limit} more improved")

    changes = OBJDIFF_DIR / "report-changes.json"
    changes.write_text(json.dumps({
        "regressed": [{"function": n, "from": a, "to": b} for a, b, n in regressed],
        "improved": [{"function": n, "from": a, "to": b} for a, b, n in improved],
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
        engine = ["--engine-path", _wine_path(VOSTOK_DIR / "sources") + "\\"]
        hint = "build first (python3 scripts/rebuild.py)"
    elif side == "target":
        survarium_bin = Path(
            os.environ.get("SURVARIUM_BIN", VOSTOK_DIR / "binaries" / "nix-store" / "survarium-game")
        )
        exe = survarium_bin / "survarium.exe"
        pdb = survarium_bin / "survarium.pdb"
        engine = ["--engine-path", "c:/survarium/sources"]
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
        ],
        check=True,
    )

    log("Refreshing objdiff config ...")
    subprocess.run(
        [sys.executable, str(SCRIPT_DIR / "generate_objdiff_config.py")],
        check=True,
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
