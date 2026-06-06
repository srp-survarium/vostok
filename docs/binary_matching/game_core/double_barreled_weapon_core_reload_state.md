# double_barreled_weapon_core_reload_state

Unit of 5 functions in `vostok/game_core/sources/double_barreled_weapon_core_reload_state.cpp`:
- ctor `??0...@@IAE@...` (protected)
- `weapon_core_state_cook_template<...>::new_object` `?new_object@...@@AAE...` (private)
- `get_user_hands_expression` `?...@@ABE...` (private const)
- `get_weapon_lexeme_pair` `?...@@ABE...` (private const)
- `weapon_and_hands_expression` `?...@@EBE...` (private virtual const)

Sibling of `double_barreled_weapon_core_fire_state` (same shape).

## Target RVAs (binaries/rich/target/index.jsonl)
- 0x798f20 ctor
- 0x799100 new_object
- 0x799170 get_user_hands_expression
- 0x7992c0 get_weapon_lexeme_pair
- 0x7993a0 weapon_and_hands_expression

## Resume context
Previous matcher crashed with body source mostly written but two build blockers:
1. temp_include_all anchor `use_game_core_double_barreled_weapon_core_reload_state`
   called private `new_object` without a friend decl -> C2248. Fixed by adding
   the forward decl + friend entry in weapon_core_state_cook_template.h (matching
   the fire_state pattern) AND a `template<typename T> friend class
   weapon_core_state_cook_template;` + `friend void ::vostok::use_..._reload_state();`
   in the reload header.
2. Header had all members `public` -> wrong mangled access char -> objdiff scored
   0% (no pairing). Fixed access specifiers to match target mangling:
   ctor protected, weapon_and_hands_expression private virtual, get_weapon_lexeme_pair
   / get_user_hands_expression private. NOTE: editing the .h alone did NOT recompile
   the .cpp; had to `touch` the .cpp to force the rebuild (mangled name comes from .cpp TU).

## Scores after access-specifier fix (touch + rebuild)
- ctor 99.966%
- new_object 86.5%
- get_user_hands_expression 73.313%
- get_weapon_lexeme_pair 99.929%
- weapon_and_hands_expression 83.517%

## Iteration 1 (batched)
- ctor: target splits the inner assignment (0x3a) from three `+0x2` closing-brace
  jmps (L31/L32/L33) -> braced the three nested for-loops in both nests.
- get_weapon_lexeme_pair: target attributes captions[0]/captions[1] to two source
  lines (L58/L59, two `<0x7>`); base merged into one `<0xe>` -> split the array
  initializer literal across two physical lines.
- weapon_and_hands_expression: target return is right-associative
  `hands + ( expression(main) + offset )` (expression::expression<animation_lexeme>
  on main, then operator+<animation_lexeme>, then operator+) -> changed return to
  `hands_expression + ( main_lexeme + offset_lexeme )`.
- get_user_hands_expression: target L96 calls `.weight_interpolator(interpolator)`
  (order: animated_object, weight_interpolator, bones_mask, playback_type). Tried
  inlining `.weight_interpolator( linear_interpolator( s_aim_transition_time ) )` as
  a temporary -> REGRESSED 73.66 -> 33.1 (wrong construction order/lifetime). Reverted
  to a named `interpolator` local + `.weight_interpolator( interpolator )` -> 73.66%
  (best, +0.35 over the 73.31 unused-local baseline).

## Iteration 2/3 results and final walls

Scores converged to:
- ctor 99.97% DONE -- structure byte-identical to target (the brace fix made the
  3 nested-loop closing braces emit their `+0x2` jmps exactly as the target). Residual
  is sub-byte reloc/symbol-attribution noise; carcass deleted.
- get_weapon_lexeme_pair 99.93% DONE -- structure byte-identical (the captions
  literal split across two physical lines gave the L58/L59 two-statement attribution).
  Residual reloc-level; carcass deleted.
- new_object 86.5% PARTIAL -- base FOLDS the STUB `computed_reload_animation_time_scale`
  (constant) into `fldz` at the call site; target keeps it out-of-line, LTCG returns
  float in xmm0 (`movss [esp],xmm0`) with frame 0x28 vs base 0x0C. Call-boundary
  inline-vs-call; not source-steerable until that callee is matched. IDENTICAL wall to
  fire_state sibling new_object (86.5%).
- get_user_hands_expression 73.66% PARTIAL -- animation_lexeme_parameters setter chain
  (animated_object/weight_interpolator/bones_mask/playback_type) + weapon_core::get_user
  kept out-of-line in target, inlined in base. IDENTICAL wall to fire_state sibling
  (73.17%). Tried right-assoc/temporary variants; none source-steerable.
- weapon_and_hands_expression 83.52% PARTIAL -- `hands + main + offset` operator+
  template-selection / inline-vs-call LTCG. Tried sibling's
  `main + expression(offset) + hands` form -> 83.18% (worse); kept the natural
  `hands + main + offset` (83.52%). IDENTICAL wall to fire_state sibling (83.18%).

No regressions vs the first-session report.json (the 10 deltas vs the 0603 baseline are
pre-existing COMDAT-fold/thunk/empty_stub re-attribution churn from prior commits already
on HEAD, disjoint from this unit's edits).
