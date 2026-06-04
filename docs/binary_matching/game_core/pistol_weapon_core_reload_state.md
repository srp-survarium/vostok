# pistol_weapon_core_reload_state (game_core)

Pistol WeaponCore reload-state subclass. Shape mirrors pistol_weapon_core_fire_state
(#174) + weapon_core_reload_state (#192). 5 functions in the unit:

- ctor                          IAE (protected, non-const)
- weapon_and_hands_expression   EBE (private virtual const)
- get_weapon_lexeme_pair        ABE (private const)
- get_user_hands_expression     ABE (private const)
- new_object (cook_template)    AAE (private)

Target RVAs (rich/target/index.jsonl): ctor 0x7989b0, new_object 0x798b60,
get_user_hands_expression 0x798bd0, get_weapon_lexeme_pair 0x798cf0,
weapon_and_hands_expression 0x798dc0. Base RVAs: new_object 0x453c40,
get_user_hands 0x453ca0, get_weapon_lexeme_pair 0x453dd0, weapon_and_hands 0x453ea0.

## Commands
- `nm binaries/objdiff/target/.../pistol_weapon_core_reload_state.cpp.obj` -> mangled
  access chars (IAE/EBE/ABE/ABE + cook AAE) -> header access specifiers.
- `pdb_fetch --target-index ... --rva <rva> --view target` for each fn.
- `pdb_fetch --base-index ... --rva <baseRva> --view base` for the base side after build.
- `pdb_fetch ... --view structure` for srcline mapping.
- `python3 scripts/rebuild.py` (no module arg), read report.json units[].functions[]
  fuzzy_match_percent.

Header given fire_state's protected-ctor/private-virtual layout + the
`template<T> friend class weapon_core_state_cook_template` and
`friend void ::vostok::use_game_core_pistol_weapon_core_reload_state()` anchor.
cook_template.h needed the new friend fwd-decl + friend line (else C2248 private
new_object at temp_include_all). temp_include_all.cpp got the anchor body (animations[16])
+ a call in the registration block.

## ctor -> 100% DONE
Two triple-nested loops (both arrays are [2][2][2] = 8 each, 16 total), single shared
`animation_index`. `ASSERT_CMP_U(animations_count, ==, 16)` (failure branch pushes 0x10=16)
+ trailing `ASSERT(UNKNOWN_EXPRESSION)`. First build: 100%.

## get_weapon_lexeme_pair -> 99.92% DONE (LTCG this-register, arg passing)
captions = {"pistol-reload","pistol-reload_empty"}; weapon_state_index = (ammo_in_magazine()
==0); identifier = captions[weapon_state_index]; selected = m_weapon_animations[is_third_view
!=0][user_state_id==type_crouch][weapon_state_index]; set_animation_to_wait; impl(buffer,
identifier, selected, &m_weapon, m_animation_playback_state, time_sync_group=2,
m_animation_timescale, playback=play_once_and_freeze_at_end(1),
linear_interpolator(s_aim_transition_time)).

First attempt had sync/playback swapped (sync=1, playback=play_once_and_remove_at_end=2) ->
99.89%, the two push consts at 0x85/0x94 were reversed in the diff. push@0x85 = playback_type
(closer arg), push@0x94 = time_sync_group. Target: 0x85 push 1, 0x94 push 2 -> playback=1
(freeze), sync=2. Fixed -> 99.92%. Sole residual: the `this` argument for the LTCG-folded ammo_in_magazine()
call in ecx (base) vs eax (target) - a link-time calling-convention register choice for the
implicit argument (the documented call-boundary exception). Marked DONE per MATCHING.md
("mark DONE when the ONLY remaining difference is argument passing").

## get_user_hands_expression -> 77.49% (setter/get_user/~params inline wall)
No early return (unlike base reload_state). captions[2][2] = stand/crouch x normal/empty
("..._reload_pistol" / "..._reload_empty_pistol"). user_state_index = (user_state_id==
type_crouch); weapon_state_index = (ammo==0); interpolator(s_aim_transition_time) local;
override_lexeme( params(buffer, captions[us][ws], m_user_animations[v][us][ws],
&weapon_lexeme, &weight_driving_animation).animated_object(get_user()).bones_mask(2)
.playback_type(play_once_and_freeze_at_end) ); return override_lexeme.

Control flow + statements all match. Residual is the known wall: target keeps
animation_lexeme_parameters setters / weapon_core::get_user() / ~animation_lexeme_parameters
OUT-OF-LINE (base @0xc0-0xe9 inlines them as direct member stores [ecx+20h], [eax+4Ch]=2,
[ecx+3Ch]=1; target calls animated_object/bones_mask/playback_type/~params). Header in-class
inlines under /Ob2 /GL; not de-inlinable from this TU. Same class as
weapon_core_reload_state::get_user_hands_expression (73.6%).

## weapon_and_hands_expression -> 73.33% (operator+ overload wall)
lexeme_pair = get_weapon_lexeme_pair(...); if(user_state_id==type_sprint) return
lexeme_pair.main_lexeme + lexeme_pair.offset_lexeme; ASSERT(UNKNOWN_EXPRESSION);
hands_expression = get_user_hands_expression(lexeme_pair.offset_lexeme, buffer, is_third_view,
user_state_id, weight_driving_animation); return hands_expression + lexeme_pair.main_lexeme +
lexeme_pair.offset_lexeme.

Control flow (sprint branch + cmp 2 + ASSERT + both returns) matches byte-for-byte. Residual
is the mixing operator+ overload family: target builds addition_lexeme by value +
cloned_in_buffer + ~addition_lexeme + expression::expression<addition_lexeme>; on-disk mixing
headers only provide the inline `operator+ -> addition_lexeme&` form. Cross-unit header gap
(same wall as weapon_core_reload_state, 83.52% there; lower here due to the extra sprint
branch and the operator+<expression,animation_lexeme> selection). Not source-steerable here.

## new_object -> 86.5% (computed_reload LTCG calling convention)
new(buffer.c_ptr()) pistol_weapon_core_reload_state(params->weapon,
computed_reload_animation_time_scale(animations[0], params->reload_time), animations,
animations_count). Placement-new + ctor call + arg order match. Residual: target emits
`call computed_reload_animation_time_scale` with LTCG calling convention (ref in reg, ret
xmm0->movss). Base keeps the inline-STUB helper (return 0.0f -> fldz). Same as
weapon_core_reload_state::new_object (86.5%); bounded by that helper's own unit.

## Regressions
report-changes.json: 21 regressed / 21 improved, all unrelated folded COMDATs (sun_cascade
dtor, ai::selectors dtors, btSoftBody, mutex, buffer_string::operator=, etc.) reassigned
between objects when the new TU is enabled. Several names appear in BOTH lists. Standard
COMDAT-fold churn from adding a TU, net-neutral; no source-caused regression.
