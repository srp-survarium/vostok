"""
copy_lib_files.py - Stage prebuilt 3rd party libraries ('.dll's, '.lib's, ...)
into the repo.

These are binary blobs, not source, so they land in a top-level
`binaries.prebuilt/` tree (gitignored) that mirrors how the shipped game stored
them - NOT in `sources/`, which is the SDK source tree. The GFx (Scaleform)
Shipping libs are remapped onto the original game's layout
(`binaries.prebuilt/Win32/libraries/shipping/`); every other blob keeps its
source-relative subpath under `binaries.prebuilt/`. Include dirs still point at
`sources/` (the SDK headers), so only the link search path moves.
"""

import argparse
import os
import shutil
from pathlib import Path

from vostok.core.paths import PREBUILT, REPO

EXTS = {'.pdb', '.exe', '.dll', '.a', '.lib'}

# Lib source defaults to the vostok-libs Nix package (VOSTOK_LIBS_DIR inside
# `nix develop`), falling back to a sibling checkout for standalone use.
LIBS_DIR    = Path(os.environ.get("VOSTOK_LIBS_DIR", str(REPO.parent / "vostok-libs")))

SRC         = LIBS_DIR   / "sources"
DEST        = PREBUILT

# `COPYING.LIB` is LGPL license TEXT (cell SDK), not a binary blob - the `*.lib`
# ext glob catches it. It is committed as normal source under sources/, so we
# SKIP it here rather than staging a copy (no special destination needed).
LICENSE_NAMES  = {"COPYING.LIB"}

# Our from-source 4.2.22 GFx suite (built per the shipped PDB's recipe - non-/GL,
# /Ox, pristine SDK; see build_gfx_suite.py) ships inside vostok-libs at the shipped
# Win32 Shipping config path. Remap it onto the game's binaries.prebuilt layout
# (`Win32/libraries/shipping/`), where the exe's `#pragma comment(lib,"libgfx.lib")`
# resolves it. The foreign 4.0.15 GFx libs were removed from vostok-libs and replaced
# by these - no skip/clobber dance needed.
GFX_SRC = Path("scaleform/Lib/Win32/Msvc90/Shipping")
GFX_DST = Path("Win32/libraries/shipping")


def dest_for(rel_path: Path, dest: Path) -> Path:
    """Resolve the absolute target path for a source-relative blob.

    The GFx Shipping libs remap onto the game's `Win32/libraries/shipping/` layout;
    everything else keeps its source-relative subpath under `dest`.
    """
    if rel_path.parent == GFX_SRC:
        return dest / GFX_DST / rel_path.name
    return dest / rel_path


def human_size(n: int) -> str:
    size = float(n)
    for unit in ("B", "KiB", "MiB", "GiB", "TiB"):
        if size < 1024 or unit == "TiB":
            return f"{size:.1f} {unit}" if unit != "B" else f"{int(size)} B"
        size /= 1024


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("src", nargs="?", type=Path, default=SRC)
    parser.add_argument("dest", nargs="?", type=Path, default=DEST)
    parser.add_argument("--reverse", action="store_true", help="Swap src and dest")
    parser.add_argument("-v", "--verbose", action="store_true",
                        help="Print every copied file (default: summary only)")
    args = parser.parse_args()

    src = args.src.resolve()
    dest = args.dest.resolve()
    if args.reverse:
        src, dest = dest, src

    copied = 0
    total_bytes = 0

    for file in src.rglob("*"):
        # License text (COPYING.LIB) is committed source, not a staged blob - skip it.
        if file.is_file() and file.suffix.lower() in EXTS and file.name not in LICENSE_NAMES:
            rel_path = file.relative_to(src)
            target = dest_for(rel_path, dest)
            target.parent.mkdir(parents=True, exist_ok=True)

            # Prior copies came from /nix/store (read-only). Atomically remove any
            # existing one (unlink needs write on the dir, not the file), then copy
            # contents only - copyfile gives the new file a default writable mode,
            # unlike copy2 which would preserve the read-only source mode.
            target.unlink(missing_ok=True)
            shutil.copyfile(file, target)
            if args.verbose:
                print(f"COPIED: {file} -> {target}")

            copied += 1
            total_bytes += file.stat().st_size

    print(f"Copied {copied} files ({human_size(total_bytes)}) -> {dest}")

if __name__ == "__main__":
    main()
