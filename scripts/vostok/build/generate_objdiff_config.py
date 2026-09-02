# SPDX-License-Identifier: GPL-3.0-or-later

"""
vostok.build.generate_objdiff_config - generate the config `objdiff` consumes.
"""

import argparse
import json
import struct

from pathlib import Path

from vostok.core.paths import OBJDIFF_DIR


VOSTOK_PRESET = [
    "vostok/collision",
    "vostok/game",
    "vostok/game_core",
    "vostok/logging",
    "vostok/network",
    "vostok/network_core",
    "vostok/physics",
]


def find_base_path(objdiff_dir: Path, unit: str) -> str:
    direct = objdiff_dir / "base" / f"{unit}.obj"
    if direct.is_file():
        return f"./base/{unit}.obj"

    parts = Path(unit).parts
    scope_parts = parts[:2] if parts and parts[0] == "vostok" else parts[:1]
    scope = objdiff_dir / "base" / Path(*scope_parts)
    if scope.is_dir():
        candidates = sorted(scope.rglob(f"{Path(unit).name}.obj"))
        if len(candidates) == 1:
            relative = candidates[0].relative_to(objdiff_dir).as_posix()
            return f"./{relative}"

    return "./dummy.obj"


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
    objdiff_dir: Path,
    use_preset_config: bool,
):
    units = []
    for file in sorted((objdiff_dir / "target").rglob("*.obj")):
        if not file.is_file():
            continue

        try:
            file.stat()  # guard: skip files that can't be stat'd
        except OSError:
            continue

        unit = file.as_posix().removeprefix(objdiff_dir.as_posix()).removeprefix("/target/").removesuffix(".obj")

        if use_preset_config and not any(path in unit for path in VOSTOK_PRESET):
            continue

        target_path = f"./target/{unit}.obj"
        base_path = find_base_path(objdiff_dir, unit)

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
    objdiff_dir: Path,
    use_preset_config: bool
):
    obj = {
        "build_base": False,
        "build_target": False,
        "units": gather_units(objdiff_dir, use_preset_config),
    }

    write_dummy(objdiff_dir / "dummy.obj")

    with (objdiff_dir / "objdiff.json").open("w", encoding="utf-8") as f:
        json.dump(obj, f, indent=2)
        f.write("\n")


def cli():
    """`-m vostok.build.generate_objdiff_config [--use-preset-config] [--objdiff-dir DIR]`."""
    parser = argparse.ArgumentParser()
    parser.add_argument("--use-preset-config", action="store_true", default=False)
    parser.add_argument("--objdiff-dir", default=OBJDIFF_DIR, type=Path)
    args = parser.parse_args()

    main(
        objdiff_dir=args.objdiff_dir,
        use_preset_config=args.use_preset_config,
    )


if __name__ == "__main__":
    cli()
