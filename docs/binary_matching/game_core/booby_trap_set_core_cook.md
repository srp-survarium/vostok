# booby_trap_set_core_cook - structure verification (v2)

Verified the five `booby_trap_set_core_cook` cook functions. The `// STATE[..]`
markers were all STALE; re-confirmed against report.json / objdiff. All five have
CORRECT source structure (structure-diff quantity-diffs 0 on every function).

Diagnosis command (per function):
```
nix develop -c pdb_fetch --target-index binaries/rich/target/index.jsonl \
  --base-index binaries/rich/base/index.jsonl --function <name> \
  --view structure-diff --condensed \
  --objdiff-base-dir binaries/objdiff/base --objdiff-target-dir binaries/objdiff/target
```

## Results (stale STATE -> real objdiff fuzzy %)

| function                | stale | real    | structure        |
|-------------------------|-------|---------|------------------|
| ctor                    | 66.69 | 96.53%  | MATCH (1/1)      |
| translate_query         | 90.58 | 95.75%  | MATCH (17/17)    |
| on_config_ready         | 92.27 | 82.01%  | MATCH (28/28)    |
| on_subresources_loaded  | 87.46 | 89.02%  | MATCH (11/11)    |
| finish_query            | 74.00 | 100.00% | MATCH (2/2)      |

No structural restructure available on any of them. Edits this pass are
COMMENT-ONLY (STATE sync + embedded structure-diff + VERDICT, carcass deleted) -
byte-neutral; rebuild confirmed the percentages above are unchanged.

## finish_query - 100%
Already byte-perfect. Stripped carcass, reduced to bare `// STATE[100%|DONE]`.

## ctor - 96.53%, non-steerable
structure-diff: 1/1 stmt, all `.. same ..`. Byte diff (`--view diff`): the only
divergence is a single `~` on a `call` (the base-init / member-construct call) -
a relocated call target, not a code-shape diff. Non-steerable.

## translate_query - 95.75%, non-steerable
structure-diff: 17/17 stmts, size-diffs 0. Byte diff has ZERO `+`/`-` instruction
add/remove lines - every diff is a `~` offset shift stemming from `sub esp, 188h`
(target) vs `sub esp, 190h` (base), an 8-byte-larger frame. The extra 8 bytes is
the boost::bind closure temporary that captures `cook_data`
(`booby_trap_set_cook_data`) for the `on_config_ready` bind; its layout is a
header/template decision. Source form is already the direct nested call. Non-steerable.

## on_config_ready - 82.01%, non-steerable
structure-diff: 28/28 stmts, 5 SIZE diffs, 0 quantity. The driving SIZE diff is the
`static_cast_resource_ptr< binary_config_ptr >( data[0].get_unmanaged_resource() )`
line:
- TARGET passes the `get_unmanaged_resource()` result as a direct stack prvalue
  (`push ecx; mov esi, esp; ...; call`) into `static_cast_resource_ptr`.
- BASE materializes the result into a named slot `[ebp-0Ch]`, then emits an EXTRA
  `resource_ptr` copy-construct call into a second slot `[ebp-10h]` - this is the
  recurring +8-byte frame (`sub esp, 0BCh` vs `0B4h`).
The remaining SIZE diffs (`create_request(...)` push_back, the `for`, the closing
`)`) are the same inline-vs-temp / create_request ABI family. `static_cast_resource_ptr`
takes its arg by value (`const src_ptr`, resources_resource_ptr_inline.h:55); the
extra copy is an inline expansion / RVO decision in header template code, not steerable
from this `.cpp`. The source already uses the maximally direct nested-call form (same
as the matched `inventory_cook.cpp:266` `sushi@MATCH` site).

## on_subresources_loaded - 89.02%, non-steerable
structure-diff: 11/11 stmts, sole SIZE diff is the same
`static_cast_resource_ptr< booby_trap_core_ptr >( data[i].get_unmanaged_resource() )`
line as on_config_ready - target direct prvalue, base extra copy-construct
(`+ call` + 8B frame). Same wall, non-steerable.

## Net
No regressions. All structures match; residuals are LTCG header/template inline-vs-temp
(resource_ptr conversion, boost::bind closure size, create_request ABI) plus one
call-target relocation - all banked as DONE.
