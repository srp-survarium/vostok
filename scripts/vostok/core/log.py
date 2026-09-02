# SPDX-License-Identifier: GPL-3.0-or-later
"""vostok.core.log - the one `[tag] message` logger and the one `die`."""
import sys


def logger(tag: str):
    """A print-style logger whose lines read `[tag] message` (stdout, flushed)."""
    def log(msg) -> None:
        print(f"[{tag}] {msg}", flush=True)
    return log


def die(tag: str, msg: str, *hints: str, code: int = 1) -> None:
    """`[tag] ERROR: msg` on stderr, one indented hint line each, then exit."""
    print(f"[{tag}] ERROR: {msg}", file=sys.stderr)
    for h in hints:
        print(f"  {h}", file=sys.stderr)
    sys.exit(code)
