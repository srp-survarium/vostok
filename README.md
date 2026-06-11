# vostok

Binary-matching decompilation of the **Vostok Engine** used by Survarium
(v0.100b). The goal is C++ sources that, compiled with the original toolchain
(MSVC 8.0 / VS2008 SP1), produce byte-identical objects to `survarium.exe`.

This README covers the **Nix / Linux** workflow, which builds the exact VS2008
toolchain under Wine for you - no Windows install required. For the original
manual Windows/VS2008 setup, see [docs/windows-setup.md](docs/windows-setup.md).

<!-- match-score:start -->
## Match status

_Auto-generated from `binaries/objdiff/report.json` by `scripts/match_score.py` - re-run after every re-delink; do not hand-edit. Diff this block across commits to spot regressions._

**Overall: 51.78% fuzzy &middot; 9,547 / 25,372 functions exact (37.63%).**

| Module | Units | Functions exact | Code matched |
|---|--:|--:|--:|
| `render` | 351 | 384 / 2,805 (13.7%) | 3.5% |
| `shared` | 112 | 840 / 2,255 (37.3%) | 36.1% |
| `game` | 141 | 20 / 1,634 (1.2%) | 0.1% |
| `game_core` | 189 | 553 / 1,429 (38.7%) | 22.8% |
| `core` | 136 | 658 / 1,331 (49.4%) | 34.2% |
| `animation` | 102 | 161 / 880 (18.3%) | 5.4% |
| `ai` | 124 | 428 / 759 (56.4%) | 38.9% |
| `sound` | 69 | 214 / 517 (41.4%) | 16.8% |
| `collision` | 52 | 371 / 516 (71.9%) | 31.0% |
| `particle` | 25 | 263 / 485 (54.2%) | 26.1% |
| `vfs` | 71 | 190 / 412 (46.1%) | 16.3% |
| `scaleform` | 15 | 0 / 280 (0.0%) | 0.0% |
| `ui` | 27 | 179 / 255 (70.2%) | 42.9% |
| `physics` | 14 | 99 / 203 (48.8%) | 24.4% |
| `fs` | 25 | 70 / 189 (37.0%) | 27.8% |
| `engine` | 22 | 52 / 165 (31.5%) | 11.1% |
| `network` | 25 | 7 / 163 (4.3%) | 0.5% |
| `network_core` | 22 | 31 / 140 (22.1%) | 12.8% |
| `debug` | 16 | 96 / 127 (75.6%) | 65.5% |
| `logging` | 10 | 32 / 73 (43.8%) | 33.0% |
| `input` | 9 | 31 / 56 (55.4%) | 27.7% |
| `survarium` | 5 | 10 / 22 (45.5%) | 13.1% |
| `ai_navigation` | 3 | 7 / 14 (50.0%) | 19.4% |

_Updated 2026-06-10 &middot; delinker `5118e2a` (folded-symbol reconciliation)._
<!-- match-score:end -->

## Requirements

- Linux (x86_64) with [Nix](https://nixos.org/download) and flakes enabled
  (`experimental-features = nix-command flakes`).

That's it - everything else (the VS2008 SP1 compiler, Windows/DirectX SDKs,
Wine, ninja, and the Rust tools) is provided by the flake.

## Quickstart

```sh
nix develop
```

> **Disk space:** the first `nix develop` realizes **~18 GiB** into the Nix store
> - the VS2008 toolchain, Wine, the Rust toolchain, the game install (incl. the
> ~1.5 GiB packed resources plus the ~1.6 GiB unpacked resource tree), and the
> third-party libs (~6 GiB). Later entries reuse it and are near-instant.

The first entry fetches/builds everything and runs `scripts/setup-toolchain.py`,
which:

- pins the toolchain, libs, and game binaries as gcroots under `binaries/nix-store/`,
- initialises the Wine prefix and registry (PATH / INCLUDE / LIB),
- generates the ninja build graph from the `.sln` via `vcproj2ninja`, and
- generates the **target-side** diff inputs once from the original game
  (`binaries/objdiff/target`, `binaries/structure/target`).

Then, after editing sources, run the build/diff loop:

```sh
python3 scripts/rebuild.py
```

This first refreshes the ninja graph from the `.vcproj`s (write-if-changed: a no-op
regen touches nothing, so there are no spurious rebuilds), builds
`survarium - PC - DirectX 11` with ninja under Wine, then regenerates the
**base-side** diff inputs in parallel:

| | base (your build) | target (original game) |
|---|---|---|
| COFF objs (objdiff) | `binaries/objdiff/base` | `binaries/objdiff/target` |
| C++ stubs (pdb-parser) | `binaries/structure/base` | `binaries/structure/target` |

Open the result in [objdiff](https://github.com/encounter/objdiff) (config at
`binaries/objdiff/objdiff.json`) and match `base` against `target`. The rebuild
also writes an overall match summary to `binaries/objdiff/report.json` and logs
the code / function match percentages.

Useful individual scripts (all run inside `nix develop`):

```sh
python3 scripts/ninja_build.py [target]              # build only (verbose, keep-going)
python3 scripts/generate_delink.py {base|target}     # COFF split for one side
python3 scripts/generate_structure.py {base|target}  # pdb-parser stubs for one side
```

## Game data

The whole game comes from one installer extraction, split into three outputs of
the `survarium` derivation. `nix develop` realizes and pins **all three** under
`binaries/nix-store/` by default - no extra step:

- `survarium-game` - `survarium.exe`, `survarium.pdb`, DLLs (also `SURVARIUM_BIN`)
- `survarium-resources` - `resources.db` + `resources/` (packed game data, ~1.5 GiB)
- `survarium-keys` - lobby/login server SSL certs + private keys

Plus a fourth, derived output pinned the same way:

- `survarium-resources-unpacked` - `resources.db` expanded into its file tree
  (~1.6 GiB, 12557 files) by [`vostok-resources-db`](https://github.com/srp-survarium/vostok-resources-db),
  for inspecting/diffing game assets directly.

To build any one standalone (e.g. outside the shell): `nix build .#survarium-resources`,
`.#survarium-resources-unpacked`, `.#survarium-keys`, `.#survarium-game`.

## Matching (the orchestrator)

Binary-matching is driven by an **orchestrator** that dispatches `matcher` +
`structure-verifier` (+ a tiny `reviewer`) workers and stacks their PRs onto
`feature/agentic-matching-loop-2` for review. **Start it from a fresh top-level Claude Code
session** - it spawns worker subagents, so it cannot run as a nested subagent - with the
`/match` slash command:

```sh
/match network_core        # whole queue, default 3 workers in parallel
/match game_core 2         # whole queue, but only 2 workers in parallel
```

Per unit it then: prepares a sibling worktree `vostok_<N>` off the stack tip, dispatches a
`matcher` (up to the worker cap, default 3), runs the `structure-verifier` + `reviewer` onto the same
branch, and opens a **stacked PR** (match + verify commits, minimal body). Matchers spawn
off the **top** of the stack so percentages compound; you review the stack **bottom-up** and
merge one PR at a time (the `pr-verifier` agent prepares each onto the advancing base).
Prereqs: worktrees `vostok_1..3` clean + warm (`binaries/rich/target` + `binaries/objdiff`
present). Full rules: [`.claude/agents/orchestrator.md`](.claude/agents/orchestrator.md).

## Docs

- [Matching guide](https://gist.github.com/sushi-shi/8bf16f82c3b1c65fd357d73ecfda909e) - how to actually match assembly.
- [docs/index.md](docs/index.md) - index of build and per-module matching notes.
- [docs/build/toolchain-build.md](docs/build/toolchain-build.md) - how the VS2008 toolchain is built under Wine (and why Wine must be staging).
- [docs/windows-setup.md](docs/windows-setup.md) - the legacy manual Windows/VS2008 workflow.
