# SPDX-License-Identifier: GPL-3.0-or-later

"""Generate and compare retail/base image data without touching function scores."""

from __future__ import annotations

import bisect
import csv
import hashlib
import json
import os
import re
import shutil
import subprocess
import tempfile
from collections import Counter, defaultdict
from pathlib import Path

from vostok.core import paths
from vostok.core.tsv import write_if_changed
from vostok.data.inventory import DataSymbol, load
from vostok.data.pe import PEImage
from vostok.core.wine import pdb_path
from vostok.core.log import logger
from vostok.derive.aliases import dyn_canon_base


LEDGER_COLUMNS = (
    "identity", "module", "section", "target_rva", "base_rva",
    "target_size", "base_size", "status", "target_hash", "base_hash",
    "relocation_hash", "note",
)
GAP_COLUMNS = (
    "rva", "length", "section", "verdict", "addressed", "touched", "sites",
    "payload_nonzero", "relocs", "prev_object", "prev_name", "next_object",
    "next_name", "first_bytes",
)
EXACT = "EXACT"
PAIRED_STATUSES = frozenset({EXACT, "BYTES", "RELOCS", "RELOC_TOPOLOGY", "SIZE"})


log = logger("data")


def image_paths(side: str) -> tuple[Path, Path]:
    if side == "target":
        root = paths.survarium_bin()
        return root / "survarium.exe", root / "survarium.pdb"
    if side == "base":
        return paths.BASE_EXE, paths.BASE_PDB
    raise ValueError(f"unknown data side {side!r}")


def index_path(side: str) -> Path:
    return paths.DATA_TARGET_INDEX if side == "target" else paths.DATA_BASE_INDEX


def access_path(side: str) -> Path:
    return paths.DATA_TARGET_ACCESS if side == "target" else paths.DATA_BASE_ACCESS


def reloc_path(side: str) -> Path:
    return paths.DATA_TARGET_RELOCS if side == "target" else paths.DATA_BASE_RELOCS


def _engine_args(side: str) -> list[str]:
    if side == "target":
        return [
            "--engine-path", paths.RETAIL_SOURCE_PREFIX,
            "--engine-path", paths.GFX_TARGET_PREFIX + "\\",
        ]
    return [
        "--engine-path", paths.RETAIL_SOURCE_PREFIX + "\\",
        "--engine-path", pdb_path(paths.SOURCES) + "\\",
        "--engine-path", pdb_path(paths.GFX_BUILD_TREE) + "\\",
        "--engine-path", paths.GFX_RELEASE_PREFIX + "\\",
        "--engine-path", pdb_path(paths.SCALEFORM_SDK) + "\\",
    ]


def export_index(side: str) -> Path:
    exe, pdb = image_paths(side)
    for source in (exe, pdb):
        if not source.is_file():
            raise RuntimeError(f"{source} is missing")
    delinker = os.environ.get("VOSTOK_DATA_DELINKER", "vostok-data-delinker")
    if shutil.which(delinker) is None:
        raise RuntimeError("vostok-delinker is not on PATH; enter `nix develop`")
    help_result = subprocess.run(
        [delinker, "--help"], capture_output=True, text=True, check=False
    )
    if "--write-data-index" not in help_result.stdout + help_result.stderr:
        raise RuntimeError(
            "installed vostok-delinker lacks --write-data-index; re-enter the "
            "updated Nix development shell"
        )
    output = index_path(side)
    output.parent.mkdir(parents=True, exist_ok=True)
    log(f"exporting {side} PDB data inventory")
    subprocess.run([
        delinker,
        "--pdb-path", str(pdb),
        "--exe-path", str(exe),
        "--write-data-index", str(output),
        *_engine_args(side),
    ], check=True)
    return output


def _load_rich(side: str) -> list[dict]:
    path = paths.TARGET_IDX if side == "target" else paths.BASE_IDX
    if not path.is_file():
        return []
    with path.open(encoding="utf-8") as source:
        return [json.loads(line) for line in source if line.strip()]


class AddressResolver:
    def __init__(self, symbols: list[DataSymbol], functions: list[dict], *,
                 peer_identities: set[str] | None = None,
                 peer_function_identities: set[str] | None = None):
        trusted = [symbol for symbol in symbols if symbol.size and symbol.size > 0]
        self.data_starts: dict[int, list[DataSymbol]] = defaultdict(list)
        for symbol in symbols:
            self.data_starts[symbol.rva].append(symbol)
        self.data_pages: dict[int, list[DataSymbol]] = defaultdict(list)
        for symbol in trusted:
            assert symbol.end is not None
            for page in range(symbol.rva >> 12, ((symbol.end - 1) >> 12) + 1):
                self.data_pages[page].append(symbol)
        self.peer_identities = peer_identities or set()
        self.peer_function_identities = peer_function_identities or set()
        self.function_starts: dict[int, set[str]] = defaultdict(set)
        for record in functions:
            rva, size = record.get("rva"), record.get("size")
            if not isinstance(rva, int) or not isinstance(size, int) or size <= 0:
                continue
            self.function_starts[rva].update(_function_identity_tokens(record))

    def data_symbol_at(self, rva: int) -> DataSymbol | None:
        def rank(item: DataSymbol) -> tuple:
            return (item.identity not in self.peer_identities, item.size or 0, item.identity)

        starts = self.data_starts.get(rva)
        if starts:
            return min(starts, key=rank)
        candidates = [
            symbol for symbol in self.data_pages.get(rva >> 12, ())
            if symbol.rva <= rva and symbol.end is not None and rva < symbol.end
        ]
        return min(candidates, key=rank, default=None)

    def resolve(self, rva: int, image: PEImage) -> str:
        return " | ".join(sorted(self.resolve_all(rva, image)))

    def resolve_all(self, rva: int, image: PEImage) -> frozenset[str]:
        data = list(self.data_starts.get(rva, ()))
        data.extend(
            symbol for symbol in self.data_pages.get(rva >> 12, ())
            if symbol.rva <= rva and symbol.end is not None and rva < symbol.end
        )
        if data:
            shared = [
                symbol for symbol in data
                if symbol.identity in self.peer_identities
            ]
            owners = shared or data
            return frozenset(
                f"D:{symbol.identity}+{rva - symbol.rva:#x}"
                for symbol in owners
            )
        if functions := self.function_starts.get(rva):
            shared = functions & self.peer_function_identities
            identities = shared or functions
            return frozenset(f"F:{identity}" for identity in identities)
        section = image.section_at(rva)
        if section is not None:
            return frozenset((f"S:{section.name}+{rva - section.rva:#x}",))
        return frozenset((f"RVA:{rva:#x}",))


def _trusted(symbol: DataSymbol, image: PEImage) -> bool:
    if symbol.size is None or symbol.size <= 0:
        return False
    try:
        return image.section(symbol.section).contains(symbol.rva, symbol.size)
    except KeyError:
        return False


def _function_identity_tokens(record: dict) -> frozenset[str]:
    """Preserve every PDB spelling plus the shared dynamic-thunk identity."""
    identities = set()
    if mangled := record.get("mangled"):
        identities.add(f"M:{mangled}")
        if dynamic := dyn_canon_base(mangled):
            identities.add(f"C:{dynamic[0]}:{dynamic[1]}")
    if name := record.get("name"):
        identities.add(f"D:{name}")
    return frozenset(identities)


def _relocation_signature(symbol: DataSymbol, image: PEImage,
                          sites: tuple[int, ...], resolver: AddressResolver
                          ) -> tuple[list[tuple[int, frozenset[str]]], bytes]:
    assert symbol.size is not None
    begin = bisect.bisect_left(sites, symbol.rva)
    end = bisect.bisect_left(sites, symbol.rva + symbol.size)
    signature = []
    normalized = bytearray(image.read_rva(symbol.rva, symbol.size))
    for site in sites[begin:end]:
        offset = site - symbol.rva
        if offset + 4 > symbol.size:
            continue
        value = image.u32_rva(site)
        destination = value - image.image_base if value >= image.image_base else value
        if symbol.rva <= destination <= symbol.rva + symbol.size:
            identities = frozenset((f"SELF+{destination - symbol.rva:#x}",))
        else:
            identities = resolver.resolve_all(destination, image)
        signature.append((offset, identities))
        normalized[offset:offset + 4] = bytes(4)
    return signature, bytes(normalized)


def _hash(value: bytes) -> str:
    return hashlib.sha256(value).hexdigest()


def _signature_hash(signature: list[tuple[int, frozenset[str]]]) -> str:
    serializable = [(offset, sorted(identities)) for offset, identities in signature]
    return _hash(json.dumps(serializable, separators=(",", ":")).encode())


def _relocation_signatures_match(
    target: list[tuple[int, frozenset[str]]],
    base: list[tuple[int, frozenset[str]]],
) -> bool:
    return (
        [offset for offset, _ in target] == [offset for offset, _ in base]
        and all(left & right for (_, left), (_, right) in zip(target, base))
    )


def _compare_pair(target: DataSymbol, base: DataSymbol,
                  target_image: PEImage, base_image: PEImage,
                  target_sites: tuple[int, ...], base_sites: tuple[int, ...],
                  target_resolver: AddressResolver, base_resolver: AddressResolver,
                  ) -> dict:
    row = _ledger_row(target, base)
    if target.size is None or base.size is None:
        row["status"] = "UNKNOWN_SIZE"
        row["note"] = "PDB type extent unavailable"
        return row
    if not _trusted(target, target_image) or not _trusted(base, base_image):
        row["status"] = "INVALID_EXTENT"
        row["note"] = "PDB type extent leaves its PE section"
        return row
    target_signature, target_bytes = _relocation_signature(
        target, target_image, target_sites, target_resolver
    )
    base_signature, base_bytes = _relocation_signature(
        base, base_image, base_sites, base_resolver
    )
    row["target_hash"] = _hash(target_bytes)
    row["base_hash"] = _hash(base_bytes)
    row["relocation_hash"] = (
        _signature_hash(target_signature) + "/" + _signature_hash(base_signature)
    )
    if target.size != base.size:
        row["status"] = "SIZE"
        row["note"] = f"type extent target={target.size} base={base.size}"
        return row
    if target_bytes != base_bytes:
        row["status"] = "BYTES"
        offset = next(index for index, values in enumerate(zip(target_bytes, base_bytes))
                      if values[0] != values[1])
        row["note"] = (
            f"first normalized byte differs at +{offset:#x}: "
            f"target={target_bytes[offset]:#04x} base={base_bytes[offset]:#04x}"
        )
        return row
    target_offsets = [offset for offset, _ in target_signature]
    base_offsets = [offset for offset, _ in base_signature]
    if target_offsets != base_offsets:
        row["status"] = "RELOC_TOPOLOGY"
        row["note"] = f"pointer offsets target={target_offsets} base={base_offsets}"
        return row
    if not _relocation_signatures_match(target_signature, base_signature):
        row["status"] = "RELOCS"
        mismatch = next(
            (
                index
                for index, ((_, left), (_, right))
                in enumerate(zip(target_signature, base_signature))
                if not left & right
            ),
            0,
        )
        row["note"] = (
            f"pointer +{target_signature[mismatch][0]:#x}: "
            f"target={' | '.join(sorted(target_signature[mismatch][1]))} "
            f"base={' | '.join(sorted(base_signature[mismatch][1]))}"
        )
        return row
    row["status"] = EXACT
    return row


def _ledger_row(target: DataSymbol | None, base: DataSymbol | None) -> dict:
    symbol = target or base
    assert symbol is not None
    return {
        "identity": symbol.identity,
        "module": (target or base).owner_module,
        "section": (target or base).section,
        "target_rva": f"{target.rva:#x}" if target else "-",
        "base_rva": f"{base.rva:#x}" if base else "-",
        "target_size": str(target.size) if target and target.size is not None else "-",
        "base_size": str(base.size) if base and base.size is not None else "-",
        "status": "TARGET_ONLY" if target else "BASE_ONLY",
        "target_hash": "-",
        "base_hash": "-",
        "relocation_hash": "-",
        "note": "-",
    }


def _comparison_key(symbol: DataSymbol) -> tuple[str, bytes]:
    """Disambiguate same-leaf PDB locals without changing their source identity."""
    qualified = b"" if symbol.scope == "external" else (
        symbol.public_name or symbol.name
    )
    return symbol.identity, qualified


def _physical_allocations(symbols: list[DataSymbol]) -> list[DataSymbol]:
    """Collapse duplicate PDB records, never distinct linked allocations."""
    by_allocation: dict[tuple[int, str, str, int | None], list[DataSymbol]] = (
        defaultdict(list)
    )
    for symbol in symbols:
        by_allocation[(
            symbol.rva, symbol.section, symbol.storage, symbol.size,
        )].append(symbol)
    return [
        min(group, key=lambda row: (
            row.display_name, row.type_index, row.size_kind,
        ))
        for _, group in sorted(by_allocation.items())
    ]


_PAIR_RANK = {
    EXACT: 0,
    "RELOCS": 1,
    "BYTES": 2,
    "RELOC_TOPOLOGY": 3,
    "SIZE": 4,
    "UNKNOWN_SIZE": 5,
    "INVALID_EXTENT": 6,
}


def _pair_candidates(
    target: list[DataSymbol], base: list[DataSymbol],
    target_image: PEImage, base_image: PEImage,
    target_sites: tuple[int, ...], base_sites: tuple[int, ...],
    target_resolver: AddressResolver, base_resolver: AddressResolver,
) -> tuple[list[dict], list[DataSymbol], list[DataSymbol]]:
    """Compare a same-name allocation multiset without address-order guesses."""
    candidates = []
    for target_index, target_symbol in enumerate(target):
        for base_index, base_symbol in enumerate(base):
            row = _compare_pair(
                target_symbol, base_symbol, target_image, base_image,
                target_sites, base_sites, target_resolver, base_resolver,
            )
            size_delta = (
                abs(target_symbol.size - base_symbol.size)
                if target_symbol.size is not None and base_symbol.size is not None
                else 1 << 30
            )
            candidates.append((
                _PAIR_RANK[row["status"]], size_delta,
                target_symbol.rva, base_symbol.rva,
                target_index, base_index, row,
            ))

    paired_target = set()
    paired_base = set()
    rows = []
    for *_, target_index, base_index, row in sorted(candidates):
        if target_index in paired_target or base_index in paired_base:
            continue
        paired_target.add(target_index)
        paired_base.add(base_index)
        rows.append(row)
    unpaired_target = [
        symbol for index, symbol in enumerate(target)
        if index not in paired_target
    ]
    unpaired_base = [
        symbol for index, symbol in enumerate(base)
        if index not in paired_base
    ]
    if len(target) != 1 or len(base) != 1:
        context = f"PDB allocation multiset target={len(target)} base={len(base)}"
        for row in rows:
            row["note"] = (
                context if row["note"] == "-" else f"{row['note']}; {context}"
            )
    return rows, unpaired_target, unpaired_base


_ORDINAL_LOCAL = re.compile(rb"^\$S\d+$")


def _ordinal_local(symbol: DataSymbol) -> bool:
    """MSVC's $S<n> names are TU ordinals, not cross-build identities."""
    return symbol.scope != "external" and bool(_ORDINAL_LOCAL.fullmatch(symbol.name))


def _consumer_fingerprints(
    side: str, symbols: list[DataSymbol], image: PEImage,
) -> dict[int, tuple]:
    """Describe each allocation by all code accesses which consume it.

    This is the safe fallback for local symbols whose spelling or LTCG owner
    moved.  Initializer equality alone is deliberately insufficient: many
    unrelated guards and constants contain the same all-zero bytes.
    """
    physical = _physical_allocations([
        symbol for symbol in symbols if _trusted(symbol, image)
    ])
    resolver = AddressResolver(physical, [])
    accesses: dict[int, list[tuple]] = defaultdict(list)
    path = access_path(side)
    if not path.is_file():
        return {}
    with path.open(newline="", encoding="utf-8") as source:
        for row in csv.DictReader(source, delimiter="\t"):
            rva = int(row["target_rva"], 0)
            symbol = resolver.data_symbol_at(rva)
            if symbol is None:
                continue
            accesses[symbol.rva].append((
                row["caller_mangled"], row["caller_file"],
                row["access"], row["width"], row.get("form", "-"),
                row.get("scale", "0"), rva - symbol.rva,
            ))
    return {
        rva: tuple(sorted(rows))
        for rva, rows in accesses.items()
        if rows
    }


def _consumer_fallback_key(
    symbol: DataSymbol, fingerprints: dict[int, tuple],
) -> tuple | None:
    if symbol.scope == "external" or symbol.rva not in fingerprints:
        return None
    return (
        symbol.section, symbol.storage, symbol.size,
        fingerprints[symbol.rva],
    )


def compare(target_symbols: list[DataSymbol], base_symbols: list[DataSymbol],
            target_image: PEImage, base_image: PEImage) -> list[dict]:
    target_by_key: dict[tuple[str, bytes], list[DataSymbol]] = defaultdict(list)
    base_by_key: dict[tuple[str, bytes], list[DataSymbol]] = defaultdict(list)
    ordinal_target = []
    ordinal_base = []
    for symbol in target_symbols:
        if symbol.type_index != 0:  # public-only rows are address anchors
            if _ordinal_local(symbol):
                ordinal_target.append(symbol)
            else:
                target_by_key[_comparison_key(symbol)].append(symbol)
    for symbol in base_symbols:
        if symbol.type_index != 0:
            if _ordinal_local(symbol):
                ordinal_base.append(symbol)
            else:
                base_by_key[_comparison_key(symbol)].append(symbol)

    target_sites = target_image.base_relocations()
    base_sites = base_image.base_relocations()
    target_functions = _load_rich("target")
    base_functions = _load_rich("base")
    target_function_names = {
        token for record in target_functions
        for token in _function_identity_tokens(record)
    }
    base_function_names = {
        token for record in base_functions
        for token in _function_identity_tokens(record)
    }

    target_identities = {symbol.identity for symbol in target_symbols}
    base_identities = {symbol.identity for symbol in base_symbols}
    target_resolver = AddressResolver(
        target_symbols, target_functions,
        peer_identities=base_identities,
        peer_function_identities=base_function_names,
    )
    base_resolver = AddressResolver(
        base_symbols, base_functions,
        peer_identities=target_identities,
        peer_function_identities=target_function_names,
    )
    rows = []
    unpaired_target = _physical_allocations(ordinal_target)
    unpaired_base = _physical_allocations(ordinal_base)
    for identity in sorted(target_by_key.keys() | base_by_key.keys()):
        target = _physical_allocations(target_by_key.get(identity, []))
        base = _physical_allocations(base_by_key.get(identity, []))
        paired, target_remainder, base_remainder = _pair_candidates(
            target, base, target_image, base_image, target_sites, base_sites,
            target_resolver, base_resolver,
        )
        rows.extend(paired)
        unpaired_target.extend(target_remainder)
        unpaired_base.extend(base_remainder)

    target_fingerprints = _consumer_fingerprints(
        "target", target_symbols, target_image
    )
    base_fingerprints = _consumer_fingerprints("base", base_symbols, base_image)
    target_by_consumer: dict[tuple, list[DataSymbol]] = defaultdict(list)
    base_by_consumer: dict[tuple, list[DataSymbol]] = defaultdict(list)
    for symbol in unpaired_target:
        key = _consumer_fallback_key(symbol, target_fingerprints)
        if key is not None:
            target_by_consumer[key].append(symbol)
    for symbol in unpaired_base:
        key = _consumer_fallback_key(symbol, base_fingerprints)
        if key is not None:
            base_by_consumer[key].append(symbol)

    consumed_target = set()
    consumed_base = set()
    for key in sorted(target_by_consumer.keys() & base_by_consumer.keys()):
        target = sorted(target_by_consumer[key], key=lambda row: row.rva)
        base = sorted(base_by_consumer[key], key=lambda row: row.rva)
        paired, target_remainder, base_remainder = _pair_candidates(
            target, base, target_image, base_image, target_sites, base_sites,
            target_resolver, base_resolver,
        )
        for row in paired:
            row["note"] = (
                "paired by complete consumer fingerprint"
                if row["note"] == "-"
                else f"{row['note']}; paired by complete consumer fingerprint"
            )
        rows.extend(paired)
        target_left = {symbol.rva for symbol in target_remainder}
        base_left = {symbol.rva for symbol in base_remainder}
        consumed_target.update(
            symbol.rva for symbol in target if symbol.rva not in target_left
        )
        consumed_base.update(
            symbol.rva for symbol in base if symbol.rva not in base_left
        )

    rows.extend(
        _ledger_row(symbol, None)
        for symbol in unpaired_target if symbol.rva not in consumed_target
    )
    rows.extend(
        _ledger_row(None, symbol)
        for symbol in unpaired_base if symbol.rva not in consumed_base
    )
    return rows


def _write_ledger(rows: list[dict]) -> None:
    lines = ["\t".join(LEDGER_COLUMNS)]
    for row in rows:
        lines.append("\t".join(str(row[column]).replace("\t", "\\t")
                               for column in LEDGER_COLUMNS))
    write_if_changed(paths.DATA_STATE, "\n".join(lines) + "\n")


def _retail_kind(symbols: list[DataSymbol]) -> str:
    names = [symbol.display_name for symbol in symbols]
    checks = (
        ("vtable", lambda name: name.startswith("??_7")),
        ("rtti", lambda name: name.startswith("??_R")),
        ("string", lambda name: name.startswith("??_C@")),
        ("fppool", lambda name: name.startswith(("__real@", "$T"))),
        ("guard", lambda name: name.startswith("??_B")),
        ("ehtable", lambda name: any(token in name for token in (
            "__ehfuncinfo", "__unwindtable", "__tryblocktable", "__ip2state",
        ))),
    )
    return next((kind for kind, test in checks if any(test(name) for name in names)),
                "datum")


def _write_retail_census(symbols: list[DataSymbol], *, force: bool = False) -> None:
    """Seed Gruntz's structural retail census from the authoritative PDB.

    Once committed, this is an admitted start/kind table rather than a build
    output. A normal refresh therefore validates it but never rewrites it.
    """
    if paths.RETAIL_DATA.is_file() and not force:
        return
    by_rva: dict[int, list[DataSymbol]] = defaultdict(list)
    for symbol in symbols:
        by_rva[symbol.rva].append(symbol)
    lines = [
        "# MANUALLY MANAGED - admitted retail data starts from the shipped PDB.",
        "# Extents and owner/name evidence are derived separately; builds do not rewrite this.",
        "rva\tkind",
    ]
    for rva, aliases in sorted(by_rva.items()):
        lines.append(f"{rva:#x}\t{_retail_kind(aliases)}")
    paths.RETAIL_DATA.parent.mkdir(parents=True, exist_ok=True)
    write_if_changed(paths.RETAIL_DATA, "\n".join(lines) + "\n")


def _interval_union(intervals: list[tuple[int, int]]) -> int:
    total = 0
    cursor = -1
    for start, end in sorted(intervals):
        if end <= start:
            continue
        if start > cursor:
            total += end - start
            cursor = end
        elif end > cursor:
            total += end - cursor
            cursor = end
    return total


def coverage(rows: list[dict], symbols: list[DataSymbol], image: PEImage,
             consumer: dict | None = None) -> dict:
    sections = [image.section(".rdata"), image.section(".data")]
    gross = sum(section.virtual_size for section in sections)
    claimed = [(symbol.rva, symbol.rva + symbol.size)
               for symbol in symbols if _trusted(symbol, image)]
    by_rva = {symbol.rva: symbol for symbol in symbols if _trusted(symbol, image)}
    exact = []
    compared = []
    for row in rows:
        target_rva = row["target_rva"]
        symbol = by_rva.get(int(target_rva, 0)) if target_rva != "-" else None
        if symbol is None or symbol.size is None:
            continue
        interval = (symbol.rva, symbol.rva + symbol.size)
        if row["status"] in PAIRED_STATUSES:
            compared.append(interval)
        if row["status"] == EXACT:
            exact.append(interval)
    claimed_bytes = _interval_union(claimed)
    compared_bytes = _interval_union(compared)
    exact_bytes = _interval_union(exact)
    metrics = {
        "gross_bytes": gross,
        "claimed_bytes": claimed_bytes,
        "gross_coverage_percent": 100.0 * claimed_bytes / gross if gross else 0.0,
        "compared_bytes": compared_bytes,
        "exact_bytes": exact_bytes,
        "fidelity_percent": 100.0 * exact_bytes / compared_bytes if compared_bytes else 0.0,
        "image_exact_percent": 100.0 * exact_bytes / gross if gross else 0.0,
        "sections": {
            section.name: {
                "rva": section.rva,
                "virtual_size": section.virtual_size,
                "raw_size": section.raw_size,
            }
            for section in sections
        },
    }
    if consumer:
        reachable = int(consumer.get("unique_allocation_bytes", 0))
        paired = int(consumer.get("paired_unique_bytes", 0))
        metrics.update({
            "consumer_reachable_bytes": reachable,
            "consumer_reachable_percent": 100.0 * reachable / gross if gross else 0.0,
            "consumer_paired_bytes": paired,
            "consumer_paired_percent": 100.0 * paired / gross if gross else 0.0,
        })
    if paths.DATA_OBJDIFF_REPORT.is_file():
        objdiff = json.loads(
            paths.DATA_OBJDIFF_REPORT.read_text(encoding="utf-8")
        )["measures"]
        projected = int(objdiff.get("total_data", 0))
        matched = int(objdiff.get("matched_data", 0))
        metrics.update({
            # The same retail allocation is intentionally emitted into every
            # consumer unit, so this is a copy-counting denominator. It must
            # never be divided by the unique retail image size.
            "objdiff_projected_bytes": projected,
            "objdiff_matched_bytes": matched,
            "objdiff_match_percent": (
                100.0 * matched / projected if projected else 0.0
            ),
        })
    return metrics


def _write_relocations(side: str, symbols: list[DataSymbol], image: PEImage) -> None:
    resolver = AddressResolver(symbols, _load_rich(side))
    lines = ["site_rva\tsource_identity\tsource_offset\traw_value\ttarget"]
    for site in image.base_relocations():
        section = image.section_at(site)
        if section is None or section.name not in (".data", ".rdata"):
            continue
        source = resolver.data_symbol_at(site)
        value = image.u32_rva(site)
        destination = value - image.image_base if value >= image.image_base else value
        lines.append("\t".join((
            f"{site:#x}", source.identity if source else "-",
            f"{site - source.rva:#x}" if source else "-", f"{value:#x}",
            resolver.resolve(destination, image),
        )))
    write_if_changed(reloc_path(side), "\n".join(lines) + "\n")


_DISASM = re.compile(r"^\s*([0-9a-fA-F]+):\s+(.+?)\s*$")


def _access_kind(instruction: str, absolute_value: int | None = None) -> tuple[str, str]:
    lower = instruction.lower()
    width = next((name for name in ("xmmword", "qword", "dword", "word", "byte")
                  if f"{name} ptr" in lower), "-")
    mnemonic, _, operands = lower.partition("\t")
    if not operands:
        mnemonic, _, operands = lower.partition(" ")
    split_operands = [operand.strip() for operand in operands.split(",")]
    first = split_operands[0] if split_operands else ""
    selected = next(
        (index for index, operand in enumerate(split_operands)
         if absolute_value is not None and f"{absolute_value:#x}" in operand),
        None,
    )
    if selected is not None and "[" not in split_operands[selected]:
        return "address", width
    if mnemonic == "lea" or "[" not in operands:
        return "address", width
    if selected is not None and selected > 0:
        return "read", width
    if "[" in first:
        if mnemonic in {"call", "jmp", "cmp", "test", "push", "fld", "movzx", "movsx"}:
            return "read", width
        if mnemonic in {"inc", "dec", "add", "sub", "and", "or", "xor", "xchg"}:
            return "readwrite", width
        return "write", width
    return "read", width


def _access_form(instruction: str) -> tuple[str, int]:
    lower = instruction.lower()
    pieces = lower.split(None, 1)
    mnemonic = pieces[0] if pieces else ""
    scales = [int(value) for value in re.findall(r"\*\s*([1248])\b", lower)]
    if mnemonic in {"call", "jmp"} and "[" in lower:
        return "indcall", max(scales, default=0)
    if mnemonic == "lea":
        return "lea", max(scales, default=0)
    if "[" not in lower:
        return "imm", 0
    if scales:
        return "indexed", max(scales)
    return "direct", 0


class _FunctionIndex:
    def __init__(self, functions: list[dict]):
        self.records = sorted(
            (record for record in functions
             if isinstance(record.get("rva"), int)
             and isinstance(record.get("size"), int)
             and record["size"] > 0),
            key=lambda record: record["rva"],
        )
        self.starts = [record["rva"] for record in self.records]
        self.prefix_end = []
        maximum = 0
        for record in self.records:
            maximum = max(maximum, record["rva"] + record["size"])
            self.prefix_end.append(maximum)

    def containing(self, rva: int) -> list[dict]:
        found = []
        index = bisect.bisect_right(self.starts, rva) - 1
        while index >= 0 and self.prefix_end[index] > rva:
            record = self.records[index]
            if rva < record["rva"] + record["size"]:
                found.append(record)
            index -= 1
        return found


def _write_access(side: str, symbols: list[DataSymbol], image: PEImage) -> None:
    objdump = shutil.which("llvm-objdump")
    if objdump is None:
        raise RuntimeError("llvm-objdump is required for the data access map")
    exe, _ = image_paths(side)
    functions = _load_rich(side)
    function_index = _FunctionIndex(functions)
    resolver = AddressResolver(symbols, functions)
    text = image.section(".text")

    def disassemble(path: Path) -> tuple[list[tuple[int, str]], list[int]]:
        result = subprocess.run(
            [objdump, "-d", "--no-show-raw-insn", "--x86-asm-syntax=intel",
             str(path)],
            capture_output=True, text=True, check=True,
        )
        decoded = []
        for line in result.stdout.splitlines():
            match = _DISASM.match(line)
            if match:
                decoded.append((
                    int(match.group(1), 16) - image.image_base, match.group(2)
                ))
        return decoded, [rva for rva, _ in decoded]

    linear, linear_starts = disassemble(exe)
    anchored_bytes = bytearray(image.data)
    anchored_bytes[text.raw_offset:text.raw_offset + text.raw_size] = b"\x90" * text.raw_size
    for record in functions:
        rva, size = record.get("rva"), record.get("size")
        if not isinstance(rva, int) or not isinstance(size, int) or size <= 0:
            continue
        start = max(rva, text.rva)
        end = min(rva + size, text.rva + text.raw_size)
        if start >= end:
            continue
        raw_start = text.raw_offset + start - text.rva
        raw_end = text.raw_offset + end - text.rva
        anchored_bytes[raw_start:raw_end] = image.data[raw_start:raw_end]
    with tempfile.NamedTemporaryFile(suffix=".exe") as anchored_file:
        anchored_file.write(anchored_bytes)
        anchored_file.flush()
        anchored, anchored_starts = disassemble(Path(anchored_file.name))

    def at_site(decoded: list[tuple[int, str]], starts: list[int], site: int):
        index = bisect.bisect_right(starts, site) - 1
        if index < 0:
            return None
        instruction_rva, instruction = decoded[index]
        next_rva = decoded[index + 1][0] if index + 1 < len(decoded) else text.end
        if instruction_rva <= site < next_rva:
            return instruction_rva, instruction
        return None

    lines = [
        "site_rva\tinstruction_rva\taccess\twidth\tend_rva\tform\tscale\t"
        "target_rva\ttarget_identity\tcaller_mangled\tcaller_name\tcaller_file\t"
        "instruction"
    ]
    for site in image.base_relocations():
        if not text.contains(site, 4):
            continue
        value = image.u32_rva(site)
        destination = value - image.image_base if value >= image.image_base else value
        target_section = image.section_at(destination)
        if target_section is None or target_section.name not in (".data", ".rdata"):
            continue
        callers = function_index.containing(site)
        decoded = at_site(
            anchored if callers else linear,
            anchored_starts if callers else linear_starts,
            site,
        )
        if decoded is None:
            instruction_rva, instruction = site, "-"
            access, width, form, scale = "undecoded", "-", "undecoded", 0
        else:
            instruction_rva, instruction = decoded
            access, width = _access_kind(instruction, value)
            form, scale = _access_form(instruction)
        width_bytes = {
            "byte": 1, "word": 2, "dword": 4, "qword": 8, "xmmword": 16,
        }.get(width, 0)
        for caller in callers or [{}]:
            fields = (
                f"{site:#x}", f"{instruction_rva:#x}", access, width,
                f"{destination + width_bytes:#x}", form, str(scale),
                f"{destination:#x}", resolver.resolve(destination, image),
                caller.get("mangled", "-"), caller.get("name", "-"),
                caller.get("file", "-"), instruction.replace("\t", " "),
            )
            lines.append("\t".join(str(field).replace("\t", " ") for field in fields))
    write_if_changed(access_path(side), "\n".join(lines) + "\n")


def _manifest_runs(image: PEImage) -> list[tuple[int, int]]:
    """Unique retail extents actually enrolled by the target manifest."""
    intervals = set()
    if paths.DELINK_DATA_MANIFEST.is_file():
        with paths.DELINK_DATA_MANIFEST.open(newline="", encoding="utf-8") as source:
            for row in csv.DictReader(source, delimiter="\t"):
                start = int(row["rva"], 0)
                end = start + int(row["size"], 0)
                section = image.section_at(start)
                if section is not None and section.contains(start, end - start):
                    intervals.add((start, end))
    runs: list[list[int]] = []
    for start, end in sorted(intervals):
        if runs and start <= runs[-1][1]:
            runs[-1][1] = max(runs[-1][1], end)
        else:
            runs.append([start, end])
    return [(start, end) for start, end in runs]


def _write_coverage_gaps(symbols: list[DataSymbol], image: PEImage) -> None:
    """Write Gruntz's unenrolled-range table for the retail image."""
    by_start: dict[int, list[DataSymbol]] = defaultdict(list)
    by_end: dict[int, list[DataSymbol]] = defaultdict(list)
    for symbol in symbols:
        if _trusted(symbol, image) and symbol.size is not None:
            by_start[symbol.rva].append(symbol)
            by_end[symbol.rva + symbol.size].append(symbol)

    widths = {"byte": 1, "word": 2, "dword": 4, "qword": 8, "xmmword": 16}
    touches: list[tuple[int, int]] = []
    if paths.DATA_TARGET_ACCESS.is_file():
        with paths.DATA_TARGET_ACCESS.open(newline="", encoding="utf-8") as source:
            for row in csv.DictReader(source, delimiter="\t"):
                width = widths.get(row["width"], 0)
                if width and row["access"] in {"read", "write", "readwrite"}:
                    start = int(row["target_rva"], 0)
                    touches.append((start, start + width))
    touches.sort()
    touch_starts = [start for start, _ in touches]

    relocation_sites = image.base_relocations()
    destinations = sorted(
        (value - image.image_base if value >= image.image_base else value)
        for value in (image.u32_rva(site) for site in relocation_sites)
    )

    lines = [
        "# GENERATED by vostok.data - retail bytes no PDB-typed datum covers.",
        "\t".join(GAP_COLUMNS),
    ]
    for section_name in (".rdata", ".data"):
        section = image.section(section_name)
        runs = [
            (max(start, section.rva), min(end, section.end))
            for start, end in _manifest_runs(image)
            if start < section.end and end > section.rva
        ]
        cursor = section.rva
        gaps = []
        for start, end in runs:
            if start > cursor:
                gaps.append((cursor, start))
            cursor = max(cursor, end)
        if cursor < section.end:
            gaps.append((cursor, section.end))

        for start, end in gaps:
            payload = image.read_rva(start, end - start)
            nonzero = sum(byte != 0 for byte in payload)
            rel_begin = bisect.bisect_left(relocation_sites, start)
            rel_end = bisect.bisect_left(relocation_sites, end)
            reloc_count = rel_end - rel_begin
            addressed = bisect.bisect_left(destinations, end) - bisect.bisect_left(
                destinations, start
            )
            touched_intervals = []
            touch_index = bisect.bisect_left(touch_starts, start)
            while touch_index and touches[touch_index - 1][1] > start:
                touch_index -= 1
            while touch_index < len(touches) and touches[touch_index][0] < end:
                left, right = touches[touch_index]
                if right > start:
                    touched_intervals.append((max(start, left), min(end, right)))
                touch_index += 1
            touched = _interval_union(touched_intervals)
            sites = len(touched_intervals)
            verdict = (
                "POINTER" if reloc_count and nonzero else
                "NONZERO" if nonzero else
                "PAD" if end - start < 8 else
                "ZERO-GAP"
            )
            previous = min(by_end.get(start, ()), key=lambda item: item.identity,
                           default=None)
            following = min(by_start.get(end, ()), key=lambda item: item.identity,
                            default=None)
            fields = {
                "rva": f"{start:#x}",
                "length": str(end - start),
                "section": section_name,
                "verdict": verdict,
                "addressed": str(addressed),
                "touched": str(touched),
                "sites": str(sites),
                "payload_nonzero": str(nonzero),
                "relocs": str(reloc_count),
                "prev_object": previous.owner_module if previous else "-",
                "prev_name": previous.display_name if previous else "-",
                "next_object": following.owner_module if following else "-",
                "next_name": following.display_name if following else "-",
                "first_bytes": payload[:16].hex(),
            }
            lines.append("\t".join(fields[column].replace("\t", "\\t")
                                   for column in GAP_COLUMNS))
    write_if_changed(paths.DATA_COVERAGE_GAPS, "\n".join(lines) + "\n")


def init_target(*, force: bool = False) -> None:
    if paths.DATA_TARGET_INDEX.is_file() and not force:
        with paths.DATA_TARGET_INDEX.open(encoding="ascii") as source:
            has_public_anchors = any(
                len(fields := line.rstrip("\n").split("\t")) == 11
                and fields[5] == "0x0"
                for line in source
            )
        if not has_public_anchors:
            log("retail inventory predates public address anchors; regenerating")
            force = True
    if paths.DATA_TARGET_INDEX.is_file() and not force:
        symbols = load(paths.DATA_TARGET_INDEX)
        image = PEImage(image_paths("target")[0])
        # The PDB census is immutable, but these maps encode our current
        # resolver and access-classification rules. Re-derive them so a tooling
        # change can never leave apparently valid target evidence stale.
        _write_relocations("target", symbols, image)
        _write_access("target", symbols, image)
        _write_retail_census(symbols)
        log("retail inventory reused; derived evidence refreshed")
    else:
        export_index("target")
        symbols = load(paths.DATA_TARGET_INDEX)
        image = PEImage(image_paths("target")[0])
        _write_relocations("target", symbols, image)
        _write_access("target", symbols, image)
        _write_retail_census(symbols, force=force)
        log(f"retail inventory ready: {len(symbols):,} PDB data records")


    from vostok.data import missing
    missing_report = missing.refresh()
    log(
        "non-PDB data: {unique_missing_targets:,} referenced targets; "
        "{unresolved_targets:,} still need review".format(**missing_report)
    )
    if missing_report["unresolved_targets"]:
        raise RuntimeError(
            "non-PDB data census is incomplete: "
            f"{missing_report['unresolved_targets']:,} target(s) need manual review; "
            "run `python3 -m vostok data missing-next`"
        )


def prepare_manifests() -> dict:
    """Refresh both access graphs and project consumer-owned delink manifests."""
    init_target()
    export_index("base")
    base_symbols = load(paths.DATA_BASE_INDEX)
    base_image = PEImage(image_paths("base")[0])
    _write_relocations("base", base_symbols, base_image)
    _write_access("base", base_symbols, base_image)
    from vostok.data import manifest
    summary = manifest.generate()
    _write_coverage_gaps(load(paths.DATA_TARGET_INDEX), PEImage(image_paths("target")[0]))
    log(
        "consumer projection: {consumer_units:,} units, "
        "{allocation_copies:,} allocation copies, {blockers:,} blockers".format(
            **summary
        )
    )
    return summary


def refresh() -> dict:
    consumer_summary = prepare_manifests()
    target_symbols = load(paths.DATA_TARGET_INDEX)
    base_symbols = load(paths.DATA_BASE_INDEX)
    target_image = PEImage(image_paths("target")[0])
    base_image = PEImage(image_paths("base")[0])
    rows = compare(target_symbols, base_symbols, target_image, base_image)
    counts = Counter(row["status"] for row in rows)
    module_counts = defaultdict(Counter)
    for row in rows:
        module_counts[row["module"]][row["status"]] += 1
    metrics = coverage(rows, target_symbols, target_image, consumer_summary)

    strict = {}
    if paths.DATA_STRICT_REPORT.is_file() and paths.DATA_OBJDIFF_REPORT.is_file():
        normal_measures = json.loads(
            paths.DATA_OBJDIFF_REPORT.read_text(encoding="utf-8")
        )["measures"]
        strict_measures = json.loads(
            paths.DATA_STRICT_REPORT.read_text(encoding="utf-8")
        )["measures"]
        strict = {
            "normal_matched_functions": int(normal_measures["matched_functions"]),
            "strict_matched_functions": int(strict_measures["matched_functions"]),
            "referent_debt_functions": max(
                0,
                int(normal_measures["matched_functions"])
                - int(strict_measures["matched_functions"]),
            ),
            "normal_matched_code": int(normal_measures["matched_code"]),
            "strict_matched_code": int(strict_measures["matched_code"]),
            "referent_debt_code_bytes": max(
                0,
                int(normal_measures["matched_code"])
                - int(strict_measures["matched_code"]),
            ),
        }

    def inputs(side: str) -> dict[str, str]:
        exe, pdb = image_paths(side)
        return {
            "exe_sha256": _file_hash(exe),
            "pdb_sha256": _file_hash(pdb),
            "index_sha256": _file_hash(index_path(side)),
            "access_sha256": _file_hash(access_path(side)),
            "relocations_sha256": _file_hash(reloc_path(side)),
        }

    report = {
        "schema": 2,
        "inputs": {side: inputs(side) for side in ("target", "base")},
        "counts": dict(sorted(counts.items())),
        "modules": {module: dict(sorted(values.items()))
                    for module, values in sorted(module_counts.items())},
        "coverage": metrics,
        "consumer_projection": consumer_summary,
        "strict_referents": strict,
        "rows": rows,
    }
    report["inputs"]["target"].update({
        "retail_data_sha256": _file_hash(paths.RETAIL_DATA),
        "coverage_gaps_sha256": _file_hash(paths.DATA_COVERAGE_GAPS),
        "consumer_closure_sha256": _file_hash(paths.DATA_CONSUMER_CLOSURE),
        "function_state_sha256": _file_hash(paths.DATA_FUNCTION_STATE),
        "manifest_blockers_sha256": _file_hash(paths.DATA_MANIFEST_BLOCKERS),
        "target_manifest_sha256": _file_hash(paths.DELINK_DATA_MANIFEST),
        "target_section_manifest_sha256": _file_hash(
            paths.DELINK_DATA_SECTION_MANIFEST
        ),
        "base_manifest_sha256": _file_hash(paths.BASE_DELINK_DATA_MANIFEST),
        "base_section_manifest_sha256": _file_hash(
            paths.BASE_DELINK_DATA_SECTION_MANIFEST
        ),
    })
    if paths.DATA_STRICT_REPORT.is_file():
        report["inputs"]["target"]["strict_report_sha256"] = _file_hash(
            paths.DATA_STRICT_REPORT
        )
    if paths.DATA_OBJDIFF_REPORT.is_file():
        report["inputs"]["target"]["data_objdiff_report_sha256"] = _file_hash(
            paths.DATA_OBJDIFF_REPORT
        )
    paths.GEN_DIR.mkdir(parents=True, exist_ok=True)
    paths.DATA_REPORT.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    paths.DATA_COVERAGE.write_text(json.dumps(metrics, indent=2) + "\n", encoding="utf-8")
    _write_ledger(rows)
    log(
        f"{counts[EXACT]:,} exact definitions; "
        f"coverage {metrics['gross_coverage_percent']:.2f}%; "
        f"fidelity {metrics['fidelity_percent']:.2f}%"
    )
    return report


def load_report() -> dict:
    if not paths.DATA_REPORT.is_file():
        raise RuntimeError("no data report; run `python3 -m vostok data refresh`")
    return json.loads(paths.DATA_REPORT.read_text(encoding="utf-8"))


def _file_hash(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as source:
        while chunk := source.read(1024 * 1024):
            digest.update(chunk)
    return digest.hexdigest()
