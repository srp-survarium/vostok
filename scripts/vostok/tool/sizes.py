# SPDX-License-Identifier: GPL-3.0-or-later

"""
count_sized.py - Display how much disk memory different extensions take in the project.
"""

from collections import defaultdict
from math import log
from pathlib import Path
import argparse
import os
import sys

from vostok.core.paths import SOURCES as SRC


####################


def humanize(n):
    if n < 1024:
        return f"{n}B"
    units = ["K","M","G","T","P","E","Z","Y"]
    i = int(log(n, 1024))
    i = min(i, len(units))
    v = n / (1024 ** i)
    return f"{v:.1f}{units[i-1]}"

def ext_of(name: str) -> str:
    # ".bashrc" => (none); ".env.local" => "local"; "archive.tar.gz" => "gz"
    if name.startswith(".") and name.count(".") == 1:
        return "(none)"
    return name.rsplit(".", 1)[1] if "." in name else "(none)"

root = sys.argv[1] if len(sys.argv) > 1 else "."
totals = defaultdict(int)


####################


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("src", nargs="?", type=Path, default=SRC)
    args = parser.parse_args()

    src = args.src.resolve()

    for dirpath, _, files in os.walk(src):
        for fn in files:
            path = os.path.join(dirpath, fn)
            try:
                st = os.stat(path, follow_symlinks=False)
            except OSError:
                continue
            totals[ext_of(fn)] += st.st_size

    # print sorted by size ascending
    for ext, size in sorted(totals.items(), key=lambda kv: kv[1]):
        print(f"{humanize(size)} {ext}")

if __name__ == "__main__":
    main()
