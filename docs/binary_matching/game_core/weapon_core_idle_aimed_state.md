# game_core::weapon_core_idle_state + weapon_core_aimed_state (batched unit)

Two sibling weapon-state classes, byte-for-byte identical in shape. Matched as one
unit (one branch/commit/PR). 8 functions total (4 each):
- ctor `(weapon_core&, managed_resource_ptr const*, u32)`
- `weapon_and_hands_expression(...) const`
- `get_weapon_lexeme_pair(...) const`
- `weapon_core_state_cook_template<T>::new_object(...)`

Branch off stack tip `chore/game_core-delinker-reconciliation-and-score` (PR #150).

## Target RVAs
idle:  ctor 0x79d1f0  new_object 0x79d2d0  get_weapon_lexeme_pair 0x79d330  weapon_and_hands_expression 0x79d3c0
aimed: ctor 0x79cf50  new_object 0x79d030  get_weapon_lexeme_pair 0x79d090  weapon_and_hands_expression 0x79d120

## Key facts recovered from the target
- The two classes differ ONLY in: the auto vtable symbols (class name), and the
  string literal in get_weapon_lexeme_pair: idle "weapon-idle", aimed "weapon-aimed_idle".
- s_aim_transition_time = 0.3f (file static, both classes; from .rdata @ obj offset).
- weapon_state_creation_params::weapon is at +4 (struct, weapon_core&).
- m_weapon_animations[2][2] is at this+0x138; managed_resource_ptr = 4 bytes (row stride 8).

### ctor (lines 21-27)
```
ASSERT_CMP_U( animations_count, ==, 4 );          // line 21: identity(false) + expression_eater(count,4,0)
u32 animation_index = 0;                          // line 22 (0x62: [ebp-4]=0)
for ( u32 view = 0 ; view < 2 ; ++view )          // line 23 (block 1, [ebp-8])
    for ( u32 user_state = 0 ; user_state < 2 ; ++user_state )  // line 24 (block 2, [ebp-0Ch])
        m_weapon_animations[view][user_state] = animations[animation_index++];  // line 25
```
The 0x3f..0x5f block: `mov byte[ebp-0Dh],0; lea eax; call identity` (=identity(false)) then
`je; push 0; push 4; push count; call expression_eater; add esp,0Ch` = ASSERT_CMP_U(count,==,4)
which expands to `if (identity(false)) { expression_eater(count, 4, assert_untyped); }`.
The two trailing +0x002 (lines 26,27) are the two for-loop back-edge `}` jmps.

### get_weapon_lexeme_pair (lines 39-41)
```
pcstr animation_identifier = "weapon-idle";       // line 39
resources::managed_resource_ptr const& selected_animation =
    m_weapon_animations[is_third_view][user_state_id == type_crouch];  // line 40
return get_weapon_lexeme_pair_impl(               // line 41
    buffer, animation_identifier, selected_animation, &m_weapon,
    m_animation_playback_state, u32(-1), 1.0f, play_cyclically,
    linear_interpolator( s_aim_transition_time ) );
```
Indexing: byte[ebp+10h]=is_third_view normalized (neg/sbb/neg) -> *8 first index;
cmp [ebp+14h],1 / sete -> *4 second index (user_state_id == type_crouch(1)).
impl pushes (R->L): &interpolator, 0(play_cyclically), 1.0f(fld1/fstp), -1,
&m_animation_playback_state(@0x120), m_weapon(@0x128), selected_animation, identifier, buffer, retptr.

### weapon_and_hands_expression (lines 32-34)
```
ASSERT( UNKNOWN_EXPRESSION_T( weight_driving_animation ) );  // line 32 (GUESS - see iterations)
weapon_lexeme_pair lexeme_pair = get_weapon_lexeme_pair( buffer, is_third_view, user_state_id );  // line 33
return animation::mixing::expression( lexeme_pair.main_lexeme + lexeme_pair.offset_lexeme );  // line 34
```
Line 34: addition_lexeme(main,offset).cloned_in_buffer() then expression(that) -> exactly
`operator+(main,offset)` wrapped in `expression(...)`. Confirmed by the asm:
addition_lexeme<animation_lexeme,animation_lexeme> ctor, cloned_in_buffer, ~addition_lexeme,
expression::expression<addition_lexeme>, then ~animation_lexeme x2 (lexeme_pair members).
Line 32 ASSERT: 0x11 mov byte/lea/call identity(false); branch copies weight_driving_animation
(0x84 bytes, rep movsd 0x21) into a call to the folded-empty eater, cleanup exactly 0x84.

### new_object (line 53, single statement)
```
return new ( buffer ) weapon_core_idle_state( params->weapon, animations, animations_count );
```
asm: lea &buffer; call <placement op new misnamed operator*>; push; push 0x148; call operator new;
null-check; ctor(params->weapon @[params+4], animations, animations_count); return.
get_weapon_lexeme_pair_impl is a free fn STUB in weapon_lexeme_pair.cpp - only need it to link
(declare + provide a stub body), not matched here.

## Commands run (verbatim, in order)
- git checkout -b match/game_core-weapon_core_idle_aimed_state   (off chore/game_core-delinker-reconciliation-and-score)
- pdb_rich_query --index binaries/rich/target/index.jsonl --function weapon_core_idle_state --list
- pdb_rich_query --index binaries/rich/target/index.jsonl --function weapon_core_aimed_state --list
- pdb_fetch --target-index ... --rva 0x79d1f0/0x79d2d0/0x79d330/0x79d3c0 --view target   (idle)
- pdb_fetch --target-index ... --rva 0x79cf50/0x79d090 --view target                       (aimed)
- COFF-parse target objs for s_aim_transition_time (0.3f) + strings (weapon-idle / weapon-aimed_idle)

## Iterations

### Build 1 (after operator-new fix: new(buffer.c_ptr()))
report.json (both idle & aimed identical):
- ctor                          98.21%
- weapon_and_hands_expression   27.41%
- get_weapon_lexeme_pair        93.48%
- new_object                    100.00%  DONE
- get_weapon_lexeme_pair_impl   0% (STUB, not a target of this unit)

Diffs found (base vs target):
- ctor: only diff is loop exit `jae` (base, `view < 2`) vs `je` (target, `view != 2`).
  -> change loops to `view != 2` / `user_state != 2`.
- get_weapon_lexeme_pair: only diff is the target's `neg;sbb;neg` (bool `!= 0` normalize)
  on the first array index; base `m_weapon_animations[is_third_view]` does just `movzx`.
  -> `m_weapon_animations[is_third_view != false]`.
- weapon_and_hands_expression: (a) line 32 ASSERT - target copies the lexeme (sub 0x84,
  rep movsd, call eater) under if(identity(false)); my ASSERT(UNKNOWN_EXPRESSION_T) emitted
  no copy. -> ASSERT_U( weight_driving_animation ). (b) line 34 - target INLINES operator+
  (addition_lexeme ctor + cloned_in_buffer + ~addition_lexeme); my `main + offset` emitted a
  `call operator+<...>`. -> write the operator+ expansion inline:
  `*addition_lexeme( main, offset ).cloned_in_buffer( )`.

### Build 2 (all four fixes applied)
report.json (both idle & aimed identical):
- ctor                          100.0%  DONE
- weapon_and_hands_expression   80.30%
- get_weapon_lexeme_pair        100.0%  DONE
- new_object                    100.0%  DONE
All four fixes worked. ctor `!= 2` loops -> 100; get_weapon_lexeme_pair `is_third_view != false`
-> 100. weapon_and_hands_expression up 27->80 (ASSERT_U gave the lexeme copy; the
`*addition_lexeme(...).cloned_in_buffer()` form for line 34 introduced extra ~expression temps
and an inlined base_lexeme::cloned_in_buffer<> instead of the target's out-of-line
addition_lexeme::cloned_in_buffer()).

### Build 3 (line 34 reverted to the idiomatic `main + offset`)
- weapon_and_hands_expression   85.65%  (idle & aimed)
`main + offset` (operator+) is the cleaner/original form and scores higher than the manual
.cloned_in_buffer() expansion. Diff base vs target now shows exactly two residuals:
1. line 32 ASSERT: base `...rep movsd; push 0; call eater; add 0x88` vs target
   `...rep movsd; call eater; add 0x84`. ASSERT_U passes (assert_untyped, lexeme); target's
   eater got ONLY the lexeme. No standard ASSERT macro emits a single-arg expression_eater;
   ASSERT_U is the closest (one extra `push 0`). The full lexeme-copy block IS matched.
2. line 34: base `call operator+<animation_lexeme,animation_lexeme>` (out-of-line) vs target
   inlining operator+ at this call site (addition_lexeme ctor + cloned_in_buffer + ~addition_lexeme
   expanded inline). operator+ is STANDALONE in BOTH rich indexes (target 0xb42f0, base 0x8b590),
   so this is the documented per-call-site whole-program LTCG inline-vs-call class (operator| /
   fixed_string entries in assembly_patterns.md) - NOT steerable from this caller's source.

## Outcome
STATE: 7 of 8 functions 100% DONE; weapon_and_hands_expression (both classes) 85.65% PARTIAL.
- ctor (idle & aimed)                       100% DONE
- get_weapon_lexeme_pair (idle & aimed)     100% DONE
- new_object (idle & aimed)                 100% DONE
- weapon_and_hands_expression (idle & aimed) 85.65% PARTIAL - residual = LTCG per-call-site
  operator+ inline-vs-call (dominant) + ASSERT_U eater's assert_untyped push.
Regressions caused: none real. report-changes shows ICF/COMDAT-folding churn only (vector
deleting destructors, float3::float3, empty_stub, resource_ptr ops) - every 100->0 has a
matching 0->100 fold-representative bounce, none in files I authored; this is the expected
relink folding noise (PR #150 reconciliation territory).
Access specifiers (objdiff pairs by mangled name): ctor AAE / new_object AAE (private),
get_weapon_lexeme_pair ABE (private const), weapon_and_hands_expression EBE (private virtual
const) - set all to `private:` in the headers; new_object moved to `private:` in the cook
template. Anchors `use_game_core_weapon_core_{idle,aimed}_state` befriended on both the state
class and the cook template; new_object reached via a qualified call on a fabricated null cook
pointer (constructing a cook would emit its vtable and force codegen of still-STUB cook virtuals).
get_weapon_lexeme_pair_impl: provided as a STUB body in weapon_lexeme_pair.cpp so the callers
link/score; the impl itself is out of scope for this unit (stays STUB, scores 0).
