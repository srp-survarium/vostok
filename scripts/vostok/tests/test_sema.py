# SPDX-License-Identifier: GPL-3.0-or-later
import json
import unittest

from vostok.derive import set_quiet
from vostok.derive.index import index_by_mangled, overload_key
from vostok.derive.pairing import pair as derive_pair
from vostok.sema.cfg import contract, notes, seal, starved
from vostok.derive.inventory import unit_of
from vostok.sema.index import _fold_aliases, _matcher, va_of
from vostok.sema.pairing import _Inputs, ledger_row
from vostok.sema.strings import _decode_literal
from vostok.sema.xref import _call_operand, _qualified_name


def _rec(rva, mangled):
    return {"rva": rva, "mangled": mangled, "name": mangled}


def _full(rva, mangled, name=None, file="vostok/x.cpp"):
    """A rich-index record with everything the pairing passes look at."""
    return {"rva": rva, "mangled": mangled, "name": name or mangled,
            "file": file, "image_base": 0x10000, "size": 4,
            "statements": [{"off": 0, "size": 4, "line": 7}],
            "instructions": [{"off": 0, "len": 4, "text": "ret"}]}


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


class TrimNoteTests(unittest.TestCase):
    def test_a_starved_graph_is_flagged(self):
        cut = {"base": {"kept": 1, "trimmed": 73}, "target": {"kept": 1, "trimmed": 73}}
        self.assertTrue(starved(cut))
        self.assertTrue(any("do NOT read it" in n for n in notes(cut)))

    def test_an_ordinary_trim_is_reported_but_not_flagged(self):
        cut = {"base": {"kept": 14, "trimmed": 3}, "target": {"kept": 14, "trimmed": 4}}
        self.assertFalse(starved(cut))
        self.assertTrue(any("trimmed 3 base / 4 target" in n for n in notes(cut)))

    def test_an_untrimmed_graph_says_nothing(self):
        cut = {"base": {"kept": 48, "trimmed": 0, "contracted": 0},
               "target": {"kept": 48, "trimmed": 0, "contracted": 0}}
        self.assertEqual(notes(cut), [])


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


class AddressSpaceTests(unittest.TestCase):
    """RVA and VA differ by the image base, and sema must never conflate them."""

    def test_va_is_rva_plus_image_base(self):
        self.assertEqual(va_of({"rva": 0x6243E0, "image_base": 0x10000}), 0x6343E0)

    def test_a_record_without_an_image_base_reads_as_its_own_va(self):
        self.assertEqual(va_of({"rva": 0x1000}), 0x1000)


class PairingTests(unittest.TestCase):
    """The pairing that replaced match.db's `pairs` table."""

    def _pair_of(self, target_records, base_records, scored=()):
        """Pair through the same call sema makes, adapter included."""
        set_quiet()
        target = index_by_mangled(target_records)
        base = index_by_mangled(base_records)
        pairing = derive_pair(_Inputs(
            target, base, target_records, base_records,
            {mangled: 100.0 for mangled in scored},
        ))
        return {key: (p.target_rva, p.base_rva) for key, p in pairing.pairs.items()}

    def test_identical_mangled_names_pair(self):
        t = [_full(0x1000, "?f@@YAXXZ")]
        b = [_full(0x2000, "?f@@YAXXZ")]
        self.assertEqual(self._pair_of(t, b), {"?f@@YAXXZ": (0x1000, 0x2000)})

    def test_unscored_primary_pair_accepts_icf_aliased_call_operands(self):
        caller = "?caller@@YAXXZ"
        target_caller = dict(
            _full(0x1000, caller, name="void caller()"),
            size=5,
            instructions=[{"off": 0, "len": 5, "text": "call  target_alias"}],
        )
        base_caller = dict(
            _full(0x2000, caller, name="void caller()"),
            size=5,
            instructions=[{"off": 0, "len": 5, "text": "call  base_alias"}],
        )
        target_records = [
            target_caller,
            _full(0x1100, "?target_alias@@YAXXZ", name="void target_alias()"),
            _full(0x1100, "?shared_alias@@YAXXZ", name="void shared_alias()"),
        ]
        base_records = [
            base_caller,
            _full(0x2100, "?base_alias@@YAXXZ", name="void base_alias()"),
            _full(0x2100, "?shared_alias@@YAXXZ", name="void shared_alias()"),
        ]
        pairing = derive_pair(_Inputs(
            index_by_mangled(target_records),
            index_by_mangled(base_records),
            target_records,
            base_records,
            {},
        ))

        self.assertEqual(pairing.pairs[caller].fuzzy, 100.0)

    def test_unscored_primary_pair_accepts_icf_aliased_mov_relocations(self):
        caller = "?caller@@YAXXZ"
        target_caller = dict(
            _full(0x1000, caller, name="void caller()"),
            size=5,
            instructions=[{"off": 0, "len": 5, "text": "mov   ecx, target_alias"}],
        )
        base_caller = dict(
            _full(0x2000, caller, name="void caller()"),
            size=5,
            instructions=[{"off": 0, "len": 5, "text": "mov   ecx, base_alias"}],
        )
        target_records = [
            target_caller,
            _full(0x1100, "?target_alias@@YAXXZ", name="void target_alias()"),
            _full(0x1100, "?shared_alias@@YAXXZ", name="void shared_alias()"),
        ]
        base_records = [
            base_caller,
            _full(0x2100, "?base_alias@@YAXXZ", name="void base_alias()"),
            _full(0x2100, "?shared_alias@@YAXXZ", name="void shared_alias()"),
        ]
        pairing = derive_pair(_Inputs(
            index_by_mangled(target_records),
            index_by_mangled(base_records),
            target_records,
            base_records,
            {},
        ))

        self.assertEqual(pairing.pairs[caller].fuzzy, 100.0)

    def test_icf_call_alias_evidence_refuses_an_ambiguous_operand(self):
        caller = "?caller@@YAXXZ"
        target_caller = dict(
            _full(0x1000, caller, name="void caller()"),
            size=5,
            instructions=[{"off": 0, "len": 5, "text": "call  target_alias"}],
        )
        base_caller = dict(
            _full(0x2000, caller, name="void caller()"),
            size=5,
            instructions=[{"off": 0, "len": 5, "text": "call  base_alias"}],
        )
        target_records = [
            target_caller,
            _full(0x1100, "?target_alias1@@YAXXZ", name="void target_alias()"),
            _full(0x1100, "?shared_alias@@YAXXZ", name="void shared_alias()"),
            _full(0x1200, "?target_alias2@@YAXXZ", name="void target_alias()"),
        ]
        base_records = [
            base_caller,
            _full(0x2100, "?base_alias@@YAXXZ", name="void base_alias()"),
            _full(0x2100, "?shared_alias@@YAXXZ", name="void shared_alias()"),
        ]
        pairing = derive_pair(_Inputs(
            index_by_mangled(target_records),
            index_by_mangled(base_records),
            target_records,
            base_records,
            {},
        ))

        self.assertIsNone(pairing.pairs[caller].fuzzy)

    def test_a_dynamic_initializer_pairs_across_the_namespace_gap(self):
        # the retail PDB puts the namespace OUTSIDE the quotes and ours puts it
        # inside; ~589 pairs exist only because this is reconciled, and they are
        # what used to make the match.db lookup unavoidable.
        t = [_full(0x1000, "vostok::render::`dynamic initializer for 's_cc''")]
        b = [_full(0x2000, "`dynamic initializer for 'vostok::render::s_cc''")]
        self.assertEqual(
            self._pair_of(t, b),
            {"vostok::render::`dynamic initializer for 's_cc''": (0x1000, 0x2000)})

    def test_a_dynamic_initializer_selects_the_same_source_owner(self):
        target_name = "vostok::render::`dynamic initializer for 's_world''"
        base_name = "`dynamic initializer for 'vostok::render::s_world''"
        owner = "vostok/render/engine/sources/world.cpp"
        t = [_full(0x1000, target_name, file=owner)]
        b = [
            _full(0x2000, base_name, file="vostok/render/facade/sources/world.cpp"),
            _full(0x2100, base_name, file=owner),
        ]
        self.assertEqual(
            self._pair_of(t, b),
            {target_name: (0x1000, 0x2100)},
        )

    def test_an_ambiguous_canonical_owner_is_left_unpaired(self):
        # two target spellings claim the same canonical owner, so neither is
        # proven to be the base's `??__Es@a@@YAXXZ` - refuse rather than guess
        t = [_full(0x1000, "a::`dynamic initializer for 's''"),
             _full(0x1100, "`dynamic initializer for 'a::s''", file="vostok/y.cpp")]
        b = [_full(0x2000, "??__Es@a@@YAXXZ")]
        self.assertEqual(self._pair_of(t, b), {})

    def test_an_unscored_icf_alias_needs_an_identical_body(self):
        # same signature, different owner and a different body: not a pair
        t = [_full(0x1000, "?f@@YAXXZ", name="void f()")]
        b = [dict(_full(0x2000, "?g@@YAXXZ", name="void f()", file="vostok/y.cpp"),
                  size=8)]
        self.assertEqual(self._pair_of(t, b), {})

    def test_a_local_predicate_pairs_across_scope_ordinal_drift(self):
        target_mangled = "`a::build'::`9'::predicate::operator()"
        base_mangled = "`a::build'::`2'::predicate::operator()"
        target_name = "bool `a::build'::`9'::predicate::operator()() const"
        base_name = "bool `a::build'::`2'::predicate::operator()() const"
        t = [_full(0x1000, target_mangled, name=target_name)]
        b = [_full(0x2000, base_mangled, name=base_name)]
        self.assertEqual(
            self._pair_of(t, b),
            {target_mangled: (0x1000, 0x2000)},
        )


class DynamicInitializerNameTests(unittest.TestCase):
    """Either spelling of a dynamic initializer must find the same function.

    The retail PDB has no mangled name for these and stores the display form;
    report.json and our base PDB use `??__E...`. Neither contains the other, so
    a name copied out of one used to find nothing in the other - the tool said
    "no function matches" for a function it held.
    """

    RICH = "survarium::`dynamic initializer for 's_cc''"
    MSVC = "??__Es_cc@survarium@@YAXXZ"

    def test_the_msvc_spelling_matches_the_rich_record(self):
        want = _matcher(self.MSVC)
        rec = want(json.dumps(_full(0x1000, self.RICH)))
        self.assertIsNotNone(rec)
        self.assertEqual(rec["mangled"], self.RICH)

    def test_the_rich_spelling_matches_the_msvc_record(self):
        want = _matcher(self.RICH)
        rec = want(json.dumps(_full(0x2000, self.MSVC)))
        self.assertIsNotNone(rec)

    def test_an_atexit_thunk_does_not_match_an_initializer(self):
        want = _matcher("??__Fs_cc@survarium@@YAXXZ")
        self.assertIsNone(want(json.dumps(_full(0x1000, self.RICH))))

    def test_an_unrelated_name_is_still_no_match(self):
        want = _matcher(self.MSVC)
        self.assertIsNone(want(json.dumps(_full(0x3000, "?f@@YAXXZ"))))


class UnitFallbackTests(unittest.TestCase):
    """A record whose mangled spelling report.json does not share still gets
    filed under the file the PDB attributes it to - 1,765 functions, 799 of them
    dynamic initializers, were filed under no unit at all and `--unit` could not
    see them."""

    def test_report_unit_wins_when_it_knows_the_symbol(self):
        rec = {"file": "vostok/game/sources/player.cpp"}
        self.assertEqual(unit_of(rec, ["vostok/game/sources/player.cpp"]),
                         "vostok/game/sources/player.cpp")

    def test_falls_back_to_the_records_own_file(self):
        rec = {"file": "vostok/game/sources/player.cpp"}
        self.assertEqual(unit_of(rec, None), "vostok/game/sources/player.cpp")

    def test_no_file_and_no_unit_is_still_none(self):
        self.assertIsNone(unit_of({}, None))


class LedgerRowTests(unittest.TestCase):
    """A PDB placeholder overload owns a signature-keyed row, not the plain one."""

    def test_the_signature_key_wins_when_the_ledger_has_it(self):
        rec = _full(0x1000, "process", name="void a::process()")
        key = overload_key("process", "void a::process()")
        rows = {"process": {"cur": 1.0}, key: {"cur": 2.0}}
        self.assertEqual(ledger_row(rec, rows), {"cur": 2.0})

    def test_the_plain_mangled_row_is_the_fallback(self):
        rec = _full(0x1000, "process", name="void a::process()")
        rows = {"process": {"cur": 1.0}}
        self.assertEqual(ledger_row(rec, rows), {"cur": 1.0})


if __name__ == "__main__":
    unittest.main()
