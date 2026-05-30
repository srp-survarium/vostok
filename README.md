# vostok

Binary-matching decompilation of the **Vostok Engine** used by Survarium
(v0.100b). The goal is C++ sources that, compiled with the original toolchain
(MSVC 8.0 / VS2008 SP1), produce byte-identical objects to `survarium.exe`.

This README covers the **Nix / Linux** workflow, which builds the exact VS2008
toolchain under Wine for you — no Windows install required. For the original
manual Windows/VS2008 setup, see [docs/windows-setup.md](docs/windows-setup.md).

## Requirements

- Linux (x86_64) with [Nix](https://nixos.org/download) and flakes enabled
  (`experimental-features = nix-command flakes`).

That's it — everything else (the VS2008 SP1 compiler, Windows/DirectX SDKs,
Wine, ninja, and the Rust tools) is provided by the flake.

## Quickstart

```sh
nix develop
```

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
`binaries/objdiff/objdiff.json`) and match `base` against `target`.

Useful individual scripts (all run inside `nix develop`):

```sh
python3 scripts/ninja_build.py [target]        # build only (verbose, keep-going)
python3 scripts/delink.py {base|target}        # COFF split for one side
python3 scripts/generate_structure.py {base|target}   # pdb-parser stubs for one side
```

## Game data

The single installer extraction also exposes the rest of the game as separate
Nix outputs (no extra download — it all comes from the same unpack as the
binaries):

```sh
nix build .#survarium-resources   # resources.db + resources/ (packed game data)
nix build .#survarium-keys         # lobby/login server SSL certs + private keys
```

`nix develop` pins all three under `binaries/nix-store/` (`survarium-game`,
`survarium-resources`, `survarium-keys`).

## Docs

- [Matching guide](https://gist.github.com/sushi-shi/8bf16f82c3b1c65fd357d73ecfda909e) — how to actually match assembly.
- [docs/index.md](docs/index.md) — index of build and per-module matching notes.
- [docs/build/toolchain-build.md](docs/build/toolchain-build.md) — how the VS2008 toolchain is built under Wine (and why Wine must be staging).
- [docs/windows-setup.md](docs/windows-setup.md) — the legacy manual Windows/VS2008 workflow.
