#!/usr/bin/env python3
"""
rebuild.py — full base-side refresh after editing sources.

  1. Build survarium via ninja under Wine (scripts/ninja_build.py).
  2. In parallel (both IO-bound, disjoint outputs):
       a. binaries/structure/base  — pdb-parser --as-base  (generate_structure.py)
       b. binaries/objdiff/base     — vostok-delinker COFF   (generate_delink.py)
          + refreshed objdiff.json

The target side (binaries/structure/target, binaries/objdiff/target) is the
original game and does not change between recompiles; it is generated once on
first `nix develop` (see setup-toolchain.py).

Any extra args are forwarded to ninja_build.py:
  python3 scripts/rebuild.py            # build the game, then refresh base diff inputs
  python3 scripts/rebuild.py logging    # build just one project first
"""

import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

import generate_delink
import generate_structure


SCRIPT_DIR = Path(__file__).resolve().parent


def log(msg: str) -> None:
    print(f"[rebuild] {msg}", flush=True)


def die(msg: str) -> None:
    print(f"[rebuild] ERROR: {msg}", file=sys.stderr)
    sys.exit(1)


def main() -> None:
    log("Building survarium via ninja ...")
    try:
        subprocess.run(
            [sys.executable, str(SCRIPT_DIR / "ninja_build.py"), *sys.argv[1:]],
            check=True,
        )
    except subprocess.CalledProcessError as e:
        die(f"ninja build failed (exit {e.returncode}); not regenerating diff inputs")

    log("Build OK. Regenerating base structure + COFF in parallel ...")
    steps = {
        "base structure": lambda: generate_structure.generate("base"),
        "base COFF":       lambda: generate_delink.generate("base"),
    }
    failures = []
    with ThreadPoolExecutor(max_workers=len(steps)) as ex:
        futures = {name: ex.submit(fn) for name, fn in steps.items()}
        for name, fut in futures.items():
            try:
                fut.result()
                log(f"{name}: OK")
            except Exception as e:  # noqa: BLE001 — report every step's failure
                failures.append(name)
                log(f"{name}: FAILED — {e}")

    if failures:
        die(f"{len(failures)} step(s) failed: {', '.join(failures)}")
    log("All done — base diff inputs refreshed.")


if __name__ == "__main__":
    main()
