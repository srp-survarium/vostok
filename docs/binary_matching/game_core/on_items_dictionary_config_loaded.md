# items_dictionary_cook::on_items_dictionary_config_loaded

target 0x7548b0 | base 0x5689a0 | report.json: 68.01% (was STATE 65.57%)

## Structure
`--view structure-diff --condensed`: target 43 / base 41 stmts, 8 SIZE diffs,
6 "quantity" diffs. The 6 quantity diffs are NOT real statement count divergences:
- 4 are `EMPTY only target|base` empty-line-run alignment artifacts.
- The remaining pair (`L52 item_dict_ids` ONLY target at 0x104 / `0x135` ONLY base)
  is the SAME statement (`u32* item_dict_ids = VOSTOK_NEW_ARRAY_IMPL(...)`)
  de-paired because its SIZE differs (target 0x22 vs base 0x1a) AND the surrounding
  `<0>` empty rows shift the alignment. Source statement ORDER matches the target
  (requests_count L50, item_dict_ids L52, items_it L54 ...).

So: STRUCTURE MATCH; the residual is byte SIZE within matched statements.

## The 8 SIZE diffs (all inline-vs-call of shared helpers - the sushi@MATCH set)
- L44 `cooked_resource->dict_config = static_cast_resource_ptr<...>( data[0].get_unmanaged_resource() )` : 0x45 vs 0x3a
- L48 `vectora<resources::request> requests( g_allocator )` : 0x8 vs 0x44.
  Biggest diff. The vectora ctor inlines the allocator setup in the TARGET (8 bytes
  at this stmt, the rest folded into the ctor body) while our base inlines the ctor
  body here (0x44 bytes). Direction of inlining is reversed (sushi's note). The
  vectora ctor codegen is fixed by the container header template, not steerable from
  this call site.
- L52 `u32* item_dict_ids = VOSTOK_NEW_ARRAY_IMPL( g_allocator, u32, requests_count )` : 0x22 vs 0x1a
- L60 `STR_JOINA( item_cfg_path, "resources/", (pcstr)(*items_it)["cfg_name"] )` : 0x115 vs 0x7a.
  The string-join macro expands very differently; the target keeps far more inline.
- L65 `requests.push_back( resources::create_request(...) )` : 0x27 vs 0x2e
- L69 `const u8 item_category_id = (u8)(*items_it)["item_category"]` : 0x15 vs 0x21
- L79 `item_dict.item_cfg_name = (pcstr)(*items_it)["cfg_name"]` : 0x18 vs 0x2a
- L93 the wrapping `query_resources(...)` call (the `);`) : 0xb4 vs 0xc6

All of these come down to how `binary_config_value::operator[]` / `operator pcstr` /
the `vectora` ctor / `STR_JOINA` / `create_request` were inlined at link time - shared
template/macro codegen, non-steerable from this function's source.

VERDICT: STRUCTURE MATCH - non-steerable inline-vs-call SIZE diffs.
