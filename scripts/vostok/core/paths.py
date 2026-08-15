"""vostok.core.paths - every repo path, in one place.

THE RULE: no module outside this one spells a repo-relative path. Nine scripts
used to re-derive the repo root and eight hard-coded artifact locations; moving
`match.db` then meant editing four copies, one was missed, and the miss silently
created an empty database that corrupted the README score block. A path that
lives here can only be wrong once.

Layout, mirrored by the constants below:

    sources/                    the engine source we compile
    binaries/                   ALL generated artifacts (gitignored)
      Win32/                      our build's exe + pdb
      objdiff/{base,target}/      delinked COFF, plus report.json
      structure/{base,target}/    pdb-parser's rendered headers/statements
      rich/{base,target}/         pdb_rich_context index for pdb_fetch
      match.db                    regenerable derivation cache
    docs/binary_matching/       committed matching state (the ledger, queues,
                                reviewed override tables)

Everything under `binaries/` is regenerable; everything under `docs/` is
committed truth. When in doubt, that is the line the constant tells you.
"""

from __future__ import annotations

import os
from pathlib import Path

_MARKERS = ("flake.nix", "sources")


def _find_repo() -> Path:
    """The worktree this package belongs to.

    `__file__` comes first: `python3 scripts/<shim>.py` puts that worktree's
    `scripts/` at the head of sys.path, so the package a shim imports is always
    its own worktree's - resolving anywhere else would silently mix two
    worktrees' artifacts. CWD is the fallback for an odd sys.path (a stale
    PYTHONPATH pointing at another checkout's scripts/).
    """
    for base in (Path(__file__).resolve().parent, Path.cwd().resolve()):
        for candidate in (base, *base.parents):
            if all((candidate / m).exists() for m in _MARKERS):
                return candidate
    raise RuntimeError("not inside a vostok worktree (no flake.nix + sources/ "
                       "above scripts/vostok/core/ nor above the cwd)")


REPO = _find_repo()

SCRIPTS = REPO / "scripts"
SOURCES = REPO / "sources"
ENGINE = SOURCES / "vostok"
SLN = SOURCES / "vostok v2.0.sln"
README = REPO / "README.md"
FLAKE_LOCK = REPO / "flake.lock"
COMPILE_COMMANDS = REPO / "compile_commands.json"

# --- generated artifacts (binaries/, gitignored) ---------------------------
BINARIES = REPO / "binaries"
PREBUILT = REPO / "binaries.prebuilt"
NIX_STORE = BINARIES / "nix-store"
NINJA_DIR = BINARIES / "ninja"
WINEPREFIX = BINARIES / ".wineprefix"
SETUP_STAMP = BINARIES / ".setup-stamp"

WIN32_DIR = BINARIES / "Win32"
BASE_PDB = WIN32_DIR / "survarium-dx11-win32-gold.pdb"
BASE_EXE = WIN32_DIR / "survarium-dx11-win32-gold.exe"

OBJDIFF_DIR = BINARIES / "objdiff"
OBJDIFF_CONFIG = OBJDIFF_DIR / "objdiff.json"
REPORT = OBJDIFF_DIR / "report.json"
REPORT_CHANGES = OBJDIFF_DIR / "report-changes.json"
REPORT_HEAD = OBJDIFF_DIR / "report.head"
CROSS_UNIT_REPORT = OBJDIFF_DIR / "report-cross-unit.json"
SYMBOL_MAP = OBJDIFF_DIR / "target-symbol-map.tsv"
EFFECTIVE_SYMBOL_MAP = OBJDIFF_DIR / "effective-target-symbol-map.tsv"

STRUCTURE_DIR = BINARIES / "structure"
TARGET_HEADERS = STRUCTURE_DIR / "target" / "headers"
TARGET_SOURCES = STRUCTURE_DIR / "target" / "sources"

RICH_DIR = BINARIES / "rich"
TARGET_IDX = RICH_DIR / "target" / "index.jsonl"
BASE_IDX = RICH_DIR / "base" / "index.jsonl"
DECLARATIONS = RICH_DIR / "target" / "declarations.jsonl"

#: the derivation cache. Committed truth is MATCH_STATE; this is regenerable
#: and gitignored (SQLite could not be diffed or merged - see ledger.store).
MATCH_DB = BINARIES / "match.db"

REBUILD_LOG = BINARIES / "rebuild.log"
MATCH_DB_LOG = BINARIES / "match_db.log"

# --- committed matching state (docs/, in git) ------------------------------
DOCS_MATCHING = REPO / "docs" / "binary_matching"
MATCH_STATE = DOCS_MATCHING / "match_state.tsv"
EXACT_FOLD_ALIASES = DOCS_MATCHING / "exact_fold_aliases.tsv"
MODULE_OWNERSHIP_OVERRIDES = DOCS_MATCHING / "module_ownership_overrides.tsv"
SYMBOL_MAP_OVERRIDES = DOCS_MATCHING / "folded_symbol_overrides.tsv"
ENUM_QUEUE = DOCS_MATCHING / "enum_queue.md"
STRUCTURE_MISMATCH_QUEUE = DOCS_MATCHING / "structure_mismatch_queue.md"


def survarium_bin() -> Path:
    """The shipped game (exe + pdb): $SURVARIUM_BIN, else the pinned gcroot."""
    return Path(os.environ.get("SURVARIUM_BIN", NIX_STORE / "survarium-game"))
