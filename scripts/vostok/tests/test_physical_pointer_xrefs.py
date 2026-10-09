# SPDX-License-Identifier: GPL-3.0-or-later

import unittest
import contextlib
import hashlib
import io
import json
import sqlite3
import tempfile
from pathlib import Path
from unittest.mock import patch
from types import SimpleNamespace

from vostok.data.pe import Section
from vostok.sema.xref import _cmd_physical_pointers, _physical_pointer_rows, _physical_span, _stale_stripping_note, _verified_pointer_labels


class Image:
    image_base = 0x400000

    def __init__(self):
        self.sections = [Section(".text", 0x1000, 0x100, 0, 0x100, 0),
                         Section(".rdata", 0x2000, 0x100, 0, 0x100, 0)]
        self.words = {0x1020: 0x401000, 0x2004: 0x401000,
                      0x2008: 0x401040, 0x2010: 0x401000}

    def section_at(self, rva):
        return next((s for s in self.sections if s.contains(rva)), None)

    def base_relocations(self):
        return (0x1020, 0x2004, 0x2008)

    def u32_rva(self, rva):
        return self.words[rva]

    def read_rva(self, rva, size):
        return (b"\x55\x8b\xec\xc3" + bytes(size))[:size]


def symbol(rva, name, end=None):
    return SimpleNamespace(rva=rva, display_name=name, end=end)


class PhysicalPointerTests(unittest.TestCase):
    def test_relocated_exact_destination_only_and_no_false_extent(self):
        rows = _physical_pointer_rows(Image(), {"rva": 0x1000},
                                      [symbol(0x2000, "??_7example@@6B@")])
        self.assertEqual([r["site_rva"] for r in rows], [0x1020, 0x2004])
        self.assertTrue(rows[1]["vtable_candidate"])
        self.assertFalse(rows[1]["owner_extent_known"])
        self.assertEqual(rows[1]["label_offset"], 4)

    def test_same_name_distinct_rvas_remain_distinct(self):
        image = Image()
        rows = _physical_pointer_rows(image, {"rva": 0x1040, "name": "duplicate"})
        self.assertEqual([r["site_rva"] for r in rows], [0x2008])
        self.assertEqual(rows[0]["destination_rva"], 0x1040)

    def test_label_cannot_cross_sections_and_known_extent_is_bounded(self):
        rows = _physical_pointer_rows(Image(), {"rva": 0x1000},
                                      [symbol(0x1000, "??_7wrong@@6B@", 0x3000)])
        self.assertEqual(rows[1]["label"], "")
        rows = _physical_pointer_rows(Image(), {"rva": 0x1000},
                                      [symbol(0x2000, "table", 0x2008)])
        self.assertTrue(rows[1]["owner_extent_known"])

    def test_labels_require_both_current_pe_and_exact_index_bytes(self):
        with tempfile.TemporaryDirectory() as temp:
            index, report = Path(temp) / "index.tsv", Path(temp) / "report.json"
            index.write_bytes(b"current index")
            bound = {"exe_sha256": "current PE", "index_sha256": hashlib.sha256(index.read_bytes()).hexdigest()}
            report.write_text(json.dumps({"inputs": {"base": bound}}))
            with patch("vostok.data.inventory.load", return_value=["verified table"]) as loader:
                self.assertEqual(_verified_pointer_labels("base", "current PE", index, report),
                                 (["verified table"], True))
                loader.reset_mock()
                self.assertEqual(_verified_pointer_labels("base", "new code-only PE", index, report), ([], False))
                loader.assert_not_called()
                index.write_bytes(b"stale replacement")
                self.assertEqual(_verified_pointer_labels("base", "current PE", index, report), ([], False))
                loader.assert_not_called()
                report.unlink()
                self.assertEqual(_verified_pointer_labels("base", "current PE", index, report), ([], False))

    def test_index_replaced_while_loading_cannot_label_pointer_sites(self):
        with tempfile.TemporaryDirectory() as temp:
            index, report = Path(temp) / "index.tsv", Path(temp) / "report.json"
            index.write_bytes(b"original")
            report.write_text(json.dumps({"inputs": {"base": {
                "exe_sha256": "PE", "index_sha256": hashlib.sha256(index.read_bytes()).hexdigest()}}}))
            def changed_index(_):
                index.write_bytes(b"replaced")
                return ["wrong vtable"]
            with patch("vostok.data.inventory.load", side_effect=changed_index):
                self.assertEqual(_verified_pointer_labels("base", "PE", index, report), ([], False))

    def test_target_span_cannot_disprove_base_stripping_note(self):
        image = Image()
        image.data = b"measured image"
        selected = {"rva": 0x1000, "size": 4, "name": "build"}
        with tempfile.TemporaryDirectory() as temp:
            db = Path(temp) / "evidence.sqlite"
            with contextlib.closing(sqlite3.connect(db)) as connection:
                connection.execute("CREATE TABLE metadata (key TEXT, value TEXT)")
                connection.execute("INSERT INTO metadata VALUES ('exe_sha256', ?)",
                                   (hashlib.sha256(image.data).hexdigest(),))
                connection.commit()
            with patch("vostok.sema.index._index_path", return_value=db), \
                 patch("vostok.data.pe.PEImage", return_value=image), \
                 patch("vostok.data.inventory.load", return_value=[]), \
                 patch("vostok.data.pipeline.image_paths", return_value=(Path("unused"), None)), \
                 patch("vostok.sema.pairing.ledger_row", return_value={"note": "Base stripped; no standalone body."}):
                target, base = io.StringIO(), io.StringIO()
                with contextlib.redirect_stdout(target):
                    _cmd_physical_pointers("target", selected)
                with contextlib.redirect_stdout(base):
                    _cmd_physical_pointers("base", selected)
                self.assertNotIn("WARNING:", target.getvalue())
                self.assertIn("WARNING:", base.getvalue())

    def test_mismatched_pe_evidence_fails_before_any_pointer_claim(self):
        with tempfile.TemporaryDirectory() as temp:
            db = Path(temp) / "evidence.sqlite"
            with contextlib.closing(sqlite3.connect(db)) as connection:
                connection.execute("CREATE TABLE metadata (key TEXT, value TEXT)")
                connection.execute("INSERT INTO metadata VALUES ('exe_sha256', 'stale')")
                connection.commit()
            with patch("vostok.sema.index._index_path", return_value=db), \
                 patch("vostok.data.pe.PEImage", return_value=SimpleNamespace(data=b"new PE")), \
                 patch("vostok.data.pipeline.image_paths", return_value=(Path("unused"), None)):
                with self.assertRaisesRegex(ValueError, "matching its PDB evidence hash"):
                    _cmd_physical_pointers("target", {"rva": 0x1000, "size": 4})

    def test_physical_extent_and_historical_note_are_separate(self):
        span = _physical_span(Image(), {"rva": 0x1000, "size": 4})
        self.assertTrue(_stale_stripping_note(span, "link-stripped build; no direct callers"))
        self.assertFalse(_stale_stripping_note(span, "Prior stripped-body note disproved."))
        self.assertFalse(_stale_stripping_note(span, "No direct callers."))
        self.assertFalse(_stale_stripping_note({"nonpadding_bytes": False}, "not emitted"))
        with self.assertRaises(ValueError):
            _physical_span(Image(), {"rva": 0x10fe, "size": 4})
        with self.assertRaises(ValueError):
            _physical_span(Image(), {"rva": 0x2000, "size": 4})


if __name__ == "__main__":
    unittest.main()
