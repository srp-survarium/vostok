# player_logic_crouch_state (unit)

7 STUB functions in `sources/vostok/game_core/sources/player_logic_crouch_state.cpp`.
A `player_logic_base_state` derivative for the crouch weapon-user state. Shape mirrors
`player_logic_jump_state` (PR #177) and the per-subclass lexeme machinery
(`movement_lexeme`/`get_recoil_animation_lexeme`/`look_expression`) shared with
`player_logic_stand_state` (also still STUB).

## Target symbols + access (target index rva, mangled)
```
0x57ece0  finalize                   ?finalize@..@@EAEXXZ                 private virtual
0x57ed00  initialize                 ?initialize@..@@EAEXXZ               private virtual
0x57ed20  ctor                       ??0..@@QAE@AAV..@                    public
0x57ed50  get_recoil_animation_lexeme ?get_recoil_animation_lexeme@..@@ABE..  private const (non-virtual)
0x57efb0  movement_lexeme            ?movement_lexeme@..@@ABE..           private const (non-virtual)
0x57f1e0  look_expression            ?look_expression@..@@ABE..           private const (non-virtual)
0x57f680  selected_animations        ?selected_animations@..@@EBE..       private virtual const
```
ACCESS-SPECIFIER FIX: moved all members except the ctor into a `private:` section of the
header (ctor stays public). Without this objdiff cannot pair the symbols.

NOTE: carcass `// FUNCTION BODY` addresses are +0x1000 vs the rich-index rvas
(0x58ed20 carcass == 0x57ed20 rva).

## Commands
```
nix develop -c pdb_rich_query --index binaries/rich/target/index.jsonl --list | grep player_logic_crouch_state
nix develop -c bash -c "grep player_logic_crouch_state binaries/rich/target/index.jsonl | python3 -c '...print rva+mangled...'"
nix develop -c pdb_fetch --target-index binaries/rich/target/index.jsonl --rva <rva> --view target
nix develop -c python3 scripts/rebuild.py    # NO module arg
```

## Results
- ctor 100%: `player_logic_base_state( owner, type_crouch )`. `type_crouch` == 1 (`push 1`).
- initialize 100%: `m_user->crouch();`  (asm: virtual call `[vtable+0x40]` on m_user @+0x1c).
- finalize 100%:   `m_user->stand_up();` (asm: virtual call `[vtable+0x44]` on m_user @+0x1c).
  base_player declares jump/end_jump/crouch/stand_up consecutively -> crouch=+0x40,
  stand_up=+0x44. Added `#include <vostok/game_core/base_player.h>`.

### selected_animations (6.47% PARTIAL - UNREACHABLE stub)
Full body decoded from target @0x57f680:
```cpp
u32 movement_animation_index;
if ( (*m_user->damage_model()).broken_legs_count() > 1 )      // cmp edx,1; jle
    movement_animation_index = 0;
else
    movement_animation_index = player_logic_base_state::movement_animation_index( m_user->input() );
animation::mixing::animation_lexeme main_lexeme = movement_lexeme( buffer,
    movement_animation_index, weapon_parameters.body_part_mask, weapon_parameters.is_aimed,
    is_third_view, weapon_parameters.is_firing );
return std::make_pair( look_expression( buffer, movement_animation_index,
    weapon_parameters.is_aimed, is_third_view, weapon_parameters, main_lexeme )
    + animation::mixing::expression( main_lexeme ), main_lexeme );
```
Arg decode of the two delegating calls (from the push order):
- movement_lexeme push order @0xa2..0xc1: weapon_parameters.is_firing[+0x11],
  is_third_view[ebp+14], weapon_parameters.is_aimed[+0x10], weapon_parameters.body_part_mask[+0x0c],
  movement_animation_index, buffer.
- look_expression push order @0xfe..0x11a: &main_lexeme, weapon_parameters[ebp+10],
  is_third_view[ebp+14], weapon_parameters.is_aimed[+0x10], movement_animation_index, buffer.
- The return: `expression::expression<animation_lexeme>(main_lexeme)` is constructed @0xe0
  into [ebp-0A4], `look_expression` @0x128 into [ebp-9C], then
  `operator+( look_result, expression(main_lexeme) )` @0x13b -> `make_pair(<sum>, main_lexeme)`.

BLOCKED (C2678): the target calls the free overload
`vostok::animation::mixing::operator+(expression&, expression const&) -> expression`
(present in target index), but the codebase only declares the template
`operator+(T1&, T2&) -> addition_lexeme&` (mixing_addition_lexeme.h). Adding the
expression-returning overload is the shared lexeme/operator+ machinery (same wall as
`jump_logic_state_start::selected_animations`), out of this unit's scope. Reverted to
`UNREACHABLE_CODE()`; emitted/paired at 6.47%.

### movement_lexeme / get_recoil_animation_lexeme / look_expression (0% PARTIAL - UNREACHABLE stubs)
These are the per-subclass lexeme machinery. movement_lexeme @0x57efb0 and look_expression
@0x57f1e0 both build an `animation_lexeme` via:
- `weapon_user_animations_container::get_animation_impl<27,6>` (template),
- the `crouch_animations_captions` static array (`?crouch_animations_captions@survarium@@3QBQBDB`),
- `linear_interpolator` / `instant_interpolator`,
- the `animation_lexeme_parameters` builder chain (weight/time sync ids, weight/time-scale
  interpolators, time_scale, animated_object, bones_mask, time_calculator),
- `weapon_user_animations_selector::look_time_factor` / `look_time_calculator` (look_expression),
- multiple compiled-out ASSERTs (`call ...fixed_size_allocator..finalize_impl` = empty_stub).
look_expression sums three near-identical lexeme-build blocks via the same
`operator+(expression&, expression const&)` overload that blocks selected_animations.
get_recoil_animation_lexeme @0x57ed50 is the recoil variant (managed_resource_ptr additive
animation + interpolators). All three are the same machinery wall as
jump_logic_state_start::get_main_lexeme/get_look_lexeme; matching them needs the
get_animation_impl + animation_lexeme_parameters + operator+ machinery resolved (separate
unit). Left UNREACHABLE; not anchored (only selected_animations reaches them, a blocked stub).

## Anchor
`use_game_core_player_logic_crouch_state()` in temp_include_all.cpp constructs the state and
calls initialize/execute/finalize through `ai::fsm_state&` (overrides are private). Added the
include and the call in `use_temp_include_all`. ctor/initialize/finalize pair at 100%.
