# SPDX-License-Identifier: GPL-3.0-or-later
"""Extract a release archive for setup.ps1 -Native (Windows' bsdtar has no xz support).

usage: python unpack.py <archive.tar.xz|archive.zip> <dest> [strip-components]
"""
import sys
import tarfile
import zipfile
from pathlib import Path


def main() -> None:
    src, dest = Path(sys.argv[1]), Path(sys.argv[2])
    strip = int(sys.argv[3]) if len(sys.argv) > 3 else 0
    dest.mkdir(parents=True, exist_ok=True)
    if src.suffix == ".zip":
        with zipfile.ZipFile(src) as z:
            for info in z.infolist():
                parts = Path(info.filename).parts[strip:]
                if not parts:
                    continue
                out = dest.joinpath(*parts)
                if info.is_dir():
                    out.mkdir(parents=True, exist_ok=True)
                    continue
                out.parent.mkdir(parents=True, exist_ok=True)
                with z.open(info) as source, open(out, "wb") as sink:
                    while chunk := source.read(1 << 20):
                        sink.write(chunk)
        return
    with tarfile.open(src) as t:
        members = []
        for m in t.getmembers():
            parts = Path(m.name).parts[strip:]
            if parts:
                m.name = str(Path(*parts))
                members.append(m)
        t.extractall(dest, members=members, filter="data")


if __name__ == "__main__":
    main()
