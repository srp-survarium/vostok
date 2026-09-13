# SPDX-License-Identifier: GPL-3.0-or-later
"""Recovered SDK materialization and cache invalidation contracts."""

import errno
from pathlib import Path
import tempfile
import unittest
from unittest import mock

from vostok.build import gfx


class RecoveredSdkTests(unittest.TestCase):
    def test_engine_allocator_variant_keeps_distinct_source_attribution(self):
        from vostok.core import paths
        legacy = "scaleform/src/kernel/heapmh/heapmh_sysallocmalloc.h"
        sdk = "src/kernel/heapmh/heapmh_sysallocmalloc.h"
        self.assertEqual(paths.gfx_source_relative(legacy), "Include/Vostok_HeapMH_SysAllocMalloc.h")
        self.assertEqual(paths.gfx_source_relative(sdk), sdk)

    def test_pin_change_replaces_links_and_removes_deleted_files(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            tree = root / "tree"
            first, second = root / "first", root / "second"
            for sdk in (first, second):
                for sub in ("Src", "Include", "3rdParty"):
                    (sdk / sub).mkdir(parents=True)
                (sdk / "Src/example.cpp").write_bytes(sdk.name.encode())
            (first / "Include/deleted.h").write_bytes(b"old")
            with mock.patch.object(gfx, "GFX_BUILD_TREE", tree):
                with mock.patch.object(gfx, "SDK", first):
                    gfx.materialize_tree()
                    self.assertEqual((tree / "Include/deleted.h").read_bytes(), b"old")
                with mock.patch.object(gfx, "SDK", second):
                    gfx.materialize_tree()
                    gfx.materialize_tree()
            self.assertTrue((tree / "Src/example.cpp").samefile(second / "Src/example.cpp"))
            self.assertFalse((tree / "Include/deleted.h").exists())

    def test_read_only_sdk_links_replace_old_overlay_and_dangling_links(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            sdk, tree = root / "sdk", root / "tree"
            for sub in ("Src", "Include", "3rdParty"):
                (sdk / sub).mkdir(parents=True)
                (tree / sub).mkdir(parents=True)
            (sdk / "Src/example.cpp").write_bytes(b"recovered\r\n")
            (tree / "Src/example.cpp").write_bytes(b"old overlay")
            (sdk / "Include/example.h").write_bytes(b"header\r\n")
            (tree / "Include/example.h").symlink_to(root / "missing-store-path")
            with (
                mock.patch.object(gfx, "SDK", sdk),
                mock.patch.object(gfx, "GFX_BUILD_TREE", tree),
                mock.patch.object(gfx.os, "link", side_effect=OSError(errno.EPERM, "read only")),
            ):
                gfx.materialize_tree()
                gfx.materialize_tree()
            self.assertEqual((tree / "Src/example.cpp").read_bytes(), b"recovered\r\n")
            self.assertTrue((tree / "Src/example.cpp").is_symlink())
            self.assertEqual((tree / "Include/example.h").read_bytes(), b"header\r\n")

    def test_mutable_override_header_change_invalidates_objects(self):
        with tempfile.TemporaryDirectory() as directory:
            sdk = Path(directory)
            for sub in ("Src", "Include", "3rdParty"):
                (sdk / sub).mkdir()
            header = sdk / "Include/example.h"
            header.write_bytes(b"before")
            with mock.patch.object(gfx, "SDK", sdk):
                first = gfx.build_key("libgfx", "flags", (), (), ("Src/example.cpp",))
                header.write_bytes(b"after")
                second = gfx.build_key("libgfx", "flags", (), (), ("Src/example.cpp",))
            self.assertNotEqual(first, second)


if __name__ == "__main__":
    unittest.main()
