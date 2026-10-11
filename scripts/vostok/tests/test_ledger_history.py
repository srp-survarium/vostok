# SPDX-License-Identifier: GPL-3.0-or-later

"""vostok ledger hist-gap / last-exact over a small synthetic git history."""

import contextlib
import io
import json
import os
import subprocess
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest import mock

from vostok.derive.maxima import source_hash
from vostok.ledger import history

OLD = "mangled\tmodule\tstatus\tcls\tcur\tmax\thist\ttries\thash\tnote"
NEW = "mangled\tunit\tmodule\tstatus\tcls\tcur\tmax\thist\ttries\tsize\tflags\thash\tnote"
DOCS = "docs/binary_matching/match_state.tsv"
CONFIG = "config/match_state.tsv"


def old_ledger(rows):
    lines = ["# ledger", OLD]
    for mangled, (cur, mx, hist, digest) in sorted(rows.items()):
        lines.append(f"{mangled}\tgame\tinprogress\tSIZE\t{cur}\t{mx}\t{hist}\t0\t{digest}\t")
    # a stray data row keyed `mangled` that some revisions carried
    lines.append("mangled\t\tparked\t\t\t\t\t0\t\tnote")
    return "\n".join(lines) + "\n"


def new_ledger(rows):
    lines = ["# ledger", NEW]
    for mangled, (cur, mx, hist, digest) in sorted(rows.items()):
        lines.append(f"{mangled}\tgame/a.cpp\tgame\tinprogress\tSIZE\t{cur}\t{mx}\t"
                     f"{hist}\t0\t64\t\t{digest}\t")
    return "\n".join(lines) + "\n"


class Repo:
    """A throwaway git repository with deterministic commit dates."""

    def __init__(self, root):
        self.root = Path(root)
        self.root.mkdir(parents=True, exist_ok=True)
        self.day = 0
        self.git("init", "-q", "-b", "main")

    def git(self, *args):
        self.day += 1
        env = dict(os.environ,
                   GIT_AUTHOR_NAME="t", GIT_AUTHOR_EMAIL="t@example.invalid",
                   GIT_COMMITTER_NAME="t", GIT_COMMITTER_EMAIL="t@example.invalid",
                   GIT_AUTHOR_DATE=f"2026-08-{self.day:02d}T12:00:00Z",
                   GIT_COMMITTER_DATE=f"2026-08-{self.day:02d}T12:00:00Z",
                   GIT_CONFIG_GLOBAL="/dev/null", GIT_CONFIG_SYSTEM="/dev/null")
        return subprocess.run(["git", *args], cwd=self.root, env=env, check=True,
                              capture_output=True, text=True).stdout.strip()

    def write(self, path, text):
        target = self.root / path
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_text(text)

    def commit(self, subject, **files):
        for path, text in files.items():
            self.write(path.replace("__", "/"), text)
        self.git("add", "-A")
        self.git("commit", "-q", "-m", subject)
        return self.git("rev-parse", "HEAD")


A, B, C = "?a@@YAXXZ", "?b@@YAXXZ", "?c@@YAXXZ"


def build_history(root):
    """main: seed -> A exact -> move+reheader -> (feature: B body changed,
    A noise) merged -> A body changed. C only ever had a carried hist."""
    repo = Repo(root)
    shas = {}
    shas["seed"] = repo.commit("seed the ledger", **{DOCS.replace("/", "__"): old_ledger({
        A: (90, 90, 90, "aaaa00000001"), B: (100, 100, 100, "bbbb00000001"),
        C: (50, 50, 80, "cccc00000001")})})
    shas["exact"] = repo.commit("match a", **{DOCS.replace("/", "__"): old_ledger({
        A: (100, 100, 100, "aaaa00000001"), B: (100, 100, 100, "bbbb00000001"),
        C: (50, 50, 80, "cccc00000001")})})
    repo.git("rm", "-q", DOCS)
    shas["move"] = repo.commit("move the ledger", **{CONFIG.replace("/", "__"): new_ledger({
        A: (100, 100, 100, "aaaa00000001"), B: (100, 100, 100, "bbbb00000001"),
        C: (50, 50, 80, "cccc00000001")})})
    repo.git("checkout", "-q", "-b", "feature")
    shas["b_drop"] = repo.commit("rewrite b", **{CONFIG.replace("/", "__"): new_ledger({
        A: (100, 100, 100, "aaaa00000001"), B: (70, 70, 100, "bbbb00000002"),
        C: (50, 50, 80, "cccc00000001")})})
    repo.git("checkout", "-q", "main")
    shas["noise"] = repo.commit("unrelated build", **{CONFIG.replace("/", "__"): new_ledger({
        A: (95, 100, 100, "aaaa00000001"), B: (100, 100, 100, "bbbb00000001"),
        C: (50, 50, 80, "cccc00000001")})})
    repo.git("merge", "-q", "--no-commit", "-s", "ours", "feature")
    shas["merge"] = repo.commit("merge feature", **{CONFIG.replace("/", "__"): new_ledger({
        A: (95, 100, 100, "aaaa00000001"), B: (70, 70, 100, "bbbb00000002"),
        C: (50, 50, 80, "cccc00000001")})})
    shas["a_drop"] = repo.commit("rewrite a", **{CONFIG.replace("/", "__"): new_ledger({
        A: (60, 60, 100, "aaaa00000002"), B: (70, 70, 100, "bbbb00000002"),
        C: (50, 50, 80, "cccc00000001")})})
    return repo, shas


class WalkTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.tmp = tempfile.TemporaryDirectory()
        cls.repo, cls.shas = build_history(Path(cls.tmp.name) / "repo")
        cls.cache = Path(cls.tmp.name) / "cache"
        cls.history = history.walk([A, B, C], repo=cls.repo.root, cache_dir=cls.cache)

    @classmethod
    def tearDownClass(cls):
        cls.tmp.cleanup()

    def sha(self, index):
        return self.history.commits[index].sha

    def test_walk_follows_the_move_the_header_change_and_the_merge(self):
        subjects = [c.subject for c in self.history.commits]
        self.assertEqual(len(subjects), 7)
        self.assertEqual(subjects[0], "seed the ledger")
        self.assertEqual(subjects[-1], "rewrite a")
        series = dict(zip(subjects, self.history.series[A]))
        self.assertEqual(series["seed the ledger"], (90.0, 90.0, 90.0, "aaaa00000001"))
        self.assertEqual(series["move the ledger"], (100.0, 100.0, 100.0, "aaaa00000001"))
        self.assertEqual(series["merge feature"], (95.0, 100.0, 100.0, "aaaa00000001"))
        self.assertEqual(series["rewrite a"], (60.0, 60.0, 100.0, "aaaa00000002"))
        # the stray `mangled` row is neither a header nor tracked
        self.assertNotIn("mangled", self.history.series)

    def test_cache_round_trip(self):
        again = history.walk([A, B, C], repo=self.repo.root, cache_dir=self.cache)
        self.assertEqual(again.series, self.history.series)
        self.assertEqual(again.stats["computed"], 0)
        self.assertEqual(again.stats["cached"], again.stats["blobs"])

    def test_peak_and_drop_across_a_body_change(self):
        event = history.find_event(self.history, self.history.series[A], 100.0)
        commits = self.history.commits
        self.assertEqual(event.measure, "max")
        self.assertEqual(commits[event.peak].subject, "match a")
        self.assertEqual(commits[event.drop].subject, "rewrite a")
        self.assertTrue(history.body_changed(self.history, self.history.series[A], event.drop))

    def test_drop_on_a_merged_branch_names_the_branch_commit(self):
        event = history.find_event(self.history, self.history.series[B], 100.0)
        self.assertEqual(self.history.commits[event.peak].subject, "seed the ledger")
        self.assertEqual(self.history.commits[event.drop].subject, "rewrite b")

    def test_carried_hist_is_inherited_and_never_drops(self):
        event = history.find_event(self.history, self.history.series[C], 80.0)
        self.assertTrue(event.inherited)
        self.assertEqual(self.history.commits[event.peak].subject, "seed the ledger")
        self.assertIsNone(event.drop)

    def test_last_exact_counts_an_exact_cur(self):
        event = history.last_exact_event(self.history, self.history.series[A])
        self.assertEqual(self.history.commits[event.last].subject, "merge feature")
        self.assertEqual(self.history.commits[event.drop].subject, "rewrite a")

    def test_hist_gap_command(self):
        rows = {
            A: {"mangled": A, "module": "game", "unit": "game/a.cpp", "size": 64,
                "cur": 60.0, "max": 60.0, "hist": 100.0, "hash": "aaaa00000002"},
            B: {"mangled": B, "module": "game", "unit": "game/a.cpp", "size": 640,
                "cur": 70.0, "max": 70.0, "hist": 100.0, "hash": "bbbb00000002"},
            C: {"mangled": C, "module": "core", "unit": "core/c.cpp", "size": 8,
                "cur": 50.0, "max": 50.0, "hist": 80.0, "hash": "cccc00000001"},
            "?d@@YAXXZ": {"mangled": "?d@@YAXXZ", "module": "game", "unit": "",
                          "size": 9, "cur": 99.0, "max": 99.0, "hist": 99.0, "hash": ""},
        }
        args = SimpleNamespace(module=None, unit=None, min_gap=0.01, exact=False,
                               sort="recoverable", limit=None, json=True, cache_dir=None)
        out = io.StringIO()
        with contextlib.redirect_stdout(out), contextlib.redirect_stderr(io.StringIO()):
            history.cmd_hist_gap(args, rows=rows, history=self.history)
        records = json.loads(out.getvalue())
        self.assertEqual([r["mangled"] for r in records], [B, A, C])
        self.assertEqual(records[0]["recoverable"], 640 * 30.0)
        self.assertEqual(records[0]["drop"]["subject"], "rewrite b")
        self.assertTrue(records[0]["drop"]["body_changed"])
        self.assertEqual(records[0]["drop"]["score"], 70.0)
        self.assertTrue(records[2]["inherited"])

        args.json, args.module = False, "game"
        out = io.StringIO()
        with contextlib.redirect_stdout(out), contextlib.redirect_stderr(io.StringIO()):
            history.cmd_hist_gap(args, rows=rows, history=self.history)
        text = out.getvalue()
        self.assertIn("rewrite b", text)
        self.assertIn("(body changed)", text)
        self.assertNotIn(C, text)


class ParseTests(unittest.TestCase):
    def test_parse_log_inherits_the_first_parents_blob_through_a_merge(self):
        text = ("\x01c1\x02\x022026-08-01\x02one\n\n"
                ":000000 100644 " + "0" * 40 + " " + "1" * 40 + " A\tconfig/m.tsv\n"
                "\x01c2\x02c1 x9\x022026-08-02\x02merge\n\n")
        commits = history.parse_log(text)
        self.assertEqual([c.blob for c in commits], ["1" * 40, "1" * 40])
        self.assertIsNone(commits[0].src)
        self.assertEqual(commits[1].parents, ("c1", "x9"))

    def test_delta_reads_rows_with_the_new_header(self):
        diff = "\n".join([
            "diff --git a/x b/y", "--- a/x", "+++ b/y", "@@ -2 +2 @@",
            "-" + OLD, "+" + NEW,
            "-?a@@\tgame\tdone\tMATCH\t100\t100\t100\t0\th1\t",
            "+?a@@\tgame/a.cpp\tgame\tdone\tMATCH\t99\t100\t100\t0\t8\t\th1\t",
            "-?gone@@\tgame\tdone\tMATCH\t1\t1\t1\t0\th\t",
        ])
        header, delta = history.parse_delta(diff, tuple(OLD.split("\t")))
        self.assertEqual(header, tuple(NEW.split("\t")))
        self.assertEqual(delta, {"?a@@": (99.0, 100.0, 100.0, "h1"), "?gone@@": None})


SOURCE = """\
namespace ns {

// a comment naming run( ) must not count
int widget::run( int x ) const
{
\tif ( x ) { return 1; }
\treturn 0;
}

class gadget {
public:
\tvoid run( int x ) { m = x; }
\tint m;
};

widget::widget( ) :
\tm_value ( call( 1 ) ),
\tm_other ( 2 )
#line 40
{
\tstart ( );
}

} // namespace ns
"""


class LocateTests(unittest.TestCase):
    def test_names(self):
        self.assertEqual(history.names_from_mangled("?run@widget@ns@@QBEHH@Z"),
                         ("run", "widget"))
        self.assertEqual(history.names_from_mangled("??0widget@ns@@QAE@XZ"),
                         ("widget", "widget"))
        self.assertEqual(history.names_from_mangled("??1widget@ns@@QAE@XZ"),
                         ("~widget", "widget"))
        self.assertEqual(history.names_from_mangled("??4widget@ns@@QAEAAV01@ABV01@@Z"),
                         ("operator=", "widget"))
        self.assertEqual(history.names_from_mangled("?Resize@?$ArrayData@H@Scaleform@@QAEXI@Z"),
                         ("Resize", "ArrayData"))
        self.assertIsNone(history.names_from_mangled("??_Gwidget@ns@@UAEPAXI@Z"))
        self.assertEqual(history.names_from_demangled(
            "vostok::fs_new::path_string_impl::path_string_impl<char const *>(char, char const* const&)"),
            ("path_string_impl", "path_string_impl"))
        self.assertEqual(history.names_from_demangled("ns::cls<int>::operator()(int)"),
                         ("operator()", "cls"))

    def test_bodies_skip_comments_and_cross_line_pins(self):
        runs = history.find_bodies(SOURCE, "run", "widget")
        self.assertEqual([(b.first, b.open, b.last, b.qualified) for b in runs],
                         [(4, 5, 8, True), (12, 12, 12, False)])
        gadget = history.locate(SOURCE, ("run", "gadget"))
        self.assertEqual(gadget.first, 12)
        ctor = history.locate(SOURCE, ("widget", "widget"))
        self.assertEqual((ctor.first, ctor.open, ctor.last), (16, 20, 22))

    def test_locate_verifies_the_ledger_hash(self):
        lines = SOURCE.splitlines(keepends=True)
        digest = source_hash("".join(lines[4:8]))       # `{` .. `}`
        body = history.locate(SOURCE, ("run", None), digest)
        self.assertTrue(body.verified)
        self.assertEqual(body.first, 4)
        # through `#line 40`: the PDB numbers the `{` 40
        pinned = source_hash("".join(lines[19:22]))
        ctor = history.locate(SOURCE, ("widget", "widget"), pinned)
        self.assertTrue(ctor.verified)

    def test_function_diff_against_head(self):
        with tempfile.TemporaryDirectory() as tmp:
            repo = Repo(tmp)
            before = repo.commit("exact", **{"sources__ns__w.cpp": SOURCE})
            repo.commit("edit", **{"sources__ns__w.cpp": SOURCE.replace(
                "\treturn 0;", "\treturn 2;").replace("m = x;", "m = -x;")})
            lines = SOURCE.splitlines(keepends=True)
            diff = history.function_diff(repo.root, before, "sources/ns/w.cpp",
                                         "?run@widget@ns@@QBEHH@Z", None,
                                         source_hash("".join(lines[4:8])), None)
            self.assertIn("verified at the exact commit", diff["scope"])
            self.assertIn("-\treturn 0;", diff["text"])
            self.assertIn("+\treturn 2;", diff["text"])
            self.assertNotIn("m = -x", diff["text"])
            whole = history.function_diff(repo.root, before, "sources/ns/w.cpp",
                                          "??_Gwidget@ns@@UAEPAXI@Z", None, None, None)
            self.assertIn("whole-file diff", whole["scope"])
            self.assertIn("m = -x", whole["text"])


class SelectTests(unittest.TestCase):
    def test_select(self):
        rows = {m: {"mangled": m} for m in
                ("?run@widget@ns@@QBEHH@Z", "?run@gadget@ns@@QAEXH@Z", "?stop@widget@ns@@QAEXXZ")}
        self.assertEqual(len(history.select(rows, "?run@widget@ns@@QBEHH@Z")), 1)
        self.assertEqual([r["mangled"] for r in history.select(rows, "ns::widget::run")],
                         ["?run@widget@ns@@QBEHH@Z"])
        self.assertEqual(len(history.select(rows, "?run@")), 2)
        with mock.patch.object(history.store, "load", return_value=rows), \
                contextlib.redirect_stderr(io.StringIO()):
            args = SimpleNamespace(function="nothing", limit=5, diff=False,
                                   json=False, cache_dir=None)
            self.assertEqual(history.cmd_last_exact(args), 1)
            args.function, args.limit = "?run@", 1
            self.assertEqual(history.cmd_last_exact(args), 1)


if __name__ == "__main__":
    unittest.main()
