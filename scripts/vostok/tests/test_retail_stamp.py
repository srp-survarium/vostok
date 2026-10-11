# SPDX-License-Identifier: GPL-3.0-or-later
"""Retail provenance stamps: the wrong-retail target side fails fast."""

import os
import subprocess
import tempfile
import unittest
from pathlib import Path
from unittest import mock

from vostok.build import generate_pdb, retail_stamp, shared_cache
from vostok.core import paths
from vostok.tool import toolchain


class RetailStampTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        root = Path(self.tmp.name)
        self.repo = root / "repo"
        self.game = self.make_game(root / "v0.100b", b"exe 100b", b"pdb 100b")
        self.other = self.make_game(root / "v0.20e", b"exe 20e", b"pdb 20e")
        binaries = self.repo / "binaries"
        self.pdb = binaries / "pdb" / "target"
        self.objdiff = binaries / "objdiff" / "target"
        self.symbol_map = binaries / "objdiff" / "target-symbol-map.tsv"
        self.data = binaries / "data-objdiff" / "target"
        self.rich = binaries / "rich" / "target"
        self.patches = [
            mock.patch.object(paths, "REPO", self.repo),
            mock.patch.object(paths, "TARGET_PDB_DIR", self.pdb),
            mock.patch.object(paths, "TARGET_STRUCTURE_DIR", binaries / "structure" / "target"),
            mock.patch.object(paths, "OBJDIFF_DIR", binaries / "objdiff"),
            mock.patch.object(paths, "SYMBOL_MAP", self.symbol_map),
            mock.patch.object(paths, "DATA_OBJDIFF_DIR", binaries / "data-objdiff"),
            mock.patch.object(paths, "DATA_SYMBOL_MAP", binaries / "data-objdiff" / "target-symbol-map.tsv"),
            mock.patch.object(paths, "LEGACY_RICH_DIR", binaries / "rich"),
            mock.patch.dict(os.environ, {"SURVARIUM_BIN": str(self.game)}),
            mock.patch.object(toolchain, "VOSTOK_DIR", self.repo),
            mock.patch.object(toolchain, "log", lambda message: None),
        ]
        for patch in self.patches:
            patch.start()

    def tearDown(self):
        for patch in reversed(self.patches):
            patch.stop()
        self.tmp.cleanup()

    @staticmethod
    def make_game(directory, exe, pdb):
        directory.mkdir(parents=True)
        (directory / "survarium.exe").write_bytes(exe)
        (directory / "survarium.pdb").write_bytes(pdb)
        return directory

    def populate(self, game):
        for tree, name in ((self.pdb, "evidence.sqlite"), (self.objdiff, "a.obj"),
                           (self.data, "d.obj")):
            tree.mkdir(parents=True, exist_ok=True)
            (tree / name).write_bytes(b"derived")
            retail_stamp.write(tree, game)
        self.symbol_map.write_text("map\n")
        retail_stamp.write(self.symbol_map, game)

    def test_matching_stamps_pass(self):
        self.populate(self.game)
        self.assertEqual(retail_stamp.require_current(), [])
        stamp = retail_stamp.read(self.pdb)
        self.assertEqual(stamp["retail"]["survarium.exe"],
                         retail_stamp.retail_identity()["survarium.exe"])
        self.assertTrue((self.symbol_map.parent / "target-symbol-map.tsv.retail-stamp.json").is_file())

    def test_another_retail_fails_fast_with_the_fix(self):
        self.populate(self.other)
        with self.assertRaises(retail_stamp.RetailMismatch) as caught:
            retail_stamp.require_current()
        message = str(caught.exception)
        self.assertIn("binaries/pdb/target", message)
        self.assertIn("binaries/objdiff/target-symbol-map.tsv", message)
        self.assertIn("--regenerate-target", message)
        with self.assertRaises(SystemExit):
            toolchain._check_retail_stamps(force=False)
        self.assertTrue(self.objdiff.is_dir(), "a mismatch never deletes silently")

    def test_regenerate_target_accepts_the_mismatch_and_drops_data(self):
        self.populate(self.other)
        toolchain._check_retail_stamps(force=True)
        self.assertFalse(self.data.exists(), "the data COFF is rebuilt by the next full build")
        self.assertTrue(self.pdb.exists())

    def test_unstamped_artifacts_are_regenerated_not_trusted(self):
        self.populate(self.game)
        retail_stamp.clear(self.symbol_map)
        self.rich.mkdir(parents=True)
        (self.rich / "index.jsonl").write_text("{}\n")
        mismatches, unstamped, notes = retail_stamp.audit()
        self.assertEqual(mismatches, [])
        self.assertEqual([a.name for a in unstamped], ["objdiff-symbol-map"])
        self.assertIn("binaries/rich/target", notes[0])
        toolchain._check_retail_stamps(force=False)
        self.assertFalse(self.symbol_map.exists())
        self.assertFalse(self.objdiff.exists(), "the map's COFF tree regenerates with it")
        self.assertTrue(self.rich.exists(), "legacy trees are reported, never removed")

    def test_changed_retail_bytes_change_the_shared_cache_key(self):
        with mock.patch.object(shared_cache, "_tool", return_value="/nix/store/x-vostok-pdb"):
            here = shared_cache._retail_inputs("c:\\survarium\\sources")
            with mock.patch.dict(os.environ, {"SURVARIUM_BIN": str(self.other)}):
                there = shared_cache._retail_inputs("c:\\survarium\\sources")
        self.assertNotEqual(shared_cache.key_of(here), shared_cache.key_of(there))
        self.assertEqual(here["retail"], retail_stamp.retail_identity(self.game))

    def test_generate_pdb_stamps_the_target_database(self):
        def fake_index(command, check):
            Path(command[command.index("--database") + 1]).write_bytes(b"sqlite")
            return subprocess.CompletedProcess(command, 0)

        with mock.patch.object(generate_pdb.subprocess, "run", side_effect=fake_index):
            generate_pdb.generate("target", out=self.pdb)
        self.assertEqual(retail_stamp.read(self.pdb)["retail"],
                         retail_stamp.retail_identity(self.game))


if __name__ == "__main__":
    unittest.main()
