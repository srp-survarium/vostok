#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""
vostok.build.ninja - run ninja.exe under Wine to build a Vostok target.

Default target is the game project: survarium_-_PC_-_DirectX_11
Runs with -v (verbose: full command lines) and -k 0 (keep going: report
every failure, don't stop at the first). Override either by passing your
own -k/-j (the last occurrence wins).

Usage:
  python3 -m vostok.build.ninja                  # build the game (verbose, keep-going)
  python3 -m vostok.build.ninja logging          # build just the logging project
  python3 -m vostok.build.ninja -t clean         # ninja flag (clean tool)
  python3 -m vostok.build.ninja -k 1             # stop at first failure

Required env vars (set by flake.nix devShell):
  NINJA_DIR  - directory containing ninja.exe
  WINEPREFIX - wine prefix initialised by vostok.tool.toolchain

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
build's stdout/stderr, so a caller that reads us through a pipe (`vostok build`,
an agent's shell) waits the full 10 minutes for EOF even though everything
exited long ago - that was a constant ~600s tax on every fresh-worktree
rebuild. We kill our prefix's mspdbsrv after every build; see
_kill_prefix_processes for why the kill must be prefix-scoped.
"""

import os
import signal
import shutil
import subprocess
import sys
import time
from contextlib import contextmanager
from pathlib import Path

from vostok.core.paths import BASE_EXE
from vostok.core.paths import BASE_PDB
from vostok.core.paths import NINJA_DIR as BUILD_DIR
from vostok.core.paths import WINEPREFIX as OWN_WINEPREFIX

DEFAULT_TARGET = "survarium_-_PC_-_DirectX_11"

# The shipping core library and game executable were compiled on different
# days.  MSVC 9 refuses to override the reserved __DATE__ macro, so reproduce
# the compiler environment instead of changing the recovered source spelling.
RETAIL_CORE_BUILD_TIME = "2013-05-11 12:00:00"
RETAIL_GAME_BUILD_TIME = "2013-05-09 12:00:00"

# Outputs the final link writes; the watchdog waits for both to be refreshed.
LINK_OUTPUTS = (
    BASE_EXE,
    BASE_PDB,
)
FINAL_LINK_RSP_MARKER = f"{DEFAULT_TARGET}_link.rsp".encode()
NINJA_OPTIONS_WITH_VALUE = frozenset(
    {"-C", "-d", "-f", "-j", "-k", "-l", "-t", "-w"}
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

# Build processes that Wine may detach from the launcher process group.  Keep
# this narrower than _BUILD_COMMS: it is used for interruption cleanup, where
# killing an unrelated persistent Wine service would be destructive.
_INTERRUPT_BUILD_COMMS = (
    "wine", "wine-preloader", "wine64-preloade", "ninja.exe", "cmd", "cmd.exe",
    "cl", "cl.exe", "link", "link.exe", "lib", "lib.exe",
    "mspdbsrv.exe", "c1.dll", "c2.dll", "conhost.exe",
)

# Processes that prove another build is actively using this worktree's prefix.
# Do not include Wine services or mspdbsrv.exe: they legitimately persist after
# a completed build and do not by themselves make a new invocation unsafe.
_ACTIVE_BUILD_COMMS = (
    "ninja", "ninja.exe", "cmd", "cmd.exe", "cl", "cl.exe",
    "link", "link.exe", "lib", "lib.exe",
)


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


def _wine_tree_jiffies(exclude_pids: frozenset[int] = frozenset()) -> int:
    """Sum utime+stime (jiffies) over the live Wine build/compiler/linker tree.

    Excludes the persistent wineserver session and any pre-existing build PIDs,
    so an orphan left by an older interrupted invocation cannot keep a healthy
    build's watchdog busy forever. It is also scoped to our WINEPREFIX so a
    sibling worktree's parallel build cannot mask this build's zombie. Remaining
    over-inclusion is harmless: we only read the delta.
    """
    total = 0
    for entry in Path("/proc").iterdir():
        if not entry.name.isdigit():
            continue
        if int(entry.name) in exclude_pids:
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


def _prefix_process_ids(comms: tuple[str, ...]) -> set[int]:
    """Return matching process IDs in this worktree's Wine prefix."""
    if not os.environ.get("WINEPREFIX"):
        return set()
    targets = {c.lower() for c in comms}
    result: set[int] = set()
    for entry in Path("/proc").iterdir():
        if not entry.name.isdigit():
            continue
        try:
            if (entry / "comm").read_text().strip().lower() not in targets:
                continue
            if _in_our_prefix(entry):
                result.add(int(entry.name))
        except (OSError, ValueError):
            continue
    return result


def _kill_prefix_processes(
    comms: tuple[str, ...], *, exclude_pids: frozenset[int] = frozenset(),
) -> None:
    """SIGKILL wine processes by comm name, scoped to THIS worktree's WINEPREFIX.

    Sibling worktrees build in parallel inside their own prefixes; a global
    pkill could hit a healthy cl/link in flight there, so match each candidate's
    /proc/<pid>/environ against our prefix. Best-effort: a process we can't
    kill is fine - the point is that we no longer BLOCK on it, not that it
    must die.
    """
    if not os.environ.get("WINEPREFIX"):
        return  # cannot scope the kill safely
    for pid in _prefix_process_ids(comms) - exclude_pids:
        try:
            os.kill(pid, signal.SIGKILL)
        except OSError:
            continue


def _stop_interrupted_build(proc: subprocess.Popen, existing_pids: set[int]) -> None:
    """Stop one interrupted Ninja run, including Wine-detached descendants.

    Wine reparents cl/link processes to the user manager and gives them their
    own process groups, so killing Ninja's group alone is insufficient.  The
    pre-spawn PID snapshot keeps cleanup scoped to processes created by this
    invocation and leaves older or unrelated jobs in the same prefix alone.
    """
    try:
        os.killpg(os.getpgid(proc.pid), signal.SIGKILL)
    except (ProcessLookupError, PermissionError):
        pass
    _kill_prefix_processes(
        _INTERRUPT_BUILD_COMMS,
        exclude_pids=frozenset(existing_pids),
    )
    try:
        proc.wait(timeout=30)
    except subprocess.TimeoutExpired:
        pass


def _raise_on_termination(signum: int, _frame) -> None:
    """Turn supervisor termination into an unwind that reaps Wine children."""
    raise SystemExit(128 + signum)


@contextmanager
def _termination_cleanup_scope():
    """Make SIGTERM/SIGHUP pass through the build's BaseException cleanup."""
    previous = {
        signum: signal.signal(signum, _raise_on_termination)
        for signum in (signal.SIGTERM, signal.SIGHUP)
    }
    try:
        yield
    finally:
        for signum, handler in previous.items():
            signal.signal(signum, handler)


def _assert_no_existing_build() -> None:
    """Refuse to overlap a build already using this worktree's Wine prefix."""
    pids = sorted(_prefix_process_ids(_ACTIVE_BUILD_COMMS))
    if pids:
        shown = ", ".join(map(str, pids[:12]))
        if len(pids) > 12:
            shown += f", ... ({len(pids) - 12} more)"
        die(
            "another build is already active in this worktree's Wine prefix",
            "active PIDs: " + shown,
            "wait for it to finish or clean up that exact stale invocation first",
        )


def _ninja_argv(
    ninja_exe: Path,
    args: list[str],
    build_time: str | None = None,
) -> list[str]:
    command = ["wine", str(ninja_exe), "-v", "-k", "0", *args]
    if build_time is None:
        return command
    if shutil.which("faketime") is None:
        die("faketime not found - re-enter `nix develop`")
    return ["faketime", build_time, *command]


def _ninja_env(build_time: str | None = None) -> dict[str, str] | None:
    if build_time is None:
        return None
    return dict(
        os.environ,
        FAKETIME_DONT_FAKE_MONOTONIC="1",
        FAKETIME_DONT_FAKE_STAT="1",
        WINEDEBUG="-all",
    )


def _stop_wine_session(build_time: str | None = None) -> None:
    """Stop only this WINEPREFIX before changing its emulated wall clock."""
    command = ["wineserver", "-k"]
    if build_time is not None:
        command = ["faketime", build_time, *command]
    try:
        subprocess.run(
            command,
            env=_ninja_env(build_time),
            stdout=subprocess.DEVNULL,
            stderr=subprocess.DEVNULL,
            timeout=30,
            check=False,
        )
    except (OSError, subprocess.TimeoutExpired):
        _kill_prefix_processes(tuple(_WINE_SESSION))


def _run_plain(
    ninja_exe: Path,
    args: list[str],
    build_time: str | None = None,
) -> int:
    """Run a module-only Ninja build with interruption-safe Wine cleanup."""
    _assert_no_existing_build()
    existing_pids = _prefix_process_ids(_INTERRUPT_BUILD_COMMS)
    with _termination_cleanup_scope():
        proc = subprocess.Popen(
            _ninja_argv(ninja_exe, args, build_time),
            cwd=str(BUILD_DIR),
            env=_ninja_env(build_time),
            start_new_session=True,
        )
        try:
            return proc.wait()
        except BaseException:
            _stop_interrupted_build(proc, existing_pids)
            raise


def _prepare_clean_final_pdb(
    ninja_exe: Path,
    args: list[str],
    build_time: str | None = None,
) -> None:
    """Remove the executable PDB only when Ninja has already scheduled its link.

    MSVC's linker reuses an existing output PDB even for a non-incremental LTCG
    link. Its TPI stream can therefore retain obsolete class records after a
    header correction: the new record is present, but the old one remains too.
    That makes a truthful PDB structure comparison report both source states.

    A dry-run tells us whether the final link is already dirty. Removing the PDB
    after that decision cannot create an extra link (Ninja does not list it as a
    separate output), but guarantees that a link which is happening anyway
    writes one clean type stream. If the PDB is unexpectedly absent, remove the
    EXE first so the normal output edge self-heals both files.
    """
    if "-t" in args:
        return

    if not BASE_PDB.exists() and BASE_EXE.exists():
        BASE_EXE.unlink()

    probe = subprocess.run(
        _ninja_argv(ninja_exe, ["-n", *args], build_time),
        cwd=str(BUILD_DIR),
        env=_ninja_env(build_time),
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        check=False,
    )
    output = probe.stdout or b""
    if probe.returncode != 0:
        print(
            "[ninja] warning: could not probe whether the final link is dirty; "
            "leaving the output PDB untouched",
            flush=True,
        )
        return
    if b"&& link " not in output or FINAL_LINK_RSP_MARKER not in output:
        return

    try:
        BASE_PDB.unlink()
    except FileNotFoundError:
        pass
    print(
        "[ninja] final link scheduled: starting it with a fresh output PDB",
        flush=True,
    )


def _explicit_targets(args: list[str]) -> list[str]:
    """Return Ninja targets, excluding option arguments such as ``-j 6``."""
    targets: list[str] = []
    skip_next = False
    options_done = False
    for arg in args:
        if skip_next:
            skip_next = False
            continue
        if options_done:
            targets.append(arg)
            continue
        if arg == "--":
            options_done = True
            continue
        if arg in NINJA_OPTIONS_WITH_VALUE:
            skip_next = True
            continue
        if arg.startswith("-"):
            continue
        targets.append(arg)
    return targets


def _ninja_options(args: list[str]) -> list[str]:
    """Preserve Ninja options while replacing its explicit build targets."""
    options: list[str] = []
    index = 0
    while index < len(args):
        arg = args[index]
        if arg == "--":
            break
        if not arg.startswith("-"):
            index += 1
            continue
        options.append(arg)
        if arg in NINJA_OPTIONS_WITH_VALUE and index + 1 < len(args):
            index += 1
            options.append(args[index])
        index += 1
    return options


def _run_with_watchdog(
    ninja_exe: Path,
    args: list[str],
    build_time: str | None = None,
) -> int:
    """Run the full-game ninja build, reaping the post-link Wine zombie wait."""
    _assert_no_existing_build()
    start = time.time()
    existing_pids = _prefix_process_ids(_INTERRUPT_BUILD_COMMS)
    # inherit stdout/stderr (no pipe) so the matcher still sees the -v output and
    # so leaked wine children can't hold a pipe fd open (that itself hangs EOF).
    with _termination_cleanup_scope():
        proc = subprocess.Popen(
            _ninja_argv(ninja_exe, args, build_time),
            cwd=str(BUILD_DIR),
            env=_ninja_env(build_time),
            start_new_session=True,
        )

        idle_seconds = 0.0
        old_pids = frozenset(existing_pids)
        prev_jiffies = _wine_tree_jiffies(old_pids)
        prev_t = time.time()
        reaped = False

        try:
            while True:
                rc = proc.poll()
                if rc is not None:
                    return rc  # normal exit (the overwhelmingly common path)

                time.sleep(POLL_SECONDS)
                now = time.time()
                jiffies = _wine_tree_jiffies(old_pids)
                elapsed = now - prev_t
                cores = ((jiffies - prev_jiffies) / (elapsed * CLK_TCK)
                         if elapsed > 0 else 0.0)
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
        except BaseException:
            _stop_interrupted_build(proc, existing_pids)
            raise

        # We broke out because the build is stuck. Reap only processes created by
        # this invocation. Return success only if we saw the clean outputs-ready +
        # idle signal; a hard-timeout is a genuine failure.
        _stop_interrupted_build(proc, existing_pids)
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
            "Run: python3 -m vostok tool toolchain",
        )

    # THIS worktree's prefix, always - an inherited WINEPREFIX from a sibling
    # checkout would run our compiles in THEIR prefix, where each session's
    # post-run mspdbsrv reap kills the other's in-flight cl (C1090/C2471,
    # LNK1318 RPC(23), seemingly at random).
    inherited = os.environ.get("WINEPREFIX")
    if inherited and Path(inherited) != OWN_WINEPREFIX:
        print(f"[ninja] ignoring inherited WINEPREFIX {inherited} "
              f"(another worktree's); using {OWN_WINEPREFIX}", flush=True)
    os.environ["WINEPREFIX"] = str(OWN_WINEPREFIX)

    # Silence unactionable Wine debug spam (fixme stubs + kerberos err) from
    # ninja.exe and the cl.exe/link.exe it spawns. setdefault so an explicit
    # WINEDEBUG (e.g. for debugging Wine itself) still wins.
    os.environ.setdefault("WINEDEBUG", "fixme-all,err-kerberos")

    # Native msvcr90 (so cl/link render PDB type names the retail way - see
    # vostok.build.native_crt) is provisioned into the prefix at setup via a
    # persistent registry override; a build needs nothing here.

    # Defaults for the matching workflow:
    #   -v      verbose - print the full cl.exe/link.exe command lines
    #   -k 0    keep going on errors - compile every TU and report all
    #           failures in one pass instead of stopping at the first
    # Both come before the user's args, so a later -k/-j on the command line
    # still wins (ninja takes the last occurrence).
    args = sys.argv[1:] or [DEFAULT_TARGET]

    # The stall watchdog only makes sense for the full-game build (the one that
    # relinks the EXE+PDB and so can hit the post-link zombie wait). A module-only
    # build (`vostok.build.ninja game_core`) doesn't relink and finishes fast, so run
    # it plainly.
    targets = _explicit_targets(args)
    full_build = (not targets) or (DEFAULT_TARGET in targets)
    date_sensitive = "-t" not in args
    if date_sensitive and (full_build or "core" in targets):
        _stop_wine_session()
        core_args = [*_ninja_options(args), "core"]
        rc = _run_plain(ninja_exe, core_args, RETAIL_CORE_BUILD_TIME)
        _kill_prefix_processes(("mspdbsrv.exe",))
        _stop_wine_session(RETAIL_CORE_BUILD_TIME)
        if rc != 0 or (targets == ["core"] and not full_build):
            sys.exit(rc)

    if full_build:
        build_time = RETAIL_GAME_BUILD_TIME if date_sensitive else None
        _prepare_clean_final_pdb(ninja_exe, args, build_time)
        rc = _run_with_watchdog(ninja_exe, args, build_time)
    else:
        rc = _run_plain(ninja_exe, args)

    # cl.exe/link.exe spawn mspdbsrv.exe (the PDB-writer daemon), which then
    # idles for ~10 minutes before exiting on its own. It inherits the build's
    # stdout/stderr fds, so any caller reading us through a pipe (`vostok build`,
    # an agent's shell) only sees EOF when mspdbsrv dies - a constant ~600s
    # stall per rebuild. The PDB is fully written once ninja returns, so
    # killing it here is safe; respawning next build costs ~a second.
    _kill_prefix_processes(("mspdbsrv.exe",))
    if full_build and date_sensitive:
        _stop_wine_session(RETAIL_GAME_BUILD_TIME)
    sys.exit(rc)


if __name__ == "__main__":
    main()
