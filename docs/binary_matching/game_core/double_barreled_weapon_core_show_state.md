# double_barreled_weapon_core_show_state

Sibling of `pistol_weapon_core_show_state` (already matched) and
`weapon_core_show_state`. Same 5-function shape. Reference results from pistol:
- ctor: 100% DONE
- weapon_and_hands_expression: 83.52% PARTIAL (operator+ overloads wall, shared header)
- get_weapon_lexeme_pair: 99.92% DONE (ammo_in_magazine this in eax vs ecx, call-boundary reg)
- get_user_hands_expression: 72.12% PARTIAL (inline wall, shared animation headers)
- new_object: 100% DONE

Differences for double-barreled:
- ctor adds `ASSERT_CMP_U( weapon.get_magazine_capacity(), ==, 2 )` and animations_count == 16
- m_weapon_animations is [2][2][3] (3 weapon states), 16 animations total
- get_weapon_lexeme_pair: 3 captions (show_both_barrels_empty / show_one_barrel_loaded /
  show_two_barrels_loaded), index = ammo_in_magazine() directly (not ==0)
- new_object: user_anim at animations[12]

## Target symbols (binaries/rich/target/index.jsonl)
- 0x79d9d0 ctor
- 0x79dba0 new_object
- 0x79dc70 get_user_hands_expression
- 0x79dd50 get_weapon_lexeme_pair
- 0x79de30 weapon_and_hands_expression

## Commands
- pdb_rich_query --index binaries/rich/target/index.jsonl --list --function double_barreled_weapon_core_show_state

## Build fix
- temp_include_all anchor calls private new_object -> added friend
  `use_game_core_double_barreled_weapon_core_show_state` to weapon_core_state_cook_template.h
  (fwd-decl + friend list).

## Log / Final results (report.json fuzzy_match_percent)
- ctor 0x79d9d0: 99.96% DONE. Sole diff: `weapon` for get_magazine_capacity( ) staged into
  eax (target) vs ecx (base) at 0x83. Call-boundary register choice. Unsteerable.
- new_object 0x79dba0: 100% DONE.
- get_weapon_lexeme_pair 0x79dd50: 99.93% DONE. Sole diff: inlined m_weapon.ammo_in_magazine( )
  reads [ecx+128h] into eax (target) vs ecx (base). Inline-result register choice. Same wall
  the matched pistol sibling accepts. Unsteerable.
- weapon_and_hands_expression 0x79de30: 83.52% PARTIAL. operator+ overload wall - shared
  mixing_addition_lexeme_inline.h lacks the expression-returning operator+ overloads the target
  uses; base emits operator+<expression,animation_lexeme> + operator+<addition_lexeme,...> +
  expression(addition_lexeme). Out of file scope; identical to pistol sibling (83.52%).
- get_user_hands_expression 0x79dc70: 63.63% PARTIAL. Inline wall - target keeps weapon_core::get_user
  and the three animation_lexeme_parameters setters out-of-line (frame 0x110); base inlines them
  (frame 0x128, direct member stores). Plus the type_sprint early-return ctor staged in eax (target)
  vs expression::expression<animation_lexeme>(edi,eax) (base). Both shared-header inline/overload
  decisions, out of file scope. Identical wall to pistol sibling (pistol 72.12%; ours slightly lower
  only because of relative instruction counts - structure is identical).

## Commands run
- pdb_fetch --view target --rva 0x79d9d0 / 0x79dba0 / 0x79dc70 / 0x79dd50 / 0x79de30
- pdb_fetch --view base  --rva 0x452fe0 / 0x453280 / 0x453380 / 0x453460 (+ ctor base 0x452fe0)
- diff target vs base asm (sed-stripped addresses) per function
- nix develop -c python3 scripts/rebuild.py (after cook-template friend fix)

## Walls summary
All 5 functions either 100%/near-100% DONE (call-boundary register choice) or PARTIAL on the
documented shared-animation-header inline/overload walls that also bound the matched pistol sibling.
No further source-shaping available within this file.
