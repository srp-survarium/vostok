# SPDX-License-Identifier: GPL-3.0-or-later
"""vostok.build.retail_stamp - which retail binary a target artifact came from.

Every target-side artifact is derived from the retail survarium.exe and .pdb
the flake pins (survarium-game). A worktree whose target side was generated
from another branch's retail - v0.20e instead of v0.100b - measures every
function against the wrong game, and nothing downstream notices: scores just
collapse. So each generator stamps its output with the sha256 of the two retail
files it read (after homm3's provenance stamps), and setup and build compare
the stamps with the retail the current flake pins:

  binaries/pdb/target/            vostok-pdb evidence index
  binaries/structure/target/      vostok-pdb carcass
  binaries/objdiff/target/        delinked retail COFF, plus its symbol map
  binaries/data-objdiff/target/   retail data COFF, plus its symbol map
  binaries/rich/target/           legacy index; nothing generates or reads it

A directory carries `.retail-stamp.json`; a file carries a sibling
`<name>.retail-stamp.json`. A different stamp is a hard error naming both
retails, fixed by `vostok build --regenerate-target`. A missing stamp predates
stamping: setup regenerates that artifact rather than trust it.
"""

from __future__ import annotations

import functools
import hashlib
import json
import os
import time
from dataclasses import dataclass
from pathlib import Path

from vostok.core import paths

SCHEMA = 1
STAMP_NAME = ".retail-stamp.json"
STAMP_SUFFIX = ".retail-stamp.json"
RETAIL_FILES = ("survarium.exe", "survarium.pdb")
REGENERATE_HINT = ("regenerate the target side: `python3 -m vostok build --regenerate-target` "
                   "(or enter the shell with `VOSTOK_REGENERATE_TARGET=1 nix develop`)")


class RetailMismatch(RuntimeError):
    pass


@dataclass(frozen=True)
class Artifact:
    name: str
    path: Path
    legacy: bool = False        # nothing regenerates it; report, never fail


def artifacts() -> tuple[Artifact, ...]:
    return (
        Artifact("pdb-target", paths.TARGET_PDB_DIR),
        Artifact("structure-target", paths.TARGET_STRUCTURE_DIR),
        Artifact("objdiff-target", paths.OBJDIFF_DIR / "target"),
        Artifact("objdiff-symbol-map", paths.SYMBOL_MAP),
        Artifact("data-objdiff-target", paths.DATA_OBJDIFF_DIR / "target"),
        Artifact("data-symbol-map", paths.DATA_SYMBOL_MAP),
        Artifact("rich-target", paths.LEGACY_RICH_DIR / "target", legacy=True),
    )


@functools.lru_cache(maxsize=8)
def _digest(path: str, size: int, mtime_ns: int) -> str:
    digest = hashlib.sha256()
    with open(path, "rb") as source:
        for chunk in iter(lambda: source.read(1 << 20), b""):
            digest.update(chunk)
    return digest.hexdigest()


def retail_identity(game: Path | None = None) -> dict[str, str]:
    """sha256 of the pinned retail exe and pdb (memoized per file state)."""
    game = Path(game or paths.survarium_bin())
    identity = {}
    for name in RETAIL_FILES:
        file = (game / name).resolve()
        info = file.stat()
        identity[name] = _digest(str(file), info.st_size, info.st_mtime_ns)
    return identity


def stamp_path(artifact: Path) -> Path:
    artifact = Path(artifact)
    if artifact.is_dir() or not artifact.suffix:
        return artifact / STAMP_NAME
    return artifact.with_name(artifact.name + STAMP_SUFFIX)


def clear(artifact: Path) -> None:
    """Drop a stamp before regenerating, so a failed run leaves none."""
    stamp_path(artifact).unlink(missing_ok=True)


def write(artifact: Path, game: Path | None = None) -> Path:
    stamp = stamp_path(artifact)
    stamp.parent.mkdir(parents=True, exist_ok=True)
    payload = {"schema": SCHEMA, "retail": retail_identity(game),
               "game": str(Path(game or paths.survarium_bin()).resolve()),
               "stamped": time.strftime("%Y-%m-%d %H:%M:%S")}
    temporary = stamp.with_name(f".{stamp.name}.{os.getpid()}.tmp")
    stamp.unlink(missing_ok=True)       # may be a read-only cache link
    temporary.write_text(json.dumps(payload, indent=1, sort_keys=True) + "\n", encoding="utf-8")
    os.replace(temporary, stamp)
    return stamp


def read(artifact: Path) -> dict | None:
    try:
        payload = json.loads(stamp_path(artifact).read_text(encoding="utf-8"))
    except (OSError, ValueError):
        return None
    return payload if isinstance(payload, dict) and payload.get("schema") == SCHEMA else None


def _exists(path: Path) -> bool:
    return path.is_file() or (path.is_dir() and any(
        child.name != STAMP_NAME for child in path.iterdir()))


def _short(identity: dict) -> str:
    return ", ".join(f"{name} {identity.get(name, '?')[:12]}" for name in RETAIL_FILES)


def audit(expected: dict[str, str] | None = None) -> tuple[list[str], list[Artifact], list[str]]:
    """(mismatches, unstamped regenerable artifacts, legacy notes) for this worktree."""
    expected = expected or retail_identity()
    mismatches, unstamped, notes = [], [], []
    for artifact in artifacts():
        if not _exists(artifact.path):
            continue
        stamp = read(artifact.path)
        where = artifact.path.relative_to(paths.REPO) if artifact.path.is_relative_to(paths.REPO) \
            else artifact.path
        if stamp is None:
            if artifact.legacy:
                notes.append(f"{where} has no retail stamp; no tool reads or regenerates it "
                             "(delete it)")
            else:
                unstamped.append(artifact)
        elif stamp.get("retail") != expected:
            mismatches.append(f"{where} was derived from {_short(stamp.get('retail', {}))}, "
                              f"but this worktree pins {_short(expected)}")
    return mismatches, unstamped, notes


def require_current(expected: dict[str, str] | None = None) -> list[Artifact]:
    """Raise RetailMismatch on a stamp from another retail; return the unstamped."""
    mismatches, unstamped, _ = audit(expected)
    if mismatches:
        raise RetailMismatch(
            "the target side was generated from a DIFFERENT retail binary than the one "
            "this worktree's flake pins (e.g. another branch's v0.20e):\n  "
            + "\n  ".join(mismatches) + "\n" + REGENERATE_HINT)
    return unstamped
