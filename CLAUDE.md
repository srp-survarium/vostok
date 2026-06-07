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
                                 # binaries/structure/base + binaries/objdiff/base

A full engine build under Wine takes ~20 minutes; run it in the background. The
objdiff config is `binaries/objdiff/objdiff.json`; a match report is
`objdiff-cli report generate -p binaries/objdiff`. Every base delink regenerates
`binaries/objdiff/report.json` (and `report-changes.json`).

## Match score (README regression tracker)

`scripts/match_score.py` rolls `report.json` up into the overall fuzzy % plus a
per-module functions/code-matched table, and `--write-readme` refreshes the
`<!-- match-score -->` block at the top of README.md. The numbers come straight
from objdiff's measures, NOT from the `// STATE[..]` markers (which are often
absent or stale), so the README is an honest, no-run regression tracker - diff
the block across commits. Re-run `python3 scripts/match_score.py --write-readme`
whenever `report.json` moves (after a re-delink, or a toolchain/delinker change
that shifts many symbols).

### IMPORTANT: headers are NOT in the ninja dependency graph

ninja here only tracks `.cpp` inputs - there are no dependency edges from `.h`
files. So **editing a header never triggers a rebuild on its own**; the change is
silently ignored and you compile stale (e.g. a freshly added function reads as
`C2039: '<fn>' is not a member of ...`). After ANY header edit you must force the
consuming compile to re-run, by EITHER:
- a full clean rebuild, OR
- `touch`-ing the `.cpp` file(s) that include that header (directly or
  transitively) so ninja recompiles them.

The worst case is a header pulled into a module's **precompiled header** (e.g.
`math_float4x4_inline.h` reaches game_core's PCH via `extensions.h ->
math_extensions.h -> math_float4x4.h`). Touching the included `pch.h` does nothing
(it is a header, not a graph node); instead delete the stale `.pch`
(`binaries/Win32/intermediates/Master Gold/<module>/vostok_<module>-static-gold.pch`)
and `touch sources/vostok/<module>/sources/pch.cpp` (the explicit `/Yc` input) so
the PCH and every dependent TU rebuild. Never work around a no-op header edit by
moving a definition into a consuming `.cpp` - that misplaces the symbol.

## Keep the README current

README.md documents this Nix/Linux workflow and goes stale easily. Whenever you
change something fundamental - the build/diff flow, script names or locations,
devShell tools, generated artifacts, or paths - check README.md and update it in
the same change.
