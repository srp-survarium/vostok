# SPDX-License-Identifier: GPL-3.0-or-later
"""
vostok.build.gfx_mspdbsrv - mspdbsrv-stall-free `wine cmd /c cl @rsp` for the direct GFx drivers.

The direct GFx build drivers (build_libgfx_direct.py / build_gfx_lib_direct.py)
compile one TU per `wine cmd /c cl @rsp` and read its result through a captured
pipe. cl.exe spawns mspdbsrv.exe (the PDB-writer daemon - even under /Z7 the /FD
+ /Fd"vc90.pdb" minimal-PDB write touches it), which then idles ~10 MINUTES after
the compile before exiting on its own. mspdbsrv inherits the compile's stdout/stderr
fds, so a parent that captures output (subprocess.run(capture_output=True)) only
sees pipe EOF when mspdbsrv finally dies - so EVERY fresh TU "compiles in seconds,
then hangs ~10 min". That made the 8-lib direct build effectively never finish.

vostok.build.ninja already solved this for the in-graph build (PR #280): kill the
worktree-prefix's mspdbsrv.exe after the build so the pipe EOFs. The direct
drivers bypass vostok.build.ninja, so we port the same prefix-scoped reap here, but
per-TU: spawn the compile in its own session writing output to a FILE (no
inherited pipe to block the parent), poll for the expected .obj, and once the obj
is written (or the process exits) kill the prefix's mspdbsrv.exe + reap the wine
children so the wait returns immediately. A fresh TU now finishes in seconds.

The kill is scoped to THIS worktree's WINEPREFIX (same reasoning as vostok.build.ninja:
a sibling worktree may have a healthy build in flight in its own prefix).
"""

import os
import signal
import subprocess
import tempfile
import time
from pathlib import Path

# Poll cadence and how long after the obj appears (or proc exit) we wait before
# force-killing mspdbsrv to release the pipe. The compile itself is already done
# by then; this is purely the daemon-shutdown wait we refuse to pay.
POLL_SECONDS = 0.5
GRACE_AFTER_OBJ = 1.0          # let cl flush + exit cleanly before we reap mspdbsrv
HARD_TIMEOUT_SECONDS = 900     # a genuinely stuck cl (not just mspdbsrv) ceiling


def _in_our_prefix(entry: Path) -> bool:
    """True if /proc/<pid> belongs to THIS worktree's WINEPREFIX (or none is set)."""
    prefix = os.environ.get("WINEPREFIX")
    if not prefix:
        return True
    try:
        return (b"WINEPREFIX=" + prefix.encode()) in (entry / "environ").read_bytes().split(b"\0")
    except OSError:
        return False


def _kill_prefix_processes(comms):
    """SIGKILL wine processes by comm name, scoped to THIS worktree's WINEPREFIX.

    Mirrors vostok.build.ninja: a global pkill could hit a healthy cl/link in a
    sibling worktree's prefix, so match each candidate's environ against ours.
    Best-effort - the point is to stop BLOCKING on the process, not that it dies.
    """
    if not os.environ.get("WINEPREFIX"):
        return  # cannot scope the kill safely
    targets = {c.lower() for c in comms}
    for entry in Path("/proc").iterdir():
        if not entry.name.isdigit():
            continue
        try:
            if (entry / "comm").read_text().strip().lower() not in targets:
                continue
            if not _in_our_prefix(entry):
                continue
            os.kill(int(entry.name), signal.SIGKILL)
        except (OSError, ValueError):
            continue


def kill_mspdbsrv():
    """Reap this prefix's mspdbsrv.exe (releases any pipe it is holding open)."""
    _kill_prefix_processes(("mspdbsrv.exe",))


def wine_cl(cmdline, cwd, obj_path):
    """Run `wine cmd /c <cmdline>`, reaping the post-compile mspdbsrv pipe-hold.

    The compile is spawned in its own session with stdout/stderr redirected to a
    temp file (NOT an inherited pipe - that is what blocks on mspdbsrv's lingering
    fd). We poll for `obj_path`; once it appears (the compile finished) we give cl
    a short grace to exit, then kill the prefix's mspdbsrv.exe so the wait returns
    in seconds instead of ~10 minutes.

    Returns a CompletedProcess-like object with `.returncode`, `.stdout`, `.stderr`
    (stderr is folded into stdout - cl writes diagnostics to stdout under cmd /c).
    """
    env = dict(os.environ)
    env.setdefault("WINEDEBUG", "fixme-all,err-kerberos")

    with tempfile.TemporaryFile(mode="w+", encoding="utf-8", errors="replace") as out:
        proc = subprocess.Popen(
            ["wine", "cmd", "/c", cmdline],
            cwd=str(cwd),
            env=env,
            stdout=out,
            stderr=subprocess.STDOUT,
            stdin=subprocess.DEVNULL,
            start_new_session=True,
        )

        start = time.time()
        obj_seen_at = None
        reaped = False
        while True:
            rc = proc.poll()
            if rc is not None:
                break  # cl actually exited (output file may already be flushed)

            time.sleep(POLL_SECONDS)

            # Once the obj is on disk the compile is done; the only thing left to
            # wait on is the mspdbsrv daemon holding the pipe. Reap it.
            try:
                done = obj_path.is_file() and obj_path.stat().st_size > 0
            except OSError:
                done = False
            if done:
                if obj_seen_at is None:
                    obj_seen_at = time.time()
                elif time.time() - obj_seen_at >= GRACE_AFTER_OBJ:
                    kill_mspdbsrv()
                    reaped = True
                    break

            if time.time() - start > HARD_TIMEOUT_SECONDS:
                break

        if reaped:
            # mspdbsrv was killed; cl (cmd) should now unblock and exit. Give it a
            # moment, then force-reap the wine compiler children if it lingers.
            try:
                proc.wait(timeout=15)
            except subprocess.TimeoutExpired:
                _kill_prefix_processes(("cl.exe", "cmd.exe", "conhost.exe",
                                        "c1.dll", "c2.dll"))
                try:
                    os.killpg(os.getpgid(proc.pid), signal.SIGKILL)
                except (ProcessLookupError, PermissionError):
                    pass
                try:
                    proc.wait(timeout=15)
                except subprocess.TimeoutExpired:
                    pass

        out.seek(0)
        text = out.read()

    return subprocess.CompletedProcess(
        args=cmdline, returncode=proc.returncode or 0, stdout=text, stderr="")
