# structure verify: victory_item_core_cook::delete_resource

VERDICT: STRUCTURE MATCH (shape ok)

target 0x7520c0 | base 0x565c90 | report.json fuzzy 33.38%

## condensed structure-diff
    ; target 1 stmts / base 1 stmts
    0x009 <0x13> | 0x00a <0x16> | VOSTOK_DELETE_IMPL( g_allocator, resource );   SIZE
    ; aligned 0, size-diffs 1, quantity-diffs 0

## Divergence
Single statement on both sides, one SIZE diff, zero quantity diffs.

asm: target `lea eax,[ebp+8]; push eax; mov ecx,[g_allocator]; push ecx; call
delete_helper<doug_lea_allocator,resource_base>; add esp,8`. Base inserts an extra
`mov eax,[g_allocator]; call strip_pointer(folded stub); push eax` and differs in the
delete_helper arg shape (register-vs-stack). Both call the identical
`delete_helper<doug_lea_allocator,resource_base>(doug_lea_allocator&, resource_base*&)`
instantiation.

## Cause / fix
Non-steerable LTCG. Target inlines `strip_pointer(g_allocator)`; base emits it
out-of-line (the extra call), plus the shared cook-family register-vs-stack arg-passing
convention on the delete_helper instantiation. Confirmed NOT steerable by dereferencing
(`*g_allocator`): the animation_analysis_result_cook sibling spells `*g_allocator` and
still scores 31%. This is the documented shared cook-base delete_resource wall - see
../delete_resource.md. No source restructure available.
