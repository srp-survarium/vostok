# bullet_manager::free_bullet (0x5913f0)

~89.30% PARTIAL (marker was stale 87.80%). Structure MATCH (4/4 stmts), one SIZE.

## Residual (non-steerable wall)
SIZE on `VOSTOK_DELETE_IMPL( *m_bullets_allocator_ref, bullet )`.
- TARGET calls the `delete_helper<allocator,bullet>` WRAPPER out-of-line (rva
  0xae9f0); the wrapper materializes a `call_destructor_predicate` and forwards to
  `delete_helper_impl<...,call_destructor_predicate>` (rva 0xaed80). So target pushes
  a 3rd arg (the predicate bool at [ebp-9]) and `add esp,0Ch`.
- BASE has NO `delete_helper` wrapper symbol (only delete_helper_impl @0x7e150); the
  wrapper inlined away, so base calls delete_helper_impl directly with 2 args
  (`add esp,8`).
Whole-program LTCG inline-vs-call of the wrapper template. Source `VOSTOK_DELETE_IMPL`
correct. The `mov byte[ebp-1],0; lea eax; call empty_stub` before it is the
compiled-out ASSERT, present in BOTH.

## Commands
pdb_fetch --function free_bullet --view structure-diff/diff/target/base
pdb_rich_query --function delete_helper --list (target wrapper 0xae9f0, base none)
