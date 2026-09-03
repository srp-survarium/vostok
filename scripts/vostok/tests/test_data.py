# SPDX-License-Identifier: GPL-3.0-or-later

import dataclasses
import struct
import tempfile
import unittest
from unittest import mock
from collections import Counter
from pathlib import Path

from vostok.data.inventory import DataSymbol
from vostok.data.inventory import identity_for
from vostok.data.gate import _summary as _gate_summary
from vostok.data.missing import (
    Candidate,
    ExtentIndex,
    PdbDataCoverage,
    PdbCodeCoverage,
    PdbExtent,
    PdbReferent,
    Symbol,
    _is_game_candidate,
    _derive_eh_symbols,
    _derive_seh_symbols,
    _validate_pdb_extents,
    _validate_referents,
    _validate_reviewed,
    load_symbols,
)
from vostok.data.manifest import (
    _canonical_name,
    _classify_function_refs,
    _direct_consumers,
    _section_properties,
)
from vostok.data.pe import PEImage
from vostok.data.pipeline import (
    AddressResolver,
    _FunctionIndex,
    _access_form,
    _access_kind,
    _apply_access_extents,
    _apply_target_extents,
    _comparison_key,
    _consumer_fallback_key,
    _derived_target_extent,
    _function_identity_tokens,
    _has_identity_extent,
    _infer_base_symbols_from_data_xrefs,
    _infer_base_symbols_from_code_xrefs,
    _paired_function_access_referents,
    _interval_union,
    _load_pdb_functions,
    _load_function_symbols,
    _ordinal_local,
    _physical_allocations,
    _relocation_signatures_match,
    _transfer_target_extents,
    compare,
)
from vostok.data.cli import _all_zero_failures
from vostok.data.render_relocs import (
    Access,
    CallConeIndex,
    Datum,
    DatumIndex,
    Site,
    _accept_unique,
    _classify_extentless_comparison,
    _datum_token,
    _datum_token_aliases,
    _direct_call_graph,
    _extentless_end,
    _first_diff,
    _function_data_rows,
    _function_data_resolution,
    _current_source_hash,
    _function_fingerprint_pairs,
    _pattern,
    _problem_tags,
    _record_function_identities,
    _relocation_target_differences,
    _relocation_targets_match,
    _select_candidate_vote,
    _select_extentless_candidate,
    _window,
)
from vostok.data.reviews import matches as _review_matches


class FunctionDataResolutionTests(unittest.TestCase):
    def test_extentless_review_hashes_current_owning_unit(self):
        with tempfile.TemporaryDirectory() as directory:
            source = Path(directory) / "sources/vostok/particle/action.h"
            source.parent.mkdir(parents=True)
            source.write_text("class action {};\n", encoding="latin-1")
            with mock.patch(
                "vostok.derive.maxima.SOURCES", Path(directory) / "sources"
            ):
                self.assertEqual(
                    _current_source_hash(
                        "??_Gaction@@UAEPAXI@Z",
                        "vostok/particle/action.h",
                        {"module": "particle", "hash": "stale-ledger"},
                        None,
                    ),
                    "e2f12c7bf31a",
                )

    def test_gfx_review_hashes_lib_overlay_before_pristine_sdk(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            overlay = root / "sources/scaleform/sdk-overlay/GFx/example.cpp"
            sdk = root / "sdk/Src/GFx/example.cpp"
            overlay.parent.mkdir(parents=True)
            sdk.parent.mkdir(parents=True)
            overlay.write_text("overlay\n", encoding="latin-1")
            sdk.write_text("sdk\n", encoding="latin-1")
            with (
                mock.patch(
                    "vostok.derive.maxima.SOURCES", root / "sources"
                ),
                mock.patch(
                    "vostok.data.render_relocs.paths.SCALEFORM_SDK",
                    root / "sdk",
                ),
            ):
                self.assertEqual(
                    _current_source_hash(
                        "?gfx@@YAXXZ",
                        "src/gfx/example.cpp",
                        {"module": "gfx"},
                        None,
                    ),
                    "e3eb0451f80e",
                )

    def test_direct_exact_needs_no_ledger_evidence(self):
        self.assertEqual(_function_data_resolution("EXACT", {}), "EXACT")

    def test_hash_scoped_max_does_not_acquit_wrong_data(self):
        row = {"max": 100.0, "status": "done", "hash": "abc", "note": ""}
        self.assertEqual(
            _function_data_resolution("BASE_DEFINITION_MISSING", row),
            "OPEN",
        )

    def test_current_exact_does_not_acquit_wrong_data(self):
        row = {"cur": 100.0, "max": 99.0, "hash": "abc"}
        self.assertEqual(
            _function_data_resolution("USE_DIFF", row),
            "OPEN",
        )

    def test_parked_code_note_does_not_acquit_data_drift(self):
        row = {
            "max": 99.0,
            "status": "parked",
            "hash": "abc",
            "note": "exact structure; inline boundary",
        }
        self.assertEqual(
            _function_data_resolution("USE_DIFF", row), "OPEN"
        )

    def test_two_sided_missing_definition_stays_open(self):
        row = {
            "max": 99.0,
            "status": "parked",
            "hash": "abc",
            "note": "build path",
        }
        self.assertEqual(
            _function_data_resolution("DEFINITION_MISSING_BOTH", row), "OPEN"
        )

    def test_unreviewed_difference_stays_open(self):
        self.assertEqual(
            _function_data_resolution("USE_DIFF", {"status": "inprogress"}),
            "OPEN",
        )

    def test_exact_call_cone_settles_inline_boundary(self):
        self.assertEqual(
            _function_data_resolution(
                "USE_DIFF", {}, cone_status="EXACT"
            ),
            "CALL_CONE_EXACT",
        )

    def test_current_diff_scoped_review_settles_wall(self):
        self.assertEqual(
            _function_data_resolution(
                "USE_DIFF", {}, cone_status="DIFF", reviewed=True
            ),
            "REVIEWED_WALL",
        )

    def test_review_requires_current_source_and_diff_hashes(self):
        review = {
            "status": "bounded",
            "src_hash": "source-a",
            "diff_hash": "diff-a",
        }
        self.assertTrue(_review_matches(review, "source-a", "diff-a"))
        self.assertFalse(_review_matches(review, "source-b", "diff-a"))
        self.assertFalse(_review_matches(review, "source-a", "diff-b"))

    def test_open_review_is_not_terminal(self):
        review = {
            "status": "open",
            "src_hash": "source-a",
            "diff_hash": "diff-a",
        }
        self.assertFalse(_review_matches(review, "source-a", "diff-a"))


class DataGateTests(unittest.TestCase):
    class _EmptyIndex:
        exact = {}
        by_identity = {}

        @staticmethod
        def evidence_owner(_rva, _hint=""):
            return None

        @staticmethod
        def resolve_all(_rva):
            return frozenset(("E:candidate_only",))

    class _TokenIndex(_EmptyIndex):
        def __init__(self, token):
            self.token = token

        def resolve_all(self, _rva):
            return frozenset((self.token,))

    def test_exact_paired_window_aliases_unstable_content_tokens(self):
        audit = [{
            "pair_status": "PAIRED",
            "unit": "unit.cpp",
            "function": "?function@@YAXXZ",
            "target_function_rva": "0x1000",
            "base_function_rva": "0x2000",
            "target_target_rva": "0x3000",
            "base_target_rva": "0x4000",
            "target_identity": "E:same+0x0",
            "base_identity": "E:same+0x0",
            "target_access": "address",
            "base_access": "address",
            "identity_status": "EXACT",
            "datum_status": "EXACT",
        }]
        rows = _function_data_rows(
            audit,
            self._TokenIndex("C:target-alias-set"),
            self._TokenIndex("C:base-alias-set"),
            {},
        )
        self.assertEqual(rows[0]["status"], "EXACT")
        self.assertEqual(rows[0]["resolution"], "EXACT")

    def test_logical_icf_owner_inherits_cross_module_candidate_body(self):
        target_access = Access(
            site=0x1010,
            instruction=0x100F,
            target=0x3000,
            access="read",
            width="dword",
            form="direct",
            scale=0,
            identity="E:shared",
            instruction_text="mov eax, dword ptr [0x3000]",
            function="?retail_alias@@YAXXZ",
            unit="unit.cpp",
            function_rva=0x1000,
            function_size=0x20,
            partner_rva=0x2000,
        )
        base_access = dataclasses.replace(
            target_access,
            site=0x2010,
            instruction=0x200F,
            target=0x4000,
            instruction_text="mov eax, dword ptr [0x4000]",
            function="?candidate_representative@@YAXXZ",
            unit="other.cpp",
            function_rva=0x2000,
            partner_rva=0x1000,
        )
        index = self._TokenIndex("E:shared")
        rows = _function_data_rows(
            [],
            index,
            index,
            {},
            target_sites={0x1010: Site(0x1010, (target_access,))},
            base_sites={},
            base_function_accesses={0x2000: (base_access,)},
        )
        self.assertEqual(len(rows), 1)
        self.assertEqual(rows[0]["function"], "?retail_alias@@YAXXZ")
        self.assertEqual(rows[0]["target_datum_count"], "1")
        self.assertEqual(rows[0]["base_datum_count"], "1")
        self.assertEqual(rows[0]["resolution"], "EXACT")

    def test_candidate_only_uses_in_paired_function_reach_zero_gate(self):
        audit = [{
            "pair_status": "BASE_ONLY",
            "unit": "unit.cpp",
            "function": "?function@@YAXXZ",
            "target_function_rva": "0x1000",
            "base_function_rva": "0x2000",
            "target_target_rva": "-",
            "base_target_rva": "0x3000",
            "target_identity": "-",
            "base_identity": "E:candidate_only",
            "datum_status": "NO_TARGET_OWNER",
        }]
        rows = _function_data_rows(
            audit, self._EmptyIndex(), self._EmptyIndex(), {},
        )
        self.assertEqual(len(rows), 1)
        self.assertEqual(rows[0]["target_datum_count"], "0")
        self.assertEqual(rows[0]["base_datum_count"], "1")
        self.assertEqual(rows[0]["status"], "TARGET_DEFINITION_MISSING")
        self.assertEqual(rows[0]["resolution"], "OPEN")

    def test_call_cone_proves_datum_moved_from_callee_into_caller(self):
        audit = [{
            "pair_status": "BASE_ONLY",
            "unit": "unit.cpp",
            "function": "?function@@YAXXZ",
            "target_function_rva": "0x1000",
            "base_function_rva": "0x2000",
            "target_target_rva": "-",
            "base_target_rva": "0x3000",
            "target_identity": "-",
            "base_identity": "E:candidate_only",
            "datum_status": "NO_TARGET_OWNER",
        }]
        target_cones = CallConeIndex(
            {0x1000: frozenset({0x1100})},
            {0x1100: frozenset({"E:candidate_only"})},
        )
        base_cones = CallConeIndex(
            {}, {0x2000: frozenset({"E:candidate_only"})}
        )
        rows = _function_data_rows(
            audit,
            self._EmptyIndex(),
            self._EmptyIndex(),
            {},
            target_cones,
            base_cones,
        )
        self.assertEqual(rows[0]["status"], "TARGET_DEFINITION_MISSING")
        self.assertEqual(rows[0]["cone_status"], "EXACT")
        self.assertEqual(rows[0]["resolution"], "CALL_CONE_EXACT")
        self.assertEqual(
            rows[0]["cone_referent_paths"],
            "E:candidate_only@T:0x1000>0x1100,B:0x2000",
        )

    def test_call_cone_does_not_acquit_a_wrong_datum(self):
        audit = [{
            "pair_status": "BASE_ONLY",
            "unit": "unit.cpp",
            "function": "?function@@YAXXZ",
            "target_function_rva": "0x1000",
            "base_function_rva": "0x2000",
            "target_target_rva": "-",
            "base_target_rva": "0x3000",
            "target_identity": "-",
            "base_identity": "E:candidate_only",
            "datum_status": "NO_TARGET_OWNER",
        }]
        rows = _function_data_rows(
            audit,
            self._EmptyIndex(),
            self._EmptyIndex(),
            {},
            CallConeIndex({}, {}),
            CallConeIndex(
                {}, {0x2000: frozenset({"E:candidate_only"})}
            ),
        )
        self.assertEqual(rows[0]["cone_status"], "DIFF")
        self.assertEqual(rows[0]["resolution"], "OPEN")

    def test_aggregate_counts_only_open_resolution_as_debt(self):
        summary = _gate_summary({
            "game": {
                "target_relocation_sites": 10,
                "base_relocation_sites": 11,
                "open_function_data": 2,
            },
            "render": {
                "target_relocation_sites": 20,
                "base_relocation_sites": 19,
                "open_function_data": 0,
            },
        })
        self.assertEqual(summary, {
            "modules": 2,
            "target_relocation_sites": 30,
            "base_relocation_sites": 30,
            "open_function_data": 2,
        })

    def test_all_data_zero_gate_covers_raw_projection_and_referent_debt(self):
        report = {
            "counts": {"EXACT": 10, "BYTES": 2, "TARGET_ONLY": 1},
            "coverage": {
                "objdiff_matched_bytes": 80,
                "objdiff_projected_bytes": 100,
            },
            "consumer_projection": {
                "blockers": 3,
                "allocation_copies": 8,
                "paired_copies": 7,
                "allocation_copy_bytes": 100,
                "paired_copy_bytes": 90,
                "unique_allocation_bytes": 60,
                "paired_unique_bytes": 50,
                "functions": {"EXACT": 4, "WRONG_REFERENT": 1},
                "wrong_referent_regions": 1,
            },
            "strict_referents": {
                "referent_debt_functions": 1,
                "referent_debt_code_bytes": 20,
            },
        }
        failures = _all_zero_failures(report)
        self.assertIn("BYTES=2", failures)
        self.assertIn("TARGET_ONLY=1", failures)
        self.assertIn("blockers=3", failures)
        self.assertIn("functions.WRONG_REFERENT=1", failures)
        self.assertIn("paired_copies=7/8 allocation_copies", failures)
        self.assertIn("objdiff_matched_bytes=80/100 projected", failures)
        self.assertIn("referent_debt_functions=1", failures)

    def test_all_data_zero_gate_accepts_complete_exact_state(self):
        report = {
            "counts": {"EXACT": 10},
            "coverage": {
                "objdiff_matched_bytes": 100,
                "objdiff_projected_bytes": 100,
            },
            "consumer_projection": {
                "blockers": 0,
                "allocation_copies": 8,
                "paired_copies": 8,
                "allocation_copy_bytes": 100,
                "paired_copy_bytes": 100,
                "unique_allocation_bytes": 60,
                "paired_unique_bytes": 60,
                "functions": {"EXACT": 4},
            },
            "strict_referents": {
                "referent_debt_functions": 0,
                "referent_debt_code_bytes": 0,
            },
        }
        self.assertEqual(_all_zero_failures(report), [])


def _synthetic_pe(path: Path) -> None:
    data = bytearray(0x800)
    data[:2] = b"MZ"
    struct.pack_into("<I", data, 0x3C, 0x80)
    data[0x80:0x84] = b"PE\0\0"
    coff = 0x84
    struct.pack_into("<HHIIIHH", data, coff, 0x14C, 4, 0, 0, 0, 0xE0, 0x102)
    optional = coff + 20
    struct.pack_into("<H", data, optional, 0x10B)
    struct.pack_into("<I", data, optional + 28, 0x10000)
    struct.pack_into("<I", data, optional + 92, 16)
    struct.pack_into("<II", data, optional + 96 + 5 * 8, 0x4000, 12)
    sections = (
        (b".text", 0x100, 0x1000, 0x100, 0x200),
        (b".rdata", 0x100, 0x2000, 0x100, 0x300),
        (b".data", 0x200, 0x3000, 0x20, 0x400),
        (b".reloc", 0x100, 0x4000, 0x100, 0x500),
    )
    cursor = optional + 0xE0
    for name, virtual_size, rva, raw_size, raw_offset in sections:
        data[cursor:cursor + 8] = name.ljust(8, b"\0")
        struct.pack_into("<IIIIIIHHI", data, cursor + 8, virtual_size, rva,
                         raw_size, raw_offset, 0, 0, 0, 0, 0)
        cursor += 40
    data[0x400:0x420] = bytes(range(0x20))
    struct.pack_into("<IIHH", data, 0x500, 0x3000, 12, 0x3004, 0)
    path.write_bytes(data)


class PEImageTests(unittest.TestCase):
    def test_reads_loader_zero_bss_and_highlow_relocations(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.exe"
            _synthetic_pe(path)
            image = PEImage(path)
            self.assertEqual(image.image_base, 0x10000)
            self.assertEqual(image.read_rva(0x301C, 8), bytes(range(0x1C, 0x20)) + bytes(4))
            self.assertEqual(image.base_relocations(), (0x3004,))

    def test_direct_call_graph_uses_encoded_destination_not_display_name(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.exe"
            _synthetic_pe(path)
            data = bytearray(path.read_bytes())
            data[0x200:0x205] = b"\xE8" + struct.pack("<i", 0x1010 - 0x1005)
            path.write_bytes(data)
            records = [
                {
                    "rva": 0x1000,
                    "size": 8,
                    "instructions": [{
                        "off": 0, "len": 5, "text": "call ambiguous"
                    }],
                },
                {"rva": 0x1010, "size": 1, "instructions": []},
            ]
            self.assertEqual(
                _direct_call_graph(records, PEImage(path)),
                {0x1000: frozenset({0x1010})},
            )


class DataIdentityTests(unittest.TestCase):
    def test_external_identity_uses_public_mangled_name(self):
        self.assertEqual(
            identity_for("external", b"x.obj", b"x.lib", b"plain", b"?value@@3HA"),
            "E:?value@@3HA",
        )

    def test_local_identity_keeps_archive_and_compiland(self):
        self.assertEqual(
            identity_for("local", b"C:\\obj\\file.obj", b"C:\\lib\\core.lib",
                         b"local", b""),
            "L:core.lib:file.obj:local",
        )


class NonPdbDataTests(unittest.TestCase):
    @staticmethod
    def _pdb_symbol(rva=0x2000, size=8):
        return DataSymbol(
            rva=rva, section=".rdata", storage="rdata", size=size,
            size_kind="pdb-type", type_index=0x1000, scope="external",
            module_path=b"", archive_path=b"", name=b"known",
            public_name=b"known", identity="E:known", owner_module="test",
        )

    def test_pdb_coverage_removes_exact_and_interior_referents(self):
        coverage = PdbDataCoverage([self._pdb_symbol()])
        self.assertTrue(coverage.owns(0x2000))
        self.assertTrue(coverage.owns(0x2007))
        self.assertFalse(coverage.owns(0x2008))

    def test_reviewed_pdb_extent_covers_public_only_symbol_interior(self):
        public = self._pdb_symbol(size=None)
        coverage = PdbDataCoverage([public], [PdbExtent(0x2000, 16, "reviewed")])
        self.assertTrue(coverage.owns(0x200C))
        self.assertFalse(coverage.owns(0x2010))

    def test_reviewed_pdb_extent_requires_exact_pdb_start(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.exe"
            _synthetic_pe(path)
            with self.assertRaisesRegex(ValueError, "no exact retail PDB symbol start"):
                _validate_pdb_extents(
                    [PdbExtent(0x2010, 4, "wrong start")],
                    [self._pdb_symbol(size=None)],
                    PEImage(path),
                )

    def test_symbol_addend_referent_stays_out_of_non_pdb_queue(self):
        symbol = self._pdb_symbol(rva=0x2000, size=8)
        candidate = Candidate(
            0x1010, ".text", 0x2008, ".rdata",
            "source.obj", "source.lib", 0x1000, 0x100,
            "target.obj", "target.lib", 0x2000, 0x100, "outside",
        )
        referent = PdbReferent(
            0x1000, 0x2008, 0x1010, "known", 8, 1, "one-past"
        )
        self.assertEqual(
            _validate_referents([referent], [candidate], [symbol]),
            {(0x1010, 0x2008)},
        )

    def test_reviewed_non_pdb_extent_owns_interior_referents(self):
        symbol = Symbol(0x2100, "lookup", 16, "u32[4]", "reviewed")
        owner, ambiguous = ExtentIndex([symbol]).owner(0x210C)
        self.assertEqual(owner, symbol)
        self.assertFalse(ambiguous)
        self.assertTrue(symbol.reviewed)

    def test_derived_extent_is_not_manual_review(self):
        symbol = Symbol(
            0x2100, "DAT_00012100", 16, "u32[4]",
            "derived: inferred from a contribution",
        )
        self.assertTrue(symbol.complete)
        self.assertFalse(symbol.reviewed)

    def test_unknown_placeholder_is_not_manual_review(self):
        symbol = Symbol(
            0x2100, "DAT_00012100", None, "unknown",
            "auto: absent from retail PDB",
        )
        self.assertFalse(symbol.complete)
        self.assertFalse(symbol.reviewed)

    def test_every_registry_field_is_required_for_manual_review(self):
        complete = Symbol(0x2100, "datum", 4, "u32", "reviewed from retail")
        self.assertTrue(complete.complete)
        self.assertTrue(complete.reviewed)
        for symbol in (
            Symbol(0x2100, "", 4, "u32", "reviewed from retail"),
            Symbol(0x2100, "datum", 4, "", "reviewed from retail"),
            Symbol(0x2100, "datum", 4, "u32", ""),
        ):
            self.assertFalse(symbol.complete)
            self.assertFalse(symbol.reviewed)

    def test_registry_schema_is_exact(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "data_symbols.tsv"
            path.write_text(
                "rva\tname\tsize\ttype\tcomment\n"
                "0x2100\tDAT_00012100\t?\tunknown\tauto: absent from retail PDB\n",
                encoding="utf-8",
            )
            symbols = load_symbols(path)
            self.assertEqual(len(symbols), 1)
            self.assertFalse(symbols[0].complete)

    def test_registry_rejects_a_symbol_already_owned_by_pdb(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.exe"
            _synthetic_pe(path)
            with self.assertRaisesRegex(ValueError, "already in the PDB"):
                _validate_reviewed(
                    [Symbol(0x2000, "duplicate", 4, "u32", "reviewed")],
                    PEImage(path),
                    PdbDataCoverage([self._pdb_symbol(size=4)]),
                )

    def test_missing_fence_classification_matches_gruntz_policy(self):
        def candidate(module, archive):
            return Candidate(
                0x1010, ".text", 0x2000, ".rdata",
                module, archive, 0x1000, 0x100,
                "target.obj", "target.lib", 0x2000, 0x100, "missing",
            )

        self.assertFalse(_is_game_candidate(candidate(
            r"..\Obj\Shipping\vendor.obj",
            r"C:\survarium\binaries.prebuilt\Win32\libraries\vendor.lib",
        )))
        self.assertFalse(_is_game_candidate(candidate("* Linker *", "")))
        self.assertTrue(_is_game_candidate(candidate(
            r"..\binaries\Win32\intermediates\Master Gold\render\world.obj",
            r"c:\survarium\binaries\win32\libraries\vostok_render.lib",
        )))
        self.assertTrue(_is_game_candidate(candidate("-", "-")))

    def test_retail_funcinfo_derives_its_unwind_map_extent(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.exe"
            _synthetic_pe(path)
            data = bytearray(path.read_bytes())
            struct.pack_into(
                "<IiIiIiIII", data, 0x320,
                0x19930522, 3, 0x12060, 0, 0, 0, 0, 0, 1,
            )
            path.write_bytes(data)
            rows = {
                0x2020: [Candidate(
                    0x1010, ".text", 0x2020, ".rdata",
                    "vendor.obj", "vendor.lib", 0x1000, 0x100,
                    "vendor.obj", "vendor.lib", 0x2000, 0x100, "missing",
                )],
                0x2060: [Candidate(
                    0x2028, ".rdata", 0x2060, ".rdata",
                    "vendor.obj", "vendor.lib", 0x2000, 0x100,
                    "vendor.obj", "vendor.lib", 0x2000, 0x100, "missing",
                )],
            }
            derived = _derive_eh_symbols(PEImage(path), rows, [])
            self.assertEqual(
                [(row.rva, row.size, row.type_name) for row in derived],
                [
                    (0x2020, 0x24, "EH::FuncInfo"),
                    (0x2060, 0x18, "EH::UnwindMapEntry[3]"),
                ],
            )

    def test_retail_seh4_table_derives_only_valid_scope_records(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.exe"
            _synthetic_pe(path)
            data = bytearray(path.read_bytes())
            data[0x22F] = 0x68
            struct.pack_into(
                "<iiiiiII", data, 0x380,
                -2, 0, -0x40, 0, -2, 0x1010, 0x1020,
            )
            path.write_bytes(data)
            row = Candidate(
                0x1030, ".text", 0x2080, ".rdata",
                "vendor.obj", "vendor.lib", 0x1000, 0x100,
                "vendor.obj", "vendor.lib", 0x2000, 0x100, "missing",
            )
            derived = _derive_seh_symbols(
                PEImage(path), {0x2080: [row], 0x20A0: [row]}, [], [],
            )
            self.assertEqual(
                [(symbol.rva, symbol.size, symbol.type_name) for symbol in derived],
                [(0x2080, 0x1C, "SEH4::ScopeTable[1]")],
            )

    def test_code_contribution_cannot_overlap_a_pdb_function(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.exe"
            _synthetic_pe(path)
            code = PdbCodeCoverage([{"rva": 0x1040, "size": 0x10}])
            with self.assertRaisesRegex(ValueError, "overlaps PDB-owned"):
                _validate_reviewed(
                    [Symbol(
                        0x1020, "DAT_00011020", 0x60,
                        "COFF::CodeContribution", "derived",
                    )],
                    PEImage(path), PdbDataCoverage([]), code,
                )


class DataPipelineTests(unittest.TestCase):
    @staticmethod
    def _symbol(
        rva: int, name: bytes, public_name: bytes = b"",
        identity: str = "L:module:unit.obj:value",
    ) -> DataSymbol:
        return DataSymbol(
            rva=rva, section=".rdata", storage="rdata", size=4,
            size_kind="type", type_index=1, scope="local",
            module_path=b"unit.obj", archive_path=b"module.lib", name=name,
            public_name=public_name, identity=identity,
            owner_module="module",
        )

    def test_comparison_key_uses_qualified_pdb_name_for_same_leaf_locals(self):
        first = self._symbol(0x2000, b"value", b"?value@first@@3HA")
        second = self._symbol(0x2004, b"value", b"?value@second@@3HA")
        self.assertNotEqual(_comparison_key(first), _comparison_key(second))

    def test_compiler_public_symbol_extents_are_derived_from_retail_bytes(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.exe"
            _synthetic_pe(path)
            data = bytearray(path.read_bytes())
            data[0x300:0x306] = b"hello\0"
            struct.pack_into("<II", data, 0x320, 0x11000, 0x11000)
            path.write_bytes(data)
            image = PEImage(path)
            string = dataclasses.replace(
                self._symbol(0x2000, b"??_C@_05dummy@hello?$AA@"),
                scope="external", size=None, type_index=0,
            )
            real = dataclasses.replace(
                self._symbol(0x2010, b"__real@3f800000"),
                scope="external", size=None, type_index=0,
            )
            vtable = dataclasses.replace(
                self._symbol(0x2020, b"??_7owner@@6B@"),
                scope="external", size=None, type_index=0,
            )
            rtti = dataclasses.replace(
                self._symbol(0x2040, b"??_R0?AVowner@@@8"),
                scope="external", size=None, type_index=0,
            )
            data[0x348:0x354] = b".?AVowner@@\0"
            path.write_bytes(data)
            image = PEImage(path)
            self.assertEqual(
                _derived_target_extent(string, image, frozenset()),
                (6, "derived-string"),
            )
            self.assertEqual(
                _derived_target_extent(real, image, frozenset()),
                (4, "derived-fppool"),
            )
            self.assertEqual(
                _derived_target_extent(
                    vtable, image, frozenset({0x2020, 0x2024})
                ),
                (8, "derived-vtable"),
            )
            self.assertEqual(
                _derived_target_extent(rtti, image, frozenset()),
                (20, "derived-rtti-type-descriptor"),
            )

    def test_fixed_abi_public_symbol_extents_are_derived_by_class(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.exe"
            _synthetic_pe(path)
            image = PEImage(path)

            def extent(name: bytes):
                symbol = dataclasses.replace(
                    self._symbol(0x2000, name),
                    scope="external", size=None, type_index=0,
                )
                return _derived_target_extent(symbol, image, frozenset())

            self.assertEqual(extent(b"__GUID_01234567_89ab_cdef_0123_456789abcdef"),
                             (16, "derived-guid"))
            self.assertEqual(extent(b"??_B?1??owner@@YAXXZ@51"),
                             (1, "derived-static-guard"))
            self.assertEqual(extent(b"?stored_vtable@owner@@4Utable@@B"),
                             (8, "derived-boost-function-vtable"))
            self.assertEqual(extent(b"?error@placeholders@@3U?$arg@$00@boost@@A"),
                             (1, "derived-boost-placeholder"))
            self.assertEqual(
                extent(
                    b"??_7?$verify_callback@V?$bind_t@_NVowner@@"
                    b"U?$arg@$00@boost@@@detail@@6B@"
                ),
                None,
            )
            self.assertEqual(extent(b"?current_id@owner@@4GA"),
                             (2, "derived-u16-static"))
            self.assertEqual(extent(b"?v@owner@@4T__m128i@@A"),
                             (16, "derived-simd-constant"))
            self.assertEqual(extent(b"__DELAY_IMPORT_DESCRIPTOR_USER32_dll"),
                             (32, "derived-delay-import-descriptor"))
            self.assertEqual(extent(b"___xc_z"),
                             (4, "derived-crt-initializer-sentinel"))
            self.assertEqual(extent(b"__TI2?AVbad_alloc@std@@"),
                             (16, "derived-throw-info"))
            self.assertEqual(extent(b"__CTA4?AVout_of_range@stlp_std@@"),
                             (20, "derived-catchable-type-array"))
            self.assertEqual(extent(b"__CT??_R0?AVbad_alloc@std@@@8copy"),
                             (28, "derived-catchable-type"))
            self.assertEqual(extent(b"??_8?$basic_istream@D@@7B@"),
                             (8, "derived-stlport-vbtable"))
            self.assertEqual(extent(b"___clocalestr"),
                             (8, "derived-crt-locale-anchor"))
            self.assertEqual(extent(b"___lc_handle"),
                             (24, "derived-crt-locale-handles"))

    def test_resolver_recognizes_exact_negative_four_datum_addend(self):
        symbol = self._symbol(0x2004, b"array")
        resolver = AddressResolver([symbol], [])
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.exe"
            _synthetic_pe(path)
            image = PEImage(path)
            self.assertIs(resolver.data_symbol_at(0x2000), symbol)
            self.assertEqual(
                resolver.resolve(0x2000, image),
                "D:L:module:unit.obj:value-0x4",
            )
            self.assertIsNone(resolver.data_symbol_at(0x1FFC))

    def test_access_extent_expands_for_real_reads_not_address_uses(self):
        first = dataclasses.replace(self._symbol(0x2000, b"first"), size=1)
        second = dataclasses.replace(self._symbol(
            0x2004, b"second", identity="L:module:unit.obj:second"
        ), size=1)
        rows = (
            "site_rva\tinstruction_rva\taccess\twidth\tend_rva\tform\tscale\t"
            "target_rva\ttarget_identity\tcaller_mangled\tcaller_name\t"
            "caller_file\tinstruction\n"
            "0x1000\t0x1000\treadwrite\tdword\t0x2004\tdirect\t0\t0x2000\t"
            "D:first\tf\tf\tu.cpp\tor dword ptr [first], eax\n"
            "0x1004\t0x1004\taddress\tdword\t0x2008\tdirect\t0\t0x2004\t"
            "D:second\tf\tf\tu.cpp\tmov [out], second\n"
        )
        with tempfile.TemporaryDirectory() as directory:
            access = Path(directory) / "access.tsv"
            access.write_text(rows, encoding="utf-8")
            with mock.patch("vostok.data.pipeline.access_path", return_value=access):
                result = _apply_access_extents([first, second], "target")
        self.assertEqual(
            [(row.size, row.size_kind) for row in result],
            [(4, "retail-access"), (1, "type")],
        )

    def test_code_xref_infers_unique_candidate_allocation_alias(self):
        target = self._symbol(0x2000, b"value")
        base_image_path = None
        row = {
            "pair_status": "PAIRED",
            "access_status": "EXACT",
            "target_datum_rva": "0x2000",
            "inferred_base_datum_rva": "0x2020",
        }
        with tempfile.TemporaryDirectory() as directory:
            base_image_path = Path(directory) / "base.exe"
            _synthetic_pe(base_image_path)
            inferred = _infer_base_symbols_from_code_xrefs(
                [target], [], PEImage(base_image_path), [row]
            )
        self.assertEqual(len(inferred), 1)
        self.assertEqual(
            (inferred[0].rva, inferred[0].identity, inferred[0].size_kind),
            (0x2020, target.identity, "retail-code-xref"),
        )

    def test_code_xref_rejects_ambiguous_candidate_vote(self):
        target = self._symbol(0x2000, b"value")
        rows = [
            {
                "pair_status": "PAIRED", "access_status": "EXACT",
                "target_datum_rva": "0x2000",
                "inferred_base_datum_rva": candidate,
            }
            for candidate in ("0x2020", "0x2030")
        ]
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "base.exe"
            _synthetic_pe(path)
            self.assertEqual(
                _infer_base_symbols_from_code_xrefs(
                    [target], [], PEImage(path), rows
                ),
                [],
            )

    def test_code_xref_accepts_overwhelming_candidate_vote(self):
        target = self._symbol(0x2000, b"value")
        rows = [
            {
                "pair_status": "PAIRED", "access_status": "EXACT",
                "target_datum_rva": "0x2000",
                "inferred_base_datum_rva": candidate,
            }
            for candidate in ("0x2020",) * 8 + ("0x2030",)
        ]
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "base.exe"
            _synthetic_pe(path)
            inferred = _infer_base_symbols_from_code_xrefs(
                [target], [], PEImage(path), rows
            )
        self.assertEqual([row.rva for row in inferred], [0x2020])

    def test_code_xref_does_not_replace_exact_candidate_pdb_datum(self):
        target = self._symbol(0x2000, b"retail_value")
        candidate = dataclasses.replace(
            self._symbol(0x2020, b"candidate_value"),
            size=None,
            size_kind="unknown",
            type_index=0,
        )
        row = {
            "pair_status": "PAIRED",
            "access_status": "EXACT",
            "target_datum_rva": "0x2000",
            "inferred_base_datum_rva": "0x2020",
        }
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "base.exe"
            _synthetic_pe(path)
            inferred = _infer_base_symbols_from_code_xrefs(
                [target], [candidate], PEImage(path), [row]
            )
        self.assertEqual(inferred, [])

    def test_code_xref_prefers_function_offset_over_sequence_vote(self):
        target = self._symbol(0x2000, b"value")
        rows = [
            {
                "pair_status": "PAIRED", "access_status": "EXACT",
                "pair_kind": pair_kind,
                "target_datum_rva": "0x2000",
                "inferred_base_datum_rva": candidate,
            }
            for pair_kind, candidate in (
                ("function-offset", "0x2020"),
                ("access-sequence", "0x2030"),
            )
        ]
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "base.exe"
            _synthetic_pe(path)
            inferred = _infer_base_symbols_from_code_xrefs(
                [target], [], PEImage(path), rows
            )
        self.assertEqual([row.rva for row in inferred], [0x2020])

    def test_complete_named_function_access_sequence_pairs_vendor_referent(self):
        header = (
            "site_rva\tinstruction_rva\taccess\twidth\tend_rva\tform\tscale\t"
            "target_rva\ttarget_identity\tcaller_mangled\tcaller_name\t"
            "caller_file\tinstruction\n"
        )
        target_row = (
            "0x1010\t0x100e\tread\tdword\t0x3004\tdirect\t0\t0x3000\t"
            "D:target\t_vendor\tvendor\t\tmov eax, [0x13000]\n"
        )
        base_row = (
            "0x2010\t0x200e\tread\tdword\t0x4004\tdirect\t0\t0x4000\t"
            "D:base\t_vendor\tvendor\t\tmov eax, [0x14000]\n"
        )
        functions = {
            "target": [{
                "rva": 0x1000, "size": 0x40, "mangled": "_vendor", "file": "",
            }],
            "base": [{
                "rva": 0x2000, "size": 0x40, "mangled": "_vendor", "file": "",
            }],
        }
        with tempfile.TemporaryDirectory() as directory:
            target_path = Path(directory) / "target.tsv"
            base_path = Path(directory) / "base.tsv"
            target_path.write_text(header + target_row, encoding="utf-8")
            base_path.write_text(header + base_row, encoding="utf-8")
            with (
                mock.patch(
                    "vostok.data.pipeline._load_function_symbols",
                    side_effect=lambda side: functions[side],
                ),
                mock.patch(
                    "vostok.data.pipeline.access_path",
                    side_effect=lambda side: (
                        target_path if side == "target" else base_path
                    ),
                ),
            ):
                self.assertEqual(
                    _paired_function_access_referents(),
                    [(0x3000, 0x4000, "function-sequence")],
                )

    def test_physical_allocation_deduplicates_only_same_rva_and_extent(self):
        duplicate = self._symbol(0x2000, b"value", b"?value@first@@3HA")
        aliases = [duplicate, duplicate, self._symbol(
            0x2004, b"value", b"?value@first@@3HA"
        )]
        self.assertEqual(
            [symbol.rva for symbol in _physical_allocations(aliases)],
            [0x2000, 0x2004],
        )

    def test_reviewed_target_extent_replaces_unusable_pdb_extent(self):
        symbol = self._symbol(0x2000, b"value")
        symbol = dataclasses.replace(symbol, size=0)
        extent = mock.Mock(rva=0x2000, size=0x40)
        with mock.patch(
            "vostok.data.missing.load_pdb_extents", return_value=[extent]
        ):
            result = _apply_target_extents([symbol])
        self.assertEqual((result[0].size, result[0].size_kind), (0x40, "reviewed-retail"))
        self.assertTrue(_has_identity_extent(result[0]))

    def test_unique_target_extent_transfers_to_extentless_candidate(self):
        target = dataclasses.replace(self._symbol(0x2000, b"value"), size=0x40)
        base = dataclasses.replace(self._symbol(0x3000, b"value"), size=0)
        result = _transfer_target_extents([target], [base])
        self.assertEqual((result[0].size, result[0].size_kind), (0x40, "retail-paired"))

    def test_retail_paired_public_anchor_becomes_comparable_allocation(self):
        target = dataclasses.replace(
            self._symbol(0x2000, b"value"),
            scope="external",
            identity="E:?value@@3HA",
            size=4,
        )
        base_anchor = dataclasses.replace(
            target,
            rva=0x2004,
            size=None,
            size_kind="unknown",
            type_index=0,
        )
        base = _transfer_target_extents([target], [base_anchor])[0]
        self.assertTrue(_has_identity_extent(base))
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.exe"
            _synthetic_pe(path)
            image = PEImage(path)
            with mock.patch(
                "vostok.data.pipeline._load_function_symbols", return_value=[]
            ):
                rows = compare([target], [base], image, image)
        self.assertEqual([row["status"] for row in rows], ["EXACT"])

    def test_reviewed_target_extent_replaces_candidate_scalar_extent(self):
        target = dataclasses.replace(
            self._symbol(0x2000, b"value"),
            size=0x40,
            size_kind="reviewed-retail",
        )
        base = dataclasses.replace(self._symbol(0x3000, b"value"), size=8)
        result = _transfer_target_extents([target], [base])
        self.assertEqual((result[0].size, result[0].size_kind), (0x40, "retail-paired"))

    def test_reviewed_datum_transfers_through_exact_paired_data_xref(self):
        with tempfile.TemporaryDirectory() as directory:
            target_path = Path(directory) / "target.exe"
            base_path = Path(directory) / "base.exe"
            _synthetic_pe(target_path)
            _synthetic_pe(base_path)
            for path, destination in ((target_path, 0x2020), (base_path, 0x2040)):
                data = bytearray(path.read_bytes())
                struct.pack_into("<I", data, 0x404, 0x10000 + destination)
                data[0x320:0x324] = b"name"
                data[0x340:0x344] = b"name"
                path.write_bytes(data)
            source_target = dataclasses.replace(
                self._symbol(0x3000, b"source"),
                section=".data", storage="data", size=8,
            )
            source_base = dataclasses.replace(source_target, rva=0x3000)
            reviewed = dataclasses.replace(
                self._symbol(0x2020, b"literal"),
                identity="R:literal", size_kind="reviewed-retail",
            )
            inferred = _infer_base_symbols_from_data_xrefs(
                [source_target], [source_base], [reviewed],
                PEImage(target_path), PEImage(base_path), (0x3004,), (0x3004,),
            )
            self.assertEqual(
                [(row.rva, row.identity, row.size) for row in inferred],
                [(0x2040, "R:literal", 4)],
            )

    def test_same_name_local_allocations_compare_as_a_multiset(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.exe"
            _synthetic_pe(path)
            image = PEImage(path)
            target = [
                self._symbol(0x2000, b"value"),
                self._symbol(0x2004, b"value"),
            ]
            base = [
                self._symbol(0x2008, b"value"),
                self._symbol(0x200C, b"value"),
            ]
            with mock.patch(
                "vostok.data.pipeline._load_function_symbols", return_value=[]
            ):
                rows = compare(target, base, image, image)
            self.assertEqual([row["status"] for row in rows], ["EXACT", "EXACT"])
            self.assertTrue(all("allocation multiset" in row["note"] for row in rows))

    def test_same_name_multiset_prefers_exact_consumer_ownership(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.exe"
            _synthetic_pe(path)
            image = PEImage(path)
            target = [
                self._symbol(0x2000, b"value"),
                self._symbol(0x2004, b"value"),
            ]
            base = [
                self._symbol(0x2008, b"value"),
                self._symbol(0x200C, b"value"),
            ]
            first = (("first", "unit.cpp", "read", "byte", "direct", "0", 0),)
            second = (("second", "unit.cpp", "read", "byte", "direct", "0", 0),)
            with mock.patch(
                "vostok.data.pipeline._load_function_symbols", return_value=[]
            ), mock.patch(
                "vostok.data.pipeline._consumer_fingerprints",
                side_effect=[
                    {0x2000: first, 0x2004: second},
                    {0x2008: second, 0x200C: first},
                ],
            ):
                rows = compare(target, base, image, image)
            self.assertEqual(
                {(row["target_rva"], row["base_rva"]) for row in rows},
                {("0x2000", "0x200c"), ("0x2004", "0x2008")},
            )

    def test_initializer_bytes_alone_do_not_pair_unrelated_locals(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.exe"
            _synthetic_pe(path)
            image = PEImage(path)
            target = [self._symbol(
                0x2000, b"target", b"?target@owner_a@@3HB",
                "L:module:owner_a.obj:target",
            )]
            base = [self._symbol(
                0x2004, b"base", b"?base@owner_b@@3HB",
                "L:module:owner_b.obj:base",
            )]
            with mock.patch("vostok.data.pipeline._load_function_symbols", return_value=[]), \
                 mock.patch(
                     "vostok.data.pipeline._consumer_fingerprints",
                     return_value={},
                 ):
                rows = compare(target, base, image, image)
            self.assertEqual(
                sorted(row["status"] for row in rows),
                ["BASE_ONLY", "TARGET_ONLY"],
            )

    def test_complete_consumer_fingerprint_pairs_local_ownership_drift(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.exe"
            _synthetic_pe(path)
            image = PEImage(path)
            target = [self._symbol(
                0x2000, b"target", b"?target@owner_a@@3HB",
                "L:module_a:owner_a.obj:target",
            )]
            base = [self._symbol(
                0x2004, b"base", b"?base@owner_b@@3HB",
                "L:module:owner_b.obj:base",
            )]
            fingerprint = (("?owner@@YAXXZ", "owner.cpp", "read", "dword", "direct", "0", 0),)
            with mock.patch("vostok.data.pipeline._load_function_symbols", return_value=[]), \
                 mock.patch(
                     "vostok.data.pipeline._consumer_fingerprints",
                     side_effect=[{0x2000: fingerprint}, {0x2004: fingerprint}],
                 ):
                rows = compare(target, base, image, image)
            self.assertEqual(len(rows), 1)
            self.assertEqual(rows[0]["status"], "EXACT")
            self.assertIn("consumer fingerprint", rows[0]["note"])

    def test_ordinal_local_requires_consumer_pairing(self):
        guard = self._symbol(
            0x2000, b"$S3", identity="L:module:unit.obj:$S3"
        )
        self.assertTrue(_ordinal_local(guard))
        self.assertIsNotNone(_consumer_fallback_key(guard, {0x2000: (("fn",),)}))

    def test_relocation_alias_sets_match_on_any_shared_identity(self):
        self.assertTrue(_relocation_signatures_match(
            [(0, frozenset({"F:M:representative", "F:D:folded"}))],
            [(0, frozenset({"F:M:other", "F:D:folded"}))],
        ))
        self.assertFalse(_relocation_signatures_match(
            [(0, frozenset({"F:M:first"}))],
            [(0, frozenset({"F:M:second"}))],
        ))

    def test_complete_pdb_function_index_preserves_aliases_and_missing_sizes(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "functions.tsv"
            path.write_text(
                "rva\tsize\tname_hex\tmodule_hex\n"
                "0x1000\t12\t5f66756e63\t756e69742e6f626a\n"
                "0x1000\t-\t66756e63\t\n",
                encoding="ascii",
            )
            with mock.patch(
                "vostok.data.pipeline.function_index_path", return_value=path
            ), mock.patch(
                "vostok.data.pipeline._load_rich",
                return_value=[{"file": "module/unit.cpp"}],
            ):
                records = _load_pdb_functions("target")
        self.assertEqual(
            records,
            [
                {
                    "rva": 0x1000, "size": 12, "mangled": "_func",
                    "file": "module/unit.cpp", "module": "unit.obj",
                },
                {
                    "rva": 0x1000, "size": 0, "mangled": "func",
                    "file": "", "module": "",
                },
            ],
        )

    def test_complete_function_alias_inherits_same_rva_compiland_owner(self):
        rich = [{
            "rva": 0x1000, "size": 12, "mangled": "module_name",
            "file": "module/unit.cpp", "name": "module name",
        }]
        aliases = [{
            "rva": 0x1000, "size": 12, "mangled": "public_name",
            "file": "", "module": "",
        }]
        with mock.patch(
            "vostok.data.pipeline._load_rich", return_value=rich
        ), mock.patch(
            "vostok.data.pipeline._load_pdb_functions", return_value=aliases
        ):
            records = _load_function_symbols("base")
        public = next(row for row in records if row["mangled"] == "public_name")
        self.assertEqual(public["file"], "module/unit.cpp")

    def test_extentless_pdb_function_alias_still_resolves_at_its_start(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.exe"
            _synthetic_pe(path)
            image = PEImage(path)
            resolver = AddressResolver(
                [], [{"rva": 0x1000, "size": 0, "mangled": "_func"}]
            )
            self.assertEqual(
                resolver.resolve_all(0x1000, image), frozenset({"F:M:_func"})
            )

    def test_function_interior_resolves_as_owner_plus_addend(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.exe"
            _synthetic_pe(path)
            image = PEImage(path)
            resolver = AddressResolver(
                [], [{"rva": 0x1000, "size": 0x40, "mangled": "_func"}]
            )
            self.assertEqual(
                resolver.resolve_all(0x1014, image),
                frozenset({"F:M:_func+0x14"}),
            )

    def test_function_identities_bridge_dynamic_initializer_spellings(self):
        target = _function_identity_tokens({
            "mangled": "`dynamic initializer for 'owner::value''",
            "name": "target display",
        })
        base = _function_identity_tokens({
            "mangled": "??__Evalue@owner@@YAXXZ",
            "name": "base display",
        })
        self.assertEqual(target & base, {"C:E:owner::value"})

    def test_interval_union_does_not_double_count_aliases(self):
        self.assertEqual(_interval_union([(10, 20), (10, 20), (15, 30), (40, 41)]), 21)

    def test_access_classification_distinguishes_reads_and_writes(self):
        self.assertEqual(_access_kind("mov\tdword ptr [0x1234], eax"), ("write", "dword"))
        self.assertEqual(_access_kind("mov\teax, dword ptr [0x1234]"), ("read", "dword"))
        self.assertEqual(_access_kind("lea\teax, [0x1234]"), ("address", "-"))
        self.assertEqual(
            _access_kind("mov\tdword ptr [0x1234], 0x5678", 0x5678),
            ("address", "-"),
        )

    def test_access_form_preserves_index_scale_evidence(self):
        self.assertEqual(_access_form("mov eax, dword ptr [ecx*8 + 0x1234]"),
                         ("indexed", 8))
        self.assertEqual(_access_form("lea eax, [ecx + 0x1234]"), ("lea", 0))

    def test_function_index_preserves_folded_aliases(self):
        records = [
            {"rva": 0x100, "size": 8, "mangled": "a"},
            {"rva": 0x100, "size": 8, "mangled": "b"},
            {"rva": 0x200, "size": 4, "mangled": "c"},
        ]
        self.assertEqual(
            {record["mangled"] for record in _FunctionIndex(records).containing(0x104)},
            {"a", "b"},
        )


class RenderRelocationTests(unittest.TestCase):
    @staticmethod
    def _access(site, function_rva, identity, function="duplicate"):
        return Access(
            site=site,
            instruction=site - 1,
            target=0x2000,
            access="address",
            width="-",
            form="imm",
            scale=0,
            identity=identity,
            instruction_text=f"push {site:#x}",
            function=function,
            unit="unit.cpp",
            function_rva=function_rva,
            function_size=0x20,
            partner_rva=None,
        )

    def test_interior_xref_uses_extentless_start_only_as_evidence(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.exe"
            _synthetic_pe(path)
            first = Datum(
                0x2000, None, ".rdata", "rdata", "E:first", "first",
                "public", "pdb",
            )
            second = Datum(
                0x2010, 4, ".rdata", "rdata", "E:second", "second",
                "u32", "pdb",
            )
            index = DatumIndex([first, second], PEImage(path))
            self.assertIsNone(index.owner(0x2004))
            self.assertEqual(index.evidence_owner(0x2004), first)
            self.assertEqual(index.evidence_owner(0x2010), second)
            self.assertFalse(index.evidence_owner(0x2004).complete)
            self.assertEqual(index.by_identity["E:first"], [first])

    def test_complete_window_normalizes_one_past_self_pointer(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.exe"
            _synthetic_pe(path)
            data = bytearray(path.read_bytes())
            struct.pack_into("<I", data, 0x404, 0x10000 + 0x3020)
            path.write_bytes(data)
            image = PEImage(path)
            index = DatumIndex([], image)
            _, _, offsets, targets = _window(image, 0x3000, 0x20, index)
            self.assertEqual(offsets, (4,))
            self.assertEqual(targets, (frozenset({"SELF+0x20"}),))

    def test_unindexed_adjustor_thunk_resolves_through_named_destination(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.exe"
            _synthetic_pe(path)
            data = bytearray(path.read_bytes())
            destination = 0x1080
            displacement = destination - (0x1000 + 8)
            data[0x200:0x208] = (
                b"\x83\xe9\x10\xe9" + struct.pack("<i", displacement)
            )
            path.write_bytes(data)
            index = DatumIndex(
                [], PEImage(path),
                {destination: frozenset(("P:?callee@@YAXXZ",))},
            )
            self.assertEqual(
                index.resolve_all(0x1000),
                frozenset(("THUNK:83e910e9:P:?callee@@YAXXZ",)),
            )

    def test_unindexed_delay_purecall_stub_uses_import_identity(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.exe"
            _synthetic_pe(path)
            data = bytearray(path.read_bytes())
            data[0x200:0x206] = b"\xff\x35" + struct.pack("<I", 0x13000)
            path.write_bytes(data)
            purecall = Datum(
                0x3000, 4, ".data", "data", "E:___pPurecall",
                "___pPurecall", "void*", "pdb",
            )
            index = DatumIndex([purecall], PEImage(path))
            self.assertEqual(
                index.resolve_all(0x1000),
                frozenset(("DELAY_PURECALL:E:___pPurecall+0x0",)),
            )

    def test_vftable_extent_excludes_the_next_tables_rtti_locator(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.exe"
            _synthetic_pe(path)
            table = Datum(
                0x3000, None, ".data", "data", "E:??_7table@@6B@", "table",
                "public", "pdb",
            )
            following = Datum(
                0x3008, None, ".data", "data", "E:??_7next@@6B@", "next",
                "public", "pdb",
            )
            index = DatumIndex([table, following], PEImage(path))
            self.assertEqual(_extentless_end(table, index), 0x3004)

    def test_vftable_extent_keeps_a_final_function_pointer(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.exe"
            _synthetic_pe(path)
            data = bytearray(path.read_bytes())
            struct.pack_into("<I", data, 0x304, 0x11010)
            path.write_bytes(data)
            table = Datum(
                0x2000, None, ".rdata", "rdata", "E:??_7table@@6B@",
                "table", "public", "pdb",
            )
            following = Datum(
                0x2008, None, ".rdata", "rdata", "E:??_7next@@6B@",
                "next", "public", "pdb",
            )
            index = DatumIndex([table, following], PEImage(path))
            index.relocations = (0x2004,)
            self.assertEqual(_extentless_end(table, index), 0x2008)

    def test_narrow_literal_extent_ends_at_terminator(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.exe"
            _synthetic_pe(path)
            data = bytearray(path.read_bytes())
            data[0x300:0x310] = b"literal\0padding!"
            path.write_bytes(data)
            literal = Datum(
                0x2000, None, ".rdata", "rdata", "E:??_C@_0H@test?$AA@",
                "??_C@_0H@test?$AA@", "public", "pdb",
            )
            following = Datum(
                0x2010, None, ".rdata", "rdata", "E:next", "next",
                "public", "pdb",
            )
            index = DatumIndex([literal, following], PEImage(path))
            self.assertEqual(_extentless_end(literal, index), 0x2008)

    def test_encoded_float_and_guard_have_semantic_extents(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.exe"
            _synthetic_pe(path)
            following = Datum(
                0x2020, None, ".rdata", "rdata", "E:next", "next",
                "public", "pdb",
            )
            image = PEImage(path)
            scalar = Datum(
                0x2000, None, ".rdata", "rdata", "E:__real@3f800000",
                "float", "public", "pdb",
            )
            guard = Datum(
                0x2010, None, ".rdata", "rdata", "E:??_B?guard", "guard",
                "public", "pdb",
            )
            index = DatumIndex([scalar, guard, following], image)
            self.assertEqual(_extentless_end(scalar, index), 0x2004)
            self.assertEqual(_extentless_end(guard, index), 0x2014)

    def test_same_shape_vftable_target_drift_is_owned_by_code_matching(self):
        table = Datum(
            0x3000, None, ".rdata", "rdata", "E:??_7type@@6B@", "table",
            "public", "pdb",
        )
        self.assertEqual(
            _classify_extentless_comparison(table, "EXACT", {
                "status": "RELOC_TARGETS",
                "normalized": "EXACT",
                "layout": "EXACT",
            }),
            "VTABLE_CODE_TARGETS",
        )

    def test_candidate_vote_uses_unique_exact_identity_majority(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.exe"
            _synthetic_pe(path)
            wanted = Datum(
                0x2000, None, ".rdata", "rdata", "E:wanted", "wanted",
                "public", "pdb",
            )
            other = Datum(
                0x2010, None, ".rdata", "rdata", "E:other", "other",
                "public", "pdb",
            )
            wanted_alias = Datum(
                0x2020, None, ".rdata", "rdata", "E:wanted", "wanted",
                "public", "pdb",
            )
            index = DatumIndex([wanted, other, wanted_alias], PEImage(path))
            self.assertEqual(
                _select_candidate_vote(
                    Counter({0x2000: 4, 0x2010: 1}), index, "E:wanted"
                ),
                0x2000,
            )
            self.assertEqual(_select_candidate_vote(
                Counter({0x2000: 1, 0x2010: 1}), index, "E:wanted"
            ), 0x2000)
            self.assertIsNone(_select_candidate_vote(
                Counter({0x2000: 1, 0x2020: 1}), index, "E:wanted"
            ))

    def test_unique_extentless_identity_overrides_shifted_xref_vote(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.exe"
            _synthetic_pe(path)
            wanted = Datum(
                0x2000, None, ".rdata", "rdata", "E:wanted", "wanted",
                "public", "pdb",
            )
            shifted = Datum(
                0x2010, None, ".rdata", "rdata", "E:shifted", "shifted",
                "public", "pdb",
            )
            index = DatumIndex([wanted, shifted], PEImage(path))
            self.assertEqual(
                _select_extentless_candidate(
                    Counter({0x2010: 4}), index, "E:wanted"
                ),
                (0x2000, True),
            )

    def test_complete_local_datum_token_uses_whole_normalized_bytes(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.exe"
            _synthetic_pe(path)
            image = PEImage(path)
            left = Datum(
                0x2000, 4, ".rdata", "rdata", "L:left", "left", "float", "pdb",
            )
            right = Datum(
                0x2010, 4, ".rdata", "rdata", "L:right", "right", "float", "pdb",
            )
            external = Datum(
                0x2020, 4, ".rdata", "rdata", "E:global", "global", "float", "pdb",
            )
            index = DatumIndex([left, right, external], image)
            self.assertEqual(
                _datum_token(index, 0x2000, "L:left"),
                _datum_token(index, 0x2010, "L:right"),
            )
            self.assertNotEqual(
                _datum_token(index, 0x2000, "L:left"),
                _datum_token(index, 0x2020, "E:global"),
            )

    def test_aliases_include_negative_four_indexed_table_base(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.exe"
            _synthetic_pe(path)
            preceding = Datum(
                0x1FF0, 0x14, ".rdata", "rdata", "E:preceding",
                "preceding", "table", "pdb",
            )
            table = Datum(
                0x2004, 24, ".rdata", "rdata", "L:table", "table",
                "u32[6]", "pdb",
            )
            index = DatumIndex([preceding, table], PEImage(path))
            self.assertEqual(
                {row.identity for row in index.alias_owners(0x2000)},
                {"E:preceding", "L:table"},
            )

    def test_paired_index_bias_recovers_shared_later_table_identity(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.exe"
            _synthetic_pe(path)
            image = PEImage(path)
            target = DatumIndex([
                Datum(
                    0x2000, 0x60, ".rdata", "rdata", "E:target_owner",
                    "target_owner", "public", "pdb",
                ),
                Datum(
                    0x2040, 0x10, ".rdata", "rdata", "E:table",
                    "table", "void*[4]", "pdb",
                ),
            ], image)
            base = DatumIndex([
                Datum(
                    0x2080, 0x60, ".rdata", "rdata", "E:base_owner",
                    "base_owner", "public", "pdb",
                ),
                Datum(
                    0x20C0, 0x10, ".rdata", "rdata", "E:table",
                    "table", "void*[4]", "pdb",
                ),
            ], image)
            audit = [{
                "pair_status": "PAIRED",
                "unit": "unit.cpp",
                "function": "?indexed@@YAXXZ",
                "target_function_rva": "0x1000",
                "base_function_rva": "0x1100",
                "target_target_rva": "0x2000",
                "base_target_rva": "0x2080",
                "target_identity": "D:E:target_owner+0x0",
                "base_identity": "D:E:base_owner+0x0",
                "target_access": "read",
                "base_access": "read",
                "target_scale": "4",
                "base_scale": "4",
                "identity_status": "DIFF",
                "datum_status": "BYTES",
            }]
            rows = _function_data_rows(audit, target, base, {})
            self.assertEqual(rows[0]["status"], "EXACT")
            self.assertEqual(rows[0]["resolution"], "EXACT")

    def test_folded_vtable_representatives_share_exact_bounded_prefix(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.exe"
            _synthetic_pe(path)
            data = bytearray(path.read_bytes())
            for raw in (0x300, 0x304, 0x340, 0x344):
                struct.pack_into("<I", data, raw, 0x11010)
            path.write_bytes(data)
            image = PEImage(path)
            identities = {0x1010: frozenset(("P:?callee@@YAXXZ",))}
            target = DatumIndex([
                Datum(
                    0x2000, 0x20, ".rdata", "rdata",
                    "E:??_7target@@6B@", "target", "public", "pdb",
                ),
            ], image, identities)
            base = DatumIndex([
                Datum(
                    0x2040, 8, ".rdata", "rdata",
                    "E:??_7base@@6B@", "base", "public", "pdb",
                ),
            ], image, identities)
            target.relocations = (0x2000, 0x2004)
            base.relocations = (0x2040, 0x2044)
            audit = [{
                "pair_status": "PAIRED",
                "unit": "unit.cpp",
                "function": "?folded@@YAXXZ",
                "target_function_rva": "0x1000",
                "base_function_rva": "0x1100",
                "target_target_rva": "0x2000",
                "base_target_rva": "0x2040",
                "target_identity": "D:E:??_7target@@6B@+0x0",
                "base_identity": "D:E:??_7base@@6B@+0x0",
                "target_access": "address",
                "base_access": "address",
                "target_scale": "0",
                "base_scale": "0",
                "identity_status": "DIFF",
                "datum_status": "BYTES",
            }]
            rows = _function_data_rows(audit, target, base, {})
            self.assertEqual(rows[0]["status"], "EXACT")
            self.assertEqual(rows[0]["resolution"], "EXACT")

    def test_datum_token_deduplicates_interior_references(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.exe"
            _synthetic_pe(path)
            datum = Datum(
                0x2000, 16, ".rdata", "rdata", "E:array", "array",
                "float[4]", "pdb",
            )
            index = DatumIndex([datum], PEImage(path))
            self.assertEqual(
                _datum_token(index, 0x2000, datum.identity),
                _datum_token(index, 0x200C, datum.identity),
            )

    def test_address_at_next_object_start_keeps_one_past_alias(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.exe"
            _synthetic_pe(path)
            previous = Datum(
                0x2000, 0x1C, ".rdata", "rdata", "L:ignored",
                "ignored", "enum[7]", "pdb",
            )
            following = Datum(
                0x201C, 0x24, ".rdata", "rdata", "L:captions",
                "captions", "char*[9]", "pdb",
            )
            index = DatumIndex([previous, following], PEImage(path))
            aliases = _datum_token_aliases(
                index, 0x201C, following.identity, one_past=True
            )
            self.assertIn(
                _datum_token(index, previous.rva, previous.identity), aliases
            )
            self.assertIn(
                _datum_token(index, following.rva, following.identity), aliases
            )

    def test_compiler_constant_tokens_ignore_storage_and_encoded_name(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.exe"
            _synthetic_pe(path)
            data = bytearray(path.read_bytes())
            for offset in (0x300, 0x310, 0x400):
                struct.pack_into("<f", data, offset, 1.0)
            path.write_bytes(data)
            encoded = Datum(
                0x2000, None, ".rdata", "rdata", "E:__real@3f800000",
                "__real@3f800000", "public", "pdb",
            )
            local_rdata = Datum(
                0x2010, 4, ".rdata", "rdata", "L:rdata", "rdata",
                "float", "pdb",
            )
            local_data = Datum(
                0x3000, 4, ".data", "data", "L:data", "data",
                "float", "pdb",
            )
            index = DatumIndex([encoded, local_rdata, local_data], PEImage(path))
            tokens = {
                _datum_token(index, 0x2000, encoded.identity),
                _datum_token(index, 0x2010, local_rdata.identity),
                _datum_token(index, 0x3000, local_data.identity),
            }
            self.assertEqual(len(tokens), 1)

    def test_asio_placeholder_hashes_share_their_four_zero_bytes(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.exe"
            _synthetic_pe(path)
            image = PEImage(path)
            target_datum = Datum(
                0x3040, None, ".data", "bss",
                "E:?error@?A0x11111111@placeholders@asio@boost@@"
                "3AAU?$arg@$00@4@A",
                "error", "public", "pdb",
            )
            base_datum = Datum(
                0x3060, None, ".data", "bss",
                "E:?error@?A0x22222222@placeholders@asio@boost@@"
                "3AAU?$arg@$00@4@A",
                "error", "public", "pdb",
            )
            target = DatumIndex([target_datum], image)
            base = DatumIndex([base_datum], image)
            self.assertEqual(_extentless_end(target_datum, target), 0x3044)
            self.assertEqual(
                _datum_token(target, 0x3040, target_datum.identity, base),
                _datum_token(base, 0x3060, base_datum.identity, target),
            )

    def test_asio_placeholder_pairs_local_and_public_pdb_spellings(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.exe"
            _synthetic_pe(path)
            image = PEImage(path)
            local = Datum(
                0x2040, 1, ".rdata", "rdata",
                "L:network:login_client_impl_connect.obj:_2",
                "_2", "cv:0x1234", "pdb",
            )
            public = Datum(
                0x3060, None, ".data", "bss",
                "E:?iterator@?A0x22222222@placeholders@asio@boost@@"
                "3AAU?$arg@$01@4@A",
                "iterator", "public", "pdb",
            )
            target = DatumIndex([local], image)
            base = DatumIndex([public], image)
            self.assertEqual(
                _datum_token(target, 0x2040, local.identity, base),
                _datum_token(base, 0x3060, public.identity, target),
            )

    def test_asio_placeholder_indices_remain_distinct(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.exe"
            _synthetic_pe(path)
            image = PEImage(path)
            first = Datum(
                0x2040, 1, ".rdata", "rdata",
                "L:network_core:async_connector.obj:_1",
                "_1", "cv:0x1234", "pdb",
            )
            second = Datum(
                0x2050, 1, ".rdata", "rdata",
                "L:network_core:async_connector.obj:_2",
                "_2", "cv:0x1234", "pdb",
            )
            index = DatumIndex([first, second], image)
            self.assertNotEqual(
                _datum_token(index, 0x2040, first.identity),
                _datum_token(index, 0x2050, second.identity),
            )

    def test_boost_stored_vtable_uses_bytes_and_paired_referents(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.exe"
            _synthetic_pe(path)
            data = bytearray(path.read_bytes())
            struct.pack_into("<I", data, 0x404, 0x11010)
            path.write_bytes(data)
            image = PEImage(path)
            target_datum = Datum(
                0x3000, 8, ".data", "data",
                "L:target.obj:stored_vtable", "stored_vtable",
                "boost::function::basic_vtable", "pdb",
            )
            base_datum = Datum(
                0x3000, 8, ".data", "data",
                "L:base.obj:stored_vtable", "stored_vtable",
                "boost::function::basic_vtable", "pdb",
            )
            identities = {0x1010: frozenset(("P:?callee@@YAXXZ",))}
            target = DatumIndex([target_datum], image, identities)
            base = DatumIndex([base_datum], image, identities)
            self.assertEqual(
                _datum_token(target, 0x3000, target_datum.identity, base),
                _datum_token(base, 0x3000, base_datum.identity, target),
            )

    def test_boost_stored_vtable_rejects_different_referents(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.exe"
            _synthetic_pe(path)
            data = bytearray(path.read_bytes())
            struct.pack_into("<I", data, 0x404, 0x11010)
            path.write_bytes(data)
            image = PEImage(path)
            target_datum = Datum(
                0x3000, 8, ".data", "data",
                "L:target.obj:stored_vtable", "stored_vtable",
                "boost::function::basic_vtable", "pdb",
            )
            base_datum = Datum(
                0x3000, 8, ".data", "data",
                "L:base.obj:stored_vtable", "stored_vtable",
                "boost::function::basic_vtable", "pdb",
            )
            target = DatumIndex(
                [target_datum], image,
                {0x1010: frozenset(("P:?target@@YAXXZ",))},
            )
            base = DatumIndex(
                [base_datum], image,
                {0x1010: frozenset(("P:?base@@YAXXZ",))},
            )
            self.assertNotEqual(
                _datum_token(target, 0x3000, target_datum.identity, base),
                _datum_token(base, 0x3000, base_datum.identity, target),
            )

    def test_public_boost_stored_vtable_has_eight_byte_extent(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.exe"
            _synthetic_pe(path)
            image = PEImage(path)
            datum = Datum(
                0x3000, None, ".data", "data",
                "E:?stored_vtable@?1???$assign_to@boost@@",
                "stored_vtable", "public", "pdb",
            )
            index = DatumIndex([datum], image)
            self.assertEqual(_extentless_end(datum, index), 0x3008)

    def test_external_zero_global_remains_identity_sensitive(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.exe"
            _synthetic_pe(path)
            image = PEImage(path)
            left = Datum(
                0x3040, None, ".data", "bss", "E:left", "left",
                "public", "pdb",
            )
            right = Datum(
                0x3060, None, ".data", "bss", "E:right", "right",
                "public", "pdb",
            )
            index = DatumIndex([left, right], image)
            self.assertNotEqual(
                _datum_token(index, 0x3040, left.identity),
                _datum_token(index, 0x3060, right.identity),
            )

    def test_shared_local_identity_wins_over_content_canonicalization(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.exe"
            _synthetic_pe(path)
            image = PEImage(path)
            target_datum = Datum(
                0x2000, None, ".rdata", "rdata", "L:shared", "shared",
                "public", "pdb",
            )
            base_datum = Datum(
                0x2010, 4, ".rdata", "rdata", "L:shared", "shared",
                "float", "pdb",
            )
            target = DatumIndex([target_datum], image)
            base = DatumIndex([base_datum], image)
            self.assertEqual(
                _datum_token(target, 0x2000, target_datum.identity, base),
                _datum_token(base, 0x2010, base_datum.identity, target),
            )

    def test_unique_site_pairing_requires_a_bijection(self):
        self.assertEqual(
            _accept_unique({1: {10}, 2: {20}}, set(), set()),
            {1: 10, 2: 20},
        )
        self.assertEqual(_accept_unique({1: {10}, 2: {10}}, set(), set()), {})
        self.assertEqual(_accept_unique({1: {10, 20}}, set(), set()), {})
        self.assertEqual(_accept_unique({1: {10}}, {1}, set()), {})
        self.assertEqual(_accept_unique({1: {10}}, set(), {10}), {})

    def test_duplicate_functions_pair_by_unique_complete_relocation_fingerprint(self):
        target = {
            0x1104: Site(0x1104, (self._access(0x1104, 0x1100, "E:first"),)),
            0x1204: Site(0x1204, (self._access(0x1204, 0x1200, "E:second"),)),
        }
        base = {
            0x2104: Site(0x2104, (self._access(0x2104, 0x2100, "E:second"),)),
            0x2204: Site(0x2204, (self._access(0x2204, 0x2200, "E:first"),)),
        }
        self.assertEqual(
            _function_fingerprint_pairs(target, base),
            ({0x1100: 0x2200, 0x1200: 0x2100}, {0x2200: 0x1100, 0x2100: 0x1200}),
        )

    def test_ambiguous_duplicate_fingerprint_is_not_paired(self):
        target = {
            0x1104: Site(0x1104, (self._access(0x1104, 0x1100, "E:same"),)),
            0x1204: Site(0x1204, (self._access(0x1204, 0x1200, "E:same"),)),
        }
        base = {
            0x2104: Site(0x2104, (self._access(0x2104, 0x2100, "E:same"),)),
        }
        self.assertEqual(_function_fingerprint_pairs(target, base), ({}, {}))

    def test_function_pointer_targets_match_on_any_shared_symbol(self):
        self.assertTrue(_relocation_targets_match(
            (0,), (frozenset({"P:a", "M:folded"}),),
            (0,), (frozenset({"P:a", "M:other"}),),
        ))
        self.assertFalse(_relocation_targets_match(
            (0,), (frozenset({"P:a"}),),
            (0,), (frozenset({"P:b"}),),
        ))
        self.assertFalse(_relocation_targets_match(
            (0,), (frozenset({"P:a"}),),
            (4,), (frozenset({"P:a"}),),
        ))
        self.assertEqual(
            _relocation_target_differences(
                (0, 4), (frozenset({"P:a"}), frozenset({"P:b", "M:alias"})),
                (0, 4), (frozenset({"P:a"}), frozenset({"P:c", "M:alias"})),
            ),
            [],
        )
        self.assertEqual(
            _relocation_target_differences(
                (0,), (frozenset({"P:a"}),),
                (4,), (frozenset({"P:a"}),),
            ),
            [
                (0, frozenset({"P:a"}), frozenset()),
                (4, frozenset(), frozenset({"P:a"})),
            ],
        )

    def test_function_identity_keeps_mangled_and_demangled_aliases(self):
        self.assertEqual(
            _record_function_identities({
                "mangled": "??_Grepresentative@@UAEPAXI@Z",
                "name": "alias::~alias(unsigned int)",
            }),
            frozenset({
                "M:??_Grepresentative@@UAEPAXI@Z",
                "D:alias::~alias(unsigned int)",
            }),
        )

    def test_extentless_pattern_hints_preserve_observed_shape(self):
        self.assertEqual(_pattern(bytes(12)), "zero[12]")
        self.assertEqual(_pattern(b"ABABABAB"), "repeat[2]x4")
        self.assertEqual(_pattern(b"hello\0world\0"), "ascii:hello")

    def test_first_byte_difference_is_reported(self):
        self.assertEqual(_first_diff(b"abc", b"axc"), "0x1:62!=78")
        self.assertEqual(_first_diff(b"abc", b"abc"), "-")
        self.assertEqual(_first_diff(b"abc", b"abcd"), "0x3:length")

    def test_base_only_site_is_not_misclassified_as_unowned_retail(self):
        row = {
            "pair_status": "BASE_ONLY",
            "access_status": "-",
            "identity_status": "-",
            "extent_status": "UNOWNED",
            "datum_status": "NO_TARGET_OWNER",
        }
        self.assertEqual(_problem_tags(row), ["BASE_ONLY"])


class ConsumerManifestTests(unittest.TestCase):
    @staticmethod
    def _symbol(scope="local", identity="L:render:file.obj:s_value"):
        return DataSymbol(
            rva=0x2000, section=".data", storage="data", size=4,
            size_kind="pdb-type", type_index=0x1000, scope=scope,
            module_path=b"file.obj", archive_path=b"render.lib", name=b"s_value",
            public_name=b"?g_value@@3HA" if scope == "external" else b"",
            identity=identity, owner_module="render",
        )

    def test_local_projection_name_is_stable_and_promoted(self):
        first = _canonical_name(self._symbol())
        second = _canonical_name(self._symbol())
        self.assertEqual(first, second)
        self.assertRegex(first, r"^__vostok_data_[0-9a-f]{24}$")

    def test_external_projection_preserves_pdb_identity(self):
        self.assertEqual(_canonical_name(self._symbol("external", "E:?g_value@@3HA")),
                         "?g_value@@3HA")

    def test_projection_sections_are_one_byte_aligned_comdats(self):
        for storage in ("rdata", "data", "bss"):
            name, characteristics = _section_properties(storage)
            self.assertTrue(name.startswith("."))
            self.assertNotEqual(characteristics & 0x1000, 0)
            self.assertNotEqual(characteristics & 0x100000, 0)

    def test_projection_ignores_references_without_a_comparison_unit(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.exe"
            _synthetic_pe(path)
            image = PEImage(path)
            symbol = dataclasses.replace(
                self._symbol(), section=".rdata", storage="rdata"
            )
            accesses = [
                {
                    "caller_file": unit,
                    "target_rva": "0x2000",
                    "target_identity": symbol.identity,
                    "width": "dword",
                    "form": "direct",
                    "scale": "0",
                }
                for unit in ("", "-", "render/unit.cpp")
            ]
            with mock.patch(
                "vostok.data.manifest._access_rows", return_value=accesses
            ):
                consumers, blockers = _direct_consumers(
                    [symbol], image, {symbol.rva: symbol}
                )
        self.assertEqual(consumers, {"render/unit.cpp": {symbol.rva}})
        self.assertEqual(blockers, [])

    def test_function_referent_classification_matches_gruntz_categories(self):
        def ref(offset, identity):
            return (offset, "read", "dword", identity)

        self.assertEqual(
            _classify_function_refs([ref(0, "a"), ref(4, "b")],
                                    [ref(0, "b"), ref(4, "a")])[0],
            "ORDERING_ONLY",
        )
        self.assertEqual(
            _classify_function_refs([ref(0, "a"), ref(4, "a")], [ref(0, "a")])[0],
            "MULTIPLICITY_ONLY",
        )
        self.assertEqual(
            _classify_function_refs([ref(0, "a")], [ref(0, "b")])[0],
            "WRONG_REFERENT",
        )


if __name__ == "__main__":
    unittest.main()
