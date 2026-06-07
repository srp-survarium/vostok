# items_dictionary_cook::on_subresources_loaded

target 0x754630 | base 0x568710 | report.json: 94.02% (was STATE 80.43%)

## Structure
`--view structure-diff --condensed`: target 27 / base 26 stmts, 3 SIZE diffs,
3 quantity diffs. All 3 quantity diffs are `EMPTY only target|base` empty-line-run
alignment artifacts (target has one extra collapsed blank line). No real statement
count divergence -> STRUCTURE MATCH.

The ASSERT at L100 (`mov byte[ebp-1],0; lea; call empty_stub`) is matched inside a
`same` run. Upgraded the guess from bare `ASSERT( UNKNOWN_EXPRESSION )` to
`ASSERT( UNKNOWN_EXPRESSION_T( parent != NULL ) )` - `parent` is fetched immediately
above and later receives `set_unmanaged_resource` / `finish_query`, so a non-null
parent-query assert is the natural condition. The `_T` form is discarded by Master
Gold, so this is byte-identical (verified: rebuild kept the score).

## The 3 SIZE diffs (inline-vs-call / reg-alloc)
- L104 `configs::binary_config_ptr item_cfg = static_cast_resource_ptr<...>( data[i].get_unmanaged_resource() )` : 0x24 vs 0x1f - get_unmanaged_resource indexing inline.
- L116/118 `... : 0;` the combat_log_icon ternary
  `value_exists("combat_log_icon") ? (u8)...["combat_log_icon"] : 0` : 0x7a vs 0x86 -
  value_exists + operator[] inline shape.
- L127 `VOSTOK_DELETE_IMPL( g_allocator, item_dict_ids )` (u32* array delete) : 0x2e vs 0x32.
  asm diff: pure register choice (eax/edx, ecx/edx) plus one extra out-of-line
  `call strip_pointer` the target keeps and our base inlines. Same delete_helper
  family wall as delete_resource, here on the `u32` instantiation.

All three are template/macro inline-vs-call or register-allocation differences,
non-steerable from this function's source.

VERDICT: STRUCTURE MATCH - non-steerable inline-vs-call / reg-alloc SIZE diffs.
