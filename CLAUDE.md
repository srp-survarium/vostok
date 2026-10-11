# vostok - offline lane (AGENTS.md is this file)

This branch (`v0.10b-offline`) is `v0.10b` - the decompiled Vostok Engine
(Survarium v0.100b) - plus offline-only commits: a playable single-player client
and its release script. The binary-matching tooling (ledger, objdiff, PDB
evidence, matching docs and skills) is stripped here; it lives on `v0.10b`.

    nix develop
    python3 -m vostok build                 # -> binaries/Win32/survarium-dx11-win32-gold.exe
    scripts/release/make-release.sh --resources-db <resources.db> --out <dir>

- Engine fixes that are not offline-specific belong on `v0.10b`, where they are
  matched; keep offline changes small and in `offline:` / `release:` commits.
- New project-written files start with `// SPDX-License-Identifier: GPL-3.0-or-later`
  (`#` in shell/Python/Nix). After editing `scripts/`, run `ruff check scripts/`.
