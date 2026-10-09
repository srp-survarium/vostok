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


def registrations():
    with paths.RETAIL_COMPILER_TABLES.open(newline="", encoding="utf-8") as source:
        reader = csv.DictReader(source, delimiter="\t")
        if tuple(reader.fieldnames or ()) != COLUMNS:
            raise ValueError("unexpected reviewed compiler-table columns")
        rows = list(reader)
    if len(rows) != 4 or len({(row["unit"], row["function"]) for row in rows}) != 2:
        raise ValueError("reviewed mesh enrollment must retain both four-table owners")
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


def _validate(image, owner, rows):
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
                    if offset in _CASE_OFFSETS:
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
                instruction=owner["rva"] + (0 if selector else 7),
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
