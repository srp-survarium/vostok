# SPDX-License-Identifier: GPL-3.0-or-later
"""vostok.build.shared_cache - one copy of the artifacts every worktree shares.

Three trees are the same in every worktree that pins the same inputs, and a
build only reads them:

  prebuilt          binaries.prebuilt/          staged from VOSTOK_LIBS_DIR  (~2.6 GB)
  pdb-target        binaries/pdb/target/        vostok-pdb index of retail   (~0.5 GB)
  structure-target  binaries/structure/target/  vostok-pdb carcass of retail (~0.2 GB)

The cache (paths.SHARED_CACHE) keeps one copy per entry and input key -
`<entry>/<key>/tree/` plus `manifest.json` with every file's size and sha256 -
and setup hard-links each worktree's files to it. Hard links, not a directory
symlink, so no writer can reach the shared copy: the cached files are
read-only, so an in-place write fails loudly, and a writer that replaces or
deletes a file (stage_file's atomic rename, an rmtree before regenerating)
only drops this worktree's link.

The key hashes the pinned inputs: the store path of the package or tool, the
sha256 of the retail exe and pdb (the retail_stamp identity), and the
prefixes the generator is given. A new key is generated
fresh into the cache, never adopted from a worktree whose copy may predate its
toolchain. When a worktree's recorded key changes, the cached tree is verified
by hash before it is linked; a damaged tree is set aside and regenerated.

A worktree file that differs from the cache and is not itself a cache link
is a local override and is kept for `prebuilt` (the GFx tool rebuilds shipping
libs there). The retail
entries are fully determined by their key, so a differing copy there is stale
and is replaced. `VOSTOK_SHARED_CACHE=0` turns the whole mechanism off.
"""

from __future__ import annotations

import contextlib
import fcntl
import hashlib
import json
import os
import shutil
import stat
import time
from dataclasses import dataclass
from pathlib import Path
from typing import Callable, Iterable

from vostok.core import paths
from vostok.core.log import logger

log = logger("shared-cache")

MANIFEST = "manifest.json"
TREE = "tree"
READ_ONLY = 0o444
RETAIL = ("pdb-target", "structure-target")


@dataclass(frozen=True)
class Entry:
    name: str
    path: Path
    inputs: Callable[[], dict | None]
    populate: Callable[[Path], None]
    keep_overrides: bool


def _resolved(value: str | os.PathLike | None) -> str | None:
    if not value:
        return None
    path = Path(value)
    return str(path.resolve()) if path.exists() else None


def _tool(name: str) -> str | None:
    found = shutil.which(os.environ.get("PDB_TOOL", name))
    return _resolved(found)


def _libs_dir() -> str | None:
    return _resolved(os.environ.get("VOSTOK_LIBS_DIR"))


def _prebuilt_inputs() -> dict | None:
    libs = _libs_dir()
    return {"vostok-libs": libs} if libs else None


def _retail_inputs(*engine: str) -> dict | None:
    """Keyed by the retail bytes (retail_stamp), never by a path: a v0.20e
    worktree can never link v0.100b's evidence, or the reverse."""
    from vostok.build import retail_stamp
    tool = _tool("vostok-pdb")
    try:
        retail = retail_stamp.retail_identity()
    except OSError:
        return None
    if not tool:
        return None
    return {"retail": retail, "vostok-pdb": tool, "engine": list(engine)}


def _populate_prebuilt(out: Path) -> None:
    """Stage vostok-libs exactly as vostok.tool.libs does, keeping store mtimes."""
    from vostok.tool import libs
    source = Path(_libs_dir() or "") / "sources"
    if not source.is_dir():
        raise RuntimeError(f"vostok-libs not found at {source}")
    for file in sorted(source.rglob("*")):
        if not file.is_file() or file.suffix.lower() not in libs.EXTS \
                or file.name in libs.LICENSE_NAMES:
            continue
        rel = file.relative_to(source)
        if libs.console_library_path(rel):
            continue
        target = libs.dest_for(rel, out)
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(file, target)


def _populate_pdb(out: Path) -> None:
    from vostok.build import generate_pdb
    generate_pdb.generate("target", out=out)


def _populate_structure(out: Path) -> None:
    from vostok.build import generate_structure
    generate_structure.generate("target", out=out)


def entries() -> tuple[Entry, ...]:
    return (
        Entry("prebuilt", paths.PREBUILT, _prebuilt_inputs, _populate_prebuilt, True),
        Entry("pdb-target", paths.TARGET_PDB_DIR,
              lambda: _retail_inputs(paths.RETAIL_SOURCE_PREFIX, paths.GFX_TARGET_PREFIX),
              _populate_pdb, False),
        Entry("structure-target", paths.TARGET_STRUCTURE_DIR,
              lambda: _retail_inputs(paths.RETAIL_SOURCE_PREFIX),
              _populate_structure, False),
    )


def enabled() -> bool:
    return os.environ.get("VOSTOK_SHARED_CACHE", "1") != "0"


def key_of(inputs: dict) -> str:
    return hashlib.sha256(json.dumps(inputs, sort_keys=True).encode()).hexdigest()[:16]


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as source:
        for chunk in iter(lambda: source.read(1 << 20), b""):
            digest.update(chunk)
    return digest.hexdigest()


def _files(root: Path) -> Iterable[Path]:
    for directory, _, names in os.walk(root):
        for name in names:
            yield Path(directory) / name


@contextlib.contextmanager
def _locked(path: Path):
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("a") as handle:
        try:
            fcntl.flock(handle, fcntl.LOCK_EX | fcntl.LOCK_NB)
        except BlockingIOError:
            log(f"waiting for {path.parent.name}: another worktree is filling it ...")
            fcntl.flock(handle, fcntl.LOCK_EX)
        yield


def _read_json(path: Path) -> dict:
    try:
        return json.loads(path.read_text(encoding="utf-8"))
    except (OSError, ValueError):
        return {}


def _write_json(path: Path, value: dict) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    temporary = path.with_name(f".{path.name}.{os.getpid()}.tmp")
    temporary.write_text(json.dumps(value, indent=1, sort_keys=True) + "\n", encoding="utf-8")
    os.replace(temporary, path)


def _remove(path: Path) -> None:
    if path.is_symlink() or path.is_file():
        path.unlink()
    elif path.is_dir():
        shutil.rmtree(path)


def verify(entry_dir: Path, manifest: dict) -> list[str]:
    """Relative paths whose cached bytes no longer match the manifest."""
    tree, bad = entry_dir / TREE, []
    for rel, (size, digest) in manifest["files"].items():
        file = tree / rel
        try:
            if file.stat().st_size != size or sha256(file) != digest:
                bad.append(rel)
        except OSError:
            bad.append(rel)
    return bad


def verify_key(name: str, key: str) -> list[str]:
    entry_dir = paths.SHARED_CACHE / name / key
    manifest = _read_json(entry_dir / MANIFEST)
    return verify(entry_dir, manifest) if manifest else ["manifest.json"]


def _populate(entry: Entry, inputs: dict, entry_dir: Path) -> dict:
    staging = entry_dir.with_name(f".{entry_dir.name}.{os.getpid()}.tmp")
    _remove(staging)
    (staging / TREE).mkdir(parents=True)
    log(f"{entry.name}: generating the shared copy for key {entry_dir.name} ...")
    started = time.monotonic()
    try:
        entry.populate(staging / TREE)
        files = {}
        for file in sorted(_files(staging / TREE)):
            rel = file.relative_to(staging / TREE).as_posix()
            files[rel] = [file.stat().st_size, sha256(file)]
            file.chmod(READ_ONLY)
        if not files:
            raise RuntimeError("generator produced no files")
        manifest = {"inputs": inputs, "files": files, "created": time.time(),
                    "bytes": sum(size for size, _ in files.values())}
        _write_json(staging / MANIFEST, manifest)
        os.replace(staging, entry_dir)
    except BaseException:
        _remove(staging)
        raise
    log(f"{entry.name}: {len(files):,} files, {manifest['bytes'] / 1e9:.2f} GB "
        f"in {time.monotonic() - started:.0f}s")
    return manifest


def _set_aside(entry_dir: Path, why: str) -> None:
    aside = entry_dir.with_name(f".bad-{entry_dir.name}-{int(time.time())}")
    os.replace(entry_dir, aside)
    log(f"{entry_dir.parent.name}: cached tree {entry_dir.name} {why}; moved to "
        f"{aside.name} and regenerating")


def _link(entry: Entry, tree: Path, manifest: dict, known: dict) -> tuple[int, int, int, dict]:
    """Point every manifest file of `entry.path` at the cached inode."""
    linked = present = 0
    overrides: dict[str, list[int]] = {}
    if entry.path.is_symlink():
        entry.path.unlink()
    for rel, (size, digest) in manifest["files"].items():
        source, target = tree / rel, entry.path / rel
        cached = source.stat()
        try:
            current = target.lstat()
        except FileNotFoundError:
            current = None
        if current is not None and stat.S_ISREG(current.st_mode):
            if (current.st_ino, current.st_dev) == (cached.st_ino, cached.st_dev):
                present += 1
                continue
            # A local writer leaves a single-link file; more links means an
            # older cache tree's inode, which is never an override.
            if entry.keep_overrides and current.st_nlink == 1:
                signature = [current.st_size, current.st_mtime_ns, current.st_ino]
                differs = (current.st_size != size or known.get(rel) == signature
                           or sha256(target) != digest)
                if differs:
                    overrides[rel] = signature
                    continue
        elif current is not None:
            _remove(target)
        target.parent.mkdir(parents=True, exist_ok=True)
        temporary = target.with_name(f".{target.name}.shared-link")
        temporary.unlink(missing_ok=True)
        os.link(source, temporary)
        os.replace(temporary, target)
        linked += 1
    if not entry.keep_overrides:
        _prune(entry.path, manifest["files"])
    return linked, present, len(overrides), overrides


def _prune(root: Path, files: dict) -> None:
    """Drop what a stale copy holds beyond the key-determined tree."""
    stale = [file for file in _files(root)
             if file.relative_to(root).as_posix() not in files]
    for file in stale:
        file.unlink()
    for directory, _, _ in sorted(os.walk(root), key=lambda row: -len(row[0])):
        if directory != str(root) and not os.listdir(directory):
            os.rmdir(directory)
    if stale:
        log(f"{root.name}: removed {len(stale):,} stale files "
            "the cached tree does not have")


def sync(refresh: Iterable[str] = (), names: Iterable[str] | None = None) -> set[str]:
    """Link this worktree's shared entries; return the names served from the cache.

    `refresh` names entries whose cached copy is regenerated even if present
    (setup's `--force target`). Best-effort: an entry that cannot be shared is
    logged and left to the per-worktree setup path, never failing setup.
    """
    served: set[str] = set()
    if not enabled():
        return served
    refresh, wanted = set(refresh), set(names) if names is not None else None
    stamp = _read_json(paths.SHARED_CACHE_STAMP)
    for entry in entries():
        if wanted is not None and entry.name not in wanted:
            continue
        try:
            record = _sync_one(entry, stamp.get(entry.name, {}), entry.name in refresh)
        except (OSError, RuntimeError, ValueError) as error:
            log(f"WARNING: {entry.name} not shared ({error}); this worktree keeps its own copy")
            continue
        except Exception as error:  # noqa: BLE001 - generator failures stay per-worktree
            log(f"WARNING: {entry.name} not shared ({type(error).__name__}: {error})")
            continue
        if record is not None:
            stamp[entry.name] = record
            served.add(entry.name)
    _write_json(paths.SHARED_CACHE_STAMP, stamp)
    return served


def _sync_one(entry: Entry, record: dict, refresh: bool) -> dict | None:
    inputs = entry.inputs()
    if inputs is None:
        log(f"{entry.name}: its pinned inputs are not available here; not shared")
        return None
    key = key_of(inputs)
    entry_dir = paths.SHARED_CACHE / entry.name / key
    with _locked(paths.SHARED_CACHE / entry.name / ".lock"):
        manifest = _read_json(entry_dir / MANIFEST) if entry_dir.is_dir() else {}
        if manifest and refresh:
            _set_aside(entry_dir, "refresh requested")
            manifest = {}
        elif manifest and record.get("key") != key:
            bad = verify(entry_dir, manifest)
            if bad:
                _set_aside(entry_dir, f"failed hash verification ({len(bad)} files, "
                                      f"e.g. {bad[0]})")
                manifest = {}
            else:
                log(f"{entry.name}: verified cached tree {key} by hash")
        elif not manifest and entry_dir.exists():
            _set_aside(entry_dir, "has no manifest")
        if not manifest:
            manifest = _populate(entry, inputs, entry_dir)
        known = record.get("overrides", {}) if record.get("key") == key else {}
        linked, present, kept, overrides = _link(entry, entry_dir / TREE, manifest, known)
    if linked or kept:
        log(f"{entry.name}: linked {linked:,} files to the cache, {present:,} already "
            f"linked" + (f", kept {kept:,} local overrides" if kept else ""))
    return {"key": key, "overrides": overrides}


def unlinked_bytes(entry: Entry) -> int:
    """Bytes this worktree holds for `entry` outside the cache (overrides)."""
    total = 0
    for file in _files(entry.path) if entry.path.is_dir() else ():
        info = file.lstat()
        if stat.S_ISREG(info.st_mode) and info.st_nlink == 1:
            total += info.st_size
    return total


def status() -> list[dict]:
    """Every cached tree, newest first, with its size and inputs."""
    rows = []
    for entry in entries():
        root = paths.SHARED_CACHE / entry.name
        for entry_dir in sorted(root.glob("*")) if root.is_dir() else ():
            if not entry_dir.is_dir():
                continue
            manifest = _read_json(entry_dir / MANIFEST)
            rows.append({"entry": entry.name, "key": entry_dir.name,
                         "files": len(manifest.get("files", {})),
                         "bytes": manifest.get("bytes", 0),
                         "created": manifest.get("created", 0),
                         "inputs": manifest.get("inputs", {}),
                         "valid": bool(manifest) and not entry_dir.name.startswith(".")})
    return sorted(rows, key=lambda row: -row["created"])
