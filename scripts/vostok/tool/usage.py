# SPDX-License-Identifier: GPL-3.0-or-later

"""vostok.tool.usage - read back binaries/vostok_usage.log.

Every `python3 -m vostok...` process appends one line there (vostok.core.log).
This is the reader, and the reason the log exists: what the matching agents
ACTUALLY ran is the evidence for which tools grow, which need a better error
message, and which retire. gruntz ran this analysis over a 9,771-call log and
deleted two tools that had never once been invoked.

    python3 -m vostok tool usage              # per-verb counts, failures, time
    python3 -m vostok tool usage --failures   # every nonzero exit, newest last
    python3 -m vostok tool usage --slow 20    # the slowest calls
    python3 -m vostok tool usage --unused     # surface nobody has ever run
    python3 -m vostok tool usage --since 2026-09-01 --branch render-lane

Read the failures first. A verb with a high nonzero rate is usually not a
broken verb - it is a verb whose interface or error message misleads the agent
that keeps reaching for it.
"""

from __future__ import annotations

import argparse
import re
import shlex
from collections import Counter, defaultdict

from vostok.core import log as _log
from vostok.core.paths import USAGE_LOG

# [<ts>][<branch>][<rc>][<elapsed>]: <cmd>[  # <err>]
_LINE = re.compile(r"^\[([^\]]*)\]\[([^\]]*)\]\[(-?\d+)\]\[([^\]]*)\]: (.*)$")


def _seconds(text: str) -> float:
    """Parse the `86ms` / `4.2s` / `38m08s` back to seconds."""
    try:
        if text.endswith("ms"):
            return float(text[:-2]) / 1000
        if "m" in text:
            mins, _, secs = text.partition("m")
            return int(mins) * 60 + float(secs.rstrip("s"))
        return float(text.rstrip("s"))
    except ValueError:
        return 0.0


class Call:
    __slots__ = ("ts", "branch", "rc", "secs", "cmd", "err", "key")

    def __init__(self, ts, branch, rc, elapsed, rest):
        self.ts, self.branch, self.rc = ts, branch, rc
        self.secs = _seconds(elapsed)
        self.cmd, _, self.err = rest.partition("  # ")
        self.key = _verb(self.cmd)


def _verb(cmd: str) -> str:
    """The command down to its verb: `vostok ledger report`, `vostok.sema blocks`.

    One level under the umbrella's lane, one under a direct module entry - past
    that the tokens are arguments (a function name, a module) and would shatter
    the histogram into one bucket per call.
    """
    try:
        parts = shlex.split(cmd)
    except ValueError:
        return cmd
    if parts[:2] != ["python3", "-m"] or len(parts) < 3:
        return cmd
    prog, rest = parts[2], parts[3:]
    depth = 2 if prog == "vostok" else 1
    verbs = []
    for tok in rest:
        if tok.startswith("-") or len(verbs) == depth:
            break
        verbs.append(tok)
    return " ".join([prog, *verbs])


def read(since: str | None = None, branch: str | None = None) -> list[Call]:
    if not USAGE_LOG.exists():
        return []
    calls = []
    with open(USAGE_LOG, encoding="utf-8", errors="replace") as f:
        for line in f:
            m = _LINE.match(line.rstrip("\n"))
            if not m:
                continue  # a hand-added note or a torn line: skip, never fail
            call = Call(m[1], m[2], int(m[3]), m[4], m[5])
            if since and call.ts < since:
                continue
            if branch and call.branch != branch:
                continue
            calls.append(call)
    return calls


# The surface `--unused` asks about: everything the umbrella names, plus the
# lane entries that are the only spelling for their tools. A verb absent here
# still shows up in the histogram - this list only bounds the "never run" answer.
SURFACE = [
    "vostok build", "vostok ledger", "vostok derive", "vostok sema",
    "vostok data", "vostok diff", "vostok tool",
    "vostok.build.ninja", "vostok.build.ninja_regen", "vostok.build.gfx",
    "vostok.build.generate_delink", "vostok.build.generate_structure",
    "vostok.build.generate_rich", "vostok.build.generate_objdiff_config",
    "vostok.ledger.store", "vostok.ledger.readme", "vostok.ledger.queue",
    "vostok.core.symbols", "vostok.shaders", "vostok.derive", "vostok.sema",
    "vostok.data", "vostok.ledger",
]


def _summary(calls: list[Call]) -> int:
    counts: Counter[str] = Counter()
    fails: Counter[str] = Counter()
    secs: defaultdict[str, float] = defaultdict(float)
    for c in calls:
        counts[c.key] += 1
        secs[c.key] += c.secs
        if c.rc:
            fails[c.key] += 1
    width = max((len(k) for k in counts), default=20)
    print(f"{'verb':{width}}  {'calls':>6} {'fail':>5} {'fail%':>6} {'total':>9}")
    for key, n in counts.most_common():
        bad = fails[key]
        print(f"{key:{width}}  {n:6d} {bad:5d} {100 * bad / n:5.0f}% "
              f"{_log.elapsed(secs[key]):>9}")
    total, bad = len(calls), sum(fails.values())
    print(f"\n{total} calls, {bad} nonzero ({100 * bad / total:.0f}%), "
          f"{len(counts)} distinct verbs, "
          f"{len({c.branch for c in calls})} branches, "
          f"{_log.elapsed(sum(secs.values()))} total")
    return 0


def _failures(calls: list[Call]) -> int:
    bad = [c for c in calls if c.rc]
    for c in bad:
        print(f"[{c.ts}][{c.branch}][{c.rc}]: {c.cmd}"
              + (f"  # {c.err}" if c.err else ""))
    print(f"\n{len(bad)} nonzero of {len(calls)}")
    return 0


def _slow(calls: list[Call], n: int) -> int:
    for c in sorted(calls, key=lambda c: -c.secs)[:n]:
        print(f"{_log.elapsed(c.secs):>9}  [{c.ts}] {c.cmd}")
    return 0


def _unused(calls: list[Call]) -> int:
    seen = {c.key for c in calls}
    cold = [v for v in SURFACE if not any(k == v or k.startswith(v + " ")
                                          for k in seen)]
    for v in cold:
        print(f"never run: {v}")
    print(f"\n{len(cold)} of {len(SURFACE)} known entry points never invoked")
    return 0


def main() -> int:
    ap = argparse.ArgumentParser(
        prog="vostok tool usage", description=__doc__.split("\n\n")[0],
        epilog=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--failures", action="store_true",
                    help="every nonzero exit with its reason")
    ap.add_argument("--slow", type=int, nargs="?", const=20, metavar="N",
                    help="the N slowest calls (default 20)")
    ap.add_argument("--unused", action="store_true",
                    help="entry points the log has never seen")
    ap.add_argument("--since", metavar="YYYY-MM-DD",
                    help="ignore calls before this date")
    ap.add_argument("--branch", help="only calls made on this branch")
    args = ap.parse_args()

    calls = read(args.since, args.branch)
    if not calls:
        print(f"no calls logged ({USAGE_LOG})")
        return 0
    if args.failures:
        return _failures(calls)
    if args.slow is not None:
        return _slow(calls, args.slow)
    if args.unused:
        return _unused(calls)
    return _summary(calls)


if __name__ == "__main__":
    raise SystemExit(_log.run("vostok.tool.usage", main))
