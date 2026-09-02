# SPDX-License-Identifier: GPL-3.0-or-later
"""Referenced retail allocations absent from the retail PDB."""

from __future__ import annotations

import bisect
import csv
import dataclasses
import json
import os
import shutil
import string
import struct
import subprocess
from collections import Counter, defaultdict
from pathlib import Path

from vostok.core import paths
from vostok.core.tsv import write_if_changed
from vostok.data import pipeline
from vostok.data.inventory import DataSymbol, display_bytes, load
from vostok.data.pe import PEImage


COLUMNS = ("rva", "name", "size", "type", "comment")
PDB_EXTENT_COLUMNS = ("rva", "size", "comment")
REFERENT_COLUMNS = (
    "function_rva", "target_rva", "site_rva", "owner", "addend",
    "occurrences", "provenance",
)
CANDIDATE_COLUMNS = (
    "site_rva", "source_section", "target_rva", "target_section",
    "source_module", "source_archive", "source_contribution_rva",
    "source_contribution_size", "target_module", "target_archive",
    "target_contribution_rva", "target_contribution_size", "reason",
)
XREF_COLUMNS = (
    "site_rva", "source_section", "target_rva", "target_section",
    "source_module", "source_archive", "source_contribution_rva",
    "source_contribution_size", "target_module", "target_archive",
    "target_contribution_rva", "target_contribution_size", "symbol_rva", "addend",
    "name", "size", "type", "status", "comment",
)
_AUTO = "auto:"
_MSVC90_FUNCINFO_MAGIC = 0x19930522
_MSVC90_FUNCINFO_SIZE = 0x24


@dataclasses.dataclass(frozen=True)
class Symbol:
    rva: int
    name: str
    size: int | None
    type_name: str
    comment: str

    @property
    def complete(self) -> bool:
        return (
            bool(self.name.strip())
            and self.size is not None
            and self.size > 0
            and bool(self.type_name.strip())
            and self.type_name != "unknown"
            and bool(self.comment.strip())
        )

    @property
    def reviewed(self) -> bool:
        return (
            self.complete
            and not self.comment.startswith((_AUTO, "derived:"))
        )


@dataclasses.dataclass(frozen=True)
class Candidate:
    site_rva: int
    source_section: str
    target_rva: int
    target_section: str
    source_module: str
    source_archive: str
    source_contribution_rva: int | None
    source_contribution_size: int | None
    target_module: str
    target_archive: str
    target_contribution_rva: int | None
    target_contribution_size: int | None
    reason: str


@dataclasses.dataclass(frozen=True)
class PdbExtent:
    rva: int
    size: int
    comment: str


@dataclasses.dataclass(frozen=True)
class PdbReferent:
    function_rva: int | None
    target_rva: int
    site_rva: int
    owner: str
    addend: int
    occurrences: int
    provenance: str


class ExtentIndex:
    def __init__(self, symbols: list[Symbol]):
        self.starts: dict[int, Symbol] = {}
        self.pages: dict[int, list[Symbol]] = defaultdict(list)
        for symbol in symbols:
            if symbol.rva in self.starts:
                raise ValueError(f"duplicate data symbol RVA {symbol.rva:#x}")
            self.starts[symbol.rva] = symbol
            if symbol.size is None or symbol.size <= 0:
                continue
            for page in range(
                symbol.rva >> 12,
                ((symbol.rva + symbol.size - 1) >> 12) + 1,
            ):
                self.pages[page].append(symbol)

    def owner(self, rva: int) -> tuple[Symbol | None, bool]:
        if exact := self.starts.get(rva):
            return exact, False
        contained = sorted(
            (
                symbol for symbol in self.pages.get(rva >> 12, ())
                if symbol.size is not None and symbol.rva <= rva < symbol.rva + symbol.size
            ),
            key=lambda symbol: (symbol.size or 0, symbol.rva, symbol.name),
        )
        return (contained[0], len(contained) > 1) if contained else (None, False)


class PdbDataCoverage:
    """PDB data starts and only its proven complete data extents."""

    def __init__(
        self,
        symbols: list[DataSymbol],
        reviewed_extents: list[PdbExtent] | None = None,
    ):
        self.starts = {symbol.rva for symbol in symbols}
        self.sorted_starts = sorted(self.starts)
        self.pages: dict[int, list[tuple[int, int]]] = defaultdict(list)
        reviewed_by_rva = {
            extent.rva: extent for extent in (reviewed_extents or [])
        }
        for symbol in symbols:
            size = symbol.size
            if override := reviewed_by_rva.get(symbol.rva):
                size = override.size
            if size is None or size <= 0:
                continue
            end = symbol.rva + size
            for page in range(symbol.rva >> 12, ((end - 1) >> 12) + 1):
                self.pages[page].append((symbol.rva, end))

    def owns(self, rva: int) -> bool:
        if rva in self.starts:
            return True
        return any(start <= rva < end for start, end in self.pages.get(rva >> 12, ()))

    def overlaps(self, rva: int, size: int) -> bool:
        if size <= 0:
            return False
        end = rva + size
        start_index = bisect.bisect_left(self.sorted_starts, rva)
        if (
            start_index < len(self.sorted_starts)
            and self.sorted_starts[start_index] < end
        ):
            return True
        seen = set()
        for page in range(rva >> 12, ((end - 1) >> 12) + 1):
            for extent in self.pages.get(page, ()):
                if extent in seen:
                    continue
                seen.add(extent)
                if rva < extent[1] and extent[0] < end:
                    return True
        return False


class PdbCodeCoverage:
    """Complete procedure extents from the retail rich/PDB index."""

    def __init__(self, functions: list[dict]):
        self.pages: dict[int, list[tuple[int, int]]] = defaultdict(list)
        for function in functions:
            rva, size = function.get("rva"), function.get("size")
            if not isinstance(rva, int) or not isinstance(size, int) or size <= 0:
                continue
            end = rva + size
            for page in range(rva >> 12, ((end - 1) >> 12) + 1):
                self.pages[page].append((rva, end))

    def owns(self, rva: int) -> bool:
        return any(start <= rva < end for start, end in self.pages.get(rva >> 12, ()))

    def overlaps(self, rva: int, size: int) -> bool:
        if size <= 0:
            return False
        end = rva + size
        seen = set()
        for page in range(rva >> 12, ((end - 1) >> 12) + 1):
            for extent in self.pages.get(page, ()):
                if extent in seen:
                    continue
                seen.add(extent)
                if rva < extent[1] and extent[0] < end:
                    return True
        return False


def _clean(value: str) -> str:
    return value.replace("\t", "\\t").replace("\r", "\\r").replace("\n", "\\n")


def _is_game_candidate(candidate: Candidate) -> bool:
    """Match Gruntz's fail-closed game/library fence distinction."""
    module = candidate.source_module.casefold().replace("/", "\\")
    archive = candidate.source_archive.casefold().replace("/", "\\")
    if module == "* linker *":
        return False
    if "binaries.prebuilt" in archive or "microsoft visual studio" in archive:
        return False
    return (
        candidate.source_module == "-"
        or candidate.source_archive == "-"
        or "\\binaries\\win32\\intermediates\\" in module
        or "\\survarium\\binaries\\win32\\libraries\\" in archive
    )


def _placeholder_name(
    image: PEImage, rva: int, candidates: list[Candidate] | None = None,
) -> str:
    prefix = (
        "UNPROVISIONED"
        if candidates and any(_is_game_candidate(row) for row in candidates)
        else "DAT"
    )
    return f"{prefix}_{image.image_base + rva:08x}"


def _is_placeholder(symbol: Symbol, image: PEImage) -> bool:
    return (
        symbol.name in {
            f"DAT_{image.image_base + symbol.rva:08x}",
            f"UNPROVISIONED_{image.image_base + symbol.rva:08x}",
            f"DAT_{image.image_base + symbol.rva:08X}",
            f"UNPROVISIONED_{image.image_base + symbol.rva:08X}",
        }
        and symbol.size is None
        and symbol.type_name == "unknown"
        and symbol.comment.startswith(_AUTO)
    ) or (
        symbol.type_name == "COFF::CodeContribution"
        and symbol.comment.startswith("derived:")
    )


def load_symbols(path: Path | str = paths.RETAIL_DATA_SYMBOLS) -> list[Symbol]:
    path = Path(path)
    if not path.is_file():
        return []
    with path.open(newline="", encoding="utf-8") as source:
        reader = csv.DictReader(source, delimiter="\t")
        if tuple(reader.fieldnames or ()) != COLUMNS:
            raise ValueError(f"{path}: expected exactly " + "\t".join(COLUMNS))
        result = []
        for raw in reader:
            size = None if raw["size"] == "?" else int(raw["size"], 0)
            if size is not None and size <= 0:
                raise ValueError(f"{path}: non-positive size at {raw['rva']}")
            result.append(Symbol(
                rva=int(raw["rva"], 0),
                name=_clean(raw["name"]),
                size=size,
                type_name=_clean(raw["type"]),
                comment=_clean(raw["comment"]),
            ))
    result.sort(key=lambda symbol: symbol.rva)
    ExtentIndex(result)
    return result


def load_pdb_extents(
    path: Path | str = paths.RETAIL_PDB_DATA_EXTENTS,
) -> list[PdbExtent]:
    path = Path(path)
    if not path.is_file():
        return []
    with path.open(newline="", encoding="utf-8") as source:
        reader = csv.DictReader(source, delimiter="\t")
        if tuple(reader.fieldnames or ()) != PDB_EXTENT_COLUMNS:
            raise ValueError(
                f"{path}: expected exactly " + "\t".join(PDB_EXTENT_COLUMNS)
            )
        result = [PdbExtent(
            rva=int(raw["rva"], 0),
            size=int(raw["size"], 0),
            comment=_clean(raw["comment"]),
        ) for raw in reader]
    if any(extent.size <= 0 for extent in result):
        raise ValueError(f"{path}: PDB extents must have positive sizes")
    if len({extent.rva for extent in result}) != len(result):
        raise ValueError(f"{path}: duplicate PDB extent RVA")
    return sorted(result, key=lambda extent: extent.rva)


def _validate_pdb_extents(
    extents: list[PdbExtent], symbols: list[DataSymbol], image: PEImage,
) -> None:
    starts = sorted({symbol.rva for symbol in symbols})
    start_set = set(starts)
    for extent in extents:
        if extent.rva not in start_set:
            raise ValueError(
                f"PDB extent {extent.rva:#x}: no exact retail PDB symbol start"
            )
        section = image.section_at(extent.rva)
        if section is None or not section.contains(extent.rva, extent.size):
            raise ValueError(
                f"PDB extent {extent.rva:#x}+{extent.size:#x} leaves its PE section"
            )
        next_index = bisect.bisect_right(starts, extent.rva)
        if next_index < len(starts) and starts[next_index] < extent.rva + extent.size:
            raise ValueError(
                f"PDB extent {extent.rva:#x}+{extent.size:#x} crosses PDB symbol "
                f"at {starts[next_index]:#x}"
            )


def load_referents(
    path: Path | str = paths.RETAIL_RELOC_REFERENTS,
) -> list[PdbReferent]:
    path = Path(path)
    if not path.is_file():
        return []
    with path.open(newline="", encoding="utf-8") as source:
        reader = csv.DictReader(
            (line for line in source if not line.lstrip().startswith("#")),
            delimiter="\t",
        )
        if tuple(reader.fieldnames or ()) != REFERENT_COLUMNS:
            raise ValueError(
                f"{path}: expected exactly " + "\t".join(REFERENT_COLUMNS)
            )
        result = []
        for raw in reader:
            encoded_addend = int(raw["addend"], 0)
            addend = (
                encoded_addend - 0x100000000
                if encoded_addend >= 0x80000000
                else encoded_addend
            )
            result.append(PdbReferent(
                function_rva=(
                    None if raw["function_rva"] == "-"
                    else int(raw["function_rva"], 0)
                ),
                target_rva=int(raw["target_rva"], 0),
                site_rva=int(raw["site_rva"], 0),
                owner=_clean(raw["owner"]),
                addend=addend,
                occurrences=int(raw["occurrences"], 0),
                provenance=_clean(raw["provenance"]),
            ))
    if len({(row.site_rva, row.target_rva) for row in result}) != len(result):
        raise ValueError(f"{path}: duplicate site/target pair")
    if any(row.occurrences != 1 for row in result):
        raise ValueError(f"{path}: exact site rows must have occurrences=1")
    return result


def _validate_referents(
    referents: list[PdbReferent],
    candidates: list[Candidate],
    pdb_symbols: list[DataSymbol],
) -> set[tuple[int, int]]:
    candidate_pairs = {(row.site_rva, row.target_rva) for row in candidates}
    by_spelling: dict[str, list[DataSymbol]] = defaultdict(list)
    for symbol in pdb_symbols:
        for spelling in {
            symbol.display_name,
            display_bytes(symbol.public_name) if symbol.public_name else "",
        } - {""}:
            by_spelling[spelling].append(symbol)
    result = set()
    for row in referents:
        pair = (row.site_rva, row.target_rva)
        if pair not in candidate_pairs:
            raise ValueError(
                f"referent {row.site_rva:#x}->{row.target_rva:#x} is not in the "
                "raw PDB-complement candidates"
            )
        owners = [
            symbol for symbol in by_spelling.get(row.owner, ())
            if symbol.rva + row.addend == row.target_rva
        ]
        if len({symbol.rva for symbol in owners}) != 1:
            raise ValueError(
                f"referent {row.site_rva:#x}: {row.owner}+{row.addend:#x} does "
                "not resolve uniquely through the retail PDB"
            )
        result.add(pair)
    return result


def _write_symbols(symbols: list[Symbol]) -> None:
    lines = ["\t".join(COLUMNS)]
    for symbol in sorted(symbols, key=lambda item: item.rva):
        lines.append("\t".join((
            f"{symbol.rva:#x}", symbol.name,
            "?" if symbol.size is None else f"{symbol.size:#x}",
            symbol.type_name, symbol.comment,
        )))
    write_if_changed(paths.RETAIL_DATA_SYMBOLS, "\n".join(lines) + "\n")


def export_candidates() -> Path:
    exe, pdb = pipeline.image_paths("target")
    for source in (exe, pdb):
        if not source.is_file():
            raise RuntimeError(f"{source} is missing")
    delinker = os.environ.get("VOSTOK_DATA_DELINKER", "vostok-data-delinker")
    if shutil.which(delinker) is None:
        raise RuntimeError("vostok-data-delinker is not on PATH; enter `nix develop`")
    help_result = subprocess.run(
        [delinker, "--help"], capture_output=True, text=True, check=False
    )
    if "--write-missing-data-index" not in help_result.stdout + help_result.stderr:
        raise RuntimeError(
            "installed vostok-data-delinker lacks --write-missing-data-index; "
            "re-enter the updated Nix development shell"
        )
    output = paths.DATA_MISSING_CANDIDATES
    output.parent.mkdir(parents=True, exist_ok=True)
    pipeline.log("exporting referents absent from the retail PDB")
    subprocess.run([
        delinker,
        "--pdb-path", str(pdb),
        "--exe-path", str(exe),
        "--write-missing-data-index", str(output),
        *pipeline._engine_args("target"),
    ], check=True)
    return output


def load_candidates(path: Path | str) -> tuple[dict[str, int], list[Candidate]]:
    path = Path(path)
    metadata = {}
    for line in path.read_text(encoding="utf-8").splitlines():
        if line.startswith("# ") and "=" in line:
            name, value = line[2:].split("=", 1)
            metadata[name] = int(value, 0)
    with path.open(newline="", encoding="utf-8") as source:
        reader = csv.DictReader(
            (line for line in source if not line.startswith("#")), delimiter="\t"
        )
        if tuple(reader.fieldnames or ()) != CANDIDATE_COLUMNS:
            raise ValueError(f"{path}: unexpected missing-data columns")
        result = [Candidate(
            site_rva=int(raw["site_rva"], 0),
            source_section=raw["source_section"],
            target_rva=int(raw["target_rva"], 0),
            target_section=raw["target_section"],
            source_module=_clean(raw["source_module"]),
            source_archive=_clean(raw["source_archive"]),
            source_contribution_rva=(
                None if raw["source_contribution_rva"] == "-"
                else int(raw["source_contribution_rva"], 0)
            ),
            source_contribution_size=(
                None if raw["source_contribution_size"] == "-"
                else int(raw["source_contribution_size"], 0)
            ),
            target_module=_clean(raw["target_module"]),
            target_archive=_clean(raw["target_archive"]),
            target_contribution_rva=(
                None if raw["target_contribution_rva"] == "-"
                else int(raw["target_contribution_rva"], 0)
            ),
            target_contribution_size=(
                None if raw["target_contribution_size"] == "-"
                else int(raw["target_contribution_size"], 0)
            ),
            reason=_clean(raw["reason"]),
        ) for raw in reader]
    return metadata, result


def _validate_reviewed(
    symbols: list[Symbol], image: PEImage, pdb: PdbDataCoverage,
    code: PdbCodeCoverage | None = None,
) -> None:
    complete = []
    for symbol in symbols:
        section = image.section_at(symbol.rva)
        if section is None:
            raise ValueError(f"{symbol.name}: RVA {symbol.rva:#x} is outside the image")
        if pdb.owns(symbol.rva) or (
            section.name == ".text" and code is not None and code.owns(symbol.rva)
        ):
            raise ValueError(f"{symbol.name}: RVA {symbol.rva:#x} is already in the PDB")
        if symbol.size is None:
            continue
        if not section.contains(symbol.rva, symbol.size):
            raise ValueError(f"{symbol.name}: extent leaves {section.name}")
        if pdb.overlaps(symbol.rva, symbol.size) or (
            section.name == ".text"
            and code is not None
            and code.overlaps(symbol.rva, symbol.size)
        ):
            raise ValueError(f"{symbol.name}: extent overlaps PDB-owned data")
        complete.append(symbol)
    for left, right in zip(sorted(complete, key=lambda item: item.rva),
                           sorted(complete, key=lambda item: item.rva)[1:]):
        assert left.size is not None
        if left.rva + left.size > right.rva:
            raise ValueError(f"reviewed data symbols overlap: {left.name} and {right.name}")


def _counted_type(name: str, count: int) -> str:
    return name if count == 1 else f"{name}[{count}]"


def _derive_eh_symbols(
    image: PEImage,
    by_target: dict[int, list[Candidate]],
    existing: list[Symbol],
) -> list[Symbol]:
    """Recover self-describing MSVC90 EH allocations from retail bytes."""
    existing_index = ExtentIndex(existing)
    derived: dict[int, Symbol] = {}

    def add(rva: int, size: int, type_name: str, provenance: str) -> None:
        if rva not in by_target or existing_index.owner(rva)[0] is not None:
            return
        section = image.section_at(rva)
        if section is None or section.name != ".rdata" or not section.contains(rva, size):
            return
        candidate = Symbol(
            rva=rva,
            name=_placeholder_name(image, rva, by_target[rva]),
            size=size,
            type_name=type_name,
            comment=f"derived: {provenance}",
        )
        previous = derived.get(rva)
        if previous is not None and (
            previous.size != candidate.size or previous.type_name != candidate.type_name
        ):
            derived.pop(rva)
            return
        derived[rva] = candidate

    for rva in sorted(by_target):
        section = image.section_at(rva)
        if (
            section is None
            or section.name != ".rdata"
            or not section.contains(rva, _MSVC90_FUNCINFO_SIZE)
        ):
            continue
        if image.u32_rva(rva) != _MSVC90_FUNCINFO_MAGIC:
            continue
        header = image.read_rva(rva, _MSVC90_FUNCINFO_SIZE)
        (
            magic, states, unwind_va, tries, try_va,
            ip_entries, ip_map_va, _estype_list_va, _flags,
        ) = struct.unpack("<IiIiIiIII", header)
        if (
            magic != _MSVC90_FUNCINFO_MAGIC
            or not 0 <= states <= 4096
            or not 0 <= tries <= 4096
            or not 0 <= ip_entries <= 4096
        ):
            continue
        add(
            rva, _MSVC90_FUNCINFO_SIZE, "EH::FuncInfo",
            "MSVC90 FuncInfo header proves its 0x24-byte extent",
        )

        def pointed_rva(value: int) -> int | None:
            target = value - image.image_base
            pointed = image.section_at(target)
            return target if pointed is not None and pointed.name == ".rdata" else None

        unwind_rva = pointed_rva(unwind_va) if states else None
        if unwind_rva is not None:
            add(
                unwind_rva, states * 8,
                _counted_type("EH::UnwindMapEntry", states),
                f"FuncInfo {rva:#x} states={states} points to this unwind map",
            )

        try_rva = pointed_rva(try_va) if tries else None
        if try_rva is not None:
            add(
                try_rva, tries * 0x14,
                _counted_type("EH::TryBlockMapEntry", tries),
                f"FuncInfo {rva:#x} try_blocks={tries} points to this try map",
            )
            for index in range(tries):
                block = image.read_rva(try_rva + index * 0x14, 0x14)
                _low, _high, _catch_high, catches, handler_va = struct.unpack(
                    "<iiiiI", block
                )
                if not 0 <= catches <= 4096:
                    continue
                handler_rva = pointed_rva(handler_va) if catches else None
                if handler_rva is not None:
                    add(
                        handler_rva, catches * 0x10,
                        _counted_type("EH::HandlerType", catches),
                        f"try map {try_rva:#x}[{index}] points to {catches} handler(s)",
                    )

        ip_map_rva = pointed_rva(ip_map_va) if ip_entries else None
        if ip_map_rva is not None:
            add(
                ip_map_rva, ip_entries * 8,
                _counted_type("EH::IPtoStateMapEntry", ip_entries),
                f"FuncInfo {rva:#x} ip_entries={ip_entries} points to this IP map",
            )

    return sorted(derived.values(), key=lambda symbol: symbol.rva)


def _derive_seh_symbols(
    image: PEImage,
    by_target: dict[int, list[Candidate]],
    existing: list[Symbol],
    pdb_starts: list[int],
) -> list[Symbol]:
    """Recover variable-length x86 SEH3/SEH4 scope tables."""
    text = image.section(".text")
    rdata = image.section(".rdata")
    boundaries = sorted(set(by_target) | {row.rva for row in existing} | set(pdb_starts))
    existing_index = ExtentIndex(existing)
    derived = []

    for rva in sorted(by_target):
        if not rdata.contains(rva, 0xC) or existing_index.owner(rva)[0] is not None:
            continue
        source_rows = by_target[rva]
        if not source_rows or any(
            row.source_section != ".text"
            or row.site_rva == 0
            or image.read_rva(row.site_rva - 1, 1) != b"\x68"
            for row in source_rows
        ):
            continue
        first = image.u32_rva(rva)
        boundary_index = bisect.bisect_right(boundaries, rva)
        end = (
            boundaries[boundary_index]
            if boundary_index < len(boundaries)
            else rdata.end
        )
        end = min(end, rdata.end)
        is_eh4 = first != 0xFFFFFFFF
        if is_eh4:
            header = struct.unpack("<iiii", image.read_rva(rva, 0x10))
            if any(not -0x100000 <= value <= 0x100000 for value in header):
                continue
        records_rva = rva + (0x10 if is_eh4 else 0)
        if records_rva + 0xC > end:
            continue
        count = 0
        cursor = records_rva
        while cursor + 0xC <= end:
            enclosing, filter_rva, handler_rva = struct.unpack(
                "<iII", image.read_rva(cursor, 0xC)
            )
            if enclosing < -2 or enclosing >= count:
                break
            if filter_rva and not text.contains(filter_rva):
                break
            if handler_rva and not text.contains(handler_rva):
                break
            if not filter_rva and not handler_rva:
                break
            count += 1
            cursor += 0xC
        if not count:
            continue
        prefix = "SEH4" if is_eh4 else "SEH3"
        size = (0x10 if is_eh4 else 0) + count * 0xC
        derived.append(Symbol(
            rva=rva,
            name=_placeholder_name(image, rva, by_target[rva]),
            size=size,
            type_name=f"{prefix}::ScopeTable[{count}]",
            comment=(
                f"derived: {prefix} header and {count} validated scope record(s) "
                "prove this extent"
            ),
        ))
    return derived


def refresh(*, export: bool = True) -> dict:
    if export:
        export_candidates()
    if not paths.DATA_MISSING_CANDIDATES.is_file():
        raise RuntimeError(f"{paths.DATA_MISSING_CANDIDATES} is missing")
    if not paths.DATA_TARGET_INDEX.is_file():
        raise RuntimeError(f"{paths.DATA_TARGET_INDEX} is missing; run `vostok data init-target`")

    image = PEImage(pipeline.image_paths("target")[0])
    metadata, candidates = load_candidates(paths.DATA_MISSING_CANDIDATES)
    pdb_symbols = load(paths.DATA_TARGET_INDEX)
    pdb_extents = load_pdb_extents()
    _validate_pdb_extents(pdb_extents, pdb_symbols, image)
    pdb = PdbDataCoverage(pdb_symbols, pdb_extents)
    code = PdbCodeCoverage(pipeline._load_rich("target"))
    referent_pairs = _validate_referents(
        load_referents(), candidates, pdb_symbols,
    )
    redundant_referents = {
        pair for pair in referent_pairs if pdb.owns(pair[1])
    }
    if redundant_referents:
        site, target = min(redundant_referents)
        raise ValueError(
            f"reloc referent {site:#x}->{target:#x} is already extent-owned by PDB"
        )
    reviewed = [
        symbol for symbol in load_symbols()
        if not _is_placeholder(symbol, image)
    ]
    _validate_reviewed(reviewed, image, pdb, code)
    pdb_filtered = [row for row in candidates if pdb.owns(row.target_rva)]
    referent_filtered = [
        row for row in candidates
        if (row.site_rva, row.target_rva) in referent_pairs
    ]
    missing = [
        row for row in candidates
        if not pdb.owns(row.target_rva)
        and (row.site_rva, row.target_rva) not in referent_pairs
    ]
    by_target: dict[int, list[Candidate]] = defaultdict(list)
    for row in missing:
        by_target[row.target_rva].append(row)

    reviewed.extend(_derive_eh_symbols(image, by_target, reviewed))
    reviewed.extend(_derive_seh_symbols(
        image, by_target, reviewed, pdb.sorted_starts,
    ))
    reviewed.sort(key=lambda symbol: symbol.rva)
    _validate_reviewed(reviewed, image, pdb, code)
    reviewed_index = ExtentIndex(reviewed)

    generated: dict[int, Symbol] = {}
    owners: dict[int, tuple[Symbol, bool]] = {}
    for target_rva, rows in sorted(by_target.items()):
        owner, ambiguous = reviewed_index.owner(target_rva)
        if owner is None:
            sections = ",".join(sorted({row.source_section for row in rows}))
            modules = ",".join(
                sorted({row.target_module for row in rows} - {"-"})
            ) or "-"
            owner = Symbol(
                rva=target_rva,
                name=_placeholder_name(image, target_rva, rows),
                size=None,
                type_name="unknown",
                comment=(
                    f"{_AUTO} absent from retail PDB; "
                    f"section={rows[0].target_section}; xrefs={len(rows)}; "
                    f"sources={sections}; contributions={modules}"
                ),
            )
            generated[owner.rva] = owner
        owners[target_rva] = (owner, ambiguous)

    registry = sorted(
        {symbol.rva: symbol for symbol in [*generated.values(), *reviewed]}.values(),
        key=lambda symbol: symbol.rva,
    )
    _write_symbols(registry)

    output = ["\t".join(XREF_COLUMNS)]
    status_counts: Counter[str] = Counter()
    complete_sites = 0
    for row in sorted(missing, key=lambda item: (item.site_rva, item.target_rva)):
        owner, ambiguous = owners[row.target_rva]
        status = (
            "ambiguous" if ambiguous
            else "reviewed" if owner.reviewed
            else "unreviewed" if owner.complete
            else "incomplete"
        )
        status_counts[status] += 1
        complete_sites += status == "reviewed"
        output.append("\t".join((
            f"{row.site_rva:#x}", row.source_section,
            f"{row.target_rva:#x}", row.target_section,
            row.source_module, row.source_archive,
            "-" if row.source_contribution_rva is None
            else f"{row.source_contribution_rva:#x}",
            "-" if row.source_contribution_size is None
            else f"{row.source_contribution_size:#x}",
            row.target_module, row.target_archive,
            "-" if row.target_contribution_rva is None
            else f"{row.target_contribution_rva:#x}",
            "-" if row.target_contribution_size is None
            else f"{row.target_contribution_size:#x}",
            f"{owner.rva:#x}", f"{row.target_rva - owner.rva:#x}",
            owner.name, "?" if owner.size is None else f"{owner.size:#x}",
            owner.type_name, status, row.reason,
        )))
    write_if_changed(paths.DATA_MISSING_XREFS, "\n".join(output) + "\n")

    unresolved_targets = sum(
        ambiguous or not owner.reviewed for owner, ambiguous in owners.values()
    )
    used_owner_rvas = {owner.rva for owner, _ in owners.values()}
    orphan_rows = sum(symbol.rva not in used_owner_rvas for symbol in registry)
    report = {
        "schema": 1,
        "relevant_xref_sites": metadata.get("relevant_xrefs", 0),
        "delinker_pdb_owned_xref_sites": metadata.get("pdb_owned_xrefs", 0),
        "candidate_xref_sites": len(candidates),
        "pdb_extent_filtered_xref_sites": len(pdb_filtered),
        "pdb_referent_filtered_xref_sites": len(referent_filtered),
        "missing_xref_sites": len(missing),
        "unique_missing_targets": len(by_target),
        "registry_rows": len(registry),
        "complete_registry_rows": sum(symbol.complete for symbol in registry),
        "incomplete_registry_rows": sum(not symbol.complete for symbol in registry),
        "reviewed_registry_rows": sum(symbol.reviewed for symbol in registry),
        "unreviewed_registry_rows": sum(not symbol.reviewed for symbol in registry),
        "reviewed_xref_sites": complete_sites,
        "unreviewed_xref_sites": len(missing) - complete_sites,
        "unresolved_targets": unresolved_targets,
        "orphan_registry_rows": orphan_rows,
        "statuses": dict(sorted(status_counts.items())),
        "target_sections": dict(sorted(Counter(
            row.target_section for row in missing
        ).items())),
        "unique_target_sections": dict(sorted(Counter(
            rows[0].target_section for rows in by_target.values()
        ).items())),
    }
    report["inputs"] = {
        "exe_sha256": pipeline._file_hash(pipeline.image_paths("target")[0]),
        "pdb_index_sha256": pipeline._file_hash(paths.DATA_TARGET_INDEX),
        "rich_index_sha256": pipeline._file_hash(paths.TARGET_IDX),
        "pdb_extents_sha256": pipeline._file_hash(paths.RETAIL_PDB_DATA_EXTENTS),
        "reloc_referents_sha256": pipeline._file_hash(paths.RETAIL_RELOC_REFERENTS),
        "candidates_sha256": pipeline._file_hash(paths.DATA_MISSING_CANDIDATES),
        "registry_sha256": pipeline._file_hash(paths.RETAIL_DATA_SYMBOLS),
        "xrefs_sha256": pipeline._file_hash(paths.DATA_MISSING_XREFS),
    }
    write_if_changed(paths.DATA_MISSING_REPORT, json.dumps(report, indent=2) + "\n")
    return report


def _expected_relevant(image: PEImage) -> set[tuple[int, int]]:
    result = set()
    for site in image.base_relocations():
        value = image.u32_rva(site)
        target = (value - image.image_base) & 0xFFFFFFFF
        section = image.section_at(target)
        if section is not None and section.name in {".text", ".rdata", ".data"}:
            result.add((site, target))
    return result


def check(*, require_complete: bool = True) -> int:
    required = (
        paths.DATA_MISSING_CANDIDATES, paths.RETAIL_DATA_SYMBOLS,
        paths.RETAIL_PDB_DATA_EXTENTS,
        paths.RETAIL_RELOC_REFERENTS,
        paths.DATA_MISSING_XREFS, paths.DATA_MISSING_REPORT,
    )
    missing_paths = [path for path in required if not path.is_file()]
    if missing_paths:
        print("missing non-PDB data artifacts: " + ", ".join(map(str, missing_paths)))
        return 1
    report = json.loads(paths.DATA_MISSING_REPORT.read_text(encoding="utf-8"))
    if report.get("schema") != 1:
        print("unsupported non-PDB data report schema")
        return 1
    actual_inputs = {
        "exe_sha256": pipeline._file_hash(pipeline.image_paths("target")[0]),
        "pdb_index_sha256": pipeline._file_hash(paths.DATA_TARGET_INDEX),
        "rich_index_sha256": pipeline._file_hash(paths.TARGET_IDX),
        "pdb_extents_sha256": pipeline._file_hash(paths.RETAIL_PDB_DATA_EXTENTS),
        "reloc_referents_sha256": pipeline._file_hash(paths.RETAIL_RELOC_REFERENTS),
        "candidates_sha256": pipeline._file_hash(paths.DATA_MISSING_CANDIDATES),
        "registry_sha256": pipeline._file_hash(paths.RETAIL_DATA_SYMBOLS),
        "xrefs_sha256": pipeline._file_hash(paths.DATA_MISSING_XREFS),
    }
    if report.get("inputs") != actual_inputs:
        print("non-PDB data report is stale")
        return 1
    image = PEImage(pipeline.image_paths("target")[0])
    expected = _expected_relevant(image)
    if report.get("relevant_xref_sites") != len(expected):
        print("delinker did not inspect every relevant retail relocation")
        return 1
    metadata, candidates = load_candidates(paths.DATA_MISSING_CANDIDATES)
    if metadata.get("relevant_xrefs") != len(expected):
        print("missing-data candidate metadata is inconsistent")
        return 1
    pdb_symbols = load(paths.DATA_TARGET_INDEX)
    pdb_extents = load_pdb_extents()
    _validate_pdb_extents(pdb_extents, pdb_symbols, image)
    pdb = PdbDataCoverage(pdb_symbols, pdb_extents)
    code = PdbCodeCoverage(pipeline._load_rich("target"))
    referent_pairs = _validate_referents(
        load_referents(), candidates, pdb_symbols,
    )
    final_expected = {
        (row.site_rva, row.target_rva) for row in candidates
        if not pdb.owns(row.target_rva)
        and (row.site_rva, row.target_rva) not in referent_pairs
    }
    with paths.DATA_MISSING_XREFS.open(newline="", encoding="utf-8") as source:
        reader = csv.DictReader(source, delimiter="\t")
        if tuple(reader.fieldnames or ()) != XREF_COLUMNS:
            print("non-PDB xref report has an unexpected schema")
            return 1
        xrefs = list(reader)
    final_actual = {
        (int(row["site_rva"], 0), int(row["target_rva"], 0)) for row in xrefs
    }
    if final_actual != final_expected or len(xrefs) != len(final_expected):
        print("non-PDB xref report does not equal the PDB complement")
        return 1
    symbols = load_symbols()
    _validate_reviewed(symbols, image, pdb, code)
    statuses = Counter(row["status"] for row in xrefs)
    if dict(sorted(statuses.items())) != report.get("statuses"):
        print("non-PDB xref status counts do not match their report")
        return 1
    if report.get("orphan_registry_rows"):
        print("data_symbols.tsv contains rows not reached by any non-PDB xref")
        return 1
    if report["unresolved_targets"] and require_complete:
        print(
            f"non-PDB data review incomplete: {report['unresolved_targets']:,} "
            f"target(s), {report['unreviewed_xref_sites']:,} xref site(s) need review"
        )
        return 1
    if report["unresolved_targets"]:
        print(
            f"non-PDB data artifacts valid (shadow mode; "
            f"{report['unresolved_targets']:,} target(s) need review)"
        )
        return 0
    print(f"non-PDB data census complete: {report['registry_rows']:,} symbol(s)")
    return 0


def print_report(report: dict | None = None) -> None:
    if report is None:
        if not paths.DATA_MISSING_REPORT.is_file():
            raise RuntimeError("no non-PDB data report; run `python3 -m vostok data missing`")
        report = json.loads(paths.DATA_MISSING_REPORT.read_text(encoding="utf-8"))
    print(
        "Non-PDB data: {missing_xref_sites:,} xrefs to {unique_missing_targets:,} "
        "targets; {reviewed_registry_rows:,}/{registry_rows:,} registry rows reviewed; "
        "{unresolved_targets:,} targets need review".format(**report)
    )
    print(
        "  excluded as PDB-owned: delinker={delinker_pdb_owned_xref_sites:,} "
        "typed-extents={pdb_extent_filtered_xref_sites:,} "
        "referent-addends={pdb_referent_filtered_xref_sites:,}".format(**report)
    )
    print("  non-PDB target sections: " + "  ".join(
        f"{name}={count:,}" for name, count in report["unique_target_sections"].items()
    ))


def _load_tsv(path: Path) -> list[dict[str, str]]:
    if not path.is_file():
        return []
    with path.open(newline="", encoding="utf-8") as source:
        return list(csv.DictReader(source, delimiter="\t"))


def _hex_ascii(data: bytes) -> str:
    hex_bytes = " ".join(f"{value:02x}" for value in data)
    printable = set(string.printable.encode("ascii")) - {ord("\t"), ord("\r"), ord("\n")}
    ascii_bytes = "".join(chr(value) if value in printable else "." for value in data)
    return f"{hex_bytes:<95}  |{ascii_bytes}|"


def inspect(pattern: str) -> int:
    """Print one review dossier for each matching non-PDB allocation."""
    image = PEImage(pipeline.image_paths("target")[0])
    symbols = load_symbols()
    index = ExtentIndex(symbols)
    xrefs = _load_tsv(paths.DATA_MISSING_XREFS)
    access_by_site = {
        int(row["site_rva"], 0): row
        for row in _load_tsv(paths.DATA_TARGET_ACCESS)
    }
    pdb_symbols = sorted(load(paths.DATA_TARGET_INDEX), key=lambda row: row.rva)
    pdb_rvas = [row.rva for row in pdb_symbols]

    exacts = set()
    for token in pattern.split(","):
        try:
            exacts.add(int(token.strip(), 0))
        except ValueError:
            exacts.clear()
            break
    needle = pattern.casefold()
    selected: dict[int, Symbol] = {}
    for symbol in symbols:
        extent_hit = (
            bool(exacts)
            and symbol.size is not None
            and any(symbol.rva <= value < symbol.rva + symbol.size for value in exacts)
        )
        if (
            symbol.rva in exacts
            or extent_hit
            or needle in "\t".join((
                f"{symbol.rva:#x}", symbol.name, symbol.type_name, symbol.comment,
            )).casefold()
        ):
            selected[symbol.rva] = symbol
    for row in xrefs:
        site = int(row["site_rva"], 0)
        target = int(row["target_rva"], 0)
        symbol_rva = int(row["symbol_rva"], 0)
        if exacts.intersection((site, target, symbol_rva)) or (
            not exacts and needle in "\t".join(row.values()).casefold()
        ):
            owner, _ = index.owner(symbol_rva)
            if owner is not None:
                selected[owner.rva] = owner
    if not selected:
        return 1
    if len(selected) > 20:
        print(f"{len(selected):,} non-PDB allocations match; refine the pattern")
        return 2

    xrefs_by_owner: dict[int, list[dict[str, str]]] = defaultdict(list)
    for row in xrefs:
        owner, _ = index.owner(int(row["target_rva"], 0))
        if owner is not None:
            xrefs_by_owner[owner.rva].append(row)

    for ordinal, symbol in enumerate(sorted(selected.values(), key=lambda row: row.rva)):
        if ordinal:
            print()
        section = image.section_at(symbol.rva)
        assert section is not None
        size = "?" if symbol.size is None else f"{symbol.size:#x}"
        print(
            f"{symbol.rva:#010x}  va={image.image_base + symbol.rva:#010x}  "
            f"{section.name}  size={size}  type={symbol.type_name}"
        )
        print(f"  {symbol.name}")
        print(f"  {symbol.comment}")

        preview_size = min(symbol.size or 64, 64, section.end - symbol.rva)
        preview = image.read_rva(symbol.rva, preview_size)
        for offset in range(0, len(preview), 32):
            print(f"  +{offset:#04x}  {_hex_ascii(preview[offset:offset + 32])}")

        pdb_index = bisect.bisect_left(pdb_rvas, symbol.rva)
        neighbors = pdb_symbols[max(0, pdb_index - 2):pdb_index + 2]
        print("  nearest PDB:")
        for neighbor in neighbors:
            delta = neighbor.rva - symbol.rva
            neighbor_size = "?" if neighbor.size is None else f"{neighbor.size:#x}"
            print(
                f"    {delta:+#x}  {neighbor.rva:#010x}  size={neighbor_size:<8} "
                f"{neighbor.display_name}"
            )

        owner_xrefs = sorted(
            xrefs_by_owner.get(symbol.rva, ()),
            key=lambda row: (int(row["site_rva"], 0), int(row["target_rva"], 0)),
        )
        print(f"  xrefs ({len(owner_xrefs)}):")
        for row in owner_xrefs:
            site = int(row["site_rva"], 0)
            target = int(row["target_rva"], 0)
            access = access_by_site.get(site)
            if access is None:
                print(
                    f"    site={site:#010x} {row['source_section']} -> "
                    f"{target:#010x} ({target - symbol.rva:+#x})"
                )
                continue
            caller = access.get("caller_mangled") or access.get("caller_name") or "-"
            print(
                f"    site={site:#010x} -> {target:#010x} "
                f"({target - symbol.rva:+#x})  {access.get('instruction', '-')}"
            )
            if row.get("module") not in (None, "", "-"):
                print(f"      contribution={row['module']}  archive={row['archive']}")
            print(f"      {caller}")
            if access.get("caller_file") not in (None, "", "-"):
                print(f"      {access['caller_file']}")
    return 0


def inspect_next() -> int:
    """Open the first datum which still needs a manual label review."""
    pending = [symbol for symbol in load_symbols() if not symbol.reviewed]
    if not pending:
        print("non-PDB data census complete: no datum needs review")
        return 0
    symbol = min(pending, key=lambda row: row.rva)
    print(
        f"next non-PDB datum: {symbol.rva:#x} "
        f"({len(pending):,} allocation(s) still need review)"
    )
    return inspect(f"{symbol.rva:#x}")
