# SPDX-License-Identifier: GPL-3.0-or-later

"""vostok.eh.x86 - just enough IA-32 to walk an EH funclet.

An unwind funclet is a few instructions that load `this` and tail-call a
destructor (`lea ecx,[ebp-1Ch] / jmp ??1foo@@QAE@XZ`), or push a pointer and
call `operator delete`, sometimes behind a `test`/`je` on a construction flag.
Walking one needs instruction LENGTHS, the rel32 call/jmp targets, which bytes
are relocated operands (masked before comparing), and the frame operand that
names the destroyed object. This module decodes exactly that, for any 32-bit
instruction MSVC 2008 emits; it is not a disassembler.
"""

from __future__ import annotations

import dataclasses
import struct

_PREFIXES = frozenset((0x26, 0x2E, 0x36, 0x3E, 0x64, 0x65, 0x66, 0x67, 0xF0, 0xF2, 0xF3))

# one-byte opcode -> (has_modrm, immediate kind); kinds: 0 none, 'b' imm8,
# 'z' imm16/32 by operand size, 'w' imm16, 'wb' imm16+imm8, 'p' ptr16:32,
# 'm' moffs32, 'r8'/'r32' relative branch
_ONE: dict[int, tuple[bool, object]] = {}
for _base in range(0x00, 0x40, 8):
    for _i in range(4):
        _ONE[_base + _i] = (True, 0)
    _ONE[_base + 4] = (False, "b")
    _ONE[_base + 5] = (False, "z")
    _ONE[_base + 6] = (False, 0)
    _ONE[_base + 7] = (False, 0)
for _op in range(0x40, 0x62):
    _ONE[_op] = (False, 0)
_ONE.update({0x62: (True, 0), 0x63: (True, 0), 0x68: (False, "z"), 0x69: (True, "z"),
             0x6A: (False, "b"), 0x6B: (True, "b")})
for _op in range(0x6C, 0x70):
    _ONE[_op] = (False, 0)
for _op in range(0x70, 0x80):
    _ONE[_op] = (False, "r8")
_ONE.update({0x80: (True, "b"), 0x81: (True, "z"), 0x82: (True, "b"), 0x83: (True, "b")})
for _op in range(0x84, 0x90):
    _ONE[_op] = (True, 0)
for _op in range(0x90, 0xA0):
    _ONE[_op] = (False, 0)
_ONE[0x9A] = (False, "p")
for _op in range(0xA0, 0xA4):
    _ONE[_op] = (False, "m")
for _op in range(0xA4, 0xB0):
    _ONE[_op] = (False, 0)
_ONE[0xA8] = (False, "b")
_ONE[0xA9] = (False, "z")
for _op in range(0xB0, 0xB8):
    _ONE[_op] = (False, "b")
for _op in range(0xB8, 0xC0):
    _ONE[_op] = (False, "z")
_ONE.update({0xC0: (True, "b"), 0xC1: (True, "b"), 0xC2: (False, "w"), 0xC3: (False, 0),
             0xC4: (True, 0), 0xC5: (True, 0), 0xC6: (True, "b"), 0xC7: (True, "z"),
             0xC8: (False, "wb"), 0xC9: (False, 0), 0xCA: (False, "w"), 0xCB: (False, 0),
             0xCC: (False, 0), 0xCD: (False, "b"), 0xCE: (False, 0), 0xCF: (False, 0)})
for _op in range(0xD0, 0xD4):
    _ONE[_op] = (True, 0)
_ONE.update({0xD4: (False, "b"), 0xD5: (False, "b"), 0xD6: (False, 0), 0xD7: (False, 0)})
for _op in range(0xD8, 0xE0):
    _ONE[_op] = (True, 0)
for _op in range(0xE0, 0xE4):
    _ONE[_op] = (False, "r8")
_ONE.update({0xE4: (False, "b"), 0xE5: (False, "b"), 0xE6: (False, "b"), 0xE7: (False, "b"),
             0xE8: (False, "r32"), 0xE9: (False, "r32"), 0xEA: (False, "p"),
             0xEB: (False, "r8")})
for _op in (0xEC, 0xED, 0xEE, 0xEF, 0xF1, 0xF4, 0xF5, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD):
    _ONE[_op] = (False, 0)
_ONE.update({0xF6: (True, "F6"), 0xF7: (True, "F7"), 0xFE: (True, 0), 0xFF: (True, 0)})

_TWO_NO_MODRM = frozenset((0x05, 0x06, 0x07, 0x08, 0x09, 0x0B, 0x0E, 0x30, 0x31, 0x32,
                           0x33, 0x34, 0x35, 0x37, 0x77, 0xA0, 0xA1, 0xA2, 0xA8, 0xA9,
                           0xAA, *range(0xC8, 0xD0)))
_TWO_IMM8 = frozenset((0x70, 0x71, 0x72, 0x73, 0xA4, 0xAC, 0xBA, 0xC2, 0xC4, 0xC5, 0xC6))

_REG32 = ("eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi")


@dataclasses.dataclass(frozen=True)
class Insn:
    offset: int          # from the start of the decoded buffer
    length: int
    opcode: int          # 0x0Fxx for two-byte opcodes
    modrm: int | None
    disp: int | None     # signed memory displacement
    disp_at: int | None  # offset of the displacement inside the instruction
    imm: int | None
    imm_at: int | None
    branch: int | None   # buffer offset a relative call/jmp/jcc lands on
    mem: str | None      # "[ebp-0x1c]", "[esp+0x10]", ... for a memory operand
    reg: int | None      # ModR/M reg field


class DecodeError(ValueError):
    pass


def _memory(blob: bytes, at: int, modrm: int) -> tuple[int, int | None, int | None, str | None]:
    """-> (bytes after ModR/M, disp, disp offset relative to `at`, operand text)."""
    mod, rm = modrm >> 6, modrm & 7
    if mod == 3:
        return 0, None, None, None
    used, base, index = 0, _REG32[rm], None
    if rm == 4:
        sib = blob[at]
        used = 1
        scale, idx, b = 1 << (sib >> 6), (sib >> 3) & 7, sib & 7
        base = None if (b == 5 and mod == 0) else _REG32[b]
        if idx != 4:
            index = f"{_REG32[idx]}*{scale}" if scale > 1 else _REG32[idx]
    elif rm == 5 and mod == 0:
        base = None
    if mod == 1:
        disp = struct.unpack_from("<b", blob, at + used)[0]
        width = 1
    elif mod == 2 or base is None:
        disp = struct.unpack_from("<i", blob, at + used)[0]
        width = 4
    else:
        disp, width = 0, 0
    parts = [p for p in (base, index) if p]
    text = "+".join(parts)
    if width:
        if parts:
            text += f"-{-disp:#x}" if disp < 0 else f"+{disp:#x}"
        else:
            text = f"{disp & 0xFFFFFFFF:#x}"
    return used + width, (disp if width else None), (used if width else None), f"[{text}]"


def decode(blob: bytes, offset: int) -> Insn:
    start, cursor, opsize = offset, offset, 4
    while cursor < len(blob) and blob[cursor] in _PREFIXES:
        if blob[cursor] == 0x66:
            opsize = 2
        cursor += 1
    if cursor >= len(blob):
        raise DecodeError(f"truncated at +{offset:#x}")
    op = blob[cursor]
    cursor += 1
    if op == 0x0F:
        op2 = blob[cursor]
        cursor += 1
        opcode = 0x0F00 | op2
        if 0x80 <= op2 <= 0x8F:
            has_modrm, imm = False, "r32"
        elif op2 in _TWO_NO_MODRM:
            has_modrm, imm = False, 0
        elif op2 in (0x38, 0x3A):
            cursor += 1  # third opcode byte
            has_modrm, imm = True, ("b" if op2 == 0x3A else 0)
        else:
            has_modrm, imm = True, ("b" if op2 in _TWO_IMM8 else 0)
    else:
        opcode = op
        if op not in _ONE:
            raise DecodeError(f"opcode {op:#04x} at +{offset:#x}")
        has_modrm, imm = _ONE[op]
    modrm = disp = disp_at = mem = reg = None
    if has_modrm:
        modrm = blob[cursor]
        reg = (modrm >> 3) & 7
        cursor += 1
        used, disp, rel, mem = _memory(blob, cursor, modrm)
        disp_at = None if rel is None else cursor + rel - start
        cursor += used
        if imm == "F6":
            imm = "b" if reg in (0, 1) else 0
        elif imm == "F7":
            imm = "z" if reg in (0, 1) else 0
    value = imm_at = branch = None
    width = {"b": 1, "w": 2, "wb": 3, "p": 6, "m": 4, "r8": 1, "r32": 4,
             "z": opsize}.get(imm, 0) if imm else 0
    if width:
        imm_at = cursor - start
        if imm in ("r8", "r32"):
            rel = struct.unpack_from("<b" if imm == "r8" else "<i", blob, cursor)[0]
            branch = cursor + width + rel
        elif width in (1, 2, 4):
            value = int.from_bytes(blob[cursor:cursor + width], "little")
        cursor += width
    if imm == "m":
        disp_at, mem = imm_at, f"[{value:#x}]"
    if cursor > len(blob):
        raise DecodeError(f"truncated at +{offset:#x}")
    return Insn(start, cursor - start, opcode, modrm, disp, disp_at, value, imm_at,
                branch, mem, reg)


def is_terminator(insn: Insn) -> bool:
    """ret / jmp: control does not fall through."""
    if insn.opcode in (0xC3, 0xC2, 0xE9, 0xEB):
        return True
    return insn.opcode == 0xFF and insn.reg == 4


def walk(blob: bytes, limit: int = 96) -> list[Insn]:
    """Decode a funclet from offset 0 until a terminator that no earlier
    forward branch skips past (the `je` over a flagged destructor)."""
    insns: list[Insn] = []
    cursor = reach = 0
    while cursor < len(blob) and len(insns) < limit:
        insn = decode(blob, cursor)
        insns.append(insn)
        cursor += insn.length
        if insn.branch is not None and insn.opcode not in (0xE8,) and insn.branch > reach:
            if not is_terminator(insn) or insn.opcode == 0xEB:
                reach = insn.branch
        if is_terminator(insn) and cursor > reach:
            break
    return insns


def render(insn: Insn, blob: bytes) -> str:
    """A short spelling for the handful of shapes a funclet is made of."""
    op = insn.opcode
    reg = _REG32[insn.reg] if insn.reg is not None else None
    if op == 0x8D and insn.mem:
        return f"lea {reg},{insn.mem}"
    if op == 0x8B and insn.mem:
        return f"mov {reg},{insn.mem}"
    if op == 0x89 and insn.mem:
        return f"mov {insn.mem},{reg}"
    if 0x50 <= op <= 0x57:
        return f"push {_REG32[op - 0x50]}"
    if 0x58 <= op <= 0x5F:
        return f"pop {_REG32[op - 0x58]}"
    if op == 0xFF and insn.reg == 6 and insn.mem:
        return f"push dword {insn.mem}"
    if op in (0x68, 0x6A):
        return f"push {insn.imm:#x}"
    if op == 0xC3:
        return "ret"
    if op == 0xC2:
        return f"ret {insn.imm:#x}"
    alu = ("add", "or", "adc", "sbb", "and", "sub", "xor", "cmp")
    if op in (0x83, 0x81) and insn.modrm is not None:
        dest = insn.mem and f"dword {insn.mem}" or _REG32[insn.modrm & 7]
        return f"{alu[insn.reg]} {dest},{insn.imm:#x}"
    if op < 0x40 and op & 7 in (1, 3) and insn.modrm is not None and insn.modrm >> 6 == 3:
        a, b = _REG32[insn.reg], _REG32[insn.modrm & 7]
        return f"{alu[op >> 3]} {a},{b}" if op & 2 else f"{alu[op >> 3]} {b},{a}"
    if op == 0x85 and insn.modrm is not None and insn.modrm >> 6 == 3:
        return f"test {_REG32[insn.modrm & 7]},{reg}"
    if 0xB8 <= op <= 0xBF:
        return f"mov {_REG32[op - 0xB8]},{insn.imm:#x}"
    return blob[insn.offset:insn.offset + insn.length].hex(" ")
