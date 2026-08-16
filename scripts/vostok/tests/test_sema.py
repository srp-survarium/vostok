import unittest

from vostok.sema.cfg import contract, seal
from vostok.sema.index import _fold_aliases
from vostok.sema.strings import _decode_literal
from vostok.sema.xref import _call_operand, _qualified_name


def _rec(rva, mangled):
    return {"rva": rva, "mangled": mangled, "name": mangled}


class FoldAliasTests(unittest.TestCase):
    def test_one_rva_many_names_is_one_function(self):
        hits = [_rec(0x86200, "??1game_scene@survarium@@QAE@XZ"),
                _rec(0x86200, "??1?$storage3@X@_bi@boost@@QAE@XZ")]
        self.assertEqual(len(_fold_aliases("target", hits, "")), 1)

    def test_prefers_the_selected_symbol_of_a_fold_group(self):
        hits = [_rec(0x86200, "??1?$storage3@X@_bi@boost@@QAE@XZ"),
                _rec(0x86200, "??1game_scene@survarium@@QAE@XZ")]
        kept = _fold_aliases("target", hits, "??1game_scene@survarium@@QAE@XZ")
        self.assertEqual([r["mangled"] for r in kept],
                         ["??1game_scene@survarium@@QAE@XZ"])

    def test_distinct_rvas_stay_ambiguous(self):
        hits = [_rec(0x1000, "?a@@YAXXZ"), _rec(0x2000, "?b@@YAXXZ")]
        self.assertEqual(len(_fold_aliases("target", hits, "")), 2)


class SealTests(unittest.TestCase):
    """A trailing trim can leave a terminator naming a block that is gone."""

    def test_dangling_destination_becomes_ext(self):
        blocks = [(0, ["cmp eax, 1"], "jcc B7 | fall B1", ""),
                  (4, ["ret"], "ret", "")]
        self.assertEqual([b[2] for b in seal(blocks)],
                         ["jcc <ext> | fall B1", "ret"])

    def test_live_destination_and_back_edge_marker_survive(self):
        blocks = [(0, ["nop"], "fall B1", ""),
                  (1, ["cmp eax, 1"], "jcc B0^ | fall B2", ""),
                  (5, ["jmp"], "jmp B9", "")]
        self.assertEqual([b[2] for b in seal(blocks)],
                         ["fall B1", "jcc B0^ | fall B2", "jmp <ext>"])

    def test_contract_survives_a_sealed_graph(self):
        # unsealed, this raised KeyError in _renumber's remap
        blocks = [(0, ["cmp eax, 1"], "jcc B9 | fall B1", ""),
                  (4, ["mov eax, 0"], "fall B2", ""),
                  (8, ["ret"], "ret", "")]
        out, n = contract(seal(blocks))
        self.assertEqual(n, 1)
        self.assertEqual([b[2] for b in out], ["jcc <ext> | fall B1", "ret"])


class SemaNavigationTests(unittest.TestCase):
    def test_decodes_msvc_literal_prefix(self):
        self.assertEqual(
            _decode_literal("??_C@_0N@BHGPFDAK@resources?1?$CFs?$AA@"),
            "resources/%s",
        )
        self.assertEqual(
            _decode_literal("??_C@_07INEOAPOI@render?3?$AA@"),
            "render:",
        )

    def test_qualified_name_drops_return_and_arguments(self):
        self.assertEqual(
            _qualified_name("void vostok::render::device::on_device_removed()"),
            "vostok::render::device::on_device_removed",
        )

    def test_call_operand_rejects_non_calls(self):
        self.assertEqual(_call_operand("call  vostok::logging::append"),
                         "vostok::logging::append")
        self.assertIsNone(_call_operand("jmp short .1"))


if __name__ == "__main__":
    unittest.main()
