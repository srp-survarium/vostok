#!/usr/bin/env python3
"""Normalize project-specific PDB symbol names in objdiff target objects.

vostok-delinker deliberately emits PDB procedure names verbatim.  The retail
PDB spells static initializer/finalizer thunks as readable backtick names,
while MSVC 8 emits decorated ``??__E``/``??__F`` names in candidate COFF
objects.  A few template instantiations also differ only in PDB type-rendering
spelling.  objdiff pairs functions by COFF symbol identity, so normalize the
safe, fully-qualified retail names and exact known aliases to the compiler-side
spelling in the disposable target comparison objects.

This is intentionally a project-side comparison transform: the delinker's raw
output remains generic, the source PDB identity remains available in the rich
index, and local/anonymous/template scopes that cannot be inverted exactly are
left untouched.
"""

from __future__ import annotations

import argparse
import json
import re
import shutil
import subprocess
import sys
from collections import defaultdict
from pathlib import Path


_DYNAMIC_RE = re.compile(
    r"^(?P<prefix>(?:[A-Za-z_][A-Za-z0-9_]*::)*)"
    r"`dynamic (?P<kind>initializer|atexit destructor) for "
    r"'(?P<inner>[^']+)''$"
)
_IDENT_RE = re.compile(r"^[A-Za-z_][A-Za-z0-9_]*$")
_NM_RE = re.compile(
    r"^\s*(?:[0-9A-Fa-f]+\s+)?[A-Za-z?]\s+(?P<name>.+)$"
)
_EXACT_COMPILER_NAMES = {
    "vostok::sound::closest_point_on_segment":
        "?closest_point_on_segment@sound@vostok@@YA?AVfloat3@math@2@ABV342@00@Z",
    "vostok::sound::compare_propagator_info_by_distance":
        "?compare_propagator_info_by_distance@sound@vostok@@YA_NABUpropagator_info@12@0@Z",
    (
        "vostok::render::create_shadow_pass_geometry_type<"
        "`vostok::render::static_render_surface::create_shadow_pass_geometry'::"
        "`2'::colored_static_vertex,"
        "`vostok::render::static_render_surface::create_shadow_pass_geometry'::"
        "`3'::colored_opt_static_vertex>"
    ): (
        "vostok::render::create_shadow_pass_geometry_type<"
        "`vostok::render::static_render_surface::create_shadow_pass_geometry'::"
        "`2'::colored_static_vertex,"
        "`vostok::render::static_render_surface::colored_opt_static_vertex'::"
        "`3'::colored_opt_static_vertex>"
    ),
    (
        "vostok::render::create_shadow_pass_geometry_type<"
        "`vostok::render::static_render_surface::create_shadow_pass_geometry'::"
        "`2'::static_vertex0,"
        "`vostok::render::static_render_surface::create_shadow_pass_geometry'::"
        "`2'::opt_static_vertex>"
    ): (
        "vostok::render::create_shadow_pass_geometry_type<"
        "`vostok::render::static_render_surface::create_shadow_pass_geometry'::"
        "`2'::static_vertex0,"
        "`vostok::render::static_render_surface::opt_static_vertex'::"
        "`2'::opt_static_vertex>"
    ),
}
_SOUND_COOK_LOCAL_ATEXIT_PREFIX = (
    "`vostok::sound::sound_world::register_sound_cooks'::`2'::"
    "`dynamic atexit destructor for '"
)
_SOUND_COOK_LOCAL_ATEXIT_SUFFIX = "''"
_SOUND_COOK_LOCALS = {
    "s_composite_sound_cook",
    "s_encoded_sound_with_qualities_cook",
    "s_ogg_encoded_sound_interface_cook",
    "s_ogg_file_contents_cook",
    "s_ogg_sound_cook",
    "s_ogg_source_cook",
    "s_panning_lut_cook",
    "s_single_sound_cook",
    "s_sound_cll_cook",
    "s_sound_environment_cook",
    "s_sound_rms_cook",
    "s_sound_scene_cook",
    "s_sound_spl_cook",
    "s_wav_encoded_sound_interface_cook",
}

_PDB_ALIAS_TO_COMPILER_NAME = {
    "??$propagate@Vn_ary_tree_multiplication_node@mixing@animation@vostok@@@n_ary_tree_size_calculator@mixing@animation@vostok@@AAEXAAVn_ary_tree_multiplication_node@123@@Z":
        "??$propagate@Vn_ary_tree_addition_node@mixing@animation@vostok@@@n_ary_tree_size_calculator@mixing@animation@vostok@@AAEXAAVn_ary_tree_addition_node@123@@Z",
    "load_function<void __stdcall(enum BUGTRAP_ACTIVITY_tag)>":
        "load_function<void __stdcall(BUGTRAP_ACTIVITY_tag)>",
    "load_function<void __stdcall(enum BUGTRAP_DIALOGMESSAGE_tag,char const *)>":
        "load_function<void __stdcall(BUGTRAP_DIALOGMESSAGE_tag,char const *)>",
    "load_function<void __stdcall(enum BUGTRAP_REPORTFORMAT_tag)>":
        "load_function<void __stdcall(BUGTRAP_REPORTFORMAT_tag)>",
    "load_function<long (__stdcall*__stdcall(void))(_EXCEPTION_POINTERS *)>":
        "load_function<long (__stdcall*)(_EXCEPTION_POINTERS *) __stdcall(void)>",
}


def pdb_alias_name(pdb_name: str) -> str | None:
    """Return a compiler-side name for an exact, audited PDB spelling alias."""
    return _PDB_ALIAS_TO_COMPILER_NAME.get(pdb_name)


def compiler_name(pdb_name: str) -> str | None:
    """Return the compiler-side spelling when the mapping is exact."""
    alias = pdb_alias_name(pdb_name)
    if alias:
        return alias
    exact = _EXACT_COMPILER_NAMES.get(pdb_name)
    if exact:
        return exact

    if (
        pdb_name.startswith(_SOUND_COOK_LOCAL_ATEXIT_PREFIX)
        and pdb_name.endswith(_SOUND_COOK_LOCAL_ATEXIT_SUFFIX)
    ):
        variable = pdb_name[
            len(_SOUND_COOK_LOCAL_ATEXIT_PREFIX) : -len(_SOUND_COOK_LOCAL_ATEXIT_SUFFIX)
        ]
        if variable in _SOUND_COOK_LOCALS:
            return (
                f"??__F{variable}@?1??register_sound_cooks@"
                "sound_world@sound@vostok@@AAEXXZ@YAXXZ"
            )

    match = _DYNAMIC_RE.match(pdb_name)
    if not match:
        return None

    prefix = match.group("prefix").removesuffix("::")
    inner = match.group("inner")
    if "::" in inner:
        if prefix:
            return None
        parts = inner.split("::")
    else:
        parts = ([*prefix.split("::"), inner] if prefix else [inner])
    if not parts or any(not _IDENT_RE.fullmatch(part) for part in parts):
        return None

    variable, scopes = parts[-1], parts[:-1]
    kind = "E" if match.group("kind") == "initializer" else "F"
    scope_suffix = "".join(f"@{scope}" for scope in reversed(scopes))
    return f"??__{kind}{variable}{scope_suffix}@@YAXXZ"


def object_symbols(nm: str, obj: Path) -> set[str]:
    result = subprocess.run(
        [nm, str(obj)],
        check=True,
        capture_output=True,
        text=True,
    )
    names = set()
    for line in result.stdout.splitlines():
        match = _NM_RE.match(line)
        if match:
            names.add(match.group("name"))
    return names


def rich_pdb_aliases(
    target_index: Path,
    base_index: Path,
    *,
    source_prefix: str | None = None,
) -> dict[str, str]:
    """Derive unambiguous retail-placeholder -> compiler symbol aliases.

    Retail PDBs sometimes expose a readable template name as the COFF symbol,
    while the candidate PDB exposes the actual MSVC decoration.  A complete
    demangled signature plus the same source owner is an exact identity.  Only
    map a readable target spelling to one decorated base spelling when that
    identity and both spellings are unique on their respective sides.
    """

    def records_by_identity(path: Path) -> dict[tuple[str, str], set[str]]:
        records: dict[tuple[str, str], set[str]] = defaultdict(set)
        with path.open(encoding="utf-8") as source:
            for line in source:
                record = json.loads(line)
                file = record.get("file")
                name = record.get("name")
                mangled = record.get("mangled")
                if file and name and mangled:
                    records[(file, name)].add(mangled)
        return records

    target = records_by_identity(target_index)
    base = records_by_identity(base_index)
    candidates: dict[str, set[str]] = defaultdict(set)
    for identity in target.keys() & base.keys():
        if source_prefix and not identity[0].startswith(source_prefix):
            continue
        target_names = target[identity]
        base_names = base[identity]
        if len(target_names) != 1 or len(base_names) != 1:
            continue
        target_name = next(iter(target_names))
        base_name = next(iter(base_names))
        if (
            target_name != base_name
            and not target_name.startswith("?")
            and base_name.startswith("?")
        ):
            candidates[target_name].add(base_name)

    unique = {
        target_name: next(iter(base_names))
        for target_name, base_names in candidates.items()
        if len(base_names) == 1
    }
    target_names_by_base: dict[str, set[str]] = defaultdict(set)
    for target_name, base_name in unique.items():
        target_names_by_base[base_name].add(target_name)
    return {
        target_name: base_name
        for target_name, base_name in unique.items()
        if len(target_names_by_base[base_name]) == 1
    }


def normalize_tree(
    root: Path,
    *,
    nm: str,
    objcopy: str,
    aliases: dict[str, str] | None = None,
) -> tuple[int, int]:
    """Normalize target objects atomically; return (objects, renamed symbols)."""
    aliases = aliases or {}
    mappings: dict[str, str] = {}
    symbols_by_object: dict[Path, set[str]] = {}
    for obj in sorted(root.rglob("*.obj")):
        names = object_symbols(nm, obj)
        symbols_by_object[obj] = names
        for name in names:
            replacement = aliases.get(name) or compiler_name(name)
            if replacement:
                previous = mappings.setdefault(name, replacement)
                if previous != replacement:
                    raise RuntimeError(
                        f"ambiguous normalization for {name!r}: "
                        f"{previous!r} vs {replacement!r}"
                    )

    changed_objects = 0
    renamed_symbols = 0
    for obj, names in symbols_by_object.items():
        local = sorted((old, mappings[old]) for old in names if old in mappings)
        if not local:
            continue
        tmp = obj.with_name(f".{obj.name}.normalize.tmp")
        command = [objcopy]
        for old, new in local:
            command.extend(["--redefine-sym", f"{old}={new}"])
        command.extend([str(obj), str(tmp)])
        try:
            subprocess.run(command, check=True)
            tmp.replace(obj)
        finally:
            tmp.unlink(missing_ok=True)
        changed_objects += 1
        renamed_symbols += len(local)

    return changed_objects, renamed_symbols


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Normalize safe retail static-init thunk names for objdiff"
    )
    parser.add_argument("root", type=Path)
    args = parser.parse_args()

    nm = shutil.which("llvm-nm")
    objcopy = shutil.which("llvm-objcopy")
    if not nm or not objcopy:
        parser.error("llvm-nm and llvm-objcopy must be available on PATH")
    if not args.root.is_dir():
        parser.error(f"{args.root} is not a directory")

    objects, symbols = normalize_tree(args.root, nm=nm, objcopy=objcopy)
    print(
        f"[symbol-normalize] normalized {symbols} symbols in "
        f"{objects} target objects"
    )
    return 0


if __name__ == "__main__":
    sys.exit(main())
