# SPDX-License-Identifier: GPL-3.0-or-later

import tempfile
import unittest
from pathlib import Path

from vostok.tool.pdb_order_probe import (
    CASES,
    OUTPUT_MARKER,
    ProbeRunner,
    SCOPED_CHANNELS,
    SEQUENCE_CHANNELS,
    main,
    summarize_topology,
)


class PdbOrderProbeTests(unittest.TestCase):
    def test_summary_keeps_each_causal_channel_separate(self):
        sequence = {
            "base_total": 3,
            "target_total": 3,
            "shared_unique": 3,
            "order_metrics": {"inversions": 1},
            "moved": [{}, {}],
            "changed": [{}],
            "only_base": [],
            "only_target": [{}],
        }
        report = {name: sequence for name in SEQUENCE_CHANNELS}
        report.update(
            {
                name: {
                    "paired": 2,
                    "different": 1,
                    "only_base_scopes": [],
                    "only_target_scopes": ["one"],
                    "streams": [{"comparison": sequence}],
                }
                for name in SCOPED_CHANNELS
            }
        )
        report["msf_layout"] = {
            "base": {"file_bytes": 100},
            "target": {"file_bytes": 100},
            "stable_roles": [
                {"base": {"pages": [1]}, "target": {"pages": [2]}},
                {"base": {"pages": [3]}, "target": {"pages": [3]}},
            ],
            "unidentified_base": [],
            "unidentified_target": [{}],
        }

        summary = summarize_topology(report)

        self.assertEqual(summary["channels"]["modules"]["inversions"], 1)
        self.assertEqual(summary["channels"]["modules"]["moved"], 2)
        self.assertEqual(
            summary["channels"]["module_file_streams"]["different"], 1
        )
        self.assertEqual(summary["msf_layout"]["changed_stable_roles"], 1)
        self.assertEqual(summary["msf_layout"]["unidentified_target"], 1)

    def test_list_does_not_require_the_toolchain(self):
        self.assertEqual(main(["--list"]), 0)
        self.assertIn("function-order", CASES)

    def test_custom_output_requires_probe_marker_before_replacement(self):
        with tempfile.TemporaryDirectory() as temporary:
            output = Path(temporary) / "existing"
            output.mkdir()
            keep = output / "unrelated.txt"
            keep.write_text("keep\n", encoding="utf-8")
            runner = ProbeRunner.__new__(ProbeRunner)
            runner.output = output
            runner.work = output / "work"
            runner.artifacts = output / "artifacts"
            runner.reports = output / "comparisons"
            runner.logs = output / "logs"
            runner.output_marker = output / OUTPUT_MARKER

            with self.assertRaises(SystemExit):
                runner.prepare()
            self.assertTrue(keep.is_file())

            runner.output_marker.write_text("generated\n", encoding="utf-8")
            runner.prepare()
            self.assertFalse(keep.exists())
            self.assertTrue(runner.output_marker.is_file())


if __name__ == "__main__":
    unittest.main()
