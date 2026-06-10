# `if (a && b)` materializing a THIRD bool temp = an inlined predicate, not a bare `&&`
tags: cpp:if cpp:bool cpp:inline | asm:mov asm:movzx asm:test | topic:structure-shape
symptoms: extra [ebp-0Ch] && result slot retested, sub esp 0Ch vs 08, per-member mov al round-trips
confidence: 8/10
variants: inlined-bool-helper-in-condition.md

A /Od `if ( x && y )` over member-bool reads short-circuits DIRECTLY into the body (no
intermediate slot). A SEPARATE bool temp computed first and re-tested is the RETURN-VALUE
temp of an inlined predicate whose body IS the `&&` - write the wrapper, not the bare
chain. The per-member `mov al,[..]; mov [ebp-N],al` round-trips are the inlined leaf
getters' return temps.

```cpp
inline bool is_full_on_ground( ) const { return is_heel_on_ground( ) && is_toe_on_ground( ); }
if ( is_full_on_ground( ) )
```
```asm
mov al,[this+2Ch]; mov [ebp-1],al; movzx ecx,[ebp-1]; test; je .else
mov al,[this+2Dh]; mov [ebp-2],al; movzx ecx,[ebp-2]; test; je .else
mov [ebp-0Ch],1; jmp .end
.else: mov [ebp-0Ch],0
.end: movzx edx,[ebp-0Ch]; test edx,edx; je .skip
```
Evidence: legs_ik_processor::leg_params::set_{heel,toe}_on_ground (59.90 -> 78.19 leaf getters -> 100 via is_full_on_ground; frame 0x0C matched).
