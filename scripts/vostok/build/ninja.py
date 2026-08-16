#!/usr/bin/env python3
"""
ninja_build.py - Run ninja.exe under Wine to build a Vostok target.

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
  NINJA_DIR  - directory containing ninja.exe
  WINEPREFIX - wine prefix initialised by setup-toolchain.py

## Stall watchdog (full-game build only)

Wine intermittently leaves a `cl.exe`/`link.exe` child in a finished-but-never-
exits state: the linker has already written the EXE+PDB and succeeded, yet the
process never reaps, so ninja blocks on it and the whole rebuild appears to hang
for many minutes with zero CPU activity. That dead wait dwarfs the real ~1-2 min
of build work.

So for the full-game build we don't just block on `wine ninja`: we run it as a
child and watch (a) whether the EXE+PDB were freshly written and (b) the CPU of
the whole Wine compiler/linker tree. If the outputs are written AND the tree has
been idle (no real compute) for IDLE_LIMIT seconds while ninja still hasn't
returned, the build is done and we're only waiting on a zombie - so we reap the
Wine children and return success. A normal build never trips this: ninja exits
within ~1s of the link finishing, long before the idle timer fills.

Safety: we only declare done once BOTH the EXE and PDB mtimes have advanced past
build start (so we never proceed on a half-written EXE - that would show up as a
blown-up diff downstream); a real LTCG link keeps a core busy, so it never reads
as idle. Module-only builds (which don't relink the EXE) skip the watchdog.

## mspdbsrv reaping (every build)

cl.exe/link.exe spawn mspdbsrv.exe (the PDB-writer daemon), which idles for
~10 minutes after the last build before exiting on its own. It inherits the
build's stdout/stderr, so a caller that reads us through a pipe (rebuild.py,
an agent's shell) waits the full 10 minutes for EOF even though everything
exited long ago - that was a constant ~600s tax on every fresh-worktree
rebuild. We kill our prefix's mspdbsrv after every build; see
_kill_prefix_processes for why the kill must be prefix-scoped.
"""

import os
import signal
import subprocess
import sys
import time
from pathlib import Path

from vostok.core.paths import NINJA_DIR as BUILD_DIR
from vostok.core.paths import WIN32_DIR

DEFAULT_TARGET = "survarium_-_PC_-_DirectX_11"

# Outputs the final link writes; the watchdog waits for both to be refreshed.
LINK_OUTPUTS = (
    WIN32_DIR / "survarium-dx11-win32-gold.exe",
    WIN32_DIR / "survarium-dx11-win32-gold.pdb",
)

# Watchdog tuning.
POLL_SECONDS = 5
IDLE_LIMIT_SECONDS = 60          # idle-with-outputs-ready this long => zombie wait
IDLE_CPU_CORES = 0.15            # whole wine tree below this many cores = idle
HARD_TIMEOUT_SECONDS = 2400      # absolute ceiling, then give up (treat as failure)
CLK_TCK = os.sysconf("SC_CLK_TCK")

# comm names that are the persistent wineserver session, never the build itself.
_WINE_SESSION = {
    "wineserver", "services.exe", "winedevice.exe", "plugplay.exe",
    "svchost.exe", "rpcss.exe", "explorer.exe", "conhost.exe",
}
# substrings identifying a build (compiler/linker/ninja) process under wine.
_BUILD_COMMS = ("wine", "cl", "link", "ninja", "lib", "cmd", "mspdb", "c1", "c2")


def die(msg: str, *hints: str) -> None:
    print(f"[ninja] ERROR: {msg}", file=sys.stderr)
    for h in hints:
        print(f"  {h}", file=sys.stderr)
    sys.exit(1)


def _in_our_prefix(entry: Path) -> bool:
    """True if /proc/<pid> belongs to THIS worktree's WINEPREFIX (or no prefix
    is set, in which case scoping is impossible and we include everything)."""
    prefix = os.environ.get("WINEPREFIX")
    if not prefix:
        return True
    try:
        return (b"WINEPREFIX=" + prefix.encode()) in (entry / "environ").read_bytes().split(b"\0")
    except OSError:
        return False


def _wine_tree_jiffies() -> int:
    """Sum utime+stime (jiffies) over the live Wine build/compiler/linker tree.

    Excludes the persistent wineserver session so its idle background processes
    don't mask a stall, and is scoped to our WINEPREFIX so a sibling worktree's
    parallel build can't mask THIS build's zombie either (without the scoping,
    a stuck link here would wait out the busy sibling all the way to the hard
    timeout). Over-inclusion is harmless: we only read the DELTA.
    """
    total = 0
    for entry in Path("/proc").iterdir():
        if not entry.name.isdigit():
            continue
        try:
            comm = (entry / "comm").read_text().strip()
        except OSError:
            continue
        low = comm.lower()
        if low in _WINE_SESSION:
            continue
        if not any(k in low for k in _BUILD_COMMS):
            continue
        if not _in_our_prefix(entry):
            continue
        try:
            fields = (entry / "stat").read_text().rsplit(") ", 1)[1].split()
        except (OSError, IndexError):
            continue
        # after "(comm) " field 0 is state; utime=13, stime=14 in the full stat,
        # i.e. indices 11 and 12 here (we split off everything up to ") ").
        try:
            total += int(fields[11]) + int(fields[12])
        except (ValueError, IndexError):
            continue
    return total


def _outputs_refreshed(since: float) -> bool:
    """True once every link output's mtime has advanced past build start."""
    for out in LINK_OUTPUTS:
        try:
            if out.stat().st_mtime <= since:
                return False
        except OSError:
            return False
    return True


def _kill_prefix_processes(comms: tuple[str, ...]) -> None:
    """SIGKILL wine processes by comm name, scoped to THIS worktree's WINEPREFIX.

    Sibling worktrees build in parallel inside their own prefixes; a global
    pkill could hit a healthy cl/link in flight there, so match each candidate's
    /proc/<pid>/environ against our prefix. Best-effort: a process we can't
    kill is fine - the point is that we no longer BLOCK on it, not that it
    must die.
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


def _reap_wine_children() -> None:
    """Kill leftover wine compiler/linker processes (NOT the wineserver session)."""
    _kill_prefix_processes(("cl", "cl.exe", "link", "link.exe", "conhost.exe"))


def _run_with_watchdog(ninja_exe: Path, args: list[str]) -> int:
    """Run the full-game ninja build, reaping the post-link Wine zombie wait."""
    start = time.time()
    # inherit stdout/stderr (no pipe) so the matcher still sees the -v output and
    # so leaked wine children can't hold a pipe fd open (that itself hangs EOF).
    proc = subprocess.Popen(
        ["wine", str(ninja_exe), "-v", "-k", "0", *args],
        cwd=str(BUILD_DIR),
        start_new_session=True,
    )

    idle_seconds = 0.0
    prev_jiffies = _wine_tree_jiffies()
    prev_t = time.time()
    reaped = False

    while True:
        rc = proc.poll()
        if rc is not None:
            return rc  # normal exit (the overwhelmingly common path)

        time.sleep(POLL_SECONDS)
        now = time.time()
        jiffies = _wine_tree_jiffies()
        elapsed = now - prev_t
        cores = (jiffies - prev_jiffies) / (elapsed * CLK_TCK) if elapsed > 0 else 0.0
        prev_jiffies, prev_t = jiffies, now

        if cores < IDLE_CPU_CORES and _outputs_refreshed(start):
            idle_seconds += elapsed
        else:
            idle_seconds = 0.0

        if idle_seconds >= IDLE_LIMIT_SECONDS:
            print(
                f"[ninja] watchdog: EXE+PDB written and the wine tree has been idle "
                f"~{int(idle_seconds)}s while ninja has not returned - the link "
                f"finished but a child is stuck; reaping and proceeding.",
                flush=True,
            )
            reaped = True
            break

        if now - start > HARD_TIMEOUT_SECONDS:
            print(
                f"[ninja] watchdog: hard timeout ({HARD_TIMEOUT_SECONDS}s) with no "
                f"idle-completion signal; killing the build.",
                flush=True,
            )
            break

    # We broke out because the build is stuck. Kill our process group, then reap
    # the reparented wine children. Return success only if we saw the clean
    # outputs-ready + idle signal; a hard-timeout is a genuine failure.
    try:
        os.killpg(os.getpgid(proc.pid), signal.SIGKILL)
    except (ProcessLookupError, PermissionError):
        pass
    _reap_wine_children()
    try:
        proc.wait(timeout=30)
    except subprocess.TimeoutExpired:
        pass
    return 0 if reaped else 1


def main() -> None:
    ninja_dir = os.environ.get("NINJA_DIR")
    if not ninja_dir:
        die("NINJA_DIR not set - run from `nix develop`")

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
    #   -v      verbose - print the full cl.exe/link.exe command lines
    #   -k 0    keep going on errors - compile every TU and report all
    #           failures in one pass instead of stopping at the first
    # Both come before the user's args, so a later -k/-j on the command line
    # still wins (ninja takes the last occurrence).
    args = sys.argv[1:] or [DEFAULT_TARGET]

    # The stall watchdog only makes sense for the full-game build (the one that
    # relinks the EXE+PDB and so can hit the post-link zombie wait). A module-only
    # build (`ninja_build.py game_core`) doesn't relink and finishes fast, so run
    # it plainly.
    full_build = (not sys.argv[1:]) or (DEFAULT_TARGET in args)
    if full_build:
        rc = _run_with_watchdog(ninja_exe, args)
    else:
        rc = subprocess.run(
            ["wine", str(ninja_exe), "-v", "-k", "0", *args],
            cwd=str(BUILD_DIR),
        ).returncode

    # cl.exe/link.exe spawn mspdbsrv.exe (the PDB-writer daemon), which then
    # idles for ~10 minutes before exiting on its own. It inherits the build's
    # stdout/stderr fds, so any caller reading us through a pipe (rebuild.py,
    # an agent's shell) only sees EOF when mspdbsrv dies - a constant ~600s
    # stall per rebuild. The PDB is fully written once ninja returns, so
    # killing it here is safe; respawning next build costs ~a second.
    _kill_prefix_processes(("mspdbsrv.exe",))
    sys.exit(rc)


if __name__ == "__main__":
    main()
