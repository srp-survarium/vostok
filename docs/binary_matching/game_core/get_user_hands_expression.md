# pistol_weapon_core_show_state::get_user_hands_expression

## Outcome
report.json 72.12%, PARTIAL. STRUCTURE MATCH - the control structure reproduces the
target exactly; the residual is whole-program LTCG inline of shared header one-liners,
non-steerable from this file.

## Structure (matches the target)
- `if ( user_state_id == type_sprint ) return weapon_lexeme;` (early return; the target's
  return path also constructs a `simple_lock`).
- `u32 user_animation_index = user_state_id == type_crouch;`
- `pcstr animation_captions[2] = { "stand_show", "crouch_show" };` (target keeps this as a
  SINGLE statement - both stores attributed to one line, so it is NOT split here, unlike
  get_weapon_lexeme_pair's captions).
- the `animation_lexeme_parameters(...).animated_object(...).bones_mask(...).playback_type(...)`
  builder chain feeding `animation_lexeme hands_only_lexeme(...)`.
- `return hands_only_lexeme;`

## The wall (72.12%)
`--view structure-diff` SIZE diffs at `return weapon_lexeme;` (0x10 vs 0x13) and the builder
`);` (0x79 vs 0x8f). Cause: the target keeps `weapon_core::get_user()` and the
`animation_lexeme_parameters` setters (animated_object / bones_mask / playback_type) OUT-OF-LINE
(proven: `bones_mask` is a standalone symbol in the target rich index, ABSENT from the base
index), while the base inlines all of them (frame 0x110 -> 0x128, direct member stores). These
are `inline`-declared one-liners in shared animation/weapon_core headers; forcing them out-of-line
would require editing those shared headers - out of this file's scope. Non-steerable LTCG inline class.
