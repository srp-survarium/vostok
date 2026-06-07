# items_cook::on_config_ready

target 0x751a60 | base 0x566b60 | report.json: 93.210526% (STATE marker was stale at 84.63%)

Source (matches target shape; ASSERT recovered, 4 body statements + the assert):

    void items_cook::on_config_ready( resources::queries_result& data, resources::query_result_for_cook* parent )
    {
        ASSERT( UNKNOWN_EXPRESSION );
        configs::binary_config_ptr config     = static_cast_resource_ptr< configs::binary_config_ptr >( data[0].get_unmanaged_resource( ) );
        configs::binary_config_value current  = config->get_root( )["data"];
        item_types_enum item_type             = (item_types_enum)(u32)current["type"];
        create_item_and_finish_query( item_type, config, parent );
    }

## Structure
`--view structure-diff --condensed`:

    target 6 / base 5 stmts
    .. same ..
    0x016 <0x22> | 0x016 <0x1d> | configs::binary_config_ptr config = static_cast_resource_ptr<...>( data[0].get_unmanaged_resource( ) );   SIZE
    .. same ..
    <0>         | --          |    EMPTY only target
    .. same ..
    ; aligned 4, size-diffs 1, quantity-diffs 1

One SIZE divergence on the `config = static_cast_resource_ptr<...>` statement (target 0x22 bytes,
base 0x1d bytes - target is 5 bytes LARGER), plus one `EMPTY only target` no-address line. Every
OTHER statement (ASSERT, current = root["data"], item_type cast, create_item_and_finish_query, the
trailing resource_ptr dtor) aligns and is byte-identical.

## Why 93.21% - resource_ptr by-value temporary
`get_unmanaged_resource()` is declared (target index, resources_query_result.h):

    resource_ptr<unmanaged_resource, unmanaged_intrusive_base> query_result_for_user::get_unmanaged_resource() const

i.e. it returns a `resource_ptr` BY VALUE. The target build MATERIALIZES that by-value temporary
on the stack and destroys it:

    TARGET                                  BASE
    lea esi,[ebp-28h]                       (no temp materialization)
    call get_unmanaged_resource             call get_unmanaged_resource
    lea esi,[ebp-1Ch]                       lea esi,[ebp-1Ch]
    call static_cast_resource_ptr<...>      call static_cast_resource_ptr<...>
    lea ecx,[ebp-28h]                       add esp,4        ; cdecl cleanup, no temp
    call intrusive_ptr<...>::dec            (no dec - temp elided)

The target constructs the returned resource_ptr into [ebp-28h], passes its address to
static_cast_resource_ptr, then runs the temporary's destructor (`call ...::dec`) afterward. The base
build elides this temporary entirely (the result flows directly; `add esp,4` cleans the by-value
return slot). That accounts for both divergences: the 5 larger target bytes on the `config` statement
(the extra `lea esi,[ebp-28h]` setup + the `lea ecx,[ebp-28h]; call ::dec` destruction) and the
`EMPTY only target` no-address line (the temp's destructor sub-statement).

The text-fallback `--view diff` (objdiff reloc parse fails here) prints spurious call-name mismatches
(intrusive_ptr<inventory_item>::dec, intrusive_ptr<booby_trap_core>::operator*, intrusive_ptr<sound_emitter>,
configs::binary_config_value::operator float2) - these are all ICF COMDAT folds of byte-identical
intrusive_ptr / binary_config_value template instantiations, NOT real divergences.

Whether the by-value resource_ptr return temporary is materialized-and-destroyed or elided is a
whole-program / LTCG decision, not steerable from the on_config_ready source - the call
`static_cast_resource_ptr< configs::binary_config_ptr >( data[0].get_unmanaged_resource( ) )` is
already the correct spelling. Same class as the "intrusive_ptr-by-value" temp materialization
residuals documented in assembly_patterns.md.

VERDICT: STRUCTURE MATCH (shape ok) - non-steerable resource_ptr by-value temp materialization
(target materializes+destroys the get_unmanaged_resource() return temp, base elides it).
