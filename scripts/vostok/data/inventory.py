"""PDB data-index records and stable cross-build identities."""

from __future__ import annotations

import csv
import dataclasses
import json
import re
from collections import defaultdict
from pathlib import Path, PureWindowsPath

from vostok.core.paths import REPORT
from vostok.derive.modules import module_of


@dataclasses.dataclass(frozen=True)
class DataSymbol:
    rva: int
    section: str
    storage: str
    size: int | None
    size_kind: str
    type_index: int
    scope: str
    module_path: bytes
    archive_path: bytes
    name: bytes
    public_name: bytes
    identity: str
    owner_module: str

    @property
    def end(self) -> int | None:
        return self.rva + self.size if self.size is not None else None

    @property
    def display_name(self) -> str:
        return display_bytes(self.public_name or self.name)


def display_bytes(value: bytes) -> str:
    text = value.decode("utf-8", "backslashreplace")
    return text.replace("\t", "\\t").replace("\r", "\\r").replace("\n", "\\n")


def _win_basename(value: bytes) -> str:
    if not value:
        return "?"
    text = value.decode("utf-8", "backslashreplace").replace("/", "\\")
    return PureWindowsPath(text).name.lower()


def identity_for(scope: str, module: bytes, archive: bytes,
                 name: bytes, public_name: bytes,
                 owner_module: str | None = None) -> str:
    if scope == "external":
        return "E:" + display_bytes(public_name or name)
    return "L:{}:{}:{}".format(
        owner_module or _win_basename(archive), _win_basename(module), display_bytes(name)
    )


def _unit_modules() -> dict[str, str]:
    if not REPORT.is_file():
        return {}
    by_stem: dict[str, set[str]] = defaultdict(set)
    for unit in json.loads(REPORT.read_text(encoding="utf-8")).get("units", []):
        name = unit.get("name") or ""
        stem = Path(name).stem.lower()
        if stem:
            by_stem[stem].add(module_of(name.lower()))
    return {
        stem: next(iter(modules))
        for stem, modules in by_stem.items()
        if len(modules) == 1
    }


_VOSTOK_ARCHIVE = re.compile(r"vostok[_-]([a-z0-9_]+?)(?:-static)?(?:-gold)?\.lib$")
_KNOWN_VENDOR = (
    "boost", "stlport", "bullet", "vorbis", "ogg", "zlib", "opcode",
    "speedtree", "wildmagic", "fastdelegate", "gfx", "pcre", "expat",
)


def owner_for(module_path: bytes, archive_path: bytes,
              units: dict[str, str]) -> str:
    module_name = _win_basename(module_path)
    stem = module_name.rsplit(".", 1)[0]
    if stem in units:
        return units[stem]

    archive = _win_basename(archive_path)
    match = _VOSTOK_ARCHIVE.search(archive)
    if match:
        return match.group(1)
    if "libeay" in archive or "ssleay" in archive:
        return "openssl"
    for vendor in _KNOWN_VENDOR:
        if vendor in archive or vendor in module_name:
            return vendor

    path = module_path.decode("utf-8", "ignore").replace("/", "\\").lower()
    parts = [part for part in path.split("\\") if part]
    for marker in ("master gold", "master"):
        if marker in parts:
            index = parts.index(marker)
            if index + 1 < len(parts) - 1:
                return parts[index + 1]
    return "unknown"


def _owner_from_name(name: bytes) -> str:
    qualified = name.decode("utf-8", "ignore").lower()
    for namespace, module in (
        ("vostok::render::", "render"),
        ("vostok::animation::", "animation"),
        ("vostok::collision::", "collision"),
        ("vostok::physics::", "physics"),
        ("vostok::sound::", "sound"),
        ("vostok::network::", "network"),
        ("vostok::resources::", "resources"),
        ("scaleform::", "gfx"),
        ("survarium::", "game"),
        ("vostok::", "vostok"),
    ):
        if namespace in qualified:
            return module
    return "unknown"


def load(path: Path | str) -> list[DataSymbol]:
    units = _unit_modules()
    rows = []
    with Path(path).open(newline="", encoding="ascii") as source:
        reader = csv.DictReader(source, delimiter="\t")
        required = {
            "rva", "section", "storage", "size", "size_kind", "type_index",
            "scope", "module_hex", "archive_hex", "name_hex", "public_name_hex",
        }
        if set(reader.fieldnames or ()) != required:
            raise ValueError(f"{path}: unexpected data-index columns")
        for raw in reader:
            module_path = bytes.fromhex(raw["module_hex"])
            archive_path = bytes.fromhex(raw["archive_hex"])
            name = bytes.fromhex(raw["name_hex"])
            public_name = bytes.fromhex(raw["public_name_hex"])
            owner_module = owner_for(module_path, archive_path, units)
            if owner_module == "unknown":
                owner_module = _owner_from_name(name)
            rows.append(DataSymbol(
                rva=int(raw["rva"], 0),
                section=raw["section"],
                storage=raw["storage"],
                size=None if raw["size"] == "-" else int(raw["size"], 0),
                size_kind=raw["size_kind"],
                type_index=int(raw["type_index"], 0),
                scope=raw["scope"],
                module_path=module_path,
                archive_path=archive_path,
                name=name,
                public_name=public_name,
                identity=identity_for(
                    raw["scope"], module_path, archive_path, name, public_name,
                    owner_module,
                ),
                owner_module=owner_module,
            ))
    return rows
