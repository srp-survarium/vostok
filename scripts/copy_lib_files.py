"""
copy_lib_files.py - Copy 3rd party libraries ('.dll's and '.lib's) into the project.
"""

import argparse
import shutil
from pathlib import Path

EXTS = {'.pdb', '.exe', '.dll', '.a', '.lib'}

SCRIPT_DIR = Path(__file__).resolve().parent
ROOT_DIR   = (SCRIPT_DIR / ".." / "..").resolve()

VOSTOK_DIR = ROOT_DIR / "vostok"
LIBS_DIR   = ROOT_DIR / "vostok-libs"

SRC        = VOSTOK_DIR / "sources"
DEST       = LIBS_DIR   / "sources"

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("src", nargs="?", type=Path, default=SRC)
    parser.add_argument("dest", nargs="?", type=Path, default=DEST)
    args = parser.parse_args()

    src = args.src.resolve()
    dest = args.dest.resolve()
    copied = 0

    for file in src.rglob("*"):
        if file.is_file() and file.suffix.lower() in EXTS:
            rel_path = file.relative_to(src)
            target = dest / rel_path
            target.parent.mkdir(parents=True, exist_ok=True)

            shutil.copy2(file, target)
            print(f"COPIED: {file} -> {target}")

            copied += 1

    print(f"Total files copied: {copied}")

if __name__ == "__main__":
    main()
