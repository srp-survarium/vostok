# SPDX-License-Identifier: GPL-3.0-or-later
"""Same printed names must not silently imply closure of several TU bodies."""

import unittest
from unittest import mock

from vostok.derive import artifacts
from vostok.derive.index import index_by_mangled, same_name_distinct_bodies


class DuplicateBodyIdentityTests(unittest.TestCase):
    def record(self, file, rva, name="static helper()"):
        return {"mangled": "helper", "name": name, "file": file, "rva": rva}

    def test_warns_selected_and_hidden_body_without_changing_ledger_identity(self):
        core = self.record("core.cpp", 0x1000)
        stage = self.record("stage.cpp", 0x2000)
        indexed = index_by_mangled([stage, core])
        with mock.patch.object(artifacts, "log") as log:
            artifacts._warn_hidden_bodies([stage, core], indexed)
        self.assertEqual(indexed, {"helper": core})
        message = log.call_args.args[0]
        self.assertIn("core.cpp@0x1000", message)
        self.assertIn("stage.cpp@0x2000", message)
        self.assertIn("score does not close sibling bodies", message)
        self.assertIn("--file/--rva", message)

    def test_does_not_confuse_icf_aliases_with_distinct_bodies(self):
        records = [self.record("first.cpp", 0x1000), self.record("second.cpp", 0x1000)]
        self.assertEqual(same_name_distinct_bodies(records), {})

    def test_does_not_warn_preserved_overloads_or_one_tu_compiler_clones(self):
        records = [self.record("first.cpp", 0x1000, "helper(int)"),
                   self.record("second.cpp", 0x2000, "helper(float)")]
        self.assertEqual(same_name_distinct_bodies(records), {})
        records = [self.record("first.cpp", 0x1000), self.record("first.cpp", 0x2000)]
        self.assertEqual(same_name_distinct_bodies(records), {})


if __name__ == "__main__":
    unittest.main()
