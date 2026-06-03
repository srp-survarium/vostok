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

**Overall: 47.96% fuzzy &middot; 7,996 / 25,372 functions exact (31.52%).**

| Module | Units | Functions exact | Code matched |
|---|--:|--:|--:|
| `render` | 351 | 355 / 2,805 (12.7%) | 3.3% |
| `game` | 141 | 18 / 1,634 (1.1%) | 0.1% |
| `game_core` | 189 | 337 / 1,429 (23.6%) | 14.7% |
| `core` | 136 | 653 / 1,331 (49.1%) | 34.1% |
| `animation` | 102 | 156 / 880 (17.7%) | 5.3% |
| `ai` | 124 | 413 / 759 (54.4%) | 38.0% |
| `sound` | 69 | 186 / 517 (36.0%) | 15.6% |
| `collision` | 52 | 363 / 516 (70.3%) | 30.9% |
| `particle` | 25 | 214 / 485 (44.1%) | 24.8% |
| `vfs` | 71 | 183 / 412 (44.4%) | 16.1% |
| `scaleform` | 15 | 0 / 280 (0.0%) | 0.0% |
| `ui` | 27 | 166 / 255 (65.1%) | 41.2% |
| `physics` | 14 | 97 / 203 (47.8%) | 24.3% |
| `fs` | 25 | 66 / 189 (34.9%) | 26.8% |
| `engine` | 22 | 51 / 165 (30.9%) | 11.0% |
| `network` | 25 | 6 / 163 (3.7%) | 0.4% |
| `network_core` | 22 | 6 / 140 (4.3%) | 6.1% |
| `debug` | 16 | 77 / 127 (60.6%) | 48.1% |
| `logging` | 10 | 32 / 73 (43.8%) | 33.0% |
| `input` | 9 | 30 / 56 (53.6%) | 27.6% |
| `survarium` | 5 | 10 / 22 (45.5%) | 13.1% |
| `ai_navigation` | 3 | 7 / 14 (50.0%) | 19.4% |

_Updated 2026-06-03 &middot; delinker `5118e2a` (folded-symbol reconciliation)._
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

This builds `survarium - PC - DirectX 11` with ninja under Wine, then regenerates
the **base-side** diff inputs in parallel:

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

## Docs

- [Matching guide](https://gist.github.com/sushi-shi/8bf16f82c3b1c65fd357d73ecfda909e) - how to actually match assembly.
- [docs/index.md](docs/index.md) - index of build and per-module matching notes.
- [docs/build/toolchain-build.md](docs/build/toolchain-build.md) - how the VS2008 toolchain is built under Wine (and why Wine must be staging).
- [docs/windows-setup.md](docs/windows-setup.md) - the legacy manual Windows/VS2008 workflow.
