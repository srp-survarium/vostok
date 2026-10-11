# SPDX-License-Identifier: GPL-3.0-or-later
"""Native x87 memory inputs update ST; memory stores update the addressed datum."""

import unittest

from vostok.data.pipeline import _access_kind


class X87AccessRoleTests(unittest.TestCase):
    # Physical opcode fixtures, independently decoded with flake llvm-mc.
    instructions = (
        ("d80578563412", "fadd dword ptr [0x12345678]", "read"),
        ("d80d78563412", "fmul dword ptr [0x12345678]", "read"),
        ("d83d78563412", "fdivr dword ptr [0x12345678]", "read"),
        ("d91578563412", "fst dword ptr [0x12345678]", "write"),
        ("d91d78563412", "fstp dword ptr [0x12345678]", "write"),
    )

    def test_physical_x87_read_and_store_fixtures(self):
        for opcode, instruction, role in self.instructions:
            with self.subTest(opcode=opcode):
                self.assertEqual(_access_kind(instruction, 0x12345678), (role, "dword"))

    def test_two_operand_pdb_and_native_tab_syntax_agree(self):
        for mnemonic in ("fmul", "fadd", "fdivr"):
            with self.subTest(mnemonic=mnemonic):
                self.assertEqual(_access_kind(f"{mnemonic}\tdword ptr [0x12345678]", 0x12345678),
                                 _access_kind(f"{mnemonic} st, dword ptr [0x12345678]", 0x12345678))

    def test_integer_x87_inputs_and_state_loads_are_reads(self):
        for mnemonic in ("fiadd", "fisub", "fisubr", "fimul", "fidiv", "fidivr",
                         "ficom", "ficomp", "fild", "fldcw", "fldenv", "frstor"):
            with self.subTest(mnemonic=mnemonic):
                self.assertEqual(_access_kind(f"{mnemonic} word ptr [0x12345678]", 0x12345678)[0],
                                 "read")

    def test_x87_stores_and_normal_rmw_keep_their_roles(self):
        for mnemonic in ("fst", "fstp", "fist", "fistp", "fisttp", "fbstp", "fnstcw", "fnstenv"):
            with self.subTest(mnemonic=mnemonic):
                self.assertEqual(_access_kind(f"{mnemonic} dword ptr [0x12345678]", 0x12345678)[0],
                                 "write")
        self.assertEqual(_access_kind("add dword ptr [0x12345678], 1", 0x12345678)[0], "readwrite")
        self.assertEqual(_access_kind("lea eax, [0x12345678]", 0x12345678)[0], "address")


if __name__ == "__main__":
    unittest.main()
