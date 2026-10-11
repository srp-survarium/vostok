# vostok

Binary-matching decompilation of the **Vostok Engine** used by Survarium
(v0.100b): C++ sources that, compiled with the original MSVC 8.0 / VS2008 SP1
toolchain, produce byte-identical objects to `survarium.exe`. Linux only; the
flake builds the exact toolchain under Wine, no Windows install needed.

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

## Requirements

Linux x86_64 with [Nix](https://nixos.org/download) and flakes enabled
(`experimental-features = nix-command flakes`). Everything else - the VS2008 SP1
compiler, the Windows and DirectX SDKs, Wine, ninja, the Rust tools, the game
binaries and the third-party libs - comes from the flake.

## Quickstart

Target branches are named by version: `v0.10b` (Survarium v0.100b) and `v0.20e`.
Root matching stacks in the target branch they belong to and land them there.

```sh
nix develop                 # first entry realizes ~18 GiB into the store, sets up Wine + ninja,
                            # and generates the target-side diff inputs from the original game
python3 -m vostok build     # build under Wine, regenerate the base-side inputs, refresh the ledger
```

Open the result in [objdiff](https://github.com/encounter/objdiff) with the
config at `binaries/objdiff/objdiff.json` and compare `base` (your build) against
`target` (the original game). Every full build also refreshes the committed ledger
`config/match_state.tsv` and the score block at the top of this README;
`python3 -m vostok build --code-only` is the faster iteration build.

How builds run under agents (notifications, locks, logs, timings) is in
[docs/build/agent-builds.md](docs/build/agent-builds.md); campaign queues and the
PDB declaration checks are in [docs/binary_matching/tools.md](docs/binary_matching/tools.md).

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

To investigate linker/PDB ordering without rebuilding game sources, run the
fixed-input VS2008 toy matrix with `python3 -m vostok tool pdb-order-probe`.
It records commands, artifacts, and complete `vostok-pdb topology --order --json`
comparisons under `binaries/gen/pdb-order-probes/`; see
[`vostok-pdb topology.md`](docs/binary_matching/vostok-pdb topology.md#causal-order-probes).
`python3 -m vostok tool link-layout runs|owners` reads the section contributions
of both PDBs: archive runs per section side by side, and the COMDAT functions
whose kept copy comes from a different compiland than retail's; see
[`link-layout.md`](docs/binary_matching/link-layout.md).
`python3 -m vostok diff image` compares the linked EXE with retail region by
region (headers, Rich header, imports, resources, code and data bytes and
order) against the banked counts in `config/image_diff.tsv`; see
[`target-vs-base.md`](docs/target-vs-base.md). It runs on demand, not in the build.

## Third-party libs and the GFx suite

The toolchain and third-party library archives are published in this repo's
[build-environment release](https://github.com/srp-survarium/vostok/releases/tag/v0.100b).
`flake.nix` pins their download URLs and SHA-256 hashes.

`binaries.prebuilt/` is staged on first entry from the pinned `vostok-libs` zip.
The Scaleform GFx static libs in it are the one part this repo builds
itself: `python3 -m vostok.build.gfx` compiles them from the pinned
[scaleform-gfx](https://github.com/srp-survarium/scaleform-gfx) source (the
`scaleform-gfx-src` flake input, recovered GFx 4.2.21) with the recipe the shipped
PDB records, and
`python3 -m vostok tool libs-release --tag <tag>` packages a new zip and prints the
hash to pin. Add `--upload` to upload it to the `v0.100b` release in
`srp-survarium/vostok`; `gh` is included in the dev shell. The
[toolchain packaging instructions](docs/build/toolchain-build.md) publish to the
same release. To *play* the game rather than match it, `scripts/create-wine-prefix.py`
builds a run-only Wine prefix from a bare clone.

The library archive and staging tool exclude the Sony Cell SDK and the PS3/Xbox
builds of `cs`, Lua, and luabind. Staging removes these directories from older
`binaries.prebuilt/` installations too. Console-only engine files and tools
are omitted; shared Windows/console implementations remain for the PC build.

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
python3 -m vostok ledger hist-gap --module render # real drops: the commits that reached and lost the peak (--all: every hist > max row)
python3 -m vostok ledger last-exact <fn> --diff   # the last exact commit's source for <fn>, against HEAD
git diff <rev> -- config/match_state.tsv          # which functions moved, and how
```

One function's evidence in one process - the ledger row, the PDB statement
partition (the `structure-diff` verdict), the relative-line geometry, the
`sema blocks` flow verdict and unmatched callees - read from the last build's
`binaries/pdb/*/evidence.sqlite` and the ledger, without rebuilding:

```sh
python3 -m vostok evidence 'weapon_core::tick'      # verdict line, then only what needs action
python3 -m vostok evidence <fn> --full              # matched rows, block diffs, callers too
python3 -m vostok evidence <fn> --json --only structure,flow
```

The first line is the verdict (`STRUCTURE MATCH, 2 SIZE residuals; cur 97.30,
max 98.10 (held)`); exit status 0 means nothing needs action, 1 something
does, 2 an ambiguous or unknown selector. A warning line flags evidence built
from an older revision of the source, whose line numbers no longer apply.

Campaigns are agent-driven: the skills under [`.agents/skills/`](.agents/skills/)
cover unit matching, structure verification, review and module-scale orchestration;
the loop is [`docs/binary_matching/agentic_loop.md`](docs/binary_matching/agentic_loop.md).

## Docs

- [docs/binary_matching/matching_guide.md](docs/binary_matching/matching_guide.md) - how to actually match assembly.
- [docs/index.md](docs/index.md) - index of build and per-module matching notes.
- [docs/build/toolchain-build.md](docs/build/toolchain-build.md) - how the VS2008 toolchain is built under Wine.
- [docs/binary_matching/ltcg.md](docs/binary_matching/ltcg.md) - what LTCG does to code, LTCG-or-source decision guide, fast-loop trust classes; `vostok sema ltcg <fn>` tags one residual.

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

**Proprietary SDKs** (each under its vendor's own license; included only so the
engine and its tools build as shipped)

| Tree | Version | Terms as carried in the tree | Used by |
| :-- | :-- | :-- | :-- |
| `scaleform` (not committed here: the [scaleform-gfx](https://github.com/srp-survarium/scaleform-gfx) flake input, written to `sources/scaleform` at setup) | GFx 4.2.21, recovered on the 4.2.22 release | Autodesk Scaleform SDK license | UI (Flash) |
| `SpeedTree` | 5.2.1 | IDV SDK license, confidentiality markings | render (trees, grass) |
| `maya2011`, `maya2012` | devkits | Autodesk devkit license, proprietary markings | Maya plugin |
| `nvidia/PerfSDK` | 6.62 | NVIDIA SDK license | benchmark |
| `amd` | compress, GPUPerfAPI 2.3, tootle 2.2 | AMD library licenses (`compress/LibraryLicense.rtf`) | editor, tools |

`cs` (1.3) is not third-party: it is a GSC-authored helper library carrying the
same headers as the engine, and falls under the same notice as the engine
sources.
