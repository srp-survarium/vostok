# SPDX-License-Identifier: GPL-3.0-or-later

"""Small PE32 reader for data matching.

The data lane needs virtual section extents, loader-zero bytes, and HIGHLOW
base relocations.  Keeping that reader here avoids making objdiff's object
model authoritative for image data it currently does not represent.
"""

from __future__ import annotations

import dataclasses
import struct
from pathlib import Path


@dataclasses.dataclass(frozen=True)
class Section:
    name: str
    rva: int
    virtual_size: int
    raw_offset: int
    raw_size: int
    characteristics: int

    @property
    def end(self) -> int:
        return self.rva + self.virtual_size

    def contains(self, rva: int, size: int = 1) -> bool:
        return self.rva <= rva and rva + size <= self.end


class PEImage:
    def __init__(self, path: Path | str):
        self.path = Path(path)
        self.data = self.path.read_bytes()
        self.image_base, self.sections, self.reloc_rva, self.reloc_size = (
            self._parse_headers()
        )

    def _parse_headers(self) -> tuple[int, tuple[Section, ...], int, int]:
        if self.data[:2] != b"MZ":
            raise ValueError(f"{self.path}: not an MZ executable")
        pe_offset = self._u32(0x3C)
        if self.data[pe_offset:pe_offset + 4] != b"PE\0\0":
            raise ValueError(f"{self.path}: missing PE signature")
        coff = pe_offset + 4
        section_count = self._u16(coff + 2)
        optional_size = self._u16(coff + 16)
        optional = coff + 20
        if self._u16(optional) != 0x10B:
            raise ValueError(f"{self.path}: data matcher supports PE32 only")
        image_base = self._u32(optional + 28)
        directory_count = self._u32(optional + 92)
        reloc_rva = reloc_size = 0
        if directory_count > 5:
            reloc_rva = self._u32(optional + 96 + 5 * 8)
            reloc_size = self._u32(optional + 96 + 5 * 8 + 4)

        sections = []
        cursor = optional + optional_size
        for _ in range(section_count):
            name = self.data[cursor:cursor + 8].split(b"\0", 1)[0].decode(
                "ascii", "replace"
            )
            virtual_size = self._u32(cursor + 8)
            rva = self._u32(cursor + 12)
            raw_size = self._u32(cursor + 16)
            raw_offset = self._u32(cursor + 20)
            characteristics = self._u32(cursor + 36)
            sections.append(Section(
                name, rva, virtual_size, raw_offset, raw_size, characteristics
            ))
            cursor += 40
        return image_base, tuple(sections), reloc_rva, reloc_size

    def section(self, name: str) -> Section:
        for section in self.sections:
            if section.name == name:
                return section
        raise KeyError(f"{self.path}: no {name} section")

    def section_at(self, rva: int) -> Section | None:
        return next((section for section in self.sections if section.contains(rva)), None)

    def read_rva(self, rva: int, size: int) -> bytes:
        if size < 0:
            raise ValueError("negative PE read")
        if size == 0:
            return b""
        section = self.section_at(rva)
        if section is None or not section.contains(rva, size):
            raise ValueError(f"{self.path}: RVA {rva:#x}+{size:#x} leaves a section")
        offset = rva - section.rva
        raw_available = max(0, min(size, section.raw_size - offset))
        raw_start = section.raw_offset + offset
        raw = self.data[raw_start:raw_start + raw_available]
        if len(raw) != raw_available:
            raise ValueError(f"{self.path}: truncated raw bytes for RVA {rva:#x}")
        return raw + bytes(size - len(raw))

    def u32_rva(self, rva: int) -> int:
        return struct.unpack("<I", self.read_rva(rva, 4))[0]

    def base_relocations(self) -> tuple[int, ...]:
        """Return every PE32 HIGHLOW relocation site as an RVA."""
        if not self.reloc_rva or not self.reloc_size:
            return ()
        blob = self.read_rva(self.reloc_rva, self.reloc_size)
        sites = []
        cursor = 0
        while cursor + 8 <= len(blob):
            page_rva, block_size = struct.unpack_from("<II", blob, cursor)
            if block_size < 8 or cursor + block_size > len(blob):
                break
            for entry_offset in range(cursor + 8, cursor + block_size, 2):
                entry = struct.unpack_from("<H", blob, entry_offset)[0]
                kind, offset = entry >> 12, entry & 0xFFF
                if kind == 3:  # IMAGE_REL_BASED_HIGHLOW
                    sites.append(page_rva + offset)
            cursor += block_size
        return tuple(sorted(set(sites)))

    def _u16(self, offset: int) -> int:
        return struct.unpack_from("<H", self.data, offset)[0]

    def _u32(self, offset: int) -> int:
        return struct.unpack_from("<I", self.data, offset)[0]
