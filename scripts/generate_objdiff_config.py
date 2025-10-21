"""
generate_objdiff_config.py - Generate config consumed by `objdiff` for binary matching.
"""

from pathlib import Path
from typing import Optional
import json

SCRIPT_DIR = Path(__file__).resolve().parent
ROOT_DIR   = (SCRIPT_DIR / ".." / "..").resolve()

VOSTOK_DIR = ROOT_DIR / "vostok"
COFF_DIR   = ROOT_DIR / "vostok-coff-delinker"

exclude_prefixes = [
    "buffer_vector",
    "enum_flags",
    "fixed_string",
    "fixed_vector",
    "flags_type",
    "horrible_cast",
    "intrusive_double_linked_list",
    "intrusive_list",
    "intrusive_mpmc_stack",
    "intrusive_mpsc_queue",
    "intrusive_ptr",
    "quasi_singleton",
    "threads_channel_query_base",
    "threads_channel_query_base_helper",
    "uid_object",
    "uninitialized_reference",
    "vectora",
    "vectora_allocator",
]

####################

def should_exclude(unit: str) -> bool:
    return any(unit.startswith(f"vostok/{p}[") for p in exclude_prefixes)

def gather_units(skip_missing: bool, filter_prefix: Optional[str] = None):
    units = []
    for file in sorted((COFF_DIR / "target").rglob("*.obj")):
        if not file.is_file():
            continue

        try:
            size = file.stat().st_size
        except OSError:
            continue

        if size < 400: # (bytes) No functions in the object file
            continue

        unit = file.as_posix().removeprefix(COFF_DIR.as_posix()).removeprefix("/target/").removesuffix(".obj")

        if should_exclude(unit):
            continue

        if filter_prefix and filter_prefix not in unit:
            continue

        target_path = f"./target/{unit}.obj"
        base_path   = f"./base/{unit}.obj"

        if skip_missing and not (COFF_DIR / base_path).exists():
            base_path = f"./base/dummy.obj"

        units.append({
            "name": unit,
            "target_path": target_path,
            "base_path": base_path
        })

    return units

####################

def main(skip_missing: bool, filter_prefix: Optional[str]):
    obj = {
        "build_base": False,
        "build_target": False,
        "units": gather_units(skip_missing, filter_prefix),
    }

    with (COFF_DIR / "objdiff.json").open("w", encoding="utf-8") as f:
        json.dump(obj, f, indent=2)
        f.write("\n")

if __name__ == "__main__":
    # main(skip_missing=True, filter_prefix="vostok/network_core")
    main(skip_missing=True, filter_prefix=None)
