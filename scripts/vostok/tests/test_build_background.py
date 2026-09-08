# SPDX-License-Identifier: GPL-3.0-or-later

import contextlib
import io
import os
from pathlib import Path
import subprocess
import tempfile
import unittest
from unittest import mock

from vostok.build import background, rebuild


class BackgroundBuildTests(unittest.TestCase):
    def _supervise(self, build_result=0, queue_result=0):
        def run(command, **kwargs):
            outcome = queue_result if command[0] == "codex" else build_result
            if isinstance(outcome, Exception):
                raise outcome
            return subprocess.CompletedProcess(command, outcome)

        with tempfile.TemporaryDirectory() as tmp:
            logfile = Path(tmp) / "job.log"
            with mock.patch.object(background.subprocess, "run", side_effect=run) as called:
                with contextlib.redirect_stdout(io.StringIO()):
                    rc = background.supervise("thread-123", "codex", "nix", logfile,
                                              ["logging", "-j2"])
            return rc, logfile.read_text(), called.call_args_list

    def test_success_notifies_after_whole_build(self):
        rc, log, calls = self._supervise()
        self.assertEqual(rc, 0)
        self.assertEqual(len(calls), 2)
        command = calls[0].args[0]
        self.assertEqual(command[:4], ["nix", "develop", str(background.paths.REPO), "--command"])
        self.assertEqual(command[4:], ["python3", "-u", "-m", "vostok", "build", "logging", "-j2"])
        self.assertEqual(calls[1].args[0][:5], ["codex", "queue", "--thread", "thread-123", "--message"])
        self.assertIn("SUCCEEDED: exit 0", log)
        self.assertIn(str(background.paths.REPO), calls[1].args[0][-1])

    def test_build_failure_and_signal_preserve_status(self):
        for status, expected in ((2, 2), (-15, 143)):
            with self.subTest(status=status):
                rc, log, calls = self._supervise(status)
                self.assertEqual(rc, expected)
                self.assertIn(f"FAILED: exit {expected}", log)
                self.assertEqual(len(calls), 2)

    def test_setup_start_failure_notifies(self):
        rc, log, calls = self._supervise(FileNotFoundError("missing nix"))
        self.assertEqual(rc, 127)
        self.assertIn("Build could not start", log)
        self.assertEqual(len(calls), 2)

    def test_delivery_failure_does_not_replace_build_status_or_retry(self):
        for build_rc in (0, 2):
            for failure in (1, FileNotFoundError("codex"),
                            subprocess.TimeoutExpired("codex", 30)):
                with self.subTest(build_rc=build_rc, failure=failure):
                    rc, log, calls = self._supervise(build_rc, failure)
                    self.assertEqual(rc, build_rc)
                    self.assertIn("WARNING: completion delivery failed", log)
                    self.assertEqual(len(calls), 2)

    def test_launcher_requires_destination_before_starting_service(self):
        with mock.patch.dict(os.environ, {}, clear=True), \
                mock.patch.object(background.subprocess, "run") as run, \
                contextlib.redirect_stderr(io.StringIO()):
            with self.assertRaises(SystemExit):
                background.launch(["--background"])
        run.assert_not_called()

    def test_launcher_pins_worktree_and_keeps_build_arguments(self):
        with tempfile.TemporaryDirectory() as tmp, \
                mock.patch.dict(os.environ, {"CODEX_THREAD_ID": "thread-123"}, clear=True), \
                mock.patch.object(background, "_executable", side_effect=lambda s: "/bin/" + s), \
                mock.patch.object(background, "_codex", return_value="/bin/codex"), \
                mock.patch.object(background.paths, "BUILD_JOBS", Path(tmp)), \
                mock.patch.object(background.subprocess, "run") as run, \
                contextlib.redirect_stdout(io.StringIO()):
            run.return_value = subprocess.CompletedProcess([], 0, "--thread --message")
            background.launch(["--background", "logging", "-j2"])
            command = run.call_args.args[0]
        self.assertEqual(command[0], "/bin/systemd-run")
        self.assertIn(f"--working-directory={background.paths.REPO}", command)
        self.assertIn(f"--setenv=PYTHONPATH={background.paths.SCRIPTS}", command)
        self.assertEqual(command[-3:], ["--", "logging", "-j2"])
        self.assertNotIn("--background", command)

    def test_launcher_rejects_unsupported_cli_without_service(self):
        with mock.patch.dict(os.environ, {"CODEX_THREAD_ID": "thread-123"}), \
                mock.patch.object(background, "_executable", side_effect=lambda s: s), \
                mock.patch.object(background, "_codex", return_value="codex"), \
                mock.patch.object(background.subprocess, "run") as run, \
                contextlib.redirect_stderr(io.StringIO()):
            run.return_value = subprocess.CompletedProcess([], 2, "")
            with self.assertRaises(SystemExit):
                background.launch(["--background"])
            self.assertEqual(run.call_count, 1)

    def test_stale_executable_environment_falls_back(self):
        with mock.patch.dict(os.environ, {"CODEX_EXECUTABLE_PATH": "/missing/codex"}), \
                mock.patch.object(background.shutil, "which", side_effect=lambda s:
                                  "/bin/codex" if s == "codex" else None):
            self.assertEqual(background._codex(None), "/bin/codex")
            with self.assertRaises(ValueError):
                background._codex("/missing/explicit-codex")

    def test_help_does_not_launch_or_build(self):
        with mock.patch.object(rebuild.sys, "argv", ["build", "--background", "--help"]), \
                mock.patch.object(background, "launch") as launch, \
                mock.patch.object(rebuild, "_acquire_build_lock") as lock, \
                contextlib.redirect_stdout(io.StringIO()):
            rebuild.main()
        launch.assert_not_called()
        lock.assert_not_called()

    def test_background_dispatch_bypasses_foreground_build(self):
        with mock.patch.object(rebuild.sys, "argv", ["build", "--background"]), \
                mock.patch.object(background, "launch") as launch, \
                mock.patch.object(rebuild, "_acquire_build_lock") as lock:
            rebuild.main()
        launch.assert_called_once_with(["--background"])
        lock.assert_not_called()


if __name__ == "__main__":
    unittest.main()
