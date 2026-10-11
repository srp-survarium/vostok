# SPDX-License-Identifier: GPL-3.0-or-later
"""Setup does not invalidate measured inputs when generated content is unchanged."""

import os
import tempfile
import unittest
from pathlib import Path
from unittest import mock

from vostok.tool import toolchain


class SetupGraphTests(unittest.TestCase):
    def test_setup_preserves_unchanged_graph_rsp_and_clangd_mtimes(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            build = root / "ninja"
            build.mkdir()
            graph = {"build.ninja": "rule cl\n", "probe_cl_0.rsp": "unchanged\n"}
            clangd = {"compile_commands.json": "[]\n", "clangd-vfs.yaml": "{}\n"}
            files = []
            for directory, payload in ((build, graph), (root, clangd)):
                for name, content in payload.items():
                    path = directory / name
                    path.write_text(content)
                    os.utime(path, ns=(1000000000, 1000000000))
                    files.append(path)
            before = {path: path.stat().st_mtime_ns for path in files}
            calls = []

            def converter(out, target="ninja"):
                calls.append(target)
                for name, content in (graph if target == "ninja" else clangd).items():
                    (out / name).write_text(content)

            regen = toolchain.ninja_regen
            with mock.patch.object(regen, "BUILD_DIR", build), \
                 mock.patch.object(regen, "VOSTOK_DIR", root), \
                 mock.patch.object(regen, "gen_fresh", side_effect=converter), \
                 mock.patch.object(toolchain.subprocess, "run") as raw_converter:
                toolchain.generate_ninja()
            self.assertEqual(calls, ["ninja", "clangd"])
            raw_converter.assert_not_called()
            self.assertEqual(before, {path: path.stat().st_mtime_ns for path in files})


if __name__ == "__main__":
    unittest.main()
