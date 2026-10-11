# SPDX-License-Identifier: GPL-3.0-or-later
"""vostok tool worktree - a fixed pool of reusable build worktrees.

A fresh worktree costs ~15 GB and a full 40-50 minute build. The pool keeps
POOL_SIZE worktrees beside the main checkout (vostok-w1..vostok-w4 by default)
whose binaries/ survive between tasks, so switching one to another branch and
building recompiles only what changed (header edits included, through the
compiler-recorded deps).

    vostok tool worktree run BRANCH [--owner NAME] -- COMMAND...
        claim a free worktree (waiting while all are busy), switch it to
        BRANCH, run COMMAND inside it and release it - also on failure
    vostok tool worktree claim BRANCH [--owner NAME] [--from START] [--detach]
        take a free worktree for interactive work and print its path
    vostok tool worktree release [WORKTREE] [--owner NAME] [--save|--discard]
    vostok tool worktree status [--verify]
    vostok tool worktree gc [--apply]
        remove NON-pool worktrees that are clean, pushed and idle

A claim is a JSON lock file under .git/vostok-pool/ (owner, branch, pid,
time), created atomically under a pool-wide flock. A lock whose recorded
process is gone, or that is older than VOSTOK_POOL_STALE_HOURS (default 24),
is freed automatically unless a build still holds the worktree's build lock.
A new pool worktree is created lazily, seeded from the main checkout's
binaries/ without the build outputs, logs and release scratch it regenerates
or never reads; the shared cache (vostok.build.shared_cache) links the rest
at its first setup.
"""

from __future__ import annotations

import argparse
import contextlib
import fcntl
import json
import os
import signal
import socket
import subprocess
import time
from dataclasses import dataclass
from pathlib import Path

from vostok.core import log as _log
from vostok.core import paths
from vostok.core.log import logger

log = logger("worktree")

DEFAULT_START = "origin/v0.10b"
STALE_HOURS = float(os.environ.get("VOSTOK_POOL_STALE_HOURS", "24"))
WAIT_POLL_SECONDS = 20
GC_IDLE_HOURS = 24


class PoolError(RuntimeError):
    pass


# --- git --------------------------------------------------------------------

def git(*args: str, cwd: Path | None = None, check: bool = True,
        env: dict | None = None) -> str:
    result = subprocess.run(["git", *args], cwd=str(cwd or paths.MAIN_WORKTREE),
                            capture_output=True, text=True, env=env)
    if check and result.returncode:
        raise PoolError(f"git {' '.join(args)}: {result.stderr.strip() or result.stdout.strip()}")
    return result.stdout.strip()


def worktrees() -> list[dict]:
    """`git worktree list --porcelain`, one dict per worktree."""
    rows, row = [], {}
    for line in git("worktree", "list", "--porcelain").splitlines() + [""]:
        if not line:
            if row:
                rows.append(row)
            row = {}
            continue
        field, _, value = line.partition(" ")
        row[field] = value or True
    return rows


def dirty(path: Path) -> list[str]:
    """Tracked changes and untracked files; ignored artifacts do not count."""
    return git("status", "--porcelain", cwd=path).splitlines()


def build_running(path: Path) -> bool:
    lock = path / paths.BUILD_LOCK.relative_to(paths.REPO)
    if not lock.is_file():
        return False
    with lock.open("a") as handle:
        try:
            fcntl.flock(handle, fcntl.LOCK_EX | fcntl.LOCK_NB)
        except BlockingIOError:
            return True
        fcntl.flock(handle, fcntl.LOCK_UN)
    return False


def wait_for_build(path: Path) -> None:
    """Block until no build holds the worktree (a launched background build)."""
    lock = path / paths.BUILD_LOCK.relative_to(paths.REPO)
    if lock.is_file():
        with lock.open("a") as handle:
            fcntl.flock(handle, fcntl.LOCK_EX)


# --- the pool ---------------------------------------------------------------

def pool_paths() -> list[Path]:
    return [paths.POOL_ROOT / f"{paths.POOL_PREFIX}{index}"
            for index in range(1, paths.POOL_SIZE + 1)]


def _lock_path(path: Path) -> Path:
    return paths.POOL_LOCKS / f"{path.name}.json"


@contextlib.contextmanager
def _pool_mutex():
    paths.POOL_LOCKS.mkdir(parents=True, exist_ok=True)
    with (paths.POOL_LOCKS / ".pool.lock").open("a") as handle:
        fcntl.flock(handle, fcntl.LOCK_EX)
        yield


def read_lock(path: Path) -> dict | None:
    try:
        return json.loads(_lock_path(path).read_text(encoding="utf-8"))
    except FileNotFoundError:
        return None
    except (OSError, ValueError):
        return {"owner": "?", "branch": "?", "time": 0, "corrupt": True}


def _pid_alive(pid: int) -> bool:
    try:
        os.kill(pid, 0)
    except ProcessLookupError:
        return False
    except PermissionError:
        return True
    return True


def stale_reason(path: Path, lock: dict, now: float | None = None) -> str | None:
    """Why `lock` no longer protects anything, or None while it is live."""
    if build_running(path):
        return None
    now = time.time() if now is None else now
    pid, host = lock.get("pid"), lock.get("host")
    if pid and host == socket.gethostname() and not _pid_alive(int(pid)):
        return f"owner process {pid} is gone"
    age = now - float(lock.get("time", 0))
    if age > STALE_HOURS * 3600:
        return f"claimed {age / 3600:.1f}h ago (limit {STALE_HOURS:g}h)"
    return None


def _free_stale_locks() -> None:
    for path in pool_paths():
        lock = read_lock(path)
        if lock is None:
            continue
        reason = stale_reason(path, lock)
        if reason:
            _lock_path(path).unlink(missing_ok=True)
            log(f"freed stale claim on {path.name} by {lock.get('owner')} "
                f"({lock.get('branch')}): {reason}")


def _write_lock(path: Path, record: dict) -> None:
    descriptor = os.open(_lock_path(path), os.O_CREAT | os.O_EXCL | os.O_WRONLY, 0o644)
    with os.fdopen(descriptor, "w", encoding="utf-8") as handle:
        json.dump(record, handle, indent=1)
        handle.write("\n")


def _registered() -> set[Path]:
    return {Path(row["worktree"]) for row in worktrees() if "worktree" in row}


def _current_branch(path: Path) -> str:
    return git("rev-parse", "--abbrev-ref", "HEAD", cwd=path, check=False)


def _seed(path: Path, source: Path) -> None:
    """Copy the source checkout's reusable binaries/ (the worktree recipe).

    Win32 and the ninja logs are left out: the new tree's absolute paths make
    its first build a full one anyway. The shared cache links prebuilt libs and
    retail evidence at the first setup, so they are not copied either.
    """
    src = source / paths.BINARIES.relative_to(paths.REPO)
    if not src.is_dir():
        log(f"{source} has no binaries/ to seed from; the first setup builds everything")
        return
    excludes = ["*.log", ".ninja_log", ".ninja_deps", ".ninja_lock", ".build.lock",
                f"/{paths.LIBS_RELEASE_ZIPS}"]
    excludes += [f"/{p.relative_to(paths.BINARIES).as_posix()}" for p in paths.POOL_SEED_EXCLUDES]
    command = ["rsync", "-a", *(f"--exclude={item}" for item in excludes),
               f"{src}/", f"{path / paths.BINARIES.relative_to(paths.REPO)}/"]
    log(f"seeding {path.name}/binaries from {source} ...")
    subprocess.run(command, check=True)


def _create(path: Path, start: str, seed_from: Path) -> None:
    log(f"creating pool worktree {path} at {start}")
    git("worktree", "add", "--detach", str(path), start)
    _seed(path, seed_from)


def _switch(path: Path, branch: str, start: str | None, detach: bool) -> None:
    if detach:
        git("switch", "--detach", branch, cwd=path)
        return
    if start is not None:
        git("switch", "-c", branch, start, cwd=path)
        return
    if git("rev-parse", "--verify", "--quiet", f"refs/heads/{branch}", check=False):
        git("switch", branch, cwd=path)
        return
    remote = f"refs/remotes/origin/{branch}"
    if not git("rev-parse", "--verify", "--quiet", remote, check=False):
        git("fetch", "origin", branch, check=False)
    if git("rev-parse", "--verify", "--quiet", remote, check=False):
        git("switch", "--track", "-c", branch, f"origin/{branch}", cwd=path)
        return
    raise PoolError(f"no branch {branch!r} locally or on origin; pass --from START to create it")


@dataclass
class Claim:
    path: Path
    record: dict


def _same_claim(lock: dict | None, owner: str, branch: str, pid: int | None) -> bool:
    return (lock is not None and lock.get("owner") == owner
            and lock.get("branch") == branch and lock.get("pid") == pid)


def _pick(branch: str, owner: str, wanted: Path | None, force: bool,
          pid: int | None) -> tuple[Path | None, list[str]]:
    """Choose a worktree under the pool mutex; returns (path, reasons skipped)."""
    registered, skipped = _registered(), []
    candidates = [p for p in pool_paths() if wanted is None or p == wanted]
    if wanted is not None and not candidates:
        raise PoolError(f"{wanted} is not a pool worktree ({', '.join(p.name for p in pool_paths())})")
    free = []
    for path in candidates:
        lock = read_lock(path)
        if _same_claim(lock, owner, branch, pid):
            return path, skipped                    # an idempotent re-claim
        if lock is not None and not force:
            skipped.append(f"{path.name}: held by {lock.get('owner')} ({lock.get('branch')})")
            continue
        if path not in registered:
            if path.exists():
                skipped.append(f"{path.name}: exists but is not a worktree of this repository")
                continue
            free.append((2, path))                  # create lazily, last resort
            continue
        if build_running(path):
            skipped.append(f"{path.name}: a build is running")
            continue
        changes = dirty(path)
        if changes:
            skipped.append(f"{path.name}: dirty ({len(changes)} changes, e.g. {changes[0].strip()})")
            continue
        free.append((0 if _current_branch(path) == branch else 1, path))
    if not free:
        if wanted is not None:
            raise PoolError(f"cannot claim {skipped[0]}" if skipped else f"cannot claim {wanted.name}")
        return None, skipped
    return min(free, key=lambda item: (item[0], pool_paths().index(item[1])))[1], skipped


def claim(branch: str, owner: str, *, start: str | None = None, detach: bool = False,
          force: bool = False, wanted: Path | None = None, pid: int | None = None,
          command: list[str] | None = None, seed_from: Path | None = None) -> Claim | None:
    """Take a pool worktree and switch it to `branch`; None while all are busy."""
    with _pool_mutex():
        _free_stale_locks()
        path, skipped = _pick(branch, owner, wanted, force, pid)
        if path is None:
            if wanted is not None or skipped:
                for line in skipped:
                    log(f"  {line}")
            return None
        lock = read_lock(path)
        if _same_claim(lock, owner, branch, pid):
            lock["time"] = time.time()
            _lock_path(path).write_text(json.dumps(lock, indent=1) + "\n", encoding="utf-8")
            log(f"{path.name} is already claimed by {owner} for {branch}; refreshed")
            return Claim(path, lock)
        if lock is not None:
            log(f"--force: taking {path.name} from {lock.get('owner')} ({lock.get('branch')})")
            _lock_path(path).unlink(missing_ok=True)
        record = {"owner": owner, "branch": branch, "pid": pid, "host": socket.gethostname(),
                  "time": time.time(), "claimed": time.strftime("%Y-%m-%d %H:%M:%S"),
                  "command": command}
        _write_lock(path, record)
    try:
        if not path.exists():
            _create(path, start or DEFAULT_START, seed_from or paths.MAIN_WORKTREE)
        _switch(path, branch, start, detach)
    except BaseException:
        _lock_path(path).unlink(missing_ok=True)
        raise
    log(f"claimed {path} for {owner}: {branch} @ {git('rev-parse', '--short', 'HEAD', cwd=path)}")
    return Claim(path, record)


def _save_changes(path: Path, note: str) -> str:
    """Record tracked and untracked changes as a commit under refs/vostok-pool/."""
    index = path / ".git-vostok-pool-index"
    env = {**os.environ, "GIT_INDEX_FILE": str(index)}
    try:
        git("read-tree", "HEAD", cwd=path, env=env)
        git("add", "-A", cwd=path, env=env)
        tree = git("write-tree", cwd=path, env=env)
    finally:
        index.unlink(missing_ok=True)
    commit = git("commit-tree", tree, "-p", "HEAD", "-m", note, cwd=path)
    ref = f"refs/vostok-pool/{path.name}/{time.strftime('%Y%m%d-%H%M%S')}"
    git("update-ref", ref, commit, cwd=path)
    return ref


def release(path: Path, *, owner: str | None = None, save: bool = False,
            discard: bool = False, force: bool = False) -> str | None:
    """Free a pool worktree, leaving it clean and detached; returns a saved ref."""
    lock = read_lock(path)
    if lock is None:
        log(f"{path.name} was not claimed")
    elif owner and lock.get("owner") != owner and not force:
        raise PoolError(f"{path.name} is held by {lock.get('owner')}, not {owner}; pass --force")
    if build_running(path):
        raise PoolError(f"a build is running in {path.name}; release it when the build ends")
    saved = None
    if path.exists():
        changes = dirty(path)
        if changes and not (save or discard):
            raise PoolError(f"{path.name} has {len(changes)} uncommitted changes "
                            f"(e.g. {changes[0].strip()}); commit them, or pass --save / --discard")
        if changes and save:
            saved = _save_changes(path, f"vostok-pool: {path.name} "
                                        f"{lock.get('branch') if lock else '?'} at release")
            log(f"saved {len(changes)} uncommitted changes as {saved}")
        if changes:
            git("reset", "--hard", "-q", cwd=path)
            git("clean", "-fdq", cwd=path)
        # Detach so the branch can be checked out elsewhere; files are untouched.
        git("switch", "--detach", "-q", cwd=path)
    _lock_path(path).unlink(missing_ok=True)
    log(f"released {path.name}")
    return saved


def _resolve(name: str | None, owner: str | None) -> Path:
    """The named pool worktree, else the one the cwd is in, else the owner's."""
    if name:
        candidate = Path(name)
        for path in pool_paths():
            if candidate.name == path.name or candidate.resolve() == path.resolve():
                return path
        raise PoolError(f"{name} is not a pool worktree")
    here = Path.cwd().resolve()
    for path in pool_paths():
        if here == path.resolve() or path.resolve() in here.parents:
            return path
    held = [p for p in pool_paths() if (read_lock(p) or {}).get("owner") == owner]
    if len(held) != 1:
        raise PoolError(f"{owner} holds {len(held)} pool worktrees; name the one to release")
    return held[0]


def default_owner() -> str:
    return os.environ.get("VOSTOK_POOL_OWNER") or os.environ.get("USER", "agent")


# --- run --------------------------------------------------------------------

def child_env(path: Path) -> dict[str, str]:
    """The caller's environment re-pointed at the pool worktree.

    vostok resolves its repository from the `scripts/` it was imported from,
    so an inherited PYTHONPATH naming another checkout would build THAT one.
    """
    env = dict(os.environ)
    scripts = str(path / paths.SCRIPTS.relative_to(paths.REPO))
    parts = [p for p in env.get("PYTHONPATH", "").split(os.pathsep)
             if p and not (Path(p) / "vostok" / "core" / "paths.py").is_file()]
    env.update(VOSTOK_DIR=str(path), PYTHONPATH=os.pathsep.join([scripts, *parts]),
               WINEPREFIX=str(path / paths.WINEPREFIX.relative_to(paths.REPO)))
    # A Codex thread id would turn `vostok build` into a launch that returns at
    # once; the pool must hold the worktree until the build is really over.
    env.pop("CODEX_THREAD_ID", None)
    return env


def run(branch: str, command: list[str], *, owner: str, start: str | None = None,
        detach: bool = False, timeout: float | None = None) -> int:
    started, announced = time.monotonic(), 0.0
    while True:
        held = claim(branch, owner, start=start, detach=detach, pid=os.getpid(),
                     command=command)
        if held is not None:
            break
        waited = time.monotonic() - started
        if timeout is not None and waited > timeout:
            raise PoolError(f"no pool worktree became free within {timeout:.0f}s")
        if waited - announced >= 300 or announced == 0:
            log(f"all {paths.POOL_SIZE} pool worktrees are busy; waiting "
                f"({waited / 60:.0f} min so far) ...")
            announced = waited or 1e-9
        time.sleep(WAIT_POLL_SECONDS)

    process = None

    def forward(signum, _frame):
        if process is not None and process.poll() is None:
            process.send_signal(signum)
        else:
            raise KeyboardInterrupt

    previous = {s: signal.signal(s, forward) for s in (signal.SIGINT, signal.SIGTERM, signal.SIGHUP)}
    code = 1
    try:
        log(f"running in {held.path}: {' '.join(command)}")
        process = subprocess.Popen(command, cwd=str(held.path), env=child_env(held.path))
        code = process.wait()
        log(f"command exited {code} after {_log.elapsed(time.monotonic() - started)}")
    finally:
        for signum, handler in previous.items():
            signal.signal(signum, handler)
        wait_for_build(held.path)
        try:
            release(held.path, owner=owner, save=True)
        except PoolError as error:
            log(f"WARNING: {error}")
    return code


# --- gc ---------------------------------------------------------------------

def _idle_hours(path: Path, gitdir: Path) -> float:
    stamps = [gitdir / "HEAD", gitdir / "index", path / paths.BUILD_LOCK.relative_to(paths.REPO),
              path / paths.REBUILD_LOG.relative_to(paths.REPO)]
    newest = max((s.stat().st_mtime for s in stamps if s.exists()), default=0)
    return (time.time() - newest) / 3600


def gc_candidates(idle_hours: float = GC_IDLE_HOURS) -> list[dict]:
    """Every non-pool, non-main worktree with the verdict on removing it."""
    pool = {p.resolve() for p in pool_paths()}
    rows = []
    for row in worktrees():
        path = Path(row["worktree"])
        if path.resolve() == paths.MAIN_WORKTREE.resolve() or path.resolve() in pool:
            continue
        branch = row.get("branch", "").removeprefix("refs/heads/") or None
        verdict = {"path": path, "branch": branch or "(detached)", "head": row.get("HEAD", "")[:9],
                   "remove": False, "why": ""}
        rows.append(verdict)
        if "locked" in row:
            verdict["why"] = "locked (git worktree lock)"
            continue
        if not path.is_dir():
            verdict["why"] = "missing on disk (git worktree prune)"
            continue
        if build_running(path):
            verdict["why"] = "a build is running"
            continue
        changes = dirty(path)
        if changes:
            verdict["why"] = f"dirty ({len(changes)} changes)"
            continue
        head = row.get("HEAD", "")
        merged = subprocess.run(["git", "merge-base", "--is-ancestor", head, DEFAULT_START],
                                cwd=str(paths.MAIN_WORKTREE)).returncode == 0
        pushed = bool(git("branch", "-r", "--contains", head, check=False))
        if not (merged or pushed):
            verdict["why"] = "has commits on no remote branch"
            continue
        gitdir = Path(git("rev-parse", "--absolute-git-dir", cwd=path))
        idle = _idle_hours(path, gitdir)
        if idle < idle_hours:
            verdict["why"] = f"active {idle:.1f}h ago (idle limit {idle_hours:g}h)"
            continue
        verdict["remove"] = True
        verdict["why"] = ("merged into " + DEFAULT_START) if merged else "pushed"
    return rows


def gc(apply: bool, idle_hours: float) -> int:
    rows = gc_candidates(idle_hours)
    for row in rows:
        mark = "remove" if row["remove"] else "keep"
        log(f"{mark:6} {row['path']}  [{row['branch']} {row['head']}]  {row['why']}")
        if row["remove"] and apply:
            # No --force: git itself refuses anything modified or untracked.
            git("worktree", "remove", str(row["path"]))
            log(f"       removed {row['path']}")
    if not apply and any(row["remove"] for row in rows):
        log("dry run; pass --apply to remove the worktrees marked 'remove'")
    return 0


# --- status -----------------------------------------------------------------

def status(verify: bool) -> int:
    from vostok.build import shared_cache
    registered = _registered()
    for path in pool_paths():
        lock = read_lock(path)
        if path not in registered:
            state = "not created"
        elif lock:
            age = (time.time() - float(lock.get("time", 0))) / 3600
            stale = stale_reason(path, lock)
            state = (f"held by {lock.get('owner')} for {lock.get('branch')} ({age:.1f}h"
                     + (f", STALE: {stale}" if stale else "") + ")")
        else:
            state = "free"
        if path in registered:
            head = _current_branch(path)
            busy = " build running" if build_running(path) else ""
            changes = dirty(path)
            state += f"  [{head}{', dirty' if changes else ''}{busy}]"
        print(f"{path.name:12} {state}")
    print(f"\nshared cache {paths.SHARED_CACHE}"
          + ("" if shared_cache.enabled() else "  (disabled by VOSTOK_SHARED_CACHE=0)"))
    for row in shared_cache.status():
        created = time.strftime("%Y-%m-%d %H:%M", time.localtime(row["created"]))
        line = (f"  {row['entry']:17} {row['key']}  {row['files']:>6,} files "
                f"{row['bytes'] / 1e9:6.2f} GB  {created}")
        if not row["valid"]:
            line += "  (set aside)"
        elif verify:
            bad = shared_cache.verify_key(row["entry"], row["key"])
            line += "  verified" if not bad else f"  CORRUPT: {len(bad)} files"
        print(line)
    return 0


# --- CLI --------------------------------------------------------------------

def main() -> int:
    ap = argparse.ArgumentParser(prog="vostok tool worktree", description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="verb", required=True)

    def claiming(parser):
        parser.add_argument("branch")
        parser.add_argument("--owner", default=default_owner(),
                            help="who holds the claim (default $VOSTOK_POOL_OWNER, else $USER)")
        parser.add_argument("--from", dest="start", metavar="START",
                            help="create BRANCH from START instead of checking it out")
        parser.add_argument("--detach", action="store_true",
                            help="check out BRANCH (any commit-ish) detached, e.g. when the "
                                 "branch is checked out in another worktree")

    c = sub.add_parser("claim", help="take a free pool worktree and print its path")
    claiming(c)
    c.add_argument("--worktree", help="claim this pool worktree specifically")
    c.add_argument("--force", action="store_true", help="take it even from another owner")
    c.add_argument("--pid", type=int, help="process whose exit frees the claim")
    c.add_argument("--seed-from", type=Path, help="checkout to seed a new worktree's binaries from")

    r = sub.add_parser("release", help="free a pool worktree, leaving it clean")
    r.add_argument("worktree", nargs="?")
    r.add_argument("--owner", default=default_owner())
    r.add_argument("--force", action="store_true", help="release another owner's claim")
    mode = r.add_mutually_exclusive_group()
    mode.add_argument("--save", action="store_true",
                      help="keep uncommitted changes as a refs/vostok-pool/ commit, then clean")
    mode.add_argument("--discard", action="store_true", help="drop uncommitted changes")

    x = sub.add_parser("run", help="claim, switch, run COMMAND, release")
    claiming(x)
    x.add_argument("--timeout", type=float, help="give up waiting for a free worktree after N seconds")
    x.add_argument("command", nargs=argparse.REMAINDER)

    s = sub.add_parser("status", help="list the pool and the shared cache")
    s.add_argument("--verify", action="store_true", help="hash every cached tree against its manifest")

    g = sub.add_parser("gc", help="list (and --apply: remove) finished non-pool worktrees")
    g.add_argument("--apply", action="store_true")
    g.add_argument("--idle-hours", type=float, default=GC_IDLE_HOURS)

    args = ap.parse_args()
    try:
        if args.verb == "claim":
            wanted = _resolve(args.worktree, None) if args.worktree else None
            held = claim(args.branch, args.owner, start=args.start, detach=args.detach,
                         force=args.force, wanted=wanted, pid=args.pid, seed_from=args.seed_from)
            if held is None:
                log(f"no free pool worktree (pool size {paths.POOL_SIZE}); "
                    "`vostok tool worktree status` shows who holds them")
                return 3
            print(held.path)
            return 0
        if args.verb == "release":
            release(_resolve(args.worktree, args.owner), owner=args.owner, save=args.save,
                    discard=args.discard, force=args.force)
            return 0
        if args.verb == "run":
            command = args.command[1:] if args.command[:1] == ["--"] else args.command
            if not command:
                ap.error("run needs a command after --")
            return run(args.branch, command, owner=args.owner, start=args.start,
                       detach=args.detach, timeout=args.timeout)
        if args.verb == "status":
            return status(args.verify)
        return gc(args.apply, args.idle_hours)
    except PoolError as error:
        log(f"ERROR: {error}")
        return 2


if __name__ == "__main__":
    raise SystemExit(_log.run("vostok.tool.worktree", main))
