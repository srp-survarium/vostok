# inventory_cook.cpp - structure verification (item/material batch)

| fn | old marker | report.json | structure | wall |
|---|---|---|---|---|
| ctor() | 84.05% DONE (STALE) | 100% | MATCH | now 100% |
| translate_query | 92.44% PARTIAL | 91.38% | MATCH 75/75 | c_str/create_request/push_back inline-vs-call (LTCG) |
| on_subresources_loaded | 73.38% PARTIAL (STALE) | 82.10% | MATCH (3 blank-gap quantity) | resource_ptr cast + intrusive_ptr temp + setter inline-vs-call (LTCG) |
| delete_resource | 100% DONE | 100% | - | carcass stripped |

## ctor: now 100%
register_cook + 3 function-local statics. Marker bumped 84.05% -> 100%.

## translate_query (91.38%)
structure-diff: 75/75 stmts. 64 aligned, 9 SIZE, 4 quantity. The 4 quantity diffs are
`EMPTY only base`/`EMPTY only target` blank-line gap markers (no real missing statement;
addressed stmts all align). The 9 SIZE diffs are all on
`item_by_id(...).item_cfg_name.c_str()` and `requests.push_back( create_request(...) )` -
string c_str() + create_request() + buffer_vector push_back kept out-of-line in target,
inlined in base. The switch braces (case_base_trap braced per +0x002, others braced) match.
Non-steerable container/string inline-vs-call LTCG.

## on_subresources_loaded (82.10%, marker was stale at 73.38%)
structure-diff: target 57 / base 54 stmts. The 3 quantity-diffs are all `<0>` empty-line
runs only on target (blank-line gap markers), NOT missing statements - every ADDRESSED
statement aligns. The 6 SIZE diffs are on `static_cast_resource_ptr<>(...)`,
`wpn->set_dict_id`, `iitem->set_amount`, `iitem->set_dict_id`: the resource_ptr cast
constructs/destroys an intrusive_ptr temp (the `mov byte[ebp-N],0; lea; call` ASSERT plus
intrusive_ptr `call`s) and the virtual setters are kept out-of-line in target vs inlined in
base, cascading frame-slot renames across the 1021-byte body. Non-steerable resource_ptr/
intrusive_ptr LTCG class.

VERDICT: ctor now 100%, translate_query + on_subresources_loaded both STRUCTURE MATCH (walls).
Markers synced, embeds added, all 4 carcasses stripped. No logic change.
