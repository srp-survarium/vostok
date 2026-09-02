# SPDX-License-Identifier: GPL-3.0-or-later

"""Consumer-owned data projections for target/base delinking.

The linked LTCG image does not preserve ordinary compiler object boundaries.
Instead of guessing them, this module gives each existing function-comparison
unit a private projection of every typed datum its functions reach.  The same
retail allocation may consequently appear in many synthetic objects.
"""

from __future__ import annotations

import bisect
import csv
import hashlib
import json
from collections import Counter, defaultdict, deque
from dataclasses import dataclass

from vostok.core import paths
from vostok.core.tsv import write_if_changed
from vostok.data.inventory import DataSymbol, display_bytes, load
from vostok.data.pe import PEImage
from vostok.data.pipeline import AddressResolver, image_paths


MANIFEST_COLUMNS = (
    "name", "object", "rva", "size", "storage", "alignment",
    "section_ordinal", "section_offset", "scope", "provenance",
)
SECTION_COLUMNS = (
    "object", "ordinal", "name", "rva", "size", "alignment",
    "characteristics", "comdat_selection", "associative_ordinal", "storage",
    "provenance",
)
CLOSURE_COLUMNS = (
    "unit", "identity", "symbol", "target_rva", "base_rva", "size",
    "storage", "reach", "status",
)
BLOCKER_COLUMNS = ("unit", "target_rva", "identity", "reason")
FUNCTION_COLUMNS = (
    "unit", "mangled", "status", "target_refs", "base_refs", "note",
)

_LNK_COMDAT = 0x00001000
_ALIGN_1 = 0x00100000
_CNT_INITIALIZED = 0x00000040
_CNT_UNINITIALIZED = 0x00000080
_MEM_READ = 0x40000000
_MEM_WRITE = 0x80000000
_WIDTH_BYTES = {
    "byte": 1,
    "word": 2,
    "dword": 4,
    "qword": 8,
    "xmmword": 16,
    "-": 0,
}


@dataclass(frozen=True)
class ConsumerRow:
    unit: str
    target: DataSymbol
    base: DataSymbol | None
    direct: bool


def _trusted(symbol: DataSymbol, image: PEImage) -> bool:
    if symbol.type_index == 0 or symbol.size is None or symbol.size <= 0:
        return False
    try:
        return image.section(symbol.section).contains(symbol.rva, symbol.size)
    except KeyError:
        return False


def _canonical_name(symbol: DataSymbol) -> str:
    if symbol.scope == "external":
        return display_bytes(symbol.public_name or symbol.name)
    digest = hashlib.sha1(symbol.identity.encode("utf-8")).hexdigest()[:24]
    return f"__vostok_data_{digest}"


def _storage(symbol: DataSymbol) -> str:
    if symbol.storage in {"rdata", "data", "bss"}:
        return symbol.storage
    return "rdata" if symbol.section == ".rdata" else "data"


def _section_properties(storage: str) -> tuple[str, int]:
    if storage == "rdata":
        return ".rdata", _CNT_INITIALIZED | _MEM_READ | _LNK_COMDAT | _ALIGN_1
    if storage == "bss":
        return ".bss", _CNT_UNINITIALIZED | _MEM_READ | _MEM_WRITE | _LNK_COMDAT | _ALIGN_1
    return ".data", _CNT_INITIALIZED | _MEM_READ | _MEM_WRITE | _LNK_COMDAT | _ALIGN_1


def _unique_by_identity(symbols: list[DataSymbol], image: PEImage) -> tuple[
    dict[str, DataSymbol], dict[str, str]
]:
    grouped: dict[str, list[DataSymbol]] = defaultdict(list)
    for symbol in symbols:
        if _trusted(symbol, image):
            grouped[symbol.identity].append(symbol)
    unique = {}
    blockers = {}
    for identity, group in grouped.items():
        allocations = {(row.rva, row.size, _storage(row)) for row in group}
        if len(allocations) == 1:
            unique[identity] = min(group, key=lambda row: row.display_name)
        else:
            blockers[identity] = f"identity has {len(allocations)} allocation shapes"
    return unique, blockers


def _access_rows(side: str) -> list[dict[str, str]]:
    path = paths.DATA_TARGET_ACCESS if side == "target" else paths.DATA_BASE_ACCESS
    if not path.is_file():
        return []
    with path.open(newline="", encoding="utf-8") as source:
        return list(csv.DictReader(source, delimiter="\t"))


def _direct_consumers(
    symbols: list[DataSymbol], image: PEImage, admitted: dict[str, DataSymbol]
) -> tuple[dict[str, set[str]], list[dict[str, str]]]:
    resolver = AddressResolver(list(admitted.values()), [])
    raw_resolver = AddressResolver(symbols, [])
    consumers: dict[str, set[str]] = defaultdict(set)
    blockers = []
    for row in _access_rows("target"):
        unit = row["caller_file"]
        if unit == "-":
            continue
        rva = int(row["target_rva"], 0)
        symbol = resolver.data_symbol_at(rva)
        if symbol is None:
            raw_symbol = raw_resolver.data_symbol_at(rva)
            if raw_symbol is not None and raw_symbol.display_name.startswith("__imp_"):
                continue
            blockers.append({
                "unit": unit, "target_rva": f"{rva:#x}",
                "identity": row["target_identity"],
                "reason": "no trusted complete PDB extent",
            })
        else:
            consumers[unit].add(symbol.identity)
            width = _WIDTH_BYTES.get(row["width"], 0)
            if width and symbol.end is not None and rva + width > symbol.end:
                blockers.append({
                    "unit": unit, "target_rva": f"{rva:#x}",
                    "identity": symbol.identity,
                    "reason": (
                        f"{row['width']} access leaves PDB extent by "
                        f"{rva + width - symbol.end} byte(s)"
                    ),
                })
            scale = int(row.get("scale", "0"))
            if row.get("form") == "indexed" and symbol.size is not None:
                if symbol.size <= scale:
                    reason = (
                        f"indexed scale {scale} reaches a PDB extent of only "
                        f"{symbol.size} byte(s)"
                    )
                elif scale and symbol.size % scale:
                    reason = (
                        f"indexed scale {scale} does not tile PDB extent "
                        f"{symbol.size}"
                    )
                else:
                    reason = ""
                if reason:
                    blockers.append({
                        "unit": unit, "target_rva": f"{rva:#x}",
                        "identity": symbol.identity, "reason": reason,
                    })
    return consumers, blockers


def _data_edges(
    symbols: list[DataSymbol], image: PEImage, admitted: dict[str, DataSymbol]
) -> tuple[dict[str, set[str]], dict[str, list[tuple[int, str]]]]:
    resolver = AddressResolver(list(admitted.values()), [])
    raw_resolver = AddressResolver(symbols, [])
    sites = image.base_relocations()
    edges: dict[str, set[str]] = defaultdict(set)
    unresolved: dict[str, list[tuple[int, str]]] = defaultdict(list)
    for identity, source in admitted.items():
        assert source.size is not None
        begin = bisect.bisect_left(sites, source.rva)
        end = bisect.bisect_left(sites, source.rva + source.size)
        for site in sites[begin:end]:
            if site + 4 > source.rva + source.size:
                continue
            value = image.u32_rva(site)
            destination = value - image.image_base if value >= image.image_base else value
            section = image.section_at(destination)
            if section is None or section.name not in {".data", ".rdata"}:
                continue
            target = resolver.data_symbol_at(destination)
            if target is not None and target.identity in admitted:
                edges[identity].add(target.identity)
            else:
                raw_target = raw_resolver.data_symbol_at(destination)
                if raw_target is not None and raw_target.display_name.startswith("__imp_"):
                    continue
                unresolved[identity].append((
                    destination, raw_resolver.resolve(destination, image)
                ))
    return edges, unresolved


def build_consumer_rows() -> tuple[list[ConsumerRow], list[dict[str, str]]]:
    target_symbols = load(paths.DATA_TARGET_INDEX)
    base_symbols = load(paths.DATA_BASE_INDEX)
    target_image = PEImage(image_paths("target")[0])
    base_image = PEImage(image_paths("base")[0])
    target, target_identity_blockers = _unique_by_identity(target_symbols, target_image)
    base, base_identity_blockers = _unique_by_identity(base_symbols, base_image)
    direct, blockers = _direct_consumers(target_symbols, target_image, target)
    edges, edge_blockers = _data_edges(target_symbols, target_image, target)

    rows = []
    for unit, roots in sorted(direct.items()):
        reached = set(roots)
        queue = deque(sorted(roots))
        while queue:
            identity = queue.popleft()
            for child in sorted(edges.get(identity, ())):
                if child not in reached:
                    reached.add(child)
                    queue.append(child)
        for identity in sorted(reached):
            symbol = target.get(identity)
            if symbol is None:
                blockers.append({
                    "unit": unit, "target_rva": "-", "identity": identity,
                    "reason": target_identity_blockers.get(identity, "target allocation unavailable"),
                })
                continue
            base_symbol = base.get(identity)
            rows.append(ConsumerRow(unit, symbol, base_symbol, identity in roots))
            if base_symbol is None:
                blockers.append({
                    "unit": unit,
                    "target_rva": f"{symbol.rva:#x}",
                    "identity": identity,
                    "reason": base_identity_blockers.get(
                        identity, "no base allocation with the retail PDB identity"
                    ),
                })
            for rva, referent in edge_blockers.get(identity, ()):
                blockers.append({
                    "unit": unit, "target_rva": f"{rva:#x}",
                    "identity": referent,
                    "reason": f"initializer referent from {identity} has no trusted extent",
                })
    return rows, blockers


def _write_manifest_side(rows: list[ConsumerRow], side: str) -> None:
    manifest = ["\t".join(MANIFEST_COLUMNS)]
    sections = ["\t".join(SECTION_COLUMNS)]
    by_unit: dict[str, list[ConsumerRow]] = defaultdict(list)
    for row in rows:
        # Objdiff needs one identical section sequence on both sides. A
        # target-only allocation is real debt, but emitting it only on target
        # shifts every later ordinal and poisons unrelated comparisons. Keep
        # it in the closure/blocker tables and enroll it once a base identity
        # exists.
        if row.base is not None:
            by_unit[row.unit].append(row)
    for unit, unit_rows in sorted(by_unit.items()):
        unit_rows.sort(key=lambda row: row.target.identity)
        for ordinal, row in enumerate(unit_rows, 1):
            symbol = row.target if side == "target" else row.base
            assert symbol is not None and symbol.size is not None
            storage = _storage(symbol)
            section_name, characteristics = _section_properties(storage)
            name = _canonical_name(row.target)
            manifest.append("\t".join((
                name, unit, f"{symbol.rva:#x}", f"{symbol.size:#x}", storage,
                "0x1", str(ordinal), "0x0", "external",
                "consumer-owned-pdb-extent",
            )))
            sections.append("\t".join((
                unit, str(ordinal), section_name, f"{symbol.rva:#x}",
                f"{symbol.size:#x}", "0x1", f"{characteristics:#x}", "2", "-",
                storage, "consumer-owned-canonical-section",
            )))
    if side == "target":
        manifest_path = paths.DELINK_DATA_MANIFEST
        section_path = paths.DELINK_DATA_SECTION_MANIFEST
    else:
        manifest_path = paths.BASE_DELINK_DATA_MANIFEST
        section_path = paths.BASE_DELINK_DATA_SECTION_MANIFEST
    write_if_changed(manifest_path, "\n".join(manifest) + "\n")
    write_if_changed(section_path, "\n".join(sections) + "\n")


def _write_rows(rows: list[ConsumerRow], blockers: list[dict[str, str]]) -> None:
    closure = ["\t".join(CLOSURE_COLUMNS)]
    for row in rows:
        target = row.target
        assert target.size is not None
        closure.append("\t".join((
            row.unit, target.identity, _canonical_name(target), f"{target.rva:#x}",
            f"{row.base.rva:#x}" if row.base else "-", str(target.size),
            _storage(target), "direct" if row.direct else "transitive",
            "PAIRED" if row.base else "TARGET_ONLY",
        )))
    write_if_changed(paths.DATA_CONSUMER_CLOSURE, "\n".join(closure) + "\n")

    unique = sorted({tuple(row[column] for column in BLOCKER_COLUMNS) for row in blockers})
    lines = ["\t".join(BLOCKER_COLUMNS)]
    lines.extend("\t".join(values) for values in unique)
    write_if_changed(paths.DATA_MANIFEST_BLOCKERS, "\n".join(lines) + "\n")


def _rich_records(side: str) -> list[dict]:
    path = paths.TARGET_IDX if side == "target" else paths.BASE_IDX
    if not path.is_file():
        return []
    with path.open(encoding="utf-8") as source:
        return [json.loads(line) for line in source if line.strip()]


def _function_access_signatures(side: str) -> dict[tuple[str, str], list[tuple]]:
    records: dict[tuple[str, str], list[dict]] = defaultdict(list)
    for record in _rich_records(side):
        if record.get("mangled") and record.get("file"):
            records[(record["mangled"], record["file"])].append(record)
    out: dict[tuple[str, str], list[tuple]] = defaultdict(list)
    for row in _access_rows(side):
        key = (row["caller_mangled"], row["caller_file"])
        instruction = int(row["instruction_rva"], 0)
        owners = [
            record for record in records.get(key, ())
            if record.get("rva", 0) <= instruction
            < record.get("rva", 0) + record.get("size", 0)
        ]
        if not owners:
            continue
        owner = min(owners, key=lambda record: (record["size"], record["rva"]))
        signature = (
            instruction - owner["rva"], row["access"], row["width"],
            row.get("form", "-"), row.get("scale", "0"),
            row["target_identity"],
        )
        out[key].append(signature)
    for signatures in out.values():
        signatures.sort()
    return out


def _classify_function_refs(target: list[tuple], base: list[tuple]) -> tuple[str, str]:
    if target == base:
        return "EXACT", "-"
    if not base:
        return "BASE_MISSING", "no paired base data references"

    target_identities = [row[-1] for row in target]
    base_identities = [row[-1] for row in base]
    target_counts = Counter(target_identities)
    base_counts = Counter(base_identities)
    if target_counts == base_counts:
        if target_identities != base_identities:
            return "ORDERING_ONLY", "same referent multiset in another order"
        return "ACCESS_SHAPE", "same referents with different site/access shape"
    if set(target_counts) == set(base_counts):
        return "MULTIPLICITY_ONLY", "same referents with different occurrence counts"
    missing = sum((target_counts - base_counts).values())
    extra = sum((base_counts - target_counts).values())
    return "WRONG_REFERENT", f"missing={missing}; extra={extra}"


def write_function_state() -> dict[str, int]:
    target = _function_access_signatures("target")
    base = _function_access_signatures("base")
    lines = ["\t".join(FUNCTION_COLUMNS)]
    counts = Counter()
    for key in sorted(target):
        target_refs = target[key]
        base_refs = base.get(key, [])
        status, note = _classify_function_refs(target_refs, base_refs)
        counts[status] += 1
        lines.append("\t".join((
            key[1], key[0], status, str(len(target_refs)), str(len(base_refs)), note,
        )))
    write_if_changed(paths.DATA_FUNCTION_STATE, "\n".join(lines) + "\n")
    return dict(counts)


def generate() -> dict:
    rows, blockers = build_consumer_rows()
    _write_manifest_side(rows, "target")
    _write_manifest_side(rows, "base")
    _write_rows(rows, blockers)
    function_counts = write_function_state()
    unique_extents = {
        (row.target.rva, row.target.rva + row.target.size)
        for row in rows
        if row.target.size is not None
    }
    paired_extents = {
        (row.target.rva, row.target.rva + row.target.size)
        for row in rows
        if row.target.size is not None and row.base is not None
    }
    summary = {
        "consumer_units": len({row.unit for row in rows}),
        "allocation_copies": len(rows),
        "allocation_copy_bytes": sum(row.target.size or 0 for row in rows),
        "unique_allocations": len({row.target.identity for row in rows}),
        "unique_allocation_bytes": _interval_union(unique_extents),
        "paired_copies": sum(row.base is not None for row in rows),
        "paired_copy_bytes": sum(
            row.target.size or 0 for row in rows if row.base is not None
        ),
        "paired_unique_bytes": _interval_union(paired_extents),
        "blockers": len({tuple(row.values()) for row in blockers}),
        "functions": function_counts,
    }
    summary.update({
        "wrong_referent_regions": function_counts.get("WRONG_REFERENT", 0),
        "ordering_only_regions": function_counts.get("ORDERING_ONLY", 0),
        "multiplicity_only_regions": function_counts.get("MULTIPLICITY_ONLY", 0),
        "access_shape_functions": function_counts.get("ACCESS_SHAPE", 0),
        "base_missing_functions": function_counts.get("BASE_MISSING", 0),
    })
    return summary


def _interval_union(intervals: set[tuple[int, int]]) -> int:
    total = 0
    cursor = -1
    for start, end in sorted(intervals):
        if end <= cursor:
            continue
        total += end - max(start, cursor)
        cursor = end
    return total
