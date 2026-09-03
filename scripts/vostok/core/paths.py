# SPDX-License-Identifier: GPL-3.0-or-later

"""vostok.core.paths - every repo path, in one place.

THE RULE: no module outside this one spells a repo-relative path. Nine scripts
used to re-derive the repo root and eight hard-coded artifact locations; moving
the match database then meant editing four copies, one was missed, and the miss
silently created an empty database that corrupted the README score block. A path
that lives here can only be wrong once.

Layout, mirrored by the constants below:

    sources/                    the engine source we compile
    config/                     committed machine-readable ledgers and policy
      retail/                     retail image censuses
      cleanliness/                audit ratchets
    binaries/                   ALL generated artifacts (gitignored)
      Win32/                      our build's exe + pdb
      objdiff/{base,target}/      delinked COFF, plus report.json
      data-objdiff/{base,target}/ consumer-owned data comparison project
      gen/                        generated manifests and data audits
      structure/{base,target}/    pdb-parser's rendered headers/statements
      rich/{base,target}/         pdb_rich_context index for pdb_fetch
      base_only.tsv               why we emit what the target does not
    docs/binary_matching/       matching workflow and evidence prose

Everything under `binaries/` is regenerable. Machine-readable committed state
lives under `config/`; explanatory text lives under `docs/`.
"""

from __future__ import annotations

import mmap
import os
import re
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
CONFIG = REPO / "config"
DOCS = REPO / "docs"
# Recovered HLSL shader sources (the tree the shipped blobs' dependency tables
# name as `resources.sources/shaders/...`). 176 of the 261 shipped shader
# names are present; vostok.shaders coverage reports the live figure.
SHADER_SOURCES = REPO / "resources" / "sources" / "shaders"
ENGINE = SOURCES / "vostok"
SLN = SOURCES / "vostok v2.0.sln"
README = REPO / "README.md"
FLAKE_LOCK = REPO / "flake.lock"
COMPILE_COMMANDS = REPO / "compile_commands.json"

# Retail compiled the engine from this virtual source root.  Its project
# working directories used a lower-case drive while its source include switch
# used an upper-case one; MSVC preserves both spellings in __FILE__.
RETAIL_SOURCE_PREFIX = r"c:\survarium\sources"
RETAIL_INCLUDE_SOURCE_PREFIX = r"C:\survarium\sources"

# --- generated artifacts (binaries/, gitignored) ---------------------------
BINARIES = REPO / "binaries"
GEN_DIR = BINARIES / "gen"
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
DATA_OBJDIFF_DIR = BINARIES / "data-objdiff"
OBJDIFF_CONFIG = OBJDIFF_DIR / "objdiff.json"
REPORT = OBJDIFF_DIR / "report.json"
DATA_OBJDIFF_REPORT = DATA_OBJDIFF_DIR / "report.json"
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

# Data-topology names mirror Gruntz: tracked retail facts and ratchets live in
# config/{retail,cleanliness}; every generated audit or delinker input lives in
# binaries/gen. The image audit remains separate from objdiff until the two
# delink manifests below actually enroll target COFF data.
DATA_TARGET_INDEX = GEN_DIR / "target_data_index.tsv"
DATA_BASE_INDEX = GEN_DIR / "base_data_index.tsv"
DATA_TARGET_ACCESS = GEN_DIR / "data_access_map.tsv"
DATA_BASE_ACCESS = GEN_DIR / "base_data_access_map.tsv"
DATA_TARGET_RELOCS = GEN_DIR / "target_data_relocations.tsv"
DATA_BASE_RELOCS = GEN_DIR / "base_data_relocations.tsv"
DATA_MISSING_CANDIDATES = GEN_DIR / "missing_data_candidates.tsv"
DATA_MISSING_XREFS = GEN_DIR / "missing_data_xrefs.tsv"
DATA_MISSING_REPORT = GEN_DIR / "missing_data_report.json"
DATA_REPORT = GEN_DIR / "data_image_report.json"
DATA_COVERAGE = GEN_DIR / "data_coverage.json"
DATA_STATE = GEN_DIR / "data_image_state.tsv"
DATA_COVERAGE_GAPS = GEN_DIR / "data_coverage_gaps.tsv"
DATA_BASE_GEN = GEN_DIR / "base"
# Keep the target manifest names and locations identical to Gruntz. Vostok
# additionally needs a base projection because LTCG left us no compiler objs.
DELINK_DATA_MANIFEST = GEN_DIR / "delink_data_manifest.tsv"
DELINK_DATA_SECTION_MANIFEST = GEN_DIR / "delink_data_section_manifest.tsv"
BASE_DELINK_DATA_MANIFEST = DATA_BASE_GEN / "delink_data_manifest.tsv"
BASE_DELINK_DATA_SECTION_MANIFEST = DATA_BASE_GEN / "delink_data_section_manifest.tsv"
DATA_CONSUMER_CLOSURE = GEN_DIR / "data_consumer_closure.tsv"
DATA_FUNCTION_STATE = GEN_DIR / "data_function_state.tsv"
DATA_MANIFEST_BLOCKERS = GEN_DIR / "data_manifest_blockers.tsv"
DATA_STRICT_REPORT = GEN_DIR / "data_strict_report.json"
DATA_SYMBOL_MAP = DATA_OBJDIFF_DIR / "target-symbol-map.tsv"


def _data_module_name(module: str) -> str:
    if not module or not module.replace("_", "").isalnum():
        raise ValueError(f"invalid data-audit module name: {module!r}")
    return module


def data_module_reloc_audit(module: str) -> Path:
    return GEN_DIR / f"{_data_module_name(module)}_reloc_audit.tsv"


def data_module_extentless(module: str) -> Path:
    return GEN_DIR / f"{_data_module_name(module)}_extentless_data.tsv"


def data_module_function_data(module: str) -> Path:
    return GEN_DIR / f"{_data_module_name(module)}_function_data.tsv"


def data_module_reloc_report(module: str) -> Path:
    return GEN_DIR / f"{_data_module_name(module)}_reloc_report.json"


DATA_RENDER_RELOC_AUDIT = data_module_reloc_audit("render")
DATA_RENDER_EXTENTLESS = data_module_extentless("render")
DATA_RENDER_FUNCTION_DATA = data_module_function_data("render")
DATA_RENDER_RELOC_REPORT = data_module_reloc_report("render")
DATA_MODULE_RELOC_REPORT = GEN_DIR / "data_module_reloc_report.json"
DATA_FUNCTION_OPEN = GEN_DIR / "data_function_open.tsv"

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
GFX_OBJECT_TREE = WIN32_DIR / "intermediates" / "gfx"

# The suite is compiled through a fixed Wine-side alias of GFX_BUILD_TREE
# (C:\survarium\gfx-sdk, a symlink the toolchain creates beside
# C:\survarium\sources), so every object records this machine-independent
# prefix instead of the checkout's Z: path - and so does the vostok-libs
# release, which `vostok tool libs-release` cuts from the same tree. The base
# readers strip it like the retail GFX_TARGET_PREFIX above, so release-staged
# and locally rebuilt libs key to the same `Src\...` paths.
GFX_TREE_PREFIX = r"c:\survarium\gfx-sdk"
GFX_RELEASE_PREFIX = GFX_TREE_PREFIX
GFX_OBJECT_PREFIX = r"c:\survarium\gfx-obj"
GFX_SHIPPING_ARCHIVES = tuple(
    PREBUILT / "Win32" / "libraries" / "shipping" / name
    for name in (
        "libgfx.lib",
        "libgfx_as2.lib",
        "libgfx_as3.lib",
        "libgfx_zlib.lib",
        "libgfx_libpng.lib",
        "libgfx_libjpeg.lib",
        "libgfxexpat.lib",
        "pcre.lib",
    )
)

_GFX_ARCHIVE_INCLUDE = re.compile(
    rb"-I([A-Za-z]:\\[^ \x00\"\r\n]{1,300}\\(?:gfx-sdk|scaleform_sdk))"
    rb"\\(?:Include|Src|3rdParty)",
    re.IGNORECASE,
)


def gfx_recorded_prefixes(artifacts: tuple[Path, ...] | None = None) -> tuple[str, ...]:
    """Return GFx source roots recorded by the current PDB or staged archives.

    Release archives are supposed to record ``GFX_RELEASE_PREFIX``.  Older
    C++ and C payloads recorded two different builder-absolute Wine paths
    instead. Readers must recognize the provenance in the PDB they are parsing
    so restaging those payloads cannot silently delete GFx functions from a
    generated comparison. Before the first link, fall back to inspecting the
    staged archives themselves.
    """
    if artifacts is None:
        artifacts = (BASE_PDB,) if BASE_PDB.is_file() else GFX_SHIPPING_ARCHIVES
    result: list[str] = []
    seen: set[str] = set()
    for artifact in artifacts:
        if not artifact.is_file() or artifact.stat().st_size == 0:
            continue
        with artifact.open("rb") as source:
            with mmap.mmap(source.fileno(), 0, access=mmap.ACCESS_READ) as contents:
                matches = [match.group(1) for match in _GFX_ARCHIVE_INCLUDE.finditer(contents)]
        for raw in matches:
            prefix = raw.decode("ascii")
            key = prefix.casefold()
            if key not in seen:
                seen.add(key)
                result.append(prefix)
    return tuple(result)


def gfx_release_prefixes() -> tuple[str, ...]:
    """All fixed and artifact-recorded GFx roots accepted on the base side."""
    result = [GFX_RELEASE_PREFIX]
    seen = {prefix.casefold() for prefix in result}
    for recorded in gfx_recorded_prefixes():
        if recorded.casefold() not in seen:
            seen.add(recorded.casefold())
            result.append(recorded)
    return tuple(result)

GFX_TU_LISTS = SCRIPTS / "vostok" / "build" / "data"

# Tiny committed VS2008 inputs and generated evidence for causal PDB-order
# experiments. These never compile or link the game.
PDB_ORDER_PROBE_FIXTURES = SCRIPTS / "vostok" / "tool" / "data" / "pdb_order_probes"
PDB_ORDER_PROBE_OUTPUT = GEN_DIR / "pdb-order-probes"

REBUILD_LOG = BINARIES / "rebuild.log"


def _main_worktree() -> Path:
    """REPO, or - when REPO is a LINKED worktree - the main checkout.

    Matching runs in throwaway sibling worktrees (`.claude/worktrees/`), each
    with its own gitignored `binaries/`. A usage trail written there dies with
    the worktree, so the one log that answers "how did the agents use the
    tooling" is kept in the main checkout and keyed by branch. `.git` is read
    rather than shelled out to: this resolves on every tool invocation.
    """
    dot_git = REPO / ".git"
    try:
        if dot_git.is_file():  # linked worktree: "gitdir: <main>/.git/worktrees/<n>"
            gitdir = dot_git.read_text(encoding="utf-8").split("gitdir:", 1)[1]
            main_git, sep, _ = gitdir.strip().partition("/worktrees/")
            if sep:
                return Path(main_git).parent
    except (OSError, IndexError):
        pass  # detached checkout, unreadable .git: log where we stand
    return REPO


# Every `python3 -m vostok...` invocation appends one line here (vostok.core.log
# `record`); `vostok tool usage` reads it back. Same shape as pdb_fetch.log /
# rebuild.log, but pooled in the main checkout so worktree agents
# all feed the one trail.
USAGE_LOG = _main_worktree() / "binaries" / "vostok_usage.log"

# --- committed machine-readable state (config/, in git) --------------------
DOCS_MATCHING = REPO / "docs" / "binary_matching"
DOCS_DATA_MATCHING = DOCS / "data_matching"
RETAIL_CONFIG = CONFIG / "retail"
CLEANLINESS_CONFIG = CONFIG / "cleanliness"
MATCH_STATE = CONFIG / "match_state.tsv"
EXACT_FOLD_ALIASES = CONFIG / "exact_fold_aliases.tsv"
MEASURED_FOLD_ALIASES = CONFIG / "measured_fold_aliases.tsv"
MODULE_OWNERSHIP_OVERRIDES = CONFIG / "module_ownership_overrides.tsv"
SYMBOL_MAP_OVERRIDES = CONFIG / "folded_symbol_overrides.tsv"
ENUM_QUEUE = DOCS_MATCHING / "enum_queue.md"
STRUCTURE_MISMATCH_QUEUE = GEN_DIR / "structure_mismatch_queue.md"
RETAIL_DATA = RETAIL_CONFIG / "data.tsv"
RETAIL_DATA_SYMBOLS = RETAIL_CONFIG / "data_symbols.tsv"
RETAIL_PDB_DATA_EXTENTS = RETAIL_CONFIG / "pdb_data_extents.tsv"
RETAIL_RELOC_REFERENTS = RETAIL_CONFIG / "reloc_referents.tsv"
DATA_INTEGRITY_RATCHET = CLEANLINESS_CONFIG / "data-integrity-ratchet.tsv"


def data_module_problems(module: str) -> Path:
    return DOCS_DATA_MATCHING / f"{_data_module_name(module)}_data_problems.md"


DATA_RENDER_PROBLEMS = data_module_problems("render")


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
