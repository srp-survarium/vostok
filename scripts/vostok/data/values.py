# SPDX-License-Identifier: GPL-3.0-or-later

"""vostok data values - compare the VALUES that paired code reads from data.

The code audits compare instructions. Where an instruction addresses data
(`fld dword ptr [__real@...]`, `movss xmm0, [clear_value]`, `push offset
??_C@...`, `jmp [eax*4+table]`) the two sides necessarily name different
addresses, so a code comparison treats the operand as layout noise and never
looks at what is stored there. A wrong constant then survives every code
review: retail reads 1.0 where we read 0.0, through an instruction that is
otherwise identical.

This verb closes that hole. For every function pair (`sema.pairing`) it aligns
the two instruction streams (the PDB evidence disassembly, symbol operands
masked), pairs each data reference by its aligned instruction, reads the
referenced bytes from BOTH images and compares the values:

  direct reads/writes   the accessed width (`dword ptr`, `qword ptr`, ...)
  indexed accesses      the whole referenced datum when its extent is known
                        (bounded), else the operand width
  address immediates    `push offset X` / `mov reg, offset X` / `lea`:
                        dereferenced - a C or UTF-16 string, a `__real@`
                        constant, or the datum's complete extent when known
  switch tables in .text jump-table entries as aligned code offsets, the
                        byte index table up to the `cmp` bound
  immediates            with --immediates only: aligned instructions whose
                        literal operand differs (frame/stack adjustments
                        excluded; `__LINE__`-shaped values classed
                        EQUIVALENT). A literal is visible to code review
                        already, so this is opt-in.

Pointer words inside a compared window are compared by what they point at
(function pair, data identity), never by address, so relinking, ICF and
renamed folds compare SAME.

Classes: SAME (values agree), DIFFERENT (the leads), UNRESOLVABLE (no aligned
partner value could be read, or the pointed-at identities cannot be related),
EQUIVALENT (differs only by policy: `__LINE__`, build path or timestamp).

    python3 -m vostok data values                        # summary + DIFFERENT leads
    python3 -m vostok data values --module game_core     # one module
    python3 -m vostok data values --function weapon_dispersion
    python3 -m vostok data values --class all            # every compared reference

Every compared reference is written to binaries/gen/data_value_diff.tsv. It
reads the full build's data lane (access maps and data indexes in
binaries/gen), so run it after a full `vostok build`, not a `--code-only` one.
"""

from __future__ import annotations

import argparse
import bisect
import csv
import math
import re
import struct
import sys
from collections import Counter, defaultdict
from dataclasses import dataclass
from difflib import SequenceMatcher

from vostok.core import paths
from vostok.core import log as _log

SAME = "SAME"
DIFFERENT = "DIFFERENT"
UNRESOLVABLE = "UNRESOLVABLE"
EQUIVALENT = "EQUIVALENT"
CLASSES = (DIFFERENT, UNRESOLVABLE, EQUIVALENT, SAME)

COLUMNS = (
    "class", "kind", "module", "unit", "function", "pairing",
    "target_offset", "base_offset", "target_instruction_rva",
    "base_instruction_rva", "access", "width", "form", "compare_size",
    "target_value", "base_value", "target_datum", "base_datum", "source",
    "target_source", "instruction", "note",
)

#: the largest window compared for one reference; a table larger than this is
#: compared up to the bound and says so in its note.
TABLE_CAP = 4096
#: strings longer than this are compared by their first STRING_CAP bytes.
STRING_CAP = 4096

_WIDTHS = {"byte": 1, "word": 2, "dword": 4, "qword": 8, "tbyte": 10, "xmmword": 16}

_REGISTERS = frozenset(
    "eax ebx ecx edx esi edi ebp esp ax bx cx dx si di bp sp al bl cl dl ah bh ch dh "
    "st st0 st1 st2 st3 st4 st5 st6 st7 cs ds es fs gs ss".split()
    + [f"xmm{i}" for i in range(8)] + [f"mm{i}" for i in range(8)]
)
_KEYWORDS = frozenset(
    "byte word dword qword tbyte xmmword fword ptr offset short near far".split()
)
_IDENT = re.compile(r"(?<![\w?@$])[A-Za-z_?@$.][\w?@$<>:.`'~]*")
_NUMBER = re.compile(r"(?<![\w?@$])(?:0x[0-9a-fA-F]+|[0-9][0-9a-fA-F]*h?)(?![\w?@$])")
_REGISTER_OPERAND = re.compile(
    r"\[[^\]]*\b(?:e[abcd]x|e[sd]i|ebp|esp)\b[^\]]*\]", re.IGNORECASE)
_REAL = re.compile(r"__(real|xmm)@([0-9a-fA-F]+)")


# --------------------------------------------------------------------------
# instruction alignment


def _mnemonic(text: str) -> str:
    return text.split(None, 1)[0].lower() if text.strip() else ""


def _operands(text: str) -> str:
    parts = text.split(None, 1)
    return parts[1].strip() if len(parts) > 1 else ""


def _parse_number(token: str) -> int | None:
    token = token.strip()
    try:
        if token.lower().startswith("0x"):
            return int(token, 16)
        if token.lower().endswith("h"):
            return int(token[:-1], 16)
        return int(token, 10)
    except ValueError:
        return None


def normalize(text: str) -> str:
    """The instruction with every symbol operand masked.

    Both sides come from the same disassembler, so a symbolic operand is the
    only textual difference a pure relink introduces. Registers, size keywords
    and numbers stay: they are what the alignment keys on."""
    mnemonic = _mnemonic(text)

    def ident(match: re.Match) -> str:
        word = match.group(0)
        low = word.lower()
        if low in _REGISTERS or low in _KEYWORDS:
            return low
        if re.fullmatch(r"[0-9a-fA-F]+h", word):
            return low
        return "S"

    def memory(match: re.Match) -> str:
        # Inside brackets only registers survive: a datum may well be NAMED
        # like a keyword (render's `offset`).
        return _IDENT.sub(
            lambda m: m.group(0).lower() if m.group(0).lower() in _REGISTERS
            or re.fullmatch(r"[0-9a-fA-F]+h", m.group(0)) else "S",
            match.group(0))

    operands = re.sub(r"\[[^\]]*\]", memory, _operands(text))
    operands = _IDENT.sub(ident, operands)
    operands = re.sub(r"\s+", " ", operands)
    return f"{mnemonic} {operands}".strip()


def _shape(text: str) -> str:
    """`normalize` with every number masked too: same instruction, any value."""
    return _NUMBER.sub("N", normalize(text))


def align(target: list[dict], base: list[dict]) -> dict[int, int]:
    """target instruction offset -> base instruction offset.

    Content alignment (difflib) over normalized instructions; a replaced run of
    equal length is paired position by position where the instruction SHAPE
    (numbers masked) agrees, which is exactly where a differing immediate or
    displacement sits."""
    t_keys = [normalize(row["text"]) for row in target]
    b_keys = [normalize(row["text"]) for row in base]
    mapping: dict[int, int] = {}
    matcher = SequenceMatcher(None, t_keys, b_keys, autojunk=False)
    for tag, i1, i2, j1, j2 in matcher.get_opcodes():
        if tag == "equal":
            for k in range(i2 - i1):
                mapping[target[i1 + k]["off"]] = base[j1 + k]["off"]
        elif tag == "replace":
            if i2 - i1 == j2 - j1:
                for k in range(i2 - i1):
                    left, right = target[i1 + k], base[j1 + k]
                    if _shape(left["text"]) == _shape(right["text"]):
                        mapping[left["off"]] = right["off"]
            else:
                # Inside an unequal run, still pair instructions whose shape is
                # unique on both sides: a lone `fld dword ptr [S]` is the same
                # load however much code moved around it.
                t_shapes = Counter(_shape(row["text"]) for row in target[i1:i2])
                b_shapes = Counter(_shape(row["text"]) for row in base[j1:j2])
                b_by_shape = {_shape(row["text"]): row for row in base[j1:j2]}
                for row in target[i1:i2]:
                    shape = _shape(row["text"])
                    if t_shapes[shape] == 1 and b_shapes.get(shape) == 1 \
                            and "S" in shape:
                        mapping[row["off"]] = b_by_shape[shape]["off"]
    return mapping


# --------------------------------------------------------------------------
# value rendering


def _is_c_string(data: bytes) -> bool:
    end = data.find(b"\0")
    if end <= 0:
        return False
    text = data[:end]
    printable = sum(32 <= b < 127 or b in (9, 10, 13) or b >= 0x80 for b in text)
    return printable == len(text) and (end >= 2 or 32 < text[0] < 127)


def _is_wide_string(data: bytes) -> bool:
    for index in range(0, len(data) - 1, 2):
        lo, hi = data[index], data[index + 1]
        if lo == 0 and hi == 0:
            return index >= 4
        if hi != 0 or not (32 <= lo < 127 or lo in (9, 10, 13)):
            return False
    return False


def _plain_text(side, rva: int, text: bytes) -> bool:
    """A string that is not a pointer in disguise.

    Without a `??_C@` literal name, a short run of bytes before a NUL is as
    likely a pointer or a float. Text means: no relocation inside it, and
    mostly printable ASCII."""
    if not text:
        return False
    begin = bisect.bisect_left(side.relocations, rva - 3)
    if begin < len(side.relocations) and side.relocations[begin] < rva + len(text) + 1:
        return False
    ascii_text = sum(32 <= b < 127 or b in (9, 10, 13) for b in text)
    return ascii_text * 10 >= len(text) * 9 and (len(text) >= 3 or text.isalnum())


def _c_string(data: bytes) -> bytes:
    end = data.find(b"\0")
    return data if end < 0 else data[:end]


def _wide_string(data: bytes) -> bytes:
    for index in range(0, len(data) - 1, 2):
        if data[index] == 0 and data[index + 1] == 0:
            return data[:index]
    return data


def _quote(data: bytes, wide: bool = False) -> str:
    text = data.decode("utf-16-le" if wide else "latin-1", "backslashreplace")
    text = text.encode("unicode_escape").decode("ascii").replace('"', '\\"')
    if len(text) > 160:
        text = text[:157] + "..."
    return ('L"' if wide else '"') + text + '"'


def _float(value: float) -> str:
    if math.isnan(value) or math.isinf(value):
        return str(value)
    return repr(value)


def render(data: bytes, kind: str) -> str:
    """A human value for a compared window."""
    if kind == "string":
        return _quote(_c_string(data))
    if kind == "wstring":
        return _quote(_wide_string(data), wide=True)
    if kind == "float" and len(data) == 4:
        return f"{_float(struct.unpack('<f', data)[0])} ({data[::-1].hex()})"
    if kind == "double" and len(data) == 8:
        return f"{_float(struct.unpack('<d', data)[0])} ({data[::-1].hex()})"
    if kind == "float" and len(data) % 4 == 0 and len(data) <= 16:
        values = struct.unpack(f"<{len(data) // 4}f", data)
        return "{" + ", ".join(_float(v) for v in values) + "}"
    if kind == "int" and len(data) in (1, 2, 4):
        fmt = {1: "<b", 2: "<h", 4: "<i"}[len(data)]
        signed = struct.unpack(fmt, data)[0]
        return f"{signed} ({data[::-1].hex()})"
    if len(data) <= 16:
        return data.hex()
    return f"{len(data)} bytes {data[:16].hex()}..."


def render_difference(left: bytes, right: bytes, kind: str) -> tuple[str, str]:
    """Render two windows; long ones are shown around their first difference."""
    if len(left) <= 16 and len(right) <= 16 or kind in ("string", "wstring"):
        return render(left, kind), render(right, kind)
    first = next((i for i, (a, b) in enumerate(zip(left, right)) if a != b),
                 min(len(left), len(right)))
    start = first & ~3
    part = "float" if kind in ("float", "table") else "bytes"
    lw, rw = left[start:start + 4], right[start:start + 4]
    if part == "float" and len(lw) == 4 and len(rw) == 4:
        return (f"@+{start:#x} {render(lw, 'float')} of {len(left)} bytes",
                f"@+{start:#x} {render(rw, 'float')} of {len(right)} bytes")
    return (f"@+{start:#x} {left[start:start + 8].hex()} of {len(left)} bytes",
            f"@+{start:#x} {right[start:start + 8].hex()} of {len(right)} bytes")


_FLOAT_MNEMONICS = frozenset(
    "fld fst fstp fadd fsub fsubr fmul fdiv fdivr fcom fcomp fcompp".split())
_SSE_SINGLE = re.compile(r"^(movss|addss|subss|mulss|divss|minss|maxss|sqrtss|comiss|"
                         r"ucomiss|cvtss2sd|cvtss2si|cvttss2si|rcpss|rsqrtss|cmpss)$")
_SSE_DOUBLE = re.compile(r"^(movsd|addsd|subsd|mulsd|divsd|minsd|maxsd|sqrtsd|comisd|"
                         r"ucomisd|cvtsd2ss|cvtsd2si|cvttsd2si|cmpsd)$")
_SSE_PACKED = re.compile(r"^(movaps|movups|andps|andnps|orps|xorps|addps|subps|mulps|"
                         r"divps|minps|maxps|cmpps|shufps|movlps|movhps)$")


def value_kind(instruction: str, width: int, identity: str = "") -> str:
    """How to READ a compared value: float / double / int / bytes."""
    if real := _REAL.search(identity):
        return "double" if real.group(1) == "real" and len(real.group(2)) == 16 \
            else "float"
    mnemonic = _mnemonic(instruction)
    if mnemonic in _FLOAT_MNEMONICS:
        return {4: "float", 8: "double"}.get(width, "bytes")
    if _SSE_SINGLE.match(mnemonic):
        return "float"
    if _SSE_DOUBLE.match(mnemonic) and (mnemonic != "movsd" or "xmm" in instruction.lower()):
        return "double"  # a bare `movsd` is the string move
    if _SSE_PACKED.match(mnemonic):
        return "float"
    if width in (1, 2, 4):
        return "int"
    return "bytes"


# --------------------------------------------------------------------------
# images


@dataclass
class Side:
    """One image and everything needed to read and name its values."""

    name: str
    image: object                     # vostok.data.pe.PEImage
    index: object                     # render_relocs.DatumIndex
    relocations: tuple[int, ...]
    reloc_set: frozenset[int]

    def section_name(self, rva: int) -> str:
        section = self.image.section_at(rva)
        return section.name if section is not None else "-"

    def read(self, rva: int, size: int) -> bytes | None:
        try:
            return self.image.read_rva(rva, size)
        except ValueError:
            return None

    def pointer(self, site: int) -> int:
        value = self.image.u32_rva(site)
        return value - self.image.image_base if value >= self.image.image_base else value

    def window(self, rva: int, size: int):
        """(raw, relocation-normalized bytes, {offset: pointee identities}),
        or None when the window leaves its section."""
        raw = self.read(rva, size)
        if raw is None:
            return None
        normalized = bytearray(raw)
        pointers = {}
        begin = bisect.bisect_left(self.relocations, rva)
        end = bisect.bisect_left(self.relocations, rva + size)
        for site in self.relocations[begin:end]:
            if site + 4 > rva + size:
                continue
            destination = self.pointer(site)
            if rva <= destination <= rva + size:
                pointers[site - rva] = frozenset((f"SELF+{destination - rva:#x}",))
            else:
                pointers[site - rva] = self.index.resolve_all(destination)
            normalized[site - rva:site - rva + 4] = bytes(4)
        return bytes(raw), bytes(normalized), pointers


def _pointers_agree(left: dict, right: dict, partner) -> bool:
    if set(left) != set(right):
        return False
    for offset, ids in left.items():
        other = right[offset]
        if ids & other:
            continue
        if partner(ids, other):
            continue
        return False
    return True


# --------------------------------------------------------------------------
# comparison


@dataclass
class Ref:
    """One data reference of one instruction."""

    site: int
    instruction: int
    target: int
    access: str
    width: str
    form: str
    scale: int
    identity: str
    text: str
    function_rva: int


@dataclass
class Row:
    cls: str
    kind: str
    target_offset: int | None
    base_offset: int | None
    target_instruction: int | None
    base_instruction: int | None
    access: str
    width: str
    form: str
    size: int
    target_value: str
    base_value: str
    target_datum: str
    base_datum: str
    instruction: str
    note: str
    pairing: str = "aligned"
    t_key: tuple | None = None
    b_key: tuple | None = None


def _datum_label(side: Side, rva: int, hint: str = "") -> str:
    """The datum a reference names, with the address read (an RVA)."""
    datum = side.index.evidence_owner(rva, hint)
    if datum is None:
        return f"{side.section_name(rva)} rva={rva:#x}"
    name = datum.name or datum.identity
    offset = rva - datum.rva
    return (f"{name}+{offset:#x}" if offset else name) + f" rva={rva:#x}"


def _ending_at(side: Side, rva: int) -> frozenset[str]:
    """Names of the complete data whose one-past-the-end address is `rva`."""
    return frozenset(datum.name or datum.identity
                     for datum in side.index.by_end.get(rva, ()))


def _complete_size(side: Side, rva: int, hint: str = "") -> int | None:
    datum = side.index.evidence_owner(rva, hint)
    if datum is None or not datum.complete or datum.end is None:
        return None
    if not datum.rva <= rva < datum.end:
        return None
    return datum.end - rva


def _real_size(identity: str) -> int | None:
    real = _REAL.search(identity)
    if real is None:
        return None
    return len(real.group(2)) // 2


_BUILD_STRING = re.compile(
    r"(?i)(^[a-z]:\\|\\sources\\|\.(?:cpp|h|inl|c)$|"
    r"^(?:jan|feb|mar|apr|may|jun|jul|aug|sep|oct|nov|dec) [ \d]\d \d{4}$|"
    r"^\d\d:\d\d:\d\d$)")


def _equivalent_strings(left: bytes, right: bytes) -> str | None:
    """Why two differing strings are policy-equivalent, or None."""
    lt, rt = left.decode("latin-1"), right.decode("latin-1")
    if _BUILD_STRING.search(lt) and _BUILD_STRING.search(rt):
        if lt.replace("/", "\\").lower().rsplit("\\", 1)[-1] == \
                rt.replace("/", "\\").lower().rsplit("\\", 1)[-1]:
            return "build path"
        if re.fullmatch(r"\d\d:\d\d:\d\d", lt) or re.match(r"[A-Z][a-z]{2} ", lt):
            return "build timestamp"
    return None


def compare_ref(t: Ref, b: Ref, ts: Side, bs: Side, partner) -> Row:
    """Compare the values two aligned references read."""
    row = _compare_ref(t, b, ts, bs, partner)
    if row.cls != SAME and (t.access == "address" or t.form in ("imm", "lea")):
        # `first + count` is a one-past-the-end pointer: it lands on whatever
        # the linker placed next, so the bytes there are no value of it. Packed
        # data makes most starts some neighbour's end too, so this reading only
        # rescues a reference whose start reading disagrees.
        shared = _ending_at(ts, t.target) & _ending_at(bs, b.target)
        if shared:
            row.cls, row.kind, row.size = SAME, "endpointer", 0
            row.target_value = row.base_value = f"end of {min(shared)}"
            row.note = "one-past-the-end pointer"
    return row


def _compare_ref(t: Ref, b: Ref, ts: Side, bs: Side, partner) -> Row:
    width = _WIDTHS.get(t.width, 0)
    note = []
    t_label = _datum_label(ts, t.target, t.identity)
    b_label = _datum_label(bs, b.target, b.identity)

    def row(cls, kind, size, tv, bv, extra=""):
        if extra:
            note.append(extra)
        return Row(cls, kind, None, None, t.instruction, b.instruction, t.access,
                   t.width, t.form, size, tv, bv, t_label, b_label, t.text,
                   "; ".join(note) or "-")

    if t.form == "indcall" or "__imp_" in t.identity or "__imp_" in b.identity:
        same = t.identity.split("+")[0] == b.identity.split("+")[0]
        return row(SAME if same else UNRESOLVABLE, "import", 4, t.identity,
                   b.identity)
    if t.width != b.width or t.form != b.form or t.access != b.access:
        note.append(f"access differs: base {b.access}/{b.width}/{b.form}")

    kind = None
    address_of = t.access == "address" or t.form in ("imm", "lea")
    if address_of:
        probe = ts.read(t.target, min(STRING_CAP, _section_room(ts, t.target)))
        literal = "??_C@" in t.identity
        if real := _real_size(t.identity):
            size, kind = real, value_kind("", real, t.identity)
        elif probe and _is_c_string(probe) and (
                literal or _plain_text(ts, t.target, _c_string(probe))):
            kind = "string"
        elif probe and _is_wide_string(probe) and (
                literal or _plain_text(ts, t.target, _wide_string(probe)[::2])):
            kind = "wstring"
        else:
            complete = _complete_size(ts, t.target, t.identity)
            if complete is not None and complete <= TABLE_CAP:
                size, kind = complete, "datum"
            elif complete is not None:
                size, kind = TABLE_CAP, "datum"
                note.append(f"datum is {complete} bytes; compared the first {TABLE_CAP}")
            else:
                size, kind = 4, "word"
                note.append("extent unknown; compared the operand width")
        if kind in ("string", "wstring"):
            left = ts.read(t.target, min(STRING_CAP, _section_room(ts, t.target)))
            right = bs.read(b.target, min(STRING_CAP, _section_room(bs, b.target)))
            if right is None:
                return row(UNRESOLVABLE, kind, 0, render(left, kind), "-",
                           "base string unreadable")
            lstr = _c_string(left) if kind == "string" else _wide_string(left)
            rstr = _c_string(right) if kind == "string" else _wide_string(right)
            if lstr == rstr:
                return row(SAME, kind, len(lstr), render(left, kind), render(right, kind))
            if kind == "string" and (why := _equivalent_strings(lstr, rstr)):
                return row(EQUIVALENT, kind, len(lstr), render(left, kind),
                           render(right, kind), why)
            return row(DIFFERENT, kind, len(lstr), render(left, kind),
                       render(right, kind))
    elif t.form == "indexed" or t.scale or _REGISTER_OPERAND.search(t.text):
        # A register-relative operand: the displacement is a table base, and
        # MSVC folds `-min * scale` of a biased index into it, so the address
        # can name a NEIGHBOURING datum. Compare only when both sides name the
        # same datum at the same offset.
        t_datum = ts.index.evidence_owner(t.target, t.identity)
        b_datum = bs.index.evidence_owner(b.target, b.identity)
        if t_datum is None or b_datum is None or t_datum.name != b_datum.name \
                or t.target - t_datum.rva != b.target - b_datum.rva:
            return row(UNRESOLVABLE, "table", 0, t_label, b_label,
                       "indexed through a base that names different data "
                       "(biased table or folded datum)")
        complete = _complete_size(ts, t.target, t.identity)
        if complete is not None:
            size = min(complete, TABLE_CAP)
            kind = "table"
            if complete > TABLE_CAP:
                note.append(f"table is {complete} bytes; compared the first {TABLE_CAP}")
        else:
            size = width or 4
            kind = value_kind(t.text, size, t.identity)
            note.append("table extent unknown; compared the operand width")
    else:
        size = width or _real_size(t.identity) or 4
        kind = value_kind(t.text, size, t.identity)

    left = ts.window(t.target, size)
    right = bs.window(b.target, size)
    if left is None or right is None:
        return row(UNRESOLVABLE, kind, size, "-" if left is None else render(left[0], kind),
                   "-" if right is None else render(right[0], kind),
                   "window leaves its section")
    t_raw, t_norm, t_ptr = left
    b_raw, b_norm, b_ptr = right
    show = kind if kind in ("float", "double", "int", "string", "wstring") else (
        "float" if kind == "table" and size % 4 == 0 and _mostly_floats(t_raw) else "bytes")
    if t_norm == b_norm and _pointers_agree(t_ptr, b_ptr, partner):
        return row(SAME, kind, size, _render_window(t_raw, t_norm, t_ptr, show),
                   _render_window(b_raw, b_norm, b_ptr, show))
    if t_norm == b_norm:
        if set(t_ptr) != set(b_ptr):
            # a pointer is rendered by its pointee, so that the same pointer
            # read from another site is recognisably the same value
            return row(DIFFERENT, "pointer", size, _render_pointers(t_raw, t_ptr),
                       _render_pointers(b_raw, b_ptr), "pointer layout differs")
        bad = [off for off in t_ptr if not (t_ptr[off] & b_ptr[off])
               and not partner(t_ptr[off], b_ptr[off])]
        off = bad[0]
        return row(UNRESOLVABLE, "pointer", size,
                   f"@+{off:#x} -> {_first(t_ptr[off])}",
                   f"@+{off:#x} -> {_first(b_ptr[off])}",
                   "pointer targets could not be related")
    tv, bv = render_difference(t_raw, b_raw, show)
    return row(DIFFERENT, kind, size, tv, bv)


_IDENTITY_ORDER = ("P:", "E:", "L:", "M:", "D:")


def _first(ids: frozenset[str]) -> str:
    """One stable spelling of a pointee: the pairing key when it has one."""
    if not ids:
        return "-"
    return min(ids, key=lambda i: (next(
        (n for n, p in enumerate(_IDENTITY_ORDER) if i.startswith(p)),
        len(_IDENTITY_ORDER)), i))


def _render_window(raw: bytes, normalized: bytes, pointers: dict, show: str) -> str:
    """A compared window's value; pointer words show what they point at."""
    if not pointers:
        return render(raw, show)
    if len(raw) == 4:
        return _render_pointers(raw, pointers)
    cells = ",".join(f"+{off:#x}->{_first(pointers[off])}" for off in sorted(pointers)[:4])
    more = "..." if len(pointers) > 4 else ""
    return f"{render(normalized, show)} ptrs[{cells}{more}]"


def _render_pointers(raw: bytes, pointers: dict) -> str:
    if len(raw) == 4:
        return f"-> {_first(pointers[0])}" if 0 in pointers else render(raw, "bytes")
    return f"{len(raw)} bytes, pointers at " + (
        ",".join(f"+{off:#x}" for off in sorted(pointers)) or "none")


def _mostly_floats(data: bytes) -> bool:
    words = [data[i:i + 4] for i in range(0, len(data) - 3, 4)]
    if not words:
        return False
    plausible = 0
    for word in words:
        value = struct.unpack("<f", word)[0]
        if word == bytes(4) or (1e-6 < abs(value) < 1e7):
            plausible += 1
    return plausible * 4 >= len(words) * 3


def _section_room(side: Side, rva: int) -> int:
    section = side.image.section_at(rva)
    if section is None:
        return 0
    return max(0, section.end - rva)


# --------------------------------------------------------------------------
# switch tables in .text


_JUMP = re.compile(r"^jmp\s+dword ptr \[(\w+)\*4\+([^\]]*)\]")
_INDEX = re.compile(r"^movzx\s+(\w+),\s*byte ptr \[(\w+)\+([^\]]*)\]")
_CMP = re.compile(r"^cmp\s+(\w+),\s*(\S+)$")


def _sites_in(side: Side, start: int, length: int) -> list[int]:
    begin = bisect.bisect_left(side.relocations, start)
    end = bisect.bisect_left(side.relocations, start + length)
    return list(side.relocations[begin:end])


def _text_table(side: Side, record: dict, row: dict) -> int | None:
    """The .text address a table-reading instruction names, or None."""
    for site in _sites_in(side, record["rva"] + row["off"], row["len"]):
        destination = side.pointer(site)
        if side.section_name(destination) == ".text":
            return destination
    return None


def _jump_entries(side: Side, record: dict, table: int, limit: int | None = None,
                  stops: frozenset[int] = frozenset()) -> tuple[int, list[int]]:
    """(start, function-relative destinations) of a jump table.

    The displacement a `jmp [reg*4+X]` names is the table start only when the
    switch's smallest case is 0; otherwise MSVC folds `-min*4` into it. The
    entries themselves are relocations into the function, so the table starts
    at the first such relocation at or after X."""
    start, end = record["rva"], record["rva"] + record["size"]
    index = bisect.bisect_left(side.relocations, table)
    cursor = None
    for site in side.relocations[index:index + 64]:
        if site >= table + 1024:
            break
        if start <= side.pointer(site) < end + 64:
            cursor = site
            break
    if cursor is None:
        return table, []
    first = cursor
    entries = []
    while cursor in side.reloc_set and len(entries) < (limit or 1024):
        if entries and cursor in stops:
            break  # the next switch's table follows this one directly
        destination = side.pointer(cursor)
        if not start <= destination < end + 64:
            break
        entries.append(destination - start)
        cursor += 4
    return first, entries


def _index_bytes(side: Side, start: int, cases: int, bound: int | None) -> bytes | None:
    """A switch's byte index table: the run of bytes that are valid jump-table
    slots (the padding after the table is not), at most `bound` of them."""
    out = bytearray()
    while len(out) < min(bound or 1024, 1024):
        byte = side.read(start + len(out), 1)
        if byte is None or byte[0] >= cases:
            break
        out += byte
    return bytes(out)


def _table_starts(side: Side, record: dict) -> frozenset[int]:
    """Every .text table a function's jumps name: where one table ends."""
    starts = set()
    for row in record["instructions"]:
        if _JUMP.match(re.sub(r"\s+", " ", row["text"].strip())):
            table = _text_table(side, record, row)
            if table is not None:
                starts.add(table)
    return frozenset(starts)


def _jump_limit(instructions: list[dict], position: int) -> int | None:
    """`cmp reg, N` guarding `jmp [reg*4+X]` -> N + 1 cases."""
    match = _JUMP.match(re.sub(r"\s+", " ", instructions[position]["text"].strip()))
    return _index_bound(instructions, position, match.group(1)) if match else None


def _index_span(side: Side, record: dict, instructions: list[dict], position: int,
                table: int) -> tuple[int, int] | None:
    """(start, jump-table entry count) of the byte index table loaded at
    `position`: MSVC lays it out right after the jump table the following
    `jmp` reads, so its start is that table's end whatever the bias."""
    for row in instructions[position + 1:position + 4]:
        if not _JUMP.match(re.sub(r"\s+", " ", row["text"].strip())):
            continue
        jump_table = _text_table(side, record, row)
        if jump_table is None:
            return None
        first, entries = _jump_entries(side, record, jump_table,
                                       stops=_table_starts(side, record))
        if not entries:
            return None
        return first + 4 * len(entries), len(entries)
    return None


def _index_bound(instructions: list[dict], position: int, register: str) -> int | None:
    """`cmp reg, N` feeding a byte index table -> N + 1 entries."""
    for row in reversed(instructions[max(0, position - 8):position]):
        match = _CMP.match(re.sub(r"\s+", " ", row["text"].strip()))
        if match and match.group(1).lower() == register.lower():
            value = _parse_number(match.group(2))
            if value is not None and 0 <= value < 4096:
                return value + 1
    return None


def compare_switch_tables(t_rec: dict, b_rec: dict, ts: Side, bs: Side,
                          mapping: dict[int, int]) -> list[Row]:
    rows = []
    t_ins, b_ins = t_rec["instructions"], b_rec["instructions"]
    b_at = {row["off"]: index for index, row in enumerate(b_ins)}
    for position, t_row in enumerate(t_ins):
        text = re.sub(r"\s+", " ", t_row["text"].strip())
        jump, index = _JUMP.match(text), _INDEX.match(text)
        if not jump and not index:
            continue
        b_off = mapping.get(t_row["off"])
        t_table = _text_table(ts, t_rec, t_row)
        if t_table is None:
            continue
        common = dict(
            target_offset=t_row["off"], base_offset=b_off,
            target_instruction=t_rec["rva"] + t_row["off"],
            base_instruction=b_rec["rva"] + b_off if b_off is not None else None,
            access="read", form="table", target_datum=f".text rva={t_table:#x}",
            instruction=t_row["text"],
        )
        if b_off is None or b_off not in b_at:
            rows.append(Row(UNRESOLVABLE, "jumptable" if jump else "indextable",
                            width="dword" if jump else "byte", size=0,
                            target_value="-", base_value="-", base_datum="-",
                            note="table instruction has no aligned partner", **common))
            continue
        b_row = b_ins[b_at[b_off]]
        b_table = _text_table(bs, b_rec, b_row)
        if b_table is None:
            rows.append(Row(UNRESOLVABLE, "jumptable" if jump else "indextable",
                            width="dword" if jump else "byte", size=0,
                            target_value="-", base_value="-", base_datum="-",
                            note="aligned base instruction names no .text table",
                            **common))
            continue
        common["base_datum"] = f".text rva={b_table:#x}"
        if jump:
            _, t_entries = _jump_entries(ts, t_rec, t_table, _jump_limit(t_ins, position),
                                         _table_starts(ts, t_rec))
            _, b_entries = _jump_entries(bs, b_rec, b_table,
                                         _jump_limit(b_ins, b_at[b_off]),
                                         _table_starts(bs, b_rec))
            mapped = [mapping.get(off) for off in t_entries]
            unmapped = sum(off is None for off in mapped)
            mismatched = [
                case for case, (want, got) in enumerate(zip(mapped, b_entries))
                if want is not None and want != got
            ]
            tv = f"{len(t_entries)} cases"
            bv = f"{len(b_entries)} cases"
            if len(t_entries) != len(b_entries) or mismatched:
                detail = ", ".join(
                    f"case {case}: t+{t_entries[case]:#x}->b+{mapped[case]:#x} "
                    f"but b+{b_entries[case]:#x}" for case in mismatched[:4])
                rows.append(Row(DIFFERENT, "jumptable", width="dword",
                                size=4 * len(t_entries), target_value=tv,
                                base_value=bv, note=detail or "case count differs",
                                **common))
            elif unmapped:
                rows.append(Row(UNRESOLVABLE, "jumptable", width="dword",
                                size=4 * len(t_entries), target_value=tv,
                                base_value=bv,
                                note=f"{unmapped} case target(s) have no aligned partner",
                                **common))
            else:
                rows.append(Row(SAME, "jumptable", width="dword",
                                size=4 * len(t_entries), target_value=tv,
                                base_value=bv, note="-", **common))
        else:
            t_span = _index_span(ts, t_rec, t_ins, position, t_table)
            b_match = _INDEX.match(re.sub(r"\s+", " ", b_row["text"].strip()))
            b_span = _index_span(bs, b_rec, b_ins, b_at[b_off], b_table) \
                if b_match else None
            if t_span is None or b_span is None:
                rows.append(Row(UNRESOLVABLE, "indextable", width="byte", size=0,
                                target_value="-", base_value="-",
                                note="no jump table follows the index load",
                                **common))
                continue
            bound = _index_bound(t_ins, position, index.group(2))
            b_bound = _index_bound(b_ins, b_at[b_off], b_match.group(2))
            left = _index_bytes(ts, t_span[0], t_span[1], bound)
            right = _index_bytes(bs, b_span[0], b_span[1], b_bound)
            note = "-" if bound else "index bound unknown; read the valid slot run"
            if left is None or right is None:
                rows.append(Row(UNRESOLVABLE, "indextable", width="byte", size=0,
                                target_value="-", base_value="-",
                                note="table unreadable", **common))
                continue
            cls = SAME if left == right else DIFFERENT
            tv, bv = render_difference(left, right, "bytes")
            rows.append(Row(cls, "indextable", width="byte", size=len(left),
                            target_value=tv, base_value=bv, note=note, **common))
    return rows


# --------------------------------------------------------------------------
# immediates


_FRAME = re.compile(r"^(sub|add|and|lea)\s+(esp|ebp)\b|^ret\b|^retn\b|^enter\b")


def _immediate(text: str) -> tuple[str, int] | None:
    """(shape, value) of an instruction whose LAST operand is a bare number."""
    flat = re.sub(r"\s+", " ", text.strip())
    if _FRAME.match(flat.lower()):
        return None
    operands = _operands(flat)
    if not operands:
        return None
    last = operands.rsplit(",", 1)[-1].strip()
    if "[" in last or not _NUMBER.fullmatch(last):
        return None
    value = _parse_number(last)
    if value is None:
        return None
    return flat[: len(flat) - len(last)], value


def _line_numbers(record: dict) -> frozenset[int]:
    return frozenset(row.get("line", 0) for row in record.get("statements") or ())


def compare_immediates(t_rec: dict, b_rec: dict, mapping: dict[int, int],
                       ts: Side | None = None, bs: Side | None = None) -> list[Row]:
    rows = []
    b_by_off = {row["off"]: row for row in b_rec["instructions"]}
    t_lines, b_lines = _line_numbers(t_rec), _line_numbers(b_rec)
    for t_row in t_rec["instructions"]:
        b_off = mapping.get(t_row["off"])
        if b_off is None or b_off not in b_by_off:
            continue
        left = _immediate(t_row["text"])
        right = _immediate(b_by_off[b_off]["text"])
        if left is None or right is None or left[0] != right[0] or left[1] == right[1]:
            continue
        b_row = b_by_off[b_off]
        # A relocated immediate is an ADDRESS: the data references compare
        # what it points at.
        if ts is not None and _sites_in(ts, t_rec["rva"] + t_row["off"],
                                        t_row.get("len", 1)):
            continue
        if bs is not None and _sites_in(bs, b_rec["rva"] + b_off, b_row.get("len", 1)):
            continue
        value_width = 4
        kind = "int"
        cls = DIFFERENT
        note = "-"
        if left[1] in t_lines and right[1] in b_lines:
            cls, note = EQUIVALENT, "__LINE__"
        tv, bv = left[1], right[1]
        if tv > 0xFFFF and bv > 0xFFFF:
            kind = "float"
        render_kind = "float" if kind == "float" else "int"
        rows.append(Row(
            cls, "immediate", t_row["off"], b_off, t_rec["rva"] + t_row["off"],
            b_rec["rva"] + b_off, "imm", "-", "imm", value_width,
            render(struct.pack("<I", tv & 0xFFFFFFFF), render_kind),
            render(struct.pack("<I", bv & 0xFFFFFFFF), render_kind),
            "-", "-", t_row["text"], note))
    return rows


# --------------------------------------------------------------------------
# reference pairing


def pair_refs(t_refs: list[Ref], b_refs: list[Ref], t_rva: int, b_rva: int,
              mapping: dict[int, int]) -> tuple[list[tuple[Ref, Ref, str]],
                                                list[Ref], list[Ref]]:
    """Pair references by aligned instruction, then by access sequence."""
    b_by_insn: dict[int, list[Ref]] = defaultdict(list)
    for ref in b_refs:
        b_by_insn[ref.instruction - b_rva].append(ref)
    for refs in b_by_insn.values():
        refs.sort(key=lambda ref: ref.site)
    used: set[int] = set()
    pairs = []
    left_over = []
    t_by_insn: dict[int, list[Ref]] = defaultdict(list)
    for ref in t_refs:
        t_by_insn[ref.instruction - t_rva].append(ref)
    for t_off, refs in sorted(t_by_insn.items()):
        refs.sort(key=lambda ref: ref.site)
        b_off = mapping.get(t_off)
        partners = b_by_insn.get(b_off, []) if b_off is not None else []
        if len(partners) == len(refs):
            for left, right in zip(refs, partners):
                pairs.append((left, right, "aligned"))
                used.add(right.site)
        else:
            left_over.extend(refs)
    b_left = [ref for ref in b_refs if ref.site not in used]

    def token(ref: Ref) -> tuple:
        return ref.access, ref.width, ref.form, ref.scale, _shape(ref.text)

    if left_over and b_left:
        left_over.sort(key=lambda ref: ref.site)
        b_left.sort(key=lambda ref: ref.site)
        matcher = SequenceMatcher(None, [token(r) for r in left_over],
                                  [token(r) for r in b_left], autojunk=False)
        matched_t, matched_b = set(), set()
        for block in matcher.get_matching_blocks():
            for k in range(block.size):
                pairs.append((left_over[block.a + k], b_left[block.b + k], "sequence"))
                matched_t.add(block.a + k)
                matched_b.add(block.b + k)
        left_over = [r for i, r in enumerate(left_over) if i not in matched_t]
        b_left = [r for i, r in enumerate(b_left) if i not in matched_b]
    return pairs, left_over, b_left


def _float_of(rendered: str) -> float | None:
    try:
        value = float(rendered.split(" ", 1)[0])
    except ValueError:
        return None
    return value if math.isfinite(value) else None


def folded(rows: list[Row], t_solo: Counter, b_solo: Counter) -> None:
    """Classify a constant one side folded into a neighbouring multiply.

    Retail's `x / 180.f * math::pi` reads 1/180 and then pi; a build that
    folds the two reads pi/180 once, so its function has one float reference
    fewer. The value read differs, the computed one does not (up to rounding):
    EQUIVALENT, a statement-shape matter for the matcher, not a wrong
    constant. The factor must be read by a reference only the other side has
    (`t_solo`/`b_solo`: the unpaired references' values)."""
    def floats(values: Counter) -> set[float]:
        return {v for (k, r) in values if k in ("float", "double")
                if (v := _float_of(r)) not in (None, 0.0, 1.0, -1.0)}

    def close(a: float, b: float) -> bool:
        return abs(a - b) <= 2e-6 * max(abs(a), abs(b))

    t_floats, b_floats = floats(t_solo), floats(b_solo)
    for row in rows:
        if row.cls != DIFFERENT or row.kind not in ("float", "double"):
            continue
        tv, bv = _float_of(row.target_value), _float_of(row.base_value)
        # a product with 1 is the factor itself, which proves nothing
        if not tv or not bv or abs(tv) == 1.0 or abs(bv) == 1.0:
            continue

        def fits(w: float) -> bool:
            return not close(w, tv) and not close(w, bv)

        factor = next((w for w in t_floats if fits(w) and (
            close(tv * w, bv) or close(tv / w, bv))), None)
        if factor is None:
            factor = next((w for w in b_floats if fits(w) and (
                close(bv * w, tv) or close(bv / w, tv))), None)
        if factor is not None:
            row.cls = EQUIVALENT
            row.note = f"constant folding: the values differ by a factor {factor!r} " \
                "that only the other side reads"


def reordered(rows: list[Row], t_values: Counter, b_values: Counter) -> None:
    """Demote value differences that are only a different ORDER.

    `t_values`/`b_values` count every reference's own value (read as that
    side alone would read it). The values one side reads more often than the
    other are the function's real value differences. A DIFFERENT pair neither
    of whose values is among them is the alignment pairing two references the
    sides emit in another order (a swapped ternary, a reordered `if` chain, a
    moved block): retail's value is still read, just from another site."""
    t_extra, b_extra = t_values - b_values, b_values - t_values
    for row in rows:
        if row.cls != DIFFERENT or row.t_key is None or row.b_key is None:
            continue
        if row.t_key not in t_extra and row.b_key not in b_extra:
            row.cls = UNRESOLVABLE
            row.note = "reordered: the function reads the same values in another order" \
                + ("" if row.note == "-" else f"; {row.note}")


# --------------------------------------------------------------------------
# driver


def _load_refs(side: str, sizes: dict[int, int]) -> dict[int, list[Ref]]:
    """The data lane's access map, grouped by the containing function
    (`sizes`: function RVA -> size)."""
    path = paths.DATA_TARGET_ACCESS if side == "target" else paths.DATA_BASE_ACCESS
    if not path.is_file():
        raise RuntimeError(f"{path} is missing - run a full `vostok build` first")
    starts = sorted(sizes)
    result: dict[int, dict[int, Ref]] = defaultdict(dict)
    with path.open(newline="", encoding="utf-8") as source:
        for raw in csv.DictReader(source, delimiter="\t"):
            instruction = int(raw["instruction_rva"], 0)
            index = bisect.bisect_right(starts, instruction) - 1
            # nested/overlapping records are rare; walk back a few
            for probe in range(index, max(-1, index - 4), -1):
                rva = starts[probe]
                if rva <= instruction < rva + sizes[rva]:
                    site = int(raw["site_rva"], 0)
                    result[rva][site] = Ref(
                        site, instruction, int(raw["target_rva"], 0), raw["access"],
                        raw["width"], raw["form"], int(raw["scale"] or 0),
                        raw["target_identity"], raw["instruction"], rva)
                    break
    return {rva: list(refs.values()) for rva, refs in result.items()}


def _source_of(record: dict | None, offset: int | None) -> str:
    if record is None or offset is None:
        return "-"
    best = None
    for statement in record.get("statements") or ():
        start = statement.get("off", 0)
        if start <= offset < start + max(1, statement.get("size", 0)):
            best = statement
            break
        if start <= offset and (best is None or start >= best.get("off", 0)):
            best = statement
    if best is None:
        return "-"
    return f"{best.get('file') or record.get('file')}:{best.get('line')}"


def run(module: str | None = None, function: str | None = None,
        immediates: bool = False) -> list[dict]:
    from vostok.data import pipeline, render_relocs
    from vostok.data.pe import PEImage
    from vostok.sema import pairing as sema_pairing

    pairs = sema_pairing.Pairing()
    ledger = sema_pairing.ledger()
    t_image = PEImage(pipeline.image_paths("target")[0])
    b_image = PEImage(pipeline.image_paths("base")[0])

    def side(name, image):
        index = render_relocs._datum_index(name, image, pairs)
        relocations = image.base_relocations()
        return Side(name, image, index, relocations, frozenset(relocations))

    ts, bs = side("target", t_image), side("base", b_image)
    t_records = {}
    for rec in pairs.target_records:
        t_records.setdefault(rec["rva"], rec)
    b_records = {}
    for rec in pairs.base_records:
        b_records.setdefault(rec["rva"], rec)

    function_pairs: dict[tuple[int, int], str] = {}
    for key in sorted(pairs.pairs):
        t_rva, b_rva = pairs.pairs[key]
        if t_rva in t_records and b_rva in b_records:
            function_pairs.setdefault((t_rva, b_rva), key)

    def keep(key: str, rec: dict) -> bool:
        row = ledger.get(key) or sema_pairing.ledger_row(rec, ledger) or {}
        if module and row.get("module") != module:
            return False
        if function:
            low = function.lower()
            return low in key.lower() or low in rec["name"].lower()
        return True

    function_pairs = {
        pair: key for pair, key in function_pairs.items()
        if keep(key, t_records[pair[0]])
    }
    t_refs = _load_refs("target", {rva: rec["size"] for rva, rec in t_records.items()})
    b_refs = _load_refs("base", {rva: rec["size"] for rva, rec in b_records.items()})

    def partner(t_ids: frozenset[str], b_ids: frozenset[str]) -> bool:
        """Two pointees are the same function when the pairing says so."""
        t_keys = {i[2:] for i in t_ids if i.startswith("P:")}
        b_keys = {i[2:] for i in b_ids if i.startswith("P:")}
        return bool(t_keys & b_keys)

    out = []
    for (t_rva, b_rva), key in sorted(function_pairs.items()):
        t_rec, b_rec = t_records[t_rva], b_records[b_rva]
        ledger_row = ledger.get(key) or sema_pairing.ledger_row(t_rec, ledger) or {}
        mapping = align(t_rec["instructions"], b_rec["instructions"])
        rows: list[Row] = []
        ref_pairs, t_only, b_only = pair_refs(
            t_refs.get(t_rva, []), b_refs.get(b_rva, []), t_rva, b_rva, mapping)
        t_values, b_values = Counter(), Counter()
        t_solo, b_solo = Counter(), Counter()

        def solo(ref: Ref, side: Side) -> tuple[Row, tuple]:
            alone = compare_ref(ref, ref, side, side, partner)
            return alone, (alone.kind, alone.target_value)

        for left, right, how in ref_pairs:
            row = compare_ref(left, right, ts, bs, partner)
            row.pairing = how
            if how == "sequence" and row.cls == DIFFERENT and row.kind == "pointer":
                # a pointer paired with a non-pointer by access ORDER alone is
                # two different references, not one reference's two values
                row.cls = UNRESOLVABLE
                row.note += "; paired by access order only"
            row.target_offset = left.instruction - t_rva
            row.base_offset = right.instruction - b_rva
            row.t_key, row.b_key = solo(left, ts)[1], solo(right, bs)[1]
            t_values[row.t_key] += 1
            b_values[row.b_key] += 1
            rows.append(row)
        for ref in t_only:
            alone, key_ = solo(ref, ts)
            t_values[key_] += 1
            t_solo[(alone.kind, alone.target_value)] += 1
            rows.append(Row(UNRESOLVABLE, "unpaired", ref.instruction - t_rva, None,
                            ref.instruction, None, ref.access, ref.width, ref.form,
                            alone.size, alone.target_value, "-", alone.target_datum, "-",
                            ref.text, "target reference has no aligned base reference",
                            pairing="-"))
        for ref in b_only:
            alone, key_ = solo(ref, bs)
            b_values[key_] += 1
            b_solo[(alone.kind, alone.target_value)] += 1
            rows.append(Row(UNRESOLVABLE, "unpaired", None, ref.instruction - b_rva,
                            None, ref.instruction, ref.access, ref.width, ref.form,
                            alone.size, "-", alone.target_value, "-", alone.target_datum,
                            ref.text, "base reference has no aligned target reference",
                            pairing="-"))
        folded(rows, t_solo, b_solo)
        reordered(rows, t_values, b_values)
        rows.extend(compare_switch_tables(t_rec, b_rec, ts, bs, mapping))
        if immediates:
            rows.extend(compare_immediates(t_rec, b_rec, mapping, ts, bs))
        for row in rows:
            out.append({
                "class": row.cls,
                "kind": row.kind,
                "module": ledger_row.get("module") or "-",
                "unit": ledger_row.get("unit") or b_rec.get("file") or "-",
                "function": key,
                "pairing": row.pairing,
                "target_offset": _hex(row.target_offset),
                "base_offset": _hex(row.base_offset),
                "target_instruction_rva": _hex(row.target_instruction),
                "base_instruction_rva": _hex(row.base_instruction),
                "access": row.access,
                "width": row.width,
                "form": row.form,
                "compare_size": str(row.size),
                "target_value": row.target_value,
                "base_value": row.base_value,
                "target_datum": row.target_datum,
                "base_datum": row.base_datum,
                "source": _source_of(b_rec, row.base_offset),
                "target_source": _source_of(t_rec, row.target_offset),
                "instruction": re.sub(r"\s+", " ", row.instruction.strip()),
                "note": row.note,
            })
    return out


def _hex(value: int | None) -> str:
    return "-" if value is None else f"{value:#x}"


def _clean(value: str) -> str:
    return str(value).replace("\t", " ").replace("\n", "\\n").replace("\r", "\\r")


def write(rows: list[dict]) -> None:
    lines = ["\t".join(COLUMNS)]
    for row in rows:
        lines.append("\t".join(_clean(row[column]) for column in COLUMNS))
    paths.DATA_VALUE_DIFF.write_text("\n".join(lines) + "\n", encoding="utf-8")


def summarize(rows: list[dict]) -> str:
    by_class = Counter(row["class"] for row in rows)
    by_kind: dict[str, Counter] = defaultdict(Counter)
    for row in rows:
        by_kind[row["class"]][row["kind"]] += 1
    lines = ["data values: " + "  ".join(
        f"{cls}={by_class.get(cls, 0):,}" for cls in CLASSES)]
    for cls in CLASSES:
        if by_kind[cls]:
            lines.append(f"  {cls:<12} " + "  ".join(
                f"{kind}={count:,}" for kind, count in by_kind[cls].most_common()))
    return "\n".join(lines)


def print_rows(rows: list[dict], wanted: set[str]) -> None:
    for row in rows:
        if row["class"] not in wanted:
            continue
        print(f"{row['class']:<12} {row['kind']:<10} {row['module']:<10} "
              f"{row['function']}")
        print(f"    t+{row['target_offset']} ({row['target_instruction_rva']}) "
              f"b+{row['base_offset']} ({row['base_instruction_rva']})  "
              f"{row['instruction']}")
        print(f"    target {row['target_value']}  [{row['target_datum']}]")
        print(f"    base   {row['base_value']}  [{row['base_datum']}]")
        print(f"    source {row['source']}  (retail {row['target_source']})"
              + (f"  note: {row['note']}" if row["note"] != "-" else ""))


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(
        prog="vostok data values", description=__doc__.split("\n\n")[0],
        formatter_class=argparse.RawDescriptionHelpFormatter, epilog=__doc__)
    parser.add_argument("--module", help="only functions the ledger assigns to MODULE")
    parser.add_argument("--function", help="substring of the mangled or demangled name")
    parser.add_argument("--class", dest="classes", default=DIFFERENT,
                        help="classes to print, comma separated, or `all` "
                             f"(default {DIFFERENT}; the TSV always has every row)")
    parser.add_argument("--kind", help="only print these kinds (comma separated)")
    parser.add_argument("--immediates", action="store_true",
                        help="also compare literal operands of aligned instructions "
                             "(visible to code review already, so off by default)")
    parser.add_argument("--no-write", action="store_true",
                        help="do not rewrite binaries/gen/data_value_diff.tsv")
    args = parser.parse_args(argv)
    try:
        rows = run(args.module, args.function, args.immediates)
    except (OSError, RuntimeError, ValueError) as error:
        print(f"vostok data values: {error}", file=sys.stderr)
        return 2
    if not args.no_write and not args.module and not args.function:
        write(rows)
    wanted = set(CLASSES) if args.classes == "all" else {
        cls.strip().upper() for cls in args.classes.split(",")}
    shown = rows
    if args.kind:
        kinds = {kind.strip() for kind in args.kind.split(",")}
        shown = [row for row in rows if row["kind"] in kinds]
    print_rows(shown, wanted)
    print(summarize(rows))
    return 0


if __name__ == "__main__":
    raise SystemExit(_log.run("vostok.data.values", main))
