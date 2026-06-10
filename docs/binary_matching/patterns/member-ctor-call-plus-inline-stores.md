# Value member with its own init-list ctor: sub-object `call` THEN inlined member stores
tags: cpp:ctor cpp:init-list cpp:member | asm:call asm:movss asm:mov | topic:codegen-idiom topic:fold-icf
symptoms: add ecx call misnamed ctor then explicit stores at the same member offset
confidence: 8/10
variants: scalar-default-ctor-movss.md, fold-misname-empty-fn.md

A value member whose own default ctor has a member-init list emits, inside the enclosing
ctor: the member's empty/base sub-object ctor as a `call` (often delinker-misnamed, e.g.
`float4()`), then that ctor's init-list stores INLINED right after. Fill the member
struct's own ctor body/init-list to produce the stores.

```cpp
animation_playback_state::animation_playback_state( ) : interval_id( 0 ), interval_time( 0.0f ) { }
```
```asm
add ecx,120h; call <float4()>          ; delinker MISNAME of the member's default ctor
; ...vtable stores...
add edx,120h; mov [ebp-8],edx          ; &member
mov eax,[ebp-8]; mov dword [eax],0     ; member.interval_id = 0
mov ecx,[ebp-8]; movss [ecx+4],const   ; member.interval_time = 0.0f
```
A `member.reset()` looks identical - decide by whether the carcass has a statement line vs init-list placement (decl-order before later members).
Evidence: game_core/weapon_core_base_state ctor 100% (the `float4()` reloc was animation_playback_state's default ctor; rdata slot = 0.0f).
