# SPDX-License-Identifier: GPL-3.0-or-later

"""vostok evidence: the structure-diff port, the geometry breaks, selection over
fixture evidence databases, and the actionable-only rendering."""

import json
import shutil
import sqlite3
import subprocess
import tempfile
import unittest
from pathlib import Path
from unittest import mock

from vostok.core.paths import BASE_EVIDENCE, MATCH_STATE, TARGET_EVIDENCE
from vostok.evidence import evidence, render, sections
from vostok.evidence import gather as gather_mod
from vostok.evidence import select as select_mod
from vostok.evidence.structure import compare, diff_structure, geometry_breaks, structure_rows


def stmt(off, size, line, source=None, file="a.cpp"):
    s = {"off": off, "size": size, "line": line, "file": file}
    if source is not None:
        s["source"] = source
    return s


def record(statements, size=None, file="a.cpp", name="f()", mangled="?f@@YAXXZ",
           instructions=()):
    size = size if size is not None else sum(s["size"] for s in statements)
    return {"name": name, "mangled": mangled, "file": file, "rva": 0x1000, "image_base": 0x10000,
            "size": size, "statements": list(statements), "instructions": list(instructions)}


def body(*sizes_lines, first_line=10):
    """A record whose body statements have these (size, line) pairs, framed by
    a 1-byte `{` on `first_line` and a 1-byte `}` after the last line."""
    out, off = [stmt(0, 1, first_line, "{")], 1
    for size, line in sizes_lines:
        out.append(stmt(off, size, line, f"s{line};"))
        off += size
    out.append(stmt(off, 1, max(line for _, line in sizes_lines) + 1, "}"))
    return out


class StructurePortTests(unittest.TestCase):
    """Mirrors tools/vostok-pdb/src/rich_structure_diff.rs."""

    def test_rows_skip_frame_braces_and_collapse_gaps(self):
        rows = structure_rows(body((5, 11), (3, 14), (2, 15)))
        self.assertEqual([None if r is None else r["size"] for r in rows], [5, None, 3, 2])

    def test_body_less_has_no_rows(self):
        self.assertEqual(structure_rows([stmt(0, 1, 1), stmt(1, 1, 2)]), [])
        self.assertEqual(compare(record([stmt(0, 1, 1)]), record([stmt(0, 1, 1)]))["verdict"],
                         "NO BODY")

    def test_identical_is_a_match(self):
        f = record(body((5, 11), (3, 12)))
        self.assertEqual(compare(f, f)["verdict"], "MATCH")

    def test_equal_body_with_total_size_difference(self):
        t = record(body((8, 11)), size=10)
        b = record(body((8, 11)), size=14)
        result = compare(b, t)
        self.assertEqual(result["verdict"], "BODY MATCH")
        self.assertEqual(result["total_delta"], 4)

    def test_changed_statement_is_size_then_offset_shifts(self):
        t = record(body((5, 11), (3, 12), (4, 13)))
        b = record(body((7, 11), (3, 12), (4, 13)))
        result = compare(b, t)
        self.assertEqual(result["verdict"], "SIZE")
        self.assertEqual([r["tags"] for r in result["rows"]],
                         [["SIZE"], ["OFFSET"], ["OFFSET"]])
        self.assertEqual(result["rows"][0]["delta"], 2)
        self.assertEqual(result["rows"][1]["shift"], 2)

    def test_one_sided_statements(self):
        t = record(body((5, 11), (9, 12), (4, 13)))
        b = record(body((5, 11), (4, 13), (6, 14)))
        result = compare(b, t)
        self.assertEqual(result["verdict"], "DIFFERS")
        tags = [r["tags"][0] for r in result["rows"] if "OFFSET" not in r["tags"]]
        self.assertIn("TRGT_ONLY", tags)
        self.assertIn("BASE_ONLY", tags)

    def test_delete_run_pairs_with_following_insert_run(self):
        kinds = [k for k, _, _ in diff_structure(structure_rows(body((5, 11), (7, 12))),
                                                 structure_rows(body((5, 11), (8, 12))))]
        self.assertEqual(kinds, ["equal", "changed"])

    def test_frame_brace_deltas_are_split_out(self):
        t = record(body((5, 11)))
        b = record(body((5, 11)))
        b["statements"][0] = dict(b["statements"][0], size=3)
        self.assertEqual(compare(b, t)["frame"], {"open": 2})


class GeometryBreakTests(unittest.TestCase):
    def breaks(self, base, target):
        b, t = record(base), record(target)
        aligned = compare(b, t)["aligned"]
        return [(x["lines"], x["base"]["line"]) for x in geometry_breaks(b, t, aligned)]

    def test_same_spacing_has_no_break(self):
        self.assertEqual(self.breaks(body((5, 11), (3, 13)), body((5, 41), (3, 43), first_line=40)),
                         [])

    def test_one_inserted_line_is_one_break_not_a_cascade(self):
        ours = body((5, 11), (3, 12), (2, 13), (4, 14))
        retail = body((5, 11), (3, 13), (2, 14), (4, 15))
        self.assertEqual(self.breaks(ours, retail), [(1, 12)])

    def test_lines_are_read_in_source_order(self):
        ours = [stmt(0, 1, 10), stmt(1, 5, 14), stmt(6, 3, 11), stmt(9, 1, 15)]
        retail = [stmt(0, 1, 10), stmt(1, 5, 15), stmt(6, 3, 11), stmt(9, 1, 16)]
        self.assertEqual(self.breaks(ours, retail), [(1, 14)])


SCHEMA = """
CREATE TABLE functions(id INTEGER PRIMARY KEY, name TEXT, mangled TEXT, rva INTEGER,
                       image_base INTEGER, size INTEGER, file TEXT, origin TEXT);
CREATE TABLE function_payloads(function_id INTEGER PRIMARY KEY, statements_json TEXT,
                               instructions_json TEXT, locals_json TEXT,
                               skipped_blocks_json TEXT, statement_origin TEXT,
                               instruction_origin TEXT);
"""


def insn(off, text, length=1, label=None):
    out = {"off": off, "len": length, "text": text}
    if label:
        out["label"] = label
    return out


class FixtureDatabases:
    """Two tiny evidence databases in a temporary directory."""

    def __init__(self):
        self.dir = Path(tempfile.mkdtemp())
        self.target = self.dir / "target.sqlite"
        self.base = self.dir / "base.sqlite"
        for path in (self.target, self.base):
            with sqlite3.connect(path) as connection:
                connection.executescript(SCHEMA)
        self.ids = {}

    def add(self, side, name, mangled, rva, statements, instructions, file="a.cpp"):
        path = self.target if side == "target" else self.base
        with sqlite3.connect(path) as connection:
            cursor = connection.execute(
                "INSERT INTO functions(name,mangled,rva,image_base,size,file,origin) "
                "VALUES (?,?,?,?,?,?,'OBSERVED')",
                (name, mangled, rva, 0x10000, sum(s["size"] for s in statements), file))
            connection.execute(
                "INSERT INTO function_payloads VALUES (?,?,?,'[]','[]','OBSERVED','OBSERVED')",
                (cursor.lastrowid, json.dumps(statements), json.dumps(instructions)))

    def cleanup(self):
        shutil.rmtree(self.dir)


class SelectionTests(unittest.TestCase):
    def setUp(self):
        self.db = FixtureDatabases()
        self.addCleanup(self.db.cleanup)
        tick = [stmt(0, 1, 10), stmt(1, 4, 11), stmt(5, 1, 12)]
        ins = [insn(0, "push  ebp"), insn(1, "call  ns::helper"), insn(5, "ret")]
        for side, rva in (("target", 0x100), ("base", 0x200)):
            self.db.add(side, "void ns::thing::tick()", "?tick@thing@ns@@QAEXXZ", rva, tick, ins)
            self.db.add(side, "void ns::thing::tick_all()", "?tick_all@thing@ns@@QAEXXZ",
                        rva + 0x10, tick, ins)
        # An ICF fold: two names on one retail RVA.
        self.db.add("target", "void ns::a::fold()", "?fold@a@ns@@QAEXXZ", 0x300, tick, ins)
        self.db.add("target", "void ns::b::fold()", "?fold@b@ns@@QAEXXZ", 0x300, tick, ins)

    def resolve(self, selector):
        target, base, *connections = select_mod.resolve(selector, self.db.target, self.db.base)
        for connection in connections:
            connection.close()
        return target, base

    def test_exact_mangled_and_partner(self):
        target, base = self.resolve("?tick@thing@ns@@QAEXXZ")
        self.assertEqual((target["rva"], base["rva"]), (0x100, 0x200))

    def test_qualified_tail_beats_longer_substring_hits(self):
        target, _ = self.resolve("thing::tick")
        self.assertEqual(target["mangled"], "?tick@thing@ns@@QAEXXZ")

    def test_ambiguous_substring_lists_candidates(self):
        with self.assertRaises(select_mod.Ambiguous) as caught:
            self.resolve("ns::thing")
        self.assertEqual(len(caught.exception.candidates), 2)

    def test_qualified_tail_alone_is_enough(self):
        self.assertEqual(self.resolve("tick")[0]["mangled"], "?tick@thing@ns@@QAEXXZ")

    def test_icf_aliases_are_one_function(self):
        target, base = self.resolve("fold")
        self.assertEqual(target["rva"], 0x300)
        self.assertEqual(target["aliases"], 2)
        self.assertIsNone(base)

    def test_hex_reads_both_sides(self):
        self.assertEqual(self.resolve("0x100")[0]["rva"], 0x100)
        self.assertEqual(self.resolve("0x10200")[1]["rva"], 0x200)

    def test_unknown_selector(self):
        with self.assertRaises(LookupError):
            self.resolve("nothing_like_it")


class CallsTests(unittest.TestCase):
    def rec(self, *callees):
        return {"instructions": [insn(i, f"call  {c}") for i, c in enumerate(callees)]}

    def test_missing_extra_and_renamed(self):
        calls = gather_mod.calls_section(self.rec("f", "g", "eax", "h"),
                                         self.rec("f", "inl", "g", "edx", "h2"))
        self.assertEqual(calls["missing"], [("inl", 1)])
        self.assertEqual(calls["renamed"], [("h2", "h")])
        self.assertEqual(calls["extra"], [])


class ReportTests(unittest.TestCase):
    """End to end over fixture databases with a stubbed ledger row."""

    def setUp(self):
        self.db = FixtureDatabases()
        self.addCleanup(self.db.cleanup)
        ins = [insn(0, "push  ebp"), insn(1, "mov   ebp, esp"), insn(3, "call  ns::g"),
               insn(8, "pop   ebp"), insn(9, "ret")]
        same = [stmt(0, 3, 10), stmt(3, 5, 11), stmt(8, 2, 12)]
        self.db.add("target", "void ns::same()", "?same@ns@@YAXXZ", 0x100, same, ins)
        self.db.add("base", "void ns::same()", "?same@ns@@YAXXZ", 0x200,
                    [dict(s, source=src) for s, src in zip(same, ("{", "g( );", "}"))], ins)
        retail = [stmt(0, 3, 10), stmt(3, 5, 11), stmt(8, 4, 12), stmt(12, 2, 13)]
        ours = [stmt(0, 3, 10, "{"), stmt(3, 5, 11, "g( );"), stmt(8, 2, 12, "}")]
        self.db.add("target", "void ns::short()", "?short@ns@@YAXXZ", 0x300, retail, ins)
        self.db.add("base", "void ns::short()", "?short@ns@@YAXXZ", 0x400, ours, ins)

    def run_one(self, selector, row, wanted=None):
        with mock.patch.object(select_mod.store, "lookup",
                               return_value={row["mangled"]: row} if row else {}):
            return evidence(selector, wanted or sections(None, None),
                            target_db=self.db.target, base_db=self.db.base)

    def ledger(self, mangled, cur, mx, **extra):
        return {"mangled": mangled, "status": "done" if mx >= 100 else "inprogress", "cls": "",
                "cur": cur, "max": mx, "hist": mx, "tries": 0, "module": "m", "unit": "a.cpp",
                "note": "", "flags": ""} | extra

    def test_full_match_is_one_line_and_rc0(self):
        report = self.run_one("ns::same", self.ledger("?same@ns@@YAXXZ", 100.0, 100.0))
        self.assertEqual(report["rc"], 0)
        text = render.render(report)
        self.assertTrue(text.startswith("MATCH 100%: structure, geometry, flow identical\n"))
        self.assertNotIn("statements", text)

    def test_trgt_only_leads_the_verdict_and_held_is_named(self):
        row = self.ledger("?short@ns@@YAXXZ", 80.0, 90.0, note="tried the temp", tries=2)
        report = self.run_one("ns::short", row)
        self.assertEqual(report["rc"], 1)
        text = render.render(report)
        first = text.splitlines()[0]
        self.assertTrue(first.startswith("STRUCTURE DIFFERS: 1 TRGT_ONLY"), first)
        self.assertIn("cur 80.00, max 90.00 (held)", first)
        self.assertIn("TRGT_ONLY", text)
        self.assertIn("tried the temp", text)
        self.assertNotIn("  =  ", text)
        self.assertIn("=", render.render(report, full=True))

    def test_only_limits_sections(self):
        report = self.run_one("ns::short", None, sections(["structure"], None))
        self.assertIn("structure", report)
        self.assertNotIn("flow", report)
        self.assertNotIn("geometry", report)

    def test_sections_accept_aliases_and_reject_unknown(self):
        self.assertEqual(sections(["blocks,xref"], None), ["flow", "calls"])
        self.assertEqual(sections(None, ["sema"]), ["ledger", "structure", "geometry", "calls"])
        with self.assertRaises(SystemExit):
            sections(["bogus"], None)

    def test_ambiguity_report(self):
        report = self.run_one("ns::s", None)
        self.assertEqual(report["rc"], 2)
        self.assertTrue(report["verdict"].startswith("AMBIGUOUS"))
        self.assertEqual(len(report["candidates"]), 2)


@unittest.skipUnless(TARGET_EVIDENCE.is_file() and BASE_EVIDENCE.is_file()
                     and shutil.which("vostok-pdb") and MATCH_STATE.is_file(),
                     "needs built evidence databases and vostok-pdb")
class AgainstRustTests(unittest.TestCase):
    """The port must tag exactly what `vostok-pdb --view structure-diff` tags."""

    def test_tags_agree_on_ledger_functions(self):
        def signed(n):
            return f"+0x{n:x}" if n >= 0 else f"-0x{-n:x}"

        rows = [line.split("\t", 5) for line in MATCH_STATE.read_text().splitlines()
                if line and not line.startswith(("#", "mangled\t"))]
        picked = [r[0] for r in rows if r[4] in ("QUANTITY", "SIZE", "SPLIT")][:400:20]
        checked = 0
        for mangled in picked:
            try:
                target, base, tc, bc = select_mod.resolve(mangled, TARGET_EVIDENCE, BASE_EVIDENCE)
            except (select_mod.Ambiguous, LookupError):
                continue
            if target is None or base is None:
                continue
            select_mod.payload(tc, target)
            select_mod.payload(bc, base)
            out = subprocess.run(
                ["vostok-pdb", "inspect", "--target", str(TARGET_EVIDENCE), "--base",
                 str(BASE_EVIDENCE), "--rva", hex(target["rva"]), "--view", "structure-diff"],
                capture_output=True, text=True).stdout
            if "ambiguous" in out:
                continue
            lines = out.splitlines()
            rust = []
            if any(line.startswith("b.diff") for line in lines):
                start = next(i for i, line in enumerate(lines) if line.startswith("b.diff")) + 2
                rust = [line.split("|")[0].strip() for line in lines[start:] if line != "}"]
            ours = []
            for r in compare(base, target)["rows"]:
                if r["tags"][0] in ("TRGT_ONLY", "BASE_ONLY"):
                    ours.append(r["tags"][0])
                    continue
                parts = [f"SIZE {signed(r['delta'])}"] if "SIZE" in r["tags"] else []
                parts += [f"OFFSET {signed(r['shift'])}"] if "OFFSET" in r["tags"] else []
                ours.append("; ".join(parts))
            self.assertEqual(ours, rust, mangled)
            checked += 1
        self.assertGreater(checked, 5)


if __name__ == "__main__":
    unittest.main()
