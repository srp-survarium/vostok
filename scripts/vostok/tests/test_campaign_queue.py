# SPDX-License-Identifier: GPL-3.0-or-later

import json
from types import SimpleNamespace
import unittest

from vostok.ledger.campaign_queue import collect, recorded_locals, render


def fixture(module="game", *, pair_class="QUANTITY", fuzzy=75, maximum=75,
            target_locals=(), base_locals=(), paired=True, frameless=False):
    function = SimpleNamespace(
        demangled="void ns::function()", unit=f"vostok/{module}/file.cpp",
        file=f"vostok/{module}/file.cpp", module=module, rva=0x100,
        line=10, size=32, n_stmts=3, frameless=frameless,
    )
    pair = SimpleNamespace(cls=pair_class, fuzzy=fuzzy, base_rva=0x200,
                           t_stmts=3, b_stmts=4)
    roster = SimpleNamespace(
        target={"symbol": function},
        artifacts=SimpleNamespace(target={"symbol": {"locals": list(target_locals)}},
                                  report_fns=[], demangled={}),
        pairing=SimpleNamespace(
            pairs={"symbol": pair} if paired else {},
            base_record=lambda mangled, artifacts: {"locals": list(base_locals)},
        ),
    )
    ledger = {"symbol": {"max": maximum, "hist": 100,
                         "status": "parked", "note": "prior | attempt"}}
    return roster, ledger


class CampaignQueueTests(unittest.TestCase):
    def test_all_engine_modules_are_included(self):
        for module in ("game", "game_core", "render", "new_engine_module"):
            with self.subTest(module=module):
                rows, coverage = collect(*fixture(module))
                self.assertEqual(len(rows), 1)
                self.assertEqual(rows[0]["section"], "structure")
                self.assertEqual(coverage["engine_target_records"], 1)

    def test_vendor_and_unknown_ownership_are_counted_separately(self):
        for module, category in (("boost", "excluded_vendor_or_internal"),
                                 ("", "unknown_module_owner")):
            rows, coverage = collect(*fixture(module))
            self.assertEqual(rows, [])
            self.assertEqual(coverage, {category: 1})

    def test_held_exact_max_goes_last_even_if_current_dipped(self):
        rows, _ = collect(*fixture(fuzzy=50, maximum=100))
        self.assertEqual(rows[0]["section"], "exact-last")
        self.assertEqual(rows[0]["current"], 50)
        self.assertEqual(rows[0]["status"], "parked")
        self.assertEqual(rows[0]["note"], "prior | attempt")

    def test_historical_100_does_not_defer_changed_body(self):
        rows, _ = collect(*fixture(maximum=75))
        self.assertEqual(rows[0]["hist"], 100)
        self.assertEqual(rows[0]["section"], "structure")

    def test_unpaired_frameless_and_zero_statement_procedures_are_not_hidden(self):
        roster, ledger = fixture(paired=False, frameless=True, maximum=None)
        roster.target["symbol"].n_stmts = 0
        rows, _ = collect(roster, ledger)
        self.assertEqual(rows[0]["section"], "presence")
        self.assertEqual(rows[0]["evidence"], ["PRESENCE"])

    def test_locals_are_independent_of_statement_class_and_percentage(self):
        rows, _ = collect(*fixture(
            pair_class="MATCH", fuzzy=100, maximum=100,
            target_locals=[{"name": "result", "ty": "const bool"}],
            base_locals=[{"name": "result", "ty": "bool"}],
        ))
        self.assertEqual(rows[0]["evidence"], ["LOCALS"])
        self.assertEqual(rows[0]["section"], "exact-last")
        self.assertEqual(rows[0]["target_locals_only"][0]["type"], "const bool")

    def test_local_names_multiplicity_and_blocks_survive(self):
        local = {"name": "i", "ty": " u32 ", "scope": 1}
        self.assertEqual(recorded_locals({"locals": [local, local]}),
                         {("i", "u32", "1"): 2})
        rows, _ = collect(*fixture(
            pair_class="MATCH", target_locals=[local],
            base_locals=[{"name": "index", "ty": "u32", "scope": 2}],
        ))
        self.assertEqual(rows[0]["section"], "structure")
        self.assertEqual(rows[0]["base_locals_only"][0]["name"], "index")

    def test_no_byte_or_structure_evidence_is_dropped(self):
        rows, coverage = collect(*fixture(pair_class="MATCH", fuzzy=100, maximum=100))
        self.assertEqual(rows, [])
        self.assertEqual(coverage["exact_without_queue_evidence"], 1)
        rows, _ = collect(*fixture(pair_class="MATCH", fuzzy=75, maximum=75))
        self.assertEqual(rows[0]["evidence"], ["BYTE_ONLY"])
        self.assertEqual(rows[0]["section"], "bytes")

    def test_paired_but_unscored_is_not_claimed_exact(self):
        rows, _ = collect(*fixture(pair_class="MATCH", fuzzy=None, maximum=None))
        self.assertEqual(rows[0]["section"], "presence")
        self.assertEqual(rows[0]["evidence"], ["UNSCORED"])

    def test_render_is_deterministic_and_preserves_full_names(self):
        rows, coverage = collect(*fixture())
        payload = {"snapshot": {"head": "abc", "report_head": "abc",
                                "excluded_modules": ["boost"]},
                   "coverage": coverage, "rows": rows}
        self.assertEqual(render(payload), render(json.loads(json.dumps(payload))))
        output = render(payload)
        self.assertIn("prior &#124; attempt", output)
        self.assertIn("void ns::function() (0x100)", output)
        self.assertIn("Last: structure/presence", output)

    def test_report_only_exact_thunks_keep_coverage_without_fake_structure(self):
        roster, ledger = fixture()
        ledger["thunk"] = dict(ledger["symbol"], module="render", max=100)
        roster.artifacts.report_fns = [("unit", "thunk", 85, 12)]
        rows, coverage = collect(roster, ledger)
        row = next(row for row in rows if row["mangled"] == "thunk")
        self.assertEqual(coverage["engine_ledger_only_records"], 1)
        self.assertEqual(row["section"], "exact-last")
        self.assertEqual(row["current"], 85)
        self.assertEqual(row["evidence"], ["RICH_COVERAGE"])
        self.assertIsNone(row["target_rva"])
        self.assertIsNone(row["target_statements"])
        payload = {"snapshot": {"head": "abc", "report_head": "abc",
                                "excluded_modules": []},
                   "coverage": coverage, "rows": rows}
        self.assertIn("thunk (no PDB RVA)", render(payload))

    def test_unmeasured_ledger_only_rows_are_explicit_coverage_work(self):
        roster, ledger = fixture()
        ledger["missing"] = dict(ledger["symbol"], module="animation", max=75)
        rows, _ = collect(roster, ledger)
        row = next(row for row in rows if row["mangled"] == "missing")
        self.assertEqual(row["section"], "presence")
        self.assertEqual(row["evidence"], ["RICH_COVERAGE", "LEDGER_ONLY"])
        self.assertIsNone(row["current"])


if __name__ == "__main__":
    unittest.main()
