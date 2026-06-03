# jump_logic animation accessors batch

Worktree: /home/sheep/Projects/surv/vostok_6
Branch: match/game_core-jump_logic-animations off fc3aadf9

Unit (jump_logic.cpp):
- selected_animations (PRIORITY - unblocks player_logic_jump_state::selected_animations)
- get_animation
- get_move_animation
- get_move_look_animation
- activate

## Target RVAs (binaries/rich/target/index.jsonl)
- 0x57d9d0 selected_animations
- 0x57df30 get_animation
- 0x57dee0 get_move_animation
- 0x57de90 get_move_look_animation
- 0x57d880 activate

## Layout facts
jump_logic: m_owner@0 (weapon_user_animations_selector&), m_user@4 (base_player*),
m_logic@8 (ai::fsm*), m_animated_object@C, m_jumping_direction@10, m_is_jump_from_right_leg@14.
selector: m_animations@40 (resource_ptr), m_right_leg_is_supporting@4d.
fsm: m_current_state@10.
container: m_jump_animations[2][100]@0x538 (row stride 0x190=400), get_stand_animation non-inline.

## Asm-derived source
- selected_animations: return ((jump_logic_base_state*)m_logic->current_state())->selected_animations(
    buffer, is_third_view, m_owner.look_time_calculator(), weapon_parameters );
  ([m_logic+0x10] -> state; virtual [vtable+0x18]; CALL SITE arg order buffer,is_third_view,look_calc,weapon_params)
- get_animation: return m_owner.animations().get_jump_animation(
    get_jump_animation_index(m_jumping_direction, m_is_jump_from_right_leg, anim_part), is_third_view);
  (get_jump_animation inlined -> m_jump_animations[is_third_view][index] @ +0x538 + iv*0x190 + index*4)
- get_move_animation: m_owner.animations().get_stand_animation(false, m_jumping_direction*3, is_third_view)
- get_move_look_animation: same but *3+2
- activate: m_jumping_direction = get_move_direction(m_user->input());  // [m_user] vtable+0x24 -> player_input const&
            m_is_jump_from_right_leg = !m_owner.is_right_leg_supporting();  // neg;sbb;add1 = logical NOT of [m_owner+0x4d]

## Header edits (inline bodies provided so inlining reproduces)
- jump_logic.h owner() -> return m_owner;
- weapon_user_animations_selector.h animations() -> return *m_animations; (ASSERT(m_object) inside operator* => finalize_impl stub)
- weapon_user_animations_selector.h is_right_leg_supporting() -> return m_right_leg_is_supporting;
- weapon_user_animations_container.h get_jump_animation() -> return m_jump_animations[is_third_view][index];

## Anchor
temp_include_all.cpp: use_game_core_jump_logic() takes member-fn ptrs of the 5 methods.

## Builds / scores (final)
- jump_logic::selected_animations  100% DONE
- jump_logic::activate             100% DONE
- jump_logic::get_animation        100% DONE
  - was 91.93% PARTIAL: the inlined get_jump_animation was MISSING its bounds
    ASSERT. Target asm @0x57df30 has a SECOND `call empty_stub` (at the -0x15
    frame slot) between get_jump_animation_index and the `m_jump_animations`
    address calc; the register-allocation drift (target ecx/edx vs base eax/ecx)
    was a CONSEQUENCE of that missing temp, not LTCG. Adding
    `ASSERT( UNKNOWN_EXPRESSION_T( index < 100 ) )` to the get_jump_animation
    inline body reproduced the byte sequence -> 100%.
- weapon_user_animations_selector::look_time_calculator  100% DONE (dependency of
  selected_animations; `return look_time_functor( this, &...::look_time_factor_calculator )`).

### get_move_animation / get_move_look_animation: STILL STUB (blocked)
Bodies are verified against target asm (@0x58dee0 / @0x58de90):
  return m_owner.animations().get_stand_animation( false, m_jumping_direction*3[+2], is_third_view );
But `weapon_user_animations_container::get_stand_animation` has NO definition in
our tree (no weapon_user_animations_container.cpp - only _cook.cpp). Anchoring
these two bodies via member-fn pointers makes the linker require that symbol and
the LTCG link fails with LNK1257 (unresolved). Left STUB with the body recorded in
the .cpp comment; unblock once get_stand_animation is defined/matched.

## Scope vs PR #176
PR #176 (weapon_user_animations_selector state accessors) touches selector.cpp
(current_state/get_current_state_id/is_in_jump/is_sprinting/is_ready_to_be_deactivated)
and selector.h. Our edits to those files are DISJOINT functions required by
jump_logic and are KEPT (not reverted):
  - selector.cpp: look_time_calculator (needed by selected_animations)
  - selector.h:   animations() inline, is_right_leg_supporting() inline
  - container.h:  get_jump_animation() inline (with bounds ASSERT)
No overlap with PR #176's matched functions.

## Header-edit gotcha
After editing a header inline body, `touch sources/.../jump_logic.cpp` then rebuild,
else the no-arg rebuild reports `+0.00 / 0 changed`.
