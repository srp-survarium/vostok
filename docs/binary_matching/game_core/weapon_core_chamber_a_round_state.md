# weapon_core_chamber_a_round_state (concrete subclass)

Unit: `sources/vostok/game_core/sources/weapon_core_chamber_a_round_state.cpp`
Base: `origin/int/game_core`. Mirrors the matched `weapon_core_idle_state` shape.

Five functions:
- ctor `??0...@@IAE` (protected) - STATE[100%|DONE] (prior worker)
- `get_weapon_lexeme_pair` `@@ABE` (private const) - was DONE but never compiled; fixed
- `weapon_and_hands_expression` `@@EBE` (private virtual const)
- `get_user_hands_expression` `@@ABE` (private const)
- `new_object` cook `@@AAE` (private) - STATE[100%|DONE] (prior worker)

## Resume context
Prior worker was cut off mid-build. The unit's base `.obj` was NEVER produced
because `temp_include_all.cpp` failed to compile:
- `temp_include_all.cpp(1043): C2248` - cook template `new_object` private; the
  anchor `use_game_core_weapon_core_chamber_a_round_state` was not a friend of
  `weapon_core_state_cook_template`. FIX: add the fwd-decl + friend line in
  `weapon_core_state_cook_template.h` (mirrors every other state anchor there).
- `weapon_core_chamber_a_round_state.cpp(93): C2248` - `m_animation_timescale`
  private in `weapon_core_chamber_a_round_state_base`. FIX: make that data member
  `protected` (data-member access is not byte-visible; only mangled members are).
- `weapon_core_chamber_a_round_state.cpp(94): C2065` - `m_playback_type`
  undeclared. It was a wrong guess; see get_weapon_lexeme_pair below.

## get_weapon_lexeme_pair (target 0x79a4..; chamber RVA from --view target)
Target asm (`pdb_fetch --function chamber_a_round_state::get_weapon_lexeme_pair
--view target`) shows the `get_weapon_lexeme_pair_impl` call args (pushed R->L):
- interpolator = linear_interpolator(s_aim_transition_time)
- playback_type = `push 1` => `play_once_and_freeze_at_end`
- time_scale   = `fld [eax+140h]` => `m_animation_timescale`
- time_sync_group = `push 8` => literal 8
- playback_state = `[reg+120h]` => m_animation_playback_state
- animated_object = `[reg+128h]` => &m_weapon
- selected_animation, identifier "weapon-chamber_a_round", buffer
Also `set_animation_to_wait(selected_animation)` BEFORE the impl call (0x3a),
which idle_state does NOT have. Trailing `call finalize_impl` (0x8d) = compiled-out
ASSERT after the impl call - TODO confirm via diff.

FIX applied: sync_group 1->8, playback m_playback_type->play_once_and_freeze_at_end.

## weapon_and_hands_expression (target 0x79a690 / base 0x44fa80) - 83.52% PARTIAL
Source: `return hands_expression + lexeme_pair.main_lexeme + lexeme_pair.offset_lexeme;`
Base resolves the chain via the only operator+ our headers declare - the generic
`template<T1,T2> operator+(T1&,T2&) -> addition_lexeme&` - giving
`operator+<expression,animation_lexeme>` then `operator+<addition_lexeme,animation_lexeme>`
then `expression::expression<addition_lexeme>`.
Target instead uses the EXPRESSION-returning overloads that exist out-of-line in the
target rich index but are NOT in our `mixing_addition_lexeme_inline.h`:
  - `operator+<animation_lexeme>(expression&, animation_lexeme&) -> expression` (0x099150)
  - `operator+(expression&, expression&) -> expression` (0x0bce60)
  - plus const variants.
So target builds `expression(offset)` (expression::expression<animation_lexeme>),
`operator+<animation_lexeme>(hands, main)`, `operator+(that, expr_offset)`.
WALL: recovering needs the full expression-returning operator+ family added to the
shared `mixing_addition_lexeme` API - a separate cross-cutting unit, out of this unit's
scope (chamber .cpp + header + anchors). Left PARTIAL.

## get_user_hands_expression (target 0x79a4f0 / base 0x44f8d0) - 74.02% PARTIAL
Structure matches the carcass (if-sprint early return, ASSERT, captions, lexeme_parameters
fluent build, return override_lexeme). Residual:
  - Target keeps `animation_lexeme_parameters::animated_object/bones_mask/playback_type`
    and `~animation_lexeme_parameters` as OUT-OF-LINE calls (all standalone in target rich
    index: bones_mask 0x09ce50, animated_object 0x098d90). Our /Od base INLINES the trivial
    in-class setters (mov [ecx+20h],edx / [eax+4Ch],2 / [ecx+3Ch],1) and inlines the dtor
    down to its ASSERT (dummy::nonnull at 0xe7).
  - Minor: early-return ctor - target calls a 1-arg expression ctor (delinker misname
    simple_lock::simple_lock), base the 2-arg expression::expression<animation_lexeme>.
WALL: whole-program inline decision; not steerable from this file. Left PARTIAL.

## Regressions
Enabling this TU relinks game_core and reshuffles addresses; the delinker re-pairs some
still-STUB COMDAT functions (boost functors, thunks, empty_stub, float3 ctor, jump_logic
ctor/dtor). jump_logic::~jump_logic base body is itself an empty ASSERT stub
(pdb_fetch --base-index ... jump_logic::~jump_logic --view base => 9-line stub) - it was
never genuinely matched; its prior "100%" was a pairing artifact disturbed by the relink.
No logic regression introduced by this unit; all 5 chamber functions improved 0 -> 100/100/100/83.52/74.02.

## Commands run
- pdb_rich_query --index binaries/rich/target/index.jsonl --function "chamber_a_round_state::weapon_and_hands_expression" --list  (RVA 0x79a690)
- pdb_rich_query ... "chamber_a_round_state::get_user_hands_expression" --list   (RVA 0x79a4f0)
- pdb_fetch --target-index ... --function "chamber_a_round_state::get_weapon_lexeme_pair" --view target
- pdb_fetch ... weapon_and_hands_expression / get_user_hands_expression --view target
