"""
generate_objdiff_config.py - Generate config consumed by `objdiff` for binary matching.
"""

import argparse
import json
import struct

from pathlib import Path
from typing import Optional


SCRIPT_DIR = Path(__file__).resolve().parent
ROOT_DIR   = (SCRIPT_DIR / ".." / "..").resolve()

VOSTOK_DIR  = ROOT_DIR / "vostok"
OBJDIFF_DIR = ROOT_DIR / "vostok" / "binaries" / "objdiff"


def write_dummy(path: Path):
    symbol_table_offset = 20 + 40  # header + 1 section header

    # COFF File Header (20 bytes)
    header = struct.pack("<HHIIIHH",
        0x14c,  # Machine: IMAGE_FILE_MACHINE_I386
        1,      # NumberOfSections
        0,      # TimeDateStamp
        symbol_table_offset,  # PointerToSymbolTable
        0,      # NumberOfSymbols
        0,      # SizeOfOptionalHeader
        0,      # Characteristics
    )

    # Section Header (40 bytes)
    section = struct.pack("<8sIIIIIIHHI",
        b".text\0\0\0",  # Name
        0,      # VirtualSize
        0,      # VirtualAddress
        0,      # SizeOfRawData
        0,      # PointerToRawData
        0,      # PointerToRelocations
        0,      # PointerToLineNumbers
        0,      # NumberOfRelocations
        0,      # NumberOfLineNumbers
        0x60000020,  # Characteristics: CODE | EXECUTE | READ
    )

    # String table: 4 bytes indicating size of 4 (empty)
    string_table = struct.pack("<I", 4)

    with open(path, "wb") as f:
        f.write(header)
        f.write(section)
        f.write(string_table)


def gather_units(
    skip_missing: bool,
    objdiff_dir: Path,
    filter_prefix: Optional[str]
):
    units = []
    for file in sorted((objdiff_dir / "target").rglob("*.obj")):
        if not file.is_file():
            continue

        try:
            size = file.stat().st_size
        except OSError:
            continue

        unit = file.as_posix().removeprefix(objdiff_dir.as_posix()).removeprefix("/target/").removesuffix(".obj")

        if filter_prefix and filter_prefix not in unit:
            continue

        target_path = f"./target/{unit}.obj"
        base_path   = f"./base/{unit}.obj"

        if skip_missing and not (objdiff_dir / base_path).exists():
            base_path = f"./dummy.obj"

        units.append({
            "name": unit,
            "target_path": target_path,
            "base_path": base_path,
            "scratch": {
                "platform": "win32",
                "compiler": "msvc8.0",
            },
        })

    return units


def main(
    skip_missing: bool,
    objdiff_dir: Path,
    filter_prefix: Optional[str]
):
    obj = {
        "build_base": False,
        "build_target": False,
        "units": gather_units(skip_missing, objdiff_dir, filter_prefix),
    }

    write_dummy(objdiff_dir / "dummy.obj");

    with (objdiff_dir / "objdiff.json").open("w", encoding="utf-8") as f:
        json.dump(obj, f, indent=2)
        f.write("\n")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("filter_prefix", nargs="?", default=None)
    parser.add_argument("objdiff_dir", nargs="?", default=OBJDIFF_DIR, type=Path)
    args = parser.parse_args()

    filter_prefix = args.filter_prefix
    if filter_prefix:
        filter_prefix = filter_prefix.replace("::", "/")


    main(
        skip_missing=True,
        objdiff_dir=args.objdiff_dir,
        filter_prefix=filter_prefix
    )
