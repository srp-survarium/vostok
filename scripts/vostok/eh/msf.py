# SPDX-License-Identifier: GPL-3.0-or-later

"""vostok.eh.msf - every code symbol a PDB names, read straight from the MSF.

The EH tables point at code no evidence database records: destructors in
prebuilt libraries, `operator delete`, `___CxxFrameHandler3`, the parents of
the handful of FuncInfos the engine links. The evidence DBs keep only
source-bodied procedures, the delinker's function index is a data-lane
artifact a `--code-only` build leaves stale, and `llvm-pdbutil` refuses the
retail PDB ("Too many directory blocks"). So this reads the two places a
linker PDB names code - the public symbol records (mangled) and each module's
S_*PROC32 records (demangled, with a size) - and nothing else.

Only MSF 7.00 with a block map that fits wherever the superblock says it
starts is supported; both survarium PDBs are that.
"""

from __future__ import annotations

import bisect
import dataclasses
import struct
from pathlib import Path

_MAGIC = b"Microsoft C/C++ MSF 7.00\r\n\x1aDS\0\0\0"

S_PUB32 = 0x110E
S_LPROC32 = 0x110F
S_GPROC32 = 0x1110
S_LPROC32_ID = 0x1146
S_GPROC32_ID = 0x1147
_PROCS = (S_LPROC32, S_GPROC32, S_LPROC32_ID, S_GPROC32_ID)


class _Msf:
    def __init__(self, data: bytes):
        if data[:len(_MAGIC)] != _MAGIC:
            raise ValueError("not an MSF 7.00 PDB")
        self.data = data
        (self.block, _free, _count, dir_bytes, _unk,
         map_addr) = struct.unpack_from("<6I", data, 32)
        dir_blocks = -(-dir_bytes // self.block)
        block_map = struct.unpack_from(f"<{dir_blocks}I", data, map_addr * self.block)
        directory = b"".join(self._blk(b) for b in block_map)[:dir_bytes]
        n = struct.unpack_from("<I", directory, 0)[0]
        sizes = struct.unpack_from(f"<{n}I", directory, 4)
        cursor = 4 + 4 * n
        self.streams: list[tuple[int, tuple[int, ...]]] = []
        for size in sizes:
            if size == 0xFFFFFFFF:
                self.streams.append((0, ()))
                continue
            k = -(-size // self.block)
            blocks = struct.unpack_from(f"<{k}I", directory, cursor)
            cursor += 4 * k
            self.streams.append((size, blocks))

    def _blk(self, index: int) -> bytes:
        return self.data[index * self.block:(index + 1) * self.block]

    def stream(self, index: int) -> bytes:
        if index == 0xFFFF or index >= len(self.streams):
            return b""
        size, blocks = self.streams[index]
        return b"".join(self._blk(b) for b in blocks)[:size]


@dataclasses.dataclass(frozen=True)
class CodeSymbol:
    rva: int
    size: int | None  # None: a public, whose record carries no extent
    name: str
    public: bool


def _cstr(blob: bytes, offset: int) -> str:
    end = blob.find(b"\0", offset)
    return blob[offset:end if end >= 0 else len(blob)].decode("latin-1")


def _records(blob: bytes, start: int, end: int):
    cursor = start
    while cursor + 4 <= end:
        length, kind = struct.unpack_from("<HH", blob, cursor)
        if length < 2:
            break
        yield kind, cursor + 4
        cursor += 2 + length


def read_code_symbols(pdb: Path, section_rvas: tuple[int, ...]) -> list[CodeSymbol]:
    """Publics and procedures of `pdb`, placed by the PE's section table.

    `section_rvas[i]` is the RVA of PE section i+1 - the PDB's segment numbers
    are 1-based indexes into the very same table."""
    msf = _Msf(Path(pdb).read_bytes())
    dbi = msf.stream(3)
    (_sig, _ver, _age, _gsi, _build, _psi, _dll, sym_stream, _rbld,
     mod_size) = struct.unpack_from("<iIIHHHHHHi", dbi, 0)

    def place(segment: int, offset: int) -> int | None:
        if not 1 <= segment <= len(section_rvas):
            return None
        return section_rvas[segment - 1] + offset

    out: list[CodeSymbol] = []
    records = msf.stream(sym_stream)
    for kind, body in _records(records, 0, len(records)):
        if kind != S_PUB32:
            continue
        flags, offset, segment = struct.unpack_from("<IIH", records, body)
        rva = place(segment, offset)
        if rva is not None and flags & 2:  # cvpsfFunction
            out.append(CodeSymbol(rva, None, _cstr(records, body + 10), True))

    cursor, end = 64, 64 + mod_size
    while cursor < end:
        sym_stream_index, sym_bytes = struct.unpack_from("<HI", dbi, cursor + 34)
        name_at = cursor + 64
        module_end = dbi.find(b"\0", dbi.find(b"\0", name_at) + 1) + 1
        cursor = (module_end + 3) & ~3
        if sym_stream_index == 0xFFFF:
            continue
        stream = msf.stream(sym_stream_index)
        for kind, body in _records(stream, 4, min(sym_bytes, len(stream))):
            if kind not in _PROCS:
                continue
            size, = struct.unpack_from("<I", stream, body + 12)
            offset, segment = struct.unpack_from("<IH", stream, body + 28)
            rva = place(segment, offset)
            if rva is not None:
                out.append(CodeSymbol(rva, size, _cstr(stream, body + 35), False))
    return out


class SymbolTable:
    """Name lookup by exact RVA, and the procedure that contains an RVA."""

    def __init__(self, symbols: list[CodeSymbol]):
        self.by_rva: dict[int, list[CodeSymbol]] = {}
        self.by_name: dict[str, set[int]] = {}
        for sym in symbols:
            self.by_rva.setdefault(sym.rva, []).append(sym)
            self.by_name.setdefault(sym.name, set()).add(sym.rva)
        self.starts = sorted(self.by_rva)
        # an extent per start: the largest procedure size recorded there, else
        # (publics only) up to the next start
        self.extent: dict[int, int | None] = {}
        for rva, syms in self.by_rva.items():
            sizes = [s.size for s in syms if s.size]
            self.extent[rva] = max(sizes) if sizes else None

    def names(self, rva: int) -> list[str]:
        """Every name at `rva`, mangled publics first (they compare across
        builds), then the demangled procedure names."""
        syms = self.by_rva.get(rva, ())
        pubs = sorted({s.name for s in syms if s.public})
        procs = sorted({s.name for s in syms if not s.public} - set(pubs))
        return pubs + procs

    def name(self, rva: int) -> str | None:
        names = self.names(rva)
        return names[0] if names else None

    def display(self, rva: int) -> str | None:
        """The demangled spelling when one exists, else the mangled one."""
        syms = self.by_rva.get(rva, ())
        procs = sorted(s.name for s in syms if not s.public)
        return procs[0] if procs else self.name(rva)

    def containing(self, rva: int) -> int | None:
        """Start RVA of the symbol whose code holds `rva`.

        A procedure record bounds itself; a bare public is assumed to run to
        the next symbol start, which is right for the linked .text it names."""
        i = bisect.bisect_right(self.starts, rva) - 1
        if i < 0:
            return None
        start = self.starts[i]
        extent = self.extent[start]
        if extent is not None and rva >= start + extent:
            return None
        return start
