#!/usr/bin/env python3
"""Run rebuild.py with a stall watchdog that recovers from the Wine zombie-link hang.

The Wine linker (and occasionally `lib`) sometimes finishes writing its output but
the wrapping `cmd /c ... link` / `ninja.exe` process never exits, so ninja waits on
a dead child forever and `rebuild.py` hangs at the final link step (observed: 45+ min
parked at `[N/M] ... link ...`, EXE already on disk). This wrapper polls rebuild.py's
log; if it stops growing while parked at a link/lib step past --stall seconds, it kills
THIS worktree's build processes and retries. Because the EXE is already written by then,
the retry's ninja sees it up-to-date, skips the link, and rebuild.py reaches the
delinker - so the build completes instead of hanging.

Scope safety: only processes that belong to THIS worktree are killed - matched by the
worktree's full path in the process cwd OR its name in the cmdline. Sibling worktrees
(vostok, vostok_1, ...) building in parallel are left untouched.

Run it INSIDE `nix develop`, in place of rebuild.py:
    nix develop -c python3 scripts/rebuild_watchdog.py [rebuild.py args] \
        [--stall SECONDS] [--max-attempts N] [--log PATH]
"""
import argparse
import os
import re
import signal
import subprocess
import sys
import time

WT_FULL = os.getcwd()                       # /home/.../vostok-review
WT_NAME = os.path.basename(WT_FULL)         # vostok-review
# a line that means ninja is at a link/lib step (where the hang happens)
LINK_HINT = re.compile(r"(link @|lib @|\blink\b|\] cmd /c)", re.I)
# cmdlines that are part of a build (so we never kill an unrelated shell/editor)
BUILD_CMD = re.compile(r"(ninja|\.rsp|cl @|link @|lib @|rebuild\.py|ninja_build)", re.I)


def _read(path):
    try:
        with open(path, "rb") as f:
            return f.read().decode("utf-8", "replace")
    except OSError:
        return ""


def _cwd_of(pid):
    try:
        return os.readlink(f"/proc/{pid}/cwd")
    except OSError:
        return ""


def worktree_build_pids(exclude):
    """PIDs that are build steps belonging to THIS worktree."""
    out = []
    for pid in os.listdir("/proc"):
        if not pid.isdigit() or int(pid) in exclude:
            continue
        cmd = _read(f"/proc/{pid}/cmdline").replace("\x00", " ")
        if not BUILD_CMD.search(cmd):
            continue
        # belongs to this worktree if its cwd is under it, or its cmdline names it
        if _cwd_of(pid).startswith(WT_FULL) or WT_NAME in cmd:
            out.append(int(pid))
    return out


def kill_worktree_build(exclude):
    for sig in (signal.SIGTERM, signal.SIGKILL):
        pids = worktree_build_pids(exclude)
        if not pids:
            return
        for p in pids:
            try:
                os.kill(p, sig)
            except OSError:
                pass
        time.sleep(2)


def tail(path, n=4000):
    s = _read(path)
    return s[-n:]


def run_once(rebuild_args, log_path, stall, mine):
    with open(log_path, "wb") as log:
        proc = subprocess.Popen(
            [sys.executable, "scripts/rebuild.py", *rebuild_args],
            stdout=log, stderr=subprocess.STDOUT,
        )
    mine = mine | {proc.pid}
    last_size, stalled = -1, 0
    while proc.poll() is None:
        time.sleep(15)
        try:
            size = os.path.getsize(log_path)
        except OSError:
            size = 0
        stalled = stalled + 15 if size == last_size else 0
        last_size = size
        if stalled >= stall and LINK_HINT.search(tail(log_path, 400)):
            print(f"[watchdog] STALL {stalled}s at link/lib step -> killing {WT_NAME} build "
                  f"(EXE already written; retry will skip the link)", flush=True)
            try:
                proc.kill()
            except OSError:
                pass
            kill_worktree_build(mine - {os.getpid(), os.getppid()})
            return "stalled"
    return "done" if proc.returncode == 0 else f"exit{proc.returncode}"


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("rebuild_args", nargs="*", help="passed through to rebuild.py")
    ap.add_argument("--stall", type=int, default=150,
                    help="seconds of no log growth at a link/lib step = stalled (default 150)")
    ap.add_argument("--max-attempts", type=int, default=4)
    ap.add_argument("--log", default="/tmp/rebuild-watchdog.log")
    a = ap.parse_args()
    mine = {os.getpid(), os.getppid()}
    for attempt in range(1, a.max_attempts + 1):
        print(f"[watchdog] attempt {attempt}/{a.max_attempts}: "
              f"rebuild.py {' '.join(a.rebuild_args)}", flush=True)
        result = run_once(a.rebuild_args, a.log, a.stall, mine)
        if result == "done" and "All done" in tail(a.log):
            print("[watchdog] rebuild completed cleanly", flush=True)
            return 0
        more = attempt < a.max_attempts
        print(f"[watchdog] attempt {attempt} -> {result}; "
              f"{'retrying' if more else 'giving up'}", flush=True)
    return 1


if __name__ == "__main__":
    sys.exit(main())
