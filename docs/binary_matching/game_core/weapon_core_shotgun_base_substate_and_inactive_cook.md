# weapon_core_shotgun_reload_base_substate + weapon_core_inactive_state_cook

Unit: "pick up what was blocked" plus the sibling that frees PARTIALs.

1. `weapon_core_shotgun_reload_base_substate::weapon_and_hands_expression` and
   `::get_weapon_lexeme_pair` - matching these unblocks
   `weapon_core_shotgun_reload_state::weapon_and_hands_expression` (was a
   VOSTOK_UNREACHABLE_CODE placeholder).
2. `weapon_core_inactive_state_cook` (6 methods). `allocate_resource` (target
   0x58ef60) is byte-identical to the shotgun cook's allocate/deallocate PARTIALs
   (only malloc size differs: 0x138 vs 0x140), so the pattern lifts those.

Worktree: `/home/sheep/Projects/surv/vostok_3`. Branch off `origin/int/game_core`.

## Commands (verbatim)
- `git checkout -b match/game_core-shotgun_base_substate_and_inactive_cook origin/int/game_core`
- `nix develop -c pdb_rich_query --index binaries/rich/target/index.jsonl --function "weapon_core_shotgun_reload_base_substate" --list`
- `nix develop -c pdb_rich_query --index binaries/rich/target/index.jsonl --function "weapon_core_inactive_state_cook" --list`
- `nix develop -c pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x798840 --view target|structure`  (weapon_and_hands_expression)
- `nix develop -c pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x798790 --view target|structure|callees`  (get_weapon_lexeme_pair)
- `nix develop -c pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x589db0 --view target`  (shotgun_reload_state::weapon_and_hands_expression - the unblocked caller)
- `nix develop -c pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x58ee80|0x58eec0|0x58eee0|0x58ef30|0x58ef60|0x58efe0 --view target`  (inactive cook 6 methods)
- `nix develop -c python3 scripts/rebuild.py`

## base_substate::get_weapon_lexeme_pair (target 0x798790)
4 statements. Decode:
- L57: `m_animation_to_wait_for = m_weapon_animations[is_third_view!=0][user_state_id==type_crouch];`
  (index `[ecx + view*8 + 128h] + (user==1)*4`; operator= on this+0x124).
- L60: `return get_weapon_lexeme_pair_impl( buffer, m_animation_id, m_animation_to_wait_for /*selected*/,
  &m_weapon, *m_animation_playback_state, m_time_synchronization_group, m_animation_timescale,
  m_playback_type, animation::linear_interpolator( s_aim_transition_time ) );`
  pushes (reverse): linear_interpolator(s_aim_transition_time), m_playback_type(0x150),
  m_animation_timescale(0x14C, fld/fstp), m_time_synchronization_group(0x154),
  m_animation_playback_state(0x148), &m_weapon(0x120), &m_animation_to_wait_for(0x124),
  m_animation_id(0x158), buffer. Trailing `call finalize_impl` (compiled-out ASSERT) before ret.
Modeled on the sibling `pistol_weapon_core_fire_state::get_weapon_lexeme_pair` /
`weapon_core_aimed_state::get_weapon_lexeme_pair` (both 100%/99%).

## base_substate::weapon_and_hands_expression (target 0x798840)
6 statements, 0x162 bytes. LOCALS: override_lexeme (animation_lexeme), user_state_index (u32),
lexeme_pair (weapon_lexeme_pair). Decode:
- L69: `weapon_lexeme_pair lexeme_pair = get_weapon_lexeme_pair( buffer, is_third_view, user_state_id );`
  (sret [ebp-108h]; main_lexeme @+0 = [ebp-108h], offset_lexeme @+0x84 = [ebp-84h]).
- L71: `u32 user_state_index = user_state_id == type_crouch;` (cmp [ebp+14h],1; sete).
- L83: build override_lexeme [ebp-198h] from
  `animation_lexeme_parameters( buffer, m_hand_animation_captions[user_state_index],
   m_user_animations[is_third_view!=0][user_state_index], &lexeme_pair.offset_lexeme,
   &weight_driving_animation ).animated_object( m_weapon.get_user() ).bones_mask( 2 )
   .playback_type( m_playback_type )`. (identifier dropped by LTCG/MASTER_GOLD; the user-anim
   array at 0x138; time_driving = &lexeme_pair.offset_lexeme ([ebp-84h]); weight_driving =
   &weight_driving_animation.) Mirrors pistol_weapon_core_show_state::get_user_hands_expression.
- L84: `return override_lexeme + lexeme_pair.main_lexeme + lexeme_pair.offset_lexeme;`
  (two addition_lexeme + cloned_in_buffer + expression<addition_lexeme>(sret); same chained
  operator+ as pistol_weapon_core_show_state::weapon_and_hands_expression).

## shotgun_reload_state::weapon_and_hands_expression (target 0x589db0) - UNBLOCKED
`weapon_core_shotgun_reload_base_substate* current = static_cast<...>( m_logic->current_state() );
 return current->weapon_and_hands_expression( buffer, is_third_view, user_state_id, weight_driving_animation );`
m_logic @ this+0x138; current_state() @ [m_logic+0x10]. Was a VOSTOK_UNREACHABLE_CODE placeholder
(could not reference the no-return base_substate STUB without LNK1257); restored now that the
override is matched. Re-anchored in temp_include_all via a member-function pointer to
weapon_and_hands_expression escaped through example_callback.

## weapon_core_inactive_state_cook (6 methods)
- `~`: empty `{}` (calls inplace_unmanaged_cook::~ COMDAT-fold). Expected 100%.
- `destroy_resource`: `static_cast<weapon_core_inactive_state*>(resource)->~weapon_core_inactive_state();`
  (sibling shotgun cook destroy = 100%). Expected 100%.
- ctor: `: unmanaged_cook( weapon_inactive_state_class, reuse_false, use_current_thread_id,
  use_current_thread_id )` (asm: class 0x12C, BOTH thread ids = 0xFFFFFFFD = -3,
  flags push 0) + `register_cook( this )`.
- `deallocate_resource`: `VOSTOK_FREE_IMPL( g_allocator, (resource_base*&)buffer );` -
  byte-identical to shotgun cook deallocate (same out-of-line free_helper PARTIAL class).
- `allocate_resource`: `ASSERT( UNKNOWN_EXPRESSION ); return mutable_buffer( MALLOC(0x138), 0x138 );`
  byte-identical to shotgun cook allocate (the compiled-out validator ASSERT + malloc; only size
  0x138 vs 0x140). Same residual: ASSERT validator-by-value not recovered, malloc/mutable_buffer
  inline-vs-call.
- `create_resource`: placement-new weapon_core_inactive_state(params->weapon) into the buffer,
  set_unmanaged_resource(unmanaged_resource_ptr(obj), memory_usage_type(nocache_memory,0x138)),
  finish_query(result_success). Heavy; best-effort.

## Iterations
1. First build failed: `weapon_core_inactive_state.h(16)` ctor `weapon_core_base_state( weapon )` -
   base ctor needs `(weapon, bool serialize_animation_state)`; asm passes false -> changed to
   `weapon_core_base_state( weapon, false )`. And `weapon_core_shotgun_reload_base_substate.cpp(84)`:
   `m_animation_to_wait_for = selected` failed because the method is const and the member non-const ->
   made `m_animation_to_wait_for` `mutable` (target writes it in a const method, so it must be mutable).
2. Second build: C4716 -> LNK1257 on `weapon_core_inactive_state::has_animation_ended` (a no-return
   STUB). create_resource placement-news the object, emitting its vtable, which references the
   no-return virtual. `VOSTOK_UNREACHABLE_CODE()` did NOT suppress C4716 for a bool return; changed
   the STUB body to `return true;` (out of unit scope, harmless placeholder). After touching the
   defining .cpp the link succeeded.
3. Build OK. Scores: base_substate weapon_and_hands_expression 68.18% (PARTIAL, structure identical,
   LTCG setter inline-vs-call); inactive cook ctor 79.07%, allocate 41.38%, deallocate 55.64%,
   create_resource 91.97%, dtor + destroy_resource 100%. get_weapon_lexeme_pair and the unblocked
   shotgun_reload_state::weapon_and_hands_expression show total_code 0 in report.json: they ARE
   emitted in the base build (0x449160 and 0x44eba0) but objdiff does not score them (newly emitted,
   absent from the delink unit's function list).
4. get_weapon_lexeme_pair: first version used a `selected_animation` reference local + assignment
   (5 statements, but target uses the just-assigned member as the impl `animation` arg). Removed the
   local, assigning + passing `m_animation_to_wait_for` directly -> base asm (0x449160) is now
   BYTE-IDENTICAL to target 0x798790 (0xb0 bytes, every instruction incl. the trailing ICF-folded
   empty-stub). 100% (objdiff unscored).
5. shotgun_reload_state::weapon_and_hands_expression: first version split cast into a `current` local
   (4 stmts). Target = 3 stmts, cast+call as ONE return. Rewrote as
   `return static_cast<base_substate*>( m_logic->current_state() )->weapon_and_hands_expression(...);`.
   Base asm (0x44eba0) now matches the target statement structure; residual is a single /Od scratch
   temp (target `sub esp,0Ch` vs base `sub esp,8`) - the current_state() result materialized into an
   extra slot. [REVIEWER CORRECTION] This is NOT a non-steerable /Od artifact and NOT byte-identical:
   the target (0x589db0) materializes the downcast into a NAMED local before the call -
   `mov [ebp-8],edx; mov eax,[ebp-8]; mov [ebp-4],eax` (two slots, sub esp,0Ch) - whereas the
   single-expression form emits only `mov [ebp-4],edx` (one slot, sub esp,8). The extra slot is the
   `current` local the first version had, just declared at the wrong cardinality. The target keeps
   BOTH a 4-stmt-equivalent local materialization AND a single statement (the structure says 3
   statements because the local-init+return collapse to one srcline). NEXT: restore
   `weapon_core_shotgun_reload_base_substate* current = static_cast<...>( m_logic->current_state() );
   return current->weapon_and_hands_expression(...);` and re-diff - the cardinality of the slot is
   source-steerable, this is INPROGRESS not DONE.

## Outcome (final STATE)
- weapon_core_shotgun_reload_base_substate::get_weapon_lexeme_pair -> 100%|DONE (objdiff unscored; byte-identical)
- weapon_core_shotgun_reload_base_substate::weapon_and_hands_expression -> 68.18%|PARTIAL (LTCG setter inline-vs-call)
- weapon_core_shotgun_reload_state::weapon_and_hands_expression -> INPROGRESS, UNBLOCKED (objdiff unscored; NOT byte-identical - missing the `current` named local, target has an extra [ebp-8] slot. [reviewer correction] NEXT: introduce the local. Was mislabeled 100%|DONE.)
- weapon_core_inactive_state_cook::~ -> 100%|DONE
- weapon_core_inactive_state_cook::destroy_resource -> 100%|DONE
- weapon_core_inactive_state_cook::create_resource -> 91.97%|PARTIAL (memory_usage_type/c_ptr COMDAT fold)
- weapon_core_inactive_state_cook::weapon_core_inactive_state_cook -> 79.07%|PARTIAL (enum 0x103 vs 0x12C engine-wide + LTCG ctor convention)
- weapon_core_inactive_state_cook::allocate_resource -> 41.38%|PARTIAL (validator ASSERT not recovered; same as shotgun cook)
- weapon_core_inactive_state_cook::deallocate_resource -> 55.64%|PARTIAL (free_helper out-of-line vs inlined; same as shotgun cook)

## Regressions
report-changes.json after the final build: 0/0/0/0 (no scored function changed by my final edit).
The first (int-delta) rebuild showed ~16 functions 100->0 - all relink ICF/vcall-fold noise
(float3 ctor, mutex dtor, btHashMap, network vcall thunks, empty_stub, game_console thunks,
boost binders) - the same churn documented in weapon_core_shotgun_reload_state_cook.md; none are
this unit's functions or their callers.

## Header edits (kept minimal, in scope)
- weapon_core_shotgun_reload_base_substate.h: `m_animation_to_wait_for` -> `mutable` (target writes
  it in the const get_weapon_lexeme_pair; no layout change).
- weapon_core_inactive_state.h: ctor `weapon_core_base_state( weapon )` -> `( weapon, false )`
  (asm-confirmed); `has_animation_ended()` STUB body `/* no source */` -> `return true;` (placeholder
  so the now-anchored vtable does not trip C4716; out of unit scope, marked STATE[STUB]).
</content>
</invoke>
