# SPDX-License-Identifier: GPL-3.0-or-later
"""Library staging preserves unchanged inputs and publishes replacements safely."""

import io
import os
import stat
import tempfile
import unittest
from contextlib import redirect_stdout
from pathlib import Path
from unittest import mock

from vostok.tool import libs


class PrebuiltStagingTests(unittest.TestCase):
    def test_repeated_staging_preserves_inode_mtime_mode_and_reports_zero_copies(self):
        with tempfile.TemporaryDirectory() as tmp:
            source, destination = Path(tmp) / "source", Path(tmp) / "dest"
            source.mkdir()
            (source / "probe.lib").write_bytes(b"library")
            arguments = ["libs", str(source), str(destination)]
            with mock.patch.object(libs.os.sys, "argv", arguments), redirect_stdout(io.StringIO()):
                libs.main()
            target = destination / "probe.lib"
            os.utime(target, ns=(1000000000, 1000000000))
            before = target.stat()
            output = io.StringIO()
            with mock.patch.object(libs.os.sys, "argv", arguments), redirect_stdout(output):
                libs.main()
            after = target.stat()
            self.assertEqual((before.st_ino, before.st_mtime_ns, before.st_mode),
                             (after.st_ino, after.st_mtime_ns, after.st_mode))
            self.assertIn("Copied 0 files", output.getvalue())

    def test_equal_size_equal_mtime_changed_contents_are_replaced(self):
        with tempfile.TemporaryDirectory() as tmp:
            source, target = Path(tmp) / "source.lib", Path(tmp) / "target.lib"
            source.write_bytes(b"a" * (1024 * 1024) + b"changed")
            target.write_bytes(b"a" * (1024 * 1024) + b"oldbyte")
            os.utime(source, ns=(1000000000, 1000000000))
            os.utime(target, ns=(1000000000, 1000000000))
            self.assertTrue(libs.stage_file(source, target))
            self.assertEqual(target.read_bytes(), source.read_bytes())

    def test_identical_readonly_file_is_retained_changed_file_becomes_writable(self):
        with tempfile.TemporaryDirectory() as tmp:
            source, target = Path(tmp) / "source.lib", Path(tmp) / "target.lib"
            source.write_bytes(b"same")
            target.write_bytes(b"same")
            source.chmod(0o444)
            target.chmod(0o444)
            before = target.stat()
            self.assertFalse(libs.stage_file(source, target))
            self.assertEqual(target.stat().st_mode, before.st_mode)
            self.assertEqual(target.stat().st_ino, before.st_ino)
            source.chmod(0o644)
            source.write_bytes(b"diff")
            source.chmod(0o444)
            self.assertTrue(libs.stage_file(source, target))
            self.assertTrue(target.stat().st_mode & 0o200)
            self.assertEqual(target.read_bytes(), b"diff")

    def test_symlink_replaced_without_touching_identical_referent(self):
        with tempfile.TemporaryDirectory() as tmp:
            source, target, referent = (Path(tmp) / name for name in
                                        ("source.lib", "target.lib", "referent.lib"))
            source.write_bytes(b"same")
            referent.write_bytes(b"same")
            target.symlink_to(referent)
            before = referent.stat()
            self.assertTrue(libs.stage_file(source, target))
            self.assertFalse(target.is_symlink())
            self.assertTrue(target.stat().st_mode & 0o200)
            self.assertEqual(referent.stat().st_ino, before.st_ino)
            self.assertEqual(referent.stat().st_mtime_ns, before.st_mtime_ns)

    def test_new_and_changed_files_use_original_creation_mode_under_umask(self):
        with tempfile.TemporaryDirectory() as tmp:
            source, target = Path(tmp) / "source.lib", Path(tmp) / "target.lib"
            source.write_bytes(b"new")
            source.chmod(0o444)
            for mask in (0o027, 0o077):
                with self.subTest(umask=oct(mask)):
                    old_mask = os.umask(mask)
                    try:
                        target.unlink(missing_ok=True)
                        self.assertTrue(libs.stage_file(source, target))
                        self.assertEqual(stat.S_IMODE(target.stat().st_mode), 0o666 & ~mask)
                        target.chmod(0o644)
                        target.write_bytes(b"old")
                        target.chmod(0o444)
                        self.assertTrue(libs.stage_file(source, target))
                        self.assertEqual(stat.S_IMODE(target.stat().st_mode), 0o666 & ~mask)
                    finally:
                        os.umask(old_mask)

    def test_failed_copy_preserves_old_file_and_removes_partial_temporary(self):
        with tempfile.TemporaryDirectory() as tmp:
            source, target = Path(tmp) / "source.lib", Path(tmp) / "target.lib"
            source.write_bytes(b"new")
            target.write_bytes(b"old")
            before = target.stat()

            def fail(_source, temporary):
                temporary.write_bytes(b"partial")
                raise OSError("copy failed")

            with mock.patch.object(libs.shutil, "copyfile", side_effect=fail):
                with self.assertRaisesRegex(OSError, "copy failed"):
                    libs.stage_file(source, target)
            self.assertEqual(target.read_bytes(), b"old")
            self.assertEqual(target.stat().st_ino, before.st_ino)
            self.assertEqual(list(Path(tmp).glob(".vostok-stage-*")), [])


if __name__ == "__main__":
    unittest.main()
