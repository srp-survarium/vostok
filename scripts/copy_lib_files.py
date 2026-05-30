"""
copy_lib_files.py - Copy 3rd party libraries ('.dll's and '.lib's) into the project.
"""

import argparse
import os
import shutil
import stat
from pathlib import Path

EXTS = {'.pdb', '.exe', '.dll', '.a', '.lib'}

SCRIPT_DIR = Path(__file__).resolve().parent
ROOT_DIR   = (SCRIPT_DIR / ".." / "..").resolve()

LIBS_DIR   = ROOT_DIR / "vostok-libs"
VOSTOK_DIR = ROOT_DIR / "vostok"

SRC         = LIBS_DIR   / "sources"
DEST        = VOSTOK_DIR / "sources"


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
        if file.is_file() and file.suffix.lower() in EXTS:
            rel_path = file.relative_to(src)
            target = dest / rel_path
            target.parent.mkdir(parents=True, exist_ok=True)

            # Sources may come from /nix/store (read-only); previous copies
            # inherit that mode, so re-runs would EACCES on shutil.copy2.
            if target.exists():
                target.chmod(target.stat().st_mode | stat.S_IWUSR)
                target.unlink()
            shutil.copy2(file, target)
            target.chmod(target.stat().st_mode | stat.S_IWUSR)
            if args.verbose:
                print(f"COPIED: {file} -> {target}")

            copied += 1
            total_bytes += file.stat().st_size

    print(f"Copied {copied} files ({human_size(total_bytes)}) -> {dest}")

if __name__ == "__main__":
    main()
