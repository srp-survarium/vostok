# vostok

Decompilation of the **Vostok Engine** used by Survarium (v0.100b).

<!-- match-score:start -->
## Match status

**`survarium.exe`: 96.06% matched (MAX)** - 11,374 / 13,014 functions exact (87.4%), weighted by code size.

| Score | Functions exact | Weighted | Meaning                                        |
| :---- | --------------: | -------: | :--------------------------------------------- |
| CUR   |          10,762 |   94.38% | last measured score                            |
| MAX   |          11,374 |   96.06% | best result for each function's current source |
| HIST  |          11,488 |   96.37% | all-time peak across source revisions          |

MAX by module:

| Module          | Units |   Functions exact MAX | Weighted MAX |
| :-------------- | ----: | --------------------: | -----------: |
| `render`        |   351 | 2,290 / 2,734 (83.8%) |       94.95% |
| `game`          |   141 | 1,170 / 1,528 (76.6%) |       89.99% |
| `core`          |   136 | 1,266 / 1,325 (95.5%) |       98.83% |
| `vostok`        |   112 | 1,152 / 1,250 (92.2%) |       98.60% |
| `game_core`     |   189 |   932 / 1,181 (78.9%) |       97.62% |
| `animation`     |   102 |     628 / 727 (86.4%) |       94.63% |
| `ai`            |   124 |     660 / 691 (95.5%) |       99.65% |
| `sound`         |    69 |     500 / 510 (98.0%) |       99.90% |
| `collision`     |    52 |     469 / 503 (93.2%) |       98.75% |
| `scaleform`     |    47 |     425 / 454 (93.6%) |       96.89% |
| `particle`      |    25 |     398 / 400 (99.5%) |       99.99% |
| `vfs`           |    71 |     328 / 390 (84.1%) |       98.55% |
| `ui`            |    27 |     223 / 227 (98.2%) |       99.90% |
| `physics`       |    14 |     160 / 198 (80.8%) |       95.36% |
| `fs`            |    25 |     154 / 165 (93.3%) |       99.19% |
| `engine`        |    22 |     161 / 162 (99.4%) |      100.00% |
| `network`       |    25 |      94 / 159 (59.1%) |       91.36% |
| `network_core`  |    22 |      97 / 127 (76.4%) |       98.22% |
| `debug`         |    16 |     118 / 122 (96.7%) |       98.89% |
| `logging`       |    10 |       61 / 72 (84.7%) |       99.62% |
| `input`         |     9 |       52 / 53 (98.1%) |       99.90% |
| `survarium`     |     5 |      22 / 22 (100.0%) |      100.00% |
| `ai_navigation` |     3 |      14 / 14 (100.0%) |      100.00% |

_Generated from `config/match_state.tsv` by `vostok build`; do not edit by hand._
<!-- match-score:end -->

<!-- data-match:start -->
## Data reconstruction status

_Auto-generated from the consumer-owned objdiff projection plus the independent linked-image audit; it does not feed the function score._

**Function-reachable retail data: 98.14% (70,207,119 / 71,536,291 unique bytes) &middot; paired for comparison: 98.14% (70,205,491 / 71,536,291) &middot; matched projected copies: 97.91% (420,087,935 / 429,051,363).**

_The projection has 15,400 paired copies across 1,701 units and 30,729 unresolved blockers. Strict relocation comparison exposes 1,501 otherwise-exact functions (624,019 code bytes) with referent debt._

_The linked-image audit is 97.93% exact and covers 98.57% by PDB type extent, with 99.37% paired-only fidelity (11805 / 13321 definitions). Integrity ratchet: armed._

_Updated 2026-10-12._
<!-- data-match:end -->

## Branches

```text
v0.10b ----------------------> v0.10b-offline
  |
  v
v0.20e
```

- [`v0.10b`](https://github.com/srp-survarium/vostok/tree/v0.10b#branches) - Survarium v0.100b (May 2013), byte-matching reconstruction
- [`v0.10b-offline`](https://github.com/srp-survarium/vostok/tree/v0.10b-offline#branches) - `v0.10b` plus an offline single-player client
- `v0.20e` - Survarium v0.20e target (planned)

To play rather than match, use `v0.10b-offline`.

## Requirements

Linux x86_64 with [Nix](https://nixos.org/download) flakes. The flake provides everything else, including the VS2008 toolchain under Wine.

## Quickstart

```sh
nix develop                          # first run fetches ~18 GiB and prepares the retail side
python3 -m vostok build              # full build and measurement; refreshes the ledger and this README
python3 -m vostok build --code-only  # faster iteration build (no data lane, no README)
```

Compare `base` (our build) with `target` (retail) in [objdiff](https://github.com/encounter/objdiff) using `binaries/objdiff/objdiff.json`. Agent builds: [docs/build/agent-builds.md](docs/build/agent-builds.md). Matching tools: [docs/binary_matching/tools.md](docs/binary_matching/tools.md).

## Layout

- `sources/vostok/` - the engine, one module per directory; `sources/<lib>/` -
  vendored third-party code (see [License](#license)); `resources/sources/shaders/`
  - the shipped shader sources.
- `scripts/vostok/` - the tooling, one Python package; `python3 -m vostok` maps it.
  [CLAUDE.md](CLAUDE.md) (`AGENTS.md` is the same file) holds the working agreements:
  the ledger, structure matching, the matching rules.
- `config/` - the committed ledger and retail data tables; `docs/` - matching notes
  and the pattern knowledge base; `binaries/`, `binaries.prebuilt/` - generated,
  gitignored.

## Source navigation

`compile_commands.json` and `clangd-vfs.yaml` at the root (generated by the dev
shell, gitignored) give clangd a clang-cl view of every TU in the MSVC 8.0 dialect;
point any LSP editor at the repo root, or ask from the shell:

```sh
python3 -m vostok tool clangd symbol weapon_lexeme_pair   # fuzzy workspace symbols
python3 -m vostok tool clangd def   <file> <line> [col]   # definition; also refs, hover
```

clang is a reader here; the Wine build is the only verdict on correctness.

## Third-party libs and the GFx suite

The toolchain and third-party library archives come from this repo's
[build-environment release](https://github.com/srp-survarium/vostok/releases/tag/v0.100b),
pinned by URL and SHA-256 in `flake.nix` and staged into `binaries.prebuilt/`.
The Scaleform GFx libs in that archive are prebuilt by the project from
[scaleform-gfx](https://github.com/srp-survarium/scaleform-gfx) (GFx 4.2.21, a
flake input) with `python3 -m vostok tool libs-release`; engine builds only link
them. Packaging and publishing: [docs/build/toolchain-build.md](docs/build/toolchain-build.md).

## Game data

`nix develop` realizes the game from one installer extraction and pins it under
`binaries/nix-store/`: `survarium-game` (exe, pdb, DLLs; `$SURVARIUM_BIN`),
`survarium-resources` (`resources.db`, ~1.5 GiB), `survarium-keys` (the lobby and
login server certificates), and `survarium-resources-unpacked` (the db expanded
by [vostok-resources-db](https://github.com/srp-survarium/vostok-resources-db)).
Each is also a standalone `nix build .#<name>`.

## Matching campaigns

`config/match_state.tsv` answers progress questions on a fresh clone:

```sh
python3 -m vostok ledger report --module render   # rollup; --per-unit for worst first
python3 -m vostok ledger queue  --module render   # one batch per TU, what a matcher is handed
git diff <rev> -- config/match_state.tsv          # which functions moved, and how
```

Campaigns are agent-driven: the skills under [`.agents/skills/`](.agents/skills/)
cover unit matching, structure verification, review and module-scale orchestration;
the loop is [`docs/binary_matching/agentic_loop.md`](docs/binary_matching/agentic_loop.md).

## Docs

- [docs/binary_matching/matching_guide.md](docs/binary_matching/matching_guide.md) - how to actually match assembly.
- [docs/index.md](docs/index.md) - index of build and per-module matching notes.
- [docs/build/toolchain-build.md](docs/build/toolchain-build.md) - how the VS2008 toolchain and the library archive are built.
- [docs/build/agent-builds.md](docs/build/agent-builds.md) - builds under Codex/Claude agents.
- [docs/binary_matching/tools.md](docs/binary_matching/tools.md) - matching tools: evidence, ledger history, PDB checks, layout and image diffs.
- [docs/binary_matching/ltcg.md](docs/binary_matching/ltcg.md) - what LTCG does to the code, and LTCG vs source shape.
- [docs/binary_matching/data_matching.md](docs/binary_matching/data_matching.md) - the data lane.

## License

The project's own work - the reconstruction in the engine sources, the tooling,
the flake and the docs - is [GPL-3.0-or-later](LICENSE); files it created carry an
`SPDX-License-Identifier` tag. The engine sources keep their original
`Copyright (C) GSC Game World` headers: the Vostok Engine and Survarium remain their
rights holders' property. No game binaries or resources are stored here, and
outputs built from the retail inputs are not covered by the GPL.

### Third-party code in the tree

`sources/` vendors the libraries the engine was built against. Each keeps its own
terms; proprietary SDKs are included only so the engine builds as shipped and will
be removed on a rights holder's request.

**Open-source libraries**

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
| `BugTrap` | 1.3.3291 | IntelleSoft free-software terms, see the tree | crash reporting |
| `fastdelegate` | 2005-05-30 | CodeProject terms | engine |
| `ParticleAPI` | 2.21 | author's free-use terms, see the tree | particles |
| `nvidia/nvt`, `nvidia/DDSUtilities` | nvtt, squish | MIT | texture tools |
| `WeifenLuo.WinFormsUI.Docking`, `WPFToolkit.Extended`, `PropertyBag` | 2.3, 1.4.0, 1.0 | MIT, Ms-PL, CodeProject | editor (.NET) |

**Proprietary SDKs**

| Tree | Version | Terms as carried in the tree | Used by |
| :-- | :-- | :-- | :-- |
| `scaleform` (not committed here: the [scaleform-gfx](https://github.com/srp-survarium/scaleform-gfx) flake input, written to `sources/scaleform` at setup) | GFx 4.2.21, recovered on the 4.2.22 release | Autodesk Scaleform SDK license | UI (Flash) |
| `SpeedTree` | 5.2.1 | IDV SDK license, confidentiality markings | render (trees, grass) |
| `maya2011`, `maya2012` | devkits | Autodesk devkit license, proprietary markings | Maya plugin |
| `nvidia/PerfSDK` | 6.62 | NVIDIA SDK license | benchmark |
| `amd` | compress, GPUPerfAPI 2.3, tootle 2.2 | AMD library licenses (`compress/LibraryLicense.rtf`) | editor, tools |
