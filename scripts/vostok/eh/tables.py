# SPDX-License-Identifier: GPL-3.0-or-later

"""vostok.eh.tables - one image's C++ EH registrations, parsed.

MSVC 2008 x86 gives every function that has to unwind a frame-handler stub:

    __ehhandler$fn:  mov edx,[esp+8] / lea eax,[edx+0Ch] / mov ecx,[edx-N]
                     xor ecx,eax / call __security_check_cookie   (with /GS)
                     mov eax, offset __ehfuncinfo$fn
                     jmp ___CxxFrameHandler3

and the function's prologue registers it (`push -1 / push offset
__ehhandler$fn / mov eax, fs:[0]`). The image is scanned for that tail, the
FuncInfo it loads is validated by its magic, and the base relocation table
then yields every absolute reference to the stub - the parent's `push`. From
the FuncInfo the unwind map, try blocks, handler types and funclets follow.

Neither the stub, the tables nor the `.text$x` funclets have a PDB function
record, which is why nothing else in the toolchain sees them.
"""

from __future__ import annotations

import bisect
import dataclasses
import re
import struct
from pathlib import Path

from vostok.data.pe import PEImage
from vostok.eh import x86
from vostok.eh.msf import SymbolTable, read_code_symbols

MAGICS = (0x19930520, 0x19930521, 0x19930522)
_TAIL = re.compile(rb"\xB8(....)\xE9(....)", re.S)
_STUB_REACH = 64      # bytes a stub may span before its `mov eax, offset FuncInfo`
_FUNCLET_BYTES = 256  # decode window for one funclet

# HandlerType.adjectives
ADJECTIVES = ((1, "const"), (2, "volatile"), (4, "unaligned"), (8, "reference"),
              (0x10, "resumable"), (0x40, "std-dotdot"), (0x80000000, "complus"))


@dataclasses.dataclass
class Ref:
    """One code reference out of a funclet or stub: a call/jmp or an address."""
    at: int        # offset inside the funclet
    kind: str      # "call" / "jmp" / "jcc" / "abs"
    target: int    # RVA
    name: str | None


@dataclasses.dataclass
class Funclet:
    rva: int
    size: int
    raw: bytes
    text: list[str]
    refs: list[Ref]
    frame: str | None  # the memory operand that names the object (`[ebp-0x1c]`)
    masked: bytes      # reloc/rel32 operands zeroed
    shape: bytes       # `masked`, with the frame displacement zeroed too
    error: str | None = None

    @property
    def callee(self) -> str | None:
        """What the funclet destroys WITH: the last call/jmp target."""
        calls = [r for r in self.refs if r.kind in ("call", "jmp")]
        if not calls:
            return None
        ref = calls[-1]
        return ref.name or f"rva={ref.target:#x}"

    def summary(self) -> str:
        return "; ".join(self.text)


@dataclasses.dataclass
class Handler:
    adjectives: int
    type_rva: int | None
    type_name: str      # TypeDescriptor name, "..." for catch(...)
    disp_catch_obj: int
    handler_rva: int

    def spelling(self) -> str:
        adj = [n for bit, n in ADJECTIVES if self.adjectives & bit]
        return " ".join([*adj, self.type_name])


@dataclasses.dataclass
class TryBlock:
    low: int
    high: int
    catch_high: int
    handlers: list[Handler]


@dataclasses.dataclass
class FuncInfo:
    rva: int
    magic: int
    max_state: int
    unwind: list[tuple[int, int | None]]   # (toState, action RVA or None)
    try_blocks: list[TryBlock]
    n_ip_map: int
    es_types: list[str] | None
    eh_flags: int


@dataclasses.dataclass
class Registration:
    """One function's EH registration: parent, stub, FuncInfo, funclets."""
    parent_rva: int | None
    parent_names: list[str]
    site_rva: int
    stub_rva: int
    stub: Funclet
    info: FuncInfo
    funclets: dict[int, Funclet]   # action RVA -> decoded funclet
    handlers: dict[int, Funclet]   # catch handler RVA outside the parent -> funclet

    @property
    def display(self) -> str:
        if self.parent_names:
            return self.parent_names[-1] if len(self.parent_names) > 1 else self.parent_names[0]
        return f"sub@rva={self.site_rva:#x}"


# SEH frame handlers, by the PDB name the registration pushes or calls
SEH_HANDLERS = {"__except_handler3": "seh3", "__except_handler4": "seh4",
                "__SEH_prolog4": "seh4", "__SEH_prolog4_GS": "seh4"}
_SEH_PROLOG_CALL = re.compile(rb"\x68(....)\xE8(....)", re.S)


@dataclasses.dataclass
class Scope:
    enclosing: int
    filter_rva: int | None  # None: a __finally
    handler_rva: int

    @property
    def kind(self) -> str:
        return "__finally" if self.filter_rva is None else "__except"


@dataclasses.dataclass
class SehFrame:
    """One function's SEH registration (`__try`): its scope table."""
    parent_rva: int | None
    parent_names: list[str]
    site_rva: int
    flavor: str                    # seh3 / seh4
    table_rva: int
    cookies: tuple[int, ...] | None  # SEH4 GS/EH cookie offsets
    scopes: list[Scope]

    display = Registration.display


class Image:
    """One side: the PE, its PDB code symbols, and every EH registration."""

    def __init__(self, side: str, exe: Path, pdb: Path | None,
                 symbols: SymbolTable | None = None):
        self.side = side
        self.exe = Path(exe)
        self.pdb = Path(pdb) if pdb else None
        self.pe = PEImage(exe)
        if symbols is None:
            section_rvas = tuple(s.rva for s in self.pe.sections)
            symbols = SymbolTable(
                read_code_symbols(self.pdb, section_rvas) if self.pdb else [])
        self.symbols = symbols
        self.relocs = self.pe.base_relocations()
        self.reloc_set = frozenset(self.relocs)
        self.registrations: list[Registration] = []
        self.seh_frames: list[SehFrame] = []
        self._referenced: frozenset[int] | None = None
        self.funcinfos: dict[int, FuncInfo] = {}
        self.stubs: dict[int, int] = {}  # stub RVA -> FuncInfo RVA
        self.errors: list[str] = []
        self._scan()
        self._scan_seh()

    # -- primitives ---------------------------------------------------------
    def _in(self, rva: int, name: str, size: int = 1) -> bool:
        sec = self.pe.section_at(rva)
        return sec is not None and sec.name == name and sec.contains(rva, size)

    def _va(self, value: int) -> int | None:
        rva = value - self.pe.image_base
        return rva if self.pe.section_at(rva) else None

    def label(self, rva: int) -> str | None:
        """The PDB name at `rva`, or the EH table it is; None when unnamed."""
        if rva in self.funcinfos:
            return "__ehfuncinfo"
        return self.symbols.name(rva)

    def u32(self, rva: int) -> int:
        return self.pe.u32_rva(rva)

    def i32(self, rva: int) -> int:
        return struct.unpack("<i", self.pe.read_rva(rva, 4))[0]

    def cstr(self, rva: int, limit: int = 4096) -> str:
        sec = self.pe.section_at(rva)
        if sec is None:
            return "?"
        blob = self.pe.read_rva(rva, min(limit, sec.end - rva))
        return blob.split(b"\0", 1)[0].decode("latin-1")

    # -- scanning -----------------------------------------------------------
    def _scan(self) -> None:
        text = self.pe.section(".text")
        blob = self.pe.read_rva(text.rva, text.virtual_size)
        tails: dict[int, int] = {}
        for m in _TAIL.finditer(blob):
            info = self._va(struct.unpack("<I", m.group(1))[0])
            if info is None or not self._in(info, ".rdata", 0x10):
                continue
            if self.u32(info) not in MAGICS:
                continue
            tails[text.rva + m.start()] = info
        tail_starts = sorted(tails)

        refs: dict[int, list[int]] = {}
        for site in self.relocs:
            if not self._in(site, ".text", 4):
                continue
            target = self._va(self.u32(site))
            if target is None or not self._in(target, ".text"):
                continue
            i = bisect.bisect_left(tail_starts, target)
            if i == len(tail_starts) or tail_starts[i] - target > _STUB_REACH:
                continue
            tail = tail_starts[i]
            if target != tail and not self._decodes_to(target, tail):
                continue
            if target <= site < tail + 10:
                continue  # inside the stub itself
            refs.setdefault(target, []).append(site)

        for stub_rva, sites in sorted(refs.items()):
            tail = tail_starts[bisect.bisect_left(tail_starts, stub_rva)]
            info_rva = tails[tail]
            self.stubs[stub_rva] = info_rva
            info = self.funcinfos.get(info_rva)
            if info is None:
                try:
                    info = self._funcinfo(info_rva)
                except (ValueError, struct.error) as exc:
                    self.errors.append(f"FuncInfo rva={info_rva:#x}: {exc}")
                    continue
                self.funcinfos[info_rva] = info
            stub = self.funclet(stub_rva, tail + 10 - stub_rva)
            for site in sorted(sites):
                parent = self.symbols.containing(site)
                names = self.symbols.names(parent) if parent is not None else []
                funclets = {}
                for _, action in info.unwind:
                    if action is not None and action not in funclets:
                        funclets[action] = self.funclet(action)
                handlers = {}
                for tb in info.try_blocks:
                    for h in tb.handlers:
                        if parent is None or self.symbols.containing(h.handler_rva) != parent:
                            handlers.setdefault(h.handler_rva, self.funclet(h.handler_rva))
                self.registrations.append(Registration(
                    parent, names, site, stub_rva, stub, info, funclets, handlers))
        # registrations whose stub is reached but no relocation names a parent
        reached = set(self.stubs.values())
        for info_rva in sorted(set(tails.values()) - reached):
            self.errors.append(f"FuncInfo rva={info_rva:#x}: no parent references its stub")

    # -- SEH ----------------------------------------------------------------
    def _scan_seh(self) -> None:
        """`push offset scopetable / push offset __except_handlerN` and
        `push offset scopetable / call __SEH_prolog4`: one frame each."""
        handlers = {rva: (name, flavor) for name, flavor in SEH_HANDLERS.items()
                    for rva in self.symbols.by_name.get(name, ())}
        if not handlers:
            return
        found: dict[int, tuple[int, str]] = {}  # site -> (table, flavor)
        for site in self.relocs:
            if not self._in(site, ".text", 4) or site - 6 < 0:
                continue
            target = self._va(self.u32(site))
            if target not in handlers or not handlers[target][0].startswith("__except"):
                continue
            if self.pe.read_rva(site - 6, 1) == b"\x68" and site - 5 in self.reloc_set:
                table = self._va(self.u32(site - 5))
                if table is not None:
                    found[site - 5] = (table, handlers[target][1])
        text = self.pe.section(".text")
        blob = self.pe.read_rva(text.rva, text.virtual_size)
        for m in _SEH_PROLOG_CALL.finditer(blob):
            call = text.rva + m.start() + 5
            dest = call + 5 + struct.unpack("<i", m.group(2))[0]
            if dest in handlers and call - 4 in self.reloc_set:
                table = self._va(struct.unpack("<I", m.group(1))[0])
                if table is not None:
                    found[call - 4] = (table, handlers[dest][1])
        for site, (table, flavor) in sorted(found.items()):
            parent = self.symbols.containing(site)
            names = self.symbols.names(parent) if parent is not None else []
            try:
                cookies, scopes = self._scope_table(table, flavor, parent)
            except (ValueError, struct.error) as exc:
                self.errors.append(f"scope table rva={table:#x}: {exc}")
                continue
            self.seh_frames.append(SehFrame(parent, names, site, flavor, table, cookies,
                                            scopes))

    def _referenced_rvas(self) -> frozenset[int]:
        if self._referenced is None:
            out = set()
            for site in self.relocs:
                target = self._va(self.u32(site))
                if target is not None:
                    out.add(target)
            self._referenced = frozenset(out)
        return self._referenced

    def _max_trylevel(self, parent: int | None) -> int | None:
        """Highest `mov dword [ebp-4], imm` the parent stores, if it has an extent."""
        size = self.symbols.extent.get(parent) if parent is not None else None
        if not size:
            return None
        blob = self.pe.read_rva(parent, size)
        levels = [struct.unpack("<i", m.group(1))[0]
                  for m in re.finditer(rb"\xC7\x45\xFC(....)", blob, re.S)]
        levels = [lv for lv in levels if 0 <= lv < 64]
        return max(levels) if levels else None

    def _scope_table(self, table: int, flavor: str, parent: int | None):
        cookies = None
        records = table
        top = -1
        if flavor == "seh4":
            cookies = struct.unpack("<iIiI", self.pe.read_rva(table, 16))
            records, top = table + 16, -2
        bound = self._max_trylevel(parent)
        limit = bound + 1 if bound is not None else 64
        referenced = self._referenced_rvas()
        scopes: list[Scope] = []
        for i in range(limit):
            rec = records + 12 * i
            if i and rec in referenced:
                break  # the next table starts here
            if not self._in(rec, ".rdata", 12):
                break
            enclosing, filt, handler = struct.unpack("<iII", self.pe.read_rva(rec, 12))
            if not (enclosing == top or 0 <= enclosing < i):
                break
            h = self._va(handler) if rec + 8 in self.reloc_set else None
            if h is None or not self._in(h, ".text"):
                break
            f = None
            if filt:
                f = self._va(filt) if rec + 4 in self.reloc_set else None
                if f is None or not self._in(f, ".text"):
                    break
            scopes.append(Scope(enclosing, f, h))
        if not scopes:
            raise ValueError("no valid scope record")
        return cookies, scopes

    def _decodes_to(self, start: int, tail: int) -> bool:
        blob = self.pe.read_rva(start, tail - start)
        cursor = 0
        try:
            while cursor < len(blob):
                insn = x86.decode(blob, cursor)
                if x86.is_terminator(insn):
                    return False
                cursor += insn.length
        except (x86.DecodeError, IndexError, struct.error):
            return False
        return cursor == len(blob)

    def _funcinfo(self, rva: int) -> FuncInfo:
        magic, max_state, p_unwind, n_try, p_try, n_ip, _p_ip = struct.unpack(
            "<IiIIIII", self.pe.read_rva(rva, 28))
        es_types = None
        eh_flags = 0
        if magic >= 0x19930521:
            p_es = self.u32(rva + 28)
            if p_es:
                es_rva = self._va(p_es)
                if es_rva is None:
                    raise ValueError("pESTypeList outside the image")
                count, p_types = struct.unpack("<iI", self.pe.read_rva(es_rva, 8))
                types_rva = self._va(p_types) if p_types else None
                es_types = [self._handler(types_rva + 16 * k).spelling()
                            for k in range(count)] if types_rva is not None else []
        if magic >= 0x19930522:
            eh_flags = self.u32(rva + 32)
        if not 0 <= max_state <= 0x10000 or not 0 <= n_try <= 0x1000:
            raise ValueError(f"implausible maxState={max_state} nTryBlocks={n_try}")
        unwind = []
        if max_state:
            base = self._va(p_unwind)
            if base is None:
                raise ValueError("pUnwindMap outside the image")
            for k in range(max_state):
                to_state = self.i32(base + 8 * k)
                action = self.u32(base + 8 * k + 4)
                unwind.append((to_state, self._va(action) if action else None))
        tries = []
        if n_try:
            base = self._va(p_try)
            if base is None:
                raise ValueError("pTryBlockMap outside the image")
            for k in range(n_try):
                low, high, catch_high, n_catches, p_handlers = struct.unpack(
                    "<iiiiI", self.pe.read_rva(base + 20 * k, 20))
                h_base = self._va(p_handlers) if n_catches else None
                handlers = [self._handler(h_base + 16 * j) for j in range(n_catches)] \
                    if h_base is not None else []
                tries.append(TryBlock(low, high, catch_high, handlers))
        return FuncInfo(rva, magic, max_state, unwind, tries, n_ip, es_types, eh_flags)

    def _handler(self, rva: int) -> Handler:
        adjectives, p_type, disp, p_handler = struct.unpack(
            "<IIiI", self.pe.read_rva(rva, 16))
        type_rva = self._va(p_type) if p_type else None
        name = self.cstr(type_rva + 8) if type_rva is not None else "..."
        handler = self._va(p_handler) if p_handler else 0
        return Handler(adjectives, type_rva, name, disp, handler or 0)

    def funclet(self, rva: int, size: int | None = None) -> Funclet:
        """Decode the code at `rva`: its extent, references and frame operand."""
        sec = self.pe.section_at(rva)
        window = min(size or _FUNCLET_BYTES, (sec.end - rva) if sec else 0)
        blob = self.pe.read_rva(rva, window) if window > 0 else b""
        error = None
        try:
            insns = x86.walk(blob) if size is None else _walk_all(blob)
        except (x86.DecodeError, IndexError, struct.error) as exc:
            insns, error = [], str(exc)
        length = sum(i.length for i in insns)
        raw = blob[:length]
        masked = bytearray(raw)
        shape = bytearray(raw)
        refs: list[Ref] = []
        frame = None
        for insn in insns:
            external = insn.branch is not None and (
                insn.opcode in (0xE8, 0xE9)
                or (insn.opcode >= 0x0F80 and not 0 <= insn.branch < length))
            if external:
                target = rva + insn.branch
                kind = {0xE8: "call", 0xE9: "jmp"}.get(insn.opcode, "jcc")
                refs.append(Ref(insn.offset, kind, target, self.label(target)))
                at = insn.offset + insn.imm_at
                masked[at:at + 4] = shape[at:at + 4] = b"\0\0\0\0"
            for k in range(insn.length - 3):
                site = rva + insn.offset + k
                if site in self.reloc_set:
                    target = self._va(self.u32(site))
                    if target is not None:
                        refs.append(Ref(insn.offset + k, "abs", target,
                                        self.label(target)))
                    at = insn.offset + k
                    masked[at:at + 4] = shape[at:at + 4] = b"\0\0\0\0"
            if insn.mem and frame is None and insn.mem.startswith(("[ebp", "[esp")):
                frame = insn.mem
            if insn.mem and insn.disp_at is not None and insn.mem.startswith(("[ebp", "[esp", "[edx")):
                width = 1 if insn.modrm is not None and insn.modrm >> 6 == 1 else 4
                at = insn.offset + insn.disp_at
                shape[at:at + width] = bytes(width)
        text = _name_branches([x86.render(i, raw) for i in insns], insns, refs)
        return Funclet(rva, length, raw, text, refs, frame, bytes(masked), bytes(shape),
                       error)


def _walk_all(blob: bytes) -> list[x86.Insn]:
    insns, cursor = [], 0
    while cursor < len(blob):
        insn = x86.decode(blob, cursor)
        insns.append(insn)
        cursor += insn.length
    return insns


def _name_branches(text: list[str], insns: list[x86.Insn], refs: list[Ref]) -> list[str]:
    by_at = {r.at: r for r in refs if r.kind in ("call", "jmp", "jcc")}
    out = []
    for spelled, insn in zip(text, insns):
        ref = by_at.get(insn.offset)
        if ref is not None:
            out.append(f"{ref.kind} {ref.name or f'rva={ref.target:#x}'}")
        elif insn.branch is not None:
            out.append(f"j{'mp' if insn.opcode == 0xEB else 'cc'} +{insn.branch:#x}")
        else:
            abs_ref = next((r for r in refs if r.kind == "abs" and
                            insn.offset <= r.at < insn.offset + insn.length), None)
            if abs_ref is not None and insn.opcode in (0x68, 0xB8):
                name = abs_ref.name or f"rva={abs_ref.target:#x}"
                out.append(("push offset " if insn.opcode == 0x68 else "mov eax, offset ")
                           + name)
            else:
                out.append(spelled)
    return out
