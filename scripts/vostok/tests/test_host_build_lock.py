# SPDX-License-Identifier: GPL-3.0-or-later
"""One ninja compile/link at a time across every worktree on the host."""

import contextlib
import fcntl
import io
import os
import tempfile
import threading
import time
import unittest
from pathlib import Path
from unittest import mock

from vostok.build import ninja as build_ninja


class HostBuildLockTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.lock = Path(self.tmp.name) / "locks" / "vostok-ninja.lock"

    def tearDown(self):
        self.tmp.cleanup()

    def test_a_waiter_prints_the_holder_once_and_blocks_until_release(self):
        entered, output = threading.Event(), io.StringIO()
        with build_ninja.host_build_lock(self.lock):
            self.assertIn(f"pid {os.getpid()}", self.lock.read_text())

            def second():
                with contextlib.redirect_stdout(output):
                    with build_ninja.host_build_lock(self.lock):
                        entered.set()

            waiter = threading.Thread(target=second)
            waiter.start()
            time.sleep(0.3)
            self.assertFalse(entered.is_set(), "the second build must wait")
        waiter.join(5)
        self.assertTrue(entered.is_set())
        lines = output.getvalue().splitlines()
        self.assertEqual(len(lines), 1)
        self.assertIn("waiting for the build lock held by", lines[0])
        self.assertIn(f"pid {os.getpid()}", lines[0])
        self.assertEqual(self.lock.read_text(), "", "the holder note is cleared on release")

    def test_lock_is_released_when_the_body_fails(self):
        with self.assertRaises(RuntimeError):
            with build_ninja.host_build_lock(self.lock):
                raise RuntimeError("compile failed")
        with self.lock.open("a") as handle:
            fcntl.flock(handle, fcntl.LOCK_EX | fcntl.LOCK_NB)

    def test_no_lock_never_touches_the_lock(self):
        with build_ninja.host_build_lock(self.lock, enabled=False):
            self.assertFalse(self.lock.exists())

    def test_main_strips_no_lock_and_skips_the_lock_for_tools(self):
        seen = []

        @contextlib.contextmanager
        def recording(path=None, *, enabled=True):
            seen.append(enabled)
            yield

        ninja_dir = Path(self.tmp.name) / "ninja"
        ninja_dir.mkdir()
        (ninja_dir / "ninja.exe").write_bytes(b"")
        build_dir = Path(self.tmp.name) / "build"
        build_dir.mkdir()
        (build_dir / "build.ninja").write_text("")
        for argv, expected in ((["ninja", "--no-lock", "game"], False),
                               (["ninja", "game"], True),
                               (["ninja", "-t", "clean"], False)):
            with mock.patch.object(build_ninja, "host_build_lock", recording), \
                 mock.patch.object(build_ninja, "_build", return_value=0) as build, \
                 mock.patch.object(build_ninja, "BUILD_DIR", build_dir), \
                 mock.patch.dict(os.environ, {"NINJA_DIR": str(ninja_dir)}), \
                 mock.patch.object(build_ninja.sys, "argv", argv):
                with self.assertRaises(SystemExit):
                    build_ninja.main()
                self.assertNotIn("--no-lock", build.call_args.args[1])
            self.assertEqual(seen[-1], expected, argv)


if __name__ == "__main__":
    unittest.main()
