# `VOSTOK_DELETE_IMPL(g_allocator, resource)` one-liner: the strip_pointer wall (~31%)
tags: cpp:new-delete cpp:macro | asm:push asm:call asm:lea | topic:inline-vs-call topic:allocator
symptoms: delete_helper two pushed args add esp 8 vs one arg add esp 4, ~31% with 1/1 stmts
confidence: 7/10
variants: allocator-wrapper-inline.md, free-vs-delete-impl.md

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
