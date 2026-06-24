# vostok - Claude working notes

This repo binary-matches the Vostok Engine (Survarium v0.100b): we write C++
that, compiled with the original toolchain (MSVC 8.0 / VS2008), produces
byte-identical object files to the shipped `survarium.exe` + `survarium.pdb`.
Matching is verified with `objdiff`, comparing our compiled "base" objects
against the original game's "target" objects.

These are the repo-local working agreements, auto-loaded when working in `vostok/`.

## Comments

Comments are still wanted - just keep them lean. Don't restate what the code
plainly shows; reserve a comment for non-obvious *why* (a workaround, a quirk
being matched, an ordering that matters). When deeper rationale or context would
bloat the code, surface it in chat or record it in `CLAUDE-WORK.md` instead of a
long inline comment.

## Review TODOs (`sushi@TODO:`)

When a PR review leaves an inline comment that is an **open matching question**
(something unresolved, suspect, or to investigate later), capture it in BOTH places
in the same commit that touches that PR:

1. **In source**, at the spot the comment is anchored, as a `// sushi@TODO:` line
   (paraphrase the reviewer's point; keep it lean).
2. **In `docs/binary_matching/review_todos.md`**, as a new table row (Status /
   Function / File:line / PR / TODO) so a later sweep agent can find it even after
   squash-merges erase the branch.

Grep `sushi@TODO:` for the live set; `review_todos.md` is the curated index. Tick a
row to `done` (and drop the marker) only when the matching question is actually
resolved.

## Python scripts

After editing anything under `scripts/`, lint it with ruff (provided by the
devShell):

    ruff check scripts/         # add --fix for the safe auto-fixes

## Tools and assets come from Nix, not sibling repos

Inside `nix develop` the flake provides everything:

- binaries on PATH: `vostok-delinker`, `pdb_parser`, `objdiff`, `objdiff-cli`,
  `wine`, plus `vcproj2ninja.exe` / `ninja.exe` run under Wine.
- assets via env vars: `VOSTOK_LIBS_DIR`, `SURVARIUM_BIN`, `MSVC_DIR`, ...
- large fetched packages pinned as gcroots under `binaries/nix-store/`.

The default shell is **lean**: it realizes the toolchain, libs, and the small
`SURVARIUM_BIN` (the exe/pdb matching needs) but **not** the heavy game
resources (~1.5 GiB packed + ~1.6 GiB unpacked). Those are opt-in via
`nix develop .#with-resources`, which adds `VOSTOK_RESOURCES_DIR` /
`VOSTOK_RESOURCES_UNPACKED` and pins them as gcroots. Binary matching never
touches the resources, so the default shell is all you need; reach for
`.#with-resources` only when inspecting/diffing game assets.

## Base PDB paths are Wine paths

The base build runs MSVC under Wine, so `survarium-dx11-win32-gold.pdb` records
source paths on the Z: drive (Wine maps `/` to `Z:`), lowercased and
backslash-separated, e.g. `z:\home\you\proj\vostok\sources\vostok\...`.
pdb-parser and the delinker strip an engine-path prefix off these paths; pass
the Wine-rendered prefix (see `_wine_path()` in `scripts/generate_structure.py`
and `scripts/generate_delink.py`), not the native `/home/...` path. The original
game PDB instead uses `c:/survarium/sources`.

## Build / diff loop

    nix develop                  # first entry builds the toolchain, sets up Wine
                                 # + ninja, and generates the target side once
    python3 scripts/rebuild.py   # ninja build under Wine, then regenerate
                                 # binaries/structure/base + binaries/objdiff/base,
                                 # then regenerate docs/binary_matching/match.db

A full engine build under Wine takes ~10 minutes; run it in the background (it is no
longer the loop bottleneck - agent token cost is). The
objdiff config is `binaries/objdiff/objdiff.json`; a match report is
`objdiff-cli report generate -p binaries/objdiff`. Every base delink regenerates
`binaries/objdiff/report.json` (and `report-changes.json`); `rebuild.py` then
regenerates `match.db` from that report at the end of its run. `match_db.py
refresh` is the regen-only path (re-derive the DB from an already-built report;
run `rebuild.py` first if sources moved) - it does NOT rebuild.

## Match score (README regression tracker)

`scripts/match_score.py` rolls `report.json` up into the overall fuzzy % plus a
per-module functions/code-matched table in the `<!-- match-score -->` block at the
top of README.md. **`rebuild.py` refreshes that block at the end of every build**
(alongside the `match.db` regen), so it stays current with `report.json` on its
own - you do not run `match_score.py` by hand. The numbers come straight from
objdiff's measures (the source carries no status markers; per-function
status/queues live in `docs/binary_matching/match.db` via `scripts/match_db.py`
- refresh / list / report / queue / flag; design in
`docs/binary_matching/match_db_design.md`), so the README is an honest, no-run
regression tracker - diff the block across commits. If the block ever conflicts on
a merge/cherry-pick, don't hand-resolve it: take either side and rerun `rebuild.py`
(or, if the artifacts are already built, `match_score.py --write-readme`) to
reconcile it deterministically - same as `match.db`.

### Header edits trigger rebuilds (vcproj2ninja tracks `#include`s)

The generated ninja graph lists every header a `.cpp` transitively `#include`s as
an implicit input, so **editing a header recompiles the dependent TUs
automatically** (the precompiled-header chain included).

vcproj2ninja resolves includes through the project's `/I` dirs (it evaluates
`#if`/`#ifdef`, erring toward over-inclusion so a real dependency is never missed).
Only project headers are tracked; system/CRT headers from `%INCLUDE%` are
intentionally not (they don't change).

`rebuild.py` also regenerates the graph itself on every run (write-if-changed,
no mtime churn on no-ops), so NEW `#include`s, un-excluded TUs, and .vcproj
edits are picked up without any manual `regen_ninja.py` step.

## Keep the README current

README.md documents this Nix/Linux workflow and goes stale easily. Whenever you
change something fundamental - the build/diff flow, script names or locations,
devShell tools, generated artifacts, or paths - check README.md and update it in
the same change.
