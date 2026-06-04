# weapon_core_hide_state (5-function unit)

Concrete WeaponCore hide-state subclass. Shape mirrors the matched
`pistol_weapon_core_show_state` (ctor / get_weapon_lexeme_pair /
weapon_and_hands_expression / get_user_hands_expression / cook_template::new_object),
but the animation arrays are `[2][2]` (8 anims) not `[2][2][2]` (12), and the
playback / interpolator constants differ.

## Target symbols (binaries/rich/target/index.jsonl)
- 0x79e940 ctor                       `??0...IAE...`  -> protected
- 0x79ead0 new_object (cook_template) `?new_object@...AAE...` -> private
- 0x79eba0 get_user_hands_expression  `?...ABE...` -> private const
- 0x79ec90 get_weapon_lexeme_pair     `?...ABE...` -> private const
- 0x79ed30 weapon_and_hands_expression`?...EBE...` -> private virtual const

Access fixes applied to weapon_core_hide_state.h: ctor public->protected;
weapon_and_hands_expression public-virtual->private-virtual; get_weapon_lexeme_pair
+ get_user_hands_expression public->private. Added the sibling-anchor scaffolding
(`use_game_core_weapon_core_hide_state` fwd-decl + `template friend
weapon_core_state_cook_template` + `friend ::vostok::use_...`) so the
temp_include_all anchor can still reach the now-non-public members.

## Commands
- `pdb_rich_query --index binaries/rich/target/index.jsonl --list --function "hide_state"`
- `pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x79e940 --view target` (and 0x79ead0/0x79eba0/0x79ec90/0x79ed30)
- mangled chars pulled from index.jsonl `mangled` field.
- `python3 scripts/rebuild.py` (NO module arg).
- scores read from `binaries/objdiff/report.json` top-level units[].functions[].fuzzy_match_percent.

## Results
| function | % | tag |
|---|---|---|
| ctor | 100 | DONE |
| new_object | 100 | DONE |
| get_weapon_lexeme_pair | 100 | DONE |
| weapon_and_hands_expression | 83.52 | PARTIAL |
| get_user_hands_expression | 73.52 | PARTIAL |

0 regressed / 1 improved (report-changes.json).

## ctor (100%) - the key find
First build paired at 95.35%. The base body was missing one statement: the target
re-sets `m_body_part_mask_for_user = body_part_whole_body_but_hands` (-3 ->
`mov [ecx+130h], 0FFFFFFFDh` at +0x6f) in the DERIVED ctor body, even though the
hide_state_base ctor (matched) already sets it. Reproducing that redundant store
(after `m_time_scale = animation_timescale;`, before `ASSERT_CMP_U`) closed the gap
to 100%. Body otherwise = m_time_scale store, ASSERT_CMP_U(count==8), two nested
2x2 fill loops sharing one un-reset `animation_index` (weapon[2][2]@0x148 then
user[2][2]@0x158, managed_resource_ptr=4 bytes), trailing `ASSERT(UNKNOWN_EXPRESSION)`.

## new_object (100%)
weapon_anim_length = cubic_spline_skeleton_animation_pinned(animations[0])->length_in_frames();
user_anim_length = pinned(animations[4]) (KEY: animations[4], the first USER anim -
4 weapon + 4 user; target `add ecx,10h` = 4*4 bytes); time_scale = weapon/user;
`new(buffer.c_ptr()) weapon_core_hide_state(params->weapon, time_scale, animations,
animations_count, params->shown)`.

## get_weapon_lexeme_pair (100%)
animation_identifier = "weapon-hide"; selected = m_weapon_animations[is_third_view!=0]
[user_state_id==type_crouch]; set_animation_to_wait(selected);
get_weapon_lexeme_pair_impl(buffer, id, selected, &m_weapon, m_animation_playback_state,
7 /*time_sync*/, m_time_scale, play_once_and_freeze_at_end,
linear_interpolator(s_aim_transition_time)). (Differs from pistol_show: time_sync=7 not 6,
playback=play_once_and_freeze_at_end, interpolator=linear not instant, single array index.)

## weapon_and_hands_expression (83.52% PARTIAL - PROVEN shared wall)
Structure identical to target: lexeme_pair=get_weapon_lexeme_pair; hands=
get_user_hands_expression(lexeme_pair.offset_lexeme,...); return hands + main + offset.
Residual = the operator+ overload wall: target uses dedicated `expression operator+(
expression&, animation_lexeme&)` / `operator+(expression&, expression&)` overloads
absent from shared mixing_addition_lexeme_inline.h; base falls back to generic
`addition_lexeme& operator+`. Identical 83.52% to pistol_weapon_core_show_state. Out of
this file's scope (#192 fix-under-investigation; would shift other matched operator+ fns).

## get_user_hands_expression (73.52% PARTIAL - PROVEN shared wall)
Structure identical to target: `if (user_state_id == type_sprint) return weapon_lexeme;`
then `ASSERT(UNKNOWN_EXPRESSION)` (a bare ASSERT - the target's
`call finalize_impl` at +0x27 with `mov byte [ebp-99h],0`; note this ASSERT is present
here and in reload_state, but ABSENT in pistol_show's variant);
user_animation_index = user_state_id==type_crouch; captions {"stand_hide","crouch_hide"};
animation_lexeme_parameters(buffer, captions[idx], m_user_animations[view][idx],
&weapon_lexeme, &weight_driving_animation).animated_object(m_weapon.get_user())
.bones_mask(2).playback_type(play_once_and_freeze_at_end) -> animation_lexeme -> expression.
Residual = whole-program inline decision: target keeps get_user()/the setters out-of-line,
base inlines them (same wall as pistol_weapon_core_show_state::get_user_hands 72.12%).
