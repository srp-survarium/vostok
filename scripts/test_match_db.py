import importlib.util
import unittest
from pathlib import Path


SPEC = importlib.util.spec_from_file_location(
    "vostok_match_db", Path(__file__).with_name("match_db.py")
)
MATCH_DB = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(MATCH_DB)


class InstructionStreamExactTests(unittest.TestCase):
    def record(self, size=6, text="call  Scaleform::Render::HAL::EndFrame"):
        return {
            "size": size,
            "instructions": [{"off": 0, "len": size, "text": text}],
        }

    def test_accepts_identical_symbolic_instruction_stream(self):
        self.assertTrue(
            MATCH_DB.instruction_stream_exact(self.record(), self.record())
        )

    def test_rejects_different_instruction_or_size(self):
        self.assertFalse(
            MATCH_DB.instruction_stream_exact(
                self.record(), self.record(text="call  Scaleform::Render::HAL::BeginFrame")
            )
        )
        self.assertFalse(
            MATCH_DB.instruction_stream_exact(self.record(), self.record(size=5))
        )

    def test_rejects_missing_instruction_evidence(self):
        empty = {"size": 0, "instructions": []}
        self.assertFalse(MATCH_DB.instruction_stream_exact(empty, empty))


if __name__ == "__main__":
    unittest.main()
