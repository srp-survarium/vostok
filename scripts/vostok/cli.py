# SPDX-License-Identifier: GPL-3.0-or-later

"""vostok - build the game (offline lane).

    vostok build [ninja args...]   toolchain setup, ninja graph refresh, then
                                   ninja under Wine -> binaries/Win32/
    vostok tool toolchain          (re)run the toolchain setup on its own:
                                   staged libs, Wine prefix, registry, ninja graph
    vostok tool libs SRC DST       stage prebuilt third-party blobs

Every module is also a direct entry (`python3 -m vostok.build.ninja ...`,
`python3 -m vostok.build.ninja_regen`).
"""

from __future__ import annotations

import importlib
import sys
from vostok.core import log as _log

TOOLS = {"toolchain": "vostok.tool.toolchain", "libs": "vostok.tool.libs"}


def _run(module: str, argv: list[str], prog: str) -> int:
    sys.argv = [prog, *argv]
    return importlib.import_module(module).main() or 0


def main(argv: list[str] | None = None) -> int:
    argv = list(sys.argv[1:] if argv is None else argv)
    if not argv or argv[0] in ("-h", "--help", "help"):
        print(__doc__.strip())
        return 0
    cmd, rest = argv[0], argv[1:]
    if cmd == "build":
        return _run("vostok.build.rebuild", rest, "vostok build")
    if cmd == "tool":
        if not rest or rest[0] not in TOOLS:
            print(f"vostok tool: pick one of {', '.join(sorted(TOOLS))}",
                  file=sys.stderr)
            return 2
        return _run(TOOLS[rest[0]], rest[1:], f"vostok tool {rest[0]}")
    print(f"vostok: unknown command {cmd!r} (see `vostok --help`)",
          file=sys.stderr)
    return 2


if __name__ == "__main__":
    raise SystemExit(_log.run("vostok", main))
