#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""
vostok.build.rebuild - `vostok build`: compile and link the game.

  1. Toolchain setup (vostok.tool.toolchain): staged libs, Wine prefix, registry.
  2. Refresh the ninja graph from the .vcprojs (vostok.build.ninja_regen).
  3. Build via ninja under Wine (vostok.build.ninja); the result is
     binaries/Win32/survarium-dx11-win32-gold.exe.

Each run appends one line to binaries/rebuild.log (git-ignored):
    [<timestamp>][<git-branch>]: <elapsed>, <summary>
where <summary> names the engine modules whose TUs ninja recompiled.

Any extra args are forwarded to vostok.build.ninja:
  python3 -m vostok build            # the whole game
  python3 -m vostok build logging    # just one project
"""

import datetime
import fcntl
import os
import re
import select
import subprocess
import functools
import sys
import time

from vostok.build import ninja_regen

from vostok.core import paths
from vostok.core.paths import REBUILD_LOG as LOG_PATH
from vostok.core.paths import REPO as VOSTOK_DIR
from vostok.core.log import logger
from vostok.core.log import die as core_die
from vostok.core import log as _log

# A compiled TU shows up in ninja's verbose (-v) output as a cl command line that
# cd's into the module's source dir, e.g.
#   cmd /c cd "Z:\...\sources\vostok\game_core\sources" && cl @...rsp ...
_CL_MODULE_RE = re.compile(r"vostok[\\/]([A-Za-z0-9_]+)[\\/]sources\b[^\n]*?&&\s*cl\b")

# After vostok.build.ninja exits, Wine children leaked by the build may still
# hold the output pipe open; once the child has exited and the pipe has stayed
# silent this long, stop reading.
DRAIN_GRACE_SECONDS = 2.0


log = logger("rebuild")


die = functools.partial(core_die, "rebuild")


def _git_branch() -> str:
    try:
        out = subprocess.run(
            ["git", "rev-parse", "--abbrev-ref", "HEAD"],
            cwd=str(VOSTOK_DIR), capture_output=True, text=True, check=True,
        )
        return out.stdout.strip() or "?"
    except Exception:  # noqa: BLE001 - audit log must never break the build
        return "?"


def _fmt_elapsed(seconds: float) -> str:
    if seconds >= 60:
        m, s = divmod(int(round(seconds)), 60)
        return f"{m}m{s:02d}s"
    return f"{seconds:.1f}s"


def _summarize(modules: set[str]) -> str:
    n = len(modules)
    if n == 0:
        return "0 modules (no-op)"
    if n <= 4:
        return f"{n} module{'s' if n != 1 else ''}: {', '.join(sorted(modules))}"
    return f"{n} modules"


def _append_log(elapsed: float, modules: set[str]) -> None:
    """Best-effort audit line; a logging error must never fail the build."""
    try:
        ts = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")[:-4]
        line = f"[{ts}][{_git_branch()}]: {_fmt_elapsed(elapsed)}, {_summarize(modules)}\n"
        LOG_PATH.parent.mkdir(parents=True, exist_ok=True)
        with open(LOG_PATH, "a", encoding="utf-8") as f:
            f.write(line)
    except Exception as e:  # noqa: BLE001
        log(f"(audit log skipped: {e})")


def run_ninja() -> set[str]:
    """Run vostok.build.ninja, streaming its output, and return the set of modules
    whose TUs were recompiled (parsed from the verbose cl command lines)."""
    modules: set[str] = set()
    proc = subprocess.Popen(
        [sys.executable, "-u", "-m", "vostok.build.ninja", *sys.argv[1:]],
        stdout=subprocess.PIPE, stderr=subprocess.STDOUT, env=paths.child_env(),
    )
    fd = proc.stdout.fileno()
    tail = b""
    exited_at = None

    def scan(data: bytes) -> None:
        m = _CL_MODULE_RE.search(data.decode("utf-8", "replace"))
        if m:
            modules.add(m.group(1))

    while True:
        ready, _, _ = select.select([fd], [], [], 0.5)
        if ready:
            chunk = os.read(fd, 1 << 16)
            if not chunk:
                break                       # true EOF: every write end closed
            sys.stdout.buffer.write(chunk)  # keep the live build log intact
            sys.stdout.buffer.flush()
            lines = (tail + chunk).split(b"\n")
            tail = lines.pop()
            for ln in lines:
                scan(ln)
        if proc.poll() is not None:
            if exited_at is None:
                exited_at = time.monotonic()
            elif not ready and time.monotonic() - exited_at >= DRAIN_GRACE_SECONDS:
                break                       # child gone, pipe silent: let go
    scan(tail)
    rc = proc.wait()
    if rc != 0:
        raise subprocess.CalledProcessError(rc, "vostok.build.ninja")
    return modules


def _acquire_build_lock():
    """Serialise builds sharing this worktree (one ninja tree, one WINEPREFIX).

    flock is dropped by the kernel when the process dies, so a stale lock is
    not possible.
    """
    paths.BUILD_LOCK.parent.mkdir(parents=True, exist_ok=True)
    lock = open(paths.BUILD_LOCK, "a")
    try:
        fcntl.flock(lock, fcntl.LOCK_EX | fcntl.LOCK_NB)
    except OSError:
        log("another build holds this worktree - waiting for it to finish ...")
        fcntl.flock(lock, fcntl.LOCK_EX)
        log("lock acquired; starting.")
    return lock


def main() -> None:
    # `vostok build --help` must not reach ninja: its own --help exits 1 and
    # would still run the trailing mspdbsrv reap.
    if any(a in ("-h", "--help") for a in sys.argv[1:]):
        print(__doc__.strip())
        return

    lock = _acquire_build_lock()
    start = time.monotonic()
    modules: set[str] = set()
    try:
        from vostok.tool import toolchain
        _log.timed("toolchain setup", lambda: toolchain.setup(set()))
        log("Refreshing ninja graph from the .vcprojs ...")
        _log.timed("ninja graph", ninja_regen.regenerate)

        log("Building survarium via ninja ...")
        try:
            modules = _log.timed("ninja build", run_ninja)
        except subprocess.CalledProcessError as e:
            die(f"ninja build failed (exit {e.returncode})")
        log(f"Build OK ({_summarize(modules)}): {paths.BASE_EXE}")
    finally:
        _append_log(time.monotonic() - start, modules)
        lock.close()


if __name__ == "__main__":
    raise SystemExit(_log.run("vostok.build.rebuild", main))
