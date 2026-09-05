# vostok

Binary-matching decompilation of the **Vostok Engine** used by Survarium
(v0.100b): C++ sources that, compiled with the original MSVC 8.0 / VS2008 SP1
toolchain, produce byte-identical objects to `survarium.exe`. Linux only; the
flake builds the exact toolchain under Wine, no Windows install needed.

<!-- match-score:start -->
## Match status

_Auto-generated from `config/match_state.tsv` (the committed matching ledger) - refreshed by `vostok build` at the end of every build; do not hand-edit. Diff this block across commits to spot regressions._

**Overall: 10,447 / 13,014 functions exact (80.28%) &middot; 11,038 / 13,014 functions exact-max (84.82%) &middot; 93.38% fuzzy &middot; 94.96% fuzzy-max.**

_All figures come from the ledger over every target function (paired plus inlined/folded target-only). **Functions exact** and **Fuzzy** describe the current build (`cur`). **Exact-max** and **Fuzzy-max** use `max`, the peak proven for the function's own source body (`hash`), which resets when that body changes; the all-time `hist` peak is never promoted into it, and a banked peak carrying no `hash` is not credited. Byte-weighted code view: `python3 -m vostok ledger readme --max-code`._

| Module          | Units |       Functions exact |   Functions exact-max |  Fuzzy | Fuzzy-max |
| :-------------- | ----: | --------------------: | --------------------: | -----: | --------: |
| `render`        |   351 | 2,027 / 2,734 (74.1%) | 2,247 / 2,734 (82.2%) |  91.3% |     93.3% |
| `game`          |   141 |   975 / 1,528 (63.8%) | 1,106 / 1,528 (72.4%) |  85.5% |     88.6% |
| `core`          |   136 | 1,172 / 1,325 (88.5%) | 1,259 / 1,325 (95.0%) |  97.2% |     98.6% |
| `vostok`        |   112 | 1,095 / 1,250 (87.6%) | 1,114 / 1,250 (89.1%) |  97.0% |     97.8% |
| `game_core`     |   189 |   828 / 1,181 (70.1%) |   857 / 1,181 (72.6%) |  93.6% |     95.7% |
| `animation`     |   102 |     572 / 727 (78.7%) |     594 / 727 (81.7%) |  91.3% |     92.0% |
| `ai`            |   124 |     644 / 691 (93.2%) |     657 / 691 (95.1%) |  99.3% |     99.6% |
| `sound`         |    69 |     491 / 510 (96.3%) |     493 / 510 (96.7%) |  99.8% |     99.9% |
| `collision`     |    52 |     461 / 503 (91.7%) |     469 / 503 (93.2%) |  98.4% |     98.7% |
| `scaleform`     |    47 |     418 / 454 (92.1%) |     424 / 454 (93.4%) |  96.7% |     96.9% |
| `particle`      |    25 |     389 / 400 (97.2%) |     397 / 400 (99.2%) |  99.6% |    100.0% |
| `vfs`           |    71 |     318 / 390 (81.5%) |     318 / 390 (81.5%) |  98.2% |     98.3% |
| `ui`            |    27 |     221 / 227 (97.4%) |     222 / 227 (97.8%) |  99.5% |     99.9% |
| `physics`       |    14 |     135 / 198 (68.2%) |     155 / 198 (78.3%) |  88.3% |     93.2% |
| `fs`            |    25 |     141 / 165 (85.5%) |     144 / 165 (87.3%) |  97.9% |     98.3% |
| `engine`        |    22 |     154 / 162 (95.1%) |     159 / 162 (98.1%) |  99.4% |     99.9% |
| `network`       |    25 |      85 / 159 (53.5%) |      93 / 159 (58.5%) |  90.5% |     91.8% |
| `network_core`  |    22 |      66 / 127 (52.0%) |      66 / 127 (52.0%) |  94.8% |     94.8% |
| `debug`         |    16 |     118 / 122 (96.7%) |     118 / 122 (96.7%) |  98.9% |     98.9% |
| `logging`       |    10 |       52 / 72 (72.2%) |       59 / 72 (81.9%) |  97.0% |     99.4% |
| `input`         |     9 |       52 / 53 (98.1%) |       52 / 53 (98.1%) |  99.9% |     99.9% |
| `survarium`     |     5 |       19 / 22 (86.4%) |       21 / 22 (95.5%) |  96.0% |     98.1% |
| `ai_navigation` |     3 |      14 / 14 (100.0%) |      14 / 14 (100.0%) | 100.0% |    100.0% |

_Updated 2026-09-05 &middot; delinker `d7e9292` (folded-symbol reconciliation)._
<!-- match-score:end -->

<!-- data-match:start -->
## Data reconstruction status

_Auto-generated from the consumer-owned objdiff projection plus the independent linked-image audit; it does not feed the function score._

**Function-reachable retail data: 98.13% (70,200,521 / 71,536,291 unique bytes) &middot; paired for comparison: 4.31% (3,084,121 / 71,536,291) &middot; matched projected copies: 66.01% (17,411,369 / 26,378,673).**

_The projection has 14,393 paired copies across 1,695 units and 31,648 unresolved blockers. Strict relocation comparison exposes 1,454 otherwise-exact functions (567,462 code bytes) with referent debt._

_The linked-image audit is 3.99% exact and covers 98.57% by PDB type extent, with 86.38% paired-only fidelity (10658 / 12179 definitions). Integrity ratchet: armed._

_Updated 2026-09-05._
<!-- data-match:end -->

## Requirements

Linux x86_64 with [Nix](https://nixos.org/download) and flakes enabled
(`experimental-features = nix-command flakes`). Everything else - the VS2008 SP1
compiler, the Windows and DirectX SDKs, Wine, ninja, the Rust tools, the game
binaries and the third-party libs - comes from the flake.

## Quickstart

```sh
nix develop                 # first entry realizes ~18 GiB into the store, sets up Wine + ninja,
                            # and generates the target-side diff inputs from the original game
python3 -m vostok build     # build under Wine, regenerate the base-side inputs, refresh the ledger
```

Build output is condensed to Ninja edge progress and actionable errors. The
complete unfiltered transcript from the latest run is kept in
`binaries/rebuild-output.log` for diagnostic review.

Open the result in [objdiff](https://github.com/encounter/objdiff) with the
config at `binaries/objdiff/objdiff.json` and compare `base` (your build) against
`target` (the original game). Every build also refreshes the committed ledger
`config/match_state.tsv` and the score block at the top of this README.
The Wine supervisor scopes cleanup to this worktree's prefix; if the PDB server
holds Ninja after a completed link or a failed pre-link edge, it reaps that
server and preserves Ninja's real success or failure result.

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

Generated PDB headers must preserve base-class access. The parser's flake patch
corrects its hardcoded `public` output; existing generated trees stay stale until
regenerated. Raw evidence: [base-access audit](docs/binary_matching/pdb_base_access_rendering.md).

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
It records commands, artifacts, and complete `pdb_topology --order --json`
comparisons under `binaries/gen/pdb-order-probes/`; see
[`pdb_topology.md`](docs/binary_matching/pdb_topology.md#causal-order-probes).

## Third-party libs and the GFx suite

`binaries.prebuilt/` is staged on first entry from the `vostok-libs` package the
flake pins (a zip on the
[vostok-build-env](https://github.com/srp-survarium/vostok-build-env/releases)
release). The Scaleform GFx static libs in it are the one part this repo builds
itself: `python3 -m vostok.build.gfx` compiles them from the pristine 4.2.22 SDK
(`nix develop .#with-scaleform-sdk`) with the recipe the shipped PDB records, and
`python3 -m vostok tool libs-release --tag <tag>` packages a new zip and prints the
hash to pin. To *play* the game rather than match it, `scripts/create-wine-prefix.py`
builds a run-only Wine prefix from a bare clone.

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
- [docs/build/toolchain-build.md](docs/build/toolchain-build.md) - how the VS2008 toolchain is built under Wine.

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
| `scaleform` | GFx 4.2.22 subset (retail linked 4.2.21) | Autodesk Scaleform SDK license | UI (Flash) |
| `SpeedTree` | 5.2.1 | IDV SDK license, confidentiality markings | render (trees, grass) |
| `maya2011`, `maya2012` | devkits | Autodesk devkit license, proprietary markings | Maya plugin |
| `nvidia/PerfSDK` | 6.62 | NVIDIA SDK license | benchmark |
| `amd` | compress, GPUPerfAPI 2.3, tootle 2.2 | AMD library licenses (`compress/LibraryLicense.rtf`) | editor, tools |

`cs` (1.3) is not third-party: it is a GSC-authored helper library carrying the
same headers as the engine, and falls under the same notice as the engine
sources.
