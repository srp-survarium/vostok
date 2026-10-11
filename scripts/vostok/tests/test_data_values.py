# SPDX-License-Identifier: GPL-3.0-or-later

"""`vostok data values`: alignment, value reading and classification."""

import struct
import unittest
from collections import Counter

from vostok.data import values
from vostok.data.values import (
    DIFFERENT, EQUIVALENT, SAME, UNRESOLVABLE, Ref, Side, align,
    compare_immediates, compare_ref, compare_switch_tables, normalize, pair_refs,
)


class _Section:
    def __init__(self, name, rva, size):
        self.name, self.rva, self.end = name, rva, rva + size


class _Image:
    """A flat image: .text at 0x1000, .rdata at 0x2000, .data at 0x3000."""

    image_base = 0x10000

    def __init__(self):
        self.memory = bytearray(0x4000)
        self.sections = (_Section(".text", 0x1000, 0x1000),
                         _Section(".rdata", 0x2000, 0x1000),
                         _Section(".data", 0x3000, 0x1000))

    def section_at(self, rva):
        return next((s for s in self.sections if s.rva <= rva < s.end), None)

    def read_rva(self, rva, size):
        section = self.section_at(rva)
        if section is None or rva + size > section.end:
            raise ValueError("leaves a section")
        return bytes(self.memory[rva:rva + size])

    def u32_rva(self, rva):
        return struct.unpack_from("<I", self.memory, rva)[0]

    def put(self, rva, data):
        self.memory[rva:rva + len(data)] = data


class _Datum:
    def __init__(self, rva, size, name):
        self.rva, self.size, self.name, self.identity = rva, size, name, "E:" + name

    @property
    def end(self):
        return self.rva + self.size

    @property
    def complete(self):
        return True


class _Index:
    def __init__(self, datums=(), functions=None):
        self.datums = list(datums)
        self.functions = functions or {}
        self.by_end = {}
        for datum in self.datums:
            self.by_end.setdefault(datum.end, []).append(datum)

    def evidence_owner(self, rva, hint=""):
        return next((d for d in self.datums if d.rva <= rva < d.end), None)

    def resolve_all(self, rva):
        if rva in self.functions:
            return frozenset(self.functions[rva])
        return frozenset((f"RVA:{rva:#x}",))


def _side(image, index=None, relocations=()):
    relocations = tuple(sorted(relocations))
    return Side("t", image, index or _Index(), relocations, frozenset(relocations))


def _ref(target, text, access="read", width="dword", form="direct", identity="-",
         site=0x1010, instruction=0x100c):
    return Ref(site, instruction, target, access, width, form, 0, identity, text, 0x1000)


def _never(_left, _right):
    return False


class AlignmentTests(unittest.TestCase):
    def test_symbol_operands_are_masked_registers_kept(self):
        self.assertEqual(normalize("movss xmm0, [clear_value]"), "movss xmm0, [S]")
        self.assertEqual(normalize("mov   eax, [ebp-4]"), "mov eax, [ebp-4]")
        self.assertEqual(normalize("movss [eax+0Ch], xmm0"), "movss [eax+0Ch], xmm0")

    def test_aligns_renamed_constants_and_differing_immediates(self):
        target = [{"off": 0, "text": "push ebp"},
                  {"off": 1, "text": "movss xmm0, [clear_value]"},
                  {"off": 9, "text": "push 1Fh"},
                  {"off": 11, "text": "ret"}]
        base = [{"off": 0, "text": "push ebp"},
                {"off": 1, "text": "movss xmm0, [offset]"},
                {"off": 9, "text": "push 20h"},
                {"off": 11, "text": "ret"}]
        self.assertEqual(align(target, base), {0: 0, 1: 1, 9: 9, 11: 11})


class ValueTests(unittest.TestCase):
    def setUp(self):
        self.t_image, self.b_image = _Image(), _Image()

    def compare(self, t, b, t_index=None, b_index=None, t_relocs=(), b_relocs=(),
                partner=_never):
        return compare_ref(t, b, _side(self.t_image, t_index, t_relocs),
                           _side(self.b_image, b_index, b_relocs), partner)

    def test_float_read_at_different_addresses_with_different_value(self):
        self.t_image.put(0x2100, struct.pack("<f", 1.0))
        self.b_image.put(0x2200, struct.pack("<f", 0.0))
        row = self.compare(_ref(0x2100, "movss xmm0, dword ptr [0x12100]"),
                           _ref(0x2200, "movss xmm0, dword ptr [0x12200]"))
        self.assertEqual((row.cls, row.kind), (DIFFERENT, "float"))
        self.assertTrue(row.target_value.startswith("1.0 "))
        self.assertTrue(row.base_value.startswith("0.0 "))

    def test_identical_value_at_a_different_address_is_same(self):
        self.t_image.put(0x2100, struct.pack("<d", 0.001))
        self.b_image.put(0x2300, struct.pack("<d", 0.001))
        row = self.compare(_ref(0x2100, "fmul qword ptr [0x12100]", width="qword"),
                           _ref(0x2300, "fmul qword ptr [0x12300]", width="qword"))
        self.assertEqual((row.cls, row.kind), (SAME, "double"))

    def test_string_immediate_is_dereferenced(self):
        self.t_image.put(0x2400, b"bone_name\0")
        self.b_image.put(0x2500, b"bone_nome\0")
        row = self.compare(
            _ref(0x2400, "push 0x12400", access="address", width="-", form="imm"),
            _ref(0x2500, "push 0x12500", access="address", width="-", form="imm"))
        self.assertEqual((row.cls, row.kind), (DIFFERENT, "string"))
        self.assertEqual(row.target_value, '"bone_name"')

    def test_build_path_strings_are_equivalent(self):
        self.t_image.put(0x2400, b"c:\\survarium\\sources\\vostok\\a.cpp\0")
        self.b_image.put(0x2500, b"c:\\build\\sources\\vostok\\a.cpp\0")
        row = self.compare(
            _ref(0x2400, "push 0x12400", access="address", width="-", form="imm"),
            _ref(0x2500, "push 0x12500", access="address", width="-", form="imm"))
        self.assertEqual(row.cls, EQUIVALENT)

    def test_pointer_words_compare_by_pointee_not_address(self):
        # a two-slot table holding a pointer and a float on each side
        self.t_image.put(0x2600, struct.pack("<If", 0x10000 + 0x1800, 2.5))
        self.b_image.put(0x2700, struct.pack("<If", 0x10000 + 0x1900, 2.5))
        t_index = _Index([_Datum(0x2600, 8, "table")], {0x1800: {"P:fn"}})
        b_index = _Index([_Datum(0x2700, 8, "table")], {0x1900: {"P:fn"}})
        row = self.compare(
            _ref(0x2600, "mov ecx, 0x12600", access="address", width="-", form="imm"),
            _ref(0x2700, "mov ecx, 0x12700", access="address", width="-", form="imm"),
            t_index, b_index, (0x2600,), (0x2700,))
        self.assertEqual((row.cls, row.kind), (SAME, "datum"))

    def test_pointer_to_an_unrelated_pointee_is_unresolvable(self):
        self.t_image.put(0x2600, struct.pack("<I", 0x10000 + 0x1800))
        self.b_image.put(0x2700, struct.pack("<I", 0x10000 + 0x1900))
        t_index = _Index([_Datum(0x2600, 4, "slot")], {0x1800: {"P:one"}})
        b_index = _Index([_Datum(0x2700, 4, "slot")], {0x1900: {"P:two"}})
        row = self.compare(_ref(0x2600, "mov eax, dword ptr [0x12600]"),
                           _ref(0x2700, "mov eax, dword ptr [0x12700]"),
                           t_index, b_index, (0x2600,), (0x2700,))
        self.assertEqual((row.cls, row.kind), (UNRESOLVABLE, "pointer"))

    def test_end_pointer_of_the_same_datum_is_same(self):
        self.t_image.put(0x2604, struct.pack("<I", 11))
        t_index = _Index([_Datum(0x2600, 4, "pairs"), _Datum(0x2604, 4, "count")])
        b_index = _Index([_Datum(0x2700, 4, "pairs"), _Datum(0x2704, 4, "verts")])
        row = self.compare(
            _ref(0x2604, "push 0x12604", access="address", width="-", form="imm"),
            _ref(0x2704, "push 0x12704", access="address", width="-", form="imm"),
            t_index, b_index)
        self.assertEqual((row.cls, row.kind), (SAME, "endpointer"))

    def test_imports_compare_by_name(self):
        row = self.compare(
            _ref(0x2000, "call dword ptr [0x12000]", form="indcall",
                 identity="D:E:__imp__Sleep@4+0x0"),
            _ref(0x2004, "call dword ptr [0x12004]", form="indcall",
                 identity="D:E:__imp__Sleep@4+0x0"))
        self.assertEqual((row.cls, row.kind), (SAME, "import"))


class ImmediateTests(unittest.TestCase):
    def test_line_numbers_are_equivalent_other_values_differ(self):
        t_rec = {"rva": 0x1000, "statements": [{"off": 0, "line": 120}],
                 "instructions": [{"off": 0, "text": "push 78h"},
                                  {"off": 2, "text": "push 3F800000h"},
                                  {"off": 7, "text": "sub esp, 10h"}]}
        b_rec = {"rva": 0x5000, "statements": [{"off": 0, "line": 121}],
                 "instructions": [{"off": 0, "text": "push 79h"},
                                  {"off": 2, "text": "push 0"},
                                  {"off": 4, "text": "sub esp, 14h"}]}
        rows = compare_immediates(t_rec, b_rec, {0: 0, 2: 2, 7: 4})
        self.assertEqual([(r.cls, r.target_offset) for r in rows],
                         [(EQUIVALENT, 0), (DIFFERENT, 2)])


class SwitchTableTests(unittest.TestCase):
    def test_jump_table_entries_compare_through_the_alignment(self):
        t_image, b_image = _Image(), _Image()
        # function at 0x1000 (size 0x40); jump table at 0x1030 with two cases
        t_image.put(0x1000, bytes(3) + struct.pack("<I", 0x10000 + 0x1030))
        t_image.put(0x1030, struct.pack("<II", 0x10000 + 0x1010, 0x10000 + 0x1020))
        b_image.put(0x1100, bytes(3) + struct.pack("<I", 0x10000 + 0x1130))
        b_image.put(0x1130, struct.pack("<II", 0x10000 + 0x1120, 0x10000 + 0x1110))
        t_rec = {"rva": 0x1000, "size": 0x40, "instructions": [
            {"off": 0, "len": 7, "text": "jmp dword ptr [eax*4+]"},
            {"off": 0x10, "len": 1, "text": "nop"}, {"off": 0x20, "len": 1, "text": "ret"}]}
        b_rec = {"rva": 0x1100, "size": 0x40, "instructions": [
            {"off": 0, "len": 7, "text": "jmp dword ptr [eax*4+]"},
            {"off": 0x10, "len": 1, "text": "nop"}, {"off": 0x20, "len": 1, "text": "ret"}]}
        ts = _side(t_image, relocations=(0x1003, 0x1030, 0x1034))
        bs = _side(b_image, relocations=(0x1103, 0x1130, 0x1134))
        rows = compare_switch_tables(t_rec, b_rec, ts, bs, {0: 0, 0x10: 0x10, 0x20: 0x20})
        self.assertEqual([(r.cls, r.kind) for r in rows], [(DIFFERENT, "jumptable")])
        b_image.put(0x1130, struct.pack("<II", 0x10000 + 0x1110, 0x10000 + 0x1120))
        rows = compare_switch_tables(t_rec, b_rec, ts, bs, {0: 0, 0x10: 0x10, 0x20: 0x20})
        self.assertEqual([(r.cls, r.kind) for r in rows], [(SAME, "jumptable")])


    def test_a_following_table_ends_the_jump_table(self):
        image = _Image()
        image.put(0x1000, bytes(3) + struct.pack("<I", 0x10000 + 0x1030))
        image.put(0x1008, bytes(3) + struct.pack("<I", 0x10000 + 0x1038))
        image.put(0x1030, struct.pack("<IIII", *(0x10000 + 0x1010 + 4 * i for i in range(4))))
        rec = {"rva": 0x1000, "size": 0x40, "instructions": [
            {"off": 0, "len": 7, "text": "jmp dword ptr [eax*4+]"},
            {"off": 8, "len": 7, "text": "jmp dword ptr [ecx*4+]"}]}
        side = _side(image, relocations=(0x1003, 0x100b, 0x1030, 0x1034, 0x1038, 0x103c))
        _, entries = values._jump_entries(side, rec, 0x1030,
                                          stops=values._table_starts(side, rec))
        self.assertEqual(entries, [0x10, 0x14])


class PairingTests(unittest.TestCase):
    def test_references_pair_by_aligned_instruction_then_sequence(self):
        t = [Ref(0x1004, 0x1000, 0x2000, "read", "dword", "direct", 0, "-",
                 "fld dword ptr [x]", 0x1000),
             Ref(0x1024, 0x1020, 0x2004, "read", "dword", "direct", 0, "-",
                 "fld dword ptr [y]", 0x1000)]
        b = [Ref(0x5004, 0x5000, 0x2100, "read", "dword", "direct", 0, "-",
                 "fld dword ptr [x]", 0x5000),
             Ref(0x5034, 0x5030, 0x2104, "read", "dword", "direct", 0, "-",
                 "fld dword ptr [y]", 0x5000)]
        pairs, t_only, b_only = pair_refs(t, b, 0x1000, 0x5000, {0: 0})
        self.assertEqual([(p[0].site, p[1].site, p[2]) for p in pairs],
                         [(0x1004, 0x5004, "aligned"), (0x1024, 0x5034, "sequence")])
        self.assertEqual((t_only, b_only), ([], []))


class DemotionTests(unittest.TestCase):
    @staticmethod
    def _row(tv, bv, kind="string"):
        row = values.Row(DIFFERENT, kind, 0, 0, 0, 0, "address", "-", "imm", 4, tv, bv,
                         "-", "-", "push x", "-")
        row.t_key, row.b_key = (kind, tv), (kind, bv)
        return row

    def test_swapped_values_are_reordered_a_real_difference_stays(self):
        swap = [self._row('"a"', '"b"'), self._row('"b"', '"a"')]
        values.reordered(swap, Counter([("string", '"a"'), ("string", '"b"')]),
                         Counter([("string", '"b"'), ("string", '"a"')]))
        self.assertEqual([r.cls for r in swap], [UNRESOLVABLE, UNRESOLVABLE])
        real = [self._row("1.0 (3f800000)", "0.0 (00000000)", "float"),
                self._row("0.0 (00000000)", "0.0 (00000000)", "float")]
        values.reordered(real[:1], Counter([("float", "1.0 (3f800000)"),
                                            ("float", "0.0 (00000000)")]),
                         Counter([("float", "0.0 (00000000)")] * 2))
        self.assertEqual(real[0].cls, DIFFERENT)

    def test_a_factor_only_one_side_reads_is_folding(self):
        row = self._row("0.005555555690079927 (3bb60b61)", "0.01745329238474369 (3c8efa35)",
                        "float")
        values.folded([row], Counter([("float", "3.1415927410125732 (40490fdb)")]), Counter())
        self.assertEqual(row.cls, EQUIVALENT)
        trivial = self._row("2.0 (40000000)", "1.0 (3f800000)", "float")
        values.folded([trivial], Counter([("float", "2.0 (40000000)")]), Counter())
        self.assertEqual(trivial.cls, DIFFERENT)


class RenderTests(unittest.TestCase):
    def test_kinds(self):
        self.assertEqual(values.value_kind("fld dword ptr [x]", 4), "float")
        self.assertEqual(values.value_kind("fld qword ptr [x]", 8), "double")
        self.assertEqual(values.value_kind("mov eax, dword ptr [x]", 4), "int")
        self.assertEqual(values.value_kind("mov eax, [x]", 4, "E:__real@3f800000"),
                         "float")
        self.assertEqual(values.render(struct.pack("<i", -3), "int"), "-3 (fffffffd)")


if __name__ == "__main__":
    unittest.main()
