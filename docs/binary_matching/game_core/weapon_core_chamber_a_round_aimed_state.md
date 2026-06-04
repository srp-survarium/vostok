# weapon_core_chamber_a_round_aimed_state (concrete subclass)

Unit: `sources/vostok/game_core/sources/weapon_core_chamber_a_round_aimed_state.cpp`
Base: `origin/int/game_core`. Mirrors the matched `weapon_core_chamber_a_round_state`
(non-aimed) sibling exactly - same 5-function shape, same two cross-cutting walls.

Five functions (target access chars from the target `.obj`):
- ctor `??0...@@IAE` (protected, non-const)        -> 99.87% effective 100% (DONE)
- `new_object` cook `@@AAE` (private)              -> 99.84% (computed_* LTCG residual)
- `get_weapon_lexeme_pair` `@@ABE` (private const) -> 99.80% effective 100% (DONE)
- `weapon_and_hands_expression` `@@EBE` (private virtual const) -> 83.18% PARTIAL (wall)
- `get_user_hands_expression` `@@ABE` (private const)          -> 73.86% PARTIAL (wall)

Unit measure (report.json): fuzzy 90.63%, 3/5 matched - identical to the sibling.

## Resume / the bug that blocked pairing
Prior worker had marked ctor/get_weapon_lexeme_pair/new_object `100%|DONE` and the
two walls `STUB`, but the header declared the ctor + all three helpers under `public:`
WITHOUT the `friend` declarations the sibling uses. Result: every function compiled
with the WRONG access char and objdiff could not pair them against the target:

    obj-string access chars (`strings ...cpp.obj | grep -o '...@survarium@@[A-Z]*'`):
      function                     target   base(public hdr)
      ctor                         IAE      QAE      <- protected vs public
      get_weapon_lexeme_pair       ABE      QBE      <- private  vs public
      get_user_hands_expression    ABE      QBE      <- private  vs public
      weapon_and_hands_expression  EBE      UBE      <- private  vs public

So the whole unit reported only 8.4% / 1-of-5 (only `new_object`, a separate COMDAT,
happened to pair). FIX: rewrite the header to the sibling's layout - ctor `protected:`,
the three helpers `private:`, and add the two friends so the temp_include_all anchor
and the cook template can reach them:
    template < typename T > friend class weapon_core_state_cook_template;
    friend void ::vostok::use_game_core_weapon_core_chamber_a_round_aimed_state( );
Plus the `namespace vostok { void use_game_core_..._aimed_state( ); }` fwd-decl above
the class. After the fix + a `touch`-the-cpp rebuild: unit -> 90.63%, 3/5, no regressions.

LESSON: ACCESS-SPECIFIER FIRST is checked at the obj-symbol level. A public-vs-private
mismatch is not a low fuzzy %, it is `target_symbol: None` (no pairing at all) and a
`None` per-function measure - read the mangled access char from the target `.obj` and
match it before anything else.

## ctor (target 0x799ed0 / base 0x450390) - 99.87%, effective DONE
ASSERT_CMP_U(animations_count,==,8); two nested view/user_state double-loops sharing one
`animation_index` ([ebp-4]) into m_weapon_animations[2][2] (148h) then m_user_animations
(158h) via intrusive_ptr::operator=; trailing ASSERT(UNKNOWN_EXPRESSION). Mnemonic stream
is byte-identical to target; the only diff objdiff sees is how the delinker renders two
relocation operands (the `vector constructor iterator` element-ctor fn-ptr push, and the
`dummy::nonnull` vs `finalize_impl` ASSERT helper name). Call-boundary/relocation residual.

## get_weapon_lexeme_pair (target 0x79a1a0 / base 0x450670) - 99.80%, effective DONE
identifier "weapon-chamber_a_round_aimed"; selected = m_weapon_animations[is_third_view!=0]
[user_state==type_crouch]; set_animation_to_wait(selected) BEFORE the impl call; then
get_weapon_lexeme_pair_impl(buffer, id, selected, &m_weapon, m_animation_playback_state, 8,
m_animation_timescale, play_once_and_freeze_at_end, linear_interpolator(s_aim_transition_time));
trailing ASSERT (dummy::nonnull). Verified the full mnemonic stream is IDENTICAL to the
target (diff of the `op` column = "IDENTICAL MNEMONIC STREAM"); residual is operand-name
rendering only.

## new_object (cook COMDAT) - 99.84%
`return new (buffer.c_ptr()) weapon_core_chamber_a_round_aimed_state(params->weapon, 1.0f,
animations, animations_count);` Residual is the known `computed_*` / 1.0f-constant LTCG
artifact noted across the cook-template family.

## weapon_and_hands_expression (target 0x79a240) - 83.18% PARTIAL - WALL
Source: lexeme_pair = get_weapon_lexeme_pair(...); hands = get_user_hands_expression(...);
return hands + lexeme_pair.main_lexeme + lexeme_pair.offset_lexeme;
NO extra sprint/jump branch (checked the target asm - identical to the non-aimed sibling).
Target resolves the `+` chain through the EXPRESSION-returning operator+ family
(expression::expression<animation_lexeme>, operator+<animation_lexeme>(expression&,..),
operator+(expression&,expression&)) which is NOT declared in our shared
`mixing_addition_lexeme` headers; the generic addition_lexeme operator+ resolves a
different chain. Recovering needs that operator+ family added to the shared API - a
separate cross-cutting unit, out of this unit's scope. Same wall as the sibling (83.18%).

## get_user_hands_expression (target 0x79a0a0) - 73.86% PARTIAL - WALL
Structure matches the carcass. Target keeps animation_lexeme_parameters setters
(animated_object/bones_mask/playback_type) and the parameters dtor as OUT-OF-LINE calls;
our /Od build inlines the trivial in-class setters and the dtor down to its ASSERT.
Whole-program inline decision, not steerable from this file. Same wall as the sibling.

## Commands run
- objdiff-cli diff -p binaries/objdiff -u <unit> -o /tmp/aimed_diff.json --format json   (per-symbol match_percent + target_symbol pairing)
- strings binaries/objdiff/{base,target}/.../weapon_core_chamber_a_round_aimed_state.cpp.obj | grep -o '...@survarium@@[A-Z]*'   (access-char diagnosis)
- pdb_fetch --target-index ... --rva 0x799ed0/0x79a1a0/0x79a240 --view target
- pdb_fetch --base-index ... --rva 0x450390/0x450670 --view base
- diff of the mnemonic columns of get_weapon_lexeme_pair base vs target => identical
- python3 scripts/rebuild.py (NO module arg)  => unit 90.63%, 3/5, 0 regressed

## Regressions
report-changes.json after the fix: 0 regressed, 4 improved, 0 removed, 0 added.
