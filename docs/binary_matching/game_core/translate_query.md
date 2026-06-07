# weapon_ammunition_cook::translate_query

target 0x7506d0 | base 0x45f270 | report.json: 99.82758% (STATE marker was stale at 95.62%)

Source (4 statements, matches target shape exactly):

    void weapon_ammunition_cook::translate_query( resources::query_result_for_cook& parent )
    {
        fs_new::virtual_path_string config_name;
        config_name.assignf( "resources/%s", parent.get_requested_path( ) );

        resources::query_resource(
            config_name.c_str( ),
            resources::binary_config_class_impl,
            boost::bind( &weapon_ammunition_cook::on_config_ready, this, _1, &parent ),
            g_allocator,
            NULL,
            &parent
        );
    }

## Structure
`--view structure-diff --condensed` -> target 4 / base 4 stmts, size-diffs 0,
quantity-diffs 0. Structure MATCHES perfectly.

## Why only 99.83%
asm diff (`--view diff`): every instruction is byte-identical EXCEPT the prologue frame
size and the cascading [ebp-N] slot offsets:
- TARGET: `sub esp, 180h`
- BASE:   `sub esp, 18Ch`  (+0xC = 12 bytes larger)

The 0xC delta uniformly shifts every subsequent `[ebp-N]` reference (e.g.
[ebp-180h]->[ebp-18Ch], [ebp-154h]->[ebp-160h]); the instruction bytes and the
boost::bind / query_resource call sequence are otherwise identical. No statement is
missing (structure is fully aligned), there is no compiled-out ASSERT (no empty_stub /
finalize_impl call in this function), so the 12 extra base bytes are slot-allocation noise
for the boost::bind functor temporary materialized before the query_resource call - a
whole-program / LTCG stack-layout decision, not steerable from this function's source.

VERDICT: STRUCTURE MATCH - sole residual is a uniform +0xC frame-size/slot shift, non-steerable
LTCG slot allocation for the boost::bind functor temp. No source restructure.
