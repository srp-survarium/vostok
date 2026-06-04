# player_logic_stand_state (unit)

5 STUB functions in `sources/vostok/game_core/sources/player_logic_stand_state.cpp`.
A `player_logic_base_state` derivative for the stand weapon-user state. Shape mirrors
`player_logic_jump_state` (PR #177) and `player_logic_crouch_state` (PR #198); the
per-subclass lexeme machinery (`movement_lexeme`/`get_recoil_animation_lexeme`/
`look_expression`/`selected_animations`) hits the shared lexeme/operator+ wall.

## Target symbols + access (target index rva, mangled)
```
0x7706e0  ctor                        ??0..@@QAE@AAV..                     public
0x770710  get_recoil_animation_lexeme ?get_recoil_animation_lexeme@..@@ABE..  private const
0x770960  movement_lexeme             ?movement_lexeme@..@@ABE..           private const
0x770bc0  look_expression             ?look_expression@..@@ABE..           private const
0x771070  selected_animations         ?selected_animations@..@@EBE..       private virtual const
0x01a800  initialize/finalize/execute (empty {} bodies, ICF-folded shared stub)  private virtual
```
ACCESS-SPECIFIER FIX: header already had ctor public; moved initialize/finalize/execute/
selected_animations/movement_lexeme/get_recoil_animation_lexeme into a `private:` section
(only the ctor stays public), matching the mangled `QAE` (ctor) vs `ABE`/`EBE` (the rest).
Without this objdiff cannot pair the symbols. Initialize/finalize/execute are empty `{}`
bodies (header `/* no source */`) that ICF folds onto a shared `push ebp; ... ret` stub at
0x1a800 (None, unscorable).

NOTE: carcass `// FUNCTION BODY` addresses are +0x1000 vs the rich-index rvas
(0x7806e0 carcass == 0x7706e0 rva).

## Commands
```
nix develop -c pdb_rich_query --index binaries/rich/target/index.jsonl --list | grep stand_state
nix develop -c pdb_rich_query --index binaries/rich/target/index.jsonl --rva 0x7706e0   # ctor
nix develop -c pdb_rich_query --index binaries/rich/target/index.jsonl --rva 0x771070   # selected_animations
nix develop -c pdb_rich_query --index binaries/rich/target/index.jsonl --rva 0x770960   # movement_lexeme
nix develop -c python3 scripts/rebuild.py    # NO module arg
```

## Anchor
Added `use_game_core_player_logic_stand_state()` to temp_include_all.cpp (mirrors the
crouch anchor): constructs a `player_logic_stand_state` and calls initialize/execute/
finalize through the `vostok::ai::fsm_state&` base interface, then escapes `&state`.
Wired into the anchor driver next to the crouch call, and added
`#include "player_logic_stand_state.h"`. The four lexeme/expression stubs get
`UNREACHABLE_CODE();` bodies so the vtable codegen does not hit C4716 (no return).

## Results (report.json top-level fuzzy_match_percent)
- ctor **100% DONE**: `player_logic_base_state( owner, type_stand )`. `type_stand` == 0
  (`push 0` @0x07). Identical scaffold to crouch/jump ctors.
- initialize/finalize/execute: empty `{}`, ICF-folded at 0x1a800 (None, unscorable).
- movement_lexeme **None|PARTIAL** (0%, paired): real body @0x770960 is the lexeme
  machinery - branch on is_firing to pick animation index, `finalize_impl` (misnamed
  fixed_size_allocator construct = the animation_lexeme bookkeeping), `linear_interpolator`
  from `s_aim_transition_time`, animation_lexeme_parameters builder reading [m_user+1Ch]
  weapon offsets (+0x114, +0x40, +0x2E8/+0x1E0 by is_aimed). Needs get_animation_impl +
  animation_lexeme_parameters chain, out of this unit.
- get_recoil_animation_lexeme **None|PARTIAL** (0%, paired): real body @0x770710 is recoil
  lexeme machinery (animation_lexeme_parameters builder, managed_resource_ptr additive
  animation, interpolators). Same wall.
- look_expression **None|PARTIAL** (0%, paired): real body @0x770bc0 is look-lexeme
  machinery (get_animation_impl, stand_animations_captions, instant/linear interpolators,
  look_time_factor/look_time_calculator, three near-identical lexeme-build blocks summed via
  the expression `operator+` overload). Same wall.
- selected_animations **8.08%|None/PARTIAL** (emitted, paired): real body verified from
  target @0x771070:
  ```cpp
  u32 movement_animation_index = player_logic_base_state::movement_animation_index( m_user->input() );
  animation::mixing::animation_lexeme main_lexeme = movement_lexeme( buffer,
      movement_animation_index, weapon_parameters.body_part_mask, weapon_parameters.is_aimed,
      is_third_view, weapon_parameters.is_firing );
  return std::make_pair( look_expression( buffer, movement_animation_index,
      weapon_parameters.is_aimed, is_third_view, weapon_parameters, main_lexeme )
      + animation::mixing::expression( main_lexeme ), main_lexeme );
  ```
  Unlike crouch's selected_animations there is **NO** `broken_legs_count() > 1` branch -
  stand uses the input index directly (asm @0x11-0x2c: load m_user [+1Ch], virtual call
  [vtable+0x24] = input(), then movement_animation_index). Arg decode of the two delegating
  calls matches crouch's order. BLOCKED: target calls free
  `mixing::operator+(expression&, expression const&) -> expression` (asm @0xdb), but only the
  template `operator+(T1&,T2&) -> addition_lexeme&` is declared (mixing_addition_lexeme.h) ->
  C2678. The expression-returning overload is the shared lexeme/operator+ machinery wall
  (same as crouch/jump). Also depends on movement_lexeme/look_expression, themselves on the
  same wall. Left as `UNREACHABLE_CODE()` stub.

## Wall summary
movement_lexeme / get_recoil_animation_lexeme / look_expression / selected_animations all
sit behind the cross-cutting LEXEME/EXPRESSION build wall (get_animation_impl bodies,
animation_lexeme_parameters builder chain, and the free
`mixing::operator+(expression&, expression const&)` overload). Out of this unit; same wall
already documented for player_logic_crouch_state. Parked PARTIAL/None as instructed; did
not spin.

## Regressions
report-changes.json: only the usual ICF/COMDAT-fold churn (empty_stub, boost storage2/3
folds, vcall thunks, particle/sound dtor folds) that migrate fold targets on every relink -
none in player_logic_* or any real game_core state function. 18 regressed / 30 improved,
all volatile fold noise.
