# SPDX-License-Identifier: GPL-3.0-or-later

import struct
import tempfile
import unittest
from pathlib import Path

from vostok.diff import image
from vostok.tool.link_layout import Layout, Module, Symbol

BASE = 0x10000


def _pe(text: bytes, fixups: list[int], rich: dict[int, int] | None = None,
        stamp: int = 0) -> image.Pe:
    """A minimal PE: one .text section at RVA 0x1000 and a .reloc section."""
    body = bytearray(0x400)
    body[0:2] = b"MZ"
    pe = 0x100
    struct.pack_into("<I", body, 0x3C, pe)
    if rich:
        key = 0x12345678
        at = 0x80
        struct.pack_into("<IIII", body, at, 0x536E6144 ^ key, key, key, key)
        at += 16
        for comp, count in rich.items():
            struct.pack_into("<II", body, at, comp ^ key, count ^ key)
            at += 8
        body[at:at + 4] = b"Rich"
        struct.pack_into("<I", body, at + 4, key)
    body[pe:pe + 4] = b"PE\0\0"
    struct.pack_into("<HHI", body, pe + 4, 0x14C, 2, stamp)
    struct.pack_into("<H", body, pe + 20, 0xE0)
    struct.pack_into("<I", body, pe + 52, BASE)
    blocks = bytearray()
    if fixups:
        page = fixups[0] & ~0xFFF
        entries = [0x3000 | (f - page) for f in fixups]
        blocks = bytearray(struct.pack("<II", page, 8 + 2 * len(entries)) + struct.pack(f"<{len(entries)}H", *entries))
    struct.pack_into("<II", body, pe + 24 + 96 + 8 * 5, 0x2000, len(blocks))
    sec = pe + 24 + 0xE0
    struct.pack_into("<8sIIII", body, sec, b".text", len(text), 0x1000, 0x200, 0x400)
    struct.pack_into("<I", body, sec + 36, 0x60000020)
    struct.pack_into("<8sIIII", body, sec + 40, b".reloc", len(blocks), 0x2000, 0x200, 0x600)
    data = bytes(body) + text.ljust(0x200, b"\xcc") + bytes(blocks).ljust(0x200, b"\0")
    return image.Pe.read(data)


LAYOUT = Layout([Module(0, "a.obj", "one.lib")], [], [0, 0x1000])


def _syms(*rows) -> list[Symbol]:
    return [Symbol(rva, name, size, 0, size is not None) for rva, name, size in rows]


class CodeTests(unittest.TestCase):
    def _function(self, syms, rva):
        return image.functions(syms, (0x1000, 0x1200), LAYOUT)[rva]

    def test_relocated_field_and_branch_translate_through_names(self):
        # f: mov eax,[g_value] ; call helper ; ret   (helper and g_value move between images)
        def text(value_rva, helper_rva, f_rva, pad):
            code = b"\xa1" + struct.pack("<I", BASE + value_rva) + b"\xe8" \
                + struct.pack("<i", helper_rva - (f_rva + 10)) + b"\xc3"
            return code.ljust(pad, b"\x90")
        want = _pe(text(0x1100, 0x1080, 0x1000, 0x80) + b"\xc3", [0x1001])
        got = _pe(text(0x1104, 0x1090, 0x1000, 0x90) + b"\xc3", [0x1001])
        w = _syms((0x1000, "f", 11), (0x1080, "helper", 1), (0x1100, "g_value", None))
        g = _syms((0x1000, "f", 11), (0x1090, "helper", 1), (0x1104, "g_value", None))
        verdict = image._same_code(self._function(w, 0x1000), self._function(g, 0x1000), want, got,
                                   set(want.fixups()), set(got.fixups()), image.Names(w),
                                   image.Names(g), [0, 5, 10])
        self.assertEqual(verdict, "exact")
        g_wrong = _syms((0x1000, "f", 11), (0x1090, "other", 1), (0x1104, "g_value", None))
        verdict = image._same_code(self._function(w, 0x1000), self._function(g_wrong, 0x1000), want, got,
                                   set(want.fixups()), set(got.fixups()), image.Names(w),
                                   image.Names(g_wrong), [0, 5, 10])
        self.assertEqual(verdict, "differing")
        g_unnamed = _syms((0x1000, "f", 11), (0x1090, "helper", 1))
        verdict = image._same_code(self._function(w, 0x1000), self._function(g_unnamed, 0x1000), want, got,
                                   set(want.fixups()), set(got.fixups()), image.Names(w),
                                   image.Names(g_unnamed), [0, 5, 10])
        self.assertEqual(verdict, "untranslated")

    def test_order_breaks_and_misplaced_bytes(self):
        code = b"\x90\xc3" * 8
        want = _pe(code, [])
        got = _pe(code[4:8] + code[0:4] + code[8:], [])
        w = _syms((0x1000, "a", 2), (0x1002, "b", 2), (0x1004, "c", 2), (0x1006, "d", 2))
        g = _syms((0x1000, "c", 2), (0x1002, "d", 2), (0x1004, "a", 2), (0x1006, "b", 2))
        layout = Layout([Module(0, "a.obj", "one.lib")], [], [0, 0x1000])
        report = image.compare_code(want, got, w, g, {}, {}, layout, layout)
        self.assertEqual(report.counts["code-exact"], 8)
        self.assertEqual(report.counts["code-order"], 1)
        self.assertEqual(report.counts["code-misplaced"], 4)
        self.assertEqual(report.counts["code-absent"], 0)

    def test_ambiguous_names_pair_only_with_their_compiland(self):
        layout = Layout([Module(0, "a.obj", "one.lib"), Module(1, "b.obj", "one.lib")], [], [0, 0x1000])
        syms = [Symbol(0x1000, "init", 4, 0, True), Symbol(0x1010, "init", 4, 1, True),
                Symbol(0x1020, "visit", 4, 0, True), Symbol(0x1030, "visit", 4, 0, True)]
        fs = image.functions(syms, (0x1000, 0x1100), layout)
        self.assertEqual(fs[0x1000].keys, {("one.lib|a.obj", "init")})
        self.assertEqual(fs[0x1020].keys, set())


class HeaderTests(unittest.TestCase):
    def test_rich_counts_and_clock_fields(self):
        want = _pe(b"\xc3", [], {(0x83 << 16) | 30729: 5}, stamp=1)
        got = _pe(b"\xc3", [], {(0x83 << 16) | 30729: 3, (0x83 << 16) | 21022: 2}, stamp=2)
        total, rows = image.rich_difference(want.data, got.data)
        self.assertEqual(total, 4)
        self.assertEqual(len(rows), 2)
        differing, moved, _rows = image.header_difference(want, got)
        self.assertEqual(differing, 0)        # the timestamp is a clock field
        self.assertGreaterEqual(moved, 1)

    def test_names_blank_compiler_numbering(self):
        self.assertEqual(image.normal_name("`f'::`47'::X::run"), "`f'::`#'::X::run")
        self.assertEqual(image.normal_name("??_7Y@?A0x0ab9c976@@6B@"), "??_7Y@?A@@6B@")


class BankTests(unittest.TestCase):
    def test_banks_only_tighten(self):
        banked = {"rich": 10, "code-exact": 100}
        self.assertTrue(image.worse("rich", 11, 10))
        self.assertTrue(image.worse("code-exact", 99, 100))
        self.assertEqual(image.tightened({"rich": 12, "code-exact": 150, "new": 3, "code-differing": 9}, banked),
                         {"rich": 10, "code-exact": 150, "new": 3})
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "image_diff.tsv"
            image.write_banked({"rich": 7}, path)
            self.assertEqual(image.read_banked(path), {"rich": 7})


if __name__ == "__main__":
    unittest.main()
