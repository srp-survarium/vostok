# Nested `config["a"]["b"]` = TWO chained out-of-line `operator[]` calls with `mov ecx,eax` between
tags: cpp:operator cpp:cast cpp:member | asm:call asm:mov | topic:codegen-idiom
symptoms: call operator[] / mov ecx,eax / call operator[] / call operator T
confidence: 9/10

A nested string index chains: the outer `operator[]` returns the child node in eax, which is
moved into ecx as the `this` for the inner `operator[]`; the final cast follows. Each level is a
separate out-of-line call (string keys are never inlined). A double cast like
`(game_team_id)(u32)cfg["team"]` collapses — only `operator u32` is emitted, the enum cast is
free (value already in eax).

```cpp
cooked->additional_artefact_slots += (u8)current_item_config["additional_slots"]["artefact_slots"];
```
```asm
push  ??_C@_0BB@..additional_slots?$AA@
mov   ecx, [ebp-18h]
call  ...binary_config_value::operator[]
mov   ecx, eax            ; outer node = this for the inner index
call  ...binary_config_value::operator[]
call  ...binary_config_value::operator unsigned char
movzx eax, al
```
Steerable structurally; the inner `operator T` is where the inline-vs-call cut can wall the % (~89%, config-value-operator-cast.md). cite: player_parameters_modifyer_cook::translate_query player_parameters_cook.cpp:199-200.
