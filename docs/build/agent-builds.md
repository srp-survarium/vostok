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
