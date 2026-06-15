# `VOSTOK_DELETE_IMPL(g_allocator, resource)` one-liner: the strip_pointer wall (~31%)
tags: cpp:new-delete cpp:macro | asm:push asm:call asm:lea | topic:inline-vs-call topic:allocator
symptoms: delete_helper two pushed args add esp 8 vs one arg add esp 4, ~31% with 1/1 stmts
confidence: 7/10
variants: allocator-wrapper-inline.md, free-vs-delete-impl.md, strip-pointer-deref-allocator.md

A one-line cook `delete_resource` whose only statement is `VOSTOK_DELETE_IMPL(allocator,
resource)` = `delete_helper(strip_pointer(allocator), pointer)` (memory_macros.h:41).
Target keeps strip_pointer out-of-line (TWO pushed args); base inlines it (ONE arg).
Structure 1/1; fuzzy stalls ~31% because nearly every operand differs. Non-steerable -
byte-identical residual across cooks; mark PARTIAL/DONE.

```asm
; target: lea eax,[ebp+8]; push eax; mov eax,[g_allocator]; call <finalize_impl-misname>; push eax; call delete_helper; add esp,8
; base:   mov eax,[g_allocator]; call <Release-misname>; push eax; lea edi,[ebp+8]; call delete_helper; add esp,4
```
Evidence: items_cook::delete_resource (31% DONE) and player_parameters_modifyer_cook::delete_resource (31% PARTIAL) - byte-identical diffs.

## Variant: a target site WITHOUT the strip_pointer call = direct `delete_helper( *g_allocator, ... )`
READ THE TARGET FIRST: some cooks' targets have NO strip_pointer call at all
(`lea eax,[ebp+8]; push; mov ecx,[g_allocator]; push ecx; call delete_helper; add esp,8`
- the allocator deref is inline, /Od arg-push registers rotate eax/ecx). That site was NOT
written with VOSTOK_DELETE_IMPL: write `memory::delete_helper( *g_allocator, resource );`
directly and the base reproduces the two-stack-arg shape minus one residual: our LTCG gives
the shared `delete_helper<doug_lea_allocator,resource_base>` COMDAT a custom convention
(&pointer in EDI, one stack arg, add esp,4) while the target's takes both on stack - a pure
argument-passing LTCG boundary, bankable (~41%).
Evidence: weapon_core_cook::delete_resource (target @0x59a670; 33% with the macro, 41% direct,
remaining diff is only the EDI convention); victory_item_core_cook::delete_resource (target
@0x7520c0, 0x22 bytes vs the macro cooks' 0x26: 33% macro -> 41% direct, same EDI residual).
Tell for the direct variant: target stmt is SMALLER (no strip_pointer call), `mov ecx,[g_allocator];
push ecx` inline; the macro cooks' targets keep `call <strip_pointer-misname>` and are 0x26 bytes.
