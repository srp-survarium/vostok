#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""
vostok.build.rebuild - full base-side refresh after editing sources.

  1. Build survarium via ninja under Wine (vostok.build.ninja).
  2. Regenerate binaries/rich/base, then binaries/objdiff/base. The COFF symbol
     normalizer consumes the completed rich index, so those two steps must be
     ordered. binaries/structure/base remains disjoint and runs in parallel.
  3. Re-derive the committed ledger config/match_state.tsv from
     the fresh report.json (vostok.derive.roster.regen()), and refresh README's
     score block from it. `vostok build` is the canonical build step and owns
     the regen; `vostok derive refresh` is the regen-only path for an
     already-built report. A regen failure warns but does not fail the build.

The target side (binaries/structure/target, binaries/objdiff/target,
binaries/rich/target) is the original game and does not change between
recompiles; it is generated once on first `nix develop` (see vostok.tool.toolchain).

Each run appends one audit line to binaries/rebuild.log (git-ignored, mirrors
binaries/pdb_fetch.log):
    [<timestamp>][<git-branch>]: <elapsed>, <summary>
where <summary> reports the wall-clock and the set of engine modules whose TUs
ninja actually recompiled this run (a no-op rebuild = 0 modules).

Any extra args are forwarded to vostok.build.ninja:
  python3 -m vostok build            # build the game, then refresh base diff inputs
  python3 -m vostok build logging    # build just one project first
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
from concurrent.futures import ThreadPoolExecutor

from vostok.build import generate_delink
from vostok.build import generate_rich
from vostok.build import generate_structure
from vostok.build import ninja_regen

from vostok.core import paths
from vostok.core.paths import REBUILD_LOG as LOG_PATH
from vostok.core.paths import REPO as VOSTOK_DIR
from vostok.core.paths import REPORT_HEAD
from vostok.core.log import logger
from vostok.core.log import die as core_die
from vostok.core import log as _log

# A compiled TU shows up in ninja's verbose (-v) output as a cl command line that
# cd's into the module's source dir, e.g.
#   cmd /c cd "Z:\...\sources\vostok\game_core\sources" && cl @...rsp ...
# The "...\vostok\<module>\sources && cl" shape is unique to a recompile (link/lib
# edges run `link`/`lib`, not `cl`), so it counts TUs without double-counting the
# per-project link step. Path separators are backslashes under Wine.
_CL_MODULE_RE = re.compile(r"vostok[\\/]([A-Za-z0-9_]+)[\\/]sources\b[^\n]*?&&\s*cl\b")


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
    """Best-effort audit line; a logging error must never fail the rebuild."""
    try:
        ts = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")[:-4]
        line = f"[{ts}][{_git_branch()}]: {_fmt_elapsed(elapsed)}, {_summarize(modules)}\n"
        LOG_PATH.parent.mkdir(parents=True, exist_ok=True)
        with open(LOG_PATH, "a", encoding="utf-8") as f:
            f.write(line)
    except Exception as e:  # noqa: BLE001
        log(f"(audit log skipped: {e})")


# After vostok.build.ninja exits, a blocking read of its output pipe may STILL not
# see EOF: wine children leaked by the build inherit the write end and hold it
# open. The worst offender was mspdbsrv.exe (link.exe's PDB-writer daemon),
# which idles for ~10 minutes before exiting on its own - that one stalled
# every fresh-worktree rebuild by a constant ~600s until vostok.build.ninja
# learned to kill it. Belt and braces here: read via select() with a timeout,
# and once the child has exited and the pipe has stayed silent this long,
# stop reading - nothing real is coming.
DRAIN_GRACE_SECONDS = 2.0


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
        # mirror vostok.build.ninja's exit code so callers/watchdog see the failure,
        # but the audit line is still written by main()'s finally guard.
        raise subprocess.CalledProcessError(rc, "vostok.build.ninja")
    return modules


def _write_build_head() -> None:
    """Record WHICH source state this report.json was built from
    (binaries/objdiff/report.head: "<HEAD sha>[+dirty]"). vostok derive's
    staleness guard compares it against the current HEAD - freshness is
    created here, not at DB refresh time."""
    try:
        head = subprocess.run(
            ["git", "rev-parse", "HEAD"], cwd=VOSTOK_DIR,
            capture_output=True, text=True, timeout=10,
        ).stdout.strip()
        dirty = subprocess.run(
            ["git", "status", "--porcelain", "--", "sources/"], cwd=VOSTOK_DIR,
            capture_output=True, text=True, timeout=10,
        ).stdout.strip()
        if head:
            marker = REPORT_HEAD
            marker.write_text(head + ("+dirty" if dirty else "") + "\n")
    except Exception as e:  # noqa: BLE001 - never fail the build over bookkeeping
        log(f"report.head not written: {e}")


def _acquire_build_lock():
    """Serialise builds sharing this worktree. Returns the held lock file.

    A worktree has ONE ninja output tree, one `binaries/objdiff/base`, one
    report.json - and, worst of all, one WINEPREFIX. `vostok.build.ninja` kills
    that prefix's `mspdbsrv.exe` after every ninja run, and the kill is
    prefix-scoped, not build-scoped: a second build compiling at that moment
    loses the PDB writer out from under `cl.exe`, which is a C1090 at best and
    a truncated PDB at worst. Two agents matching in one worktree is now normal,
    so waiting is the only safe answer - and it costs nothing that racing did
    not already cost, since the two builds could not have run in parallel
    correctly anyway.
    """
    # flock, deliberately: the kernel holds it against the file descriptor and
    # drops it when the process does - crash, SIGKILL, an agent stopped mid-build.
    # A stale lock is therefore not possible, which a PID file or a timestamp
    # lockfile could not promise. The file itself is never cleaned up and does
    # not need to be; it carries no state.
    lock = open(paths.BINARIES / ".build.lock", "w")
    try:
        fcntl.flock(lock, fcntl.LOCK_EX | fcntl.LOCK_NB)
    except OSError:
        log("another build holds this worktree - waiting for it to finish ...")
        fcntl.flock(lock, fcntl.LOCK_EX)
        log("lock acquired; starting.")
    return lock


def main() -> None:
    # `vostok build --help` must NOT reach ninja. Everything else here is
    # forwarded verbatim, and ninja's own `--help` exits 1 - which used to
    # print a bogus "ninja build failed" AND, worse, still ran ninja.main()'s
    # trailing prefix-scoped mspdbsrv kill, breaking a build running
    # concurrently in the same WINEPREFIX.
    if any(a in ("-h", "--help") for a in sys.argv[1:]):
        print(__doc__.strip())
        return

    lock = _acquire_build_lock()
    start = time.monotonic()
    modules: set[str] = set()
    try:
        log("Refreshing ninja graph from the .vcprojs ...")
        ninja_regen.regenerate()

        log("Building survarium via ninja ...")
        try:
            modules = run_ninja()
        except subprocess.CalledProcessError as e:
            die(f"ninja build failed (exit {e.returncode}); not regenerating diff inputs")

        log(
            f"Build OK ({_summarize(modules)}). Regenerating base rich index "
            "then COFF; base structure runs in parallel ..."
        )
        failures = []
        with ThreadPoolExecutor(max_workers=1) as ex:
            structure = ex.submit(generate_structure.generate, "base")
            try:
                generate_rich.generate("base")
                log("base rich index: OK")
            except Exception as e:  # noqa: BLE001 - report every step's failure
                failures.append("base rich index")
                log(f"base rich index: FAILED - {e}")

            if "base rich index" not in failures:
                try:
                    from vostok.data import pipeline as data_pipeline
                    data_pipeline.prepare_manifests()
                    log("consumer-owned data manifests: OK")
                except Exception as e:  # noqa: BLE001 - report every step's failure
                    failures.append("data manifests")
                    log(f"consumer-owned data manifests: FAILED - {e}")

            if "base rich index" not in failures:
                try:
                    # The established code project stays on the measured legacy
                    # delinker and never consumes data manifests.
                    generate_delink.generate("base")
                    log("base code COFF: OK")
                except Exception as e:  # noqa: BLE001 - report every step's failure
                    failures.append("base code COFF")
                    log(f"base code COFF: FAILED - {e}")

            if "data manifests" not in failures:
                try:
                    # Data ownership follows the current consumer graph, so
                    # both synthetic sides are regenerated in their separate
                    # objdiff project. Neither report can feed code/MAX.
                    generate_delink.generate(
                        "target", reports=False, data_project=True
                    )
                    log("target data COFF: OK")
                except Exception as e:  # noqa: BLE001 - report every step's failure
                    failures.append("target data COFF")
                    log(f"target data COFF: FAILED - {e}")

            if not {"data manifests", "target data COFF"} & set(failures):
                try:
                    generate_delink.generate("base", data_project=True)
                    log("base data COFF and reports: OK")
                except Exception as e:  # noqa: BLE001 - report every step's failure
                    failures.append("base data COFF")
                    log(f"base data COFF and reports: FAILED - {e}")

            try:
                structure.result()
                log("base structure: OK")
            except Exception as e:  # noqa: BLE001 - report every step's failure
                failures.append("base structure")
                log(f"base structure: FAILED - {e}")

        if failures:
            die(f"{len(failures)} step(s) failed: {', '.join(failures)}")
        _write_build_head()

        # Data is a separate image-level ledger: refreshing it must never alter
        # or gate the function report while the census is being calibrated.
        try:
            from vostok.data import pipeline as data_pipeline
            data_pipeline.refresh()
            log("image-data ledger refreshed (shadow mode).")
        except (Exception, SystemExit) as e:  # noqa: BLE001 - independent shadow lane
            log(f"WARNING: image-data ledger NOT refreshed ({e}); "
                "refresh it with `python3 -m vostok data refresh`")

        # `vostok build` is the canonical build step, so it also re-derives the
        # committed ledger from the report.json it just produced (the inverse of
        # the old model where vostok derive refresh shelled out to `vostok build`).
        # A regen failure is logged but does NOT fail the build - the diff inputs
        # are already good, and `vostok derive refresh` can re-derive later.
        try:
            from vostok.derive import roster
            roster.regen()
            log("matching ledger re-derived.")
        # catch SystemExit too (regen's missing-artifact guard calls sys.exit) so
        # a derivation hiccup never aborts a build whose diff inputs are good.
        except (Exception, SystemExit) as e:  # noqa: BLE001 - never fail the build over the ledger
            log(f"WARNING: matching ledger NOT re-derived ({e}); "
                "re-derive it with `python3 -m vostok derive refresh`")

        # The direct relocation audits consume the final function ledger, so
        # refresh every ledger module after roster.regen rather than with the
        # earlier image ledger. The aggregate is the complete datum-use
        # denominator; its zero gate remains an explicit `vostok data check
        # --gate` while the queue is being drained.
        try:
            from vostok.data import gate as data_gate
            data_report = data_gate.refresh()
            log(
                "all-module data relocation audit refreshed: "
                f"{data_report['summary']['modules']:,} modules, "
                f"{data_report['summary']['open_function_data']:,} open "
                "function datum-use rows."
            )
        except (Exception, SystemExit) as e:  # noqa: BLE001 - independent data lane
            log(f"WARNING: all-module data relocation audit NOT refreshed ({e}); "
                "refresh it with `python3 -m vostok data module-relocs all`")

        # ...and the README score block, the other report.json-derived artifact, so a
        # build keeps both current in one shot. Separately guarded: a score-write
        # failure warns but never fails the build (the report.json is already good).
        try:
            from vostok.ledger import readme
            readme.regen_readme()
            log("README score block refreshed.")
        except (Exception, SystemExit) as e:  # noqa: BLE001 - never fail the build over the README
            log(f"WARNING: README score block NOT refreshed ({e}); "
                "re-derive it with `python3 -m vostok ledger readme --write-readme`")

        log("All done - base diff inputs refreshed.")
    finally:
        _append_log(time.monotonic() - start, modules)
        lock.close()          # releases the flock; the kernel would anyway


if __name__ == "__main__":
    raise SystemExit(_log.run("vostok.build.rebuild", main))
