# SPDX-License-Identifier: GPL-3.0-or-later
"""Reviewed embedded table extents, audited without interpreting arbitrary code."""

from __future__ import annotations

import csv
import hashlib
import json
from collections import defaultdict

from vostok.core import paths
from vostok.derive import maxima

COLUMNS = ("unit", "function", "target_owner_rva", "kind", "operand_offset",
           "table_offset", "size", "evidence")
_CASE_OFFSETS = tuple(range(14, 62, 6))
_SURFACE_LOAD = (
    "vostok/render/engine/sources/render_model.cpp",
    "?load@render_surface@render@vostok@@UAEXABVbinary_config_value@configs@3@AAVchunk_reader@memory@3@@Z",
)
_SURFACE_CASE_OFFSETS = (336, 351, 366, 381, 396, 411, 418)


def _surface_profile(rows):
    return (rows[0]["unit"], rows[0]["function"]) == _SURFACE_LOAD


def _case_offsets(rows):
    return _SURFACE_CASE_OFFSETS if _surface_profile(rows) else _CASE_OFFSETS


def registrations():
    with paths.RETAIL_COMPILER_TABLES.open(newline="", encoding="utf-8") as source:
        reader = csv.DictReader(source, delimiter="\t")
        if tuple(reader.fieldnames or ()) != COLUMNS:
            raise ValueError("unexpected reviewed compiler-table columns")
        rows = list(reader)
    expected = {
        ("vostok/render/engine/sources/render_model_cooker.cpp",
         "vostok::render::mesh_type_to_vertex_input_type"),
        ("vostok/render/engine/sources/combined_model_cooker.cpp",
         "vostok::render::mesh_type_to_vertex_input_type"),
        _SURFACE_LOAD,
    }
    if len(rows) != 6 or {(row["unit"], row["function"]) for row in rows} != expected:
        raise ValueError("reviewed enrollment must retain both mesh owners and surface load")
    seen = set()
    for row in rows:
        identity = row["unit"], row["function"], row["kind"]
        if identity in seen or not row["evidence"]:
            raise ValueError("duplicate or unproven reviewed compiler table")
        seen.add(identity)
        for field in ("target_owner_rva", "operand_offset", "table_offset", "size"):
            row[field] = int(row[field], 0)
        if row["kind"] not in {"selector", "jump"}:
            raise ValueError("unsupported reviewed compiler-table kind")
    return rows


def _owner(records, rows, *, target):
    candidates = {
        (row["rva"], row["size"]): row for row in records
        if row["file"] == rows[0]["unit"] and row["mangled"] == rows[0]["function"]
        and (not target or row["rva"] == rows[0]["target_owner_rva"])
    }
    if len(candidates) != 1:
        raise ValueError(f"owner selection found {len(candidates)} physical bodies")
    return next(iter(candidates.values()))


def _validate_surface_load(image, owner, rows):
    """Only the observed member-store/RET8 surface-load profile, not arbitrary code."""
    start, size = owner["rva"], owner["size"]
    facts = {row["kind"]: row for row in rows}
    expected = {"selector": (325, 456, 48), "jump": (332, 428, 28)}
    if len(rows) != 2 or set(facts) != set(expected):
        raise ValueError("incomplete reviewed surface selector/jump pair")
    for kind, row in facts.items():
        if ((row["unit"], row["function"]) != _SURFACE_LOAD
                or row["target_owner_rva"] != 0x62C060
                or tuple(row[key] for key in ("operand_offset", "table_offset", "size"))
                != expected[kind]):
            raise ValueError("unsupported reviewed surface table geometry/identity")
    text = image.section_at(start)
    if size != 504 or text is None or text.name != ".text" or not text.contains(start, size):
        raise ValueError("reviewed surface procedure extent changed")
    raw = image.read_rva(start, size)
    if (raw[:9] != bytes.fromhex("83ec2456578b7c2430")
            or raw[14:16] != bytes.fromhex("8bf1")
            or raw[304:325] != bytes.fromhex("0fb7000fb7c0c746040000000083f82f77600fb688")
            or raw[329:332] != bytes.fromhex("ff248d")):
        raise ValueError("reviewed surface dispatch/unsigned default encoding changed")
    epilogue = bytes.fromhex("5f5e83c424c20800")
    for offset, value in zip(_SURFACE_CASE_OFFSETS[:-1], (6, 5, 4, 3, 1, 2)):
        if (raw[offset:offset + 3] != bytes.fromhex("c74604")
                or image.u32_rva(start + offset + 3) != value
                or raw[offset + 7:offset + 15] != epilogue):
            raise ValueError("reviewed surface member-store case/epilogue changed")
    if raw[418:426] != epilogue or raw[426:428] != bytes.fromhex("8bff"):
        raise ValueError("reviewed surface default/padding boundary changed")
    relocs = set(image.base_relocations())
    roots = {start + 325, start + 332} | set(range(start + 428, start + 456, 4))
    if {site for site in relocs if start + 304 <= site < start + 504} != roots:
        raise ValueError("reviewed surface dispatch/table relocation roots changed")
    for row in rows:
        if image.u32_rva(start + row["operand_offset"]) != image.image_base + start + row["table_offset"]:
            raise ValueError("reviewed surface operand/table destination changed")
    if max(raw[456:504]) >= 7:
        raise ValueError("surface selector leaves registered jump extent")
    for offset in range(428, 456, 4):
        if image.u32_rva(start + offset) - image.image_base - start not in _SURFACE_CASE_OFFSETS:
            raise ValueError("surface jump entry is not a reviewed case boundary")
    return raw


def _validate(image, owner, rows):
    if rows and _surface_profile(rows):
        return _validate_surface_load(image, owner, rows)
    return _validate_mesh(image, owner, rows)


def _validate_mesh(image, owner, rows):
    """Validate only the reviewed two-instruction MSVC mesh dispatch family."""
    start, size = owner["rva"], owner["size"]
    if {row["kind"] for row in rows} != {"selector", "jump"} or len(rows) != 2:
        raise ValueError("incomplete reviewed selector/jump pair")
    facts = {row["kind"]: row for row in rows}
    if any(row["target_owner_rva"] != rows[0]["target_owner_rva"] for row in rows):
        raise ValueError("conflicting registered owner RVAs")
    expected = {"selector": (3, 100, 201), "jump": (10, 64, 36)}
    for kind, row in facts.items():
        if tuple(row[key] for key in ("operand_offset", "table_offset", "size")) != expected[kind]:
            raise ValueError("unsupported reviewed mesh table geometry")
    text = image.section_at(start)
    if size != 301 or text is None or text.name != ".text" or not text.contains(start, size):
        raise ValueError("reviewed procedure extent changed")
    raw = image.read_rva(start, size)
    if raw[:3] != bytes.fromhex("0fb688") or raw[7:10] != bytes.fromhex("ff248d"):
        raise ValueError("reviewed dispatch operand encoding changed")
    if raw[62:64] != bytes.fromhex("8bff") or any(
        raw[offset] != 0xB8 or raw[offset + 5] != 0xC3 for offset in _CASE_OFFSETS
    ):
        raise ValueError("reviewed case-block boundaries changed")
    relocs = set(image.base_relocations())
    for row in rows:
        site = start + row["operand_offset"]
        table = start + row["table_offset"]
        if site not in relocs or image.u32_rva(site) != image.image_base + table:
            raise ValueError("table operand relocation/address changed")
        if not text.contains(table, row["size"]):
            raise ValueError("table extent leaves its section")
    # These explicit intervals avoid enrolling procedure bytes or disassembly artifacts.
    if facts["jump"]["table_offset"] + facts["jump"]["size"] != facts["selector"]["table_offset"]:
        raise ValueError("table extents overlap or leave an unreviewed gap")
    if max(raw[100:]) >= 9:
        raise ValueError("selector leaves the registered jump extent")
    for offset in range(64, 100, 4):
        value = image.u32_rva(start + offset)
        if value == 0:
            if start + offset in relocs:
                raise ValueError("null jump entry has a relocation")
        elif start + offset not in relocs or value - image.image_base - start not in _CASE_OFFSETS:
            raise ValueError("jump entry is not a relocated reviewed case boundary")
    if any(start + 100 <= site < start + 301 for site in relocs):
        raise ValueError("selector contains an unexpected relocation")
    return raw


def audit(module, pairs, target_image, base_image, ledger, *, facts=None):
    """Return existing audit and gate rows; table debt never borrows code scores."""
    if module != "render":
        return [], []
    from vostok.data import render_relocs as reloc

    groups = defaultdict(list)
    for row in registrations() if facts is None else facts:
        groups[(row["unit"], row["function"])].append(row)
    audit_rows, function_rows = [], []
    for (unit, function), rows in sorted(groups.items()):
        target_owner = _owner(pairs.target_records, rows, target=True)
        target_raw = _validate(target_image, target_owner, rows)
        error = ""
        base_owner = None
        try:
            base_owner = _owner(pairs.base_records, rows, target=False)
            base_raw = _validate(base_image, base_owner, rows)
        except ValueError as failure:
            error = str(failure)
            base_raw = b""
            base_owner = None
        identity = f"{function}@@compiler-tables:{unit}"

        class TableIndex(reloc.DatumIndex):
            def resolve_all(self, rva):
                if self.owner_record is not None:
                    offset = rva - self.owner_record["rva"]
                    if offset in _case_offsets(rows):
                        return frozenset((f"OWNER:{identity}+{offset:#x}",))
                return super().resolve_all(rva)

        def index(image, owner):
            datums = [reloc.Datum(
                rva=owner["rva"] + row["table_offset"], size=row["size"],
                section=".text", storage="text", identity=f"N:{identity}:{row['kind']}",
                name=f"reviewed {row['kind']} table", type_name="reviewed compiler table",
                source="reviewed-compiler-table", comment=row["evidence"],
            ) for row in rows] if owner is not None else []
            result = TableIndex(datums, image)
            result.owner_record = owner
            return result

        target_index, base_index = index(target_image, target_owner), index(base_image, base_owner)

        def site(owner, partner, row):
            if owner is None:
                return None
            selector = row["kind"] == "selector"
            access = reloc.Access(
                site=owner["rva"] + row["operand_offset"],
                instruction=owner["rva"] + row["operand_offset"] - 3,
                target=owner["rva"] + row["table_offset"], access="read",
                width="byte" if selector else "dword", form="indexed" if selector else "indcall",
                scale=1 if selector else 4, identity=f"N:{identity}:{row['kind']}",
                instruction_text="reviewed selector read" if selector else "reviewed jump read",
                function=identity, unit=unit, function_rva=owner["rva"], function_size=owner["size"],
                partner_rva=partner["rva"] if partner is not None else None,
            )
            return reloc.Site(access.site, (access,))

        checked = []
        for row in rows:
            checked.append(reloc._audit_one(
                site(target_owner, base_owner, row), site(base_owner, target_owner, row),
                "reviewed-compiler-table", target_image, base_image, target_index, base_index, {},
            ))
        for checked_row in checked:
            checked_row["note"] = error or "reviewed table extent; referents use exact TU-owned case offsets"
        audit_rows.extend(checked)
        exact = not error and all(row["datum_status"] == "EXACT" for row in checked)
        evidence = {"facts": rows, "target": target_raw.hex(), "base": base_raw.hex(), "error": error}
        diff_hash = hashlib.sha256(json.dumps(evidence, sort_keys=True).encode()).hexdigest()[:12]
        source = maxima.whole_source_file_hash(unit) or "-"
        source_hash = hashlib.sha256((source + json.dumps(rows, sort_keys=True)).encode()).hexdigest()[:12]
        gate_row = dict.fromkeys(reloc.FUNCTION_DATA_COLUMNS, "-")
        gate_row.update(
            unit=unit, function=identity, target_function_rvas=f"{target_owner['rva']:#x}",
            base_function_rvas=f"{base_owner['rva']:#x}" if base_owner else "-",
            target_datum_count=str(len(rows)), base_datum_count=str(len(rows)) if base_owner else "0",
            status="EXACT" if exact else "COMPILER_TABLE_DIFF", resolution="EXACT" if exact else "OPEN",
            cone_status="NOT_CHECKED", source_hash=source_hash, diff_hash=diff_hash,
            ledger_status=(ledger.get(function) or {}).get("status") or "-",
            missing_target_datums=error or ";".join(
                row["datum_status"] for row in checked if row["datum_status"] != "EXACT"
            ) or "-",
        )
        function_rows.append(gate_row)
    return audit_rows, function_rows
