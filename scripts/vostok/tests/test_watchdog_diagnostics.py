# SPDX-License-Identifier: GPL-3.0-or-later
"""Timeout evidence survives cleanup without changing the timeout verdict."""

import io
import unittest
from contextlib import redirect_stdout
from pathlib import Path
from unittest import mock

from vostok.build import ninja


class TimeoutDiagnosticTests(unittest.TestCase):
    def test_missing_outputs_and_exited_worker_still_report_observation(self):
        output = io.StringIO()
        with mock.patch.object(ninja, "LINK_OUTPUTS", [Path("/absent/base.exe")]), \
             mock.patch.object(Path, "read_text", side_effect=FileNotFoundError), \
             redirect_stdout(output):
            ninja._report_watchdog_timeout(
                2401, workers={123}, outputs_refreshed=False,
                recent_cores=0.75, worker_idle_seconds=0,
            )
        text = output.getvalue()
        self.assertIn("outputs_refreshed=False recent_cpu_cores=0.75", text)
        self.assertIn("pid=123 exited/unreadable", text)
        self.assertIn("base.exe missing/unreadable", text)

    def test_full_timeout_reports_before_killing_and_remains_failure(self):
        proc = mock.Mock(pid=123)
        proc.poll.return_value = None
        actions = []
        with (mock.patch.object(ninja, "_assert_no_existing_build"),
              mock.patch.object(ninja, "_prefix_process_ids", side_effect=(set(), {42})),
              mock.patch.object(ninja.subprocess, "Popen", return_value=proc),
              mock.patch.object(ninja, "_wine_tree_jiffies", side_effect=(100, 175)),
              mock.patch.object(ninja, "_outputs_refreshed", return_value=False),
              mock.patch.object(ninja.time, "sleep"),
              mock.patch.object(ninja.time, "time", side_effect=(100, 100, 106)),
              mock.patch.object(ninja, "HARD_TIMEOUT_SECONDS", 5),
              mock.patch.object(ninja, "_report_watchdog_timeout",
                                side_effect=lambda *a, **k: actions.append("report")) as report,
              mock.patch.object(ninja, "_stop_interrupted_build",
                                side_effect=lambda *a: actions.append("kill"))):
            self.assertEqual(ninja._run_with_watchdog(Path("ninja.exe"), ["game"]), 1)
        self.assertEqual(actions, ["report", "kill"])
        self.assertEqual(report.call_args.kwargs["workers"], {42})
        self.assertFalse(report.call_args.kwargs["outputs_refreshed"])


if __name__ == "__main__":
    unittest.main()
