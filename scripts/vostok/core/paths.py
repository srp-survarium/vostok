"""vostok.core.paths - every repo path, in one place.

THE RULE: no module outside this one spells a repo-relative path. Nine scripts
used to re-derive the repo root and eight hard-coded artifact locations; moving
the match database then meant editing four copies, one was missed, and the miss
silently created an empty database that corrupted the README score block. A path
that lives here can only be wrong once.

Layout, mirrored by the constants below:

    sources/                    the engine source we compile
    binaries/                   ALL generated artifacts (gitignored)
      Win32/                      our build's exe + pdb
      objdiff/{base,target}/      delinked COFF, plus report.json
      data/{base,target}/         PDB data indexes, image/relocation evidence
      structure/{base,target}/    pdb-parser's rendered headers/statements
      rich/{base,target}/         pdb_rich_context index for pdb_fetch
      base_only.tsv               why we emit what the target does not
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

    `__file__` comes first: whichever `scripts/` on sys.path won the import is
    the worktree this code IS, so its artifacts are the ones we mean -
    resolving anywhere else would silently mix two worktrees'. CWD is the
    fallback for an odd sys.path (a stale PYTHONPATH pointing at another
    checkout's scripts/).
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
# Recovered HLSL shader sources (the tree the shipped blobs' dependency tables
# name as `resources.sources/shaders/...`). 176 of the 261 shipped shader
# names are present; vostok.shaders coverage reports the live figure.
SHADER_SOURCES = REPO / "resources" / "sources" / "shaders"
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

# Era-exact disassembly of the shipped shader blobs (vostok.shaders disasm);
# regenerable from resources.db + fxc, so it lives with the other artifacts.
SHADER_DISASM = BINARIES / "shaders" / "disasm"

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

#: the base-only taxonomy: one verdict per function OUR build emits and the
#: target does not. A diagnostic about our own output, not campaign memory, so
#: it is a regenerable report here rather than a row in the committed ledger.
BASE_ONLY_REPORT = BINARIES / "base_only.tsv"

# Independent image-data matching lane.  It deliberately does not feed
# objdiff/report.json: function matching and data matching have different
# identities, denominators, and failure modes.
DATA_DIR = BINARIES / "data"
DATA_TARGET_DIR = DATA_DIR / "target"
DATA_BASE_DIR = DATA_DIR / "base"
DATA_TARGET_INDEX = DATA_TARGET_DIR / "index.tsv"
DATA_BASE_INDEX = DATA_BASE_DIR / "index.tsv"
DATA_TARGET_ACCESS = DATA_TARGET_DIR / "access.tsv"
DATA_BASE_ACCESS = DATA_BASE_DIR / "access.tsv"
DATA_TARGET_RELOCS = DATA_TARGET_DIR / "relocations.tsv"
DATA_BASE_RELOCS = DATA_BASE_DIR / "relocations.tsv"
DATA_REPORT = DATA_DIR / "report.json"
DATA_COVERAGE = DATA_DIR / "coverage.json"

# TU lists for the GFx-from-source build (vostok.build.gfx reads these)
# The Scaleform GFx SDK is compiled into the game but lives OUTSIDE sources/,
# so both the delinker and pdb_rich_context need a second --engine-path to keep
# its ~1,300 compilands (a single prefix dropped them on BOTH sides and their
# functions could never pair). Give each side the prefix that leaves the SAME
# relative path, so target and base key identically: both reduce to `Src\...`.
GFX_TARGET_PREFIX = r"c:\w\42216f4658640829\scaleform\releases\gfx_4.2.21"
SCALEFORM_SDK = Path(
    os.environ.get("SCALEFORM_SDK", str(Path.home() / "Projects/survarium/scaleform_sdk"))
)

# Merged GFx build tree: the pristine SDK linked file-by-file, with the repo's
# reconstructed 4.2.21 files (sources/scaleform/...) copied over it.
# vostok.build.gfx materializes it and compiles the lib TUs FROM it, so bare
# neighbor-includes ("Render_HAL.h") resolve to the reconstructed copies -
# an -I overlay cannot reach those. Both PDB readers strip this prefix on
# the base side (in addition to SCALEFORM_SDK, so pre-tree objs still key).
GFX_BUILD_TREE = BINARIES / "gfx-sdk"

# The vostok-libs release ships the GFx suite PREBUILT, and its objects record
# the tree the release was BUILT from - a foreign prefix to every other
# checkout, exactly like the retail GFX_TARGET_PREFIX above. The base readers
# strip it too, so release-staged libs key to the same `Src\...` paths as a
# local rebuild. Bump this alongside any vostok-libs gfx release cut from a
# different tree.
GFX_RELEASE_PREFIX = r"z:\home\sheep\projects\survarium\vostok-gfx\binaries\gfx-sdk"

GFX_TU_LISTS = SCRIPTS / "vostok" / "build" / "data"

REBUILD_LOG = BINARIES / "rebuild.log"
MATCH_DB_LOG = BINARIES / "match_db.log"

# --- committed matching state (docs/, in git) ------------------------------
DOCS_MATCHING = REPO / "docs" / "binary_matching"
MATCH_STATE = DOCS_MATCHING / "match_state.tsv"
EXACT_FOLD_ALIASES = DOCS_MATCHING / "exact_fold_aliases.tsv"
MEASURED_FOLD_ALIASES = DOCS_MATCHING / "measured_fold_aliases.tsv"
MODULE_OWNERSHIP_OVERRIDES = DOCS_MATCHING / "module_ownership_overrides.tsv"
SYMBOL_MAP_OVERRIDES = DOCS_MATCHING / "folded_symbol_overrides.tsv"
ENUM_QUEUE = DOCS_MATCHING / "enum_queue.md"
STRUCTURE_MISMATCH_QUEUE = DOCS_MATCHING / "structure_mismatch_queue.md"
DATA_STATE = DOCS_MATCHING / "data_state.tsv"
DATA_EXCLUSIONS = DOCS_MATCHING / "data_exclusions.tsv"
DATA_GATE = DOCS_MATCHING / "data_gate.tsv"


def survarium_bin() -> Path:
    """The shipped game (exe + pdb): $SURVARIUM_BIN, else the pinned gcroot."""
    return Path(os.environ.get("SURVARIUM_BIN", NIX_STORE / "survarium-game"))


def child_env(env: dict[str, str] | None = None) -> dict[str, str]:
    """Environment for a `sys.executable -m vostok.*` subprocess.

    A child python does not inherit our sys.path, and the package is only
    reachable through PYTHONPATH now that no flat scripts remain. Prepending
    THIS worktree's `scripts/` also keeps a child out of a sibling checkout
    that an inherited PYTHONPATH happens to name first.
    """
    env = dict(os.environ if env is None else env)
    parts = [p for p in (env.get("PYTHONPATH") or "").split(os.pathsep) if p]
    if str(SCRIPTS) in parts:
        parts.remove(str(SCRIPTS))
    env["PYTHONPATH"] = os.pathsep.join([str(SCRIPTS), *parts])
    return env
