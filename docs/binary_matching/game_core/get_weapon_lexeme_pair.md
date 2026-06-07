# get_weapon_lexeme_pair (pistol / double_barreled idle / aimed_idle / show variants)

Covers the 7 `*_weapon_core_*_state::get_weapon_lexeme_pair` overloads in:
- pistol_weapon_core_idle_state.cpp
- pistol_weapon_core_aimed_idle_state.cpp
- pistol_weapon_core_show_state.cpp
- double_barreled_weapon_core_idle_state.cpp
- double_barreled_weapon_core_aimed_idle_state.cpp

## Outcome
report.json fuzzy: 99.918% (2-element variants) / 99.92% (3-element variants), DONE.
Structure: MATCH (size-diffs 0 after the captions fix below).

## The structure fix (this verifier pass)
Before: each variant declared the captions array as a single-line brace-init, e.g.
`pcstr weapon_animation_captions[2] = { "pistol-idle", "pistol-idle_empty" };`.
`--view structure-diff` flagged a SIZE diff on that line: target attributes the two
(or three) string-pointer stores to SEPARATE source lines (L43/L44 etc.), base
collapsed them onto one statement. Bytes were already identical (the stores are
`mov dword ptr [ebp-X], <strN>`), only the statement count diverged.

Fix: split the brace-init across physical lines, one literal per line. Under /Od
MSVC then attributes each element-store to its own source line, matching the target's
per-element statements. Verified after rebuild: every variant goes from size-diffs 1
to size-diffs 0; all 7/9/10 statements align. report.json unchanged (bytes identical).
The remaining quantity-diffs are `EMPTY only base` collapsed blank-line gaps inside
the brace-init - not real divergences.

See assembly_patterns.md "Array brace-init: per-element source lines" for the general rule.

## The sole byte residual (non-steerable, banked DONE)
`u32 animation_index = m_weapon.ammo_in_magazine( );` loads `this` into a register for
the __thiscall: target uses `mov eax, [eax+128h]` (eax), base uses ecx. `ammo_in_magazine`
is LTCG-optimized (target @0x9b270 is `mov ax,[eax+47Ah]; ret`) to take `this` in eax - a
link-time custom calling convention. This is the permitted call-boundary argument-passing
class (MATCHING.md "LTCG is an excuse ONLY for function arguments"). 1 instruction of ~189
bytes -> 99.92%.

The `s_aim_transition_time` reloc and the get_weapon_lexeme_pair_impl callee
`playback_enum`/`playing_type_enum` typedef-alias mangling are byte-identical to the
reference weapon_core_idle_state::get_weapon_lexeme_pair (#151) and not part of the residual.
