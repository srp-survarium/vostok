# double_barreled_weapon_core_aimed_fire_state

AIMED-fire sibling of double_barreled_weapon_core_fire_state (#203). Union of:
- the double-barreled first/second-barrel `m_weapon_animation_index` machinery (3D weapon
  animations array, captions, magazine_capacity==2 ASSERT) from the fire_state sibling, and
- the aimed transition (`s_aim_transition_time` linear_interpolator, aimed captions).

## Target symbols (binaries/rich/target/index.jsonl)
- ctor                       0x79bda0  @@IAE  (protected)
- new_object                 0x79bf70  @@AAE  (private template)
- get_user_hands_expression  0x79bfe0  @@ABE  (private const)
- get_weapon_lexeme_pair     0x79c160  @@ABE  (private const)
- weapon_and_hands_expression 0x79c230 @@EBE  (private virtual const)
- initialize                 0x79c320  @@MAE  (protected virtual)

Header access layout copied verbatim from double_barreled_weapon_core_fire_state.h (identical
access chars). Scaffolding: anchor use_game_core_double_barreled_weapon_core_aimed_fire_state
in temp_include_all.cpp + fwd-decl/friend in weapon_core_state_cook_template.h.

## Per-function notes

### ctor (0x79bda0)
Byte-for-byte the double_barreled fire_state ctor: member-init m_weapon_animation_index(u32(-1))
(mov [edx+178h],0FFFFFFFFh @0x61, BEFORE the ASSERTs), then
ASSERT_CMP_U(get_magazine_capacity()==2), ASSERT_CMP_U(animations_count==12), triple loop fill of
m_weapon_animations[2][2][2], double loop fill of m_user_animations[2][2], trailing ASSERT.

### initialize (0x79c320)
Identical to double_barreled fire_state initialize: base::initialize(); ASSERT;
m_weapon_animation_index = m_weapon.ammo_in_magazine() != 2. (LTCG this-in-eax at ammo call.)

### get_weapon_lexeme_pair (0x79c160)
Same as double_barreled fire_state get_weapon_lexeme_pair but captions
{ "aimed_shot_first_barrel", "aimed_shot_second_barrel" } and aimed linear_interpolator
( s_aim_transition_time ); playback_type = play_once_and_freeze_at_end, time_sync 1.

### weapon_and_hands_expression (0x79c230)
EBE = private virtual const. Like double_barreled fire_state weapon_and_hands but with a LEADING
ASSERT( UNKNOWN_EXPRESSION ) (carcass line '54' @0x10). Then get_weapon_lexeme_pair,
get_user_hands_expression(offset_lexeme,...,weight_driving_animation), return main+expr(offset)+hands.

### get_user_hands_expression (0x79bfe0)
Aimed double variant. Sprint early-return, animation_type()!=additive early-return, captions
{ "aimed_stand_shot", "aimed_crouch_shot" }, a linear_interpolator( s_aim_transition_time ) LOCAL
(built @0xca-0xd5, between captions and the lexeme_parameters build; constructed-only, not a ctor
arg — the 5-arg animation_lexeme_parameters ctor takes (buffer, caption, animation, &weapon_lexeme,
&weight_driving_animation)), then animated_object/playback_type/additivity_priority(1) and the
expression(override_lexeme) return.

### new_object (0x79bf70)
placement-new + computed_shooting_animation_time_scale (STUB-folded → fldz, LTCG residual same as
all siblings).

## Build / score log

`nix develop -c python3 scripts/rebuild.py` (no module arg) -> +7 functions, EXE relinked.
report.json units[].functions[].fuzzy_match_percent for this TU:
- ctor                        99.96%  (LTCG this-in-eax @ get_magazine_capacity)
- initialize                  99.76%  (LTCG this-in-eax @ ammo_in_magazine)
- get_weapon_lexeme_pair     100.00%  DONE
- weapon_and_hands_expression 83.90%  (operator+ inline-vs-out-of-line, == fire_state #203 83.18%)
- get_user_hands_expression   75.61%  (lexeme_parameters setter inline-vs-call + COMDAT-folded
                                       animation_type(), == fire_state #203 73.17%)
- new_object                  86.50%  (computed_shooting_animation_time_scale STUB-fold / xmm0 ret)

Diffs verified: weapon_and_hands base (0x454aa0) matches target (0x79c230) through the ASSERT and
both calls; only the operator+ tree layout diverges. get_user_hands base (0x454870) matches target
(0x79bfe0) statement-for-statement incl the interpolator local @0xbd; only the setter call/inline
ordering + the COMDAT-folded animation_type() callee diverge. All walls are the documented sibling
walls (inline-vs-out-of-line /GL template selection + STUB-callee fold), not source-steerable here.

## RE-MATCH 2026-06-07 (claude): structure fix get_user_hands_expression

Structure-verifier flagged STRUCTURE-WRONG: target = 12 statements, base = 14. The base
split the params build into a named `override_lexeme_parameters x(...);` decl PLUS a separate
`x.setter().setter()...;` statement (2 statements). Target chains ctor + all setters into ONE
statement (carcass L101, 0x6e) feeding the `animation_lexeme override_lexeme( ... )` construction.

FIX: dropped the named params local, chained `.animated_object().playback_type().additivity_priority(1)`
directly onto the `animation_lexeme_parameters( ... )` temporary inside the `override_lexeme` ctor -
exactly the shape of the matching show/hide siblings.

Commands:
  pdb_rich_query --index binaries/rich/target/index.jsonl --list --function 'double_barreled_weapon_core_aimed_fire_state::get_user_hands'   # 0x79bfe0
  pdb_rich_query --index binaries/rich/base/index.jsonl   --list --function 'double_barreled_weapon_core_aimed_fire_state::get_user_hands'   # 0x457970
  pdb_fetch --base-index ... --target-index ... --view structure --rva 0x79bfe0   # target: 12 statements
  pdb_fetch --base-index ... --target-index ... --view structure --rva 0x457970   # base now: 12 statements

Result: 75.61% -> 79.39%. Statement count base 12 == target 12 (structure matched).
Residual: unsteerable LTCG inline-vs-call (target keeps animated_object/playback_type setters
out-of-line; base inlines them). Big chained statement now single on both sides (base 0x72 vs target 0x6e).
