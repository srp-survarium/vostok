# Derived ctor BODY re-assigns an inherited member the base ctor already set
tags: cpp:ctor cpp:member | asm:mov | topic:structure-shape
symptoms: store at an inherited offset AFTER the init-list member stores, 0FFFFFFFDh
confidence: 8/10
variants: state-ctor-vtable-stores.md

A store landing at an INHERITED member offset after the derived ctor's own init-list
stores is the derived BODY overwriting a value the base ctor already initialized (the
member must be protected on the base).

```cpp
// base ctor set m_body_part_mask_for_user = body_part_whole_body (-1); derived body overwrites:
m_body_part_mask_for_user = animation::body_part_whole_body_but_hands;   // -3
```
```asm
mov eax,[ebp+0Ch]; mov [edx+140h],eax    ; the derived member (init-list)
mov dword ptr [ecx+130h], 0FFFFFFFDh     ; inherited @0x130 overwritten = body statement
```
Enum values: body_part_whole_body = -1 (0xFFFFFFFF), body_part_whole_body_but_hands = -3 (0xFFFFFFFD).
Evidence: game_core/weapon_core_show_state_base ctor 100%.
