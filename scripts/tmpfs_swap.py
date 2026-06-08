#!/usr/bin/env python3
"""
tmpfs_swap.py - relocate the write-heavy build dirs onto tmpfs (RAM).

The rebuild churns a lot of files through the build dirs under `binaries/`:
Win32 (cl.exe .obj intermediates + the linked PDB/EXE), objdiff (per-TU COFF
splits), structure (pdb-parser stubs), and rich (target asm / diff index). All
are fully regenerable from sources + the toolchain, so they are cheap to keep in
RAM: this script moves each into the already-mounted `/dev/shm` tmpfs and leaves
a symlink behind, so every read/write during a rebuild hits RAM instead of the
disk. No root needed (unlike `mount -t tmpfs`, which would need sudo on every
swap/restore).

  python3 scripts/tmpfs_swap.py            # swap Win32 + objdiff onto tmpfs
  python3 scripts/tmpfs_swap.py status     # show current state
  python3 scripts/tmpfs_swap.py restore    # move them back onto disk

The backing lives at $VOSTOK_TMPFS_DIR (default /dev/shm) under a per-checkout
directory, so multiple clones don't collide. tmpfs is volatile: after a reboot
the backing is gone and the symlinks dangle. A later `swap` (or `status`)
detects that and recreates the empty backing - but the previous contents are
lost, so re-run `scripts/setup-toolchain.py` (target side) / `scripts/rebuild.py`
(base side) to repopulate.
"""

import argparse
import hashlib
import os
import shutil
import sys
from pathlib import Path


SCRIPT_DIR = Path(__file__).resolve().parent
VOSTOK_DIR = SCRIPT_DIR.parent
BINARIES   = VOSTOK_DIR / "binaries"

# Dirs to relocate, relative to binaries/. All are regenerable from sources +
# the toolchain: Win32 (cl.exe .obj + linked PDB/EXE), objdiff (per-TU COFF),
# structure (pdb-parser stubs), rich (target asm / diff index).
SWAP_DIRS = ["Win32", "objdiff", "structure", "rich"]


def log(msg: str) -> None:
    print(f"[tmpfs] {msg}", flush=True)


def die(msg: str) -> None:
    print(f"[tmpfs] ERROR: {msg}", file=sys.stderr)
    sys.exit(1)


def tmpfs_root() -> Path:
    """Backing root for this checkout: <tmpfs>/vostok-build-<hash8>."""
    base = Path(os.environ.get("VOSTOK_TMPFS_DIR", "/dev/shm"))
    if not base.is_dir() or not os.access(base, os.W_OK):
        die(f"tmpfs dir {base} missing or not writable (set VOSTOK_TMPFS_DIR)")
    tag = hashlib.sha1(str(VOSTOK_DIR).encode()).hexdigest()[:8]
    return base / f"vostok-build-{tag}"


def points_into_backing(link: Path, backing: Path) -> bool:
    if not link.is_symlink():
        return False
    return os.path.normpath(os.readlink(link)) == str(backing)


def swap_one(name: str, backing_root: Path) -> None:
    link = BINARIES / name
    backing = backing_root / name

    if link.is_symlink():
        if not points_into_backing(link, backing):
            log(f"{name}: SKIP - symlink points elsewhere ({os.readlink(link)})")
            return
        if backing.is_dir():
            log(f"{name}: already on tmpfs")
            return
        # Symlink survived a reboot but the tmpfs backing did not - repair it.
        backing.mkdir(parents=True, exist_ok=True)
        log(f"{name}: backing was wiped (reboot?) - recreated EMPTY; repopulate it")
        return

    if backing.exists() and any(backing.iterdir()):
        die(f"{name}: backing {backing} already has data - run `restore` first")

    backing_root.mkdir(parents=True, exist_ok=True)
    if link.exists():
        shutil.move(str(link), str(backing))
        log(f"{name}: moved to {backing}")
    else:
        backing.mkdir(parents=True)
        log(f"{name}: created fresh on tmpfs (was absent on disk)")
    link.symlink_to(backing)
    log(f"{name}: {link} -> {backing}")


def restore_one(name: str, backing_root: Path) -> None:
    link = BINARIES / name
    backing = backing_root / name

    if not link.is_symlink():
        log(f"{name}: already on disk")
        return
    if not points_into_backing(link, backing):
        log(f"{name}: SKIP - symlink points elsewhere ({os.readlink(link)})")
        return

    link.unlink()
    if backing.is_dir():
        shutil.move(str(backing), str(link))
        log(f"{name}: moved back to {link}")
    else:
        link.mkdir()
        log(f"{name}: backing was gone - recreated EMPTY on disk; repopulate it")


def status_one(name: str, backing_root: Path) -> None:
    link = BINARIES / name
    backing = backing_root / name
    if link.is_symlink():
        if points_into_backing(link, backing):
            state = "tmpfs" if backing.is_dir() else "tmpfs (DANGLING - backing wiped)"
        else:
            state = f"symlink -> {os.readlink(link)}"
    elif link.is_dir():
        state = "disk"
    else:
        state = "absent"
    log(f"{name:8s} {state}")


def main() -> None:
    ap = argparse.ArgumentParser(description="Relocate build dirs onto tmpfs.")
    ap.add_argument(
        "action", nargs="?", default="swap",
        choices=["swap", "restore", "status"],
        help="swap (default), restore, or status",
    )
    args = ap.parse_args()

    backing_root = tmpfs_root()
    fn = {"swap": swap_one, "restore": restore_one, "status": status_one}[args.action]
    if args.action != "status":
        log(f"backing root: {backing_root}")
    for name in SWAP_DIRS:
        fn(name, backing_root)

    # Don't leave an empty per-checkout dir littering tmpfs after a restore.
    if args.action == "restore" and backing_root.is_dir() and not any(backing_root.iterdir()):
        backing_root.rmdir()


if __name__ == "__main__":
    main()
