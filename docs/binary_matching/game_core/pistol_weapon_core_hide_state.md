# pistol_weapon_core_hide_state (game_core)

Unit: `sources/vostok/game_core/sources/pistol_weapon_core_hide_state.cpp`
Branch: `match/game_core-pistol_weapon_core_hide_state` off `origin/int/game_core`.

A Pistol WeaponCore hide-state subclass. Shape is a fusion of the matched
`pistol_weapon_core_show_state` (#185) - the `m_weapon_animations[2][2][2]` +
`m_user_animations[2][2]` layout, count 12, ammo-based caption selection - and
`weapon_core_hide_state` (#197) - the `linear_interpolator(s_aim_transition_time)`,
time-sync group 7, `play_once_and_freeze_at_end`.

## Target functions (target index `binaries/rich/target/index.jsonl`)
- ctor                       0x79df10  `IAE` protected
- new_object (cook_template) 0x79e0b0  `AAE` private
- get_user_hands_expression  0x79e180  `ABE` private const
- get_weapon_lexeme_pair     0x79e280  `ABE` private const
- weapon_and_hands_expression 0x79e350 `EBE` private virtual const

## Commands
```
nix develop -c pdb_rich_query --index binaries/rich/target/index.jsonl --list --function "pistol_weapon_core_hide_state"
nix develop -c pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x79df10 --view target   # ctor
nix develop -c pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x79e0b0 --view target   # new_object
nix develop -c pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x79e180 --view target   # get_user_hands
nix develop -c pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x79e280 --view target   # get_weapon_lexeme_pair
nix develop -c pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x79e350 --view target   # weapon_and_hands
nix develop -c python3 scripts/rebuild.py
```

## Iteration history
1. Wrote all five fns from the show/hide references. First build FAILED:
   `temp_include_all.cpp(913): error C2248 cannot access private new_object`.
   Fix: add `friend void ::vostok::use_game_core_pistol_weapon_core_hide_state()`
   (+ forward decl) to `weapon_core_state_cook_template.h`, and add the
   `use_game_core_pistol_weapon_core_hide_state()` anchor + its call in
   `temp_include_all.cpp` (+ include the header).
2. Second build FAILED: `play_once` is not a member of mixing. The `mov ecx,1`
   in get_user_hands is `play_once_and_freeze_at_end` (enum: play_cyclically=0,
   play_once_and_freeze_at_end=1, play_once_and_remove_at_end=2). Fixed.
3. Third build OK but all 4 methods scored 0%, only new_object 100%. Cause:
   ACCESS-SPECIFIER mismatch. Comparing obj symbols:
   - ctor      target `IAE` (protected) vs base `QAE` (public)
   - the 3 methods target `ABE`/`EBE` (private) vs base `QBE`/`UBE` (public)
   objdiff pairs by mangled name, so a public-vs-private symbol does not pair -> 0%.
   Fix: header ctor -> `protected:`, the 3 methods -> `private:`. Mirrors
   `pistol_weapon_core_idle_state.h`'s access layout.
   NOTE: header edit needed a `touch` of the .cpp for ninja to recompile the obj
   (first rebuild after the header edit reused the stale obj and still showed `QAE`).
4. After touch+rebuild: ctor 100, new_object 100, get_weapon_lexeme_pair 99.92,
   weapon_and_hands 83.52, get_user_hands 73.86. report-changes: no regressions.

## Final results
- ctor                       100%  DONE
- new_object                 100%  DONE   (animations[0], animations[8], count 12)
- get_weapon_lexeme_pair    99.92% DONE   captions "pistol-hide"/"pistol-hide_empty",
   group 7, linear_interpolator(s_aim_transition_time). Sole residual: the
   `m_weapon.ammo_in_magazine()` __thiscall `this` in eax (target) vs ecx (base) -
   permitted call-boundary argument-passing exception. Same as the reference
   pistol_weapon_core_show_state::get_weapon_lexeme_pair (99.92% DONE). Trailing
   compiled-out ASSERT already reproduced (base emits dummy::nonnull at 0xc2).
- weapon_and_hands_expression 83.52% PARTIAL  shared-header operator+ wall (target
   uses dedicated `expression operator+(expression&, animation_lexeme&)` /
   `operator+(expression&, expression&)` overloads absent from shared
   mixing_addition_lexeme_inline.h). Identical residual/score to
   pistol_weapon_core_show_state::weapon_and_hands_expression and
   weapon_core_hide_state. Out of file scope.
- get_user_hands_expression  73.86% PARTIAL  shared-header whole-program inline wall:
   target keeps weapon_core::get_user() and the animation_lexeme_parameters setters
   OUT-OF-LINE (frame 0x114); base inlines them to direct member stores + extra
   compiled-out ASSERTs (frame 0x128). Source shape (sprint early-return, captions
   "stand_hide"/"crouch_hide", s_aim_transition_time interpolator local, builder
   chain, expression(override_lexeme)) matches the target exactly. Same residual
   class as pistol_weapon_core_show_state::get_user_hands_expression (72.12%) and
   weapon_core_hide_state (73.52%). Out of file scope.

All five residuals are the documented shared walls / permitted arg exception; the
source structure matches the target carcasses. No further source-shape gain available
without editing shared animation headers (out of this unit's scope).
