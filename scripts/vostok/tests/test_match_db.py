import os
import pathlib
import json
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest import mock

from vostok.build import ninja as build_ninja
from vostok.build import ninja_regen
from vostok.build.generate_objdiff_cross_unit import (_defined_owners,
                                                       _identical_units,
                                                       _resolve_reviewed_aliases)
from vostok.core import symbols as NORMALIZE
from vostok.derive import maxima
from vostok.derive.aliases import (dyn_canon_base, dyn_canon_rich,
                                   dyn_owner_compatible, dynamic_pair_score,
                                   exact_paired_source_alias,
                                   instruction_stream_exact,
                                   island_candidate_score,
                                   matching_dynamic_initializer_owners,
                                   report_source_alias_candidates,
                                   strict_source_alias_candidates)
from vostok.derive.classify import classify
from vostok.derive.index import (authoritative_demangled_names,
                                 index_by_mangled, legacy_overload_keys,
                                 overload_key)
from vostok.derive.maxima import effective_source_hash
from vostok.derive.modules import (dynamic_local_owner_modules,
                                   load_module_ownership_overrides,
                                   load_source_ownership_overrides,
                                   logical_module)
from vostok.derive.pairing import Pair, Pairing, pair
from vostok.derive.roster import (enclosing_function_mangled,
                                  report_only_observations)
from vostok.derive.scores import (cross_unit_exact_score, island_report_score,
                                  rank_island_delta, report_fuzzy_scores,
                                  report_overload_scores,
                                  report_score_for_target)
from vostok.ledger import store


class CleanFinalPdbTests(unittest.TestCase):
    def test_shipping_link_libraries_follow_retail_ltcg_order(self):
        libraries = [
            f"Z:/libs/{name}"
            for name in reversed(ninja_regen.RETAIL_LINK_LIBRARY_ORDER)
        ]
        text = "header\nkernel32.lib " + libraries[0] + " keep.lib " + " ".join(
            libraries[1:]
        ) + "\nfooter\n"

        self.assertEqual(
            ninja_regen._normalize_link_rsp_library_order(text),
            "header\nkernel32.lib "
            + " ".join(
                f"Z:/libs/{name}"
                for name in ninja_regen.RETAIL_LINK_LIBRARY_ORDER
            )
            + " keep.lib\nfooter\n",
        )

    def test_incomplete_link_library_suite_is_unchanged(self):
        text = "vostok_engine-static-gold.lib keep.lib\n"
        self.assertEqual(
            ninja_regen._normalize_link_rsp_library_order(text),
            text,
        )

    def test_link_rsp_paths_crossing_solution_parent_are_normalized(self):
        text = (
            '/OUT:"Z:/work/vostok/sources\\../binaries/base.exe" '
            '/IMPLIB:"Z:/work/vostok/sources\\/../binaries/base.lib" '
            '"Z:/work/vostok/sources\\/render.lib"'
        )

        self.assertEqual(
            ninja_regen._normalize_link_rsp_paths(
                text,
                solution_dir=Path("/work/vostok/sources"),
                repo_dir=Path("/work/vostok"),
            ),
            '/OUT:"Z:/work/vostok/binaries/base.exe" '
            '/IMPLIB:"Z:/work/vostok/binaries/base.lib" '
            '"Z:/work/vostok/sources\\/render.lib"',
        )

    def test_ninja_options_only_select_default_full_build(self):
        self.assertEqual(build_ninja._explicit_targets(["-j6"]), [])
        self.assertEqual(build_ninja._explicit_targets(["-j", "6"]), [])

    def test_explicit_module_is_not_default_full_build(self):
        self.assertEqual(
            build_ninja._explicit_targets(["render_engine_pc_dx11", "-j6"]),
            ["render_engine_pc_dx11"],
        )

    def test_interrupted_module_build_reaps_detached_wine_processes(self):
        proc = mock.Mock(pid=123)
        proc.wait.side_effect = KeyboardInterrupt
        with (mock.patch.object(build_ninja, "_assert_no_existing_build"),
              mock.patch.object(build_ninja, "_prefix_process_ids", return_value={41, 42}),
              mock.patch.object(build_ninja.subprocess, "Popen", return_value=proc),
              mock.patch.object(build_ninja, "_stop_interrupted_build") as stop):
            with self.assertRaises(KeyboardInterrupt):
                build_ninja._run_plain(Path("ninja.exe"), ["render"])

        stop.assert_called_once_with(proc, {41, 42})

    def test_interrupted_full_build_reaps_detached_wine_processes(self):
        proc = mock.Mock(pid=123)
        proc.poll.return_value = None
        with (mock.patch.object(build_ninja, "_assert_no_existing_build"),
              mock.patch.object(build_ninja, "_prefix_process_ids", return_value={41, 42}),
              mock.patch.object(build_ninja.subprocess, "Popen", return_value=proc),
              mock.patch.object(build_ninja, "_wine_tree_jiffies", return_value=0),
              mock.patch.object(build_ninja.time, "sleep", side_effect=KeyboardInterrupt),
              mock.patch.object(build_ninja, "_stop_interrupted_build") as stop):
            with self.assertRaises(KeyboardInterrupt):
                build_ninja._run_with_watchdog(Path("ninja.exe"), ["game"])

        stop.assert_called_once_with(proc, {41, 42})

    def test_existing_build_in_same_prefix_refuses_overlap(self):
        with (mock.patch.object(build_ninja, "_prefix_process_ids", return_value={42, 41}),
              mock.patch.object(build_ninja.subprocess, "Popen") as popen):
            with self.assertRaises(SystemExit):
                build_ninja._run_plain(Path("ninja.exe"), ["render"])

        popen.assert_not_called()

    def test_interrupted_cleanup_preserves_preexisting_prefix_processes(self):
        proc = mock.Mock(pid=123)
        with (mock.patch.object(build_ninja.os, "getpgid", return_value=456),
              mock.patch.object(build_ninja.os, "killpg") as killpg,
              mock.patch.object(build_ninja, "_kill_prefix_processes") as kill_prefix):
            build_ninja._stop_interrupted_build(proc, {41, 42})

        killpg.assert_called_once_with(456, build_ninja.signal.SIGKILL)
        kill_prefix.assert_called_once_with(
            build_ninja._INTERRUPT_BUILD_COMMS,
            exclude_pids=frozenset({41, 42}),
        )
        proc.wait.assert_called_once_with(timeout=30)

    def test_supervisor_termination_unwinds_through_cleanup(self):
        with self.assertRaises(SystemExit) as raised:
            build_ninja._raise_on_termination(build_ninja.signal.SIGTERM, None)

        self.assertEqual(raised.exception.code, 128 + build_ninja.signal.SIGTERM)

    def test_termination_cleanup_scope_restores_signal_handlers(self):
        old_term = object()
        old_hup = object()
        with mock.patch.object(
            build_ninja.signal,
            "signal",
            side_effect=[old_term, old_hup, None, None],
        ) as install:
            with build_ninja._termination_cleanup_scope():
                pass

        self.assertEqual(
            install.call_args_list,
            [
                mock.call(build_ninja.signal.SIGTERM, build_ninja._raise_on_termination),
                mock.call(build_ninja.signal.SIGHUP, build_ninja._raise_on_termination),
                mock.call(build_ninja.signal.SIGTERM, old_term),
                mock.call(build_ninja.signal.SIGHUP, old_hup),
            ],
        )

    def test_scheduled_link_removes_existing_pdb_only(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            exe = root / "base.exe"
            pdb = root / "base.pdb"
            exe.write_bytes(b"exe")
            pdb.write_bytes(b"stale")
            probe = SimpleNamespace(
                returncode=0,
                stdout=(
                    b'cmd /c cd "Z:/source" && link '
                    + build_ninja.FINAL_LINK_RSP_MARKER
                ),
            )
            with (mock.patch.object(build_ninja, "BASE_EXE", exe),
                  mock.patch.object(build_ninja, "BASE_PDB", pdb),
                  mock.patch.object(build_ninja.subprocess, "run", return_value=probe)):
                build_ninja._prepare_clean_final_pdb(Path("ninja.exe"), ["game"])

            self.assertTrue(exe.exists())
            self.assertFalse(pdb.exists())

    def test_noop_build_keeps_existing_pdb(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            exe = root / "base.exe"
            pdb = root / "base.pdb"
            exe.write_bytes(b"exe")
            pdb.write_bytes(b"current")
            probe = SimpleNamespace(returncode=0, stdout=b"ninja: no work to do")
            with (mock.patch.object(build_ninja, "BASE_EXE", exe),
                  mock.patch.object(build_ninja, "BASE_PDB", pdb),
                  mock.patch.object(build_ninja.subprocess, "run", return_value=probe)):
                build_ninja._prepare_clean_final_pdb(Path("ninja.exe"), ["game"])

            self.assertTrue(exe.exists())
            self.assertEqual(pdb.read_bytes(), b"current")

    def test_missing_pdb_forces_output_edge_dirty(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            exe = root / "base.exe"
            pdb = root / "base.pdb"
            exe.write_bytes(b"exe")
            probe = SimpleNamespace(returncode=0, stdout=b"ninja: no work to do")
            with (mock.patch.object(build_ninja, "BASE_EXE", exe),
                  mock.patch.object(build_ninja, "BASE_PDB", pdb),
                  mock.patch.object(build_ninja.subprocess, "run", return_value=probe)):
                build_ninja._prepare_clean_final_pdb(Path("ninja.exe"), ["game"])

            self.assertFalse(exe.exists())


class ReportOnlyObservationTests(unittest.TestCase):
    PARENT = (
        "??$create_effect@Veffect_ssao_accumulation@render@vostok@@@"
        "effect_manager@render@vostok@@QAEXPAV?$resource_ptr@Vres_effect@render@"
        "vostok@@Vunmanaged_intrusive_base@resources@3@@resources@2@@Z"
    )
    THUNK = "??__Fdescriptor_object@?1?" + PARENT + "@YAXXZ"

    def test_recovers_enclosing_function_from_local_static_thunk(self):
        self.assertEqual(enclosing_function_mangled(self.THUNK), self.PARENT)
        self.assertIsNone(enclosing_function_mangled("??__Fglobal_static@@YAXXZ"))

    def test_attributes_report_only_exact_to_enclosing_source(self):
        owner = SimpleNamespace(
            unit="vostok/render/core/dx11/effect_manager_inline.h",
            module="render",
        )
        roster = SimpleNamespace(
            target={self.PARENT: owner},
            artifacts=SimpleNamespace(
                report_fns=[
                    (
                        "vostok/render/engine/sources/stage_ambient_occlusion.cpp",
                        self.THUNK,
                        100.0,
                        11,
                    )
                ]
            ),
        )
        previous = {
            self.THUNK: {
                "mangled": self.THUNK,
                "unit": "",
                "module": "",
                "size": 0,
                "flags": "",
                "cls": "",
                "hash": "",
            }
        }

        rows = list(
            report_only_observations(
                roster,
                {self.PARENT: ("sourcehash", 100.0)},
                previous,
            )
        )

        self.assertEqual(len(rows), 1)
        self.assertEqual(rows[0]["unit"], owner.unit)
        self.assertEqual(rows[0]["module"], "render")
        self.assertEqual(rows[0]["cur"], 100.0)
        self.assertEqual(rows[0]["size"], 11)
        self.assertEqual(rows[0]["hash"], "sourcehash")

    def test_does_not_invent_ownership_from_internal_delinker_bucket(self):
        symbol = "??_Elobby_camera@survarium@@UAEPAXI@Z"
        roster = SimpleNamespace(
            target={},
            artifacts=SimpleNamespace(
                report_fns=[("_msvc_internal/survarium", symbol, 80.0, 25)]
            ),
        )
        previous = {
            symbol: {
                "mangled": symbol,
                "unit": "",
                "module": "",
                "size": 0,
                "flags": "",
                "cls": "",
                "hash": "",
            }
        }

        rows = list(report_only_observations(roster, {}, previous))

        self.assertEqual(len(rows), 1)
        self.assertEqual(rows[0]["unit"], "")
        self.assertEqual(rows[0]["module"], "")
        self.assertEqual(rows[0]["cur"], 80.0)


class CrossUnitEvidenceTests(unittest.TestCase):
    def test_resolves_reviewed_base_pdb_identity_to_delinked_coff_name(self):
        pairs = {"target": "base-pdb", "unchanged": "base-coff"}
        choices = {"base-pdb": "selected-coff"}

        self.assertEqual(
            _resolve_reviewed_aliases(pairs, choices),
            {"target": "selected-coff", "unchanged": "base-coff"},
        )

    @mock.patch("vostok.build.generate_objdiff_cross_unit.subprocess.run")
    def test_defined_owners_deduplicates_repeated_symbols(self, run):
        with tempfile.TemporaryDirectory() as temporary_directory:
            root = Path(temporary_directory)
            object_path = root / "inline.h.obj"
            object_path.write_bytes(b"object")
            run.return_value = SimpleNamespace(
                stdout=(
                    f"{object_path}: 00000000 T symbol\n"
                    f"{object_path}: 00000000 T symbol\n"
                )
            )

            self.assertEqual(
                _defined_owners(root, {"symbol"}, "llvm-nm"),
                {"symbol": [object_path.resolve()]},
            )

    def test_identifies_complete_exact_units(self):
        with tempfile.TemporaryDirectory() as temporary_directory:
            root = Path(temporary_directory)
            for side in ("target", "base"):
                (root / side / "module").mkdir(parents=True)
                (root / side / "module" / "exact.cpp.obj").write_bytes(b"exact")
                (root / side / "module" / "scored.cpp.obj").write_bytes(b"exact")
            (root / "target" / "module" / "different.cpp.obj").write_bytes(b"target")
            (root / "base" / "module" / "different.cpp.obj").write_bytes(b"base")

            report = {
                "units": [
                    {
                        "name": "module/exact.cpp",
                        "functions": [{"fuzzy_match_percent": None}],
                    },
                    {
                        "name": "module/different.cpp",
                        "functions": [{"fuzzy_match_percent": None}],
                    },
                    {
                        "name": "module/scored.cpp",
                        "functions": [{"fuzzy_match_percent": 100.0}],
                    },
                ]
            }

            self.assertEqual(
                _identical_units(report, root),
                ["module/exact.cpp", "module/scored.cpp"],
            )


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
                    "file": "vostok/core/sources/math_quaternion.cpp",
                    "name": "void core_placeholder()",
                    "mangled": "core_placeholder",
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
                    "file": "vostok/core/sources/math_quaternion.cpp",
                    "name": "void core_placeholder()",
                    "mangled": "?core_placeholder@@YAXXZ",
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

            aliases = NORMALIZE.rich_pdb_aliases(
                target_index, base_index, source_prefix="vostok/"
            )

        self.assertEqual(
            aliases,
            {
                "vostok::render::construct<int>":
                    "??$construct@H@render@vostok@@YAXH@Z",
                "core_placeholder": "?core_placeholder@@YAXXZ",
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

        indexed = index_by_mangled([first, second])

        self.assertEqual(len(indexed), 2)
        self.assertIs(indexed[first["mangled"]], first)
        self.assertIs(
            indexed[overload_key(first["mangled"], second["name"])], second
        )

    def test_uses_target_primary_signature_when_rva_order_differs(self):
        target_first = self.record("fill_surface(target*, context*)", 0x1000)
        target_second = self.record("fill_surface(target*, context*, bool)", 0x1020)
        base_second = self.record(target_second["name"], 0x2000)
        base_first = self.record(target_first["name"], 0x2020)

        indexed = index_by_mangled(
            [base_second, base_first],
            preferred_signatures={target_first["mangled"]: target_first["name"]},
        )

        self.assertIs(indexed[target_first["mangled"]], base_first)
        self.assertIs(
            indexed[overload_key(target_first["mangled"], target_second["name"])],
            base_second,
        )

    def test_maps_legacy_rva_key_to_signature_key(self):
        first = self.record("fill_surface(target*, context*)", 0x1000)
        second = self.record("fill_surface(target*, context*, bool)", 0x1020)

        aliases = legacy_overload_keys([first, second])

        self.assertEqual(
            aliases[f"{first['mangled']}@@pdb-overload:1020"],
            overload_key(first["mangled"], second["name"]),
        )

    def test_still_collapses_distinct_same_rva_aliases(self):
        first = self.record("first alias()", 0x1000)
        second = self.record("second alias()", 0x1000)

        indexed = index_by_mangled([first, second])

        self.assertEqual(indexed, {first["mangled"]: first})

    def test_still_collapses_same_signature_aliases_to_preferred_owner(self):
        first = self.record("static helper()", 0x1000, "first.cpp")
        preferred = self.record("static helper()", 0x2000, "preferred.cpp")

        indexed = index_by_mangled(
            [first, preferred], {first["mangled"]: "preferred.cpp"}
        )

        self.assertEqual(indexed, {first["mangled"]: preferred})


class AuthoritativeDemangledNamesTests(unittest.TestCase):
    def test_target_name_wins_when_base_fold_alias_reuses_mangled_name(self):
        mangled = "?animation@animation_interval@@QBEABVresource_ptr@@XZ"
        target = {mangled: {"name": "animation_interval::animation() const"}}
        base = {mangled: {"name": "collision_geometry::cast_to_collision_geometry()"}}

        self.assertEqual(
            authoritative_demangled_names(target, base)[mangled],
            target[mangled]["name"],
        )

    def test_base_only_symbol_keeps_base_name(self):
        mangled = "?base_only@@YAXXZ"
        base = {mangled: {"name": "base_only()"}}

        self.assertEqual(
            authoritative_demangled_names({}, base)[mangled],
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
            instruction_stream_exact(self.record(), self.record())
        )

    def test_rejects_different_instruction_or_size(self):
        self.assertFalse(
            instruction_stream_exact(
                self.record(), self.record(text="call  Scaleform::Render::HAL::BeginFrame")
            )
        )
        self.assertFalse(
            instruction_stream_exact(self.record(), self.record(size=5))
        )

    def test_rejects_missing_instruction_evidence(self):
        empty = {"size": 0, "instructions": []}
        self.assertFalse(instruction_stream_exact(empty, empty))

    def test_accepts_equivalent_operator_delete_pdb_spellings(self):
        target = self.record(text="call  operator delete")
        base = self.record(text="call  ??3@YAXPAX@Z")
        self.assertTrue(instruction_stream_exact(target, base))

    def test_accepts_equivalent_template_closing_whitespace(self):
        target = self.record(text="call  pinned_ptr<animation const >::operator=")
        base = self.record(text="call  pinned_ptr<animation const>::operator=")
        self.assertTrue(instruction_stream_exact(target, base))

    def test_accepts_icf_aliased_trailing_relocation_operand(self):
        target = self.record(text="mov   ecx, target_alias<class_a,class_b>::c_ptr")
        base = self.record(text="mov   ecx, base_alias<class_c,class_d>::c_ptr")
        self.assertTrue(
            instruction_stream_exact(
                target,
                base,
                lambda target_operand, base_operand: (
                    target_operand == "target_alias<class_a,class_b>::c_ptr"
                    and base_operand == "base_alias<class_c,class_d>::c_ptr"
                ),
            )
        )

    def test_rejects_icf_alias_when_instruction_destination_differs(self):
        target = self.record(text="mov   ecx, target_alias")
        base = self.record(text="mov   edx, base_alias")
        self.assertFalse(
            instruction_stream_exact(target, base, lambda _target, _base: True)
        )

    def test_island_exact_stream_overrides_stale_report_pair(self):
        mangled = "??4bone_matrices_computer_data@@"
        target = self.record(text="movq  xmm0, [eax]")
        candidate = self.record(text="movq  xmm0, [eax]")

        self.assertEqual(
            island_candidate_score(
                {}, mangled, {mangled: 59.464287}, target, candidate
            ),
            100.0,
        )

    def test_island_nonexact_stream_keeps_report_score(self):
        mangled = "??4bone_matrices_computer_data@@"
        target = self.record(text="movq  xmm0, [eax]")
        candidate = self.record(text="movq  xmm1, [eax]")

        self.assertEqual(
            island_candidate_score(
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

        scores, units = report_fuzzy_scores(report)

        self.assertEqual(scores, {"?fn@@": 100.0})
        self.assertEqual(
            units["?fn@@"],
            ["vostok/animation/a.cpp", "vostok/animation/z.cpp"],
        )
        self.assertEqual(units["?none@@"], ["vostok/animation/z.cpp"])

    def test_attributes_placeholder_overload_by_unique_source_unit(self):
        raw = "vostok::render::fill_surface"
        overload = f"{raw}@@pdb-overload:signature"
        target = {
            raw: {
                "mangled": raw,
                "file": "vostok/render/engine/sources/stage_gbuffer.cpp",
            },
            overload: {
                "mangled": raw,
                "file": (
                    "vostok/render/engine/sources/stage_ambient_occlusion.cpp"
                ),
            },
        }
        report = {
            "units": [
                {
                    "name": target[raw]["file"],
                    "functions": [
                        {"name": raw, "fuzzy_match_percent": 75.0},
                    ],
                },
                {
                    "name": target[overload]["file"],
                    "functions": [
                        {"name": raw, "fuzzy_match_percent": 100.0},
                    ],
                },
            ]
        }

        self.assertEqual(
            report_overload_scores(target, report),
            {overload: 100.0},
        )

    def test_rejects_same_unit_placeholder_overload_ambiguity(self):
        raw = "vostok::render::fill_surface"
        source = "vostok/render/engine/sources/example.cpp"
        target = {
            raw: {"mangled": raw, "file": source},
            f"{raw}@@pdb-overload:second": {"mangled": raw, "file": source},
        }
        report = {
            "units": [
                {
                    "name": source,
                    "functions": [
                        {"name": raw, "fuzzy_match_percent": 100.0},
                    ],
                },
            ]
        }

        self.assertEqual(report_overload_scores(target, report), {})

    def test_maps_scalar_deleting_destructor_to_vector_report_name(self):
        scalar = "??_Gexample@@UAEPAXI@Z"
        vector = "??_Eexample@@UAEPAXI@Z"

        self.assertEqual(
            report_score_for_target(scalar, {vector: 88.5}), 88.5
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
            report_score_for_target(pdb_name, {report_name: 90.13513}),
            90.13513,
        )

    def test_maps_generated_fold_representative_to_report_name(self):
        target = "?copy@concrete_type_helper@grass_loading_data@@"
        representative = "?copy@concrete_type_helper@physics_world@@"
        aliases = {target: representative}

        self.assertEqual(
            report_score_for_target(
                target,
                {representative: 46.125},
                aliases,
            ),
            46.125,
        )
        self.assertEqual(
            report_score_for_target(
                target,
                {target: 100.0, representative: 46.125},
                aliases,
            ),
            100.0,
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

        scores, _units = report_fuzzy_scores(report)

        self.assertEqual(scores, {"?fn@@": 42.5})

    def test_cross_unit_exact_maps_scalar_pdb_to_vector_coff_destructor(self):
        scalar = "??_Gskeleton@animation@vostok@@UAEPAXI@Z"
        vector = "??_Eskeleton@animation@vostok@@UAEPAXI@Z"

        self.assertEqual(
            cross_unit_exact_score(scalar, {vector: 100.0}),
            100.0,
        )

    def test_cross_unit_exact_accepts_same_name_and_rejects_partial_score(self):
        mangled = "??0expression@mixing@animation@vostok@@QAE@ABV0123@@Z"

        self.assertEqual(
            cross_unit_exact_score(mangled, {mangled: 100.0}),
            100.0,
        )
        self.assertIsNone(
            cross_unit_exact_score(mangled, {mangled: 99.0})
        )

    def test_island_report_score_accepts_explicit_reviewed_alias(self):
        target = "?create_resource_inplace@@"
        report_name = "?create_resource@@"

        self.assertEqual(
            island_report_score(
                {"report_mangled": report_name},
                target,
                {report_name: 92.88461},
            ),
            92.88461,
        )
        self.assertEqual(
            island_report_score({}, target, {target: 75.0}),
            75.0,
        )


class RankIslandDeltaTests(unittest.TestCase):
    def test_discovers_target_only_candidate_from_zero_floor(self):
        delta = rank_island_delta(
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
            rank_island_delta(
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
            rank_island_delta(
                60.0,
                "stale-hash",
                previous=previous,
                current_fuzzy=75.0,
            )
        )


class MaximaFoldTests(unittest.TestCase):
    """MAX accumulates while the source body holds, and restarts when it moves."""

    MANGLED = "?function@@YAXXZ"

    def fold(self, banked, fuzzy, body):
        """Fold one observation of one function whose source is `body`."""
        with tempfile.TemporaryDirectory() as temporary_directory:
            root = Path(temporary_directory)
            source = root / "sources/vostok/animation/sources/example.cpp"
            source.parent.mkdir(parents=True)
            source.write_text(body, encoding="latin-1")
            record = {
                "mangled": self.MANGLED,
                "file": "vostok/animation/sources/example.cpp",
                "statements": [{"line": 1}],
            }
            pairing = Pairing(
                pairs={self.MANGLED: Pair(
                    self.MANGLED, 0x1000, 0x2000, fuzzy, "SIZE", 1, 1, 0, 0, 0
                )},
            )
            artifacts = mock.Mock(base={self.MANGLED: record})
            with mock.patch.object(maxima, "SOURCES", root / "sources"):
                return maxima.fold(pairing, artifacts, banked)[self.MANGLED]

    def test_same_body_keeps_the_banked_peak_when_this_build_scores_lower(self):
        body = "one body\n"
        banked_hash = self.fold({}, 100.0, body)[0]

        self.assertEqual(
            self.fold({self.MANGLED: (banked_hash, 100.0)}, 62.5, body),
            (banked_hash, 100.0),
        )

    def test_same_body_raises_the_peak_when_this_build_scores_higher(self):
        body = "one body\n"
        banked_hash = self.fold({}, 50.0, body)[0]

        self.assertEqual(
            self.fold({self.MANGLED: (banked_hash, 50.0)}, 87.5, body),
            (banked_hash, 87.5),
        )

    def test_objdiff_exact_float_slop_banks_literal_100(self):
        body = "one body\n"
        body_hash, maximum = self.fold({}, 99.9982, body)

        self.assertEqual(maximum, 100.0)
        self.assertEqual(
            self.fold({self.MANGLED: (body_hash, 99.9974)}, 62.5, body),
            (body_hash, 100.0),
        )

    def test_a_changed_body_restarts_at_what_this_build_measured(self):
        stale_hash = self.fold({}, 100.0, "old body\n")[0]
        new_hash, maximum = self.fold(
            {self.MANGLED: (stale_hash, 100.0)}, 62.5, "new body\n"
        )

        self.assertNotEqual(new_hash, stale_hash)
        self.assertEqual(maximum, 62.5, "a rewritten body must prove itself again")

    def test_an_unscored_pair_makes_no_observation_at_all(self):
        """No row, rather than a zero: `project` then keeps the banked pair."""
        with tempfile.TemporaryDirectory() as temporary_directory:
            root = Path(temporary_directory)
            source = root / "sources/vostok/animation/sources/example.cpp"
            source.parent.mkdir(parents=True)
            source.write_text("body\n", encoding="latin-1")
            record = {
                "mangled": self.MANGLED,
                "file": "vostok/animation/sources/example.cpp",
                "statements": [{"line": 1}],
            }
            pairing = Pairing(
                pairs={self.MANGLED: Pair(
                    self.MANGLED, 0x1000, 0x2000, None, "SIZE", 1, 1, 0, 0, 0
                )},
            )
            artifacts = mock.Mock(base={self.MANGLED: record})
            with mock.patch.object(maxima, "SOURCES", root / "sources"):
                self.assertEqual(maxima.fold(pairing, artifacts, {}), {})


class EffectiveSourceHashTests(unittest.TestCase):
    def test_resolves_pdb_lowercase_path_against_caseful_checkout(self):
        with tempfile.TemporaryDirectory() as temporary_directory:
            root = Path(temporary_directory)
            source = root / "sources/Bullet/LinearMath/btIDebugDraw.h"
            source.parent.mkdir(parents=True)
            source.write_text("body line\n", encoding="latin-1")
            record = {
                "file": "bullet/linearmath/btidebugdraw.h",
                "statements": [{"line": 1}],
            }

            with mock.patch.object(maxima, "SOURCES", root / "sources"):
                self.assertEqual(
                    effective_source_hash(record), maxima.source_hash("body line\n")
                )

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

            with mock.patch.object(maxima, "SOURCES", root / "sources"):
                first = effective_source_hash(record)
                source.write_text(
                    "body line\nother function v2\n", encoding="latin-1"
                )
                second = effective_source_hash(record)

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

            with mock.patch.object(maxima, "SOURCES", root / "sources"):
                first = effective_source_hash(record)
                source.write_text(
                    "body line EDITED\nother function\n", encoding="latin-1"
                )
                second = effective_source_hash(record)

            self.assertNotEqual(first, second)


class StructureClassificationTests(unittest.TestCase):
    @staticmethod
    def record(sizes, lines):
        """Build a record whose BODY is the given statements.

        Wrapped in the synthetic frame braces every real rich record carries,
        so the fixtures exercise what classify() actually reads. Their sizes
        differ deliberately: a prologue that merely differs in size must not
        register as a statement difference.
        """
        body = [{"size": size, "line": line} for size, line in zip(sizes, lines)]
        return {
            "statements": [
                {"size": 9, "line": lines[0] - 1},
                *body,
                {"size": 3, "line": lines[-1] + 1},
            ]
        }

    def test_exact_relative_line_geometry_defeats_repeated_size_misalignment(self):
        target = self.record([1, 2, 1], [10, 11, 12])
        base = self.record([1, 1, 2], [30, 31, 32])

        self.assertEqual(
            classify(target, base),
            ("SIZE", 3, 3, 2, 0, 0),
        )

    def test_frame_braces_are_not_source_statements(self):
        """Identical bodies stay MATCH however the frame differs.

        Counting the raw record made a differing prologue read as SIZE, and a
        side that emits one brace against a side that emits two read as
        QUANTITY - 700 verdicts across the tree, 170 of them landing in the
        actionable queue as structural work that does not exist.
        """
        body = [{"size": 5, "line": 11}, {"size": 7, "line": 12}]
        target = {"statements": [{"size": 9, "line": 10}, *body, {"size": 3, "line": 13}]}
        base = {"statements": [{"size": 21, "line": 40}, *body, {"size": 17, "line": 43}]}

        self.assertEqual(classify(target, base), ("MATCH", 2, 2, 0, 0, 0))

        # A body-less function carries only braces - sometimes a single one.
        self.assertEqual(
            classify({"statements": [{"size": 9, "line": 1}, {"size": 3, "line": 2}]},
                     {"statements": [{"size": 4, "line": 8}]})[0],
            "MATCH",
        )

    def test_different_relative_lines_keep_size_alignment_fallback(self):
        target = self.record([1, 2, 1], [10, 11, 12])
        base = self.record([1, 1, 2], [30, 32, 33])

        classification = classify(target, base)

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
            strict_source_alias_candidates(target, aliases, set()), [base]
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
            strict_source_alias_candidates(target, aliases, {0x1000}), []
        )

    def test_accepts_exact_used_rva_when_shared_aliases_are_allowed(self):
        target = self.record()
        base = self.record()
        aliases = {target["name"]: {0x1234: base}}
        self.assertEqual(
            strict_source_alias_candidates(
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
            strict_source_alias_candidates(
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
            strict_source_alias_candidates(
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
            strict_source_alias_candidates(
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
            strict_source_alias_candidates(
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
            strict_source_alias_candidates(
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
            report_source_alias_candidates(target, aliases, set()),
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
            report_source_alias_candidates(
                target,
                aliases,
                {used["rva"]},
                target_alias_names_by_rva={target["rva"]: {target["name"]}},
                base_alias_names_by_rva={unrelated["rva"]: {target["name"]}},
            ),
            [],
        )

    def test_report_alias_can_select_one_used_same_source_rva(self):
        target = self.record(file="vostok/memory_buffer_inline.h", rva=0x1000)
        base = self.record(file=target["file"], rva=0x2000)
        aliases = {target["name"]: {base["rva"]: base}}

        self.assertEqual(
            report_source_alias_candidates(
                target,
                aliases,
                {base["rva"]},
                allow_used=True,
            ),
            [base],
        )

    def test_report_score_survives_an_exact_primary_on_the_base_rva(self):
        primary_target = self.record(
            name="boost::_bi::value<T>::value(T const&)",
            mangled="?primary@@",
            rva=0x1000,
        )
        primary_base = self.record(
            name=primary_target["name"],
            mangled=primary_target["mangled"],
            rva=0x2000,
        )
        alias_target = self.record(
            name="vostok::const_buffer::const_buffer(mutable_buffer const&)",
            mangled="?target_alias@@",
            file="vostok/memory_buffer_inline.h",
            text="ret   8",
            rva=0x3000,
        )
        alias_base = self.record(
            name=alias_target["name"],
            mangled=primary_base["mangled"],
            file=alias_target["file"],
            rva=primary_base["rva"],
        )
        artifacts = SimpleNamespace(
            target={
                primary_target["mangled"]: primary_target,
                alias_target["mangled"]: alias_target,
            },
            base={primary_base["mangled"]: primary_base},
            target_records=[primary_target, alias_target],
            base_records=[primary_base, alias_base],
            fuzzy={
                primary_target["mangled"]: 100.0,
                alias_target["mangled"]: 19.166666,
            },
            folded_fuzzy={},
            compiler_alias=lambda _mangled: None,
        )

        pairing = pair(artifacts)

        self.assertEqual(pairing.pairs[primary_target["mangled"]].fuzzy, 100.0)
        self.assertEqual(pairing.pairs[alias_target["mangled"]].fuzzy, 19.166666)
        self.assertEqual(
            pairing.pairs[alias_target["mangled"]].base_rva,
            primary_base["rva"],
        )
        self.assertIs(
            pairing.base_record_for[alias_target["mangled"]],
            alias_base,
        )

    def test_primary_recovers_generated_score_on_identical_alias_clusters(self):
        primary = "?primary@@"
        representative = "?representative@@"
        target = self.record(
            name="void primary()",
            mangled=primary,
            text="mov   eax, [111111h]",
            rva=0x1000,
        )
        base = self.record(
            name=target["name"],
            mangled=primary,
            text="mov   eax, [222222h]",
            rva=0x2000,
        )
        target_alias = dict(target, name="void representative()")
        base_alias = dict(base, name=target_alias["name"])
        artifacts = SimpleNamespace(
            target={primary: target},
            base={primary: base},
            target_records=[target, target_alias],
            base_records=[base, base_alias],
            fuzzy={representative: 100.0},
            folded_fuzzy={},
            folded_symbol_aliases={primary: representative},
            compiler_alias=lambda _mangled: None,
        )

        self.assertEqual(pair(artifacts).pairs[primary].fuzzy, 100.0)

    def test_primary_rejects_generated_score_on_different_alias_clusters(self):
        primary = "?primary@@"
        representative = "?representative@@"
        target = self.record(mangled=primary, text="ret   4", rva=0x1000)
        base = self.record(mangled=primary, text="ret   8", rva=0x2000)
        target_alias = dict(target, name="void target_alias()")
        base_alias = dict(base, name="void base_alias()")
        artifacts = SimpleNamespace(
            target={primary: target},
            base={primary: base},
            target_records=[target, target_alias],
            base_records=[base, base_alias],
            fuzzy={representative: 100.0},
            folded_fuzzy={},
            folded_symbol_aliases={primary: representative},
            compiler_alias=lambda _mangled: None,
        )

        self.assertIsNone(pair(artifacts).pairs[primary].fuzzy)

    def test_generated_fold_score_runs_after_strict_rich_evidence(self):
        exact_target = self.record(
            name="void exact_alias()",
            mangled="?exact_target@@",
            rva=0x1000,
        )
        exact_base = self.record(
            name=exact_target["name"],
            mangled="?exact_base@@",
            rva=0x2000,
        )
        fuzzy_target = self.record(
            name="void fuzzy_alias()",
            mangled="?fuzzy_target@@",
            rva=0x3000,
        )
        fuzzy_base = self.record(
            name=fuzzy_target["name"],
            mangled="?fuzzy_base@@",
            rva=0x4000,
            text="ret   8",
        )
        artifacts = SimpleNamespace(
            target={
                exact_target["mangled"]: exact_target,
                fuzzy_target["mangled"]: fuzzy_target,
            },
            base={
                exact_base["mangled"]: exact_base,
                fuzzy_base["mangled"]: fuzzy_base,
            },
            target_records=[exact_target, fuzzy_target],
            base_records=[exact_base, fuzzy_base],
            fuzzy={},
            folded_fuzzy={
                exact_target["mangled"]: 46.125,
                fuzzy_target["mangled"]: 46.125,
            },
            compiler_alias=lambda _mangled: None,
        )

        pairing = pair(artifacts)

        self.assertEqual(pairing.pairs[exact_target["mangled"]].fuzzy, 100.0)
        self.assertEqual(pairing.pairs[fuzzy_target["mangled"]].fuzzy, 46.125)

    def test_strict_rich_pairs_enum_spelling_and_proven_call_aliases(self):
        target = self.record(
            name=(
                "void vostok::buffer_vector<stlp_std::pair<object const *,"
                "enum ignorance_type> >::push_back()"
            ),
            mangled="?target_buffer_push@@",
            text="call  target::assert_sink",
            rva=0x1000,
        )
        base = self.record(
            name=target["name"].replace("enum ", ""),
            mangled="?base_buffer_push@@",
            text="call  base::assert_sink",
            rva=0x2000,
        )
        target_assert = self.record(
            name="void target::assert_sink()",
            mangled="?target_assert@@",
            rva=0x3000,
        )
        target_assert_alias = dict(
            target_assert,
            name="void shared::assert_alias()",
        )
        base_assert = self.record(
            name="void base::assert_sink()",
            mangled="?base_assert@@",
            rva=0x4000,
        )
        base_assert_alias = dict(
            base_assert,
            name="void shared::assert_alias()",
        )
        artifacts = SimpleNamespace(
            target={target["mangled"]: target},
            base={base["mangled"]: base},
            target_records=[target, target_assert, target_assert_alias],
            base_records=[base, base_assert, base_assert_alias],
            fuzzy={},
            folded_fuzzy={},
            compiler_alias=lambda _mangled: None,
        )

        pairing = pair(artifacts)

        self.assertEqual(pairing.pairs[target["mangled"]].fuzzy, 100.0)
        self.assertEqual(
            pairing.pairs[target["mangled"]].base_rva,
            base["rva"],
        )

    def test_folded_report_alias_survives_exact_pair_on_shared_base_rva(self):
        primary_target = self.record(
            name="void primary()",
            mangled="?primary@@",
            rva=0x1000,
        )
        primary_base = self.record(
            name=primary_target["name"],
            mangled=primary_target["mangled"],
            rva=0x2000,
        )
        folded_target = self.record(
            name="void folded(enum state)",
            mangled="?folded_target@@",
            text="ret   8",
            rva=0x3000,
        )
        folded_base_alias = self.record(
            name="void folded(state)",
            mangled=primary_base["mangled"],
            rva=primary_base["rva"],
        )
        artifacts = SimpleNamespace(
            target={
                primary_target["mangled"]: primary_target,
                folded_target["mangled"]: folded_target,
            },
            base={primary_base["mangled"]: primary_base},
            target_records=[primary_target, folded_target],
            base_records=[primary_base, folded_base_alias],
            fuzzy={primary_target["mangled"]: 100.0},
            folded_fuzzy={folded_target["mangled"]: 48.25},
            compiler_alias=lambda _mangled: None,
        )

        pairing = pair(artifacts)

        self.assertEqual(pairing.pairs[folded_target["mangled"]].fuzzy, 48.25)
        self.assertEqual(
            pairing.pairs[folded_target["mangled"]].base_rva,
            primary_base["rva"],
        )

    def test_cross_unit_exact_recovers_reviewed_zero_source_pdb_gap(self):
        scalar = "??_Gstage_ambient_occlusion@@UAEPAXI@Z"
        vector = "??_Estage_ambient_occlusion@@UAEPAXI@Z"
        target = self.record(
            mangled=scalar,
            file="vostok/math_color.h",
            rva=0x1000,
        )
        target["statements"] = [{"off": 0, "size": 3, "line": 0}]
        source = "vostok/render/engine/sources/stage_ambient_occlusion.h"
        artifacts = SimpleNamespace(
            target={scalar: target},
            base={},
            target_records=[target],
            base_records=[],
            fuzzy={scalar: 100.0},
            folded_fuzzy={},
            cross_unit_fuzzy={vector: 100.0},
            source_overrides={scalar: source},
            compiler_alias=lambda _mangled: None,
        )

        pairing = pair(artifacts)

        recovered = pairing.pairs[scalar]
        self.assertEqual(recovered.fuzzy, 100.0)
        self.assertEqual(recovered.cls, "MATCH")
        self.assertLess(recovered.base_rva, 0)
        self.assertEqual(pairing.base_record_for[scalar]["file"], source)

    def test_cross_unit_exact_does_not_synthesize_a_source_body(self):
        mangled = "?ordinary_function@@YAXXZ"
        target = self.record(mangled=mangled, rva=0x1000)
        target["statements"] = [
            {"off": 0, "size": 1, "line": 10},
            {"off": 1, "size": 1, "line": 11},
            {"off": 2, "size": 1, "line": 12},
        ]
        artifacts = SimpleNamespace(
            target={mangled: target},
            base={},
            target_records=[target],
            base_records=[],
            fuzzy={mangled: 100.0},
            folded_fuzzy={},
            cross_unit_fuzzy={mangled: 100.0},
            source_overrides={mangled: "vostok/render/example.h"},
            compiler_alias=lambda _mangled: None,
        )

        self.assertNotIn(mangled, pair(artifacts).pairs)

    def test_finds_exact_alias_already_represented_by_paired_rva(self):
        base = self.record(rva=0x2000)
        target = self.record(rva=0x1000)
        aliases = {base["name"]: {target["rva"]: target}}

        self.assertEqual(
            exact_paired_source_alias(base, aliases, {target["rva"]}),
            target,
        )

    def test_rejects_exact_alias_at_unpaired_rva(self):
        base = self.record(rva=0x2000)
        target = self.record(rva=0x1000)
        aliases = {base["name"]: {target["rva"]: target}}

        self.assertIsNone(exact_paired_source_alias(base, aliases, set()))


class DynamicThunkAliasTests(unittest.TestCase):
    def test_thunkfunc_method_template_static_canons_match_across_spellings(self):
        target = (
            "`dynamic initializer for 'Scaleform::GFx::AS3::ThunkFunc0<"
            "Scaleform::GFx::AS3::Instances::fl::Date,0,double>::Method''"
        )
        base = (
            "??__E?Method@?$ThunkFunc0@VDate@fl@Instances@AS3@GFx@Scaleform@@"
            "$0A@N@AS3@GFx@Scaleform@@2Q8Date@fl@Instances@234@AEXAAN@Z"
            "Q567234@@@YAXXZ"
        )
        expected = ("E", "?TF0:Scaleform::GFx::AS3::Instances::fl::Date:0")
        self.assertEqual(dyn_canon_rich(target), expected)
        self.assertEqual(dyn_canon_base(base), expected)

    def test_thunkfunc_mangled_int_ordinals_decode(self):
        base = (
            "??__E?Method@?$ThunkFunc0@VDate@fl@Instances@AS3@GFx@Scaleform@@"
            "$0{}N@@@YAXXZ"
        )
        for encoded, ordinal in [("A@", 0), ("0", 1), ("9", 10), ("L@", 11)]:
            canon = dyn_canon_base(base.format(encoded))
            self.assertIsNotNone(canon, encoded)
            self.assertTrue(canon[1].endswith(f":{ordinal}"), (encoded, canon))

    def test_nested_scope_static_initializer_canons_match(self):
        target = (
            "`dynamic initializer for '"
            "Scaleform::GFx::AS3::InstanceTraits::fl::Date::ti''"
        )
        base = (
            "??__E?ti@Date@fl@InstanceTraits@AS3@GFx@Scaleform@@"
            "2QBUThunkInfo@456@B@@YAXXZ"
        )
        expected = ("E", "Scaleform::GFx::AS3::InstanceTraits::fl::Date::ti")
        self.assertEqual(dyn_canon_rich(target), expected)
        self.assertEqual(dyn_canon_base(base), expected)

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
        self.assertEqual(dyn_canon_rich(target), expected)
        self.assertEqual(dyn_canon_base(base), expected)

    def test_allows_missing_synthetic_owner_but_rejects_known_mismatch(self):
        target = {"file": "vostok/animation/anim_track_common.h"}
        global_canon = ("F", "vostok::sound::s_debug_audio")
        local_canon = (
            "F",
            "`survarium::weapon_cook::register_cooks_for_logic_states'::`2'::"
            "s_fire_cook",
        )
        self.assertTrue(
            dyn_owner_compatible(target, {"file": None}, global_canon)
        )
        self.assertTrue(dyn_owner_compatible(target, dict(target), global_canon))
        self.assertTrue(
            dyn_owner_compatible(
                target, {"file": "vostok/game/sources/weapon_cook.cpp"}, local_canon
            )
        )
        self.assertFalse(
            dyn_owner_compatible(
                target,
                {"file": "vostok/game/sources/weapon_cook.cpp"},
                global_canon,
            )
        )

    def test_initializer_owner_proves_folded_atexit_source(self):
        target = [{
            "mangled": "ns::`dynamic initializer for 'value''",
            "file": "vostok/game_core/value.cpp",
        }]
        base = [{
            "mangled": "`dynamic initializer for 'ns::value''",
            "file": "vostok/game_core/value.cpp",
        }]

        self.assertEqual(
            matching_dynamic_initializer_owners(target, base),
            {("ns::value", "vostok/game_core/value.cpp")},
        )

        base[0]["file"] = "vostok/other/value.cpp"
        self.assertEqual(
            matching_dynamic_initializer_owners(target, base),
            set(),
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

        owners = dynamic_local_owner_modules(records)

        self.assertEqual(
            owners[dyn_canon_rich(thunk)],
            "game",
        )

    def test_exact_rich_stream_fills_only_missing_dynamic_pair_score(self):
        target = {"size": 1, "instructions": [{"off": 0, "len": 1, "text": "ret"}]}
        base = dict(target)
        self.assertEqual(
            dynamic_pair_score("target", "base", target, base, {}),
            100.0,
        )
        self.assertEqual(
            dynamic_pair_score(
                "target", "base", target, base, {"target": 75.0}
            ),
            75.0,
        )
        different = {
            "size": 1,
            "instructions": [{"off": 0, "len": 1, "text": "int 3"}],
        }
        self.assertIsNone(
            dynamic_pair_score("target", "base", target, different, {})
        )

    def test_exact_dynamic_stream_accepts_local_ordinals_and_pdb_call_aliases(self):
        target = {
            "size": 11,
            "instructions": [
                {"off": 0, "len": 5, "text": "mov   ecx, $S3"},
                {"off": 5, "len": 5, "text": "call  target::finalize"},
                {"off": 10, "len": 1, "text": "ret"},
            ],
        }
        base = {
            "size": 11,
            "instructions": [
                {"off": 0, "len": 5, "text": "mov   ecx, S4"},
                {"off": 5, "len": 5, "text": "call  base::finalize"},
                {"off": 10, "len": 1, "text": "ret"},
            ],
        }

        self.assertEqual(
            dynamic_pair_score(
                "target",
                "base",
                target,
                base,
                {},
                lambda left, right: (left, right)
                == ("target::finalize", "base::finalize"),
            ),
            100.0,
        )

    def test_dynamic_stream_rejects_unproved_nonlocal_relocation(self):
        target = {
            "size": 6,
            "instructions": [
                {"off": 0, "len": 5, "text": "mov   eax, target_global"},
                {"off": 5, "len": 1, "text": "ret"},
            ],
        }
        base = {
            "size": 6,
            "instructions": [
                {"off": 0, "len": 5, "text": "mov   eax, base_global"},
                {"off": 5, "len": 1, "text": "ret"},
            ],
        }

        self.assertIsNone(
            dynamic_pair_score(
                "target", "base", target, base, {}, lambda _left, _right: False
            )
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
                load_module_ownership_overrides(path),
                {"?symbol@@": "game_core"},
            )
            self.assertEqual(
                load_source_ownership_overrides(path),
                {"?symbol@@": "vostok/game_core/example.h"},
            )
            path.write_text(
                "?symbol@@\tanimation\tvostok/game_core/example.h\n",
                encoding="utf-8",
            )
            with self.assertRaisesRegex(ValueError, "inconsistent module ownership"):
                load_module_ownership_overrides(path)

    def test_logical_module_prefers_reviewed_owner_over_folded_unit(self):
        record = {"file": "vostok/animation/folded_inline.h"}
        self.assertEqual(
            logical_module(
                "?symbol@@",
                record,
                [record["file"]],
                {},
                {"?symbol@@": "game_core"},
            ),
            "game_core",
        )


class ReadmeScoreProvenanceTests(unittest.TestCase):
    """The score block must never print numbers off a roster it does not name.

    A wrong path once produced an empty roster and a block reading "17,585 /
    25,372 functions" (report.json's COMDAT roster) while its own first line
    still said the figures came from the matching record. There is no fallback
    now: an unreadable ledger raises and `vostok build` keeps the last true block.
    """

    def _stats(self, path):
        from vostok.ledger import readme
        with mock.patch.object(readme, "MATCH_STATE", Path(path)):
            return readme.module_stats()

    def test_missing_ledger_raises_and_creates_nothing(self):
        from vostok.ledger import readme
        with tempfile.TemporaryDirectory() as d:
            missing = Path(d) / "not-a-ledger.tsv"
            with self.assertRaises(readme.ScoreDataUnavailable):
                self._stats(missing)
            self.assertFalse(missing.exists(),
                             "must not create a phantom empty ledger")

    def test_empty_ledger_raises(self):
        from vostok.ledger import readme
        with tempfile.TemporaryDirectory() as d:
            empty = Path(d) / "empty.tsv"
            empty.write_text("")
            with self.assertRaises(readme.ScoreDataUnavailable):
                self._stats(empty)

    def test_header_only_ledger_raises(self):
        from vostok.ledger import readme, store
        with tempfile.TemporaryDirectory() as d:
            ledger = Path(d) / "state.tsv"
            store.save({}, str(ledger))
            with self.assertRaises(readme.ScoreDataUnavailable):
                self._stats(ledger)

    def test_gfx_dependency_is_excluded_from_engine_rollup(self):
        from vostok.ledger import readme, store
        with tempfile.TemporaryDirectory() as d:
            ledger = Path(d) / "state.tsv"
            rows = {
                "?engine@@": {
                    "mangled": "?engine@@", "unit": "vostok/render/engine.cpp",
                    "module": "render", "cur": 50.0, "max": 50.0,
                    "size": 10, "hash": "engine",
                },
                "?gfx@@": {
                    "mangled": "?gfx@@", "unit": "src/gfx/player.cpp",
                    "module": "gfx", "cur": 100.0, "max": 100.0,
                    "size": 1000, "hash": "vendor",
                },
            }
            store.save(rows, str(ledger))
            with mock.patch.object(readme, "MATCH_STATE", ledger), \
                    mock.patch.object(
                        readme, "_unit_counts", return_value={"render": 1, "gfx": 1}
                    ):
                stats = readme.module_stats()

            self.assertNotIn("gfx", stats)
            self.assertEqual(stats["OVERALL"]["total_funcs"], 1)
            self.assertEqual(stats["OVERALL"]["fuzzy_cur"], 50.0)


class LedgerProjectionTests(unittest.TestCase):
    """`store.project` is the build path: a fresh derivation folded ONTO the
    committed record. What the derivation measures, it owns; what the campaign
    proved, it must not be able to destroy."""

    MANGLED = "?f@@YAXXZ"

    @staticmethod
    def observation(**overrides):
        row = {
            "mangled": LedgerProjectionTests.MANGLED, "unit": "vostok/x/f.cpp",
            "module": "x", "size": 32, "frameless": False, "cls": "SIZE",
            "cur": None, "max": None, "hash": "",
        }
        row.update(overrides)
        return row

    @staticmethod
    def banked(**overrides):
        row = {
            "mangled": LedgerProjectionTests.MANGLED, "unit": "vostok/x/f.cpp",
            "module": "x", "status": "inprogress", "cls": "SIZE", "cur": 75.0,
            "max": 75.0, "hist": 75.0, "tries": 0, "size": 32, "flags": "",
            "hash": "h", "note": "",
        }
        row.update(overrides)
        return row

    def project(
        self,
        observations,
        previous=None,
        banked_previous=None,
        authoritative_roster=None,
    ):
        """Write `previous`, project `observations` onto it, read the result."""
        with tempfile.TemporaryDirectory() as d:
            ledger = str(Path(d) / "state.tsv")
            if previous is not None:
                store.save({r["mangled"]: r for r in previous}, ledger)
            proof = None
            if banked_previous is not None:
                proof = {r["mangled"]: r for r in banked_previous}
            store.project(
                observations,
                ledger,
                banked_previous=proof,
                authoritative_roster=authoritative_roster,
            )
            return store.load(ledger)

    def test_a_changed_body_resets_max_but_never_tries_or_hist(self):
        rows = self.project(
            [self.observation(cur=75.0, max=75.0, hash="newhash")],
            [self.banked(cur=100.0, max=100.0, hist=100.0, tries=3, status="done")],
        )
        row = rows[self.MANGLED]
        self.assertEqual(row["max"], 75.0, "a new hash resets max to what we measured")
        self.assertEqual(row["tries"], 3, "a changed body must NOT bump tries")
        self.assertEqual(row["hist"], 100.0, "hist never falls")
        self.assertEqual(row["status"], "inprogress")

    def test_same_body_keeps_the_proven_peak_over_a_lower_observation(self):
        rows = self.project(
            [self.observation(cur=62.5, max=62.5, hash="h")],
            [self.banked(max=100.0, hist=100.0, status="done")],
        )
        row = rows[self.MANGLED]
        self.assertEqual(row["max"], 100.0, "the ledger owns the peak for this body")
        self.assertEqual(row["cur"], 62.5)
        self.assertEqual(row["status"], "done")

    def test_an_uncommitted_probe_cannot_raise_the_committed_peak(self):
        rows = self.project(
            [self.observation(cur=62.5, max=62.5, hash="h")],
            [self.banked(cur=62.5, max=100.0, hist=100.0, status="done")],
            [self.banked(cur=62.5, max=75.0, hist=75.0)],
        )
        row = rows[self.MANGLED]
        self.assertEqual(row["max"], 75.0)
        self.assertEqual(row["hist"], 75.0)
        self.assertEqual(row["status"], "inprogress")

    def test_working_annotations_survive_a_committed_proof_fold(self):
        rows = self.project(
            [self.observation(cur=75.0, max=75.0, hash="h")],
            [self.banked(status="parked", tries=4, note="reviewed wall")],
            [self.banked(status="inprogress", tries=3, note="")],
        )
        row = rows[self.MANGLED]
        self.assertEqual(row["status"], "parked")
        self.assertEqual(row["tries"], 4)
        self.assertEqual(row["note"], "reviewed wall")

    def test_generated_status_drift_does_not_unpark_a_committed_wall(self):
        rows = self.project(
            [self.observation(cur=75.0, max=75.0, hash="h")],
            [self.banked(status="inprogress", note="reviewed wall")],
            [self.banked(status="parked", note="reviewed wall")],
        )
        self.assertEqual(rows[self.MANGLED]["status"], "parked")

    def test_an_unobserved_function_keeps_its_peak_and_the_body_it_proved_it_on(self):
        """max and hash only mean anything together, so they travel together."""
        rows = self.project(
            [self.observation(cur=None, cls=None)],
            [self.banked(cur=None, max=100.0, hist=100.0, hash="proven")],
        )
        row = rows[self.MANGLED]
        self.assertEqual((row["max"], row["hash"]), (100.0, "proven"))
        self.assertEqual(row["status"], "done")

    def test_a_row_this_build_never_saw_survives_intact(self):
        rows = self.project([], [self.banked(max=100.0, hist=100.0, note="banked")])
        self.assertEqual(set(rows), {self.MANGLED})
        self.assertEqual(rows[self.MANGLED]["note"], "banked")
        self.assertEqual(rows[self.MANGLED]["max"], 100.0)

    def test_authoritative_roster_retires_an_obsolete_identity(self):
        rows = self.project(
            [],
            [self.banked(max=100.0, hist=100.0, note="stale spelling")],
            authoritative_roster=set(),
        )
        self.assertEqual(rows, {})

    def test_authoritative_roster_keeps_a_current_unobserved_identity(self):
        rows = self.project(
            [],
            [self.banked(max=100.0, hist=100.0, note="current target")],
            authoritative_roster={self.MANGLED},
        )
        self.assertEqual(set(rows), {self.MANGLED})

    def test_a_hand_park_survives_the_next_build(self):
        """The regression that used to need a write-through to the cache: `ledger
        park` edited the record, and the build then re-derived the row without it."""
        rows = self.project(
            [self.observation(cur=75.0, max=75.0, hash="h")],
            [self.banked(status="parked", tries=4, note="parked by hand")],
        )
        row = rows[self.MANGLED]
        self.assertEqual(row["status"], "parked")
        self.assertEqual(row["note"], "parked by hand")
        self.assertEqual(row["tries"], 4)

    def test_a_park_that_reaches_100_reads_done(self):
        rows = self.project(
            [self.observation(cur=100.0, max=100.0, hash="h")],
            [self.banked(status="parked", note="LTCG wall")],
        )
        self.assertEqual(rows[self.MANGLED]["status"], "done")

    def test_an_unpaired_function_is_blocked_and_carries_the_roster_columns(self):
        rows = self.project([self.observation(cur=None, cls=None, frameless=True)])
        row = rows[self.MANGLED]
        self.assertEqual(row["status"], "blocked")
        self.assertEqual(row["flags"], "f")
        self.assertEqual(row["unit"], "vostok/x/f.cpp")
        self.assertEqual(row["size"], 32)
        self.assertIsNone(row["cur"])

    def test_a_first_observation_seeds_max_and_hist_from_cur(self):
        rows = self.project([self.observation(cur=87.5, max=87.5, hash="h")])
        row = rows[self.MANGLED]
        self.assertEqual((row["max"], row["hist"]), (87.5, 87.5))
        self.assertEqual(row["status"], "inprogress")

    def test_exact_max_is_persisted_as_literal_100(self):
        rows = self.project(
            [self.observation(cur=99.9982, max=99.9982, hash="h")]
        )
        row = rows[self.MANGLED]
        self.assertEqual(row["cur"], 99.9982)
        self.assertEqual((row["max"], row["hist"]), (100.0, 100.0))
        self.assertEqual(row["status"], "done")


class LedgerUnitEncodingTests(unittest.TestCase):
    """`unit` holds the TU path, and a new unit must not disturb other rows."""

    @staticmethod
    def _rows(*pairs):
        return {m: {"mangled": m, "unit": u, "module": "game", "status": "done",
                    "cls": "MATCH", "cur": 100.0, "max": 100.0, "hist": 100.0,
                    "tries": 0, "size": 8, "flags": "", "hash": "abc", "note": ""}
                for m, u in pairs}

    def test_round_trips_the_path(self):
        from vostok.ledger import store
        with tempfile.TemporaryDirectory() as d:
            p = os.path.join(d, "l.tsv")
            store.save(self._rows(("?a@@YAXXZ", "vostok/game/sources/player.cpp")), p)
            self.assertEqual(store.load(p)["?a@@YAXXZ"]["unit"],
                             "vostok/game/sources/player.cpp")

    def test_a_new_early_sorting_unit_touches_only_its_own_row(self):
        from vostok.ledger import store
        with tempfile.TemporaryDirectory() as d:
            p = os.path.join(d, "l.tsv")
            rows = self._rows(("?a@@YAXXZ", "vostok/z.cpp"),
                              ("?b@@YAXXZ", "vostok/y.cpp"))
            store.save(rows, p)
            before = open(p).read().splitlines()
            # a unit that sorts BEFORE every existing one: the case that used to
            # renumber the whole legend and rewrite all 19k rows
            rows["?a@@YAXXZ"]["unit"] = "aaa/brand_new.cpp"
            store.save(rows, p)
            after = open(p).read().splitlines()
            changed = sum(1 for x, y in zip(before, after) if x != y)
            self.assertEqual(len(before), len(after))
            self.assertEqual(changed, 1)

    def test_the_retired_legend_encoding_still_loads(self):
        from vostok.ledger import store
        with tempfile.TemporaryDirectory() as d:
            p = os.path.join(d, "old.tsv")
            with open(p, "w") as fh:
                fh.write(store.HEADER)
                fh.write("# [units] 0\tvostok/game/sources/player.cpp\n")
                fh.write("\t".join(store.COLUMNS) + "\n")
                fh.write("?a@@YAXXZ\t0\tgame\tdone\tMATCH\t100\t100\t100\t0\t8\t\tabc\t\n")
            self.assertEqual(store.load(p)["?a@@YAXXZ"]["unit"],
                             "vostok/game/sources/player.cpp")


class ReportChangesTests(unittest.TestCase):
    @staticmethod
    def _report(score):
        return {
            "measures": {
                "matched_code_percent": score or 0.0,
                "matched_functions": int(score == 100.0),
                "total_functions": 1,
            },
            "units": [
                {
                    "name": "module/header.h",
                    "functions": [
                        {
                            "name": "?function@@YAXXZ",
                            "fuzzy_match_percent": score,
                            "metadata": {"demangled_name": "void function(void)"},
                        }
                    ],
                }
            ],
        }

    def _changes(self, strict_exact):
        from vostok.build import generate_delink as G
        with tempfile.TemporaryDirectory() as temporary_directory:
            root = pathlib.Path(temporary_directory)
            previous = root / "previous.json"
            current = root / "current.json"
            previous.write_text(json.dumps(self._report(100.0)))
            current.write_text(json.dumps(self._report(None)))
            with (
                mock.patch.object(G, "OBJDIFF_DIR", root),
                mock.patch.object(
                    G,
                    "_strict_current_exact_symbols",
                    return_value=strict_exact,
                ),
            ):
                G._report_changes(previous, current)
            return json.loads((root / "report-changes.json").read_text())

    def test_rich_exact_owner_swap_is_fold_churn(self):
        changes = self._changes({"?function@@YAXXZ"})
        self.assertEqual(changes["regressed"], [])
        self.assertEqual(len(changes["fold_churn"]), 1)

    def test_unproven_drop_remains_a_regression(self):
        changes = self._changes(set())
        self.assertEqual(len(changes["regressed"]), 1)
        self.assertEqual(changes["fold_churn"], [])

    def test_zero_change_skips_rich_index_scan(self):
        from vostok.build import generate_delink as G
        with tempfile.TemporaryDirectory() as temporary_directory:
            root = pathlib.Path(temporary_directory)
            previous = root / "previous.json"
            current = root / "current.json"
            previous.write_text(json.dumps(self._report(100.0)))
            current.write_text(json.dumps(self._report(100.0)))
            with (
                mock.patch.object(G, "OBJDIFF_DIR", root),
                mock.patch.object(G, "_strict_current_exact_symbols") as scan,
            ):
                G._report_changes(previous, current)
            scan.assert_not_called()


class ReportArchivePruneTests(unittest.TestCase):
    """The archive is a ring: ~14 MB per build reached 11 GB unbounded."""

    @staticmethod
    def _archive(d, n):
        for i in range(n):
            (pathlib.Path(d) / f"report-202608{i//24+1:02d}-{i%24:02d}0000.json").write_text("{}")
        return pathlib.Path(d)

    def test_keeps_only_the_newest_n(self):
        from vostok.build import generate_delink as G
        with tempfile.TemporaryDirectory() as d:
            a = self._archive(d, 50)
            with mock.patch.object(G, "KEEP_REPORTS", 10):
                G._prune_reports(a)
            self.assertEqual(len(list(a.glob("report-*.json"))), 10)

    def test_the_newest_survives(self):
        """_report_changes diffs against it - pruning it would break the build."""
        from vostok.build import generate_delink as G
        with tempfile.TemporaryDirectory() as d:
            a = self._archive(d, 50)
            newest = sorted(p.name for p in a.glob("report-*.json"))[-1]
            with mock.patch.object(G, "KEEP_REPORTS", 3):
                G._prune_reports(a)
            self.assertTrue((a / newest).exists())

    def test_zero_keeps_everything(self):
        from vostok.build import generate_delink as G
        with tempfile.TemporaryDirectory() as d:
            a = self._archive(d, 20)
            with mock.patch.object(G, "KEEP_REPORTS", 0):
                G._prune_reports(a)
            self.assertEqual(len(list(a.glob("report-*.json"))), 20)

    def test_fewer_than_the_cap_is_a_no_op(self):
        from vostok.build import generate_delink as G
        with tempfile.TemporaryDirectory() as d:
            a = self._archive(d, 4)
            with mock.patch.object(G, "KEEP_REPORTS", 10):
                G._prune_reports(a)
            self.assertEqual(len(list(a.glob("report-*.json"))), 4)



if __name__ == "__main__":
    unittest.main()
