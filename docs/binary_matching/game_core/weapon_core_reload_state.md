# weapon_core_reload_state (concrete WeaponCore reload-state subclass)

Unit: 5 functions in `sources/vostok/game_core/sources/weapon_core_reload_state.cpp`
- ctor `weapon_core_reload_state(weapon_core&, float, managed_resource_ptr const*, u32)` (rva 0x79a770)
- `weapon_and_hands_expression` (rva 0x79aae0)
- `get_weapon_lexeme_pair` (rva 0x79aa40)
- `get_user_hands_expression` (rva 0x79a950)
- `weapon_core_state_cook_template<weapon_core_reload_state>::new_object` (rva 0x79a8e0)

Branched off origin/int/game_core. Resumed from a prior interrupted matcher that had
written full first-approximations for all five (mirroring the matched
weapon_core_aimed_state sibling).

## Target index / commands
- `pdb_rich_query --index binaries/rich/target/index.jsonl --function 'weapon_core_reload_state::' --list`
- `pdb_fetch --target-index binaries/rich/target/index.jsonl --rva <rva> --view target`
- `pdb_fetch --target-index .../target/index.jsonl --base-index .../base/index.jsonl --rva <rva> --view diff`
- build: `nix develop -c python3 scripts/rebuild.py` (no module arg), scores from
  `binaries/objdiff/report.json` units[].functions[].fuzzy_match_percent.

## Asm findings
- ctor: ASSERT_CMP_U(count,==,8) at 0x5e; two double for-loops over [view][user_state]
  sharing a single `animation_index` ([ebp-4]); trailing bare `ASSERT(UNKNOWN_EXPRESSION)`
  at 0x152 (byte=0, finalize_impl, no branch).
- get_weapon_lexeme_pair: loads `s_aim_transition_time` (0x3f) - SAME static symbol as
  aimed_state, value 0.3f. time_synchronization_group push=2, time_scale=m_animation_timescale
  ([eax+140h]), playback=play_once_and_freeze_at_end(1). Trailing finalize_impl assert at 0x8d.
- get_user_hands_expression: early `if (user_state_id==type_sprint(2)) return weapon_lexeme;`
  via simple_lock copy; then a BARE `ASSERT(UNKNOWN_EXPRESSION)` at 0x27 (byte=0, finalize_impl,
  no branch) - prior worker MISSED this; added. user_state_index=(id==type_crouch(1)); captions
  {"stand_reload","crouch_reload"}; bones_mask=2, playback=play_once_and_freeze_at_end(1).
- weapon_and_hands_expression: get_weapon_lexeme_pair -> get_user_hands_expression(offset_lexeme,...)
  -> return hands + main + offset.
- new_object: placement new(buffer.c_ptr) weapon_core_reload_state(params->weapon,
  computed_reload_animation_time_scale(animations[0], params->reload_time), animations, count).
  NOTE: computed_reload_animation_time_scale is an inline STUB (returns 0.0f) in
  weapon_animations_timescale_inline.h - target emits a real CALL to it, base will inline the
  stub -> new_object likely cannot reach 100% until that helper is matched (cross-unit dep).

## Plumbing fixes required to even compile + match
1. temp_include_all anchor `use_game_core_weapon_core_reload_state` calls the cook's private
   `new_object` -> needed `friend void ::vostok::use_game_core_weapon_core_reload_state();` added
   to BOTH the fwd-decl block and the friend list in `weapon_core_state_cook_template.h`
   (every sibling anchor is already befriended there).
2. Header access specifiers MUST match the mangled access chars (else objdiff scores None):
   - ctor `??0...@@IAE` -> protected, non-const
   - `weapon_and_hands_expression` `@@EBE` -> private virtual const
   - `get_weapon_lexeme_pair` / `get_user_hands_expression` `@@ABE` -> private const
   Rewrote header from all-`public:` to protected ctor + private const members + the two
   friends (cook template + anchor), mirroring weapon_core_aimed_state.h.
3. `get_weapon_lexeme_pair` reads `m_animation_timescale` (offset 0x140) directly; it was
   `private` in weapon_core_reload_state_base -> made `protected` (access-only change, no
   layout/mangling/codegen impact on the already-matched base).

## Scores (report.json units[].functions[].fuzzy_match_percent)
After build3 (access + friend + protected, prior worker's bodies):
- ctor                         100.0%   DONE
- get_weapon_lexeme_pair       100.0%   DONE
- new_object                    86.5%   PARTIAL
- weapon_and_hands_expression   83.52%  PARTIAL
- get_user_hands_expression     73.6%   PARTIAL

## Why the three are walled (NOT call-arg LTCG, but whole-program /Ob2 /GL inline decisions)
Flags are `/Od /Ob2 /Oi /GL` -> inline expansion + LTCG are ON, so inline-vs-call is a
whole-program decision (the documented residual class; see weapon_core_aimed_state STATE note).
- new_object 86.5: target emits a real `call computed_reload_animation_time_scale`
  (`fld [params+0Ch]`); base inlines that helper to the constant `fldz` because
  `computed_reload_animation_time_scale` is an inline STUB (`return 0.0f`) in
  weapon_animations_timescale_inline.h. Bounded by that helper's own (separate) unit; once it
  is matched non-stub, new_object should reach 100%. Everything else (placement-new, ctor call,
  arg order weapon/timescale/animations/count) matches.
- get_user_hands_expression 73.6: first 7 statements (srcline 62,63,65,67,69,82,83) match the
  target carcass exactly. Residual is target keeping `animation_lexeme_parameters::animated_object/
  bones_mask/playback_type`, `weapon_core::get_user`, and `~animation_lexeme_parameters` as
  OUT-OF-LINE calls while base inlines all of them (base body = 37 lines vs target 22; the extra
  `<n>` after stmt '138'/'82' are the inlined setters+dtor). These are in-class `inline` header
  defs - cannot be de-inlined from this TU without editing shared headers (out of scope, breaks
  other units).
- weapon_and_hands_expression 83.52: 3 statements; first two (+0x01f, +0x02a) match the target
  byte-for-byte. The `return hands + main + offset` statement differs (target +0x07a vs base
  +0x053): target builds an extra `expression(animation_lexeme)` temporary + 3 binary_tree
  intrusive_ptr dtors -> a different expression-template instantiation/inline set, same
  whole-program class. Left-assoc source matches the structure; the divergence is codegen, not
  shape.

## Iterations
- build1 (prior worker bodies, before access fix): FAILED - anchor C2248 private new_object.
- build2 (+anchor friend on reload header only): FAILED - cook new_object still private +
  m_animation_timescale private.
- build3 (+cook-template friend, +protected timescale, +access specifiers, +get_user_hands
  ASSERT(UNKNOWN_EXPRESSION) at 0x27): SUCCESS -> scores above.
- build4 (+const locals user_state_index/animation_captions per carcass; restored <0>..<11>
  markers): scores unchanged (const is codegen-neutral), faithfulness only.
