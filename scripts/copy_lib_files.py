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

EXTS = {'.pdb', '.exe', '.dll', '.a', '.lib'}

SCRIPT_DIR = Path(__file__).resolve().parent
VOSTOK_DIR = SCRIPT_DIR.parent

# Lib source defaults to the vostok-libs Nix package (VOSTOK_LIBS_DIR inside
# `nix develop`), falling back to a sibling checkout for standalone use.
LIBS_DIR    = Path(os.environ.get("VOSTOK_LIBS_DIR", str(VOSTOK_DIR.parent / "vostok-libs")))

SRC         = LIBS_DIR   / "sources"
DEST        = VOSTOK_DIR / "binaries.prebuilt"

# `COPYING.LIB` is LGPL license TEXT (cell SDK), not a binary blob - the `*.lib`
# ext glob catches it. It is committed as normal source under sources/, so we
# SKIP it here rather than staging a copy (no special destination needed).
LICENSE_NAMES  = {"COPYING.LIB"}

# The foreign GFx libs (libgfx*.lib under this Release dir) are NOT staged. We
# build our OWN from-source 4.2.22 GFx suite into `Win32/libraries/shipping/`
# (build_gfx_lib_direct.py) to byte-match the shipped 4.2.21 - the exe links them
# via `#pragma comment(lib,"libgfx.lib")` off that dir. Staging the 4.0.15
# distribution libs here would CLOBBER those builds on every setup (their ABI
# differs from 4.2.22, breaking the render-engine link), so skip everything under
# GFX_SRC.
GFX_SRC = Path("scaleform/Lib/Win32/Msvc90/Release")


def dest_for(rel_path: Path, dest: Path):
    """Resolve the absolute target path for a source-relative blob, or None to skip.

    Foreign GFx libs (under GFX_SRC) are skipped - the 4.2.22 suite is built from
    source into shipping/ separately. Everything else keeps its source-relative
    subpath under `dest` so nothing is lost.
    """
    if rel_path.parent == GFX_SRC:
        return None
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
            if target is None:          # foreign GFx lib - we build our own from source
                continue
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
