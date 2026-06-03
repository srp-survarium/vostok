# weapon_core_fire_state (concrete WeaponCore fire-state subclass)

Unit: 5 functions in `sources/vostok/game_core/sources/weapon_core_fire_state.cpp`.
Shape mirrors `pistol_weapon_core_fire_state` (ctor/get_weapon_lexeme_pair/new_object
DONE there) and `double_barreled_weapon_core_aimed_idle_state`.

Class layout differs from pistol: `m_weapon_animations[2][2]` and `m_user_animations[2][2]`
(both 2D, not 3D), no `m_weapon_animation_index`, ctor count == 8.

## Access (mangled chars from target index)
- ctor `??0...@@IAE` -> protected non-const
- `weapon_and_hands_expression` `@@MBE` -> protected virtual const
- `get_weapon_lexeme_pair` `@@IBE` -> protected const
- `get_user_hands_expression` `@@IBE` -> protected const

Set header access to all-protected (pistol was private for the helpers; ours is protected).

## Commands
- `pdb_rich_query --index binaries/rich/target/index.jsonl --list --function ...`
- RVAs (live target index; carcass RVAs are STALE, off by ~0x10009):
  - ctor: by name
  - new_object: 0x799b50
  - get_weapon_lexeme_pair: 0x799d30
  - weapon_and_hands_expression: 0x799df0
  - get_user_hands_expression: 0x799bc0

## ctor (confident)
asm: base ctor(weapon, timescale); two `vector constructor iterator` (the two member
arrays); first finalize = ASSERT_CMP_U(animations_count, ==, 8); two 2D fill loops
(view 0..2, user_state 0..2) for m_weapon_animations then m_user_animations sharing
animation_index; trailing finalize = ASSERT(UNKNOWN_EXPRESSION).

## get_weapon_lexeme_pair (confident)
- animation_identifier = "weapon-fire" (single string, not array)
- selected_animation = m_weapon_animations[is_third_view!=false][user_state_id==type_crouch]
- set_animation_to_wait(selected_animation)
- impl call: time_sync_group=1, time_scale=m_animation_timescale,
  playback_type = m_weapon.get_bullets_in_queue() <= 1 ? play_once_and_freeze_at_end : play_cyclically
  (asm: movzx [m_weapon+0x47C]=m_bullets_in_queue; cmp 1; setle -> 1 means freeze)
  interpolator = linear_interpolator(s_aim_transition_time)

## new_object (expect PARTIAL like pistol)
placement-new weapon_core_fire_state(params->weapon,
  computed_shooting_animation_time_scale(*animations, params->rounds_per_second),
  animations, animations_count). Residual = LTCG calling convention of
  computed_shooting_animation_time_scale (xmm0 ret) - same as pistol.

## weapon_and_hands_expression (HARD - operator+ machinery)
asm @0x799df0:
- lexeme_pair = get_weapon_lexeme_pair(buffer, is_third_view, user_state_id)  [ebp-108h]
  (main_lexeme @[ebp-108h], offset_lexeme @[ebp-84h] since pair is {0x00 main, 0x84 offset, 0x108})
- hands_expression = get_user_hands_expression(lexeme_pair.offset_lexeme, buffer, is_third_view, user_state_id)  [ebp-110h]
  NOTE: first arg is OFFSET_LEXEME ([ebp-84h]), not main!
- return: expression(offset_lexeme)@[ebp-118h]; operator+<animation_lexeme> @[ebp-120h];
  operator+ -> [ebp+8]. Exact combination TBD via diff.

## get_user_hands_expression (HARDEST)
asm @0x799bc0:
- if (user_state_id == type_sprint /*==2*/) return expression(weapon_lexeme);
- else: user_animation_index = (user_state_id == type_crouch); [ebp-98h]
  selected_animation = m_user_animations[is_third_view!=false][user_animation_index]  [ebp-94h]
  pinned_ptr_const<cubic_spline_skeleton_animation>(selected_animation); scheduler();
  bool = (scheduler->[0x18] != 1); -> if true return expression(weapon_lexeme);
- else: user_animation_captions[2] = {"stand_shoot","crouch_shoot"};
  get_user via m_weapon; animation_lexeme_parameters(...); .animated_object(); .playback_type(1);
  [params+0x48]=1; animation_lexeme hands_lexeme(params); return expression(hands_lexeme).

## RESULTS (resumed session, 2026-06-03)

Final per-function fuzzy % (report.json units[].functions[].fuzzy_match_percent):
- ctor                          100%  DONE
- get_weapon_lexeme_pair        100%  DONE
- new_object                    86.5% PARTIAL (LTCG arg-passing wall)
- weapon_and_hands_expression   82.52% PARTIAL (operator+ inline-vs-call wall)
- get_user_hands_expression     73.51% PARTIAL (lexeme_parameters setter inline-vs-call + COMDAT-fold)

### Compile fix
The cpp would not build: `error C2678: binary '+' ... animation_lexeme`. Cause: the
free `operator+(T1&,T2&)` lives in `mixing_addition_lexeme.h`, which was not included.
weapon_core_idle_state.cpp includes it; added the same include.

### weapon_and_hands_expression (63.78 -> 82.52)
Target asm @0x799df0 decoded (pdb_fetch --view target --rva 0x799df0):
  get_weapon_lexeme_pair(&[-108h]); get_user_hands_expression(&result[-110h], &[-84h]=OFFSET);
  expression::expression<animation_lexeme>(&[-118h], &[-84h]=offset);
  operator+<animation_lexeme>(main, expr_offset)  [OUT-OF-LINE in target];
  operator+(.., hands);  weight_driving_animation [ebp+18h] is UNREFERENCED.
First guess `expression(main+offset+hands)` -> 63.78. The expr ctor in the target is built
from the OFFSET lexeme alone, so the tree is `main + expression(offset) + hands` -> 82.52.
Residual: target keeps the first operator+ out-of-line, our /GL build inlines it
(addition_lexeme ctor + cloned_in_buffer). This is the SAME operator+ template-selection
residual under active investigation on PR #192 (target operator+<animation_lexeme> vs base
operator+<expression,animation_lexeme>) and on weapon_core_idle_state 85.65%; if #192 finds a
source fix it replicates here - not re-derived in this unit.

### get_user_hands_expression (13.77 -> 73.51)
Target asm @0x799bc0 decoded; reconstructed body:
  if (user_state_id == type_sprint) return expression(weapon_lexeme);
  u32 user_animation_index = (user_state_id == type_crouch);
  selected_animation = m_user_animations[is_third_view!=false][user_animation_index];
  if (pinned_ptr_const<cubic_spline_skeleton_animation>(selected_animation)->animation_type()
        != animation_type_additive) return expression(weapon_lexeme);   // !=1; additive==1
  pcstr user_animation_captions[2] = {"stand_shoot","crouch_shoot"};
  animation_lexeme_parameters params(buffer, captions[index], selected_animation, &weapon_lexeme, NULL);
  params.animated_object(m_weapon.get_user()).playback_type(play_once_and_freeze_at_end).additivity_priority(1);
  animation_lexeme hands_lexeme(params); return expression(hands_lexeme);
Key decodes:
- `!= 1` is `animation_type() != animation_type_additive` (enum: full=0, additive=1).
  The target delinked the getter as `inventory_holder::scheduler` reading `[+0x18]` - a
  COMDAT-folded trivial getter (same bytes, wrong attributed name). Our build calls the real
  `cubic_spline_skeleton_animation::animation_type`; the boolize (sub 1;neg;sbb;neg) is identical.
- `[params+0x48]=1` is offset 0x48 = m_additivity_priority -> .additivity_priority(1).
- `playback_type(1)` = play_once_and_freeze_at_end.
- identifier (captions[index]) is VOSTOK_UNREFERENCED under MASTER_GOLD (4 ctor stack args).
Residual: lexeme_parameters setters out-of-line in target vs inlined in our /GL build (only
additivity_priority stays a call), reschedules get_user(). Same unsteerable wall as
get_weapon_lexeme_pair_impl. Structure/statements/branches all match.

### new_object (86.5)
Target @0x799b50: `fld [edx+8]; fstp [esp]; call computed_shooting_animation_time_scale;
movss [esp], xmm0` - the LTCG passes the managed_resource_ptr arg in a register and returns
float in xmm0. Identical call-boundary wall as pistol new_object (92.08%). PARTIAL, advance.

### Regression check
report-changes.json: 18 regressed + 18 improved, all (100->0) and (0->100) on thunks /
vcalls / COMDAT-folded dtors (~mutex, ~Cluster, empty_stub, vector-deleting-destructors).
These float between objects on every relink (link-order-sensitive symbol attribution); none
are in this unit and none are real logic regressions.
