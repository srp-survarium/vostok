# SPDX-License-Identifier: GPL-3.0-or-later

"""vostok.eh over synthetic images: one parent, two destructible objects and
a try/catch, built the way MSVC 2008 x86 lays them out."""

import struct
import tempfile
import unittest
from pathlib import Path

from vostok.eh import x86
from vostok.eh.compare import pair_registrations, summarize
from vostok.eh.msf import CodeSymbol, SymbolTable, read_code_symbols
from vostok.eh.tables import Image

BASE = 0x10000
PARENT, DTOR_A, DTOR_B, FRAME_HANDLER, COOKIE = 0x1000, 0x1100, 0x1110, 0x1120, 0x1130
SEH_PARENT, EXCEPT_HANDLER3, SCOPES = 0x1300, 0x1140, 0x2100
FUNCLET0, FUNCLET1, STUB = 0x1200, 0x1210, 0x1240
FUNCINFO, UNWIND, TRY, HANDLERS, TYPE_DESC = 0x2000, 0x2040, 0x2060, 0x2080, 0x20A0


def _rel(at_end, target):
    return struct.pack("<i", target - at_end)


def _va(rva):
    return struct.pack("<I", BASE + rva)


def _build(path, *, eh=True, swap=False, slot=-0x14, catch=b".?AVfoo@@",
           finally_=False, filter_at=0x20):
    """Write a PE32: parent at PARENT registering STUB, whose FuncInfo has
    unwind states 0 (~A) and 1 (~B), and a try over state 2 catching `catch`.
    `swap` constructs B before A, `slot` moves A's frame slot, `eh=False`
    leaves the parent without a registration."""
    text = bytearray(b"\xCC" * 0x400)
    rdata = bytearray(0x400)
    relocs = []

    def code(rva, blob):
        text[rva - 0x1000:rva - 0x1000 + len(blob)] = blob

    def put(rva, blob):
        rdata[rva - 0x2000:rva - 0x2000 + len(blob)] = blob

    # parent: push ebp / mov ebp,esp / push -1 / push offset stub / mov eax,fs:[0]
    if eh:
        code(PARENT, b"\x55\x8B\xEC\x6A\xFF\x68" + _va(STUB)
             + b"\x64\xA1\x00\x00\x00\x00\x50\xC3")
        relocs.append(PARENT + 6)
    else:
        code(PARENT, b"\x55\x8B\xEC\x5D\xC3")
    # an SEH3 __try: push -1 / push offset scopes / push offset __except_handler3,
    # then trylevel 0 stored to [ebp-4]
    code(SEH_PARENT, b"\x55\x8B\xEC\x6A\xFF\x68" + _va(SCOPES) + b"\x68"
         + _va(EXCEPT_HANDLER3) + b"\x64\xA1\x00\x00\x00\x00\x50"
         + b"\xC7\x45\xFC\x00\x00\x00\x00\xC3")
    relocs += [SEH_PARENT + 6, SEH_PARENT + 11]
    put(SCOPES, struct.pack("<iII", -1, 0 if finally_ else BASE + SEH_PARENT + filter_at,
                            BASE + SEH_PARENT + 0x30))
    relocs += [SCOPES + 8] + ([] if finally_ else [SCOPES + 4])
    for rva in (DTOR_A, DTOR_B, FRAME_HANDLER, COOKIE, EXCEPT_HANDLER3):
        code(rva, b"\xC3")
    first, second = (DTOR_B, DTOR_A) if swap else (DTOR_A, DTOR_B)
    code(FUNCLET0, b"\x8D\x4D" + struct.pack("<b", slot) + b"\xE9"
         + _rel(FUNCLET0 + 8, first))
    code(FUNCLET1, b"\x8D\x4D\xE8\xE9" + _rel(FUNCLET1 + 8, second))
    stub = (b"\x8B\x54\x24\x08\x8D\x42\x0C\x8B\x4A\xE4\x33\xC8\xE8"
            + _rel(STUB + 17, COOKIE) + b"\xB8" + _va(FUNCINFO) + b"\xE9"
            + _rel(STUB + 27, FRAME_HANDLER))
    code(STUB, stub)
    relocs.append(STUB + 18)

    put(FUNCINFO, struct.pack("<IiIIIIIII", 0x19930522, 4, BASE + UNWIND, 1,
                              BASE + TRY, 0, 0, 0, 1))
    put(UNWIND, struct.pack("<iIiIiIiI", -1, BASE + FUNCLET0, 0, BASE + FUNCLET1,
                            1, 0, 1, 0))
    put(TRY, struct.pack("<iiiiI", 2, 2, 3, 1, BASE + HANDLERS))
    put(HANDLERS, struct.pack("<IIiI", 9, BASE + TYPE_DESC, -0x20, BASE + PARENT + 0x10))
    put(TYPE_DESC, b"\0" * 8 + catch + b"\0")
    relocs += [FUNCINFO + 8, FUNCINFO + 16, UNWIND + 4, UNWIND + 12, TRY + 16,
               HANDLERS + 4, HANDLERS + 12]

    blocks = {}
    for site in sorted(relocs):
        blocks.setdefault(site & ~0xFFF, []).append(0x3000 | (site & 0xFFF))
    reloc = bytearray()
    for page, entries in sorted(blocks.items()):
        if len(entries) % 2:
            entries.append(0)
        reloc += struct.pack("<II", page, 8 + 2 * len(entries))
        reloc += struct.pack(f"<{len(entries)}H", *entries)

    data = bytearray(0x1000)
    data[:2] = b"MZ"
    struct.pack_into("<I", data, 0x3C, 0x80)
    data[0x80:0x84] = b"PE\0\0"
    coff = 0x84
    struct.pack_into("<HHIIIHH", data, coff, 0x14C, 3, 0, 0, 0, 0xE0, 0x102)
    optional = coff + 20
    struct.pack_into("<H", data, optional, 0x10B)
    struct.pack_into("<I", data, optional + 28, BASE)
    struct.pack_into("<I", data, optional + 92, 16)
    struct.pack_into("<II", data, optional + 96 + 5 * 8, 0x3000, len(reloc))
    sections = ((b".text", 0x1000, 0x400, 0x400, text), (b".rdata", 0x2000, 0x400, 0x800, rdata),
                (b".reloc", 0x3000, 0x100, 0xC00, reloc))
    cursor = optional + 0xE0
    for name, rva, size, raw, blob in sections:
        data[cursor:cursor + 8] = name.ljust(8, b"\0")
        struct.pack_into("<IIIIIIHHI", data, cursor + 8, size, rva, size, raw, 0, 0, 0, 0, 0)
        data[raw:raw + len(blob)] = blob
        cursor += 40
    path.write_bytes(data)


def _symbols():
    return SymbolTable([
        CodeSymbol(PARENT, 0x100, "f", False), CodeSymbol(PARENT, None, "?f@@YAXXZ", True),
        CodeSymbol(DTOR_A, None, "??1A@@QAE@XZ", True),
        CodeSymbol(DTOR_B, None, "??1B@@QAE@XZ", True),
        CodeSymbol(FRAME_HANDLER, None, "___CxxFrameHandler3", True),
        CodeSymbol(COOKIE, None, "@__security_check_cookie@4", True),
        CodeSymbol(EXCEPT_HANDLER3, None, "__except_handler3", True),
        CodeSymbol(SEH_PARENT, 0x40, "guarded", False),
    ])


class EhTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.dir = Path(self.tmp.name)

    def tearDown(self):
        self.tmp.cleanup()

    def image(self, side, **kw):
        path = self.dir / f"{side}.exe"
        _build(path, **kw)
        return Image(side, path, None, symbols=_symbols())

    def pair(self, kind="c++", **base_kw):
        target, base = self.image("target"), self.image("base", **base_kw)
        pairs = [p for p in pair_registrations(target, base) if p.kind == kind]
        self.assertEqual(len(pairs), 1)
        return target, base, pairs[0]

    def test_parses_funcinfo_unwind_try_and_funclets(self):
        image = self.image("target")
        self.assertEqual(image.errors, [])
        (reg,) = image.registrations
        self.assertEqual((reg.parent_rva, reg.stub_rva, reg.site_rva), (PARENT, STUB, PARENT + 6))
        info = reg.info
        self.assertEqual((info.magic, info.max_state, info.eh_flags), (0x19930522, 4, 1))
        self.assertEqual([to for to, _ in info.unwind], [-1, 0, 1, 1])
        self.assertEqual(reg.funclets[FUNCLET0].callee, "??1A@@QAE@XZ")
        self.assertEqual(reg.funclets[FUNCLET0].frame, "[ebp-0x14]")
        self.assertEqual(reg.funclets[FUNCLET0].summary(), "lea ecx,[ebp-0x14]; jmp ??1A@@QAE@XZ")
        (tb,) = info.try_blocks
        self.assertEqual((tb.low, tb.high, tb.catch_high), (2, 2, 3))
        self.assertEqual(tb.handlers[0].spelling(), "const reference .?AVfoo@@")
        self.assertEqual(tb.handlers[0].disp_catch_obj, -0x20)
        self.assertEqual(reg.handlers, {})  # the catch block lives inside the parent
        self.assertTrue(reg.stub.summary().endswith(
            "mov eax, offset __ehfuncinfo; jmp ___CxxFrameHandler3"))

    def test_identical_images_match(self):
        target, base, pair = self.pair()
        self.assertEqual(pair.verdict, "MATCH")
        summary = summarize(target, base, [pair], {})
        self.assertEqual(summary.counts["funclet_pairs_exact"], 2)
        self.assertEqual(summary.counts["funclet_bytes_exact"],
                         summary.counts["funclet_bytes_paired"])

    def test_construction_order(self):
        _, _, pair = self.pair(swap=True)
        self.assertEqual(pair.verdict, "DIFFERS")
        self.assertEqual(pair.classes(), ["unwind_order"])

    def test_frame_slot(self):
        _, _, pair = self.pair(slot=-0x18)
        self.assertEqual(pair.classes(), ["unwind_frame"])
        self.assertIn("[ebp-0x14] vs [ebp-0x18]", pair.divergences[0].detail)

    def test_catch_type(self):
        _, _, pair = self.pair(catch=b".?AVbar@@")
        self.assertEqual(pair.classes(), ["catch_types"])

    def test_presence(self):
        _, _, pair = self.pair(eh=False)
        self.assertEqual((pair.verdict, pair.classes()), ("TARGET_ONLY", ["presence"]))

    def test_seh_scope_table(self):
        (frame,) = self.image("target").seh_frames
        self.assertEqual((frame.parent_rva, frame.flavor, frame.table_rva),
                         (SEH_PARENT, "seh3", SCOPES))
        self.assertEqual([(sc.enclosing, sc.kind) for sc in frame.scopes], [(-1, "__except")])
        self.assertEqual(self.pair("seh")[2].verdict, "MATCH")
        self.assertEqual(self.pair("seh", finally_=True)[2].classes(), ["seh_kind"])
        self.assertEqual(self.pair("seh", filter_at=0x24)[2].classes(), ["seh_offsets"])

    def test_hidden_mismatch_needs_a_perfect_ledger_row(self):
        target, base, pair = self.pair(slot=-0x18)
        row = {"cur": 100.0, "max": 100.0}
        self.assertEqual(len(summarize(target, base, [pair], {"?f@@YAXXZ": row}).hidden), 1)
        row = {"cur": 97.5, "max": 97.5}
        self.assertEqual(summarize(target, base, [pair], {"?f@@YAXXZ": row}).hidden, [])


class X86Tests(unittest.TestCase):
    def test_flagged_destructor_funclet_runs_past_its_jmp(self):
        # retail stlp_std::basic_ios<wchar_t>::imbue unwind state 0
        blob = bytes.fromhex("8b45ec83e0010f840c0000008365ecfe8b4d08e9880a9affc3cc")
        insns = x86.walk(blob)
        self.assertEqual(sum(i.length for i in insns), 25)
        self.assertEqual(insns[-1].opcode, 0xC3)

    def test_lengths(self):
        cases = {"8d8dccfeffff": 6, "8b5424 08": 4, "c7842410010000 01000000": 11,
                 "66c7450800 00": 6, "f6450801": 4, "0fb64508": 4, "d905 00000000": 6}
        for hexed, length in cases.items():
            blob = bytes.fromhex(hexed.replace(" ", ""))
            self.assertEqual(x86.decode(blob, 0).length, length, hexed)


def _msf(streams, block=512):
    """A minimal MSF 7.00: `streams[i]` is stream i's bytes."""
    blocks = [b"", b"", b""]  # superblock, two free-block maps

    def alloc(blob):
        out = []
        for k in range(0, max(len(blob), 1), block):
            out.append(len(blocks))
            blocks.append(blob[k:k + block].ljust(block, b"\0"))
        return out
    placed = [alloc(s) if s else [] for s in streams]
    directory = struct.pack("<I", len(streams)) + b"".join(
        struct.pack("<I", len(s)) for s in streams)
    for indexes in placed:
        directory += b"".join(struct.pack("<I", i) for i in indexes)
    dir_blocks = alloc(directory)
    block_map = alloc(b"".join(struct.pack("<I", i) for i in dir_blocks))[0]
    blocks[0] = (b"Microsoft C/C++ MSF 7.00\r\n\x1aDS\0\0\0" + struct.pack(
        "<6I", block, 1, len(blocks), len(directory), 0, block_map)).ljust(block, b"\0")
    blocks[1] = blocks[2] = b"\0" * block
    return b"".join(blocks)


def _record(kind, body):
    body = body + b"\0" * (-(len(body) + 4) % 4)
    return struct.pack("<HH", len(body) + 2, kind) + body


class MsfTests(unittest.TestCase):
    def test_publics_and_module_procs(self):
        pub = _record(0x110E, struct.pack("<IIH", 2, 0x10, 1) + b"?f@@YAXXZ\0")
        data_pub = _record(0x110E, struct.pack("<IIH", 0, 0x20, 2) + b"_g\0")
        proc = _record(0x110F, struct.pack("<IIIIIIIIHB", 0, 0, 0, 0x30, 0, 0, 0, 0x40, 1, 0)
                       + b"local_fn\0")
        module = struct.pack("<I", 4) + proc
        modinfo = bytearray(64)
        struct.pack_into("<HI", modinfo, 34, 5, len(module))
        modinfo += b"a.obj\0a.obj\0"
        modinfo += b"\0" * (-len(modinfo) % 4)
        dbi = bytearray(64)
        struct.pack_into("<iIIHHHHHHi", dbi, 0, -1, 19990903, 1, 0xFFFF, 0, 0xFFFF, 0, 4, 0,
                         len(modinfo))
        streams = [b"", b"", b"", bytes(dbi + modinfo), pub + data_pub, module]
        with tempfile.TemporaryDirectory() as directory:
            pdb = Path(directory) / "x.pdb"
            pdb.write_bytes(_msf(streams))
            syms = read_code_symbols(pdb, (0x1000, 0x2000))
        self.assertEqual(sorted((s.rva, s.size, s.name, s.public) for s in syms), [
            (0x1010, None, "?f@@YAXXZ", True), (0x1040, 0x30, "local_fn", False)])
        table = SymbolTable(syms)
        self.assertEqual(table.containing(0x1050), 0x1040)
        self.assertIsNone(table.containing(0x1070))  # one past local_fn
        self.assertEqual(table.containing(0x1020), 0x1010)


if __name__ == "__main__":
    unittest.main()
