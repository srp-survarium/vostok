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

## items_cook::translate_query (target 0x751b20 | base 0x566c10 | 99.84931%; STATE was stale 94.84%)
Identical case to the weapon_ammunition_cook one above. `--view structure-diff --condensed`
-> target 4 / base 4 stmts, size-diffs 0, quantity-diffs 0 (perfect structural alignment).
The lone residual is the same +0xC frame-size delta: base `sub esp,178h` vs target `sub esp,16Ch`,
shifting every [ebp-N] slot by 0xC (e.g. [ebp-16Ch]->[ebp-178h]). The boost::bind functor here
captures three values (this, _1, &parent -> list3/bind_t), copied through 4 dwords [eax]..[eax+0Ch]
into the function temp; the temp's stack layout is the non-steerable LTCG slot-allocation decision.
The text-fallback `--view diff` (objdiff reloc parse fails on this fn) prints spurious call-name
mismatches (behaviour_cook_params, res_effect/sound_emitter, function0/function2) which are ICF
COMDAT folds, NOT real divergences - the 99.85% objdiff score confirms the bytes are identical.
Differs from the 100% siblings (damage_model_cook, items_dictionary_cook, victory_item_core_cook)
only in that those pass a string literal + a simpler bind (list2, no &parent capture, NULL user_data),
so their functor temp has a different (matching) layout - not a source bug in items_cook.
VERDICT: STRUCTURE MATCH - non-steerable LTCG frame-padding on the boost functor temp.
