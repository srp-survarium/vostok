# double_barreled_weapon_core_hide_state

HIDE-state sibling of the already-matched family. Same 5-function shape as
`double_barreled_weapon_core_show_state` (#210), `pistol_weapon_core_hide_state`
(#204) and `weapon_core_hide_state` (#197). Modeled directly on those.

It combines the double-barreled 16-animation [2][2][3]/[2][2] layout (3 weapon
states) with hide-state semantics (linear_interpolator + s_aim_transition_time,
time-sync group 7, "hide_*" captions).

## Target symbols (binaries/rich/target/index.jsonl)
- 0x79d490 ctor                    `??0...@@IAE@...`  protected, non-const
- 0x79d660 new_object              `?new_object@...@@AAE...`  private
- 0x79d730 get_user_hands_expression `?...@@ABE...`  private const
- 0x79d810 get_weapon_lexeme_pair    `?...@@ABE...`  private const
- 0x79d8f0 weapon_and_hands_expression `?...@@EBE...`  private virtual const
- dtor                             `??1...@@UAE@XZ`   public virtual

## Verified-from-asm shape facts
- ctor: m_time_scale stored FIRST, then `ASSERT_CMP_U( weapon.get_magazine_capacity(), ==, 2 )`,
  then `ASSERT_CMP_U( animations_count, ==, 16 )`, then the [2][2][3] (16h/0Ch/4 strides)
  + [2][2] loops, trailing `ASSERT( UNKNOWN_EXPRESSION )`. NO m_body_part_mask_for_user store
  (confirmed - matches pistol/double_barreled hide; unlike non-pistol hide_state).
- new_object: weapon_anim animations[0], user_anim animations[12] (`[ebp+14h]+30h`, 30h/4=12).
- get_weapon_lexeme_pair: leading `ASSERT( UNKNOWN_EXPRESSION )`; 3 captions
  hide_both_barrels_empty / hide_one_barrel_loaded / hide_two_barrels_loaded; index =
  ammo_in_magazine() directly; type_crouch == 1; group 7; play_once_and_freeze_at_end;
  `linear_interpolator( s_aim_transition_time )` (NOT instant_interpolator - that's show).
- get_user_hands_expression: type_sprint (==2) early-return weapon_lexeme; user_state_index =
  (==1); captions stand_hide / crouch_hide; builder animated_object(get_user)/bones_mask(2)/
  playback_type. NO linear_interpolator local here (verified - differs from the pistol hide sibling).

## Scaffolding
- temp_include_all.cpp: added `#include double_barreled_weapon_core_hide_state.h`, a
  `use_game_core_double_barreled_weapon_core_hide_state()` anchor (calls all 4 methods +
  new_object) and its dispatcher call.
- weapon_core_state_cook_template.h: fwd-decl + friend
  `use_game_core_double_barreled_weapon_core_hide_state()` (new_object is private -> C2248 without it).
- header: protected ctor / private virtual weapon_and_hands_expression / private const
  get_*; public virtual dtor; friend template cook + friend use fn. Mirrors show_state header.

## Commands run
- pdb_rich_query --index binaries/rich/target/index.jsonl --list --function double_barreled_weapon_core_hide_state
- grep mangled names from index.jsonl for access chars (IAE/EBE/ABE/AAE)
- pdb_fetch --target-index ... --view target --rva 0x79d490 / 0x79d660 / 0x79d730 / 0x79d810
- nix develop -c python3 scripts/rebuild.py (after scaffolding)
- confirmed ctor residual: target `mov eax, [ebp+8]` (0x83) vs base `mov ecx, [ebp+8]` before
  get_magazine_capacity call (grep base index.jsonl instructions).

## Results (report.json fuzzy_match_percent) - first build, no iteration needed
- ctor 0x79d490: 99.96% DONE. Sole diff: `weapon` for get_magazine_capacity() staged into eax
  (target) vs ecx (base). Call-boundary register choice. Same as show_state ctor (99.96%).
- new_object 0x79d660: 100% DONE.
- get_user_hands_expression 0x79d730: 72.12% PARTIAL. Shared-header inline wall - target keeps
  get_user() + the 3 animation_lexeme_parameters setters out-of-line (frame 0x110); base inlines.
  Identical wall to show_state (72.12% / 63.63%) and pistol hide (73.86%). Out of file scope.
- get_weapon_lexeme_pair 0x79d810: 99.93% DONE. Sole diff: inlined ammo_in_magazine() `this`
  read into eax (target) vs ecx (base). Inline-result register choice. Same as show_state (99.93%).
- weapon_and_hands_expression 0x79d8f0: 83.52% PARTIAL. operator+ overload wall - shared
  mixing_addition_lexeme_inline.h lacks the expression-returning operator+ overloads; base falls
  back to the generic addition_lexeme& operator+. Identical to show_state / pistol hide (83.52%).

## Walls summary
All 5 functions land at the exact same scores as the matched siblings on identical, documented,
out-of-file-scope walls (operator+ overloads; shared-animation-header inline decisions) or are
100%/near-100% DONE with the call-boundary register residual. No further source-shaping available
within this file. Relink churn: generic boost/bt/math/particle/empty_stub/thunk COMDATs shuffled
(24 regressed / 45 improved) - delinker copy re-attribution from enabling the new TU, no weapon-state
function regressed.
