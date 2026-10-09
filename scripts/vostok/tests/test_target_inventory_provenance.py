# SPDX-License-Identifier: GPL-3.0-or-later

"""Retail inventory reuse must follow the actual exporter and input bytes."""

import json
import subprocess
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import patch

from vostok.data import pipeline


INDEX = (
    "rva\tsection\tstorage\tsize\tsize_kind\ttype_index\tscope\tmodule_hex\t"
    "archive_hex\tname_hex\tpublic_name_hex\n"
    "0x1000\t.rdata\trdata\t4\tdeclared\t0x0\texternal\t\t\t61\t61\n"
)


class TargetInventoryProvenanceTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        root = Path(self.tmp.name)
        self.exe, self.pdb, self.exporter = [root / n for n in ("image.exe", "image.pdb", "exporter")]
        for p in (self.exe, self.pdb, self.exporter):
            p.write_bytes(p.name.encode())
        self.index = root / "index.tsv"
        self.stamp = root / "provenance.json"
        self.exports = 0
        for mocked in (
            patch.object(pipeline.paths, "DATA_TARGET_INDEX", self.index),
            patch.object(pipeline.paths, "DATA_TARGET_INDEX_PROVENANCE", self.stamp),
            patch.object(pipeline, "image_paths", return_value=(self.exe, self.pdb)),
            patch.object(pipeline.shutil, "which", return_value=str(self.exporter)),
            patch.object(pipeline, "_engine_args", return_value=["--engine-path", "retail"]),
            patch.object(pipeline.subprocess, "run", side_effect=self.run_exporter),
            patch.object(pipeline, "PEImage"),
            patch.object(pipeline, "_write_relocations"),
            patch.object(pipeline, "_write_access"),
            patch.object(pipeline, "_write_retail_census"),
            patch("vostok.data.missing.refresh", return_value={
                "unique_missing_targets": 0, "unresolved_targets": 0,
            }),
            patch.object(pipeline, "log"),
        ):
            mocked.start()
            self.addCleanup(mocked.stop)

    def run_exporter(self, command, **kwargs):
        if command[-1] == "--help":
            return SimpleNamespace(stdout="--write-data-index", stderr="")
        self.exports += 1
        self.index.write_text(INDEX)
        return SimpleNamespace(returncode=0)

    def test_missing_provenance_exports_then_unchanged_inputs_reuse(self):
        self.index.write_text(INDEX)
        pipeline.init_target()
        stamp = self.stamp.read_bytes()
        pipeline.init_target()
        self.assertEqual(self.exports, 1)
        self.assertEqual(self.stamp.read_bytes(), stamp)

    def test_stale_provenance_does_not_force_managed_census_rewrite(self):
        with patch.object(pipeline, "_write_retail_census") as census:
            pipeline.init_target()
            census.assert_called_once()
            self.assertEqual(census.call_args.kwargs, {"force": False})
            census.reset_mock()
            self.exporter.write_bytes(b"changed exporter")
            pipeline.init_target()
            self.assertEqual(census.call_args.kwargs, {"force": False})
            census.reset_mock()
            pipeline.init_target(force=True)
            self.assertEqual(census.call_args.kwargs, {"force": True})
        self.assertEqual(self.exports, 3)

    def test_exporter_and_retail_input_bytes_invalidate_reuse(self):
        pipeline.init_target()
        for source in (self.exporter, self.exe, self.pdb):
            with self.subTest(source=source.name):
                before = self.exports
                source.write_bytes(source.read_bytes() + b"changed")
                pipeline.init_target()
                self.assertEqual(self.exports, before + 1)

    def test_exporter_arguments_invalidate_reuse(self):
        pipeline.init_target()
        with patch.object(pipeline, "_engine_args", return_value=["--engine-path", "other"]):
            pipeline.init_target()
        self.assertEqual(self.exports, 2)

    def test_inventory_tampering_invalidate_reuse(self):
        pipeline.init_target()
        self.index.write_text(INDEX.replace("\t4\tdeclared", "\t8\tdeclared"))
        pipeline.init_target()
        self.assertEqual(self.exports, 2)
        self.assertEqual(self.index.read_text(), INDEX)

    def test_corrupt_and_structurally_wrong_metadata_reexport(self):
        pipeline.init_target()
        for text in ("{", "[]", json.dumps({"inputs": {}})):
            with self.subTest(text=text):
                before = self.exports
                self.stamp.write_text(text)
                pipeline.init_target()
                self.assertEqual(self.exports, before + 1)

    def test_failed_export_cannot_leave_trusted_partial_output(self):
        pipeline.init_target()
        self.exporter.write_bytes(b"new exporter")

        def fail(command, **kwargs):
            if command[-1] == "--help":
                return SimpleNamespace(stdout="--write-data-index", stderr="")
            self.index.write_text("partial output")
            raise subprocess.CalledProcessError(1, command)

        with patch.object(pipeline.subprocess, "run", side_effect=fail):
            with self.assertRaises(subprocess.CalledProcessError):
                pipeline.init_target()
        self.assertFalse(self.stamp.exists())
        pipeline.init_target()
        self.assertEqual(self.exports, 2)

    def test_inputs_changed_during_export_get_no_provenance(self):
        original = self.run_exporter

        def changed(command, **kwargs):
            result = original(command, **kwargs)
            if command[-1] != "--help":
                self.pdb.write_bytes(b"changed during export")
            return result

        with patch.object(pipeline.subprocess, "run", side_effect=changed):
            with self.assertRaisesRegex(RuntimeError, "inputs changed"):
                pipeline.init_target()
        self.assertFalse(self.stamp.exists())

    def test_success_exit_with_invalid_inventory_gets_no_provenance(self):
        original = self.run_exporter

        def malformed(command, **kwargs):
            result = original(command, **kwargs)
            if command[-1] != "--help":
                self.index.write_text("invalid inventory header")
            return result

        with patch.object(pipeline.subprocess, "run", side_effect=malformed):
            with self.assertRaises(ValueError):
                pipeline.init_target()
        self.assertFalse(self.stamp.exists())


if __name__ == "__main__":
    unittest.main()
