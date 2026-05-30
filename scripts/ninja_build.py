#!/usr/bin/env python3
"""
ninja_build.py — Run ninja.exe under Wine to build a Vostok target.

Default target is the game project: survarium_-_PC_-_DirectX_11
Runs with -v (verbose: full command lines) and -k 0 (keep going: report
every failure, don't stop at the first). Override either by passing your
own -k/-j (the last occurrence wins).

Usage:
  python3 scripts/ninja_build.py                  # build the game (verbose, keep-going)
  python3 scripts/ninja_build.py logging          # build just the logging project
  python3 scripts/ninja_build.py -t clean         # ninja flag (clean tool)
  python3 scripts/ninja_build.py -k 1             # stop at first failure

Required env vars (set by flake.nix devShell):
  NINJA_DIR  — directory containing ninja.exe
  WINEPREFIX — wine prefix initialised by setup-toolchain.py
"""

import os
import subprocess
import sys
from pathlib import Path


SCRIPT_DIR = Path(__file__).resolve().parent
VOSTOK_DIR = SCRIPT_DIR.parent
BUILD_DIR  = VOSTOK_DIR / "binaries" / "ninja"

DEFAULT_TARGET = "survarium_-_PC_-_DirectX_11"


def die(msg: str, *hints: str) -> None:
    print(f"[ninja] ERROR: {msg}", file=sys.stderr)
    for h in hints:
        print(f"  {h}", file=sys.stderr)
    sys.exit(1)


def main() -> None:
    ninja_dir = os.environ.get("NINJA_DIR")
    if not ninja_dir:
        die("NINJA_DIR not set — run from `nix develop`")

    ninja_exe = Path(ninja_dir) / "ninja.exe"
    if not ninja_exe.exists():
        die(f"ninja.exe not found at {ninja_exe}")
    if not (BUILD_DIR / "build.ninja").is_file():
        die(
            f"{BUILD_DIR}/build.ninja missing",
            "Run: python3 scripts/setup-toolchain.py",
        )

    # Silence unactionable Wine debug spam (fixme stubs + kerberos err) from
    # ninja.exe and the cl.exe/link.exe it spawns. setdefault so an explicit
    # WINEDEBUG (e.g. for debugging Wine itself) still wins.
    os.environ.setdefault("WINEDEBUG", "fixme-all,err-kerberos")

    # Defaults for the matching workflow:
    #   -v      verbose — print the full cl.exe/link.exe command lines
    #   -k 0    keep going on errors — compile every TU and report all
    #           failures in one pass instead of stopping at the first
    # Both come before the user's args, so a later -k/-j on the command line
    # still wins (ninja takes the last occurrence).
    args = sys.argv[1:] or [DEFAULT_TARGET]
    rc = subprocess.run(
        ["wine", str(ninja_exe), "-v", "-k", "0", *args],
        cwd=str(BUILD_DIR),
    ).returncode
    sys.exit(rc)


if __name__ == "__main__":
    main()
