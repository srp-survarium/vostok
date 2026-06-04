# weapon_core_aimed_fire_state (concrete WeaponCore aimed-fire-state subclass)

Unit = the 5 stub functions in
`sources/vostok/game_core/sources/weapon_core_aimed_fire_state.cpp`:
ctor, weapon_and_hands_expression, get_weapon_lexeme_pair, get_user_hands_expression,
and `weapon_core_state_cook_template<weapon_core_aimed_fire_state>::new_object`.

Branch: `match/game_core-weapon_core_aimed_fire_state` off `origin/int/game_core`.
Worktree: `/home/sheep/Projects/surv/vostok_6`.

## Approach

Near-identical structural twin of the already-matched `weapon_core_fire_state.cpp`
(same ctor/get_weapon_lexeme_pair/get_user_hands_expression/new_object shapes,
8 animations). Reused that file as the template; differences are string literals
and one extra control-flow branch in weapon_and_hands_expression.

## Target rvas (binaries/rich/target/index.jsonl)

- ctor                       0x799480  `??0...@@IAE@...`  protected non-const
- new_object                 0x7995f0  `?new_object@...@@AAE...`  private (cook_template)
- get_user_hands_expression  0x799660  `?...@@ABE...`  private const
- get_weapon_lexeme_pair     0x7997d0  `?...@@ABE...`  private const
- weapon_and_hands_expression 0x799890 `?...@@EBE...`  private virtual const

Header access fixed BEFORE matching: ctor `protected`, the three methods `private`
(fire_state had get_* protected; aimed has them private - `A` vs `I`). Added the
`weapon_core_state_cook_template` + `use_game_core_weapon_core_aimed_fire_state`
friends and a temp_include_all anchor (new_object is private in the cook template,
so the anchor needed a matching friend in weapon_core_state_cook_template.h).

## Commands

    pdb_rich_query --index binaries/rich/target/index.jsonl --list --function weapon_core_aimed_fire_state
    pdb_fetch --target-index ... --rva 0x799480 --view target   # ctor
    pdb_fetch --target-index ... --rva 0x7997d0 --view target   # get_weapon_lexeme_pair
    pdb_fetch --target-index ... --rva 0x799660 --view target   # get_user_hands_expression
    pdb_fetch --target-index ... --rva 0x799890 --view target   # weapon_and_hands_expression
    pdb_fetch --target-index ... --rva 0x7995f0 --view target   # new_object
    grep -oE '...@@[A-Z]+' binaries/rich/target/index.jsonl     # access chars
    python3 scripts/rebuild.py                                  # no module arg

## Per-function results (report.json top-level fuzzy_match_percent)

### ctor -> 100% DONE
Identical to fire_state ctor: `ASSERT_CMP_U( animations_count, ==, 8 )` (the top
`push 0; push 8; push count; call finalize_impl`), two nested view/user_state
double loops copying `animations[idx++]` into `m_weapon_animations` then
`m_user_animations`, trailing `ASSERT( UNKNOWN_EXPRESSION )` (the `[ebp-16h]=0;
finalize_impl` carcass). The two vtable stores (fsm_state, unmanaged_resource) are
normal ctor codegen. First build: 100%.

### get_weapon_lexeme_pair -> 100% DONE
Byte-identical to fire_state's get_weapon_lexeme_pair EXCEPT the animation identifier
string: `"weapon-aimed_fire"` (mangled `??_C@_0BC@...@weapon?9aimed_fire?$AA@`) vs
fire_state's `"weapon-fire"`. Same `m_weapon_animations[is_third_view!=false][user_state_id==type_crouch]`
selection, same `set_animation_to_wait`, same `get_weapon_lexeme_pair_impl(...)` call
with `m_weapon.get_bullets_in_queue() <= 1 ? play_once_and_freeze_at_end : play_cyclically`
and `linear_interpolator( s_aim_transition_time )`. First build: 100%.

### get_user_hands_expression -> 73.51% PARTIAL
Byte-identical structure to fire_state's get_user_hands_expression (sprint
early-return, the animation_type()!=additive early-return via pinned_ptr_const, the
lexeme_parameters build + setter chain, both expression returns), EXCEPT the captions:
`{ "aimed_shoot", "crouch_aimed_shoot" }` (mangled `aimed_shoot` / `crouch_aimed_shoot`)
vs fire_state `{ "stand_shoot", "crouch_shoot" }`.
Residual is the SAME unsteerable inline-vs-call /GL wall as fire_state (73.51%): the
target keeps the animation_lexeme_parameters setters OUT-OF-LINE while our build inlines
animated_object+playback_type (only additivity_priority stays a call), shifting layout
and rescheduling m_weapon.get_user(). animation_type() is COMDAT-folded onto a trivial
getter delinked as inventory_holder::scheduler. Whole-program decisions, not source-steerable.

### new_object -> 86.5% PARTIAL
Identical to fire_state's new_object: `new ( buffer.c_ptr() ) ctor( params->weapon,
computed_shooting_animation_time_scale( *animations, params->rounds_per_second ),
animations, animations_count )`. Sole residual is the LTCG calling convention of
computed_shooting_animation_time_scale (target passes the managed_resource_ptr const&
arg in a register, returns float in xmm0; our STUB callee is default cdecl). Call-boundary
argument passing - the documented LTCG-arg wall, identical to fire_state new_object (86.5%).

### weapon_and_hands_expression -> 78.53% PARTIAL
DIFFERS from fire_state: the aimed version has an extra branch on user_state_id.
Target (0x799890): after `lexeme_pair = get_weapon_lexeme_pair(...)`,
`cmp [ebp+14h],2 / je .1 / cmp [ebp+14h],3 / jne .2`, i.e.
`if ( user_state_id == type_sprint || user_state_id == type_jump )`.
- .1 branch: `return lexeme_pair.main_lexeme + lexeme_pair.offset_lexeme;`
  (addition_lexeme<animation_lexeme,animation_lexeme>(main,offset).cloned_in_buffer ->
  expression<addition_lexeme>) - the hands expression is NOT computed.
- .2 branch: `hands = get_user_hands_expression(offset,...);
  return main + expression(offset) + hands;` (the full fire_state tree).

Iterations:
- v1 (no branch, copied fire_state body): 65.94% - missing the entire .1 branch.
- v2 (added `if ( user_state_id == type_sprint || user_state_id == type_jump ) return
  main + offset;`): 78.53% - structure now matches both branches.

Residual is the mixing operator+ inline-vs-out-of-line /GL decision, flipping at BOTH
operator+ sites: in .1 the target INLINES operator+<animation_lexeme,animation_lexeme>
(addition_lexeme ctor + cloned_in_buffer visible) while our build keeps it OUT-OF-LINE;
in .2 the target keeps operator+<animation_lexeme> OUT-OF-LINE while our build INLINES
addition_lexeme. Same mixing-unit operator+ template-selection/inline wall as
weapon_core_fire_state::weapon_and_hands_expression (82.52%); two diverging sites here vs
one there, hence the lower %. Blocked on the mixing unit; not source-steerable here.

## Regressions / delinker churn

report-changes.json showed 15-28 COMDAT symbols flipping 100<->0 (boost _bi, btSoftBody,
btHashMap, threading::mutex/event, empty_stub, ai selectors, etc.). These are NON-deterministic
delinker tie-break churn: the set CHANGED between two rebuilds whose ONLY source diff was the
weapon_and_hands `if` (which cannot touch btSoftBody/boost/threading). weapon_core_hide_state
ctor briefly showed in the list but it is an unmatched STUB (all STATE[STUB]); nothing matched
was regressed. Overall matched_code_percent 27.3498 -> 27.3414 (jitter). No real regressions.
