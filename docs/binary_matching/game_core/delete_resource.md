# items_dictionary_cook::delete_resource (and the cook-base delete_resource family)

target 0x754600 | base 0x5686e0 | report.json: 31.00%

Source (1 statement, matches target shape exactly):

    void items_dictionary_cook::delete_resource( resources::resource_base* resource )
    {
        VOSTOK_DELETE_IMPL( g_allocator, resource );
    }

## Structure
`--view structure-diff --condensed` -> target 1 stmt / base 1 stmt, one SIZE diff,
zero quantity diffs. Structure MATCHES.

## Why only 31%
asm diff (`--view diff`):

    TARGET                              BASE
    sub esp,0Ch                         sub esp,0Ch
    mov [ebp-0Ch],ecx                   push edi
    lea eax,[ebp+8]                     mov [ebp-0Ch],ecx
    push eax                            mov eax,[g_allocator]
    mov eax,[g_allocator]               call strip_pointer (folded stub)
    call strip_pointer (folded stub)    push eax
    push eax                            lea edi,[ebp+8]
    call delete_helper<doug_lea,res*&>  call delete_helper<doug_lea,res*&>
    add esp,8                           add esp,4
    mov esp,ebp                         pop edi
    ...                                 ...

Both call the SAME instantiation
`delete_helper<doug_lea_allocator,resource_base>(doug_lea_allocator&, resource_base*&)`
(confirmed identical mangled symbol in both target and base rich indexes).

The divergence is purely argument passing:
- TARGET pushes BOTH args on the stack (`add esp,8`), normal right-to-left order:
  evaluate the `resource_base*&` (lea [ebp+8]; push), then the allocator.
- BASE passes the `resource_base*&` argument in the `edi` register (push/pop edi,
  `add esp,4` cleans only the allocator arg). That is the LTCG link-time custom
  calling convention chosen for this `delete_helper` instantiation in our base build.

This is the ONE bankable LTCG excuse (an argument passed in a register vs its stack
slot). It is NOT steerable from `items_dictionary_cook.cpp`: the convention is a
property of the linked template instantiation, shared by every cook that calls
`VOSTOK_DELETE_IMPL( g_allocator, resource )` on the raw `resource` parameter.

## Shared across the cook family (the "cook-base structure" hint)
report.json delete_resource scores in namespace survarium:
- 31% : animation_analysis_result_cook, items_cook, items_dictionary_cook,
        player_parameters_modifyer_cook, weapon_ammunition_cook
        (all delete the raw `resource` param -> hit the register convention)
- 100%: inventory_cook, ladder_cook
        (delete a LOCAL pointer; delete_helper inlines fully to delete_helper_impl
         with the pointer in a local `[ebp-N]` slot, so no register-arg convention)

So this is a single shared wall affecting all 5 raw-param deleters, not a per-function
source bug. Fixing it would require changing the linker's chosen convention, which we
cannot do from source.

## victory_item_core_cook::delete_resource (target 0x7520c0 | base 0x565c90 | 33.38%)
Same family. Identical 1-stmt structure (size-diff 1, quantity-diff 0). The base's extra
instruction vs target is the `strip_pointer(g_allocator)` call left out-of-line in base
(target inlines it to `mov ecx,[g_allocator]; push ecx`), plus the same register-vs-stack
arg shape on `delete_helper<doug_lea_allocator,resource_base>`. Confirmed NOT steerable by
dereferencing: the `animation_analysis_result_cook` sibling uses `VOSTOK_DELETE_IMPL(
*g_allocator, ... )` and still scores 31% (the macro's `strip_pointer` normalizes both
spellings). Left as-is; STRUCTURE MATCH, non-steerable LTCG inline-vs-call + arg passing.

VERDICT: STRUCTURE MATCH - non-steerable LTCG register-vs-stack arg passing on the
shared delete_helper<doug_lea_allocator,resource_base> instantiation.
