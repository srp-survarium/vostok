import struct
import tempfile
import unittest
from pathlib import Path

from vostok.data.inventory import DataSymbol
from vostok.data.inventory import identity_for
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
    _section_properties,
)
from vostok.data.pe import PEImage
from vostok.data.pipeline import (
    _FunctionIndex,
    _access_form,
    _access_kind,
    _interval_union,
)


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
    def test_interval_union_does_not_double_count_aliases(self):
        self.assertEqual(_interval_union([(10, 20), (10, 20), (15, 30), (40, 41)]), 21)

    def test_access_classification_distinguishes_reads_and_writes(self):
        self.assertEqual(_access_kind("mov\tdword ptr [0x1234], eax"), ("write", "dword"))
        self.assertEqual(_access_kind("mov\teax, dword ptr [0x1234]"), ("read", "dword"))
        self.assertEqual(_access_kind("lea\teax, [0x1234]"), ("address", "-"))
        self.assertEqual(
            _access_kind("mov\tdword ptr [0x1234], 0x5678", 0x5678),
            ("address", "dword"),
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
