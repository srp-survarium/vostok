# double_barreled_weapon_core_fire_state (unit #?)

A Double-Barreled WeaponCore fire-state subclass. Shape mirrors the matched
`pistol_weapon_core_fire_state` (#174) and `weapon_core_fire_state` (#193), both on the
common ground `origin/int/game_core`.

Six functions, all in `sources/vostok/game_core/sources/double_barreled_weapon_core_fire_state.cpp`:

| fn | target rva | final | tag |
|----|-----------|-------|-----|
| ctor | 0x79c370 | 99.96% | DONE (LTCG this-reg) |
| new_object | 0x79c540 | 86.5% | PARTIAL (computed_* LTCG) |
| get_user_hands_expression | 0x79c5b0 | 73.17% | PARTIAL (lexeme_parameters inline-vs-call LTCG) |
| get_weapon_lexeme_pair | 0x79c710 | 100% | DONE |
| weapon_and_hands_expression | 0x79c7e0 | 83.18% | PARTIAL (operator+ template LTCG) |
| initialize | 0x79c8c0 | 99.76% | DONE (LTCG this-reg) |

## Commands

```
nix develop -c pdb_rich_query --index binaries/rich/target/index.jsonl --function double_barreled_weapon_core_fire_state --list
nix develop -c pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x79c8c0 --view diff   # initialize
nix develop -c pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x79c710 --view target  # get_weapon_lexeme_pair
nix develop -c pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x79c370 --view target  # ctor
nix develop -c pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x79c7e0 --view target  # weapon_and_hands_expression
nix develop -c pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x79c5b0 --view target  # get_user_hands_expression
nix develop -c pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x79c540 --view target  # new_object
nix develop -c python3 scripts/rebuild.py    # no module arg; relinks the EXE
```

Baseline (resumed from a prior matcher's first approximation):
ctor 93.51, new_object 86.5, get_user_hands 12.78, get_weapon_lexeme_pair 95.23,
weapon_and_hands 21.43, initialize 99.76.

## get_weapon_lexeme_pair  95.23% -> 100%

Same shape as pistol sibling. Captions `"shot_first_barrel" / "shot_second_barrel"`.
The base diverged on the `playback_type` argument: source passed `m_playback_type`
(`[+144h]`), but the target at 0x76 emits `push 1` = literal
`animation::mixing::play_once_and_freeze_at_end`. Changed the 8th
`get_weapon_lexeme_pair_impl` arg to `animation::mixing::play_once_and_freeze_at_end`
(NOT `animation::play_once_and_freeze_at_end` - that resolves to `vostok::animation::playback_enum`
and fails C2664; the `_impl` parameter is `mixing::playback_enum`). -> 100%.

## ctor  93.51% -> 99.96%

Frame size and the first ASSERT body differed: the target makes a real
`call get_magazine_capacity` (own symbol @0x09cc20), pushes the u16 result, then asserts;
the base INLINED `get_magazine_capacity()` to a direct `[ecx+478h]` read into a temp local
`[ebp-1Eh]`, which both added a stack slot (frame 44h vs target 40h) and changed the shape.
`get_magazine_capacity` was defined in-class (implicitly inline) in `weapon_core.h`. It is only
used by THIS cpp among matched code (grep), and is its own (unmatched) out-of-line target symbol,
so moved its definition out-of-line into `weapon_core.cpp` (mirroring the established
`ammo_in_magazine` pattern at line 103) and left only the declaration in the header. Frame size
then matched 40h. Residual = the `get_magazine_capacity()` __thiscall `this` in eax (target) vs ecx
(base): an LTCG arg-passing register choice, the same residual class as the reference idle getters.
DONE-class.

## initialize  99.76% (unchanged - already a wall)

`weapon_core_fire_state_base::initialize(); ASSERT(UNKNOWN_EXPRESSION);
m_weapon_animation_index = m_weapon.ammo_in_magazine() != 2;`. Every instruction/branch matches;
sole residual is `ammo_in_magazine()` __thiscall `this` in eax (target) vs ecx (base) - LTCG
arg register. DONE-class.

## weapon_and_hands_expression  21.43% -> 83.18%

Was a placeholder `return expression(weight_driving_animation)`. Target asm (0x79c7e0):
- `'55'`: `lexeme_pair = get_weapon_lexeme_pair(buffer, is_third_view, user_state_id);`  ([ebp-108h])
- `'56'`: `hands_expression = get_user_hands_expression(lexeme_pair.offset_lexeme, buffer,
  is_third_view, user_state_id, weight_driving_animation);`  (1st arg = `[ebp-84h]` =
  `lexeme_pair + 0x84` = `offset_lexeme`; 5th arg = `[ebp+18h]` = `weight_driving_animation`)
- `'57'`: `return lexeme_pair.main_lexeme + expression(lexeme_pair.offset_lexeme) + hands_expression;`

IDENTICAL source to `weapon_core_fire_state::weapon_and_hands_expression` (#193) modulo the extra
5th arg on our `get_user_hands_expression`. Residual is the documented operator+ template-selection
/ inline-vs-call LTCG on the `main + expression(offset)` sub-expression (target
`operator+<animation_lexeme>` out-of-line; base `operator+<addition_lexeme,expression>` inline).
Same wall as #193's 82.52%. PARTIAL.

## get_user_hands_expression  12.78% -> 73.17%

Was a placeholder `return expression(weapon_lexeme)`. Target asm (0x79c5b0) mirrors
`weapon_core_fire_state::get_user_hands_expression` (#193) but with a 5-param signature (extra
`weight_driving_animation`), the `"stand_shot_double_barrel" / "crouch_shot_double_barrel"`
captions, and the `animation_lexeme_parameters` ctor's 5th arg = `&weight_driving_animation`
(weapon_core_fire_state passes `NULL`). Statements:
- `if (user_state_id == type_sprint) return expression(weapon_lexeme);`
- `u32 user_animation_index = (user_state_id == type_crouch);`
- `selected_animation = m_user_animations[is_third_view != false][user_animation_index];`
- `if (pinned_ptr_const<cubic_spline_skeleton_animation>(selected_animation)->animation_type()
   != animation_type_additive) return expression(weapon_lexeme);`
- captions; `animation_lexeme_parameters(buffer, captions[idx], selected_animation, &weapon_lexeme,
   &weight_driving_animation).animated_object(m_weapon.get_user()).playback_type(
   play_once_and_freeze_at_end).additivity_priority(1);`
- `animation_lexeme override_lexeme(params); return expression(override_lexeme);`

Added includes: mixing_addition_lexeme.h, mixing_animation_lexeme_parameters.h,
cubic_spline_skeleton_animation.h, resources_pinned_ptr.h (same as #193).
Residual = same inline-vs-call LTCG class as #193's 73.51% (target keeps the lexeme_parameters
setters out-of-line; base inlines animated_object+playback_type; animation_type() COMDAT-folded).
PARTIAL.

## new_object  86.5% (unchanged - documented wall)

`return new(buffer.c_ptr()) double_barreled_weapon_core_fire_state(params->weapon,
computed_shooting_animation_time_scale(*animations, params->rounds_per_second), animations,
animations_count);`. The base FOLDS the STUB `computed_shooting_animation_time_scale` (returns a
constant) into `fldz` at the call site - no call; the target keeps it out-of-line and the LTCG
optimizer returns float in xmm0 (`movss [esp], xmm0`) with extra spill slots (frame 48h vs 0Ch).
Argument passing / inline-vs-call at the call boundary; not source-steerable until that callee is
matched. SAME wall as pistol sibling new_object (#174, 92.08%). PARTIAL.

## Regressions

The relink reported 16 regressed / 15 improved, net code +0.01%. The "regressed -> 0.0" set is
entirely COMDAT-fold / thunk / boost-template re-attribution noise (empty_stub, `[thunk]` vcalls,
speedtree/btPairSet/network/boost::asio symbols) disjoint from anything this unit edits; this is
the documented relink churn. Code coverage rose, confirming no real logic regression in the edited
TUs. `get_magazine_capacity` was the only cross-TU edit (weapon_core.cpp/.h) and it is used only by
this unit among matched code.
