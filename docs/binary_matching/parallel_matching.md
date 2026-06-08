# Parallel matching: worktree fan-out onto a shared integration branch

The mechanics of a module run. The orchestrator's rules are in
[`.claude/agents/orchestrator.md`](../../.claude/agents/orchestrator.md); this is how
the run is wired on disk.

## Worktrees are the workers

The module is matched across N sibling git worktrees `vostok_1 .. vostok_N`
(`/home/sheep/Projects/surv/vostok_<n>`). Each is a full checkout with its own
`binaries/` and its own `WINEPREFIX` - both `$PWD`-derived (see CLAUDE.md) - so
parallel builds never collide.

```
git worktree add /home/sheep/Projects/surv/vostok_<n> <some-branch>
```

Each worktree is seeded by copying a built `binaries/` (warm Wine prefix + the
branch-independent target-side indexes), then one `rebuild.py` to build its own base
(each worktree's PDB must bake in its OWN source path, or the delinker's path-strip
breaks - never share a base build between worktrees).

**No tmpfs.** Build dirs stay on disk; `tmpfs_swap.py` is not run in any worktree (N x
build dirs would not fit RAM, and parallel swaps would contend).

## The common ground: `int/<module>`

`int/<module>` is the cherry-pick of every matched unit's own commit on top of the
module base. It is the buildable, scoreable trunk:

- Workers **branch off `origin/int/<module>`** and PR **`--base int/<module>`** - so
  every matched function/type/anchor is present (dependents compile and match; no
  duplication).
- It holds the **true aggregate match %** and is the only place cross-unit conflicts
  and regressions show. It is `replay(unit own-commits, dependency order)` - never
  hand-edited beyond resolving conflicts.

The orchestrator lands a settled unit by `git cherry-pick`-ing its own commit onto
`int/<module>` (never `git merge` a PR), union-resolving `temp_include_all.cpp`
(anchors deduped by name, braces balanced) and `PROGRESS.md` (ledger lines), placing
any shared definition **where the target defines it**, then rebuilding once to refresh
the aggregate.

## Concurrency

Hold at most **C concurrent workers** (the human sets C; default 3). One worker per
worktree at a time. When a worker returns and fewer than C run, dispatch the next unit
from a free worktree's queue. Each `.cpp` stays wholly within one worktree, so the only
cross-worktree collisions are the two shared append-only files, resolved at landing.

## Per-unit loop

matcher (persistent: pin to one function, exhaust source-shape approaches, stop only
at 100% or a proven LTO/inline wall) -> reviewer -> structure-checker -> re-match ONLY
if a checker finds an actionable problem. An all-100% unit skips review/structure. The
authoritative per-function number is `report.json`
`units[].functions[].fuzzy_match_percent` (top-level).
