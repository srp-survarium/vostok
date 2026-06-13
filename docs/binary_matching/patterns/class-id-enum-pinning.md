# Pin reconstructed resources class_id_enum values from cook-registration ctors
tags: cpp:member cpp:ctor | asm:mov asm:push | topic:structure-shape
symptoms: push 12Ch where the enum names value 103h, creation_request/unmanaged_cook ctor takes an unexplained constant
confidence: 8/10
variants: (none)

`resources_classes.h` is reconstructed; a block of enumerators can be shifted wholesale.
When target asm pushes a class-id constant that no enumerator equals, do NOT cast a magic
number - pin the truth: every cook's ctor registers its class id as an immediate
(`unmanaged_cook( <id>, ... )` => `mov ecx, <id>` / `mov dword ptr [cook+8], <id>` in the
ctor or in `register_cooks_for_logic_states`). Fetch 2-3 cook ctors covering the block's
ends, confirm the stride, then anchor the block with one explicit `= N` on its first
enumerator (other users of the block shift with it - grep compiled .cpp users first).

Evidence: weapon state classes block: our header had weapon_inactive_state_class=259 but
weapon_core_inactive_state_cook ctor registers 0x12C (300), idle cook 0x12F, hide 0x12E,
double-barreled show 0x13A, pistol show 0x141 - contiguous from 300. One-line fix
`weapon_inactive_state_class = 300` made weapon_core_cook::query_weapon_states' constant
honest (and the inactive cook ctor target-true).
