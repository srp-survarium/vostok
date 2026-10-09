# SPDX-License-Identifier: GPL-3.0-or-later
"""Setup shares the build lock and never mutates a measured worktree."""

import fcntl
import tempfile
import unittest
from pathlib import Path
from unittest import mock

from vostok.tool import toolchain


class SetupLockTests(unittest.TestCase):
    def test_busy_build_refuses_setup_before_any_stage(self):
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / ".build.lock"
            with path.open("a") as build:
                fcntl.flock(build, fcntl.LOCK_EX | fcntl.LOCK_NB)
                with mock.patch.object(toolchain.paths, "BUILD_LOCK", path), \
                     mock.patch.object(toolchain, "setup") as setup, \
                     mock.patch.object(toolchain.sys, "argv", ["toolchain"]):
                    with self.assertRaises(SystemExit):
                        toolchain.main()
                    setup.assert_not_called()

    def test_setup_owns_lock_and_releases_it_even_on_failure(self):
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "binaries" / ".build.lock"

            def stage(force):
                self.assertEqual(force, {"ninja"})
                with path.open("a") as competitor:
                    with self.assertRaises(BlockingIOError):
                        fcntl.flock(competitor, fcntl.LOCK_EX | fcntl.LOCK_NB)
                raise RuntimeError("failed stage")

            with mock.patch.object(toolchain.paths, "BUILD_LOCK", path), \
                 mock.patch.object(toolchain, "setup", side_effect=stage), \
                 mock.patch.object(toolchain.sys, "argv", ["toolchain", "--force", "ninja"]):
                with self.assertRaisesRegex(RuntimeError, "failed stage"):
                    toolchain.main()
            with path.open("a") as next_setup:
                fcntl.flock(next_setup, fcntl.LOCK_EX | fcntl.LOCK_NB)


if __name__ == "__main__":
    unittest.main()
