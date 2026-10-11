# Builds for agents and long-running jobs

How `python3 -m vostok build` runs under Codex/Claude agents, how it reports completion, and what its logs contain. The working rules for agents are in [CLAUDE.md](../../CLAUDE.md).

The Wine supervisor scopes cleanup to this worktree's prefix; if the PDB server
holds Ninja after a completed link or a failed pre-link edge, it reaps that
server and preserves Ninja's real success or failure result.

## Header dependencies

Header dependencies come from the compiler, not only from vcproj2ninja's
include scan (which cannot follow computed includes such as
`#include VOSTOK_RENDER_CORE_FILE(...)`). Every compile edge stores the headers
cl actually opened in Ninja's deps log (`deps = msvc`), so editing any header
rebuilds exactly the edges that include it. `/GL` compiles pass the front-end
`/d1showIncludes`, which keeps `/MP` and the object bytes. Non-`/GL` objects
record their whole command line in the PDB, and PCH creation lists no includes,
so those edges first run a syntax-only `/Zs` scan and then compile with the
unchanged command line. The first build after this graph change recompiles
every edge once, because no edge has recorded dependencies yet. Ninja stamps
those records with the clock it runs under, so module builds
(`python3 -m vostok.build.ninja MODULE`) run on the same retail build clock as
the full build; a real-clock run after a faked one would see every record as
stale.

## The worktree pool

`python3 -m vostok tool worktree` keeps a fixed pool of build worktrees beside
the main checkout: `vostok-w1`..`vostok-w4` (`VOSTOK_POOL_SIZE`,
`VOSTOK_POOL_ROOT`, `VOSTOK_POOL_PREFIX`). Their `binaries/` survive between
tasks, so switching one to another branch and building recompiles only the
changed sources and the headers' dependents.

```sh
# one line: claim (waiting while all are busy), switch, run, release
python3 -m vostok tool worktree run BRANCH -- nix develop --command python3 -m vostok build --foreground
# a tree held across several steps
path=$(python3 -m vostok tool worktree claim BRANCH --owner matcher-7)
python3 -m vostok tool worktree release vostok-w2 --owner matcher-7
python3 -m vostok tool worktree status [--verify]
python3 -m vostok tool worktree gc [--apply]
```

- **Claims** are JSON files in `.git/vostok-pool/` (owner, branch, pid, host,
  time), written with `O_EXCL` under a pool-wide `flock`. A free worktree
  already on the branch is preferred; a dirty or building one is skipped. A
  claim is refused when another owner holds the tree unless `--force`.
  `--from START` creates the branch; `--detach` checks out any commit-ish
  detached (git refuses a branch checked out in another worktree).
- **Stale claims** free themselves on the next claim, with a log line, when the
  recorded process is gone (`run` records its own pid; `claim --pid PID`) or the
  claim is older than `VOSTOK_POOL_STALE_HOURS` (24). A claim is never stale
  while a build holds the worktree's build lock.
- **`run`** points the child at the pool worktree (`VOSTOK_DIR`, `PYTHONPATH`,
  `WINEPREFIX`), drops `CODEX_THREAD_ID` so `vostok build` stays in the
  foreground, forwards SIGINT/SIGTERM/SIGHUP, waits for the build lock, and
  releases in a `finally`. A dirty tree at release is saved as a commit under
  `refs/vostok-pool/<worktree>/<time>` and then reset.
- **`release`** leaves the tree clean and detached at the same commit, so the
  branch can be checked out elsewhere and no file mtime moves.
- **Lazy creation**: a claim with no free worktree creates the next one
  (`git worktree add --detach`, default start `origin/v0.10b`) and seeds its
  `binaries/` from the main checkout without `Win32/`, logs, ninja logs,
  `libs-release/`, the `vostok-libs-*.zip` archives, the unread legacy `rich/`
  and the shared trees. Its absolute paths differ from the source checkout, so
  its first build is a full one; later claims are incremental.
- **`gc`** lists every worktree outside the pool and the main checkout; with
  `--apply` it runs `git worktree remove` (never `--force`) on those that are
  clean, not locked, not building, merged into `origin/v0.10b` or contained in
  a remote branch, and idle for `--idle-hours` (24: git `HEAD`/index, build
  lock and rebuild log untouched).

## The shared build cache

Three trees are identical in every worktree with the same pinned inputs and are
only read by builds. One copy lives in `../.vostok-cache/<entry>/<key>/`
(`VOSTOK_CACHE_DIR`) and setup hard-links each worktree's files to it:

| entry | worktree path | key inputs | size |
|---|---|---|---|
| `prebuilt` | `binaries.prebuilt/` | `VOSTOK_LIBS_DIR` store path | 2.6 GB |
| `pdb-target` | `binaries/pdb/target/` | retail game, `vostok-pdb`, engine prefixes | 0.5 GB |
| `structure-target` | `binaries/structure/target/` | retail game, `vostok-pdb`, engine prefix | 0.2 GB |

Hard links rather than a directory symlink, so no worktree can write into the
shared copy: cached files are read-only (an in-place write fails), and a writer
that replaces or deletes a file only drops this worktree's link. A new key is
generated fresh into the cache, never adopted from a worktree; when a
worktree's recorded key changes, the cached tree is verified by sha256 against
its manifest first, and a damaged tree is set aside and regenerated. A
differing single-link file in `binaries.prebuilt/` is a local override (the GFx
tool writes shipping libs there) and is kept; the retail trees are fully
determined by their key, so a differing or extra file there is stale and is
replaced. `setup --force target` regenerates the cached retail trees.
`VOSTOK_SHARED_CACHE=0` turns sharing off.

Not shared, on purpose: `binaries/objdiff/target` (every base COFF step
re-normalizes it with this branch's rules), `binaries/data-objdiff/target` and
the `gen/target_data_*` inventory (rewritten by the data lane), and
`binaries/gfx-sdk` (this branch's Scaleform overlays, written by the GFx tool).
`binaries/libs-release/` and the `vostok-libs-*.zip` archives are scratch and
output of `vostok tool libs-release`, and `binaries/rich/` is a legacy index no
script reads: no build needs them, so pool worktrees do not carry them.

## Host-wide build lock

Each ninja runs about as many `cl.exe` as the machine has cores, so concurrent
builds in several worktrees multiply (five full builds: ~490 `cl.exe`, load ~55
on 24 cores and 31 GB). `vostok.build.ninja` therefore holds an `flock` on
`$XDG_RUNTIME_DIR/vostok-ninja.lock` (`VOSTOK_BUILD_LOCK` overrides it; the
runtime dir is derived from the uid when the variable is unset) for every run
that can compile or link, full or module. A second build prints one line,
`waiting for the build lock held by <worktree> pid N`, and blocks; the kernel
releases the lock when the holder exits or crashes. `-t` tools and `-n` dry
runs never take it, and `--no-lock` (`vostok build --no-lock`) opts out.

The measurement phase after ninja (PDB evidence, delink, objdiff report,
ledger) stays outside the lock: it runs at most two processes per build, so it
cannot multiply the way `-j` does, and holding the lock through it would leave
the compiler slot idle for the ~15 minutes another worktree could compile.

## Retail provenance stamps

Every target-side artifact is derived from the retail `survarium.exe` and
`survarium.pdb` that the flake pins (`survarium-game`). The generators stamp
each one with the sha256 of those two files (`vostok.build.retail_stamp`,
after homm3's provenance stamps): `binaries/pdb/target`,
`binaries/structure/target`, `binaries/objdiff/target` and
`binaries/data-objdiff/target` carry `.retail-stamp.json`, and their symbol
maps carry `<map>.retail-stamp.json`. Setup, and therefore every build, compares
the stamps with the pinned retail:

- a stamp from another retail is a hard error that names both identities.
  This is the wrong-retail trap: a target side copied or left over from a
  v0.20e branch silently collapses every score. Fix it with
  `python3 -m vostok build --regenerate-target` (or
  `vostok tool toolchain --regenerate-target`, or
  `VOSTOK_REGENERATE_TARGET=1 nix develop` when the shell itself refuses);
- an unstamped artifact predates stamping and is regenerated, not trusted;
- the legacy `binaries/rich/target` is never generated or read; setup only
  notes it.

The shared cache keys the retail trees by the same two hashes, so a v0.20e
worktree can never link v0.100b's evidence, or the reverse.

**Migrating an existing worktree** needs no copy: once its branch contains the
pool, its next setup (`nix develop` or `vostok build`) replaces identical files
with cache links, keeping any local prebuilt overrides. Delete its
`binaries/libs-release`, `binaries/vostok-libs-*.zip` and `binaries/rich` by
hand, or retire it with `gc` once its branch is pushed and use the pool.

## Notification-driven builds

Codex builds on Linux with a systemd user manager are notification-driven by
default whenever `CODEX_THREAD_ID` is set:

```sh
python3 -m vostok build
# Or select the destination and the installed CLI explicitly:
python3 -m vostok build --background --notify-thread THREAD --codex-bin /path/to/codex
# Explicit synchronous mode for CI or a supervising process:
python3 -m vostok build --foreground
```

The default destination is `CODEX_THREAD_ID`. The CLI is selected from
`--codex-bin`, `VOSTOK_CODEX_BIN`, a usable `CODEX_EXECUTABLE_PATH`, then `PATH`;
it must support `codex queue --thread --message`. Outside Codex, builds remain
synchronous unless notification mode is requested. The supervisor explicitly
uses `--foreground` to avoid recursive service launches. The launcher needs host user-bus access (request
execution approval when running in a sandbox). It starts a transient
`vostok-build-*.service`, prints its name and its unique log under
`binaries/build-jobs/`, and returns immediately. The service enters this
worktree's `nix develop`, waits for the complete build/refresh process, and
queues its exit status and log path into that existing thread, including on
setup or build failure. No timer or model polling is involved. Existing
per-worktree build locking still serializes the build itself.

`python3 -m vostok build --code-only` is the iteration build: it stops after
the function ledger, skipping the data lane (manifests, data COFF/reports,
image-data ledger, module data audits) and the README. Function scores are the
same as a full build's because the function ledger reads only code-lane
artifacts; data-side changes appear at the next full build, which is the only
kind whose README and ledger get committed.

Build logs include `[timing]` lines for each phase, expensive delinker/report
subprocesses, symbol inspection, and each module's data audit. Durations use a
monotonic clock and include failures; nested/parallel timings are not additive.
Data preparation runs once per build and its successful result feeds the later
image-data refresh. Standalone `vostok data refresh` still prepares its inputs.
Symbol normalization inspects objects in bounded batches; audit fingerprints
and content keys are shared only within that build, with no persistent cache.
Deferred performance ideas and their safety requirements are tracked in
[the performance backlog](docs/todos/perf.md).

Launcher success means only that the service started. Wait for the completion
notification; do not repeatedly inspect the session, process, service, or log.
A delivery/setup failure never silently falls back to a foreground build.

Check the log for nonfatal ledger/README refresh warnings even after exit 0.
Notification failure is recorded in that log without changing the build exit
status; delivery is attempted once with a 30-second timeout. Host shutdown or
force-stopping the service can prevent notification. To stop a job, use
`systemctl --user stop UNIT.service`; service startup diagnostics remain in
`journalctl --user -u UNIT.service`. Keep the worktree and branch unchanged
until the job finishes.
