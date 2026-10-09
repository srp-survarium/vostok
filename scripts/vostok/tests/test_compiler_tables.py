# SPDX-License-Identifier: GPL-3.0-or-later
"""Physical selector/jump contents must not borrow a sibling's code score."""

import copy
import struct
import tempfile
from pathlib import Path
import unittest
from types import SimpleNamespace
from unittest import mock

from vostok.data import compiler_tables, render_relocs, reviews
from vostok.data import gate as data_gate
from vostok.data.pe import Section


class Image:
    def __init__(self, owner_rvas, *, wrong_slot=False):
        self.image_base = 0x10000
        self.data = bytearray(0x8000)
        self.relocs = set()
        self.text = Section(".text", 0, len(self.data), 0, len(self.data), 0x60000020)
        for start in owner_rvas:
            body = bytearray(301)
            body[:3] = bytes.fromhex("0fb688")
            struct.pack_into("<I", body, 3, self.image_base + start + 100)
            body[7:10] = bytes.fromhex("ff248d")
            struct.pack_into("<I", body, 10, self.image_base + start + 64)
            for offset, value in zip(range(14, 62, 6), (1, 2, 6, 5, 4, 3, 13, 11)):
                body[offset:offset + 6] = b"\xb8" + struct.pack("<I", value) + b"\xc3"
            body[62:64] = bytes.fromhex("8bff")
            for slot, offset in enumerate(range(14, 62, 6)):
                if wrong_slot and slot == 2:
                    offset = 44
                struct.pack_into("<I", body, 64 + slot * 4, self.image_base + start + offset)
                self.relocs.add(start + 64 + slot * 4)
            body[100:] = bytes([8]) * 201
            body[140] = 2
            self.data[start:start + 301] = body
            self.relocs.update((start + 3, start + 10))

    def base_relocations(self):
        return sorted(self.relocs)

    def read_rva(self, start, size):
        if not self.text.contains(start, size):
            raise ValueError("read outside PE section")
        return bytes(self.data[start:start + size])

    def u32_rva(self, start):
        return struct.unpack("<I", self.read_rva(start, 4))[0]

    def section_at(self, rva):
        return self.text if self.text.contains(rva) else None


class CompilerTableTests(unittest.TestCase):
    function = "mesh_helper"

    def setUp(self):
        self.facts = []
        self.target_records, self.base_records = [], []
        for unit, target, base in (("first.cpp", 0x1000, 0x3000), ("second.cpp", 0x2000, 0x4000)):
            self.target_records.append(dict(file=unit, mangled=self.function, rva=target, size=301))
            self.base_records.append(dict(file=unit, mangled=self.function, rva=base, size=301))
            for kind, operand, offset, size in (("selector", 3, 100, 201), ("jump", 10, 64, 36)):
                self.facts.append(dict(unit=unit, function=self.function, target_owner_rva=target,
                                       kind=kind, operand_offset=operand, table_offset=offset,
                                       size=size, evidence="reviewed physical fixture"))
        self.target = Image([0x1000, 0x2000])
        self.base = Image([0x3000, 0x4000])

    def audit(self):
        pairs = SimpleNamespace(target_records=self.target_records, base_records=self.base_records)
        with mock.patch.object(compiler_tables.maxima, "whole_source_file_hash", return_value="source"):
            return compiler_tables.audit("render", pairs, self.target, self.base,
                                         {self.function: dict(cur=100, max=100, status="done")},
                                         facts=self.facts)

    def test_wrong_slot_with_equal_code_and_selectors_stays_open_at_code100(self):
        self.base = Image([0x3000, 0x4000], wrong_slot=True)
        self.assertEqual(self.target.read_rva(0x1000 + 14, 48), self.base.read_rva(0x3000 + 14, 48))
        self.assertEqual(self.target.read_rva(0x1000 + 100, 201), self.base.read_rva(0x3000 + 100, 201))
        audit, gates = self.audit()
        self.assertEqual([row["datum_status"] for row in audit].count("RELOC_TARGETS"), 2)
        self.assertEqual([row["resolution"] for row in gates], ["OPEN", "OPEN"])

    def test_relocated_equivalent_tables_are_exact_and_keep_null_default(self):
        audit, gates = self.audit()
        self.assertTrue(all(row["datum_status"] == "EXACT" for row in audit))
        self.assertTrue(all(row["resolution"] == "EXACT" for row in gates))
        self.assertEqual(self.base.u32_rva(0x3000 + 96), 0)
        self.assertNotIn(0x3000 + 96, self.base.base_relocations())

    def test_changed_selector_is_bytes_debt_and_owner_isolation_is_preserved(self):
        self.base.data[0x3000 + 140] = 1
        audit, gates = self.audit()
        self.assertEqual([row["datum_status"] for row in audit].count("BYTES"), 1)
        self.assertEqual([row["resolution"] for row in gates], ["OPEN", "EXACT"])
        self.assertNotEqual(gates[0]["function"], gates[1]["function"])
        self.assertEqual(gates[0]["unit"], "first.cpp")
        self.assertEqual(gates[1]["unit"], "second.cpp")

    def test_missing_and_ambiguous_candidate_are_in_gate_denominator(self):
        self.base_records = self.base_records[1:]
        _, gates = self.audit()
        self.assertEqual(len(gates), 2)
        self.assertEqual(gates[0]["resolution"], "OPEN")
        self.assertEqual(gates[0]["base_function_rvas"], "-")
        self.base_records = [dict(self.base_records[0], file="first.cpp", rva=0x3000),
                             dict(self.base_records[0], file="first.cpp", rva=0x5000)]
        _, gates = self.audit()
        self.assertTrue(all(row["resolution"] == "OPEN" for row in gates))

    def test_invalid_candidate_evidence_fails_closed(self):
        mutations = (
            lambda: self.base.data.__setitem__(0x3000, 0x90),
            lambda: self.base.relocs.remove(0x3000 + 3),
            lambda: self.base.data.__setitem__(0x3000 + 14, 0x90),
            lambda: self.base.data.__setitem__(0x3000 + 140, 9),
            lambda: struct.pack_into("<I", self.base.data, 0x3000 + 64, 0x10000 + 0x3000 + 15),
            lambda: self.base.relocs.add(0x3000 + 100),
        )
        for mutation in mutations:
            with self.subTest(mutation=mutation):
                self.base = Image([0x3000, 0x4000])
                mutation()
                _, gates = self.audit()
                self.assertEqual(gates[0]["resolution"], "OPEN")
                self.assertEqual(gates[1]["resolution"], "EXACT")

    def test_changed_null_default_is_relocation_debt(self):
        struct.pack_into("<I", self.base.data, 0x3000 + 96, 0x10000 + 0x3000 + 14)
        self.base.relocs.add(0x3000 + 96)
        audit, gates = self.audit()
        self.assertTrue(any(row["datum_status"] == "RELOC_LAYOUT" for row in audit))
        self.assertEqual(gates[0]["resolution"], "OPEN")

    def test_table_changes_stale_evidence_and_raw_name_review_cannot_close(self):
        _, before = self.audit()
        old = dict(status="bounded", src_hash=before[0]["source_hash"], diff_hash=before[0]["diff_hash"])
        self.base.data[0x3000 + 140] = 1
        _, after = self.audit()
        self.assertFalse(reviews.matches(old, after[0]["source_hash"], after[0]["diff_hash"]))
        self.assertEqual(after[0]["resolution"], "OPEN")
        original = copy.deepcopy(self.facts)
        self.facts[0]["evidence"] = "new reviewed extent provenance"
        _, updated = self.audit()
        self.assertNotEqual(updated[0]["source_hash"], after[0]["source_hash"])
        self.facts = original

    def test_existing_report_and_aggregate_gate_include_table_content_debt(self):
        self.base = Image([0x3000, 0x4000], wrong_slot=True)
        pairs = SimpleNamespace(target_records=self.target_records, base_records=self.base_records)
        context = SimpleNamespace(
            pairs=pairs, ledger={self.function: dict(cur=100, max=100)},
            target_image=self.target, base_image=self.base,
            target_index=render_relocs.DatumIndex([], self.target),
            base_index=render_relocs.DatumIndex([], self.base),
            target_sites={}, base_sites={}, target_cones=None, base_cones=None,
            reviews={self.function: dict(status="bounded")}, inputs={},
            content_keys=(frozenset(), frozenset()),
        )
        with tempfile.TemporaryDirectory() as directory:
            artifacts = SimpleNamespace(**{
                name: Path(directory) / name
                for name in ("audit", "extentless", "function_data", "report", "problems")
            })
            with mock.patch.object(render_relocs, "_artifacts", return_value=artifacts), \
                    mock.patch.object(render_relocs, "_expected_sites", return_value=set()), \
                    mock.patch.object(compiler_tables, "registrations", return_value=self.facts), \
                    mock.patch.object(compiler_tables.maxima, "whole_source_file_hash", return_value="source"):
                report = render_relocs.refresh("render", context=context)
                gate = data_gate._module_row("render", report)
                open_rows = data_gate._open_rows("render")
            self.assertEqual(report["target_relocation_sites"], 4)
            self.assertEqual(report["base_relocation_sites"], 4)
            self.assertEqual(gate["open_function_data"], 2)
            self.assertEqual(len(open_rows), 2)
            self.assertIn("RELOC_TARGETS", artifacts.audit.read_text())
            self.assertIn("compiler-table", artifacts.problems.read_text())

    def test_deleted_registration_cannot_silently_shrink_coverage(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "facts.tsv"
            path.write_text("\t".join(compiler_tables.COLUMNS) + "\n")
            with mock.patch.object(compiler_tables.paths, "RETAIL_COMPILER_TABLES", path):
                with self.assertRaisesRegex(ValueError, "enrollment"):
                    compiler_tables.registrations()

    def test_registration_overlapping_code_is_rejected(self):
        self.facts[0]["table_offset"] = 14
        with self.assertRaisesRegex(ValueError, "geometry"):
            self.audit()


class SurfaceLoadImage:
    """Retail selector/case bytes, relocated onto an isolated physical fixture."""
    suffix = bytes.fromhex(
        "0fb7000fb7c0c746040000000083f82f77600fb68828c26300ff248d0cc26300"
        "c74604060000005f5e83c424c20800c74604050000005f5e83c424c20800"
        "c74604040000005f5e83c424c20800c74604030000005f5e83c424c20800"
        "c74604010000005f5e83c424c20800c74604020000005f5e83c424c20800"
        "8bffecc16300fbc16300b0c16300bfc16300cec16300ddc1630002c26300"
        "000001060606060606060606060606060606060606060606060606060606060606060606060606060606060602030405"
    )

    def __init__(self, start=0x62C060, *, omitted_static_mesh=False):
        self.start = start
        self.image_base = 0x10000
        self.data = bytearray(304) + bytearray(self.suffix)
        self.data[:9] = bytes.fromhex("83ec2456578b7c2430")
        self.data[14:16] = bytes.fromhex("8bf1")
        assert len(self.data) == 504
        self.text = Section(".text", start, 504, 0, 504, 0x60000020)
        for operand, table in ((325, 456), (332, 428)):
            struct.pack_into("<I", self.data, operand, self.image_base + start + table)
        for slot, offset in enumerate((396, 411, 336, 351, 366, 381, 418)):
            struct.pack_into("<I", self.data, 428 + slot * 4, self.image_base + start + offset)
        self.relocs = {start + 325, start + 332} | set(range(start + 428, start + 456, 4))
        if omitted_static_mesh:
            self.data[457] = 6

    def read_rva(self, start, size):
        if not self.text.contains(start, size):
            raise ValueError("surface fixture read outside extent")
        return bytes(self.data[start - self.start:start - self.start + size])

    def u32_rva(self, start):
        return struct.unpack("<I", self.read_rva(start, 4))[0]

    def base_relocations(self):
        return sorted(self.relocs)

    def section_at(self, rva):
        return self.text if self.text.contains(rva) else None


class SurfaceLoadCompilerTableTests(unittest.TestCase):
    def setUp(self):
        self.unit, self.function = compiler_tables._SURFACE_LOAD
        self.facts = [dict(unit=self.unit, function=self.function,
                           target_owner_rva=0x62C060, kind=kind,
                           operand_offset=operand, table_offset=table, size=size,
                           evidence="retail physical member-store/RET8 fixture")
                      for kind, operand, table, size in (
                          ("selector", 325, 456, 48), ("jump", 332, 428, 28))]
        self.target = SurfaceLoadImage()
        self.base = SurfaceLoadImage(0x645650)
        self.owner = dict(file=self.unit, mangled=self.function, rva=0x62C060, size=504)
        self.base_owner = dict(self.owner, rva=0x645650)

    def audit(self):
        pairs = SimpleNamespace(target_records=[self.owner], base_records=[self.base_owner])
        with mock.patch.object(compiler_tables.maxima, "whole_source_file_hash", return_value="source"):
            return compiler_tables.audit("render", pairs, self.target, self.base,
                                         {self.function: dict(cur=100, max=100)}, facts=self.facts)

    def test_retail_case_selector_and_relocated_equivalent_are_exact(self):
        raw = compiler_tables._validate(self.target, self.owner, self.facts)
        self.assertEqual(raw[457], 0)
        audit, gates = self.audit()
        self.assertEqual([r["datum_status"] for r in audit], ["EXACT", "EXACT"])
        self.assertEqual(gates[0]["resolution"], "EXACT")

    def test_actual_old_candidate_missing_type1_is_bytes_debt_at_code100(self):
        self.base = SurfaceLoadImage(0x645650, omitted_static_mesh=True)
        compiler_tables._validate(self.base, self.base_owner, self.facts)
        audit, gates = self.audit()
        self.assertEqual([r["datum_status"] for r in audit].count("BYTES"), 1)
        self.assertEqual(gates[0]["resolution"], "OPEN")

    def test_changed_member_store_default_padding_and_relocations_fail_closed(self):
        for offset in (0, 14, 304, 310, 317, 336, 339, 418, 426):
            with self.subTest(offset=offset):
                self.base = SurfaceLoadImage(0x645650)
                self.base.data[offset] ^= 1
                _, gates = self.audit()
                self.assertEqual(gates[0]["resolution"], "OPEN")
        for site in (325, 332, 428, 452):
            with self.subTest(site=site):
                self.base = SurfaceLoadImage(0x645650)
                self.base.relocs.remove(self.base.start + site)
                _, gates = self.audit()
                self.assertEqual(gates[0]["resolution"], "OPEN")

    def test_new_profile_cannot_enroll_another_identity_or_extent(self):
        facts = copy.deepcopy(self.facts)
        facts[1]["unit"] = "unreviewed.cpp"
        with self.assertRaises(ValueError):
            compiler_tables._validate(self.target, self.owner, facts)
        self.base_owner["size"] = 503
        _, gates = self.audit()
        self.assertEqual(gates[0]["resolution"], "OPEN")

    def test_selector_out_of_range_and_nonboundary_jump_fail_closed(self):
        self.base.data[457] = 7
        _, gates = self.audit()
        self.assertEqual(gates[0]["resolution"], "OPEN")
        self.base = SurfaceLoadImage(0x645650)
        struct.pack_into("<I", self.base.data, 428, self.base.image_base + self.base.start + 397)
        _, gates = self.audit()
        self.assertEqual(gates[0]["resolution"], "OPEN")


if __name__ == "__main__":
    unittest.main()
