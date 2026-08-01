import importlib.util
import unittest
from pathlib import Path


SPEC = importlib.util.spec_from_file_location(
    "vostok_sema", Path(__file__).with_name("sema.py")
)
SEMA = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(SEMA)


class SemaNavigationTests(unittest.TestCase):
    def test_decodes_msvc_literal_prefix(self):
        self.assertEqual(
            SEMA._decode_literal("??_C@_0N@BHGPFDAK@resources?1?$CFs?$AA@"),
            "resources/%s",
        )
        self.assertEqual(
            SEMA._decode_literal("??_C@_07INEOAPOI@render?3?$AA@"),
            "render:",
        )

    def test_qualified_name_drops_return_and_arguments(self):
        self.assertEqual(
            SEMA._qualified_name("void vostok::render::device::on_device_removed()"),
            "vostok::render::device::on_device_removed",
        )

    def test_call_operand_rejects_non_calls(self):
        self.assertEqual(SEMA._call_operand("call  vostok::logging::append"),
                         "vostok::logging::append")
        self.assertIsNone(SEMA._call_operand("jmp short .1"))


if __name__ == "__main__":
    unittest.main()
