#!/usr/bin/env python3

import argparse
import shutil
from pathlib import Path

EXTS = {'.pdb', '.exe', '.dll', '.a', '.lib'}

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("src", type=Path)
    parser.add_argument("dest", type=Path)
    parser.add_argument("--combine", action="store_true")
    args = parser.parse_args()

    src = args.src.resolve()
    dest = args.dest.resolve()
    copied = 0

    if args.combine:
        src, dest = dest, src

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
