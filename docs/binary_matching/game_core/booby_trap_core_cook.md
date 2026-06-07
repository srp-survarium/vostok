# booby_trap_core_cook.cpp - structure verification (v2 verifier-matcher)

Target rvas (from `pdb_rich_query --index binaries/rich/target/index.jsonl --function booby_trap_core_cook --list`):
- ctor                  0x751d50
- finish_query          0x751da0
- on_subresources_loaded 0x751e00
- translate_query       0x751ee0

The bare `--function <name>` lookups all resolve to the WRONG boost overload; always
pass `--rva`. (carcass `<VA>` addresses are base-build, off the target rva by ~0x10000.)

## STATE markers were badly stale
report.json fuzzy_match_percent (after rebuild):
- ctor                   66.69% (stale) -> **100.00%**
- finish_query           74.00% (stale) -> **100.00%**
- on_subresources_loaded 87.99% (stale) -> **94.31%**
- translate_query        73.88% (stale) -> **90.16%**

All four are STRUCTURE MATCH (quantity-diffs 0 in `--view structure-diff --condensed`).

## ctor (100%, clean)
structure-diff: aligned 1, size-diffs 0, quantity-diffs 0. Clean. Carcass deleted,
bare `// STATE[100%|DONE]`.

## finish_query (100%, clean)
structure-diff: aligned 2, size-diffs 0, quantity-diffs 0. Clean. Carcass deleted.

## on_subresources_loaded (94.31%)
structure-diff: aligned 9, size-diffs 1, quantity-diffs 0. Sole SIZE diff at
`configs::binary_config_ptr aabb = static_cast_resource_ptr<...>( data[0].get_unmanaged_resource() )`.
`--view diff`: target passes the `get_unmanaged_resource()` prvalue directly into the
cast; base materializes an EXTRA resource_ptr copy-construct (`push ecx; mov esi,esp` +
an extra ctor/dtor call pair), inflating the frame. This is the identical
resource_ptr-temp inline-vs-temp LTCG residual already banked on the sibling
`booby_trap_set_core_cook::on_config_ready` (same statement, 82.01% there). Non-steerable.

## translate_query (90.16%)
structure-diff: aligned 13, size-diffs 3, quantity-diffs 0. Three SIZE diffs:

1. `if ( !parent.user_data()->try_get( config ) )` - target inserts `mov ecx, eax`
   between `call user_data` and `call try_get` (passes the returned pointer as `this`
   via ecx); base's LTCG `try_get` takes `this` already-in-reg, so it drops the move.
   Pure call-boundary register-vs-slot passing -> bankable LTCG.

2. `{ aabb_path.c_str(), resources::binary_config_class_impl }` - the `request` array
   element. `request` is a plain aggregate `{ pcstr path; class_id_enum id; }`. Target
   builds it via a 2-arg `call` into a temp slot `[ebp-118h]` (delinker misnames the
   methods `fixed_string::get_buffer` + `const_buffer::const_buffer`, and the
   `push 22h` is the enum value `binary_config_class_impl`==34==0x22, NOT a buffer size).
   Base emits two direct inline stores `mov [ebp-8],eax; mov dword[ebp-4],22h`.
   TESTED `resources::create_request( aabb_path.c_str(), binary_config_class_impl )`
   (the `__declspec(noinline)` request-by-value factory) -> REGRESSED 90.16 -> 85.97,
   reverted. The aggregate-init `{ ... }` form is confirmed correct; the divergence is
   the LTCG temp-slot lowering of the aggregate, not the source spelling.

3. `);` (the `query_resources( requests, boost::bind(...), ... )` call) - the boost::bind
   closure-temp build + the request-array address handoff; base's closure temp inflates
   the frame and shifts `[ebp-XX]` slots uniformly. Same boost::bind closure-temp class
   banked on sibling `booby_trap_set_core_cook::translate_query` (95.75%). Non-steerable.

Frame: target `sub esp,178h` vs base `sub esp,17Ch` (4B), cascading from diffs 1+2.

## Verdict
All four: STRUCTURE MATCH. ctor + finish_query taken to clean 100% (carcass stripped,
bare STATE). The two PARTIALs carry an embedded condensed structure-diff + VERDICT; their
residuals are LTCG call-boundary / resource_ptr-temp / boost::bind closure-temp lowering,
all non-steerable and all previously banked on the sibling cook.
