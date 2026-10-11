# vostok

Decompilation of the **Vostok Engine** used by Survarium (v0.100b).

<!-- match-score:start -->
## Match status

_Auto-generated from `config/match_state.tsv` (the committed matching ledger) - refreshed by `vostok build` at the end of every build; do not hand-edit. Diff this block across commits to spot regressions._

**Overall: 10,682 / 13,014 functions exact (82.08%) &middot; 11,266 / 13,014 functions exact-max (86.57%) &middot; 94.28% fuzzy &middot; 95.77% fuzzy-max.**

_All figures come from the ledger over every target function (paired plus inlined/folded target-only). **Functions exact** and **Fuzzy** describe the current build (`cur`). **Exact-max** and **Fuzzy-max** use `max`, the peak proven for the function's own source body (`hash`), which resets when that body changes; the all-time `hist` peak is never promoted into it, and a banked peak carrying no `hash` is not credited. Byte-weighted code view: `python3 -m vostok ledger readme --max-code`._

| Module          | Units |       Functions exact |   Functions exact-max |  Fuzzy | Fuzzy-max |
| :-------------- | ----: | --------------------: | --------------------: | -----: | --------: |
| `render`        |   351 | 2,061 / 2,734 (75.4%) | 2,281 / 2,734 (83.4%) |  92.6% |     94.6% |
| `game`          |   141 | 1,018 / 1,528 (66.6%) | 1,149 / 1,528 (75.2%) |  86.7% |     89.7% |
| `core`          |   136 | 1,172 / 1,325 (88.5%) | 1,259 / 1,325 (95.0%) |  97.4% |     98.8% |
| `vostok`        |   112 | 1,128 / 1,250 (90.2%) | 1,148 / 1,250 (91.8%) |  97.9% |     98.5% |
| `game_core`     |   189 |   864 / 1,181 (73.2%) |   891 / 1,181 (75.4%) |  94.9% |     96.6% |
| `animation`     |   102 |     604 / 727 (83.1%) |     627 / 727 (86.2%) |  93.6% |     94.3% |
| `ai`            |   124 |     647 / 691 (93.6%) |     659 / 691 (95.4%) |  99.3% |     99.6% |
| `sound`         |    69 |     499 / 510 (97.8%) |     500 / 510 (98.0%) |  99.8% |     99.9% |
| `collision`     |    52 |     461 / 503 (91.7%) |     469 / 503 (93.2%) |  98.4% |     98.7% |
| `scaleform`     |    47 |     420 / 454 (92.5%) |     425 / 454 (93.6%) |  96.7% |     96.9% |
| `particle`      |    25 |     391 / 400 (97.8%) |     398 / 400 (99.5%) |  99.7% |    100.0% |
| `vfs`           |    71 |     319 / 390 (81.8%) |     319 / 390 (81.8%) |  98.4% |     98.5% |
| `ui`            |    27 |     221 / 227 (97.4%) |     222 / 227 (97.8%) |  99.5% |     99.9% |
| `physics`       |    14 |     139 / 198 (70.2%) |     159 / 198 (80.3%) |  90.3% |     95.1% |
| `fs`            |    25 |     141 / 165 (85.5%) |     144 / 165 (87.3%) |  97.9% |     98.3% |
| `engine`        |    22 |     156 / 162 (96.3%) |     161 / 162 (99.4%) |  99.5% |    100.0% |
| `network`       |    25 |      90 / 159 (56.6%) |      93 / 159 (58.5%) |  90.6% |     91.3% |
| `network_core`  |    22 |      95 / 127 (74.8%) |      97 / 127 (76.4%) |  98.2% |     98.2% |
| `debug`         |    16 |     118 / 122 (96.7%) |     118 / 122 (96.7%) |  98.9% |     98.9% |
| `logging`       |    10 |       53 / 72 (73.6%) |       60 / 72 (83.3%) |  97.0% |     99.4% |
| `input`         |     9 |       52 / 53 (98.1%) |       52 / 53 (98.1%) |  99.9% |     99.9% |
| `survarium`     |     5 |       19 / 22 (86.4%) |       21 / 22 (95.5%) |  97.7% |     99.8% |
| `ai_navigation` |     3 |      14 / 14 (100.0%) |      14 / 14 (100.0%) | 100.0% |    100.0% |

_Updated 2026-10-11 &middot; delinker `d7e9292` (folded-symbol reconciliation)._
<!-- match-score:end -->

<!-- data-match:start -->
## Data reconstruction status

_Auto-generated from the consumer-owned objdiff projection plus the independent linked-image audit; it does not feed the function score._

**Function-reachable retail data: 98.14% (70,207,119 / 71,536,291 unique bytes) &middot; paired for comparison: 4.33% (3,095,511 / 71,536,291) &middot; matched projected copies: 66.09% (17,444,951 / 26,397,507).**

_The projection has 15,306 paired copies across 1,701 units and 30,823 unresolved blockers. Strict relocation comparison exposes 1,520 otherwise-exact functions (605,377 code bytes) with referent debt._

_The linked-image audit is 4.03% exact and covers 98.57% by PDB type extent, with 86.68% paired-only fidelity (11289 / 12807 definitions). Integrity ratchet: armed._

_Updated 2026-10-11._
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

To play rather than match, use `v0.10b-offline`; `scripts/create-wine-prefix.py` builds a run-only Wine prefix from a bare clone.

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
The Scaleform GFx libs are built by the project itself with
`python3 -m vostok.build.gfx`; packaging and publishing are described in
[docs/build/toolchain-build.md](docs/build/toolchain-build.md).

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
- [docs/binary_matching/tools.md](docs/binary_matching/tools.md) - campaign queue, PDB declaration checks, order probes.
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
| `scaleform` | GFx 4.2.22 subset (retail linked 4.2.21) | Autodesk Scaleform SDK license | UI (Flash) |
| `SpeedTree` | 5.2.1 | IDV SDK license, confidentiality markings | render (trees, grass) |
| `maya2011`, `maya2012` | devkits | Autodesk devkit license, proprietary markings | Maya plugin |
| `nvidia/PerfSDK` | 6.62 | NVIDIA SDK license | benchmark |
| `amd` | compress, GPUPerfAPI 2.3, tootle 2.2 | AMD library licenses (`compress/LibraryLicense.rtf`) | editor, tools |
