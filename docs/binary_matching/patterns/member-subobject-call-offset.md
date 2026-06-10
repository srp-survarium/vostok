# Member sub-object method call: `add ecx, <offset>` before the call
tags: cpp:member | asm:add asm:call | topic:codegen-idiom
symptoms: mov ecx this add ecx 14h call, no add for a member at offset 0
confidence: 9/10

`member.serialize( arg )` where member lives at +0x14: pushed args first, then ecx loads
the sub-object address. A member at offset 0 has no `add` (this == &member).

```cpp
state.serialize( packet );    // member at this+0x14
```
```asm
mov  ecx, [ebp-0Ch]     ; this
add  ecx, 14h           ; &this->member
call survarium::player_state::serialize
```
Evidence: game_core/client_player_update::serialize (input @+0x00 no add, state @+0x14).
