# SPDX-License-Identifier: GPL-3.0-or-later

import struct
import tempfile
import unittest
from pathlib import Path

from vostok.tool.link_layout import (
    Contribution,
    Layout,
    Module,
    code_owners,
    optimization_classes,
    owner_rows,
    parse_dbi,
    pass1_order,
    runs,
)

CODE = 0x60500020
COMDAT = CODE | 0x1000


def _module_record(name: str, archive: str) -> bytes:
    body = b"\0" * 64 + name.encode() + b"\0" + archive.encode() + b"\0"
    return body + b"\0" * (-len(body) % 4)


def _dbi(modules, contributions) -> bytes:
    mods = b"".join(_module_record(n, a) for n, a in modules)
    sc = struct.pack("<I", 0xEFFE0000 + 19970605) + b"".join(
        struct.pack("<HHiiIHHII", s, 0, o, z, ch, m, 0, 0, 0) for s, o, z, ch, m in contributions)
    header = struct.pack("<iIIHHHHHHiiiiiIiiHHI", -1, 19990903, 1, 0, 0, 0, 0, 0, 0,
                         len(mods), len(sc), 0, 0, 0, 0, 0, 0, 0, 0x14C, 0)
    return header + mods + sc


def _sections(*vas: int) -> bytes:
    return b"".join(b".sec\0\0\0\0" + struct.pack("<IIII", 0, va, 0, 0) + b"\0" * 16 for va in vas)


def _layout(modules, contributions) -> Layout:
    return Layout([Module(i, n, a) for i, (n, a) in enumerate(modules)],
                  [Contribution(*c) for c in contributions], [0, 0x1000])


class ParseTests(unittest.TestCase):
    def test_modules_contributions_and_section_addresses(self):
        layout = parse_dbi(
            _dbi([("..\\gc\\weapon_core.obj", "c:\\libs\\vostok_game_core-static-gold.lib"),
                  ("finger_print.obj", "finger_print.obj")],
                 [(1, 0x10, 0x20, COMDAT, 0), (1, 0x40, 0x8, CODE, 1)]),
            _sections(0x1000, 0x7E4000))
        self.assertEqual([m.key for m in layout.modules],
                         ["vostok_game_core-static-gold.lib|weapon_core.obj", "<obj>|finger_print.obj"])
        self.assertEqual(layout.section_va, [0, 0x1000, 0x7E4000])
        self.assertTrue(layout.contributions[0].comdat)
        self.assertFalse(layout.contributions[1].comdat)
        self.assertEqual(layout.rva(layout.contributions[1]), 0x1040)


class LayoutTests(unittest.TestCase):
    MODULES = [("a.obj", "x\\one.lib"), ("b.obj", "x\\one.lib"), ("c.obj", "x\\two.lib")]

    def test_runs_merge_same_archive_and_kind(self):
        layout = _layout(self.MODULES, [(1, 0, 4, COMDAT, 0), (1, 4, 4, COMDAT, 1),
                                         (1, 8, 4, COMDAT, 2), (1, 12, 4, CODE, 2)])
        self.assertEqual(runs(layout, 1), [("one.lib", True, 2, 8, 0), ("two.lib", True, 1, 4, 8),
                                           ("two.lib", False, 1, 4, 12)])

    def test_code_owner_drops_ambiguous_and_unplaced_names(self):
        layout = _layout(self.MODULES, [(1, 0, 0x10, COMDAT, 0), (1, 0x10, 0x10, CODE, 1)])
        owners = code_owners(layout, [("f", 0x1004), ("g", 0x1014), ("g", 0x1004), ("h", 0x2000)])
        self.assertEqual(owners, {"f": ("one.lib|a.obj", True)})

    def test_owner_verdict_uses_our_pass1_load_order(self):
        target = _layout(self.MODULES, [(1, 0, 0x10, COMDAT, 0), (1, 0x10, 0x10, COMDAT, 2)])
        base = _layout(self.MODULES, [(1, 0, 0x10, COMDAT, 2), (1, 0x10, 0x10, COMDAT, 0)])
        classes = {"one.lib": "O2", "two.lib": "Od"}
        ledger = {"f": {"cur": "40", "max": "40"}}
        log = ("Loaded one.lib(a.obj)\n Loaded two.lib(c.obj)\n Loaded one.lib(a.obj)\n")
        rows = owner_rows(target, base, [("f", 0x1000)], [("f", 0x1000)], classes, ledger, pass1_order(log))
        self.assertEqual(len(rows), 1)
        self.assertEqual((rows[0]["target_owner"], rows[0]["base_owner"]), ("one.lib|a.obj", "two.lib|c.obj"))
        self.assertEqual((rows[0]["target_class"], rows[0]["base_class"]), ("O2", "Od"))
        self.assertEqual(rows[0]["verdict"], "missing-reference")
        rows = owner_rows(target, base, [("f", 0x1000)], [("f", 0x1000)], classes, ledger,
                          pass1_order("Loaded x\\two.lib(c.obj)\nLoaded one.lib(a.obj)\n"))
        self.assertEqual(rows[0]["verdict"], "load-order")
        rows = owner_rows(target, base, [("f", 0x1000)], [("f", 0x1000)], classes, ledger)
        self.assertEqual(rows[0]["verdict"], "owner-differs")

    def test_owner_verdict_reports_roster_gap(self):
        target = _layout(self.MODULES, [(1, 0, 0x10, COMDAT, 0)])
        base = _layout(self.MODULES[1:], [(1, 0, 0x10, COMDAT, 0)])
        rows = owner_rows(target, base, [("f", 0x1000)], [("f", 0x1000)], {}, {})
        self.assertEqual(rows[0]["verdict"], "roster")


class OptimizationClassTests(unittest.TestCase):
    def test_reads_archive_and_compiler_flags(self):
        with tempfile.TemporaryDirectory() as tmp:
            rsp = Path(tmp) / "rsp"
            rsp.mkdir()
            for project, flags in (("game_core", "/Od /GL"), ("game", "/Ox /GL"), ("ai", "/Ot /GL")):
                (rsp / f"{project}_lib.rsp").write_text(f'/OUT:"Z:/b/vostok_{project}-static-gold.lib" a.obj')
                (rsp / f"{project}_cl_0.rsp").write_text(f"/c {flags} x.cpp")
            self.assertEqual(optimization_classes(Path(tmp)), {
                "vostok_game_core-static-gold.lib": "Od",
                "vostok_game-static-gold.lib": "O2",
                "vostok_ai-static-gold.lib": "partial",
            })


if __name__ == "__main__":
    unittest.main()
