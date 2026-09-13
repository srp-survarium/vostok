# SPDX-License-Identifier: GPL-3.0-or-later

"""Thin Python umbrella entry for the in-repo Rust evidence engine."""

from __future__ import annotations

import os
import subprocess
import sys

from vostok.core import log as _log


def main() -> int:
    executable = os.environ.get("PDB_TOOL", "vostok-pdb")
    try:
        return subprocess.run([executable, *sys.argv[1:]], check=False).returncode
    except FileNotFoundError:
        print(
            f"vostok pdb: {executable!r} is not on PATH; enter `nix develop`",
            file=sys.stderr,
        )
        return 2


if __name__ == "__main__":
    raise SystemExit(_log.run("vostok.pdb", main))
