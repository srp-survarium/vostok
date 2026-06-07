# artefact_container_core.cpp - structure verification

Verifier pass (chain/game_core-sv-artefact_container_core). No logic changed;
only carcass -> structure-diff embeds, STATE syncs, and two functions taken to a
clean 100% DONE.

Canonical fuzzy % from report.json after a clean rebuild (0 regressed / 0 improved):

| function          | old STATE marker | report.json % | verdict |
|-------------------|------------------|---------------|---------|
| load              | 99.77% DONE      | 100.0%        | clean -> bare 100% DONE |
| spawn_artefact    | 96.11% DONE      | 100.0%        | clean -> bare 100% DONE |
| use_execute       | 92.86% PARTIAL   | 92.86%        | STRUCTURE MATCH, non-steerable |
| artefact_spawned  | 64.31% PARTIAL   | 74.74%        | STRUCTURE MATCH, non-steerable (marker was stale) |
| transfer_artefact | 0.00% PARTIAL    | unpaired(~0%) / 59.28% rich-diff | STRUCTURE MATCH, non-steerable |

## load (100%)
structure-diff: aligned 2, size-diffs 0, quantity-diffs 0. Byte-perfect. Stripped
carcass, set bare `// STATE[100%|DONE]`.

## spawn_artefact (100%)
structure-diff: aligned 4, size-diffs 0, quantity-diffs 0. Byte-perfect (the
"LTCG for query_resource" residual is gone / closed). Stripped carcass, bare
`// STATE[100%|DONE]`.

## use_execute (92.86%)
structure-diff: target 20 / base 19 stmts; aligned 17, size-diffs 2, quantity-diffs 1.
- `EMPTY only target` (a collapsed source-line gap) - one no-address line target has,
  base lacks; not a real statement, an artifact of the conversion expansion below.
- `if ( m_artefact )` SIZE (0x29 vs 0x1f): target materializes a bool via a temp and
  the intrusive_ptr `unspecified_bool_type` conversion operator (the `mov [ebp-28h],0;
  cmp [ebp-28h],0; je` dance). base inlines the bool as `xor ecx,ecx; setne cl;
  movzx edx,cl; test edx,edx`. Same `if (smart_ptr)` source, different inline form of
  the conversion operator under LTCG. (sushi already flagged this in a @NOTE.)
- `transfer_artefact( ... )` SIZE (0x1c vs 0x1a): pure register allocation around the
  call - base materializes `this` into ecx via an extra `mov ecx,eax`; the arg/this
  land in different registers. LTCG arg/reg.
Non-steerable. Both causes are inline-vs-call / reg-alloc of shared engine templates;
source and headers are fixed.

## artefact_spawned (74.74%)
structure-diff: target 3 / base 3 stmts; aligned 1, size-diffs 2, quantity-diffs 0.
Same statements, same shape. Both sides emit the out-of-line
`call static_cast_resource_ptr<...>` (target at 0x2d) - so this is NOT a
static_cast_resource_ptr inline-vs-call.
- assignment line SIZE (0x6c vs 0x80): the resource_ptr -> artefact_base_ptr conversion
  chain materializes a DIFFERENT number of intrusive_ptr temporaries between target and
  base (target winds up with extra `intrusive_ptr<inventory_item>::dec` destructor calls
  / a larger frame, base fewer). Temporary materialization of the template conversion.
- `set_amount( 1 )` SIZE (0x22 vs 0x1f): target `push 1` (arg on stack) then
  `mov ecx,this`; base `mov ecx,1` (arg in register). LTCG arg passing.
The old `// STATE[64.31%]: Everything inlined differently` marker was stale (now
74.74%) and vague; replaced with the concrete cause. Non-steerable.

## transfer_artefact (report ~0% / rich-diff 59.28%)
structure-diff: target 2 / base 2 stmts; aligned 1, size-diffs 1, quantity-diffs 0.
Same shape. The sole SIZE is the conversion line (0x35 target vs 0x8f base):
- TARGET (0x54-byte body): reads `m_artefact`'s raw c_ptr (`mov ecx,[eax+20h]`) and
  builds the `inventory_item_ptr` temp via `intrusive_ptr<inventory_item>::set` - i.e.
  the resource_ptr overload `static_cast_checked<dest::object_type*>(src.c_ptr())`
  inlined to a bare pointer copy + `set`.
- BASE (0xb3-byte body): emits an out-of-line `call static_cast_resource_ptr<...>` AND
  inlines a heavier intrusive_ptr construct-with-typecheck (the `cmp ecx,[eax]` vtable
  compare, the `+0xD0` base-subobject adjust, `interlocked_increment`).
Same source `holder->take_inventory_item( static_cast_resource_ptr<inventory_item_ptr>(
m_artefact ) )`; the template conversion simply inlines to a different shape under LTCG.
The old STATE note "static_cast_resource_ptr didn't inline in target" is backwards -
the target inlined it to the SIMPLER form; base is the one that kept the out-of-line
call plus a fat copy. Non-steerable.

report.json leaves the transfer_artefact symbol unpaired (no `fuzzy_match_percent`
field -> README treats it ~0%); the rich-index `--view diff` pairs it at 59.28%. The
STATE marker uses the rich-diff number so the reader sees the real match level.

## Why no source fix
All four residuals are inline-vs-call / temporary-materialization / arg-reg decisions
on shared engine templates (`intrusive_ptr` / `resource_ptr` / `static_cast_resource_ptr`
/ the `unspecified_bool_type` conversion). The source statements and shapes already match
the target 1:1 (structure-diff aligned with 0 quantity divergences except one collapsed
EMPTY line); the only levers would be editing those engine headers, which is out of scope
and would regress every other consumer. These are the documented non-steerable LTCG inline
walls (cf. get_additional_length's operator|).
