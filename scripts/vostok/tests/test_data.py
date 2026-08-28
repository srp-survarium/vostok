import struct
import tempfile
import unittest
from pathlib import Path

from vostok.data.inventory import identity_for
from vostok.data.pe import PEImage
from vostok.data.pipeline import _FunctionIndex, _access_kind, _interval_union


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


if __name__ == "__main__":
    unittest.main()
