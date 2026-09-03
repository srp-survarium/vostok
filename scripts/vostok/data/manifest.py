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
import dataclasses
import hashlib
import json
from collections import Counter, defaultdict, deque
from dataclasses import dataclass

from vostok.core import paths
from vostok.core.tsv import write_if_changed
from vostok.data.inventory import DataSymbol, display_bytes, load
from vostok.data.pe import PEImage
from vostok.data.pipeline import (
    AddressResolver,
    _apply_access_extents,
    _apply_target_extents,
    _augment_comparison_symbols,
    _current_code_audit_rows,
    _has_identity_extent,
    _transfer_target_extents,
    compare,
    image_paths,
)


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
    if (
        not _has_identity_extent(symbol)
        or symbol.size is None
        or symbol.size <= 0
    ):
        return False
    try:
        return image.section(symbol.section).contains(symbol.rva, symbol.size)
    except KeyError:
        return False


def _canonical_name(symbol: DataSymbol) -> str:
    if symbol.scope == "external":
        return display_bytes(symbol.public_name or symbol.name)
    allocation = f"{symbol.identity}@{symbol.rva:#x}"
    digest = hashlib.sha1(allocation.encode("utf-8")).hexdigest()[:24]
    return f"__vostok_data_{digest}"


def _projection_name(
    row: ConsumerRow,
    split_names: set[str],
) -> str:
    """Name one private projection while retaining an external COMDAT leader."""
    name = _canonical_name(row.target)
    if name not in split_names:
        return name
    allocation = f"{name}\0{row.unit}\0{row.target.identity}\0{row.target.rva:#x}"
    digest = hashlib.sha1(allocation.encode("utf-8")).hexdigest()[:16]
    return f"{name}$vostok_projection${digest}"


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
    symbols: list[DataSymbol], image: PEImage, admitted: dict[int, DataSymbol]
) -> tuple[dict[str, set[int]], list[dict[str, str]]]:
    resolver = AddressResolver(list(admitted.values()), [])
    raw_resolver = AddressResolver(symbols, [])
    consumers: dict[str, set[int]] = defaultdict(set)
    blockers = []
    for row in _access_rows("target"):
        unit = row["caller_file"]
        if not unit or unit == "-":
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
            consumers[unit].add(symbol.rva)
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
    symbols: list[DataSymbol], image: PEImage, admitted: dict[int, DataSymbol]
) -> tuple[dict[int, set[int]], dict[int, list[tuple[int, str]]]]:
    resolver = AddressResolver(list(admitted.values()), [])
    raw_resolver = AddressResolver(symbols, [])
    sites = image.base_relocations()
    edges: dict[int, set[int]] = defaultdict(set)
    unresolved: dict[int, list[tuple[int, str]]] = defaultdict(list)
    for source_rva, source in admitted.items():
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
            if target is not None and target.rva in admitted:
                edges[source_rva].add(target.rva)
            else:
                raw_target = raw_resolver.data_symbol_at(destination)
                if raw_target is not None and raw_target.display_name.startswith("__imp_"):
                    continue
                unresolved[source_rva].append((
                    destination, raw_resolver.resolve(destination, image)
                ))
    return edges, unresolved


def _allocation_maps(
    target_symbols: list[DataSymbol], base_symbols: list[DataSymbol],
    target_image: PEImage, base_image: PEImage,
) -> tuple[dict[int, DataSymbol], dict[int, DataSymbol], dict[int, DataSymbol]]:
    """Deduplicate PDB aliases and retain the proven cross-image allocation map."""
    target_trusted = [
        symbol for symbol in target_symbols if _trusted(symbol, target_image)
    ]
    base_trusted = [
        symbol for symbol in base_symbols if _trusted(symbol, base_image)
    ]
    target_resolver = AddressResolver(
        target_trusted, [],
        peer_identities={symbol.identity for symbol in base_trusted},
    )
    base_resolver = AddressResolver(
        base_trusted, [],
        peer_identities={symbol.identity for symbol in target_trusted},
    )
    target = {
        rva: target_resolver.data_symbol_at(rva)
        for rva in sorted({symbol.rva for symbol in target_trusted})
    }
    base = {
        rva: base_resolver.data_symbol_at(rva)
        for rva in sorted({symbol.rva for symbol in base_trusted})
    }
    target = {rva: symbol for rva, symbol in target.items() if symbol is not None}
    base = {rva: symbol for rva, symbol in base.items() if symbol is not None}

    paired = {}
    for row in compare(
        target_symbols, base_symbols, target_image, base_image
    ):
        if row["target_rva"] == "-" or row["base_rva"] == "-":
            continue
        target_rva = int(row["target_rva"], 0)
        base_rva = int(row["base_rva"], 0)
        if target_rva in target and base_rva in base:
            paired[target_rva] = base[base_rva]
    return target, base, paired


def _consumer_code_pairs(
    target_symbols: list[DataSymbol],
    base_image: PEImage,
    audit_rows: list[dict[str, str]] | None = None,
) -> dict[tuple[str, int], DataSymbol]:
    """Transfer each TU's retail allocation to its candidate representative."""
    resolver = AddressResolver(target_symbols, [])
    votes: dict[tuple[str, int], Counter[int]] = defaultdict(Counter)
    relocation_sites = base_image.base_relocations()
    rows = _current_code_audit_rows() if audit_rows is None else audit_rows
    for row in rows:
        if (
            row.get("pair_status") != "PAIRED"
            or row.get("access_status") != "EXACT"
            or not row.get("unit")
            or row.get("target_datum_rva") in {None, "-"}
            or row.get("inferred_base_datum_rva") in {None, "-"}
        ):
            continue
        target_rva = int(row["target_datum_rva"], 0)
        target = resolver.data_symbol_at(target_rva)
        if target is None or target.rva != target_rva or target.size is None:
            continue
        votes[(row["unit"], target_rva)][
            int(row["inferred_base_datum_rva"], 0)
        ] += 1

    result = {}
    for key, candidates in votes.items():
        ranked = candidates.most_common(2)
        if len(ranked) > 1 and ranked[0][1] < 4 * ranked[1][1]:
            continue
        base_rva = ranked[0][0]
        target = resolver.data_symbol_at(key[1])
        assert target is not None and target.size is not None
        section = base_image.section_at(base_rva)
        end_rva = base_rva + target.size
        tail_reloc = bisect.bisect_left(
            relocation_sites, max(base_rva, end_rva - 3)
        )
        if (
            section is None
            or section.name not in {".rdata", ".data"}
            or not section.contains(base_rva, target.size)
            # A HIGHLOW cell starting in the final three bytes cannot belong
            # to this allocation.  Code-xref transfer occasionally votes for
            # an overlapping, unrelated linked datum; retain the ordinary
            # identity pairing instead of emitting an invalid COFF relocation.
            or (
                tail_reloc < len(relocation_sites)
                and relocation_sites[tail_reloc] < end_rva
            )
        ):
            continue
        storage = (
            "rdata" if section.name == ".rdata"
            else "data" if base_rva - section.rva < section.raw_size
            else "bss"
        )
        result[key] = dataclasses.replace(
            target,
            rva=base_rva,
            section=section.name,
            storage=storage,
            size_kind="retail-consumer-xref",
        )
    return result


def build_consumer_rows() -> tuple[list[ConsumerRow], list[dict[str, str]]]:
    target_image = PEImage(image_paths("target")[0])
    base_image = PEImage(image_paths("base")[0])
    target_symbols = _apply_target_extents(
        load(paths.DATA_TARGET_INDEX), target_image
    )
    target_symbols = _apply_access_extents(target_symbols, "target")
    base_symbols = _transfer_target_extents(
        target_symbols, load(paths.DATA_BASE_INDEX)
    )
    target_symbols, base_symbols = _augment_comparison_symbols(
        target_symbols, base_symbols, target_image, base_image
    )
    target, _, paired = _allocation_maps(
        target_symbols, base_symbols, target_image, base_image
    )
    consumer_pairs = _consumer_code_pairs(target_symbols, base_image)
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
        for target_rva in sorted(reached):
            symbol = target.get(target_rva)
            if symbol is None:
                blockers.append({
                    "unit": unit, "target_rva": f"{target_rva:#x}",
                    "identity": "-", "reason": "target allocation unavailable",
                })
                continue
            base_symbol = (
                consumer_pairs.get((unit, target_rva))
                if target_rva in roots else None
            ) or paired.get(target_rva)
            rows.append(ConsumerRow(
                unit, symbol, base_symbol, target_rva in roots
            ))
            if base_symbol is None:
                blockers.append({
                    "unit": unit,
                    "target_rva": f"{symbol.rva:#x}",
                    "identity": symbol.identity,
                    "reason": "no paired base allocation",
                })
            for rva, referent in edge_blockers.get(target_rva, ()):
                blockers.append({
                    "unit": unit, "target_rva": f"{rva:#x}",
                    "identity": referent, "reason": (
                        f"initializer referent from {symbol.identity} "
                        "has no trusted extent"
                    ),
                })
    return rows, blockers


def _write_manifest_side(rows: list[ConsumerRow], side: str) -> None:
    manifest = ["\t".join(MANIFEST_COLUMNS)]
    sections = ["\t".join(SECTION_COLUMNS)]
    physical_rvas: dict[str, tuple[set[int], set[int]]] = defaultdict(
        lambda: (set(), set())
    )
    for row in rows:
        if row.base is None:
            continue
        name = _canonical_name(row.target)
        physical_rvas[name][0].add(row.target.rva)
        physical_rvas[name][1].add(row.base.rva)
    split_names = {
        name for name, (target_rvas, base_rvas) in physical_rvas.items()
        if len(target_rvas) > 1 or len(base_rvas) > 1
    }
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
        unit_rows.sort(key=lambda row: row.target.rva)
        for ordinal, row in enumerate(unit_rows, 1):
            symbol = row.target if side == "target" else row.base
            assert symbol is not None and symbol.size is not None
            storage = _storage(symbol)
            section_name, characteristics = _section_properties(storage)
            name = _projection_name(row, split_names)
            manifest.append("\t".join((
                name, unit, f"{symbol.rva:#x}", f"{symbol.size:#x}", storage,
                # Associative COMDAT sections require an external leader. A
                # split name above keeps TU-specific LTCG representatives
                # distinct without discarding the readable PDB identity.
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
        "unique_allocations": len({row.target.rva for row in rows}),
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
