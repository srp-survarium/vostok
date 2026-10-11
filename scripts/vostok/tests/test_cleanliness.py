# SPDX-License-Identifier: GPL-3.0-or-later

"""vostok tool cleanliness: the per-module board, its kinds and the down-only gate."""

import contextlib
import io
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

from vostok.tool import cleanliness


class CleanlinessTests(unittest.TestCase):
    def setUp(self):
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        self.root = Path(temporary.name) / "vostok"
        self.root.mkdir()
        self.baseline = Path(temporary.name) / "baseline.tsv"
        for name, value in (("ENGINE", self.root), ("CLEANLINESS_BASELINE", self.baseline)):
            replacement = patch.object(cleanliness, name, value)
            replacement.start()
            self.addCleanup(replacement.stop)

    def write(self, rel, text, encoding="utf-8"):
        path = self.root / rel
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_bytes(text.encode(encoding))

    def board(self):
        board, _sites = cleanliness.scan(jobs=1)
        return board

    def run_main(self, *argv):
        out = io.StringIO()
        with contextlib.redirect_stdout(out), contextlib.redirect_stderr(io.StringIO()):
            rc = cleanliness.main(list(argv))
        return rc, out.getvalue()

    def test_embedded_samples(self):
        self.assertEqual(cleanliness.selftest(), [])

    def test_counts_per_module_and_root_headers(self):
        self.write("game/sources/a.cpp",
                   "void f( )\n{\n\ta( ); b( );\n\tif ( x ) return;\n\tx = (u32)y;\n}\n")
        self.write("render/b.h", "// claude@NOTE: residual is LTCG\nvolatile int v;\n")
        self.write("macros.h", "// sushi@TODO: why?\n// STATE[STUB]\n")
        board = self.board()
        self.assertEqual(board["game"]["joined"], 1)
        self.assertEqual(board["game"]["ctl-line"], 1)
        self.assertEqual(board["game"]["c-cast"], 1)
        self.assertEqual(board["render"]["prose"], 1)
        self.assertEqual(board["render"]["volatile"], 1)
        self.assertEqual(board[cleanliness.ROOT_MODULE]["todo"], 1)
        self.assertEqual(board[cleanliness.ROOT_MODULE]["state"], 1)

    def test_vendored_subtrees_are_not_scanned(self):
        self.write("core/sources/ptmalloc3/malloc.c", "void f( ) { a( ); b( ); }\n")
        self.write("core/sources/delayhlp.cpp", "void f( ) { a( ); b( ); }\n")
        self.write("core/sources/own.cpp", "void f( ) { a( ); b( ); }\n")
        _board, sites = cleanliness.scan(jobs=1)
        self.assertEqual([s.rel for s in sites], ["core/sources/own.cpp"])

    def test_utf16_sources_are_decoded(self):
        self.write("editor/form.h", "﻿void f( )\n{\n\ta( ); b( );\n}\n", "utf-16-le")
        self.assertEqual(self.board()["editor"]["joined"], 1)

    def test_list_reports_file_and_line(self):
        self.write("game/sources/a.cpp", "void f( )\n{\n\ta( ); b( );\n}\n")
        rc, out = self.run_main("--list", "joined")
        self.assertEqual(rc, 0)
        self.assertEqual(out.strip(), "sources/vostok/game/sources/a.cpp:3: [joined] a( ); b( );")

    def test_gate_passes_at_floor_and_fails_on_a_rise(self):
        self.write("game/sources/a.cpp", "void f( )\n{\n\ta( ); b( );\n}\n")
        self.assertEqual(self.run_main("--update")[0], 0)
        self.assertEqual(self.run_main("--check")[0], 0)
        self.write("game/sources/a.cpp", "void f( )\n{\n\ta( ); b( );\n\tc( ); d( );\n}\n")
        rc, _out = self.run_main("--check")
        self.assertEqual(rc, 1)
        violations = cleanliness.check(self.board())
        self.assertEqual(len(violations), 1)
        self.assertIn("game joined: 1 -> 2", violations[0])

    def test_gate_never_writes_and_a_drop_passes(self):
        self.write("game/sources/a.cpp", "void f( )\n{\n\ta( ); b( );\n}\n")
        self.run_main("--update")
        before = self.baseline.read_text()
        self.write("game/sources/a.cpp", "void f( )\n{\n\ta( );\n\tb( );\n}\n")
        self.assertEqual(self.run_main("--check")[0], 0)
        self.assertEqual(self.baseline.read_text(), before)

    def test_a_new_module_has_floor_zero(self):
        self.write("game/sources/a.cpp", "void f( ) { }\n")
        self.run_main("--update")
        self.write("sound/sources/b.cpp", "void f( )\n{\n\ta( ); b( );\n}\n")
        violations = cleanliness.check(self.board())
        self.assertTrue(any(v.startswith("sound joined: 0 -> 1") for v in violations), violations)

    def test_gate_fails_closed_without_floors(self):
        self.write("game/sources/a.cpp", "void f( ) { }\n")
        violations = cleanliness.check(self.board())
        self.assertTrue(violations and "no committed floor" in violations[0], violations)
        self.baseline.write_text("module\tjoined\ngame\t0\n")
        violations = cleanliness.check(self.board())
        self.assertIn("ctl-line", violations[0])
        self.assertNotIn("joined", violations[0].split("in ")[0])

    def test_baseline_round_trip(self):
        self.write("game/sources/a.cpp", "void f( )\n{\n\ta( ); b( );\n}\n")
        board = self.board()
        cleanliness.save_baseline(board)
        columns, floors = cleanliness.load_baseline()
        self.assertEqual(columns, list(cleanliness.KIND_KEYS))
        self.assertEqual(floors["game"]["joined"], 1)
        self.assertEqual(floors["game"]["prose"], 0)

    def test_json_and_markdown_render_every_kind(self):
        self.write("game/sources/a.cpp", "void f( )\n{\n\ta( ); b( );\n}\n")
        rc, out = self.run_main("--json")
        self.assertEqual(rc, 0)
        self.assertIn('"joined": 1', out)
        rc, out = self.run_main("--markdown")
        self.assertEqual(rc, 0)
        self.assertTrue(out.startswith("| module | " + " | ".join(cleanliness.KIND_KEYS)))
        self.assertIn("| game | 1 |", out)


if __name__ == "__main__":
    unittest.main()
