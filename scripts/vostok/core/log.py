# SPDX-License-Identifier: GPL-3.0-or-later
"""vostok.core.log - the one `[tag] message` logger, the one `die`, and the
one usage log.

Two unrelated jobs, deliberately in one module because both are "how a vostok
command talks about itself":

**Console** - `logger(tag)` / `die(tag, ...)`. Milestone lines on stdout, errors
on stderr. Unchanged; the whole tree already speaks it.

**Usage log** - `run(prog, main)` wraps a CLI entry point and appends ONE line
per process to `binaries/vostok_usage.log` (see `paths.USAGE_LOG`):

    [2026-09-02 12:03:34.96][xray-2.0-prog-v0.100b][0][86ms]: python3 -m vostok ledger report --module render

Metadata in brackets, the command after the `: ` and shell-quoted, so a line
pastes straight back into a shell. On a nonzero exit the reason follows behind
a `#`, which keeps the command half copyable:

    [...][2][14ms]: python3 -m vostok.sema blocks nope  # SystemExit: no such function

This is a usage-analysis feed, ported from gruntz (`build/gruntz_sema.log`, a
9,771-call log that named two never-invoked tools and drove the sema redesign)
and homm3 (`build/homm3_{sema,vc6}.log`, `[date][time][rc]: cmd`). What the
matching agents ACTUALLY run - and what they run that fails - is the evidence
for which tools grow, which grow a better error message, and which retire.
Read it back with `vostok tool usage`.

The three fields the reference logs did not carry are the ones this repo needs:
the branch (one worktree per matcher agent, so it names who ran it), the
duration, and the failure text.

Logging is best-effort by design: it must NEVER break, slow, or fail a tool.
"""
from __future__ import annotations

import datetime
import shlex
import sys
import time
from pathlib import Path

from vostok.core.paths import REPO, USAGE_LOG


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


# --- the usage log ---------------------------------------------------------

def _branch() -> str:
    """The checked-out branch, read straight from `.git` - no subprocess, since
    this runs on the way out of every single tool invocation."""
    try:
        dot_git = REPO / ".git"
        if dot_git.is_file():
            gitdir = dot_git.read_text(encoding="utf-8").split("gitdir:", 1)[1]
            dot_git = Path(gitdir.strip())
        head = (dot_git / "HEAD").read_text(encoding="utf-8").strip()
        return head[16:] if head.startswith("ref: refs/heads/") else head[:12]
    except Exception:  # noqa: BLE001 - never break the tool over the log
        return "?"


def elapsed(secs: float) -> str:
    """`86ms` / `4.2s` / `38m08s` - readable at every scale a verb takes."""
    if secs < 1:
        return f"{secs * 1000:.0f}ms"
    if secs < 60:
        return f"{secs:.1f}s"
    return f"{int(secs) // 60}m{int(secs) % 60:02d}s"


def record(prog: str, argv: list[str], rc: int, secs: float, err: str = "") -> None:
    """Append one usage line. Never raises."""
    try:
        now = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")[:-4]
        cmd = shlex.join(["python3", "-m", prog, *argv])
        line = f"[{now}][{_branch()}][{rc}][{elapsed(secs)}]: {cmd}"
        if err:
            line += "  # " + " ".join(err.split())[:160]
        USAGE_LOG.parent.mkdir(parents=True, exist_ok=True)
        with open(USAGE_LOG, "a", encoding="utf-8") as f:
            f.write(line + "\n")
    except Exception:  # noqa: BLE001 - logging is best-effort by design
        pass


def run(prog: str, main_fn, *args) -> int:
    """Call a CLI entry point, log the invocation, return its exit code.

    `prog` is the `-m` spelling (`vostok`, `vostok.sema`, `vostok.build.rebuild`),
    so the log distinguishes an umbrella call from a direct lane call - itself
    worth knowing. argv is read HERE, at process entry, because several mains
    rewrite `sys.argv` before dispatching.

    Only process entry points call this. `vostok.cli` dispatches by importing a
    module and calling its `main()`, which bypasses that module's `__main__`
    guard - so one process is one line, never two.
    """
    argv = list(sys.argv[1:])
    t0 = time.monotonic()
    try:
        rc = main_fn(*args) or 0
    except SystemExit as e:
        # argparse leaves this way too: `--help` is rc 0, a bad flag is rc 2.
        code = e.code
        rc = code if isinstance(code, int) else (0 if code is None else 1)
        err = "" if code is None or isinstance(code, int) else str(code)
        record(prog, argv, rc, time.monotonic() - t0, err)
        raise
    except KeyboardInterrupt:
        record(prog, argv, 130, time.monotonic() - t0, "KeyboardInterrupt")
        raise
    except BaseException as e:  # noqa: BLE001 - log the crash, then re-raise it
        record(prog, argv, 1, time.monotonic() - t0, f"{type(e).__name__}: {e}")
        raise
    record(prog, argv, rc, time.monotonic() - t0)
    return rc
