# pistol_weapon_core_show_state (5 functions)

Unit: ctor (IAE protected), new_object (AAE private), weapon_and_hands_expression
(EBE private virtual const), get_weapon_lexeme_pair (ABE private const),
get_user_hands_expression (ABE private const).

No matched show/hide sibling existed at start (all STUB); derived from target asm in
`binaries/rich/target/sources/vostok/game_core/sources/pistol_weapon_core_show_state.cpp`.
Reference shape from matched `pistol_weapon_core_idle_state.cpp` / `weapon_core_aimed_state.cpp`.

## Key asm facts
- managed_resource_ptr is 4 bytes (vector ctor iterator size=4; m_user_animations stride view*8 user*4;
  m_weapon_animations stride view*16 user*8 weapon*4).
- ctor: base ctor(weapon, is_shown); vtables; vector ctor iterators for the two arrays;
  m_time_scale=[ebp+0Ch]; ASSERT(byte[ebp-19h]); the count==12 ASSERT branch; nested loops;
  trailing ASSERT(byte[ebp-1Ah]). Two ASSERTs (finalize_impl) + one ASSERT_CMP.
- new_object: weapon_anim_length = pinned(animations[0])->length_in_frames(); user from animations[1];
  time_scale = weapon/user; placement new(buffer.c_ptr()) ctor(params->weapon, time_scale, animations,
  count, params->shown).
- get_weapon_lexeme_pair: captions {"pistol-show","pistol-show_empty"}; index=ammo==0;
  selected=m_weapon_animations[view][user==crouch][index]; set_animation_to_wait(selected);
  get_weapon_lexeme_pair_impl(buffer,id,selected,&m_weapon,m_animation_playback_state,6,m_time_scale,
  play_once_and_freeze_at_end, instant_interpolator()).
- get_user_hands_expression: if(user_state_id==type_sprint) return expression(weapon_lexeme); else
  index=(user==crouch); captions {"stand_show","crouch_show"}; animation_lexeme_parameters(buffer,
  captions[index], m_user_animations[view][index], &weapon_lexeme, &weight_driving_animation)
  .animated_object(m_weapon.get_user()).bones_mask(2).playback_type(play_once_and_freeze_at_end);
  return expression(that lexeme).
- weapon_and_hands_expression: lexeme_pair = get_weapon_lexeme_pair(...); hands = get_user_hands_expression(
  lexeme_pair.offset_lexeme, ...); return main + hands + offset (exact + nesting TBD by diff).

## Iterations
1. First pass (all 5 written from asm; new_object used animations[0]/animations[1]):
   ctor 100, new_object 99.98, get_weapon_lexeme_pair 99.92, get_user_hands 72.12,
   weapon_and_hands 65.07.
2. weapon_and_hands return changed `main + hands + offset` -> `hands + main + offset`:
   65.07 -> 83.52 (matches the chained-`+` order; left operand is the hands `expression`).
3. new_object animations[1] -> animations[8]: 99.98 -> 100.0. The target `add ecx,20h` is
   animations[8] (managed_resource_ptr is 4 bytes => 8*4 = 0x20); the show state's 12 anims are
   8 weapon + 4 user, so user_anim_length uses the first USER animation = animations[8].

## Final
- ctor                    -> 100%   DONE (count ASSERT_CMP_U==12 + two nested fill loops + trailing ASSERT)
- new_object              -> 100%   DONE
- get_weapon_lexeme_pair  -> 99.92% DONE  (residual: ammo_in_magazine `this` eax vs ecx, call-boundary LTCG)
- weapon_and_hands_expr   -> 83.52% PARTIAL (missing dedicated `operator+(expression&,...) -> expression`
                                    overloads in shared mixing_addition_lexeme_inline.h; base falls back
                                    to generic addition_lexeme& operator+; out-of-file infra gap)
- get_user_hands_expr     -> 72.12% PARTIAL (whole-program inline decision: target keeps
                                    weapon_core::get_user + animation_lexeme_parameters setters
                                    out-of-line, base inlines them; `bones_mask` is a standalone symbol
                                    in target index, absent in base index -> proven LTCG inline class)

No regressions (`report-changes.json` "regressed": []).

## Header / anchor changes (shared, but additive only)
- pistol_weapon_core_show_state.h: fixed access chars (ctor IAE protected; weapon_and_hands EBE
  private virtual const; get_weapon_lexeme_pair/get_user_hands ABE private const); added friend
  cook_template + use_game_core_pistol_weapon_core_show_state.
- weapon_core_state_cook_template.h: added use_game_core_pistol_weapon_core_show_state friend +
  fwd decl (so the anchor can reach private new_object).
- temp_include_all.cpp: added use_game_core_pistol_weapon_core_show_state anchor + its call.
