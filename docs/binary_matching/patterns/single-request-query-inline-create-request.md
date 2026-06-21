# Single-request `query_resources*` builds the request INLINE via `&create_request(...)`, not a named `request[]` array
tags: cpp:local cpp:temporary cpp:array | asm:mov asm:lea asm:push | topic:pdb-locals topic:source-fidelity topic:structure-shape
symptoms: target 0 named locals + 1 statement for a query call, base carries a phantom `requests[1]` local + 2 statements, request built inline on the call frame (`mov dword[esp+N],<class_id>`), SIZE deltas from create_request inline-vs-call
confidence: 9/10
variants: inline-temp-vs-named-temp-ctor-order.md, chained-temporary-params-object.md, array-walk-zero-locals-is-for-each.md

A `resources::query_resources*( requests, count, ... )` call with a SINGLE request. The base
declares a `resources::request requests[] = { create_request( path, id ) };` array (one named
local + one decl statement + one call statement). The target records 0 such locals and folds
the whole thing into ONE statement: it builds the request inline on the call frame
(`mov [esp+1Ch], <path>`, `mov dword ptr [esp+24h], <class_id>`) and passes its address with
`count == 1`. NO `request[1]` local.

The fix is the address-of-an-inline-aggregate-temporary spelling (MSVC8 `/Od` extension C4238,
"taking the address of an rvalue"):
```cpp
resources::query_resources_and_wait( &resources::create_request( cfg_name, resources::raw_data_class ), 1, boost::bind( ... ), g_allocator, NULL, NULL, assert_on_fail_false );
```
This drops the named `requests` local and collapses to ONE statement == the target's LOCALS +
statement structure. Keep the whole call on ONE line so the line table emits a single record.

Note the contrast with the MULTI-element case: `request[2] requests` (two `create_request`
rows) DOES record a `request[2]` named local in the target too (`on_render_output_window_created`,
`query_base_resources`), so the array form is CORRECT there - only the single-request case uses
the inline `&create_request(...)` temporary.

Byte residual: if `create_request` is `__declspec(noinline)` in your `resources.h` (a deliberate
match for another unit, e.g. player_parameters_modifyer_cook PR #85), the base emits
`push <id>; push <path>; call create_request` where the target builds the request inline - a
cross-module inline-vs-call wall (~14-16% residual). The STRUCTURE (0 locals, 1 stmt) is the
win; do NOT remove the noinline to chase the bytes (it's another unit's source).

Evidence: game::load_config_query 62.67->83.90%, scaleform_movie_cook::translate_query
65.77->86.03%, both phantom `requests[1]` local -> 0 (STRUCTURE/LOCALS MATCH), residual = the
create_request noinline call (game.cpp, this run).
