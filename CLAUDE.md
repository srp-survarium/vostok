# vostok - Claude working notes

This repo binary-matches the Vostok Engine (Survarium v0.100b): we write C++
that, compiled with the original toolchain (MSVC 8.0 / VS2008), produces
byte-identical object files to the shipped `survarium.exe` + `survarium.pdb`.
Matching is verified with `objdiff`, comparing our compiled "base" objects
against the original game's "target" objects.

These are the repo-local working agreements, auto-loaded when working in `vostok/`.

## ASCII only

Write plain ASCII everywhere: source, comments, docstrings, printed/log strings,
commit messages, and PR descriptions. Do not use Unicode punctuation. Replace:

- em / en dashes with `-` (or `--`)
- the ellipsis character with `...`
- the rightwards arrow with `->`
- check marks with `OK`
- "smart" / curly quotes with straight `'` and `"`

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
`objdiff-cli report generate -p binaries/objdiff`.

## Keep the README current

README.md documents this Nix/Linux workflow and goes stale easily. Whenever you
change something fundamental - the build/diff flow, script names or locations,
devShell tools, generated artifacts, or paths - check README.md and update it in
the same change.
