import importlib.util
import json
import sqlite3
import tempfile
import unittest
from pathlib import Path
from unittest import mock

from vostok.core import symbols as NORMALIZE

SPEC = importlib.util.spec_from_file_location(
    "vostok_match_db", Path(__file__).with_name("match_db.py")
)
MATCH_DB = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(MATCH_DB)


class CompilerNameTests(unittest.TestCase):
    def test_normalizes_render_local_type_scope_aliases(self):
        target = (
            "vostok::render::create_shadow_pass_geometry_type<"
            "`vostok::render::static_render_surface::create_shadow_pass_geometry'::"
            "`2'::static_vertex0,"
            "`vostok::render::static_render_surface::create_shadow_pass_geometry'::"
            "`2'::opt_static_vertex>"
        )
        base = (
            "vostok::render::create_shadow_pass_geometry_type<"
            "`vostok::render::static_render_surface::create_shadow_pass_geometry'::"
            "`2'::static_vertex0,"
            "`vostok::render::static_render_surface::opt_static_vertex'::"
            "`2'::opt_static_vertex>"
        )

        self.assertEqual(NORMALIZE.compiler_name(target), base)

    def test_derives_only_unique_same_owner_rich_pdb_aliases(self):
        with tempfile.TemporaryDirectory() as temporary_directory:
            root = Path(temporary_directory)
            target_index = root / "target.jsonl"
            base_index = root / "base.jsonl"
            target_records = [
                {
                    "file": "vostok/render/core/dx11/sources/custom_config.cpp",
                    "name": "void vostok::render::construct<int>(int)",
                    "mangled": "vostok::render::construct<int>",
                },
                {
                    "file": "first.cpp",
                    "name": "void duplicate<int>(int)",
                    "mangled": "duplicate<int>",
                },
                {
                    "file": "second.cpp",
                    "name": "void duplicate<float>(float)",
                    "mangled": "duplicate<int>",
                },
                {
                    "file": "third.cpp",
                    "name": "void alias_one()",
                    "mangled": "alias_one",
                },
                {
                    "file": "third.cpp",
                    "name": "void alias_two()",
                    "mangled": "alias_two",
                },
            ]
            base_records = [
                {
                    "file": "vostok/render/core/dx11/sources/custom_config.cpp",
                    "name": "void vostok::render::construct<int>(int)",
                    "mangled": "??$construct@H@render@vostok@@YAXH@Z",
                },
                {
                    "file": "first.cpp",
                    "name": "void duplicate<int>(int)",
                    "mangled": "??$duplicate@H@@YAXH@Z",
                },
                {
                    "file": "second.cpp",
                    "name": "void duplicate<float>(float)",
                    "mangled": "??$duplicate@M@@YAXM@Z",
                },
                {
                    "file": "third.cpp",
                    "name": "void alias_one()",
                    "mangled": "?shared_alias@@YAXXZ",
                },
                {
                    "file": "third.cpp",
                    "name": "void alias_two()",
                    "mangled": "?shared_alias@@YAXXZ",
                },
            ]
            target_index.write_text(
                "".join(json.dumps(record) + "\n" for record in target_records)
            )
            base_index.write_text(
                "".join(json.dumps(record) + "\n" for record in base_records)
            )

            aliases = NORMALIZE.rich_pdb_aliases(target_index, base_index)

        self.assertEqual(
            aliases,
            {
                "vostok::render::construct<int>":
                    "??$construct@H@render@vostok@@YAXH@Z"
            },
        )


class IndexByMangledTests(unittest.TestCase):
    def record(self, name, rva, file="vostok/sound/sources/sound_scene.cpp"):
        return {
            "mangled": "vostok::sound::fill_x3daudio_vector",
            "name": name,
            "rva": rva,
            "file": file,
        }

    def test_preserves_distinct_signatures_hidden_by_pdb_placeholder(self):
        first = self.record(
            "fill_x3daudio_vector(vector&, float, float, float)", 0x1000
        )
        second = self.record("fill_x3daudio_vector(vector&, float3 const&)", 0x1020)

        indexed = MATCH_DB.index_by_mangled([first, second])

        self.assertEqual(len(indexed), 2)
        self.assertIs(indexed[first["mangled"]], first)
        self.assertIs(
            indexed[MATCH_DB.overload_key(first["mangled"], second["name"])], second
        )

    def test_uses_target_primary_signature_when_rva_order_differs(self):
        target_first = self.record("fill_surface(target*, context*)", 0x1000)
        target_second = self.record("fill_surface(target*, context*, bool)", 0x1020)
        base_second = self.record(target_second["name"], 0x2000)
        base_first = self.record(target_first["name"], 0x2020)

        indexed = MATCH_DB.index_by_mangled(
            [base_second, base_first],
            preferred_signatures={target_first["mangled"]: target_first["name"]},
        )

        self.assertIs(indexed[target_first["mangled"]], base_first)
        self.assertIs(
            indexed[MATCH_DB.overload_key(target_first["mangled"], target_second["name"])],
            base_second,
        )

    def test_maps_legacy_rva_key_to_signature_key(self):
        first = self.record("fill_surface(target*, context*)", 0x1000)
        second = self.record("fill_surface(target*, context*, bool)", 0x1020)

        aliases = MATCH_DB.legacy_overload_keys([first, second])

        self.assertEqual(
            aliases[f"{first['mangled']}@@pdb-overload:1020"],
            MATCH_DB.overload_key(first["mangled"], second["name"]),
        )

    def test_still_collapses_distinct_same_rva_aliases(self):
        first = self.record("first alias()", 0x1000)
        second = self.record("second alias()", 0x1000)

        indexed = MATCH_DB.index_by_mangled([first, second])

        self.assertEqual(indexed, {first["mangled"]: first})

    def test_still_collapses_same_signature_aliases_to_preferred_owner(self):
        first = self.record("static helper()", 0x1000, "first.cpp")
        preferred = self.record("static helper()", 0x2000, "preferred.cpp")

        indexed = MATCH_DB.index_by_mangled(
            [first, preferred], {first["mangled"]: "preferred.cpp"}
        )

        self.assertEqual(indexed, {first["mangled"]: preferred})


class AuthoritativeDemangledNamesTests(unittest.TestCase):
    def test_target_name_wins_when_base_fold_alias_reuses_mangled_name(self):
        mangled = "?animation@animation_interval@@QBEABVresource_ptr@@XZ"
        target = {mangled: {"name": "animation_interval::animation() const"}}
        base = {mangled: {"name": "collision_geometry::cast_to_collision_geometry()"}}

        self.assertEqual(
            MATCH_DB.authoritative_demangled_names(target, base)[mangled],
            target[mangled]["name"],
        )

    def test_base_only_symbol_keeps_base_name(self):
        mangled = "?base_only@@YAXXZ"
        base = {mangled: {"name": "base_only()"}}

        self.assertEqual(
            MATCH_DB.authoritative_demangled_names({}, base)[mangled],
            base[mangled]["name"],
        )


class InstructionStreamExactTests(unittest.TestCase):
    def record(self, size=6, text="call  Scaleform::Render::HAL::EndFrame"):
        return {
            "size": size,
            "instructions": [{"off": 0, "len": size, "text": text}],
        }

    def test_accepts_identical_symbolic_instruction_stream(self):
        self.assertTrue(
            MATCH_DB.instruction_stream_exact(self.record(), self.record())
        )

    def test_rejects_different_instruction_or_size(self):
        self.assertFalse(
            MATCH_DB.instruction_stream_exact(
                self.record(), self.record(text="call  Scaleform::Render::HAL::BeginFrame")
            )
        )
        self.assertFalse(
            MATCH_DB.instruction_stream_exact(self.record(), self.record(size=5))
        )

    def test_rejects_missing_instruction_evidence(self):
        empty = {"size": 0, "instructions": []}
        self.assertFalse(MATCH_DB.instruction_stream_exact(empty, empty))

    def test_accepts_equivalent_operator_delete_pdb_spellings(self):
        target = self.record(text="call  operator delete")
        base = self.record(text="call  ??3@YAXPAX@Z")
        self.assertTrue(MATCH_DB.instruction_stream_exact(target, base))

    def test_accepts_equivalent_template_closing_whitespace(self):
        target = self.record(text="call  pinned_ptr<animation const >::operator=")
        base = self.record(text="call  pinned_ptr<animation const>::operator=")
        self.assertTrue(MATCH_DB.instruction_stream_exact(target, base))

    def test_island_exact_stream_overrides_stale_report_pair(self):
        mangled = "??4bone_matrices_computer_data@@"
        target = self.record(text="movq  xmm0, [eax]")
        candidate = self.record(text="movq  xmm0, [eax]")

        self.assertEqual(
            MATCH_DB.island_candidate_score(
                {}, mangled, {mangled: 59.464287}, target, candidate
            ),
            100.0,
        )

    def test_island_nonexact_stream_keeps_report_score(self):
        mangled = "??4bone_matrices_computer_data@@"
        target = self.record(text="movq  xmm0, [eax]")
        candidate = self.record(text="movq  xmm1, [eax]")

        self.assertEqual(
            MATCH_DB.island_candidate_score(
                {}, mangled, {mangled: 59.464287}, target, candidate
            ),
            59.464287,
        )


class ReportFuzzyScoreTests(unittest.TestCase):
    def test_keeps_best_duplicate_score_and_sorted_units(self):
        report = {
            "units": [
                {
                    "name": "vostok/animation/z.cpp",
                    "functions": [
                        {"name": "?fn@@", "fuzzy_match_percent": 75.0},
                        {"name": "?none@@"},
                    ],
                },
                {
                    "name": "vostok/animation/a.cpp",
                    "functions": [
                        {"name": "?fn@@", "fuzzy_match_percent": 100.0},
                    ],
                },
            ]
        }

        scores, units = MATCH_DB.report_fuzzy_scores(report)

        self.assertEqual(scores, {"?fn@@": 100.0})
        self.assertEqual(
            units["?fn@@"],
            ["vostok/animation/a.cpp", "vostok/animation/z.cpp"],
        )
        self.assertEqual(units["?none@@"], ["vostok/animation/z.cpp"])

    def test_maps_scalar_deleting_destructor_to_vector_report_name(self):
        scalar = "??_Gexample@@UAEPAXI@Z"
        vector = "??_Eexample@@UAEPAXI@Z"

        self.assertEqual(
            MATCH_DB.report_score_for_target(scalar, {vector: 88.5}), 88.5
        )

    def test_maps_folded_size_calculator_template_to_report_name(self):
        pdb_name = (
            "??$propagate@Vn_ary_tree_multiplication_node@mixing@animation@vostok@@"
            "@n_ary_tree_size_calculator@mixing@animation@vostok@@"
            "AAEXAAVn_ary_tree_multiplication_node@123@@Z"
        )
        report_name = (
            "??$propagate@Vn_ary_tree_addition_node@mixing@animation@vostok@@"
            "@n_ary_tree_size_calculator@mixing@animation@vostok@@"
            "AAEXAAVn_ary_tree_addition_node@123@@Z"
        )

        self.assertEqual(
            MATCH_DB.report_score_for_target(pdb_name, {report_name: 90.13513}),
            90.13513,
        )

    def test_uses_fuzzy_match_percent_not_unrelated_match_percent(self):
        report = {
            "units": [
                {
                    "name": "vostok/animation/example.cpp",
                    "functions": [
                        {
                            "name": "?fn@@",
                            "match_percent": 100.0,
                            "fuzzy_match_percent": 42.5,
                        }
                    ],
                }
            ]
        }

        scores, _units = MATCH_DB.report_fuzzy_scores(report)

        self.assertEqual(scores, {"?fn@@": 42.5})

    def test_cross_unit_exact_maps_scalar_pdb_to_vector_coff_destructor(self):
        scalar = "??_Gskeleton@animation@vostok@@UAEPAXI@Z"
        vector = "??_Eskeleton@animation@vostok@@UAEPAXI@Z"

        self.assertEqual(
            MATCH_DB.cross_unit_exact_score(scalar, {vector: 100.0}),
            100.0,
        )

    def test_cross_unit_exact_accepts_same_name_and_rejects_partial_score(self):
        mangled = "??0expression@mixing@animation@vostok@@QAE@ABV0123@@Z"

        self.assertEqual(
            MATCH_DB.cross_unit_exact_score(mangled, {mangled: 100.0}),
            100.0,
        )
        self.assertIsNone(
            MATCH_DB.cross_unit_exact_score(mangled, {mangled: 99.0})
        )

    def test_island_report_score_accepts_explicit_reviewed_alias(self):
        target = "?create_resource_inplace@@"
        report_name = "?create_resource@@"

        self.assertEqual(
            MATCH_DB.island_report_score(
                {"report_mangled": report_name},
                target,
                {report_name: 92.88461},
            ),
            92.88461,
        )
        self.assertEqual(
            MATCH_DB.island_report_score({}, target, {target: 75.0}),
            75.0,
        )


class RankIslandDeltaTests(unittest.TestCase):
    def test_discovers_target_only_candidate_from_zero_floor(self):
        delta = MATCH_DB.rank_island_delta(
            87.5,
            "source-hash",
            previous=None,
            current_fuzzy=None,
        )

        self.assertEqual(delta["previous_fuzzy_pct"], None)
        self.assertEqual(delta["baseline_fuzzy_pct"], 0.0)
        self.assertEqual(delta["gain_pct"], 87.5)
        self.assertEqual(delta["exact_proven"], 0)

    def test_rejects_would_be_gain_from_stale_source_epoch(self):
        previous = {
            "effective_hash": "current-hash",
            "max_fuzzy_pct": 50.0,
            "exact_proven": 0,
        }

        with self.assertRaisesRegex(ValueError, "source epoch disagrees"):
            MATCH_DB.rank_island_delta(
                75.0,
                "stale-hash",
                previous=previous,
                current_fuzzy=50.0,
            )

    def test_ignores_non_improving_stale_observation(self):
        previous = {
            "effective_hash": "current-hash",
            "max_fuzzy_pct": 75.0,
            "exact_proven": 0,
        }

        self.assertIsNone(
            MATCH_DB.rank_island_delta(
                60.0,
                "stale-hash",
                previous=previous,
                current_fuzzy=75.0,
            )
        )


class MaximumEpochArchiveTests(unittest.TestCase):
    @staticmethod
    def row(fuzzy, exact=0, origin="rebuild", evidence=None):
        return (
            "?function@@",
            "effective-hash",
            fuzzy,
            exact,
            "state-id",
            "animation",
            "vostok/animation/example.cpp",
            10,
            12,
            origin,
            evidence,
        )

    def test_plain_current_rebuild_does_not_grow_epoch_archive(self):
        self.assertFalse(
            MATCH_DB.maximum_needs_epoch_archive(self.row(75.0), 75.0)
        )

    def test_island_and_raised_rebuild_are_archived(self):
        self.assertTrue(
            MATCH_DB.maximum_needs_epoch_archive(
                self.row(100.0, exact=1, origin="island", evidence="island.json"),
                75.0,
            )
        )
        self.assertTrue(
            MATCH_DB.maximum_needs_epoch_archive(self.row(80.0), 75.0)
        )

    def test_epoch_merge_retains_strongest_proof(self):
        weaker = self.row(80.0)
        exact = self.row(100.0, exact=1, origin="island", evidence="island.json")

        merged = MATCH_DB.merge_maximum_epoch(weaker, exact)

        self.assertEqual(merged[2], 100.0)
        self.assertEqual(merged[3], 1)
        self.assertEqual(merged[9], "island")
        self.assertEqual(merged[10], "island.json")

    def test_epoch_merge_rejects_different_hash(self):
        other = list(self.row(80.0))
        other[1] = "other-hash"

        with self.assertRaisesRegex(ValueError, "different source MAX epochs"):
            MATCH_DB.merge_maximum_epoch(self.row(75.0), tuple(other))

    def test_matching_archived_hash_reactivates_after_newer_epoch(self):
        archived = self.row(
            100.0, exact=1, origin="island", evidence="island.json"
        )
        newer = list(self.row(60.0))
        newer[1] = "newer-hash"

        selected = MATCH_DB.maximum_for_effective_hash(
            "?function@@",
            "effective-hash",
            {"?function@@": tuple(newer)},
            {("?function@@", "effective-hash"): archived},
        )

        self.assertEqual(selected, archived)

    def test_different_archived_hash_is_not_credited(self):
        archived = self.row(100.0, exact=1, origin="island")

        selected = MATCH_DB.maximum_for_effective_hash(
            "?function@@",
            "unseen-hash",
            {},
            {("?function@@", "effective-hash"): archived},
        )

        self.assertIsNone(selected)


class MergePersistentMaximaTests(unittest.TestCase):
    @staticmethod
    def row(mangled, effective_hash, fuzzy, exact=0):
        return (
            mangled,
            effective_hash,
            fuzzy,
            exact,
            "state-id",
            "render",
            "vostok/render/example.cpp",
            10,
            12,
            "rebuild",
            None,
        )

    def test_merges_same_hash_and_archives_different_hash(self):
        current = sqlite3.connect(":memory:")
        incoming = sqlite3.connect(":memory:")
        current.row_factory = incoming.row_factory = sqlite3.Row
        current.executescript(MATCH_DB.SCHEMA)
        incoming.executescript(MATCH_DB.SCHEMA)

        current.execute(
            "INSERT INTO source_maxima VALUES (?,?,?,?,?,?,?,?,?,?,?)",
            self.row("?same@@", "same-hash", 80.0),
        )
        incoming.execute(
            "INSERT INTO source_maxima VALUES (?,?,?,?,?,?,?,?,?,?,?)",
            self.row("?same@@", "same-hash", 100.0, exact=1),
        )
        current.execute(
            "INSERT INTO source_maxima VALUES (?,?,?,?,?,?,?,?,?,?,?)",
            self.row("?changed@@", "old-hash", 75.0),
        )
        incoming.execute(
            "INSERT INTO source_maxima VALUES (?,?,?,?,?,?,?,?,?,?,?)",
            self.row("?changed@@", "new-hash", 100.0, exact=1),
        )

        self.assertEqual(MATCH_DB.merge_persistent_maxima(current, incoming), 2)
        self.assertEqual(
            tuple(
                current.execute(
                    "SELECT max_fuzzy_pct, exact_proven FROM source_maxima "
                    "WHERE mangled = '?same@@'"
                ).fetchone()
            ),
            (100.0, 1),
        )
        self.assertEqual(
            current.execute(
                "SELECT max_fuzzy_pct FROM source_maxima_epochs "
                "WHERE mangled = '?changed@@' AND effective_hash = 'new-hash'"
            ).fetchone()[0],
            100.0,
        )


class EffectiveSourceHashTests(unittest.TestCase):
    def test_hash_ignores_other_functions_in_the_same_tu(self):
        """A sibling edit must NOT reset this function's banked max.

        The hash used to fold in the owning module's headers and the whole
        owning .cpp, so any sibling edit reset max for every function in the TU
        (137 of them in render_engine_world_pc_dx11.cpp). That left 16,659 of
        16,661 rows with max == cur. The gate is the function body alone;
        context is diagnosis, not an eraser.
        """
        with tempfile.TemporaryDirectory() as temporary_directory:
            root = Path(temporary_directory)
            source = root / "sources/vostok/animation/sources/example.cpp"
            source.parent.mkdir(parents=True)
            source.write_text("body line\nother function v1\n", encoding="latin-1")
            record = {
                "file": "vostok/animation/sources/example.cpp",
                "statements": [{"line": 1}],
            }

            with mock.patch.object(MATCH_DB, "SOURCES", root / "sources"):
                first = MATCH_DB.effective_source_hash(record, "animation")
                source.write_text(
                    "body line\nother function v2\n", encoding="latin-1"
                )
                second = MATCH_DB.effective_source_hash(record, "animation")

            self.assertEqual(first, second)

    def test_hash_changes_when_the_function_body_changes(self):
        with tempfile.TemporaryDirectory() as temporary_directory:
            root = Path(temporary_directory)
            source = root / "sources/vostok/animation/sources/example.cpp"
            source.parent.mkdir(parents=True)
            source.write_text("body line\nother function\n", encoding="latin-1")
            record = {
                "file": "vostok/animation/sources/example.cpp",
                "statements": [{"line": 1}],
            }

            with mock.patch.object(MATCH_DB, "SOURCES", root / "sources"):
                first = MATCH_DB.effective_source_hash(record, "animation")
                source.write_text(
                    "body line EDITED\nother function\n", encoding="latin-1"
                )
                second = MATCH_DB.effective_source_hash(record, "animation")

            self.assertNotEqual(first, second)

    def test_retained_max_uses_pinned_locator_for_different_folded_owner(self):
        previous = (
            "?accessor@@",
            "expected-hash",
            100.0,
            1,
            "state-id",
            "animation",
            "vostok/animation/accessor.h",
            86,
            86,
            "island",
            "evidence.json",
        )
        folded_alias = {"file": "boost/bind/bind.hpp", "statements": []}

        with mock.patch.object(
            MATCH_DB,
            "effective_source_hash_at",
            return_value="expected-hash",
        ) as hash_at, mock.patch.object(
            MATCH_DB, "effective_source_hash"
        ) as hash_record:
            result = MATCH_DB.retained_max_effective_hash(previous, folded_alias)

        self.assertEqual(result, "expected-hash")
        hash_at.assert_called_once_with(
            "vostok/animation/accessor.h", 86, 86, "animation"
        )
        hash_record.assert_not_called()

    def test_retained_max_uses_current_extent_for_same_owner(self):
        previous = (
            "?accessor@@",
            "expected-hash",
            100.0,
            1,
            "state-id",
            "animation",
            "vostok/animation/accessor.h",
            86,
            86,
            "island",
            "evidence.json",
        )
        current = {
            "file": "vostok/animation/accessor.h",
            "statements": [{"line": 87}],
        }

        with mock.patch.object(
            MATCH_DB, "_source_extent", return_value=(current["file"], 87, 87, "body")
        ), mock.patch.object(
            MATCH_DB, "effective_source_hash", return_value="current-hash"
        ) as hash_record, mock.patch.object(
            MATCH_DB, "effective_source_hash_at"
        ) as hash_at:
            result = MATCH_DB.retained_max_effective_hash(previous, current)

        self.assertEqual(result, "current-hash")
        hash_record.assert_called_once_with(current, "animation")
        hash_at.assert_not_called()


class StructureClassificationTests(unittest.TestCase):
    @staticmethod
    def record(sizes, lines):
        return {
            "statements": [
                {"size": size, "line": line} for size, line in zip(sizes, lines)
            ]
        }

    def test_exact_relative_line_geometry_defeats_repeated_size_misalignment(self):
        target = self.record([1, 2, 1], [10, 11, 12])
        base = self.record([1, 1, 2], [30, 31, 32])

        self.assertEqual(
            MATCH_DB.classify(target, base),
            ("SIZE", 3, 3, 2, 0, 0),
        )

    def test_different_relative_lines_keep_size_alignment_fallback(self):
        target = self.record([1, 2, 1], [10, 11, 12])
        base = self.record([1, 1, 2], [30, 32, 33])

        classification = MATCH_DB.classify(target, base)

        self.assertEqual(classification[0], "SPLIT")
        self.assertGreater(classification[4], 0)
        self.assertGreater(classification[5], 0)


class StrictSourceAliasCandidateTests(unittest.TestCase):
    def record(
        self,
        file="scaleform/src/render/render_matrix2x4.h",
        text="ret   4",
        rva=0x1234,
        name="Scaleform::Render::Matrix2x4<float>::Matrix2x4<float>(copy)",
        mangled="??0Matrix2x4@Render@Scaleform@@QAE@ABV012@@Z",
    ):
        return {
            "name": name,
            "mangled": mangled,
            "file": file,
            "rva": rva,
            "size": 3,
            "instructions": [{"off": 0, "len": 3, "text": text}],
        }

    def test_accepts_unique_same_source_exact_alias(self):
        target = self.record()
        base = self.record()
        aliases = {target["name"]: {0x1234: base}}
        self.assertEqual(
            MATCH_DB.strict_source_alias_candidates(target, aliases, set()), [base]
        )

    def test_rejects_used_different_source_or_different_body(self):
        target = self.record()
        used = self.record()
        other_source = self.record(file="scaleform/src/render/other.h")
        other_body = self.record(text="ret   8")
        aliases = {
            target["name"]: {
                0x1000: used,
                0x2000: other_source,
                0x3000: other_body,
            }
        }
        self.assertEqual(
            MATCH_DB.strict_source_alias_candidates(target, aliases, {0x1000}), []
        )

    def test_accepts_exact_used_rva_when_shared_aliases_are_allowed(self):
        target = self.record()
        base = self.record()
        aliases = {target["name"]: {0x1234: base}}
        self.assertEqual(
            MATCH_DB.strict_source_alias_candidates(
                target, aliases, {0x1234}, allow_used=True
            ),
            [base],
        )

    def test_accepts_different_owner_for_shared_multi_name_icf_cluster(self):
        target = self.record(file="vostok/animation/sources/cook.cpp", rva=0x1000)
        base = self.record(file="vostok/memory_buffer.h", rva=0x2000)
        aliases = {target["name"]: {base["rva"]: base}}
        target_names = {
            target["rva"]: {target["name"], "vostok::physics::folded destructor"}
        }
        base_names = {
            base["rva"]: {target["name"], "vostok::physics::folded destructor"}
        }

        self.assertEqual(
            MATCH_DB.strict_source_alias_candidates(
                target,
                aliases,
                set(),
                target_alias_names_by_rva=target_names,
                base_alias_names_by_rva=base_names,
            ),
            [base],
        )

    def test_rejects_different_owner_without_shared_multi_name_cluster(self):
        target = self.record(file="first.cpp", rva=0x1000)
        base = self.record(file="second.cpp", rva=0x2000)
        aliases = {target["name"]: {base["rva"]: base}}

        self.assertEqual(
            MATCH_DB.strict_source_alias_candidates(
                target,
                aliases,
                set(),
                target_alias_names_by_rva={target["rva"]: {target["name"]}},
                base_alias_names_by_rva={base["rva"]: {target["name"]}},
            ),
            [],
        )

    def test_rejects_different_owner_when_alias_clusters_only_overlap(self):
        target = self.record(file="first.cpp", rva=0x1000)
        base = self.record(file="second.cpp", rva=0x2000)
        aliases = {target["name"]: {base["rva"]: base}}
        shared = "vostok::resources::resource_ptr<T>::~resource_ptr<T>()"

        self.assertEqual(
            MATCH_DB.strict_source_alias_candidates(
                target,
                aliases,
                set(),
                target_alias_names_by_rva={
                    target["rva"]: {target["name"], shared, "target-only alias"}
                },
                base_alias_names_by_rva={
                    base["rva"]: {target["name"], shared, "base-only alias"}
                },
            ),
            [],
        )

    def test_accepts_exact_reviewed_fold_alias(self):
        target = self.record(
            name="vostok::animation::mixing::animation_interval::animation() const",
            mangled="?animation@animation_interval@@QBEABVresource_ptr@@XZ",
            file="vostok/animation/mixing_animation_interval.h",
            rva=0x1000,
        )
        base = self.record(
            name="survarium::collision_geometry::cast_to_collision_geometry()",
            mangled="?cast_to_collision_geometry@collision_geometry@@QAEXXZ",
            file="vostok/game_core/collision_geometry.h",
            rva=0x2000,
        )
        self.assertEqual(
            MATCH_DB.strict_source_alias_candidates(
                target,
                {},
                set(),
                exact_fold_aliases={target["mangled"]: base["mangled"]},
                base_aliases_by_mangled={base["mangled"]: {base["rva"]: base}},
            ),
            [base],
        )

    def test_rejects_reviewed_fold_alias_with_different_body(self):
        target = self.record(mangled="?target@@", rva=0x1000)
        base = self.record(mangled="?base@@", rva=0x2000, text="ret   8")
        self.assertEqual(
            MATCH_DB.strict_source_alias_candidates(
                target,
                {},
                set(),
                exact_fold_aliases={target["mangled"]: base["mangled"]},
                base_aliases_by_mangled={base["mangled"]: {base["rva"]: base}},
            ),
            [],
        )

    def test_report_alias_accepts_same_source_with_different_body(self):
        target = self.record(text="ret   4", rva=0x1000)
        base = self.record(text="ret   8", rva=0x2000)
        aliases = {target["name"]: {base["rva"]: base}}

        self.assertEqual(
            MATCH_DB.report_source_alias_candidates(target, aliases, set()),
            [base],
        )

    def test_report_alias_rejects_used_or_unrelated_owner(self):
        target = self.record(file="first.cpp", rva=0x1000)
        used = self.record(file="first.cpp", rva=0x2000)
        unrelated = self.record(file="second.cpp", rva=0x3000)
        aliases = {
            target["name"]: {
                used["rva"]: used,
                unrelated["rva"]: unrelated,
            }
        }

        self.assertEqual(
            MATCH_DB.report_source_alias_candidates(
                target,
                aliases,
                {used["rva"]},
                target_alias_names_by_rva={target["rva"]: {target["name"]}},
                base_alias_names_by_rva={unrelated["rva"]: {target["name"]}},
            ),
            [],
        )

    def test_finds_exact_alias_already_represented_by_paired_rva(self):
        base = self.record(rva=0x2000)
        target = self.record(rva=0x1000)
        aliases = {base["name"]: {target["rva"]: target}}

        self.assertEqual(
            MATCH_DB.exact_paired_source_alias(base, aliases, {target["rva"]}),
            target,
        )

    def test_rejects_exact_alias_at_unpaired_rva(self):
        base = self.record(rva=0x2000)
        target = self.record(rva=0x1000)
        aliases = {base["name"]: {target["rva"]: target}}

        self.assertIsNone(MATCH_DB.exact_paired_source_alias(base, aliases, set()))


class DynamicThunkAliasTests(unittest.TestCase):
    def test_canonicalizes_local_static_scope_in_both_rich_spellings(self):
        target = (
            "`survarium::weapon_cook::register_cooks_for_logic_states'::`2'::"
            "`dynamic atexit destructor for 's_fire_cook''"
        )
        base = (
            "`dynamic atexit destructor for '"
            "`survarium::weapon_cook::register_cooks_for_logic_states'::`2'::"
            "s_fire_cook''"
        )
        expected = (
            "F",
            "`survarium::weapon_cook::register_cooks_for_logic_states'::`2'::"
            "s_fire_cook",
        )
        self.assertEqual(MATCH_DB.dyn_canon_rich(target), expected)
        self.assertEqual(MATCH_DB.dyn_canon_base(base), expected)

    def test_allows_missing_synthetic_owner_but_rejects_known_mismatch(self):
        target = {"file": "vostok/animation/anim_track_common.h"}
        global_canon = ("F", "vostok::sound::s_debug_audio")
        local_canon = (
            "F",
            "`survarium::weapon_cook::register_cooks_for_logic_states'::`2'::"
            "s_fire_cook",
        )
        self.assertTrue(
            MATCH_DB.dyn_owner_compatible(target, {"file": None}, global_canon)
        )
        self.assertTrue(MATCH_DB.dyn_owner_compatible(target, dict(target), global_canon))
        self.assertTrue(
            MATCH_DB.dyn_owner_compatible(
                target, {"file": "vostok/game/sources/weapon_cook.cpp"}, local_canon
            )
        )
        self.assertFalse(
            MATCH_DB.dyn_owner_compatible(
                target,
                {"file": "vostok/game/sources/weapon_cook.cpp"},
                global_canon,
            )
        )

    def test_resolves_local_static_owner_from_enclosing_target_function(self):
        thunk = (
            "`survarium::weapon_cook::register_cooks_for_logic_states'::`2'::"
            "`dynamic atexit destructor for 's_fire_cook''"
        )
        records = [
            {
                "mangled": thunk,
                "name": f"void {thunk}()",
                "file": "vostok/animation/anim_track_common.h",
            },
            {
                "mangled": "?register_cooks_for_logic_states@weapon_cook@@SAXXZ",
                "name": "void survarium::weapon_cook::register_cooks_for_logic_states()",
                "file": "vostok/game/sources/weapon_cook.cpp",
            },
        ]

        owners = MATCH_DB.dynamic_local_owner_modules(records)

        self.assertEqual(
            owners[MATCH_DB.dyn_canon_rich(thunk)],
            "game",
        )

    def test_exact_rich_stream_fills_only_missing_dynamic_pair_score(self):
        target = {"size": 1, "instructions": [{"off": 0, "len": 1, "text": "ret"}]}
        base = dict(target)
        self.assertEqual(
            MATCH_DB.dynamic_pair_score("target", "base", target, base, {}),
            100.0,
        )
        self.assertEqual(
            MATCH_DB.dynamic_pair_score(
                "target", "base", target, base, {"target": 75.0}
            ),
            75.0,
        )
        different = {
            "size": 1,
            "instructions": [{"off": 0, "len": 1, "text": "int 3"}],
        }
        self.assertIsNone(
            MATCH_DB.dynamic_pair_score("target", "base", target, different, {})
        )


class ModuleOwnershipOverrideTests(unittest.TestCase):
    def test_loads_reviewed_owner_and_rejects_inconsistent_source_module(self):
        with tempfile.TemporaryDirectory() as temporary_directory:
            path = Path(temporary_directory) / "owners.tsv"
            path.write_text(
                "?symbol@@\tgame_core\tvostok/game_core/example.h\n",
                encoding="utf-8",
            )
            self.assertEqual(
                MATCH_DB.load_module_ownership_overrides(path),
                {"?symbol@@": "game_core"},
            )
            path.write_text(
                "?symbol@@\tanimation\tvostok/game_core/example.h\n",
                encoding="utf-8",
            )
            with self.assertRaisesRegex(ValueError, "inconsistent module ownership"):
                MATCH_DB.load_module_ownership_overrides(path)

    def test_logical_module_prefers_reviewed_owner_over_folded_unit(self):
        record = {"file": "vostok/animation/folded_inline.h"}
        self.assertEqual(
            MATCH_DB.logical_module(
                "?symbol@@",
                record,
                [record["file"]],
                {},
                {"?symbol@@": "game_core"},
            ),
            "game_core",
        )


if __name__ == "__main__":
    unittest.main()
