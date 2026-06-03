# weapon_core_show_state (concrete WeaponCore show-state subclass)

Unit: 5 functions in `sources/vostok/game_core/sources/weapon_core_show_state.cpp`
+ header `sources/vostok/game_core/weapon_core_show_state.h`
+ anchors in `temp_include_all.cpp` and `weapon_core_state_cook_template.h`.

Base: `origin/int/game_core`. Branch: `match/game_core-weapon_core_show_state`.

Shape is a near-exact sibling of the already-matched `pistol_weapon_core_show_state`
(same ctor / weapon_and_hands_expression / get_weapon_lexeme_pair /
get_user_hands_expression / new_object pattern), differing only in array shapes and
constants. Reused that reasoning verbatim.

## Target RVAs (binaries/rich/target/index.jsonl)
- ctor                        0x79ee10  ??0...@@IAE  (protected, non-const)
- weapon_and_hands_expression 0x79f1e0  EBE          (public virtual, const)
- get_weapon_lexeme_pair      0x79f140  ABE          (private, const)
- get_user_hands_expression   0x79f060  ABE          (private, const)
- new_object                  0x79ef90  AAE          (private)

## Commands
- `pdb_rich_query --index binaries/rich/target/index.jsonl --list | grep weapon_core_show_state`
- `pdb_fetch --target-index binaries/rich/target/index.jsonl --rva <rva> --view target`
- access chars: `grep -oh '?<fn>@weapon_core_show_state@survarium@@[A-Z][A-Z][A-Z]' binaries/rich/target/index.jsonl`
- `python3 scripts/rebuild.py` (no module arg); scores from `binaries/objdiff/report.json`.

## Per-function result (report.json fuzzy_match_percent)

### ctor  -> 100%  DONE
`m_time_scale = animation_timescale;` (movss to [eax+168h] in member-init region),
`ASSERT_CMP_U( animations_count, ==, 8 )` (the [ebp-15h] finalize_impl eater + the
conditional `push 8; push animations_count` eater), then TWO 2x2 loops sharing one
`animation_index` ([ebp-4]): `m_weapon_animations[2][2]` (0x148) then
`m_user_animations[2][2]` (0x158), then `ASSERT( UNKNOWN_EXPRESSION )` ([ebp-16h] eater).
Carcass srclines 26..38 map exactly; the two `+0x002` brace pairs are the inner-loop
closing braces. 4+4 = 8 animations (the `push 8` in the count assert).

### get_weapon_lexeme_pair  -> 100%  DONE
Single caption `"weapon-show"`, `m_weapon_animations[is_third_view!=false][user_state_id==type_crouch]`
(2D, no third index), `set_animation_to_wait`, then `get_weapon_lexeme_pair_impl` with
group 6, `m_time_scale`, `play_once_and_freeze_at_end`,
`linear_interpolator( s_aim_transition_time )` (NOT instant_interpolator like pistol),
then a trailing `ASSERT( UNKNOWN_EXPRESSION )` (the 0x8d finalize_impl eater, baked into the
return). `s_aim_transition_time` is the per-file `static float = 0.3f` (same as every idle/show .cpp).

### new_object  -> 99.98%  PARTIAL  (real index bug, NOT a reloc artifact)
`weapon_anim_length = cubic_spline_skeleton_animation_pinned(animations[0])->length_in_frames()`,
`user_anim_length = ...animations[?]...`, `time_scale = weapon/user`,
`new ( buffer.c_ptr() ) weapon_core_show_state( params->weapon, time_scale, animations, animations_count, params->shown )`.

REVIEW CORRECTION (this was mislabeled DONE with the residual called a reloc artifact):
the 0.02% is NOT a reloc/representative artifact. objdiff (operand-aware, one-shot
`objdiff-cli diff`) flags exactly one penalizing instruction at line 90: target
`mov ecx,[ebp+14h]; add ecx, 10h` vs base `add ecx, 4`. `managed_resource_ptr` is 4 bytes,
so target offset 0x10 == `animations[4]`, but the source writes `animations[1]` (= +4).
The ctor loads weapon anims into indices 0-3 and user anims into 4-7, so the first user
animation is `animations[4]`; the sibling pistol uses `animations[8]` for the same reason
(its weapon block is 2x2x2=8). FIX: change `animations[1]` -> `animations[4]` on the
user_anim_length line, then rebuild; expected 100.0 (pistol with the analogous `animations[8]`
is 100.0). The two `call ?end@buffer_string@vostok@@QBE...` reloc-target diffs ARE benign
ICF-fold-representative churn (pistol new_object carries the identical pair and scores 100.0),
so they do not block this - the add-immediate is the sole real residual.

### weapon_and_hands_expression  -> 83.52%  PARTIAL  (known wall)
Structure matches: get_weapon_lexeme_pair, get_user_hands_expression, then the chained
`hands_expression + lexeme_pair.main_lexeme + lexeme_pair.offset_lexeme`. Residual is the
operator+ template-selection idiom: target uses dedicated expression-returning
`operator+(expression&, animation_lexeme&)` / `operator+(expression&, expression&)` overloads
absent from the shared `mixing_addition_lexeme_inline.h`. Identical to
pistol_weapon_core_show_state::weapon_and_hands_expression (83.52%). Fix under investigation
on PR #192; will replicate. Out of this file's scope.

### get_user_hands_expression  -> 72.12%  PARTIAL  (known wall)
Control structure matches exactly (type_sprint early-return of weapon_lexeme; user_state_index;
animation_lexeme_parameters builder chain animated_object/bones_mask/playback_type;
expression(override_lexeme)). Residual is whole-program inline decisions: target keeps
weapon_core::get_user() and the animation_lexeme_parameters setters OUT-OF-LINE; base inlines
them. Identical to pistol_weapon_core_show_state::get_user_hands_expression (72.12%). Shared
inline-header decision, out of this file's scope.

## report-changes.json
regressed 19 / improved 23 / added 0 / **removed 0**. Every "regressed" entry is `->0.0`
with `removed=0`: COMDAT representative-ownership churn (adding this TU re-attributes folded
COMDATs - thunks, boost _bi storage, interlocked_increment/decrement, the weapon_lexeme_pair
ctor shared with weapon_core_fire_state). The functions still exist at full % under a different
unit's representative (verified e.g. interlocked_increment still 100%). Net change positive
(+4) and no real byte regression.
