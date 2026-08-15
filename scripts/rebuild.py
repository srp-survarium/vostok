#!/usr/bin/env python3
"""
rebuild.py - full base-side refresh after editing sources.

  1. Build survarium via ninja under Wine (scripts/ninja_build.py).
  2. Regenerate binaries/rich/base, then binaries/objdiff/base. The COFF symbol
     normalizer consumes the completed rich index, so those two steps must be
     ordered. binaries/structure/base remains disjoint and runs in parallel.
  3. Regenerate docs/binary_matching/match.db from the fresh report.json
     (match_db.regen()). rebuild.py is the canonical build step and owns the DB
     regen; `match_db.py refresh` is the regen-only path for an already-built
     report. A regen failure warns but does not fail the build.

The target side (binaries/structure/target, binaries/objdiff/target,
binaries/rich/target) is the original game and does not change between
recompiles; it is generated once on first `nix develop` (see setup-toolchain.py).

Each run appends one audit line to binaries/rebuild.log (git-ignored, mirrors
binaries/pdb_fetch.log):
    [<timestamp>][<git-branch>]: <elapsed>, <summary>
where <summary> reports the wall-clock and the set of engine modules whose TUs
ninja actually recompiled this run (a no-op rebuild = 0 modules).

Any extra args are forwarded to ninja_build.py:
  python3 scripts/rebuild.py            # build the game, then refresh base diff inputs
  python3 scripts/rebuild.py logging    # build just one project first
"""

import datetime
import os
import re
import select
import subprocess
import sys
import time
from concurrent.futures import ThreadPoolExecutor

import generate_delink
import generate_rich
import generate_structure
import regen_ninja


from vostok.core.paths import REBUILD_LOG as LOG_PATH
from vostok.core.paths import REPO as VOSTOK_DIR
from vostok.core.paths import REPORT_HEAD, SCRIPTS as SCRIPT_DIR

# A compiled TU shows up in ninja's verbose (-v) output as a cl command line that
# cd's into the module's source dir, e.g.
#   cmd /c cd "Z:\...\sources\vostok\game_core\sources" && cl @...rsp ...
# The "...\vostok\<module>\sources && cl" shape is unique to a recompile (link/lib
# edges run `link`/`lib`, not `cl`), so it counts TUs without double-counting the
# per-project link step. Path separators are backslashes under Wine.
_CL_MODULE_RE = re.compile(r"vostok[\\/]([A-Za-z0-9_]+)[\\/]sources\b[^\n]*?&&\s*cl\b")


def log(msg: str) -> None:
    print(f"[rebuild] {msg}", flush=True)


def die(msg: str) -> None:
    print(f"[rebuild] ERROR: {msg}", file=sys.stderr)
    sys.exit(1)


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


# After ninja_build.py exits, a blocking read of its output pipe may STILL not
# see EOF: wine children leaked by the build inherit the write end and hold it
# open. The worst offender was mspdbsrv.exe (link.exe's PDB-writer daemon),
# which idles for ~10 minutes before exiting on its own - that one stalled
# every fresh-worktree rebuild by a constant ~600s until ninja_build.py
# learned to kill it. Belt and braces here: read via select() with a timeout,
# and once the child has exited and the pipe has stayed silent this long,
# stop reading - nothing real is coming.
DRAIN_GRACE_SECONDS = 2.0


def run_ninja() -> set[str]:
    """Run ninja_build.py, streaming its output, and return the set of modules
    whose TUs were recompiled (parsed from the verbose cl command lines)."""
    modules: set[str] = set()
    proc = subprocess.Popen(
        [sys.executable, "-u", str(SCRIPT_DIR / "ninja_build.py"), *sys.argv[1:]],
        stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
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
        # mirror ninja_build.py's exit code so callers/watchdog see the failure,
        # but the audit line is still written by main()'s finally guard.
        raise subprocess.CalledProcessError(rc, "ninja_build.py")
    return modules


def _write_build_head() -> None:
    """Record WHICH source state this report.json was built from
    (binaries/objdiff/report.head: "<HEAD sha>[+dirty]"). match_db.py's
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


def main() -> None:
    start = time.monotonic()
    modules: set[str] = set()
    try:
        log("Refreshing ninja graph from the .vcprojs ...")
        regen_ninja.regenerate()

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
                    generate_delink.generate("base")
                    log("base COFF: OK")
                except Exception as e:  # noqa: BLE001 - report every step's failure
                    failures.append("base COFF")
                    log(f"base COFF: FAILED - {e}")

            try:
                structure.result()
                log("base structure: OK")
            except Exception as e:  # noqa: BLE001 - report every step's failure
                failures.append("base structure")
                log(f"base structure: FAILED - {e}")

        if failures:
            die(f"{len(failures)} step(s) failed: {', '.join(failures)}")
        _write_build_head()

        # rebuild.py is the canonical build step, so it also regenerates the
        # match.db from the report.json it just produced (the inverse of the old
        # model where match_db.py refresh shelled out to rebuild.py). A regen
        # failure is logged but does NOT fail the build - the diff inputs are
        # already good; the DB can be re-derived later with `match_db.py refresh`.
        try:
            import match_db
            match_db.regen()
            log("match.db regenerated.")
        # catch SystemExit too (regen's missing-artifact guard calls sys.exit) so
        # a DB hiccup never aborts a build whose diff inputs are already good.
        except (Exception, SystemExit) as e:  # noqa: BLE001 - never fail the build over the DB
            log(f"WARNING: match.db NOT regenerated ({e}); "
                "re-derive it with `python3 scripts/match_db.py refresh`")

        # ...and the README score block, the other report.json-derived artifact, so a
        # build keeps both current in one shot. Separately guarded: a score-write
        # failure warns but never fails the build (the report.json is already good).
        try:
            import match_score
            match_score.regen_readme()
            log("README score block refreshed.")
        except (Exception, SystemExit) as e:  # noqa: BLE001 - never fail the build over the README
            log(f"WARNING: README score block NOT refreshed ({e}); "
                "re-derive it with `python3 scripts/match_score.py --write-readme`")

        log("All done - base diff inputs refreshed.")
    finally:
        _append_log(time.monotonic() - start, modules)


if __name__ == "__main__":
    main()
