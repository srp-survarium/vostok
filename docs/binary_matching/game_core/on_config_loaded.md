# victory_item_core_cook::on_config_loaded

target 0x752150 | base 0x565d20 | report.json: 93.71% (STATE marker was stale at 79.67%)

Source (10 statements, matches target shape exactly):

    void victory_item_core_cook::on_config_loaded( resources::queries_result& data )
    {
        resources::query_result_for_cook* parent = data.get_parent_query( );

        configs::binary_config_ptr cfg = static_cast_resource_ptr< configs::binary_config_ptr >( data[0].get_unmanaged_resource( ) );

        victory_item_core* object_to_cook = create_resource( );
        object_to_cook->load( cfg->get_root( ) );

        parent->set_unmanaged_resource(
            object_to_cook,
            resources::memory_usage_type( resources::nocache_memory, sizeof( victory_item_core ) )
        );

        parent->finish_query( result_success );
    }

## Structure
`--view structure-diff --condensed` -> target 10 stmts / base 10 stmts.
- one SIZE diff on the `cfg = static_cast_resource_ptr<...>( data[0].get_unmanaged_resource() )` statement
- two EMPTY-only rows (a collapsed source-line gap on each side) - cosmetic attribution
quantity of real statements is identical. Structure MATCHES.

## Why only 93.71%
asm diff (`--view diff`) localizes the entire divergence to the `cfg` statement:
- TARGET: `push ecx; mov esi,esp; push 0; mov ecx,[ebp+8]; call operator[]; call
  get_unmanaged_resource; lea esi,[ebp-0Ch]; call <intrusive_ptr copy>; add esp,4`
  -> operator[] on `data` is INLINED, and the resource_ptr is materialized into a single
  temp slot at [ebp-0Ch].
- BASE: `push 0; mov ecx,[ebp+8]; call operator[]; lea esi,[ebp-10h]; call <X>; lea
  esi,[ebp-0Ch]; call; lea ecx,[ebp-10h]; call` -> base spills an EXTRA temporary at
  [ebp-10h] (an intermediate by-value resource_ptr copy + its dtor) before settling into
  [ebp-0Ch].

The extra temp slot bumps the frame from `sub esp,30h` (target) to `sub esp,34h` (base)
and cascades the `[ebp-2Ch]/[ebp-30h]/[ebp-34h]` slot renaming over the rest of the body
- but every later statement is byte-exact modulo that constant slot offset.

This is the documented intrusive_ptr-by-value LTCG class (assembly_patterns.md:
"intrusive_ptr-by-value ... temp construct + ... dtor"): whether the by-value
`resource_ptr` returned through the inlined `operator[]`/`get_unmanaged_resource` chain
is materialized into one slot or through an extra intermediate is a whole-program
materialization decision, not steerable from this function's source.

VERDICT: STRUCTURE MATCH - sole SIZE diff is by-value resource_ptr temp materialization
through the inlined data[0] operator[], non-steerable LTCG. No source restructure.

---

# weapon_ammunition_cook::on_config_ready

target 0x7505d0 | base 0x45f180 | report.json: 94.625% (STATE marker was stale at 75.81%)

Source (7 statements, matches target shape exactly): the ASSERT, the `config` =
`static_cast_resource_ptr<...>( data[0].get_unmanaged_resource() )`, `wa` = VOSTOK_NEW_IMPL,
`wa->load(...)`, `parent->set_unmanaged_resource(...)`, `parent->finish_query(...)`.

## Structure
`--view structure-diff --condensed` -> target 7 / base 7 stmts, exactly ONE SIZE diff on
the `config = static_cast_resource_ptr<...>` statement, zero quantity diffs. Structure MATCHES.

## Why only 94.625%
Same by-value resource_ptr temp materialization class as victory_item_core_cook above, but
the direction is reversed for this function:
- TARGET: `push 0; mov ecx,[ebp+8]; call operator[]; lea esi,[ebp-10h]; call
  get_unmanaged_resource; lea esi,[ebp-4]; call static_cast_resource_ptr; lea ecx,[ebp-10h];
  call intrusive_ptr::dec` -> the get_unmanaged_resource() return is materialized into a NAMED
  slot [ebp-10h] and destroyed with an explicit intrusive_ptr::dec.
- BASE: `push ecx; mov esi,esp; push 0; mov ecx,[ebp+8]; call operator[]; call
  get_unmanaged_resource; lea esi,[ebp-4]; call static_cast_resource_ptr; add esp,4` -> the
  temp is built INLINE on the pushed stack slot (esi=esp), no [ebp-10h] slot and no dec dtor;
  the stack is reclaimed with `add esp,4`.

Same calls, same order, same statement; only the temp's slot vs inline-push materialization
differs. Every other statement (ASSERT, VOSTOK_NEW, wa->load, set_unmanaged_resource,
finish_query) is byte-exact. Documented intrusive_ptr-by-value LTCG class (assembly_patterns.md),
non-steerable from source.

VERDICT: STRUCTURE MATCH - sole SIZE diff is by-value resource_ptr temp materialization on the
static_cast_resource_ptr line, non-steerable LTCG. No source restructure.
