# vostok

Binary-matching decompilation of the **Vostok Engine** used by Survarium
(v0.100b). The goal is C++ sources that, compiled with the original toolchain
(MSVC 8.0 / VS2008 SP1), produce byte-identical objects to `survarium.exe`.

This README covers the **Nix / Linux** workflow, which builds the exact VS2008
toolchain under Wine for you - no Windows install required. For the original
manual Windows/VS2008 setup, see [docs/windows-setup.md](docs/windows-setup.md).

<!-- match-score:start -->
## Match status

_Auto-generated from `config/match_state.tsv` (the committed matching ledger) - refreshed by `vostok build` at the end of every build; do not hand-edit. Diff this block across commits to spot regressions._

**Overall: 10,035 / 13,014 functions exact (77.11%) &middot; 10,797 / 13,014 functions exact-max (82.96%) &middot; 89.76% fuzzy &middot; 94.56% fuzzy-max.**

_All figures come from the ledger over every target function (paired plus inlined/folded target-only). **Functions exact** and **Fuzzy** describe the current build (`cur`). **Exact-max** and **Fuzzy-max** use `max`, the peak proven for the function's own source body (`hash`), which resets when that body changes; the all-time `hist` peak is never promoted into it, and a banked peak carrying no `hash` is not credited. Byte-weighted code view: `python3 -m vostok ledger readme --max-code`._

| Module          | Units |       Functions exact |   Functions exact-max | Fuzzy | Fuzzy-max |
| :-------------- | ----: | --------------------: | --------------------: | ----: | --------: |
| `render`        |   351 | 1,947 / 2,734 (71.2%) | 2,215 / 2,734 (81.0%) | 85.5% |     93.0% |
| `game`          |   141 |   909 / 1,528 (59.5%) | 1,073 / 1,528 (70.2%) | 82.6% |     88.1% |
| `core`          |   136 | 1,135 / 1,325 (85.7%) | 1,202 / 1,325 (90.7%) | 94.3% |     97.7% |
| `vostok`        |   112 | 1,052 / 1,250 (84.2%) | 1,077 / 1,250 (86.2%) | 90.3% |     96.8% |
| `game_core`     |   189 |   787 / 1,181 (66.6%) |   850 / 1,181 (72.0%) | 90.2% |     95.5% |
| `animation`     |   102 |     561 / 727 (77.2%) |     592 / 727 (81.4%) | 89.9% |     92.1% |
| `ai`            |   124 |     619 / 691 (89.6%) |     650 / 691 (94.1%) | 97.6% |     99.5% |
| `sound`         |    69 |     473 / 510 (92.7%) |     492 / 510 (96.5%) | 98.6% |     99.8% |
| `collision`     |    52 |     460 / 503 (91.5%) |     469 / 503 (93.2%) | 98.3% |     98.7% |
| `scaleform`     |    47 |     411 / 454 (90.5%) |     423 / 454 (93.2%) | 94.1% |     96.6% |
| `particle`      |    25 |     384 / 400 (96.0%) |     392 / 400 (98.0%) | 99.4% |     99.8% |
| `vfs`           |    71 |     283 / 390 (72.6%) |     285 / 390 (73.1%) | 95.7% |     96.8% |
| `ui`            |    27 |     216 / 227 (95.2%) |     218 / 227 (96.0%) | 98.7% |     99.5% |
| `physics`       |    14 |     130 / 198 (65.7%) |     152 / 198 (76.8%) | 85.9% |     92.4% |
| `fs`            |    25 |     133 / 165 (80.6%) |     137 / 165 (83.0%) | 96.1% |     97.8% |
| `engine`        |    22 |     148 / 162 (91.4%) |     153 / 162 (94.4%) | 97.2% |     98.4% |
| `network`       |    25 |      76 / 159 (47.8%) |      89 / 159 (56.0%) | 83.2% |     92.7% |
| `network_core`  |    22 |      58 / 127 (45.7%) |      64 / 127 (50.4%) | 91.3% |     94.3% |
| `debug`         |    16 |     117 / 122 (95.9%) |     118 / 122 (96.7%) | 98.6% |     98.9% |
| `logging`       |    10 |       52 / 72 (72.2%) |       59 / 72 (81.9%) | 97.0% |     99.4% |
| `input`         |     9 |       52 / 53 (98.1%) |       52 / 53 (98.1%) | 99.9% |     99.9% |
| `survarium`     |     5 |       19 / 22 (86.4%) |       21 / 22 (95.5%) | 96.0% |     98.1% |
| `ai_navigation` |     3 |       13 / 14 (92.9%) |      14 / 14 (100.0%) | 89.9% |    100.0% |

_Updated 2026-08-31 &middot; delinker `d7e9292` (folded-symbol reconciliation)._
<!-- match-score:end -->

<!-- data-match:start -->
## Data reconstruction status

_Auto-generated from the consumer-owned objdiff projection plus the independent linked-image audit; it does not feed the function score._

**Function-reachable retail data: 98.13% (70,200,521 / 71,536,291 unique bytes) &middot; paired for comparison: 3.96% (2,829,429 / 71,536,291) &middot; matched projected copies: 65.45% (16,944,761 / 25,888,541).**

_The projection has 13,338 paired copies across 1,695 units and 32,703 unresolved blockers. Strict relocation comparison exposes 1,504 otherwise-exact functions (577,770 code bytes) with referent debt._

_The linked-image audit is 3.61% exact and covers 98.57% by PDB type extent, with 84.75% paired-only fidelity (8236 / 10838 definitions). Integrity ratchet: shadow calibration._

_Updated 2026-08-31._
<!-- data-match:end -->

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

The first entry fetches/builds everything and runs `vostok tool toolchain`,
which:

- pins the toolchain, libs, and game binaries as gcroots under `binaries/nix-store/`,
- stages the prebuilt third-party `.lib`/`.dll` blobs into `binaries.prebuilt/`
  (gitignored; mirrors the shipped game's layout - not the `sources/` tree),
- initialises the Wine prefix and registry (PATH / INCLUDE / LIB),
- generates the ninja build graph from the `.sln` via `vcproj2ninja`, and
- generates the **target-side** diff inputs once from the original game
  (`binaries/objdiff/target`, `binaries/structure/target`).

Then, after editing sources, run the build/diff loop:

```sh
python3 -m vostok build
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
also writes an overall match summary to `binaries/objdiff/report.json`, logs
the code / function match percentages, and refreshes the committed matching
ledger (`config/match_state.tsv`) and the score block at the top of this README
from that fresh report at the end of the run.

Useful individual steps (all run inside `nix develop`):

```sh
python3 -m vostok.build.ninja [target]                    # build only (verbose, keep-going)
python3 -m vostok.build.generate_delink {base|target}     # COFF split for one side
python3 -m vostok.build.generate_structure {base|target}  # pdb-parser stubs for one side
```

### Where the tooling lives

The code is a Python package at `scripts/vostok/`:

| package    | what it owns                                                         |
| :--------- | :------------------------------------------------------------------- |
| `core/`    | every repo path (`paths.py`), the tracked-table shape, symbol names   |
| `ledger/`  | the committed campaign record and the README score block              |
| `derive/`  | `report.json` + the rich indexes -> the committed ledger              |
| `sema/`    | control-flow views over one base<->target function pair               |
| `build/`   | the ninja graph, the delink/structure/rich generators, `rebuild`      |
| `diff/`    | target-vs-base source shape: layouts, declaration order, enums        |
| `tool/`    | clangd, the toolchain setup/release, prebuilt libs                    |
| `shaders/` | shipped shader blobs: coverage, dump, disasm, the fxc roundtrip proof ([docs/shaders.md](docs/shaders.md)) |

`scripts/` is the package root and the dev shell exports it on `PYTHONPATH`, so
every command above runs from anywhere in the tree. `python3 -m vostok` prints
the umbrella map; each package is also a direct entry:

```sh
python3 -m vostok                                    # a map of the whole surface
python3 -m vostok.ledger report --module render
python3 -m vostok.sema blocks <fn> --diff --lite
```

### Source navigation (clangd)

`compile_commands.json` + `clangd-vfs.yaml` at the repo root (`vcproj2ninja
--target clangd`) give clangd a clang-cl view of every TU pinned to the MSVC
8.0 dialect (`_MSC_VER=1400`, i686, C++98), with a case-insensitive VFS overlay
because the sources spell includes in Wine's case-insensitive world. They are
generated by `vostok tool toolchain` on first `nix develop` entry, and the graph
regen renews them when they are missing or a TU/module is added or removed
(they are include-invariant, so ordinary rebuilds skip them); force with
`python3 -m vostok.build.ninja_regen --compdb` after a flags-only .vcproj edit.
clangd (in the devShell) navigates the whole engine through it - **clang is a
reader here**; its diagnostics are suppressed via `.clangd` and the Wine build
remains the only verdict on correctness.

Point any LSP editor at the repo root (the compdb is found automatically), or
query from the shell / an agent:

```sh
python3 -m vostok tool clangd index                       # warm the background index once
python3 -m vostok tool clangd symbol weapon_lexeme_pair   # fuzzy workspace symbols
python3 -m vostok tool clangd def   <file> <line> [col]   # go to definition
python3 -m vostok tool clangd refs  <file> <line> [col]   # all references
python3 -m vostok tool clangd hover <file> <line> [col]   # type/expansion at point
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

## Matching campaigns

`config/match_state.tsv` is the committed per-function ledger (current score,
the peak proven for the function's own source body, the all-time peak, status),
so a fresh clone answers progress questions without compiling anything:

```sh
python3 -m vostok ledger report --module render   # byte-weighted rollup, --per-unit for worst-first
python3 -m vostok ledger queue --module render    # one batch per TU, what a matcher is handed
git diff <rev> -- config/match_state.tsv          # regression tracking: which functions moved
```

Campaigns are agent-driven. Repository skills under [`.agents/skills/`](.agents/skills/)
cover unit matching, structure verification, stack review, and module-scale
orchestration; ask the agent to use `$vostok-orchestrate-matching` for a whole
module or `$vostok-match-unit` for one translation unit. The working agreements -
ledger columns, the structure-diff and PDB evidence commands, commit rules - live
in [CLAUDE.md](CLAUDE.md) and [AGENTS.md](AGENTS.md); the full loop is
[`docs/binary_matching/agentic_loop.md`](docs/binary_matching/agentic_loop.md).

## Docs

- [docs/binary_matching/matching_guide.md](docs/binary_matching/matching_guide.md) - how to actually match assembly.
- [docs/index.md](docs/index.md) - index of build and per-module matching notes.
- [docs/build/toolchain-build.md](docs/build/toolchain-build.md) - how the VS2008 toolchain is built under Wine (and why Wine must be staging).
- [docs/windows-setup.md](docs/windows-setup.md) - the legacy manual Windows/VS2008 workflow.

## License

Everything this project adds on top - the reconstruction work in the engine
sources, the Python tooling, the Nix flake, and the docs - is licensed under the
[GNU General Public License, version 3 or later](LICENSE), to the extent the
contributors can do so. The tooling and the engine units the project generated
carry a one-line `SPDX-License-Identifier: GPL-3.0-or-later` tag.

The engine sources carry their original `Copyright (C) GSC Game World`
headers: the Vostok Engine and Survarium remain the property of their rights
holders, and the GPL covers only the contributors' work. No game binaries or
packed resources are stored in the repository; the retail inputs the flake
fetches, and build outputs that incorporate them, are not covered by this
license.

### Third-party code in the tree

`sources/` vendors the third-party code the engine was built against, as it
came with the engine's source tree. Each tree keeps its own terms; a file
carrying its own copyright or license notice is governed by that notice, not
by the GPL. Several of these SDKs are proprietary and are included only so the
engine builds as shipped; **if a rights holder asks for one to be removed, it
will be.**

| Tree | Version | Terms as carried in the tree | Used by |
| :-- | :-- | :-- | :-- |
| `boost` | 1.48.0 | Boost Software License | engine |
| `stlport` | 5.2.1 | STLport license (permissive) | engine STL |
| `bullet` | 2.79 | zlib | physics |
| `ode` | 0.11.1 | LGPL / BSD dual | physics |
| `opcode` | 1.3 | Pierre Terdiman's free-use terms | collision |
| `WildMagic` | 4.9 | LGPL (Geometric Tools) | engine math, collision |
| `zlib`, `minizip` | 1.2.3, 1.01 | zlib | fs, vfs |
| `ogg`, `vorbis`, `theora` | 1.1.4, 1.2.3, 1.1.1 | BSD | sound, video |
| `lua`, `luajit`, `luabind` | 5.1 | MIT | scripting, configs |
| `openssl` | 1.0.0g | OpenSSL / SSLeay | network |
| `mysql` | Connector 6.02 | GPL v2 (FOSS exception) | servers |
| `freeimage` | 3.12.0 | FreeImage Public License / GPL dual; bundled codecs BSD | textures |
| `BugTrap` | 1.3.3291 | IntelleSoft terms, see the tree | crash reporting |
| `fastdelegate` | 2005-05-30 | CodeProject terms | engine |
| `ParticleAPI` | 2.21 | author's terms, see the tree | particles |
| `cs` | 1.3 | GSC-authored helper library, same headers as the engine | engine |
| `nvidia` | nvtt, DDS utilities, PerfSDK 6.62 | MIT (nvtt, squish); NVIDIA SDK terms (PerfSDK) | tools, benchmark |
| `amd` | compress, GPUPerfAPI 2.3, tootle 2.2 | AMD library licenses (`compress/LibraryLicense.rtf`) | editor, tools |
| `WeifenLuo.WinFormsUI.Docking`, `WPFToolkit.Extended`, `PropertyBag` | 2.3, 1.4.0, 1.0 | MIT, Ms-PL, CodeProject | editor (.NET) |
| `scaleform` | GFx 4.2.22 subset (retail linked 4.2.21) | Autodesk Scaleform SDK terms, proprietary | UI (Flash) |
| `SpeedTree` | 5.2.1 | IDV proprietary, confidentiality markings | render (trees, grass) |
| `maya2011`, `maya2012` | devkits | Autodesk proprietary markings | Maya plugin |
