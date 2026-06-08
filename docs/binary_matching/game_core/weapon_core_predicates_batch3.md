# weapon_core batch3 - predicates + small accessors/events

Branch: match/game_core-weapon_core-batch3 off fc3aadf9 (worktree vostok_1).

## Unit
Small/related weapon_core members defined in weapon_core.cpp:
- on_reload_started (empty)
- on_player_model_added / on_player_model_removed (instant_show/hide thunks)
- must_chamber_a_round_predicate (+aimed/+animation_ended/+aimed_and_animation_ended)
- is_trying_to_aim, is_not_trying_to_aim_predicate
- is_ready_to_shoot, is_ready_to_be_deactivated
- target_and_animation_ended_predicate
- set_ammunition, maximum_ammo_in_weapon, on_user_sprint, on_hand_ik_event
- unload_chambered_round (vcall identity pending)

## Access chars (from target weapon_core.cpp.obj mangled names)
- on_reload_started UAE (public virtual) - OK as declared
- on_player_model_added/removed EAE (private virtual) -> moved to private
- must_chamber_* ABE (private const) -> moved to private
- is_trying_to_aim / is_not_trying_to_aim_predicate ABE (private const) -> private
- is_ready_to_shoot QBE (public const) - OK
- is_ready_to_be_deactivated UBE (public virtual const) - OK
- target_and_animation_ended_predicate ABE (private const) -> private
- set_ammunition QAE (public) - OK
- maximum_ammo_in_weapon QBE (public const) - OK
- on_user_sprint MAE (protected virtual) -> protected
- on_hand_ik_event AAE (private) -> private

## Bodies (from --view target asm)
- must_chamber_a_round_predicate: ASSERT(finalize_impl); return !m_is_round_chambered && m_ammo_in_magazine!=0 && !m_user_animations_selector.is_in_jump();
- must_chamber_a_round_aimed_predicate: return must_chamber_a_round_predicate() && is_trying_to_aim();
- must_chamber_a_round_and_animation_ended_predicate: ASSERT; return must_chamber_a_round_predicate() && static_cast<weapon_core_base_state*>(m_logic->current_state())->has_animation_ended();
- must_chamber_a_round_aimed_and_animation_ended_predicate: return must_chamber_a_round_and_animation_ended_predicate() && is_trying_to_aim();
- is_not_trying_to_aim_predicate: return !is_trying_to_aim();
- is_ready_to_shoot: return (m_is_there_chamber_a_round_state ? m_is_round_chambered : m_ammo_in_magazine>0) && m_bullets_in_queue!=0 && m_ready_for_fire;
- is_ready_to_be_deactivated: return static_cast<weapon_core_base_state*>(m_logic->current_state())->is_ready_to_be_deactivated() && m_user_animations_selector.is_ready_to_be_deactivated();
  - filled inline getter weapon_core_base_state::is_ready_to_be_deactivated() { return m_is_ready_to_be_deactivated; } (member @0x134)
- is_trying_to_aim: player_input const& input = m_user->input(); u32 just_toggled = ~m_old_actions_mask & input.actions_mask;
  return could_be_aimed(*get_user()) && (input.actions_mask & aim) && !(input.is_sprinting() && (just_toggled & sprint)) && m_user_animations_selector.get_current_state_id() != type_jump;
  (aim=1<<7=0x80, sprint=1<<9=0x200, type_jump=3)
- target_and_animation_ended_predicate: return m_target==target && static_cast<weapon_core_base_state*>(m_logic->current_state())->has_animation_ended();
- set_ammunition: m_ammunition = ammunition_to_set;
- maximum_ammo_in_weapon: bool chamber_a_round_but_not_on_reload = m_is_there_chamber_a_round_state && !m_chamber_a_round_on_reload; return m_magazine_capacity + chamber_a_round_but_not_on_reload;
- on_user_sprint: bool left_hand_ik_is_active = is_double_handed() || !user_is_sprinting; m_hand_ik_processor.activate_hand(left, left_hand_ik_is_active, m_last_tick_time_in_ms);
- on_hand_ik_event: 2x ASSERT; bool active = params.domain_data==9; m_hand_ik_processor.activate_hand(hand, active, params.callback_time_in_ms); return callback_return_type_call_me_again;
- unload_chambered_round: 3x ASSERT; ++m_ammo_in_magazine; m_is_round_chambered=false; [this+0xB8]() void virtual (identity pending diff).

## Includes added to weapon_core.cpp
- <vostok/game_core/base_player.h>, <vostok/game_core/player_input.h>
  (player_actions_subscriber's bit-flag enum is commented out, so aim/sprint use raw 0x80/0x200)

## Rebuild log
(see below per build)

## Final scores (after anchoring + current_base_state fill)
100 on_reload_started, on_player_model_added, on_player_model_removed, set_ammunition,
    is_ready_to_shoot, unload_chambered_round, must_chamber_a_round_predicate,
    must_chamber_a_round_aimed_predicate, must_chamber_a_round_aimed_and_animation_ended_predicate,
    is_not_trying_to_aim_predicate, weapon_user_animations_selector::is_ready_to_be_deactivated
96.55 on_hand_ik_event (xor ecx,ecx before cmp;sete - register-zeroing)
89.72 on_user_sprint (is_double_handed inline-vs-call)
88.41 maximum_ammo_in_weapon (bool->int neg;sbb;neg normalize)
87.47 must_chamber_a_round_and_animation_ended_predicate (has_animation_ended inline-vs-call)
85.68 target_and_animation_ended_predicate (has_animation_ended inline-vs-call)
84.77 is_ready_to_be_deactivated (getter inline-vs-call)
72.67 weapon_user_animations_selector::get_current_state_id (forced helper; current_state inline-decision)
66.75 is_trying_to_aim (is_sprinting inline-vs-call)
 0    weapon_user_animations_selector::current_state (forced helper; operator* ref round-trip not reproduced)

## Key learning
FIVE of the seven weapon_core PARTIALs share ONE residual: trivial getters declared
WITHOUT the `inline` keyword in the headers (is_double_handed, has_animation_ended,
is_sprinting, selector::is_ready_to_be_deactivated) are emitted as COMDAT standalones AND
called at /Od in the target objs (verified: standalone in target rich index, absent from
base), but our /GL build inlines them in the delinked EXE. This is an inline-vs-call
codegen-DECISION divergence (the /GL whole-program inliner chose differently from the
target's /Od build) - the same class the reload_state_base PROGRESS banked for
round_is_chambered. Per MATCHING.md this is NOT the bankable arg-passing LTCG (it is not an
argument dropped or moved to a register); it is a real source/codegen residual, correctly
left as an OPEN PARTIAL (never banked DONE), pending a clean re-diff by a faster machine -
do not assert it unfixable. (on_user_sprint, must_chamber_a_round_and_animation_ended_predicate,
target_and_animation_ended_predicate, is_ready_to_be_deactivated, is_trying_to_aim.)

The OTHER TWO PARTIALs are NOT this class - they are distinct source-shape micro-codegen
residuals (NOT LTCG, NOT arg-passing) that have not been diffed to a clean cause:
- on_hand_ik_event 96.55%: body byte-identical incl. both ASSERT empty_stub calls and the
  activate_hand args; the ONLY diff is base inserting `xor ecx,ecx` before `cmp eax,9;sete
  cl` (target does `cmp;sete` straight on the movzx'd eax). A register-zeroing difference -
  guidelines say such things are usually source-steerable; left as an open residual.
- maximum_ammo_in_weapon 88.41%: target stores the `&&` into a BYTE temp and applies the
  `neg;sbb;neg` (val!=0) bool-normalize before `add eax,ecx`; our base uses a DWORD temp
  and a direct `add`. A bool->int promotion shape difference - source-steerable in
  principle (cf. the `and X,mask; neg;sbb;neg` pattern in assembly_patterns.md); left open.

## #176 overlap (re-match action)
weapon_user_animations_selector::{is_ready_to_be_deactivated, get_current_state_id,
current_state} in this PR DUPLICATE PR #176, which already matched exactly these three in
the same file. This is a land-time conflict, not a note. is_trying_to_aim only requires
get_current_state_id->current_state to be reachable (provided by #176); weapon_core's own
is_ready_to_be_deactivated reaches the selector getter transitively. ON RE-MATCH: drop all
three selector edits from this PR and keep only weapon_core's own functions.

current_base_state() inline filled = *static_cast<weapon_core_base_state*>(m_logic->current_state()).
weapon_core_base_state::is_ready_to_be_deactivated() filled = m_is_ready_to_be_deactivated (@0x134).

## Rebuild commands
nix develop -c python3 scripts/rebuild.py   (no module arg; relinks EXE)
