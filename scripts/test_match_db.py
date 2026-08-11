import importlib.util
import tempfile
import unittest
from pathlib import Path
from unittest import mock


SPEC = importlib.util.spec_from_file_location(
    "vostok_match_db", Path(__file__).with_name("match_db.py")
)
MATCH_DB = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(MATCH_DB)


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


class EffectiveSourceHashTests(unittest.TestCase):
    def test_cpp_epoch_changes_when_another_function_in_the_tu_changes(self):
        with tempfile.TemporaryDirectory() as temporary_directory:
            root = Path(temporary_directory)
            source = root / "sources/vostok/animation/sources/example.cpp"
            source.parent.mkdir(parents=True)
            source.write_text("body line\nother function v1\n", encoding="latin-1")
            record = {
                "file": "vostok/animation/sources/example.cpp",
                "statements": [{"line": 1}],
            }

            with mock.patch.object(MATCH_DB, "VOSTOK", root):
                MATCH_DB._MAX_CONTEXT_CACHE.clear()
                first = MATCH_DB.effective_source_hash(record, "animation")
                source.write_text(
                    "body line\nother function v2\n", encoding="latin-1"
                )
                MATCH_DB._MAX_CONTEXT_CACHE.clear()
                second = MATCH_DB.effective_source_hash(record, "animation")

            self.assertNotEqual(first, second)


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


if __name__ == "__main__":
    unittest.main()
