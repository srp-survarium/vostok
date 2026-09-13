# SPDX-License-Identifier: GPL-3.0-or-later

"""Audit class declaration qualifiers and order against the retail PDB.

The raw evidence owner is ``pdb_topology --classes --json``. This wrapper turns
its whole-PDB class model into a module-scoped, checkable report for properties
that layout-only tools miss:

* public/protected/private access for bases, fields, and methods;
* static versus instance fields and methods;
* virtual/pure/introducing status;
* declaration order.

Examples::

    python3 -m vostok diff declarations --module network --module network_core
    python3 -m vostok diff declarations --module network_core --check
    python3 -m vostok diff declarations --module network_core --json

``--check`` exits 1 while any requested module has a qualifier or order error.
Competing target variants are preserved in the values printed for a finding;
the command never chooses one same-name PDB record as canonical.
"""

from __future__ import annotations

import argparse
from collections import defaultdict
from dataclasses import asdict, dataclass
import json
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile

from vostok.core import log as _log
from vostok.core import paths


MODULE_PREFIXES = {
    "network": ("vostok::network::",),
    "network_core": ("vostok::network_core::",),
    "logging": ("vostok::logging::",),
    "sound": ("vostok::sound::",),
    "vfs": ("vostok::vfs::",),
    "fs": ("vostok::fs::", "vostok::fs_new::"),
    "ai": ("vostok::ai::",),
    "ai_navigation": ("vostok::ai::navigation::",),
    "animation": ("vostok::animation::",),
    "render": ("vostok::render::",),
    "core": ("vostok::core::",),
    "engine": ("vostok::engine::",),
    "particle": ("vostok::particle::",),
    "input": ("vostok::input::",),
    "ui": ("vostok::ui::",),
    "physics": ("vostok::physics::",),
    "collision": ("vostok::collision::",),
    "survarium": ("survarium::",),
}

# These helper classes deliberately live at global scope because their names are
# part of the retail mangling.  Namespace-only bucketing would otherwise omit
# declarations owned by network_core translation units from the module gate.
SOURCE_OWNED_GLOBAL_CLASSES = {
    "network_core": {
        "delayed_packets_predicate",
        "packets_in_list_predicate",
        "packets_predicate",
        "remove_all_predicate",
        "sequence_id_predicate",
    },
}

_QUALIFIER_ATTRIBUTES = {
    "static",
    "virtual",
    "pure",
    "pure-virtual",
    "introducing",
    "introducing-virtual",
    "sealed",
}


@dataclass(frozen=True)
class Finding:
    module: str
    class_name: str
    member_kind: str
    member: str
    signature: str
    category: str
    target: tuple[str, ...]
    base: tuple[str, ...]
    class_status: str


def _module_of(name: str, modules: tuple[str, ...]) -> str | None:
    """Return the most-specific requested namespace owner for ``name``."""
    matches = [
        (len(prefix), module)
        for module in modules
        for prefix in MODULE_PREFIXES[module]
        if name.startswith(prefix)
    ]
    if matches:
        return max(matches)[1]
    return next(
        (
            module
            for module in modules
            if name in SOURCE_OWNED_GLOBAL_CLASSES.get(module, set())
        ),
        None,
    )


def _kind_group(declaration: dict) -> str | None:
    kind = declaration.get("kind") or ""
    if kind == "method":
        return "method"
    if kind in ("field", "static-field"):
        return "field"
    if kind == "base":
        return "base"
    return None


def _signature(declaration: dict) -> str:
    type_name = declaration.get("type_name") or ""
    if type_name.startswith("static "):
        type_name = type_name[len("static "):]
    return " ".join(type_name.split())


def _key(declaration: dict) -> tuple[str, str, str] | None:
    kind = _kind_group(declaration)
    if kind is None:
        return None
    return kind, declaration.get("name") or "<unnamed>", _signature(declaration)


def _modifiers(declaration: dict) -> str:
    attributes = set(declaration.get("attributes") or ())
    kind = declaration.get("kind") or ""
    is_static = kind == "static-field" or "static" in attributes
    qualifiers = sorted(attributes & _QUALIFIER_ATTRIBUTES)
    if is_static and "static" not in qualifiers:
        qualifiers.insert(0, "static")
    access = declaration.get("access") or "unspecified"
    suffix = ",".join(qualifiers) if qualifiers else "instance"
    return f"{access} {suffix}"


def _variant_declarations(variants: list[dict]) -> dict[tuple[str, str, str], set[str]]:
    result: dict[tuple[str, str, str], set[str]] = defaultdict(set)
    for variant in variants:
        for declaration in variant.get("declarations") or ():
            key = _key(declaration)
            if key is not None:
                result[key].add(_modifiers(declaration))
    return result


def _semantic_method_qualifiers(value: object) -> frozenset[str]:
    text = str(value).lower().replace("-", " ")
    return frozenset(
        qualifier
        for qualifier in ("static", "virtual", "pure", "introducing", "sealed")
        if qualifier in text
    )


def collect(payload: dict, modules: tuple[str, ...]) -> tuple[list[Finding], dict]:
    """Collect qualifier and order findings from a ``pdb_topology`` payload."""
    findings: list[Finding] = []
    stats = {
        module: {
            "classes": 0,
            "nonidentical_classes": 0,
            "variant_ambiguous_classes": 0,
            "qualifier_errors": 0,
            "order_errors": 0,
        }
        for module in modules
    }

    for class_record in payload.get("classes") or ():
        class_name = class_record.get("name") or ""
        module = _module_of(class_name, modules)
        if module is None:
            continue
        stats[module]["classes"] += 1
        status = class_record.get("status") or "unknown"
        if status not in ("identical", "record-multiplicity"):
            stats[module]["nonidentical_classes"] += 1
        if status in ("variant-overlap", "different") and (
            len(class_record.get("target_variants") or ()) > 1
            or len(class_record.get("base_variants") or ()) > 1
        ):
            stats[module]["variant_ambiguous_classes"] += 1

        target = _variant_declarations(class_record.get("target_variants") or [])
        base = _variant_declarations(class_record.get("base_variants") or [])
        qualifier_members: set[tuple[str, str, str]] = set()
        for key in sorted(set(target) & set(base)):
            target_values = tuple(sorted(target[key]))
            base_values = tuple(sorted(base[key]))
            if target_values == base_values:
                continue
            kind, member, signature = key
            findings.append(Finding(
                module=module,
                class_name=class_name,
                member_kind=kind,
                member=member,
                signature=signature,
                category="qualifiers",
                target=target_values,
                base=base_values,
                class_status=status,
            ))
            qualifier_members.add(key)
            stats[module]["qualifier_errors"] += 1

        differences = class_record.get("differences") or ()
        direct_qualifier_categories = {
            "inheritance-access": "base",
            "inheritance-presence": "base",
            "field-visibility": "field",
            "method-visibility": "method",
            "method-qualifiers": "method",
            "method-vtable-slot": "method",
        }
        for difference in differences:
            category = difference.get("category")
            if category not in direct_qualifier_categories:
                continue
            if category == "method-qualifiers" and (
                _semantic_method_qualifiers(difference.get("target"))
                == _semantic_method_qualifiers(difference.get("base"))
            ):
                continue
            member_kind = direct_qualifier_categories[category]
            member = difference.get("member") or "<unnamed>"
            if any(key[0] == member_kind and key[1] == member for key in qualifier_members):
                continue
            findings.append(Finding(
                module=module,
                class_name=class_name,
                member_kind=member_kind,
                member=member,
                signature="",
                category="qualifiers",
                target=(str(difference.get("target")),),
                base=(str(difference.get("base")),),
                class_status=status,
            ))
            qualifier_members.add((member_kind, member, ""))
            stats[module]["qualifier_errors"] += 1

        field_presence: dict[str, dict[str, dict]] = defaultdict(dict)
        for difference in differences:
            category = difference.get("category")
            if category in ("field-presence", "static-field-presence"):
                field_presence[difference.get("member") or "<unnamed>"][category] = difference
        for member, pair in field_presence.items():
            if set(pair) != {"field-presence", "static-field-presence"}:
                continue
            target_static = pair["static-field-presence"].get("target") is not None
            findings.append(Finding(
                module=module,
                class_name=class_name,
                member_kind="field",
                member=member,
                signature="",
                category="qualifiers",
                target=("static" if target_static else "instance",),
                base=("instance" if target_static else "static",),
                class_status=status,
            ))
            stats[module]["qualifier_errors"] += 1

        for difference in differences:
            if difference.get("category") != "declaration-order":
                continue
            findings.append(Finding(
                module=module,
                class_name=class_name,
                member_kind="declaration",
                member=difference.get("member") or "<unnamed>",
                signature="",
                category="order",
                target=(str(difference.get("target")),),
                base=(str(difference.get("base")),),
                class_status=status,
            ))
            stats[module]["order_errors"] += 1

    findings.sort(key=lambda row: (
        modules.index(row.module), row.class_name, row.category,
        row.member_kind, row.member, row.signature,
    ))
    return findings, stats


def _load_topology(path: Path | None) -> dict:
    if path is not None:
        return json.loads(path.read_text(encoding="utf-8"))

    executable = shutil.which("pdb_topology")
    if executable is None:
        raise RuntimeError("pdb_topology is not available; enter `nix develop`")
    target_pdb = paths.survarium_bin() / "survarium.pdb"
    for artifact in (target_pdb, paths.BASE_PDB):
        if not artifact.is_file():
            raise RuntimeError(f"missing PDB: {artifact}; run a full vostok build")

    with tempfile.TemporaryFile(mode="w+b") as output:
        subprocess.run(
            [
                executable,
                "--target-pdb", str(target_pdb),
                "--base-pdb", str(paths.BASE_PDB),
                "--classes",
                "--json",
            ],
            check=True,
            stdout=output,
        )
        output.seek(0)
        return json.load(output)


def _print_text(findings: list[Finding], stats: dict, modules: tuple[str, ...]) -> None:
    print("DECLARATION QUALIFIER/ORDER AUDIT (retail target -> reconstructed base)")
    for module in modules:
        current = stats[module]
        print(
            f"[{module}] classes={current['classes']} "
            f"nonidentical={current['nonidentical_classes']} "
            f"variant-ambiguous={current['variant_ambiguous_classes']} "
            f"qualifier-errors={current['qualifier_errors']} "
            f"order-errors={current['order_errors']}"
        )
        for row in (item for item in findings if item.module == module):
            if row.category == "qualifiers":
                label = f"{row.member_kind} {row.member}"
                if row.signature:
                    label += f" :: {row.signature}"
                print(f"  QUALIFIERS {row.class_name} :: {label}")
            else:
                print(f"  ORDER {row.class_name} :: {row.member}")
            print(f"    target: {' | '.join(row.target)}")
            print(f"    base:   {' | '.join(row.base)}")
            if row.class_status in ("variant-overlap", "different"):
                print(f"    variants: {row.class_status}; values above preserve all records")


def main() -> int:
    parser = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter
    )
    parser.add_argument(
        "--module", action="append", required=True, choices=sorted(MODULE_PREFIXES),
        help="namespace-owned module to audit; repeat for multiple modules",
    )
    parser.add_argument("--json", action="store_true", help="emit machine-readable JSON")
    parser.add_argument(
        "--check", action="store_true",
        help="exit 1 when qualifier or declaration-order findings remain",
    )
    parser.add_argument(
        "--topology-json", type=Path, metavar="PATH",
        help=argparse.SUPPRESS,
    )
    args = parser.parse_args()

    modules = tuple(dict.fromkeys(args.module))
    try:
        payload = _load_topology(args.topology_json)
    except (OSError, ValueError, subprocess.CalledProcessError, RuntimeError) as error:
        print(f"vostok diff declarations: {error}", file=sys.stderr)
        return 2

    findings, stats = collect(payload, modules)
    if args.json:
        json.dump(
            {
                "modules": list(modules),
                "stats": stats,
                "findings": [asdict(row) for row in findings],
            },
            sys.stdout,
            indent=2,
        )
        sys.stdout.write("\n")
    else:
        _print_text(findings, stats, modules)
    return 1 if args.check and findings else 0


if __name__ == "__main__":
    raise SystemExit(_log.run("vostok.diff.declarations", main))
