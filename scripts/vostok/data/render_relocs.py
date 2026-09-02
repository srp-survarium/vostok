# SPDX-License-Identifier: GPL-3.0-or-later

"""Direct retail/base relocation audit for one Vostok module.

This is intentionally simpler than the consumer-owned data projection.  It
starts with every physical HIGHLOW relocation in a module function whose value
lands in .rdata or .data (including the loader-zero BSS tail), pairs sites only
through function evidence, and records what each side actually addresses.

Complete retail extents get a normalized byte/relocation comparison.  A PDB
symbol with no trustworthy size gets a separate evidence dossier: all observed
accesses, the next known symbol and relocation-derived boundaries, the complete
retail byte window, pattern hints, and every candidate-start vote.
"""

from __future__ import annotations

import bisect
import csv
import dataclasses
import hashlib
import json
import math
import re
import struct
from collections import Counter, defaultdict
from difflib import SequenceMatcher
from pathlib import Path

from vostok.core import paths
from vostok.core.tsv import write_if_changed
from vostok.data import missing, pipeline
from vostok.data.inventory import load
from vostok.data.pe import PEImage
from vostok.ledger import store
from vostok.sema import pairing as sema_pairing


AUDIT_COLUMNS = (
    "pair_status", "pair_kind", "target_site_rva", "base_site_rva",
    "function", "unit", "target_function_rva", "base_function_rva",
    "target_function_offset", "base_function_offset",
    "target_instruction_rva", "base_instruction_rva",
    "target_access", "base_access", "target_width", "base_width",
    "target_form", "base_form", "target_scale", "base_scale", "access_status",
    "target_target_rva", "base_target_rva", "target_section", "base_section",
    "target_storage", "base_storage", "target_identity", "base_identity",
    "identity_status", "target_datum_rva", "base_datum_rva",
    "inferred_base_datum_rva", "target_addend", "base_addend",
    "target_size", "base_size", "extent_status", "datum_status",
    "normalized_bytes_status", "reloc_layout_status", "reloc_targets_status",
    "first_diff", "note",
)

EXTENTLESS_COLUMNS = (
    "target_rva", "identity", "name", "type", "section", "storage",
    "xref_sites", "referenced_offsets", "accesses", "observed_end_rva",
    "next_referent_rva", "next_referent_delta", "next_known_start_rva",
    "next_known_start_delta", "next_data_reloc_rva", "next_data_reloc_delta",
    "evidence_size", "candidate_base_rva", "candidate_identity",
    "candidate_identity_status", "candidate_votes", "base_next_known_delta",
    "compare_status", "raw_status",
    "normalized_status", "reloc_layout_status", "reloc_targets_status",
    "first_diff", "retail_pattern", "retail_sha256", "base_sha256",
    "retail_head", "retail_tail", "base_head", "comment",
)

FUNCTION_DATA_COLUMNS = (
    "unit", "function", "target_function_rvas", "base_function_rvas",
    "target_datum_count", "base_datum_count", "status", "resolution",
    "source_hash", "ledger_status",
    "missing_target_datums", "extra_base_datums",
    "missing_base_definitions", "missing_target_definitions",
)

_WIDTHS = {"byte": 1, "word": 2, "dword": 4, "qword": 8, "xmmword": 16}
_ABSOLUTE = re.compile(r"(?<![A-Za-z0-9_])(?:0x[0-9a-fA-F]+|[0-9]{4,})(?![A-Za-z0-9_])")
_SETTLED_EXTENTLESS = frozenset({
    "EXACT", "EXACT_WITH_OUTLIERS", "IDENTITY_FALLBACK_EXACT",
    "LINKER_VALUE", "VTABLE_CODE_TARGETS",
})


@dataclasses.dataclass(frozen=True)
class Access:
    site: int
    instruction: int
    target: int
    access: str
    width: str
    form: str
    scale: int
    identity: str
    instruction_text: str
    function: str
    unit: str
    function_rva: int
    function_size: int
    partner_rva: int | None

    @property
    def function_offset(self) -> int:
        return self.site - self.function_rva

    @property
    def token(self) -> tuple[str, str, str, int, str]:
        instruction = _ABSOLUTE.sub("<value>", self.instruction_text.casefold())
        return self.access, self.width, self.form, self.scale, instruction


@dataclasses.dataclass(frozen=True)
class Site:
    rva: int
    accesses: tuple[Access, ...]

    def primary(self, function_rva: int | None = None) -> Access:
        candidates = self.accesses
        if function_rva is not None:
            owned = tuple(row for row in candidates if row.function_rva == function_rva)
            if owned:
                candidates = owned
        return min(
            candidates,
            key=lambda row: (
                row.function_size, row.function_rva, row.function, row.unit,
                row.instruction,
            ),
        )


@dataclasses.dataclass(frozen=True)
class Datum:
    rva: int
    size: int | None
    section: str
    storage: str
    identity: str
    name: str
    type_name: str
    source: str
    comment: str = ""

    @property
    def end(self) -> int | None:
        return self.rva + self.size if self.size is not None else None

    @property
    def complete(self) -> bool:
        return self.size is not None and self.size > 0


@dataclasses.dataclass(frozen=True)
class ModuleArtifacts:
    audit: Path
    extentless: Path
    function_data: Path
    report: Path
    problems: Path


def _artifacts(module: str) -> ModuleArtifacts:
    return ModuleArtifacts(
        audit=paths.data_module_reloc_audit(module),
        extentless=paths.data_module_extentless(module),
        function_data=paths.data_module_function_data(module),
        report=paths.data_module_reloc_report(module),
        problems=paths.data_module_problems(module),
    )


class DatumIndex:
    def __init__(self, datums: list[Datum], image: PEImage,
                 function_identities: dict[int, frozenset[str]] | None = None):
        self.image = image
        self.relocations = image.base_relocations()
        self.function_identities = function_identities or {}
        self.exact: dict[int, list[Datum]] = defaultdict(list)
        self.by_identity: dict[str, list[Datum]] = defaultdict(list)
        self.pages: dict[int, list[Datum]] = defaultdict(list)
        self.starts_by_section: dict[str, list[int]] = defaultdict(list)
        for datum in datums:
            self.exact[datum.rva].append(datum)
            self.by_identity[datum.identity].append(datum)
            if datum.complete:
                assert datum.end is not None
                for page in range(datum.rva >> 12, ((datum.end - 1) >> 12) + 1):
                    self.pages[page].append(datum)
        for rva in sorted(self.exact):
            section = image.section_at(rva)
            if section is not None:
                self.starts_by_section[section.name].append(rva)

    @staticmethod
    def _hint_identity(hint: str) -> str:
        if not hint.startswith("D:"):
            return ""
        body = hint[2:]
        return body.rsplit("+", 1)[0]

    def owner(self, rva: int, hint: str = "") -> Datum | None:
        identity = self._hint_identity(hint)

        def rank(row: Datum) -> tuple:
            return (
                row.identity != identity if identity else False,
                not row.complete,
                row.size or 0,
                row.identity,
            )

        if exact := self.exact.get(rva):
            return min(exact, key=rank)
        candidates = [
            row for row in self.pages.get(rva >> 12, ())
            if row.end is not None and row.rva <= rva < row.end
        ]
        return min(candidates, key=rank, default=None)

    def evidence_owner(self, rva: int, hint: str = "") -> Datum | None:
        """Associate an interior xref with an extentless start for evidence.

        This does not make the datum complete: the bounded window still ends
        at the next known start and remains EXTENTLESS in every result.
        """
        if owner := self.owner(rva, hint):
            return owner
        section = self.image.section_at(rva)
        if section is None:
            return None
        starts = self.starts_by_section.get(section.name, ())
        index = bisect.bisect_right(starts, rva) - 1
        if index < 0:
            return None
        start = starts[index]
        candidates = [row for row in self.exact[start] if not row.complete]
        identity = self._hint_identity(hint)
        return min(
            candidates,
            key=lambda row: (
                row.identity != identity if identity else False,
                row.identity,
            ),
            default=None,
        )

    def next_start(self, rva: int) -> int | None:
        section = self.image.section_at(rva)
        if section is None:
            return None
        starts = self.starts_by_section.get(section.name, ())
        index = bisect.bisect_right(starts, rva)
        return starts[index] if index < len(starts) else None

    def resolve_all(self, rva: int) -> frozenset[str]:
        datum = self.evidence_owner(rva)
        if datum is not None:
            return frozenset((f"{datum.identity}+{rva - datum.rva:#x}",))
        if functions := self.function_identities.get(rva):
            return functions
        section = self.image.section_at(rva)
        return frozenset((
            f"S:{section.name}+{rva - section.rva:#x}"
            if section is not None else f"RVA:{rva:#x}",
        ))


class _RangeIndex:
    def __init__(self, records: list[dict]):
        self.records = sorted(
            {
                (row["rva"], row["size"]): row
                for row in records
                if isinstance(row.get("rva"), int)
                and isinstance(row.get("size"), int)
                and row["size"] > 0
            }.values(),
            key=lambda row: row["rva"],
        )
        self.starts = [row["rva"] for row in self.records]
        maximum = 0
        self.prefix_end = []
        for row in self.records:
            maximum = max(maximum, row["rva"] + row["size"])
            self.prefix_end.append(maximum)

    def contains(self, rva: int) -> bool:
        index = bisect.bisect_right(self.starts, rva) - 1
        while index >= 0 and self.prefix_end[index] > rva:
            row = self.records[index]
            if rva < row["rva"] + row["size"]:
                return True
            index -= 1
        return False


def _read_tsv(path: Path) -> list[dict[str, str]]:
    with path.open(newline="", encoding="utf-8") as source:
        return list(csv.DictReader(source, delimiter="\t"))


def _clean(value: object) -> str:
    return str(value).replace("\t", "\\t").replace("\r", "\\r").replace("\n", "\\n")


def _hex(value: int | None) -> str:
    return "-" if value is None else f"{value:#x}"


def _size(value: int | None) -> str:
    return "-" if value is None else f"{value:#x}"


def _storage(image: PEImage, rva: int, datum: Datum | None = None) -> str:
    if datum is not None and datum.storage in {"rdata", "data", "bss"}:
        return datum.storage
    section = image.section_at(rva)
    if section is None:
        return "-"
    if section.name == ".rdata":
        return "rdata"
    if section.name == ".data":
        return "data" if rva < section.rva + section.raw_size else "bss"
    return section.name.lstrip(".")


def _module_records(side: str, pairs: sema_pairing.Pairing,
                    ledger: dict[str, dict], module: str) -> list[dict]:
    records = pairs.target_records if side == "target" else pairs.base_records
    return [
        row for row in records
        if (sema_pairing.ledger_row(row, ledger) or {}).get("module") == module
    ]


def _owner_records(rows: list[dict]) -> dict[tuple[str, str], list[dict]]:
    result: dict[tuple[str, str], list[dict]] = defaultdict(list)
    by_name: dict[str, list[dict]] = defaultdict(list)
    for row in rows:
        mangled = row.get("mangled")
        if not mangled:
            continue
        result[(mangled, row.get("file") or "-")].append(row)
        by_name[mangled].append(row)
    for mangled, group in by_name.items():
        result[(mangled, "*")] = group
    return result


def _load_sites(side: str, pairs: sema_pairing.Pairing,
                ledger: dict[str, dict], module: str) -> dict[int, Site]:
    records = _module_records(side, pairs, ledger, module)
    owners = _owner_records(records)
    path = paths.DATA_TARGET_ACCESS if side == "target" else paths.DATA_BASE_ACCESS
    grouped: dict[int, set[Access]] = defaultdict(set)
    for raw in _read_tsv(path):
        instruction = int(raw["instruction_rva"], 0)
        candidates = owners.get((raw["caller_mangled"], raw["caller_file"]), ())
        if not candidates:
            candidates = owners.get((raw["caller_mangled"], "*"), ())
        containing = [
            row for row in candidates
            if row["rva"] <= instruction < row["rva"] + row["size"]
        ]
        for owner in containing:
            ledger_row = sema_pairing.ledger_row(owner, ledger)
            if not ledger_row or ledger_row.get("module") != module:
                continue
            site = int(raw["site_rva"], 0)
            grouped[site].add(Access(
                site=site,
                instruction=instruction,
                target=int(raw["target_rva"], 0),
                access=raw["access"],
                width=raw["width"],
                form=raw["form"],
                scale=int(raw["scale"], 0),
                identity=raw["target_identity"],
                instruction_text=raw["instruction"],
                function=owner.get("mangled") or raw["caller_mangled"],
                unit=ledger_row.get("unit") or owner.get("file") or raw["caller_file"],
                function_rva=owner["rva"],
                function_size=owner["size"],
                partner_rva=pairs.partner_rva(side, owner["rva"]),
            ))
    return {
        site: Site(site, tuple(sorted(accesses, key=lambda row: (
            row.function_rva, row.function, row.unit, row.instruction,
        ))))
        for site, accesses in grouped.items()
    }


def _expected_sites(side: str, image: PEImage, pairs: sema_pairing.Pairing,
                    ledger: dict[str, dict], module: str) -> set[int]:
    functions = _RangeIndex(_module_records(side, pairs, ledger, module))
    text = image.section(".text")
    result = set()
    for site in image.base_relocations():
        if not text.contains(site, 4) or not functions.contains(site):
            continue
        value = image.u32_rva(site)
        target = value - image.image_base if value >= image.image_base else value
        section = image.section_at(target)
        if section is not None and section.name in {".rdata", ".data"}:
            result.add(site)
    return result


def _accept_unique(candidates: dict[int, set[int]], used_target: set[int],
                   used_base: set[int]) -> dict[int, int]:
    reverse: dict[int, set[int]] = defaultdict(set)
    for target, bases in candidates.items():
        if target in used_target:
            continue
        for base in bases - used_base:
            reverse[base].add(target)
    accepted = {}
    for target, bases in sorted(candidates.items()):
        available = bases - used_base
        if (
            target not in used_target
            and len(available) == 1
            and len(reverse[next(iter(available))]) == 1
        ):
            accepted[target] = next(iter(available))
    return accepted


def _function_fingerprint_pairs(
    target: dict[int, Site], base: dict[int, Site]
) -> tuple[dict[int, int], dict[int, int]]:
    """Recover duplicate-name function pairs from complete data-use evidence.

    The ordinary function pairing has one key per mangled spelling.  LTCG can
    nevertheless retain multiple static functions with that spelling, so one
    copy gets paired and the others do not.  Worse, the chosen copies can be
    crossed even when the complete relocation evidence identifies the natural
    bijection (for example, two same-named helpers using different strings).

    Admit only a fingerprint present at exactly one function RVA on each side.
    Ambiguous duplicate bodies remain unpaired; this pass never guesses.
    """

    def collect(sites: dict[int, Site]) -> dict[tuple, set[int]]:
        rows: dict[int, set[tuple]] = defaultdict(set)
        names: dict[int, set[str]] = defaultdict(set)
        for site in sites.values():
            for access in site.accesses:
                rows[access.function_rva].add((
                    access.function_offset, access.token, access.identity,
                ))
                names[access.function_rva].add(access.function)
        result: dict[tuple, set[int]] = defaultdict(set)
        for function_rva, fingerprint in rows.items():
            if len(names[function_rva]) != 1:
                continue
            result[(next(iter(names[function_rva])), tuple(sorted(fingerprint)))].add(
                function_rva
            )
        return result

    target_fingerprints = collect(target)
    base_fingerprints = collect(base)
    target_to_base = {}
    for fingerprint in sorted(set(target_fingerprints) & set(base_fingerprints)):
        target_rvas = target_fingerprints[fingerprint]
        base_rvas = base_fingerprints[fingerprint]
        if len(target_rvas) == 1 and len(base_rvas) == 1:
            target_to_base[next(iter(target_rvas))] = next(iter(base_rvas))
    base_to_target = {base_rva: target_rva for target_rva, base_rva in target_to_base.items()}
    return target_to_base, base_to_target


def _pair_sites(target: dict[int, Site], base: dict[int, Site]
                ) -> tuple[dict[int, tuple[int, str]], set[int]]:
    pairs: dict[int, tuple[int, str]] = {}
    used_base: set[int] = set()
    fingerprint_target, fingerprint_base = _function_fingerprint_pairs(target, base)

    def target_partner(access: Access) -> int | None:
        return fingerprint_target.get(access.function_rva, access.partner_rva)

    def base_partner(access: Access) -> int | None:
        return fingerprint_base.get(access.function_rva, access.partner_rva)

    exact: dict[int, set[int]] = defaultdict(set)
    for target_rva, site in target.items():
        for access in site.accesses:
            partner_rva = target_partner(access)
            if partner_rva is None:
                continue
            candidate = partner_rva + access.function_offset
            base_site = base.get(candidate)
            if base_site is not None and any(
                row.function_rva == partner_rva for row in base_site.accesses
            ):
                exact[target_rva].add(candidate)
    accepted = _accept_unique(exact, set(), used_base)
    for target_rva, base_rva in accepted.items():
        fingerprint_pair = any(
            fingerprint_target.get(access.function_rva) == base_rva - access.function_offset
            and fingerprint_target[access.function_rva] != access.partner_rva
            for access in target[target_rva].accesses
        )
        pairs[target_rva] = (
            base_rva,
            "function-fingerprint" if fingerprint_pair else "function-offset",
        )
        used_base.add(base_rva)

    by_function_target: dict[tuple[int, int], set[int]] = defaultdict(set)
    by_function_base: dict[tuple[int, int], set[int]] = defaultdict(set)
    for site_rva, site in target.items():
        if site_rva in pairs:
            continue
        for access in site.accesses:
            partner_rva = target_partner(access)
            if partner_rva is not None:
                by_function_target[(access.function_rva, partner_rva)].add(site_rva)
    for site_rva, site in base.items():
        if site_rva in used_base:
            continue
        for access in site.accesses:
            partner_rva = base_partner(access)
            if partner_rva is not None:
                by_function_base[(partner_rva, access.function_rva)].add(site_rva)

    sequence: dict[int, set[int]] = defaultdict(set)
    for function_pair in sorted(set(by_function_target) & set(by_function_base)):
        target_function, base_function = function_pair
        target_sites = sorted(
            by_function_target[function_pair],
            key=lambda rva: target[rva].primary(target_function).function_offset,
        )
        base_sites = sorted(
            by_function_base[function_pair],
            key=lambda rva: base[rva].primary(base_function).function_offset,
        )
        target_tokens = [target[rva].primary(target_function).token for rva in target_sites]
        base_tokens = [base[rva].primary(base_function).token for rva in base_sites]
        matcher = SequenceMatcher(None, target_tokens, base_tokens, autojunk=False)
        for block in matcher.get_matching_blocks():
            for offset in range(block.size):
                sequence[target_sites[block.a + offset]].add(
                    base_sites[block.b + offset]
                )
    accepted = _accept_unique(sequence, set(pairs), used_base)
    for target_rva, base_rva in accepted.items():
        pairs[target_rva] = (base_rva, "access-sequence")
        used_base.add(base_rva)
    return pairs, used_base


def _record_function_identities(record: dict) -> frozenset[str]:
    identities = set()
    mangled = record.get("mangled")
    if mangled:
        identities.add(f"M:{mangled}")
    name = record.get("name")
    if name:
        # Rich PDB records can expose several folded functions at one RVA while
        # repeating one representative mangled spelling for the whole cluster.
        # The demangled record names preserve the other valid identities.
        identities.add(f"D:{name}")
    return frozenset(identities)


def _function_identities(side: str, pairs: sema_pairing.Pairing
                         ) -> dict[int, frozenset[str]]:
    records = pairs.target_records if side == "target" else pairs.base_records
    result: dict[int, set[str]] = defaultdict(set)
    for record in records:
        rva = record.get("rva")
        if isinstance(rva, int):
            result[rva].update(_record_function_identities(record))
    pair_index = 0 if side == "target" else 1
    for key, pair in pairs.pairs.items():
        result[pair[pair_index]].add(f"P:{key}")
    return {rva: frozenset(names) for rva, names in result.items()}


def _datum_index(side: str, image: PEImage,
                 pairs: sema_pairing.Pairing) -> DatumIndex:
    pdb_symbols = load(
        paths.DATA_TARGET_INDEX if side == "target" else paths.DATA_BASE_INDEX
    )
    overrides = {
        row.rva: row for row in missing.load_pdb_extents()
    } if side == "target" else {}
    datums = []
    for symbol in pdb_symbols:
        extent = overrides.get(symbol.rva)
        datums.append(Datum(
            rva=symbol.rva,
            size=extent.size if extent is not None else symbol.size,
            section=symbol.section,
            storage=symbol.storage,
            identity=symbol.identity,
            name=symbol.display_name,
            type_name=(
                f"cv:{symbol.type_index:#x}"
                if symbol.type_index else "PDB public-only"
            ),
            source="pdb-extent" if extent is not None else "pdb",
            comment=extent.comment if extent is not None else "",
        ))
    if side == "target":
        for symbol in missing.load_symbols():
            section = image.section_at(symbol.rva)
            if section is None:
                continue
            datums.append(Datum(
                rva=symbol.rva,
                size=symbol.size,
                section=section.name,
                storage=_storage(image, symbol.rva),
                identity=f"N:{symbol.name}",
                name=symbol.name,
                type_name=symbol.type_name,
                source="non-pdb",
                comment=symbol.comment,
            ))
    return DatumIndex(datums, image, _function_identities(side, pairs))


def _reference(datum: Datum | None, rva: int, fallback: str) -> str:
    return f"{datum.identity}+{rva - datum.rva:#x}" if datum is not None else fallback


def _window(image: PEImage, start: int, size: int,
            datum_index: DatumIndex) -> tuple[
                bytes, bytes, tuple[int, ...], tuple[frozenset[str], ...]
            ]:
    raw = image.read_rva(start, size)
    normalized = bytearray(raw)
    offsets = []
    targets = []
    begin = bisect.bisect_left(datum_index.relocations, start)
    end = bisect.bisect_left(datum_index.relocations, start + size)
    for site in datum_index.relocations[begin:end]:
        if site + 4 > start + size:
            continue
        offset = site - start
        value = image.u32_rva(site)
        destination = value - image.image_base if value >= image.image_base else value
        offsets.append(offset)
        if start <= destination < start + size:
            targets.append(frozenset((f"SELF+{destination - start:#x}",)))
        else:
            targets.append(datum_index.resolve_all(destination))
        normalized[offset:offset + 4] = bytes(4)
    return raw, bytes(normalized), tuple(offsets), tuple(targets)


def _relocation_targets_match(
    target_offsets: tuple[int, ...],
    target_targets: tuple[frozenset[str], ...],
    base_offsets: tuple[int, ...],
    base_targets: tuple[frozenset[str], ...],
) -> bool:
    return (
        target_offsets == base_offsets
        and all(left & right for left, right in zip(target_targets, base_targets))
    )


def _relocation_target_differences(
    target_offsets: tuple[int, ...],
    target_targets: tuple[frozenset[str], ...],
    base_offsets: tuple[int, ...],
    base_targets: tuple[frozenset[str], ...],
) -> list[tuple[int, frozenset[str], frozenset[str]]]:
    target_by_offset = dict(zip(target_offsets, target_targets))
    base_by_offset = dict(zip(base_offsets, base_targets))
    result = []
    for offset in sorted(target_by_offset.keys() | base_by_offset.keys()):
        left = target_by_offset.get(offset, frozenset())
        right = base_by_offset.get(offset, frozenset())
        if not left & right:
            result.append((offset, left, right))
    return result


def _first_diff(target: bytes, base: bytes) -> str:
    for offset, (left, right) in enumerate(zip(target, base)):
        if left != right:
            return f"{offset:#x}:{left:02x}!={right:02x}"
    if len(target) != len(base):
        return f"{min(len(target), len(base)):#x}:length"
    return "-"


def _compare_window(target_image: PEImage, base_image: PEImage,
                    target_index: DatumIndex, base_index: DatumIndex,
                    target_start: int, base_start: int, size: int) -> dict[str, str]:
    base_section = base_image.section_at(base_start)
    if base_section is None or not base_section.contains(base_start, size):
        return {
            "status": "BASE_WINDOW_MISSING", "raw": "-", "normalized": "-",
            "layout": "-", "targets": "-", "first_diff": "-",
            "target_sha256": "-", "base_sha256": "-",
        }
    target_raw, target_normalized, target_offsets, target_targets = _window(
        target_image, target_start, size, target_index
    )
    base_raw, base_normalized, base_offsets, base_targets = _window(
        base_image, base_start, size, base_index
    )
    raw_status = "EXACT" if target_raw == base_raw else "DIFF"
    normalized_status = "EXACT" if target_normalized == base_normalized else "DIFF"
    layout_status = "EXACT" if target_offsets == base_offsets else "DIFF"
    targets_status = (
        "EXACT" if _relocation_targets_match(
            target_offsets, target_targets, base_offsets, base_targets
        ) else "DIFF"
    )
    if normalized_status != "EXACT":
        status = "BYTES"
    elif layout_status != "EXACT":
        status = "RELOC_LAYOUT"
    elif targets_status != "EXACT":
        status = "RELOC_TARGETS"
    else:
        status = "EXACT"
    return {
        "status": status,
        "raw": raw_status,
        "normalized": normalized_status,
        "layout": layout_status,
        "targets": targets_status,
        "first_diff": _first_diff(target_normalized, base_normalized),
        "target_sha256": hashlib.sha256(target_normalized).hexdigest(),
        "base_sha256": hashlib.sha256(base_normalized).hexdigest(),
    }


def _access_pair(target: Site | None, base: Site | None) -> tuple[Access | None, Access | None]:
    if target is None:
        return None, base.primary() if base is not None else None
    if base is None:
        return target.primary(), None
    candidates = []
    for left in target.accesses:
        for right in base.accesses:
            candidates.append((
                (
                    left.partner_rva != right.function_rva,
                    right.partner_rva != left.function_rva,
                    left.token != right.token,
                    abs(left.function_offset - right.function_offset),
                    left.function_size + right.function_size,
                    left.function,
                ),
                left,
                right,
            ))
    _, left, right = min(candidates, key=lambda row: row[0])
    return left, right


def _audit_one(target: Site | None, base: Site | None, pair_kind: str,
               target_image: PEImage, base_image: PEImage,
               target_index: DatumIndex, base_index: DatumIndex,
               comparison_cache: dict[tuple[int, int, int], dict[str, str]],
               ) -> dict[str, str]:
    left, right = _access_pair(target, base)
    target_datum = (
        target_index.evidence_owner(left.target, left.identity)
        if left is not None else None
    )
    base_datum = (
        base_index.evidence_owner(right.target, right.identity)
        if right is not None else None
    )
    target_addend = left.target - target_datum.rva if left and target_datum else None
    base_addend = right.target - base_datum.rva if right and base_datum else None
    inferred_base = (
        right.target - target_addend
        if right is not None and target_addend is not None else None
    )
    target_reference = (
        _reference(target_datum, left.target, left.identity) if left is not None else "-"
    )
    base_reference = (
        _reference(base_datum, right.target, right.identity) if right is not None else "-"
    )

    if target_datum is None:
        extent_status = "UNOWNED"
        comparison = {key: "-" for key in (
            "status", "raw", "normalized", "layout", "targets", "first_diff",
        )}
        comparison["status"] = "NO_TARGET_OWNER"
    elif not target_datum.complete:
        extent_status = "EXTENTLESS"
        comparison = {key: "-" for key in (
            "raw", "normalized", "layout", "targets", "first_diff",
        )}
        comparison["status"] = "EXTENTLESS"
    elif inferred_base is None:
        extent_status = "COMPLETE"
        comparison = {key: "-" for key in (
            "raw", "normalized", "layout", "targets", "first_diff",
        )}
        comparison["status"] = "NO_BASE_RELOC"
    else:
        extent_status = "COMPLETE"
        assert target_datum.size is not None
        cache_key = (target_datum.rva, inferred_base, target_datum.size)
        comparison = comparison_cache.get(cache_key)
        if comparison is None:
            comparison = _compare_window(
                target_image, base_image, target_index, base_index,
                target_datum.rva, inferred_base, target_datum.size,
            )
            comparison_cache[cache_key] = comparison
        if (
            base_datum is not None
            and base_datum.identity == target_datum.identity
            and base_datum.size is not None
            and base_datum.size != target_datum.size
        ):
            comparison["status"] = "SIZE"

    access_status = (
        "-" if left is None or right is None
        else "EXACT" if left.token == right.token else "DIFF"
    )
    identity_status = (
        "-" if left is None or right is None
        else "EXACT" if target_reference == base_reference else "DIFF"
    )
    target_section = target_image.section_at(left.target) if left else None
    base_section = base_image.section_at(right.target) if right else None
    note = []
    if target is not None and len(target.accesses) > 1:
        note.append(f"target aliases={len(target.accesses)}")
    if base is not None and len(base.accesses) > 1:
        note.append(f"base aliases={len(base.accesses)}")
    if pair_kind == "access-sequence":
        note.append("site offset moved; paired by decoded access sequence")

    return {
        "pair_status": (
            "PAIRED" if target is not None and base is not None
            else "TARGET_ONLY" if target is not None else "BASE_ONLY"
        ),
        "pair_kind": pair_kind,
        "target_site_rva": _hex(target.rva if target else None),
        "base_site_rva": _hex(base.rva if base else None),
        "function": left.function if left else right.function if right else "-",
        "unit": left.unit if left else right.unit if right else "-",
        "target_function_rva": _hex(left.function_rva if left else None),
        "base_function_rva": _hex(right.function_rva if right else None),
        "target_function_offset": _hex(left.function_offset if left else None),
        "base_function_offset": _hex(right.function_offset if right else None),
        "target_instruction_rva": _hex(left.instruction if left else None),
        "base_instruction_rva": _hex(right.instruction if right else None),
        "target_access": left.access if left else "-",
        "base_access": right.access if right else "-",
        "target_width": left.width if left else "-",
        "base_width": right.width if right else "-",
        "target_form": left.form if left else "-",
        "base_form": right.form if right else "-",
        "target_scale": str(left.scale) if left else "-",
        "base_scale": str(right.scale) if right else "-",
        "access_status": access_status,
        "target_target_rva": _hex(left.target if left else None),
        "base_target_rva": _hex(right.target if right else None),
        "target_section": target_section.name if target_section else "-",
        "base_section": base_section.name if base_section else "-",
        "target_storage": _storage(target_image, left.target, target_datum) if left else "-",
        "base_storage": _storage(base_image, right.target, base_datum) if right else "-",
        "target_identity": target_reference,
        "base_identity": base_reference,
        "identity_status": identity_status,
        "target_datum_rva": _hex(target_datum.rva if target_datum else None),
        "base_datum_rva": _hex(base_datum.rva if base_datum else None),
        "inferred_base_datum_rva": _hex(inferred_base),
        "target_addend": _hex(target_addend),
        "base_addend": _hex(base_addend),
        "target_size": _size(target_datum.size if target_datum else None),
        "base_size": _size(base_datum.size if base_datum else None),
        "extent_status": extent_status,
        "datum_status": comparison["status"],
        "normalized_bytes_status": comparison["normalized"],
        "reloc_layout_status": comparison["layout"],
        "reloc_targets_status": comparison["targets"],
        "first_diff": comparison["first_diff"],
        "note": "; ".join(note) or "-",
    }


def _pattern(data: bytes) -> str:
    if not data:
        return "empty"
    if not any(data):
        return f"zero[{len(data)}]"
    for period in (1, 2, 4, 8, 12, 16, 20, 24, 32, 48, 64):
        if len(data) >= period * 2 and all(
            data[index] == data[index % period] for index in range(len(data))
        ):
            return f"repeat[{period}]x{len(data) // period}"
    printable = sum(32 <= value < 127 or value in (9, 10, 13, 0) for value in data)
    if printable / len(data) >= 0.9:
        preview = data[:64].split(b"\0", 1)[0].decode("ascii", "backslashreplace")
        return f"ascii:{_clean(preview)}"
    if len(data) >= 8 and len(data) % 4 == 0:
        floats = struct.unpack(f"<{min(len(data) // 4, 8)}f", data[:32])
        if all(math.isfinite(value) and abs(value) < 1e10 for value in floats):
            return "f32:" + ",".join(f"{value:.6g}" for value in floats)
        words = struct.unpack(f"<{min(len(data) // 4, 8)}I", data[:32])
        return "u32:" + ",".join(f"{value:#x}" for value in words)
    return "bytes"


def _preview(data: bytes, size: int = 64) -> str:
    return data[:size].hex()


def _access_summary(rows: list[dict[str, str]]) -> str:
    counts = Counter(
        (row["target_addend"], row["target_access"], row["target_width"],
         row["target_form"], row["target_scale"])
        for row in rows
    )
    return ";".join(
        f"{offset}:{access}/{width}/{form}*{scale}x{count}"
        for (offset, access, width, form, scale), count in sorted(counts.items())
    )


def _extentless_end(datum: Datum, index: DatumIndex) -> int:
    section = index.image.section_at(datum.rva)
    assert section is not None
    next_start = index.next_start(datum.rva) or section.end
    end = min(next_start, section.end)
    real = re.fullmatch(r"E:__real@([0-9A-Fa-f]+)", datum.identity)
    if real and len(real.group(1)) in {8, 16}:
        end = min(end, datum.rva + len(real.group(1)) // 2)
    elif re.fullmatch(r"E:__xmm@[0-9A-Fa-f]{32}", datum.identity):
        end = min(end, datum.rva + 16)
    elif datum.identity.startswith("E:??_B"):
        # MSVC's local static initialization guard is a 32-bit word even when
        # the fast path reads only its low byte.
        end = min(end, datum.rva + 4)
    if datum.identity.startswith("E:??_C@_0"):
        # `_0` string publics are narrow literals.  Their PDB public has no
        # trusted extent, but the first terminator is a semantic boundary;
        # alignment padding and a following allocation are not literal data.
        raw = index.image.read_rva(datum.rva, end - datum.rva)
        terminator = raw.find(b"\0")
        if terminator >= 0:
            end = datum.rva + terminator + 1
    if (
        datum.identity.startswith("E:??_7")
        and end - datum.rva >= 8
        and end - 4 in index.relocations
    ):
        # An MSVC vftable public points at its first virtual slot.  The RTTI
        # complete-object locator for the next table is stored at
        # next_vftable-4 and is not part of this symbol's table.
        end -= 4
    return end


def _classify_extentless_comparison(
    datum: Datum,
    candidate_identity_status: str,
    comparison: dict[str, str],
) -> str:
    if (
        candidate_identity_status == "EXACT"
        and datum.identity.startswith("E:__imp__")
    ):
        # An import slot contains a link-layout-dependent address, not a source
        # initializer.  Preserve its raw comparison in the dossier.
        return "LINKER_VALUE"
    if (
        candidate_identity_status == "DIFF"
        and datum.identity.startswith(("E:", "L:"))
    ):
        # The selected window belongs to another named allocation.  Its bytes
        # diagnose a wrong referent (or site pairing), not this initializer.
        return "REFERENT_DIFF"
    if (
        candidate_identity_status == "EXACT"
        and datum.identity.startswith("E:??_7")
        and comparison["status"] == "RELOC_TARGETS"
        and comparison["normalized"] == "EXACT"
        and comparison["layout"] == "EXACT"
    ):
        # Code matching and PDB virtual-order verification own this residual.
        # Every differing slot is still printed in the extentless dossier.
        return "VTABLE_CODE_TARGETS"
    return comparison["status"]


def _select_candidate_vote(
    votes: Counter[int], base_index: DatumIndex, identity: str
) -> int | None:
    if not votes:
        return None
    ranked = votes.most_common()
    if len(ranked) == 1:
        return ranked[0][0]
    exact = [
        (candidate, count) for candidate, count in ranked
        if (datum := base_index.evidence_owner(candidate)) is not None
        and datum.identity == identity
    ]
    if len(exact) == 1:
        return exact[0][0]
    if len(exact) > 1 and exact[0][1] != exact[1][1]:
        return exact[0][0]
    if not exact and ranked[0][1] != ranked[1][1]:
        return ranked[0][0]
    return None


def _select_extentless_candidate(
    votes: Counter[int], base_index: DatumIndex, identity: str
) -> tuple[int | None, bool]:
    identity_starts = sorted({
        row.rva for row in base_index.by_identity.get(identity, ())
    })
    if len(identity_starts) == 1:
        candidate = identity_starts[0]
        return candidate, candidate not in votes
    return _select_candidate_vote(votes, base_index, identity), False


def _extentless_rows(audit: list[dict[str, str]], target_index: DatumIndex,
                     base_index: DatumIndex, target_image: PEImage,
                     base_image: PEImage) -> list[dict[str, str]]:
    anchors = {}
    for row in audit:
        if row["extent_status"] != "EXTENTLESS":
            continue
        rva = int(row["target_datum_rva"], 0)
        datum = target_index.owner(rva, row["target_identity"])
        if datum is not None:
            anchors[rva] = datum
    target_destinations = sorted({
        int(row["target_target_rva"], 0)
        for row in audit if row["target_target_rva"] != "-"
    })
    data_relocs = sorted(
        site for site in target_index.relocations
        if target_image.section_at(site) is not None
        and target_image.section_at(site).name in {".rdata", ".data"}
    )
    result = []
    for anchor, datum in sorted(anchors.items()):
        section = target_image.section_at(anchor)
        assert section is not None
        next_known = target_index.next_start(anchor) or section.end
        end = _extentless_end(datum, target_index)
        evidence_size = end - anchor
        xrefs = [
            row for row in audit
            if row["target_target_rva"] != "-"
            and anchor <= int(row["target_target_rva"], 0) < end
        ]
        next_ref_index = bisect.bisect_right(target_destinations, anchor)
        next_referent = (
            target_destinations[next_ref_index]
            if next_ref_index < len(target_destinations)
            and target_image.section_at(target_destinations[next_ref_index]) == section
            else None
        )
        next_reloc_index = bisect.bisect_right(data_relocs, anchor)
        next_data_reloc = (
            data_relocs[next_reloc_index]
            if next_reloc_index < len(data_relocs)
            and target_image.section_at(data_relocs[next_reloc_index]) == section
            else None
        )
        votes = Counter()
        for row in xrefs:
            if row["base_target_rva"] == "-":
                continue
            target_rva = int(row["target_target_rva"], 0)
            base_rva = int(row["base_target_rva"], 0)
            votes[base_rva - (target_rva - anchor)] += 1
        candidate_base, identity_fallback = _select_extentless_candidate(
            votes, base_index, datum.identity
        )
        retail = target_image.read_rva(anchor, evidence_size)
        comparison = {
            "status": "NO_BASE_VOTE" if not votes else "CONFLICT",
            "raw": "-", "normalized": "-", "layout": "-", "targets": "-",
            "first_diff": "-", "target_sha256": hashlib.sha256(retail).hexdigest(),
            "base_sha256": "-",
        }
        base_bytes = b""
        base_next_delta = None
        candidate_identity = "-"
        candidate_identity_status = "-"
        if len(votes) > 1:
            candidates = []
            for base_start in votes:
                candidate_datum = base_index.evidence_owner(base_start)
                candidate_comparison = _compare_window(
                    target_image, base_image, target_index, base_index,
                    anchor, base_start, evidence_size,
                )
                candidates.append(
                    (base_start, candidate_datum, candidate_comparison)
                )
            if all(
                candidate_datum is not None
                and candidate_datum.identity == datum.identity
                and candidate_comparison["status"] == "EXACT"
                for _, candidate_datum, candidate_comparison in candidates
            ):
                base_start, _, exemplar = candidates[0]
                comparison = dict(exemplar)
                comparison["status"] = "MULTI_BASE_EXACT"
                candidate_identity = datum.identity
                candidate_identity_status = "EXACT"
                base_bytes = base_image.read_rva(base_start, evidence_size)
        if candidate_base is not None:
            candidate_datum = base_index.evidence_owner(candidate_base)
            if candidate_datum is not None:
                candidate_identity = candidate_datum.identity
                candidate_identity_status = (
                    "EXACT" if candidate_identity == datum.identity else "DIFF"
                )
            else:
                candidate_identity_status = "UNOWNED"
            comparison = _compare_window(
                target_image, base_image, target_index, base_index,
                anchor, candidate_base, evidence_size,
            )
            base_section = base_image.section_at(candidate_base)
            if base_section is not None and base_section.contains(
                candidate_base, evidence_size
            ):
                base_bytes = base_image.read_rva(candidate_base, evidence_size)
            base_next_delta = (
                _extentless_end(candidate_datum, base_index) - candidate_base
                if candidate_datum is not None else None
            )
            if base_next_delta is not None and base_next_delta != evidence_size:
                comparison["status"] = "BOUNDARY_DIFF"
            comparison["status"] = _classify_extentless_comparison(
                datum, candidate_identity_status, comparison
            )
            if identity_fallback and comparison["status"] == "EXACT":
                comparison["status"] = "IDENTITY_FALLBACK_EXACT"
            elif (
                len(votes) > 1
                and candidate_identity_status == "EXACT"
                and comparison["status"] == "EXACT"
            ):
                comparison["status"] = "EXACT_WITH_OUTLIERS"
        observed_end = max(
            (
                int(row["target_target_rva"], 0)
                + _WIDTHS.get(row["target_width"], 0)
                for row in xrefs
            ),
            default=anchor,
        )
        result.append({
            "target_rva": _hex(anchor),
            "identity": datum.identity,
            "name": datum.name,
            "type": datum.type_name,
            "section": section.name,
            "storage": _storage(target_image, anchor, datum),
            "xref_sites": str(len({row["target_site_rva"] for row in xrefs})),
            "referenced_offsets": ",".join(sorted({
                _hex(int(row["target_target_rva"], 0) - anchor) for row in xrefs
            }, key=lambda value: int(value, 0))) or "-",
            "accesses": _access_summary(xrefs) or "-",
            "observed_end_rva": _hex(observed_end),
            "next_referent_rva": _hex(next_referent),
            "next_referent_delta": _hex(
                next_referent - anchor if next_referent is not None else None
            ),
            "next_known_start_rva": _hex(next_known),
            "next_known_start_delta": _hex(next_known - anchor),
            "next_data_reloc_rva": _hex(next_data_reloc),
            "next_data_reloc_delta": _hex(
                next_data_reloc - anchor if next_data_reloc is not None else None
            ),
            "evidence_size": _size(evidence_size),
            "candidate_base_rva": _hex(candidate_base),
            "candidate_identity": candidate_identity,
            "candidate_identity_status": candidate_identity_status,
            "candidate_votes": ",".join(
                f"{rva:#x}:{count}" for rva, count in sorted(votes.items())
            ) or "-",
            "base_next_known_delta": _hex(base_next_delta),
            "compare_status": comparison["status"],
            "raw_status": comparison["raw"],
            "normalized_status": comparison["normalized"],
            "reloc_layout_status": comparison["layout"],
            "reloc_targets_status": comparison["targets"],
            "first_diff": comparison["first_diff"],
            "retail_pattern": _pattern(retail),
            "retail_sha256": comparison["target_sha256"],
            "base_sha256": comparison["base_sha256"],
            "retail_head": _preview(retail),
            "retail_tail": retail[-32:].hex() if len(retail) > 64 else "-",
            "base_head": _preview(base_bytes) if base_bytes else "-",
            "comment": datum.comment or "PDB start has no trusted complete extent",
        })
    return result


def _write_tsv(path: Path, columns: tuple[str, ...], rows: list[dict[str, str]]) -> None:
    lines = ["\t".join(columns)]
    for row in rows:
        lines.append("\t".join(_clean(row[column]) for column in columns))
    write_if_changed(path, "\n".join(lines) + "\n")


def _content_key(index: DatumIndex, datum: Datum) -> str | None:
    """Identify compiler-owned constants by their complete normalized bytes."""
    size = None
    if datum.complete and datum.identity.startswith(("L:", "N:")):
        size = datum.size
    elif datum.identity.startswith(("E:__real@", "E:__xmm@")):
        size = _extentless_end(datum, index) - datum.rva
    if not size:
        return None
    _, normalized, offsets, _ = _window(index.image, datum.rva, size, index)
    if offsets:
        return None
    digest = hashlib.sha256(normalized).hexdigest()
    preview = normalized[:16].hex()
    return f"C:{size:#x}:{preview}:{digest}"


def _datum_token(
    index: DatumIndex,
    rva: int,
    hint: str,
    counterpart: DatumIndex | None = None,
) -> str:
    datum = index.evidence_owner(rva, hint)
    if datum is None:
        return " | ".join(sorted(index.resolve_all(rva)))
    addend = rva - datum.rva
    key = _content_key(index, datum)
    counterpart_rows = (
        counterpart.by_identity.get(datum.identity, ())
        if counterpart is not None else ()
    )
    if key is not None and not (
        counterpart_rows
        and not any(_content_key(counterpart, row) for row in counterpart_rows)
    ):
        return f"{key}+{addend:#x}"
    return f"I:{datum.identity}+{addend:#x}"


def _content_keys(index: DatumIndex) -> set[str]:
    """Return address-independent keys for compiler-owned allocations."""
    result = set()
    seen = set()
    for datums in index.exact.values():
        for datum in datums:
            key = _content_key(index, datum)
            if key is None:
                continue
            allocation = (datum.rva, key)
            if allocation in seen:
                continue
            seen.add(allocation)
            result.add(key)
    return result


def _token_is_available(
    token: str, index: DatumIndex, content_keys: set[str],
) -> bool:
    """Check whether an allocation represented by ``token`` exists at all.

    This deliberately ignores whether the function currently references the
    allocation.  That distinction separates missing data definitions from
    ordinary code-use drift before the next expensive link.
    """
    if token.startswith("I:"):
        identity, addend_text = token[2:].rsplit("+", 1)
        addend = int(addend_text, 0)
        return any(
            addend >= 0 and (not datum.complete or addend <= (datum.size or 0))
            for datum in index.by_identity.get(identity, ())
        )
    if token.startswith("C:"):
        key, addend_text = token.rsplit("+", 1)
        return int(addend_text, 0) >= 0 and key in content_keys
    return False


def _function_data_rows(
    audit: list[dict[str, str]],
    target_index: DatumIndex,
    base_index: DatumIndex,
    ledger: dict[str, dict],
) -> list[dict[str, str]]:
    groups: dict[tuple[str, str], dict] = defaultdict(lambda: {
        "paired": False,
        "target_rvas": set(), "base_rvas": set(),
        "target": defaultdict(set), "base": defaultdict(set),
    })
    for row in audit:
        key = (row["unit"], row["function"])
        group = groups[key]
        group["paired"] |= row["pair_status"] == "PAIRED"
        if row["target_function_rva"] != "-":
            group["target_rvas"].add(row["target_function_rva"])
        if row["base_function_rva"] != "-":
            group["base_rvas"].add(row["base_function_rva"])
        if row["target_target_rva"] != "-":
            rva = int(row["target_target_rva"], 0)
            token = _datum_token(
                target_index, rva, row["target_identity"], base_index
            )
            group["target"][token].add(row["target_identity"])
        if row["base_target_rva"] != "-":
            rva = int(row["base_target_rva"], 0)
            token = _datum_token(
                base_index, rva, row["base_identity"], target_index
            )
            group["base"][token].add(row["base_identity"])

    target_content = _content_keys(target_index)
    base_content = _content_keys(base_index)

    # Exact complete windows also prove that an anonymous allocation exists on
    # the other side even when the candidate PDB has no transferable identity.
    # Keep this as existence evidence only; it does not make a function's wrong
    # referent set exact.
    target_proven_in_base = set()
    base_proven_in_target = set()
    for row in audit:
        if row["pair_status"] != "PAIRED" or row["datum_status"] != "EXACT":
            continue
        target_rva = int(row["target_target_rva"], 0)
        base_rva = int(row["base_target_rva"], 0)
        target_proven_in_base.add(_datum_token(
            target_index, target_rva, row["target_identity"], base_index
        ))
        base_proven_in_target.add(_datum_token(
            base_index, base_rva, row["base_identity"], target_index
        ))

    result = []
    for (unit, function), group in sorted(groups.items()):
        if not group["paired"]:
            continue
        target_tokens = set(group["target"])
        base_tokens = set(group["base"])
        missing = sorted(target_tokens - base_tokens)
        extra = sorted(base_tokens - target_tokens)
        missing_base_definitions = [
            token for token in missing
            if token not in target_proven_in_base
            and not _token_is_available(token, base_index, base_content)
        ]
        missing_target_definitions = [
            token for token in extra
            if token not in base_proven_in_target
            and not _token_is_available(token, target_index, target_content)
        ]

        def describe(side: str, tokens: list[str]) -> str:
            return ";".join(
                token + "<=" + "|".join(sorted(group[side][token]))
                for token in tokens
            ) or "-"

        if not missing and not extra:
            status = "EXACT"
        elif missing_base_definitions and missing_target_definitions:
            status = "DEFINITION_MISSING_BOTH"
        elif missing_base_definitions:
            status = "BASE_DEFINITION_MISSING"
        elif missing_target_definitions:
            status = "TARGET_DEFINITION_MISSING"
        else:
            status = "USE_DIFF"

        ledger_row = ledger.get(function, {})
        resolution = _function_data_resolution(status, ledger_row)

        result.append({
            "unit": unit,
            "function": function,
            "target_function_rvas": ",".join(sorted(
                group["target_rvas"], key=lambda value: int(value, 0)
            )) or "-",
            "base_function_rvas": ",".join(sorted(
                group["base_rvas"], key=lambda value: int(value, 0)
            )) or "-",
            "target_datum_count": str(len(target_tokens)),
            "base_datum_count": str(len(base_tokens)),
            "status": status,
            "resolution": resolution,
            "source_hash": ledger_row.get("hash") or "-",
            "ledger_status": ledger_row.get("status") or "-",
            "missing_target_datums": describe("target", missing),
            "extra_base_datums": describe("base", extra),
            "missing_base_definitions": describe(
                "target", missing_base_definitions
            ),
            "missing_target_definitions": describe(
                "base", missing_target_definitions
            ),
        })
    return result


def _function_data_resolution(
    raw_status: str, ledger_row: dict[str, object],
) -> str:
    """Separate raw relocation equality from hash-scoped byte proof.

    Direct datum ownership moves across function boundaries under LTCG.  A raw
    USE_DIFF therefore remains valuable evidence.  Only a byte-exact result for
    this exact source hash may close it automatically: ordinary parked ledger
    notes describe code-matching work and are not data-audit evidence.  This is
    deliberately conservative so an old generic wall cannot hide a newly
    exposed wrong value.
    """
    if raw_status == "EXACT":
        return "EXACT"
    maximum = ledger_row.get("max")
    if isinstance(maximum, (int, float)) and maximum >= 100:
        return "HASH_MAX_EXACT"
    return "OPEN"


def _problem_tags(row: dict[str, str]) -> list[str]:
    tags = []
    if row["pair_status"] != "PAIRED":
        tags.append(row["pair_status"])
    if row["access_status"] == "DIFF":
        tags.append("ACCESS")
    if row["identity_status"] == "DIFF":
        tags.append("REFERENT")
    # A base-only site has no retail referent to own or size.  Its only
    # problem is the extra candidate relocation; do not misreport the absent
    # retail half as an UNOWNED retail datum.
    if row["pair_status"] != "BASE_ONLY":
        if row["extent_status"] == "EXTENTLESS":
            tags.append("EXTENTLESS")
        elif row["extent_status"] == "UNOWNED":
            tags.append("UNOWNED")
        if row["datum_status"] not in {
            "EXACT", "EXTENTLESS", "NO_TARGET_OWNER"
        }:
            tags.append(row["datum_status"])
    return list(dict.fromkeys(tags))


def _markdown_cell(value: str) -> str:
    return value.replace("|", "\\|").replace("\n", " ")


def _spaced_hex(value: str) -> str:
    if value == "-":
        return value
    return " ".join(value[index:index + 2] for index in range(0, len(value), 2))


def _write_markdown(audit: list[dict[str, str]],
                    extentless: list[dict[str, str]],
                    function_data: list[dict[str, str]], report: dict,
                    target_image: PEImage, base_image: PEImage,
                    target_index: DatumIndex, base_index: DatumIndex,
                    module: str, artifacts: ModuleArtifacts) -> None:
    settled_extentless = {
        row["target_rva"] for row in extentless
        if row["compare_status"] in _SETTLED_EXTENTLESS
    }
    unresolved_extentless = [
        row for row in extentless
        if row["compare_status"] not in _SETTLED_EXTENTLESS
    ]
    problems = [(row, _problem_tags(row)) for row in audit]
    problems = [
        (row, [
            tag for tag in tags
            if not (
                tag == "EXTENTLESS"
                and row["target_datum_rva"] in settled_extentless
            )
        ])
        for row, tags in problems
    ]
    problems = [(row, tags) for row, tags in problems if tags]
    tag_counts = Counter(tag for _, tags in problems for tag in tags)
    complete_mismatches: dict[
        tuple[str, str, str, str], list[dict[str, str]]
    ] = defaultdict(list)
    for row in audit:
        if row["datum_status"] not in {
            "BYTES", "RELOC_LAYOUT", "RELOC_TARGETS", "SIZE",
            "BASE_WINDOW_MISSING",
        }:
            continue
        complete_mismatches[(
            row["target_datum_rva"], row["inferred_base_datum_rva"],
            row["target_size"], row["datum_status"],
        )].append(row)
    identity_aligned_complete = {
        key: rows for key, rows in complete_mismatches.items()
        if any(row["identity_status"] == "EXACT" for row in rows)
    }
    lines = [
        f"# {module} data relocation problems",
        "",
        f"_Generated by `python3 -m vostok data module-relocs {module}`. Every physical",
        f"retail and candidate {module} relocation into `.rdata`, `.data`, or the",
        "loader-zero BSS tail is represented exactly once in the machine audit.",
        "This file contains every problem found by that audit._",
        "",
        "## Measurement summary",
        "",
        f"- Retail relocation sites: {report['target_relocation_sites']:,}",
        f"- Candidate relocation sites: {report['base_relocation_sites']:,}",
        f"- Paired sites: {report['pair_status'].get('PAIRED', 0):,}",
        f"- Retail-only sites: {report['pair_status'].get('TARGET_ONLY', 0):,}",
        f"- Candidate-only sites: {report['pair_status'].get('BASE_ONLY', 0):,}",
        f"- Problematic audit rows: {len(problems):,}",
        f"- Unique mismatched complete data comparisons: "
        f"{len(complete_mismatches):,}",
        f"  - PDB identity-aligned source-data candidates: "
        f"{len(identity_aligned_complete):,}",
        f"  - Wrong-referent/pairing comparison windows: "
        f"{len(complete_mismatches) - len(identity_aligned_complete):,}",
        f"- Extentless allocation dossiers: {len(extentless):,}",
        f"- Paired functions with raw deduplicated datum-set differences: "
        f"{sum(count for status, count in report['function_data_status'].items() if status != 'EXACT'):,}",
        f"- Open function datum-use rows: "
        f"{report['function_data_resolution'].get('OPEN', 0):,}",
        "",
        "Pairing modes: " + ", ".join(
            f"`{name}`={count:,}" for name, count in report["pair_kind"].items()
        ),
        "",
        "Paired access shapes: " + ", ".join(
            f"`{name}`={count:,}" for name, count in report["access_status"].items()
        ),
        "",
        "Paired referents: " + ", ".join(
            f"`{name}`={count:,}" for name, count in report["identity_status"].items()
        ),
        "",
        "Complete/extent debt results: " + ", ".join(
            f"`{name}`={count:,}" for name, count in report["datum_status"].items()
        ),
        "",
        "Extentless evidence-window results: " + ", ".join(
            f"`{name}`={count:,}"
            for name, count in report["extentless_status"].items()
        ),
        "",
        "Problem tags: " + (", ".join(
            f"`{name}`={count:,}" for name, count in sorted(tag_counts.items())
        ) or "none"),
        "",
        "`EXTENTLESS` means the PDB supplies an identity and start RVA but no",
        "trustworthy complete size. Its section below records access evidence,",
        "the next referenced address, the next known data start, the next",
        "relocation cell in data, candidate-start votes, and actual byte patterns.",
        "",
        "`BYTES` is a normalized non-pointer byte mismatch. `RELOC_LAYOUT` is a",
        "different pointer-cell layout, and `RELOC_TARGETS` is a different",
        "resolved pointer target. `ACCESS` is a different decoded access shape;",
        "`REFERENT` is a different symbol identity or addend. `TARGET_ONLY` and",
        "`BASE_ONLY` are physical relocation count/placement differences.",
        "`VTABLE_CODE_TARGETS` preserves differing virtual-slot destinations",
        "for a same-identity vftable whose normalized bytes and slot layout",
        "match; code matching and PDB virtual-order verification own that debt.",
        "`LINKER_VALUE` is a link-layout-dependent import-slot value, while",
        "`REFERENT_DIFF` means the paired xref selected another named datum.",
        "Function-level `USE_DIFF` means all allocations exist on both images",
        "but the current function bodies reference different deduplicated sets.",
        "A `*_DEFINITION_MISSING` result is stronger: at least one allocation",
        "has neither a stable identity/content counterpart nor an exact complete",
        "window proved elsewhere by aligned relocation evidence.",
        "`HASH_MAX_EXACT` means this exact source-body hash previously emitted",
        "the retail body byte-for-byte. Ordinary parked code-matching notes do",
        "not close datum-use rows; every other raw difference remains `OPEN`",
        "until byte proof or dedicated data-audit evidence settles it.",
        "",
        "## Function-level datum-set differences",
        "",
    ]
    different_functions = [
        row for row in function_data if row["status"] != "EXACT"
    ]
    if different_functions:
        lines.extend((
            "Each paired function is compared after deduplicating its referents.",
            "Complete local or synthetic allocations without pointer cells pair",
            "by their whole normalized bytes, so folded constants do not depend",
            "on which TU supplied their symbol.",
            "",
            "| Class | Resolution | Unit | Function | Retail/base datums | Missing retail "
            "uses | Extra candidate uses | Missing definitions |",
            "|---|---|---|---|---:|---|---|---|",
        ))
        for row in different_functions:
            missing_definitions = "; ".join(
                value for value in (
                    row["missing_base_definitions"],
                    row["missing_target_definitions"],
                ) if value != "-"
            ) or "-"
            lines.append(
                "| `{}` | `{}` | `{}` | `{}` | `{}/{}` | `{}` | `{}` | `{}` |".format(
                    row["status"],
                    row["resolution"],
                    _markdown_cell(row["unit"]),
                    _markdown_cell(row["function"]),
                    row["target_datum_count"], row["base_datum_count"],
                    _markdown_cell(row["missing_target_datums"]),
                    _markdown_cell(row["extra_base_datums"]),
                    _markdown_cell(missing_definitions),
                )
            )
        lines.append("")
    else:
        lines.extend(("No paired function has a datum-set difference.", ""))
    lines.extend(("## Every problematic relocation", ""))
    by_unit: dict[str, list[tuple[dict[str, str], list[str]]]] = defaultdict(list)
    for row, tags in problems:
        by_unit[row["unit"]].append((row, tags))
    for unit in sorted(by_unit):
        lines.extend((
            f"### `{_markdown_cell(unit)}`",
            "",
            "| Problem | Function | Retail site → candidate site | Access | "
            "Retail referent → candidate referent | Datum result | First difference |",
            "|---|---|---|---|---|---|---|",
        ))
        for row, tags in sorted(by_unit[unit], key=lambda item: (
            int(item[0]["target_site_rva"], 0)
            if item[0]["target_site_rva"] != "-" else 0xFFFFFFFF,
            int(item[0]["base_site_rva"], 0)
            if item[0]["base_site_rva"] != "-" else 0xFFFFFFFF,
        )):
            access = (
                f"{row['target_access']}/{row['target_width']}/{row['target_form']}"
                f" → {row['base_access']}/{row['base_width']}/{row['base_form']}"
            )
            lines.append(
                "| {} | `{}` | `{}` → `{}` | `{}` | `{}` → `{}` | `{}` | `{}` |".format(
                    ", ".join(f"`{tag}`" for tag in tags),
                    _markdown_cell(row["function"]),
                    row["target_site_rva"], row["base_site_rva"], access,
                    _markdown_cell(row["target_identity"]),
                    _markdown_cell(row["base_identity"]), row["datum_status"],
                    row["first_diff"],
                )
            )
        lines.append("")

    lines.extend(("## Every identity-aligned mismatched complete datum", ""))
    for key, xrefs in sorted(
        identity_aligned_complete.items(), key=lambda item: int(item[0][0], 0)
    ):
        target_rva_text, base_rva_text, size_text, status = key
        target_rva = int(target_rva_text, 0)
        base_rva = int(base_rva_text, 0)
        size = int(size_text, 0)
        target_raw, target_normalized, target_offsets, target_targets = _window(
            target_image, target_rva, size, target_index
        )
        base_section = base_image.section_at(base_rva)
        if base_section is not None and base_section.contains(base_rva, size):
            base_raw, base_normalized, base_offsets, base_targets = _window(
                base_image, base_rva, size, base_index
            )
        else:
            base_raw = base_normalized = b""
            base_offsets = ()
            base_targets = ()
        identities = sorted({row["target_identity"] for row in xrefs})
        functions = sorted({row["function"] for row in xrefs})
        lines.extend((
            f"### `{target_rva_text}` → `{base_rva_text}` — "
            f"`{_markdown_cell(identities[0])}`",
            "",
            f"- Result: `{status}`; size: `{size_text}`; xrefs: "
            f"`{len(xrefs)}`; first normalized difference: "
            f"`{xrefs[0]['first_diff']}`",
            "- Evidence class: PDB identity-aligned source-data mismatch",
            f"- Retail pattern: `{_markdown_cell(_pattern(target_raw))}`; "
            f"candidate pattern: `{_markdown_cell(_pattern(base_raw))}`",
            f"- Retail relocation cells: "
            f"`{','.join(map(_hex, target_offsets)) or '-'}`",
            f"- Candidate relocation cells: "
            f"`{','.join(map(_hex, base_offsets)) or '-'}`",
            "- Referencing functions: " + ", ".join(
                f"`{_markdown_cell(function)}`" for function in functions
            ),
            "",
            "Retail raw bytes (head):",
            "",
            "```text",
            _spaced_hex(_preview(target_raw)),
            "```",
            "",
            "Candidate raw bytes (head):",
            "",
            "```text",
            _spaced_hex(_preview(base_raw)),
            "```",
        ))
        if len(target_raw) > 64:
            lines.extend((
                "", "Retail raw bytes (tail):", "", "```text",
                _spaced_hex(target_raw[-32:].hex()), "```",
            ))
        if len(base_raw) > 64:
            lines.extend((
                "", "Candidate raw bytes (tail):", "", "```text",
                _spaced_hex(base_raw[-32:].hex()), "```",
            ))
        if target_normalized != base_normalized:
            lines.extend((
                "", "Retail normalized bytes (head):", "", "```text",
                _spaced_hex(_preview(target_normalized)), "```",
                "", "Candidate normalized bytes (head):", "", "```text",
                _spaced_hex(_preview(base_normalized)), "```",
            ))
        relocation_differences = _relocation_target_differences(
            target_offsets, target_targets, base_offsets, base_targets
        )
        if relocation_differences:
            lines.extend((
                "",
                "Differing resolved relocations:",
                "",
                "| Offset | Retail identities | Candidate identities |",
                "|---:|---|---|",
                *[
                    "| `{}` | `{}` | `{}` |".format(
                        _hex(offset),
                        _markdown_cell(" | ".join(sorted(left)) or "-"),
                        _markdown_cell(" | ".join(sorted(right)) or "-"),
                    )
                    for offset, left, right in relocation_differences
                ],
            ))
        lines.append("")

    lines.extend(("## Unresolved extentless allocations", ""))
    if not unresolved_extentless:
        lines.extend(("No extentless allocation remains unresolved.", ""))
    for row in unresolved_extentless:
        anchor = int(row["target_rva"], 0)
        evidence_size = int(row["evidence_size"], 0)
        end = anchor + evidence_size
        xrefs = [
            item for item in audit
            if item["target_target_rva"] != "-"
            and anchor <= int(item["target_target_rva"], 0) < end
        ]
        lines.extend((
            f"### `{row['target_rva']}` — `{_markdown_cell(row['name'])}`",
            "",
            f"- Identity/type: `{_markdown_cell(row['identity'])}` / `{row['type']}`",
            f"- Section/storage: `{row['section']}` / `{row['storage']}`",
            f"- Observed end: `{row['observed_end_rva']}`; evidence window: "
            f"`{row['evidence_size']}` bytes",
            f"- Next referenced address: `{row['next_referent_rva']}` "
            f"(`{row['next_referent_delta']}`)",
            f"- Next known data start: `{row['next_known_start_rva']}` "
            f"(`{row['next_known_start_delta']}`)",
            f"- Next relocation cell in data: `{row['next_data_reloc_rva']}` "
            f"(`{row['next_data_reloc_delta']}`)",
            f"- Access pattern: `{_markdown_cell(row['accesses'])}`",
            f"- Candidate votes: `{row['candidate_votes']}`; selected start: "
            f"`{row['candidate_base_rva']}`",
            f"- Candidate identity: `{_markdown_cell(row['candidate_identity'])}` "
            f"(`{row['candidate_identity_status']}`)",
            f"- Comparison: `{row['compare_status']}`; raw `{row['raw_status']}`; "
            f"normalized `{row['normalized_status']}`; relocation layout "
            f"`{row['reloc_layout_status']}`; targets `{row['reloc_targets_status']}`",
            f"- Pattern: `{_markdown_cell(row['retail_pattern'])}`",
            f"- Normalized SHA-256: retail `{row['retail_sha256']}`, candidate "
            f"`{row['base_sha256']}`",
            f"- Evidence: {_markdown_cell(row['comment'])}",
            "",
            "Retail bytes (head):",
            "",
            "```text",
            _spaced_hex(row["retail_head"]),
            "```",
        ))
        if row["retail_tail"] != "-":
            lines.extend((
                "", "Retail bytes (tail):", "", "```text",
                _spaced_hex(row["retail_tail"]), "```",
            ))
        if row["base_head"] != "-":
            lines.extend((
                "", "Candidate bytes (head):", "", "```text",
                _spaced_hex(row["base_head"]), "```",
            ))
        if row["candidate_base_rva"] != "-":
            candidate_base = int(row["candidate_base_rva"], 0)
            base_section = base_image.section_at(candidate_base)
            if base_section is not None and base_section.contains(
                candidate_base, evidence_size
            ):
                _, _, target_offsets, target_targets = _window(
                    target_image, anchor, evidence_size, target_index
                )
                _, _, base_offsets, base_targets = _window(
                    base_image, candidate_base, evidence_size, base_index
                )
                relocation_differences = _relocation_target_differences(
                    target_offsets, target_targets, base_offsets, base_targets
                )
                if relocation_differences:
                    lines.extend((
                        "",
                        "Differing resolved relocations:",
                        "",
                        "| Offset | Retail identities | Candidate identities |",
                        "|---:|---|---|",
                        *[
                            "| `{}` | `{}` | `{}` |".format(
                                _hex(offset),
                                _markdown_cell(" | ".join(sorted(left)) or "-"),
                                _markdown_cell(" | ".join(sorted(right)) or "-"),
                            )
                            for offset, left, right in relocation_differences
                        ],
                    ))
        lines.extend((
            "",
            "| Retail site | Candidate site | Offset | Access | Function |",
            "|---|---|---:|---|---|",
        ))
        for xref in sorted(xrefs, key=lambda item: int(item["target_site_rva"], 0)):
            lines.append(
                "| `{}` | `{}` | `{}` | `{}/{}/{}` | `{}` |".format(
                    xref["target_site_rva"], xref["base_site_rva"],
                    _hex(int(xref["target_target_rva"], 0) - anchor),
                    xref["target_access"], xref["target_width"], xref["target_form"],
                    _markdown_cell(xref["function"]),
                )
            )
        lines.append("")
    write_if_changed(artifacts.problems, "\n".join(lines) + "\n")


def _inputs() -> dict[str, str]:
    return {
        "target_exe_sha256": pipeline._file_hash(pipeline.image_paths("target")[0]),
        "base_exe_sha256": pipeline._file_hash(pipeline.image_paths("base")[0]),
        "target_index_sha256": pipeline._file_hash(paths.DATA_TARGET_INDEX),
        "base_index_sha256": pipeline._file_hash(paths.DATA_BASE_INDEX),
        "target_access_sha256": pipeline._file_hash(paths.DATA_TARGET_ACCESS),
        "base_access_sha256": pipeline._file_hash(paths.DATA_BASE_ACCESS),
        "ledger_sha256": pipeline._file_hash(paths.MATCH_STATE),
        "pdb_extents_sha256": pipeline._file_hash(paths.RETAIL_PDB_DATA_EXTENTS),
        "non_pdb_symbols_sha256": pipeline._file_hash(paths.RETAIL_DATA_SYMBOLS),
    }


def refresh(module: str = "render") -> dict:
    artifacts = _artifacts(module)
    pairs = sema_pairing.Pairing()
    ledger = store.load()
    target_image = PEImage(pipeline.image_paths("target")[0])
    base_image = PEImage(pipeline.image_paths("base")[0])
    target_sites = _load_sites("target", pairs, ledger, module)
    base_sites = _load_sites("base", pairs, ledger, module)
    expected_target = _expected_sites(
        "target", target_image, pairs, ledger, module
    )
    expected_base = _expected_sites("base", base_image, pairs, ledger, module)
    if set(target_sites) != expected_target:
        raise RuntimeError(
            f"target access map does not cover every physical {module} data relocation"
        )
    if set(base_sites) != expected_base:
        raise RuntimeError(
            f"base access map does not cover every physical {module} data relocation"
        )

    site_pairs, used_base = _pair_sites(target_sites, base_sites)
    target_index = _datum_index("target", target_image, pairs)
    base_index = _datum_index("base", base_image, pairs)
    audit = []
    comparison_cache: dict[tuple[int, int, int], dict[str, str]] = {}
    for target_rva, target_site in sorted(target_sites.items()):
        if target_rva in site_pairs:
            base_rva, kind = site_pairs[target_rva]
            audit.append(_audit_one(
                target_site, base_sites[base_rva], kind,
                target_image, base_image, target_index, base_index,
                comparison_cache,
            ))
        else:
            audit.append(_audit_one(
                target_site, None, "unpaired",
                target_image, base_image, target_index, base_index,
                comparison_cache,
            ))
    for base_rva, base_site in sorted(base_sites.items()):
        if base_rva not in used_base:
            audit.append(_audit_one(
                None, base_site, "unpaired",
                target_image, base_image, target_index, base_index,
                comparison_cache,
            ))
    extentless = _extentless_rows(
        audit, target_index, base_index, target_image, base_image
    )
    function_data = _function_data_rows(audit, target_index, base_index, ledger)
    _write_tsv(artifacts.audit, AUDIT_COLUMNS, audit)
    _write_tsv(artifacts.extentless, EXTENTLESS_COLUMNS, extentless)
    _write_tsv(artifacts.function_data, FUNCTION_DATA_COLUMNS, function_data)

    report = {
        "schema": 3,
        "module": module,
        "inputs": _inputs(),
        "target_relocation_sites": len(target_sites),
        "base_relocation_sites": len(base_sites),
        "audit_rows": len(audit),
        "extentless_allocations": len(extentless),
        "pair_status": dict(sorted(Counter(
            row["pair_status"] for row in audit
        ).items())),
        "pair_kind": dict(sorted(Counter(
            row["pair_kind"] for row in audit
        ).items())),
        "access_status": dict(sorted(Counter(
            row["access_status"] for row in audit
            if row["pair_status"] == "PAIRED"
        ).items())),
        "identity_status": dict(sorted(Counter(
            row["identity_status"] for row in audit
            if row["pair_status"] == "PAIRED"
        ).items())),
        "extent_status": dict(sorted(Counter(
            row["extent_status"] for row in audit
            if row["pair_status"] != "BASE_ONLY"
        ).items())),
        "datum_status": dict(sorted(Counter(
            row["datum_status"] for row in audit
            if row["pair_status"] != "BASE_ONLY"
        ).items())),
        "extentless_status": dict(sorted(Counter(
            row["compare_status"] for row in extentless
        ).items())),
        "function_data_status": dict(sorted(Counter(
            row["status"] for row in function_data
        ).items())),
        "function_data_resolution": dict(sorted(Counter(
            row["resolution"] for row in function_data
        ).items())),
    }
    _write_markdown(
        audit, extentless, function_data, report,
        target_image, base_image, target_index, base_index, module, artifacts,
    )
    report["outputs"] = {
        "audit_sha256": pipeline._file_hash(artifacts.audit),
        "extentless_sha256": pipeline._file_hash(artifacts.extentless),
        "function_data_sha256": pipeline._file_hash(artifacts.function_data),
        "problems_sha256": pipeline._file_hash(artifacts.problems),
    }
    write_if_changed(
        artifacts.report,
        json.dumps(report, indent=2, sort_keys=True) + "\n",
    )
    return report


def _load_report(module: str = "render") -> dict:
    return json.loads(_artifacts(module).report.read_text(encoding="utf-8"))


def print_report(report: dict | None = None, module: str = "render") -> None:
    report = report or _load_report(module)
    paired = report["pair_status"].get("PAIRED", 0)
    print(
        "{module} data relocations: {target_relocation_sites:,} retail / "
        "{base_relocation_sites:,} base; {paired:,} paired; "
        "{extentless_allocations:,} extentless allocation dossier(s)".format(
            paired=paired, **report
        )
    )
    for key in (
        "pair_kind", "access_status", "identity_status", "extent_status",
        "datum_status", "extentless_status", "function_data_status",
        "function_data_resolution",
    ):
        print("  " + key + ": " + "  ".join(
            f"{name}={count:,}" for name, count in report[key].items()
        ))


def check(module: str = "render") -> int:
    artifacts = _artifacts(module)
    required = tuple(dataclasses.astuple(artifacts))
    if missing_paths := [path for path in required if not path.is_file()]:
        print(f"missing {module} relocation artifacts: " +
              ", ".join(map(str, missing_paths)))
        return 1
    report = _load_report(module)
    if report.get("schema") != 3 or report.get("module") != module:
        print(f"unsupported {module} relocation report schema")
        return 1
    if report.get("inputs") != _inputs():
        print(f"{module} relocation report inputs are stale")
        return 1
    outputs = {
        "audit_sha256": pipeline._file_hash(artifacts.audit),
        "extentless_sha256": pipeline._file_hash(artifacts.extentless),
        "function_data_sha256": pipeline._file_hash(artifacts.function_data),
        "problems_sha256": pipeline._file_hash(artifacts.problems),
    }
    if report.get("outputs") != outputs:
        print(f"{module} relocation report outputs are stale")
        return 1
    with artifacts.audit.open(newline="", encoding="utf-8") as source:
        reader = csv.DictReader(source, delimiter="\t")
        if tuple(reader.fieldnames or ()) != AUDIT_COLUMNS:
            print(f"{module} relocation audit has an unexpected schema")
            return 1
        audit = list(reader)
    with artifacts.extentless.open(newline="", encoding="utf-8") as source:
        reader = csv.DictReader(source, delimiter="\t")
        if tuple(reader.fieldnames or ()) != EXTENTLESS_COLUMNS:
            print(f"{module} extentless audit has an unexpected schema")
            return 1
        extentless = list(reader)
    with artifacts.function_data.open(newline="", encoding="utf-8") as source:
        reader = csv.DictReader(source, delimiter="\t")
        if tuple(reader.fieldnames or ()) != FUNCTION_DATA_COLUMNS:
            print(f"{module} function data audit has an unexpected schema")
            return 1
        function_data = list(reader)
    target_sites = [
        int(row["target_site_rva"], 0)
        for row in audit if row["target_site_rva"] != "-"
    ]
    base_sites = [
        int(row["base_site_rva"], 0)
        for row in audit if row["base_site_rva"] != "-"
    ]
    if len(target_sites) != len(set(target_sites)):
        print(f"a retail {module} relocation appears more than once in the audit")
        return 1
    if len(base_sites) != len(set(base_sites)):
        print(f"a base {module} relocation appears more than once in the audit")
        return 1
    pairs = sema_pairing.Pairing()
    ledger = store.load()
    target_image = PEImage(pipeline.image_paths("target")[0])
    base_image = PEImage(pipeline.image_paths("base")[0])
    if set(target_sites) != _expected_sites(
        "target", target_image, pairs, ledger, module
    ):
        print(f"{module} relocation audit does not contain every retail site")
        return 1
    if set(base_sites) != _expected_sites(
        "base", base_image, pairs, ledger, module
    ):
        print(f"{module} relocation audit does not contain every base site")
        return 1
    if len(audit) != report["audit_rows"] or len(extentless) != report[
        "extentless_allocations"
    ]:
        print(f"{module} relocation report row counts are inconsistent")
        return 1
    if dict(sorted(Counter(
        row["status"] for row in function_data
    ).items())) != report["function_data_status"]:
        print(f"{module} function data report counts are inconsistent")
        return 1
    if dict(sorted(Counter(
        row["resolution"] for row in function_data
    ).items())) != report["function_data_resolution"]:
        print(f"{module} function data resolution counts are inconsistent")
        return 1
    print(
        f"{module} relocation audit complete: {len(target_sites):,} retail and "
        f"{len(base_sites):,} base site(s), each represented exactly once"
    )
    return 0


def inspect(pattern: str, module: str = "render") -> int:
    needle = pattern.casefold()
    matches = 0
    artifacts = _artifacts(module)
    for path in (artifacts.function_data, artifacts.extentless, artifacts.audit):
        with path.open(newline="", encoding="utf-8") as source:
            for row in csv.DictReader(source, delimiter="\t"):
                if needle not in "\t".join(row.values()).casefold():
                    continue
                print(f"{path.name}: " + "  ".join(
                    f"{key}={value}" for key, value in row.items()
                    if value not in {"", "-"}
                ))
                matches += 1
                if matches >= 50:
                    print("50 matches shown; refine the pattern")
                    return 0
    return 0 if matches else 1
