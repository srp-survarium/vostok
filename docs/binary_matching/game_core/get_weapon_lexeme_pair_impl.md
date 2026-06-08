# game_core::get_weapon_lexeme_pair_impl

Free function in `survarium`, target rva `0x7a13e0` (carcass said 0x7b13e0 = +0x10000).
Builds the MAIN animation_lexeme_parameters (`[ebp-58h]`) + main animation_lexeme
(`[ebp-0E0h]` temp), then the OFFSET parameters (`[ebp-138h]`) + offset animation_lexeme
(`[ebp-1C0h]` local), then returns `weapon_lexeme_pair`.

Returns `weapon_lexeme_pair` by value (0x108 bytes) -> hidden sret in `[ebp+8]`, so the
visible args shift one slot: buffer=[ebp+0Ch], identifier=[ebp+10h], animation=[ebp+14h],
animated_object=[ebp+18h], playback_state=[ebp+1Ch], time_synchronization_group=[ebp+20h],
time_scale=[ebp+24h], playback_type=[ebp+28h], interpolator_for_offset_lexeme=[ebp+2Ch].

## Target asm (key shape)
- main ctor: `animation_lexeme_parameters( buffer, identifier, animation, NULL, NULL )` (the
  resource_ptr 5-arg ctor). identifier arg dropped by LTCG (MASTER_GOLD unreferenced); `this`
  in edi (LTCG conv). pushes only 0,0,animation,buffer.
- main chain: .animated_object .playback_type .bones_mask(2) .weight_synchronization_group_id(0x80)
  .weight_interpolator(interpolator) .time_scale(time_scale) .time_synchronization_group_id(time_sync).
- stmt 40: `mov byte[ebp-5],0` (no following lea/call) just before main lexeme ctor. Guessing a
  compiled-out `ASSERT( UNKNOWN_EXPRESSION )`.
- stmt 41: `animation_lexeme main_lexeme( main_lexeme_parameters )` -> [ebp-0E0h] (temp/local, NOT in
  PDB LOCALS list).
- offset ctor: 4th arg = `time_synchronization_group != u32(-1) ? &main_lexeme : NULL` via
  `push 0; mov edx,[ebp+20h]; sub edx,0FFFFFFFFh; neg; sbb edx,edx; lea eax,[ebp-0E0h]; and edx,eax; push edx`.
- offset chain: .animated_object .playback_type .start_animation_interval_id(playback_state.interval_id)
  [1 finalize_impl ASSERT from R_ASSERT_CMP] .start_animation_interval_time(playback_state.interval_time)
  [2 finalize_impl ASSERTs] .bones_mask(-3 = body_part_whole_body_but_hands)
  .weight_synchronization_group_id(0x100 = 256).
- stmt 55/56: `if ( ! offset_lexeme_parameters.time_driving_animation() ) { ASSERT(...); .time_scale(time_scale); }`
  (reads offset params +0xC = m_time_driving_animation, cmp 0, jne over the body).
- return stmt 61: build offset_lexeme [ebp-1C0h] from offset params, then copy: pair@0x00 <- offset_lexeme,
  pair@0x84 <- main_lexeme temp. So `return weapon_lexeme_pair( offset_lexeme, main_lexeme )`.

CONSTANTS (local enum): all_but_offset_weight_synchronization_group_id=256, offset_only=128.

CALL TREE: all callees in already-compiled `animation` module (need only decls, which exist in
mixing_animation_lexeme_parameters.h / mixing_animation_lexeme.h). finalize_impl = compiled-out
ASSERT (delinker misname) - recover with ASSERT. NO game_core STUB callees.

## Commands run (verbatim, in order)
- pdb_rich_query --index binaries/rich/target/index.jsonl --function get_weapon_lexeme_pair_impl --list
- pdb_fetch --target-index binaries/rich/target/index.jsonl --function get_weapon_lexeme_pair_impl --view target
- pdb_fetch ... --view callees
- pdb_fetch ... --view info
- python3 scripts/rebuild.py   # no module arg

## Iterations
1. INPUT: v1 - full body (resource_ptr main ctor, two setter chains, the select for the
   offset 4th arg, the `if ( ! time_driving_animation() )` guard with ASSERT + time_scale,
   `return weapon_lexeme_pair( offset_lexeme, main_lexeme )`). L40 written as
   `ASSERT( UNKNOWN_EXPRESSION )`.
   BUILD: fuzzy_match_percent = None. 53 regressed/58 improved = documented relink ICF/vcall
   churn (vector-deleting-dtors, thunks, btXxx flipping 0<->100); NONE are this fn or its callers.
   DIFF: `--view base` shows the body matches the target statement-for-statement EXCEPT:
   (a) L40 ASSERT over-produced: base `mov byte; lea; call dummy::nonnull` (<0x12>) vs target's
       bare `mov byte[ebp-5],0` (<0x4>) - so L40 is NOT an ASSERT, it's a lone 4-byte dead store.
   (b) the trivial inline-in-class setters animated_object/playback_type/bones_mask/
       weight_interpolator/start_animation_interval_id INLINED in base (e.g. bones_mask ->
       `mov dword[params+0x4C],2`) while TARGET keeps them out-of-line `call`s. Confirmed by
       querying both rich indexes: TARGET has ALL setters out-of-line (rvas 0x098d90 etc.);
       BASE has only weight_synchronization_group_id/time_scale/time_synchronization_group_id
       out-of-line, the rest inlined everywhere. Whole-program LTCG inline-vs-call.
   (c) base folds ~animation_lexeme_parameters to the empty-fn (`call dummy::nonnull`); target
       keeps `call ~animation_lexeme_parameters`. Same fold class.
2. INPUT: L40 `ASSERT(...)` -> `bool dummy = false;`.
   BUILD: still None; 0 regressed / 0 improved (the L40 change had no scoring effect because the
   fn was already None from the inline-vs-call divergence). DIFF: L40 now emits a bare
   `mov byte[ebp-169h],0` (<0x7>, the 7 bytes vs target's 4 is only the slot-offset disp size -
   allocation noise) - structurally a bare store, matching the target shape. Everything else
   unchanged: select (0x8e-0xab) byte-identical to target 0x84-0xa7; if-guard reads offset
   params+0xC, cmp, jne - identical; L56 ASSERT now matches (<0x12> both); return copies
   offset_lexeme->pair@0x00 and main_lexeme->pair@0x84 - identical to target.

## Outcome
STATE[None|PARTIAL]. The body is a faithful 1:1 reconstruction; every statement, constant,
control-flow branch, the `group != u32(-1) ? &main_lexeme : NULL` select, and the return arg
order are byte-exact in the regions that are not inlined. Blocker: whole-program inline-vs-call
of the trivial `animation_lexeme_parameters` setters - TARGET keeps them out-of-line, our /GL
LTCG inlines ~6 of them, shifting the entire [ebp-XX] layout and shortening the body (base ~415
bytes vs target 503), which exceeds objdiff's pairing threshold -> None. Same unsteerable class
as scheduler::on_frame (vectora::size/operator[]) / weapon_and_hands_expression (operator+) /
fixed_string ctor in assembly_patterns.md. The setters live in the out-of-scope `animation`
module headers as inline-in-class COMDATs; the only known lever (move them out-of-line per the
"Inline class-body accessor" pattern) is engine-wide and out of scope for this unit.
Regressions caused: none (the 53/58 churn is relink ICF/vcall-fold noise, not this fn or callers).
Inlining: the residual IS the inlining - documented above.

## Note on L40
The lone 4-byte `mov byte[ebp-N],0` standalone statement (no lea/call) is an unused `bool` local,
reproduced as `bool dummy = false;`. An ASSERT would emit lea+call (<0xc>); the interval setters'
R_ASSERT_CMP empty_stubs come from the inlined setter bodies, not from source ASSERTs here.

## Pairing blocker found: enum tag name mismatch (sweep, #159 follow-up)
A SECOND, more fundamental reason objdiff left this `None` (in addition to the body
inline-vs-call divergence above): the `playback_enum` parameter's enum was declared in
`sources/vostok/animation/mixing.h` as `enum playing_type_enum {...}; typedef playing_type_enum
playback_enum;`. MSVC mangles a parameter by the enum's REAL TAG, so our base symbol became
`?get_weapon_lexeme_pair_impl@...IMW4playing_type_enum@mixing@84@...` while the target uses
`...IMW4playback_enum@mixing@84@...`. Different symbol names cannot pair, full stop.

Evidence (index.jsonl mangled-name counts): target has 0 `playing_type_enum` and 2
`playback_enum@mixing`; base had 2 `playing_type_enum@mixing` and 0 `playback_enum@mixing`. The
two affected base symbols were THIS function and the `weapon_core_shotgun_reload_base_substate`
ctor - both confirmed `playback_enum@mixing@animation` in the target.

Fix: renamed the enum tag to `playback_enum` (dropped the typedef) in mixing.h. Byte-neutral for
all source (every call site already used the `playback_enum` alias), corrects both mangled names.
Resolved the line-30 `sushi@TODO`. After this the symbols pair by name; any remaining residual is
the body inline-vs-call class above. Also resolved: `.bones_mask( 2 )` -> the named
`animation::body_part_hands_only` (= 0x0002 in animation/type_definitions.h).
