# SPDX-License-Identifier: GPL-3.0-or-later

"""Build-scoped reuse, strict batched inspection and timing failure contracts."""

import contextlib
import io
import subprocess
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest import mock

from vostok.build import rebuild
from vostok.core import log, symbols
from vostok.data import gate, pipeline, render_relocs
from vostok.derive import roster
from vostok.ledger import readme


class SymbolBatchTests(unittest.TestCase):
    def test_exact_names_and_file_identity(self):
        objects = [Path('/objects/a: name.obj'), Path('/objects/b.obj')]
        output = (
            f"{objects[0]}: 00000000 T name with spaces::and: punctuation\n"
            f"{objects[1]}:          U ?undefined@@\n"
            f"{objects[1]}: 00000004 R ?defined@@\n"
        )
        with mock.patch.object(symbols.subprocess, 'run') as run:
            run.return_value.stdout = output
            self.assertEqual(symbols.object_symbol_tables('nm', objects), {
                objects[0]: {'name with spaces::and: punctuation'},
                objects[1]: {'?undefined@@', '?defined@@'},
            })
            self.assertEqual(run.call_count, 1)
            self.assertIn('--no-demangle', run.call_args.args[0])
            self.assertTrue(run.call_args.kwargs['check'])

    def test_empty_inventory_does_not_launch(self):
        with mock.patch.object(symbols.subprocess, 'run') as run:
            self.assertEqual(symbols.object_symbol_tables('nm', []), {})
            run.assert_not_called()

    def test_objects_without_symbols_remain_in_inventory(self):
        obj = Path('/objects/empty.obj')
        with mock.patch.object(symbols.subprocess, 'run') as run:
            run.return_value.stdout = ''
            self.assertEqual(symbols.object_symbol_tables('nm', [obj]), {obj: set()})

    def test_batches_are_bounded_by_count_and_argument_bytes(self):
        for objects in (
            [Path(f'/objects/{index}.obj') for index in range(600)],
            [Path('/objects/' + 'x' * 1000 + f'{index}.obj') for index in range(100)],
        ):
            with self.subTest(count=len(objects)):
                with mock.patch.object(symbols.subprocess, 'run') as run:
                    run.return_value.stdout = ''
                    self.assertEqual(len(symbols.object_symbol_tables('nm', objects)),
                                     len(objects))
                    self.assertGreater(run.call_count, 1)
                    inspected = []
                    for call in run.call_args_list:
                        batch = call.args[0][4:]
                        self.assertLessEqual(len(batch), 256)
                        self.assertLessEqual(sum(len(name.encode()) + 1 for name in batch),
                                             32768)
                        inspected.extend(batch)
                    self.assertEqual(inspected, list(map(str, objects)))

    def test_unknown_or_malformed_output_is_an_error(self):
        obj = Path('/objects/a.obj')
        for output in ('/other.obj: 00000000 T symbol', f'{obj}: garbage', 'garbage'):
            with self.subTest(output=output):
                with mock.patch.object(symbols.subprocess, 'run') as run:
                    run.return_value.stdout = output
                    with self.assertRaisesRegex(RuntimeError, 'unexpected llvm-nm output'):
                        symbols.object_symbol_tables('nm', [obj])

    def test_failed_inventory_never_starts_renaming(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            (root / 'a.obj').write_bytes(b'original')
            with mock.patch.object(symbols.subprocess, 'run') as run:
                run.side_effect = subprocess.CalledProcessError(
                    1, ['nm'], stderr='a.obj: invalid object'
                )
                with self.assertRaisesRegex(RuntimeError, 'invalid object'):
                    symbols.normalize_tree(root, nm='nm', objcopy='objcopy')
                self.assertEqual(run.call_count, 1)
                self.assertEqual((root / 'a.obj').read_bytes(), b'original')

    def test_interruption_propagates(self):
        with mock.patch.object(symbols.subprocess, 'run', side_effect=KeyboardInterrupt):
            with self.assertRaises(KeyboardInterrupt):
                symbols.object_symbol_tables('nm', [Path('/objects/a.obj')])

    def test_later_batch_failure_cannot_leave_renames(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            for index in range(257):
                (root / f'{index:03}.obj').write_bytes(b'original')
            name = "vostok::`dynamic initializer for 'value''"
            first = SimpleNamespace(stdout=f"{root / '000.obj'}: 00000000 T {name}\n")
            error = subprocess.CalledProcessError(1, ['nm'], stderr='last object missing')
            with mock.patch.object(symbols.subprocess, 'run', side_effect=[first, error]) as run:
                with self.assertRaisesRegex(RuntimeError, 'last object missing'):
                    symbols.normalize_tree(root, nm='nm', objcopy='objcopy')
                self.assertEqual(run.call_count, 2)
                self.assertTrue(all(path.read_bytes() == b'original' for path in root.iterdir()))


class PreparationTests(unittest.TestCase):
    def test_standalone_refresh_prepares_once(self):
        prepared = pipeline.PreparedManifests({'consumer_units': 12})
        with (
            mock.patch.object(pipeline, 'prepare_manifests', return_value=prepared) as prepare,
            mock.patch.object(pipeline, '_refresh_prepared', return_value={'ok': True}) as finish,
        ):
            self.assertEqual(pipeline.refresh(), {'ok': True})
            prepare.assert_called_once_with()
            finish.assert_called_once_with(prepared)

    def test_build_refresh_reuses_explicit_success(self):
        prepared = pipeline.PreparedManifests({})
        with (
            mock.patch.object(pipeline, 'prepare_manifests') as prepare,
            mock.patch.object(pipeline, '_refresh_prepared') as finish,
        ):
            pipeline.refresh(prepared=prepared)
            prepare.assert_not_called()
            finish.assert_called_once_with(prepared)

    def test_preparation_failure_cannot_refresh_old_outputs(self):
        with (
            mock.patch.object(pipeline, 'prepare_manifests', side_effect=RuntimeError('missing input')),
            mock.patch.object(pipeline, '_refresh_prepared') as finish,
        ):
            with self.assertRaisesRegex(RuntimeError, 'missing input'):
                pipeline.refresh()
            finish.assert_not_called()


class AuditReuseTests(unittest.TestCase):
    def test_supplied_content_keys_avoid_recomputation(self):
        empty_index = SimpleNamespace(exact={})
        with mock.patch.object(render_relocs, '_content_keys', return_value=set()) as compute:
            baseline = render_relocs._function_data_rows([], empty_index, empty_index, {})
            self.assertEqual(compute.call_count, 2)
            compute.reset_mock()
            reused = render_relocs._function_data_rows(
                [], empty_index, empty_index, {},
                content_keys=(frozenset(), frozenset()),
            )
            self.assertEqual(reused, baseline)
            compute.assert_not_called()

    def test_context_computes_shared_inputs_once(self):
        with contextlib.ExitStack() as stack:
            for name in ('_datum_index', '_function_token_index', '_direct_call_graph'):
                stack.enter_context(mock.patch.object(render_relocs, name, return_value={}))
            stack.enter_context(mock.patch.object(render_relocs.sema_pairing, 'Pairing'))
            stack.enter_context(mock.patch.object(render_relocs.store, 'load', return_value={}))
            stack.enter_context(mock.patch.object(render_relocs, 'PEImage'))
            stack.enter_context(mock.patch.object(render_relocs.pipeline, 'image_paths',
                                                   return_value=(Path('/image'), Path('/pdb'))))
            for name in ('_comparison_symbols', '_augment_comparison_symbols'):
                stack.enter_context(mock.patch.object(render_relocs.pipeline, name,
                                                       return_value=({}, {})))
            stack.enter_context(mock.patch.object(render_relocs, '_load_site_inventory',
                                                   return_value=({}, {})))
            stack.enter_context(mock.patch.object(render_relocs.data_reviews, 'load', return_value={}))
            inputs = stack.enter_context(mock.patch.object(render_relocs, '_inputs',
                                                           return_value={'image': 'hash'}))
            content = stack.enter_context(mock.patch.object(render_relocs, '_content_keys',
                                                            side_effect=[{'target'}, {'base'}]))
            context = render_relocs.build_audit_context()
            inputs.assert_called_once_with()
            self.assertEqual(content.call_count, 2)
            self.assertEqual(context.inputs, {'image': 'hash'})
            self.assertEqual(context.content_keys, (frozenset({'target'}), frozenset({'base'})))


class TimingTests(unittest.TestCase):
    def test_return_and_elapsed(self):
        output = io.StringIO()
        sentinel = object()
        with contextlib.redirect_stdout(output), mock.patch.object(
            log.time, 'monotonic', side_effect=[10, 12]
        ):
            self.assertIs(log.timed('phase', lambda: sentinel), sentinel)
        self.assertIn('phase: OK in 2.0s', output.getvalue())

    def test_failure_and_interruption_preserve_exception(self):
        for error, label in ((RuntimeError('failed'), 'FAILED'),
                             (KeyboardInterrupt(), 'INTERRUPTED'),
                             (SystemExit(7), 'FAILED')):
            output = io.StringIO()
            with contextlib.redirect_stdout(output):
                with self.assertRaises(type(error)) as caught:
                    log.timed('phase', mock.Mock(side_effect=error))
            self.assertIs(caught.exception, error)
            self.assertIn(f'phase: {label}', output.getvalue())

    def test_logging_failure_does_not_mask_original(self):
        error = RuntimeError('original')
        with mock.patch.object(log, 'logger', side_effect=BrokenPipeError):
            with self.assertRaises(RuntimeError) as caught:
                log.timed('phase', mock.Mock(side_effect=error))
        self.assertIs(caught.exception, error)


class BuildDependencyTests(unittest.TestCase):
    def setUp(self):
        self.stack = contextlib.ExitStack()
        self.addCleanup(self.stack.close)
        self.stack.enter_context(contextlib.redirect_stdout(io.StringIO()))
        self.stack.enter_context(contextlib.redirect_stderr(io.StringIO()))
        self.stack.enter_context(mock.patch.object(rebuild.sys, 'argv', ['build']))
        self.patch(rebuild, '_background_dispatch', return_value=False)
        self.patch(rebuild, '_acquire_build_lock')
        self.patch(rebuild, '_append_log')
        self.patch(rebuild, '_write_build_head')
        self.patch(rebuild.ninja_regen, 'regenerate')
        self.patch(rebuild, 'run_ninja', return_value=set())
        self.patch(rebuild.generate_structure, 'generate')
        self.pdb = self.patch(rebuild.generate_pdb, 'generate')
        self.delink = self.patch(rebuild.generate_delink, 'generate')
        self.prepared = pipeline.PreparedManifests({})
        self.prepare = self.patch(pipeline, 'prepare_manifests', return_value=self.prepared)
        self.refresh = self.patch(pipeline, 'refresh')
        self.patch(roster, 'regen')
        self.patch(gate, 'refresh', return_value={
            'summary': {'modules': 34, 'open_function_data': 14},
        })
        self.patch(readme, 'regen_readme')

    def patch(self, owner, name, **kwargs):
        return self.stack.enter_context(mock.patch.object(owner, name, **kwargs))

    def test_build_passes_its_one_preparation_to_refresh(self):
        rebuild.main()
        self.prepare.assert_called_once_with()
        self.refresh.assert_called_once_with(prepared=self.prepared)

    def test_pdb_failure_prevents_consuming_old_data_manifests(self):
        self.pdb.side_effect = RuntimeError('pdb failed')
        with self.assertRaises(SystemExit):
            rebuild.main()
        self.prepare.assert_not_called()
        self.delink.assert_not_called()
        self.refresh.assert_not_called()

    def test_preparation_failure_skips_data_but_reports_code_lane(self):
        self.prepare.side_effect = RuntimeError('preparation failed')
        with self.assertRaises(SystemExit):
            rebuild.main()
        self.delink.assert_called_once_with('base')
        self.refresh.assert_not_called()

    def test_code_only_skips_the_data_lane_and_readme(self):
        self.ledger = self.patch(roster, 'regen')
        self.gate = self.patch(gate, 'refresh')
        self.readme = self.patch(readme, 'regen_readme')
        seen = []
        rebuild.run_ninja.side_effect = lambda: seen.append(list(rebuild.sys.argv)) or set()
        with mock.patch.object(rebuild.sys, 'argv', ['build', '--code-only']):
            rebuild.main()
        self.assertEqual(seen, [['build']])
        self.prepare.assert_not_called()
        self.delink.assert_called_once_with('base')
        self.refresh.assert_not_called()
        self.ledger.assert_called_once_with()
        self.gate.assert_not_called()
        self.readme.assert_not_called()

    def test_full_build_still_runs_every_lane(self):
        self.ledger = self.patch(roster, 'regen')
        self.readme = self.patch(readme, 'regen_readme')
        rebuild.main()
        self.prepare.assert_called_once_with()
        self.assertEqual(self.delink.call_count, 3)
        self.ledger.assert_called_once_with()
        self.readme.assert_called_once_with()


if __name__ == '__main__':
    unittest.main()
