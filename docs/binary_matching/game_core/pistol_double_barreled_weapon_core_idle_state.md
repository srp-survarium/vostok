# game_core::pistol_weapon_core_idle_state + double_barreled_weapon_core_idle_state (batched unit)

Two sibling weapon-state classes, near-copies of `weapon_core_idle_state` (matched
in PR #151). Matched as one unit (one branch/commit/PR). 8 functions total (4 each):
- ctor `(weapon_core&, managed_resource_ptr const*, u32)`
- `weapon_and_hands_expression(...) const`
- `get_weapon_lexeme_pair(...) const`
- `weapon_core_state_cook_template<T>::new_object(...)`

Branch off stack tip `docs/matching-macro-headers` (PR #152).

## Target RVAs
pistol: ctor 0x79baa0  new_object 0x79bbb0  get_weapon_lexeme_pair 0x79bc10  weapon_and_hands_expression 0x79bcd0
dbl:    ctor 0x79cc30  new_object 0x79cd50  get_weapon_lexeme_pair 0x79cdb0  weapon_and_hands_expression 0x79ce80

## Access chars (from target mangled names) - mirror reference (all PRIVATE)
- ctor `??0...@@AAE@`  -> private, non-const
- weapon_and_hands `...@@EBE...` -> private virtual, const
- get_weapon_lexeme_pair `...@@ABE...` -> private, const
- cook new_object `...@@AAE...` -> private, non-const

## Differences from the reference weapon_core_idle_state
- pistol m_weapon_animations is `[2][2][2]` (8 animations); class size 0x158.
- dbl    m_weapon_animations is `[2][2][3]` (12 animations); class size 0x168.
  Inner stride: pistol view*16 user_state*8 weapon_state*4; dbl view*0x18 user_state*0x0C ammo*4.
- ctor ASSERT_CMP_U count: pistol 8, dbl 12.
- get_weapon_lexeme_pair: per-class caption table + ammo-based index instead of a single literal.
  - pistol: captions {"pistol-idle","pistol-idle_empty"}; index = (ammo_in_magazine()==0)
    (asm: movzx ax; neg;sbb;add 1 -> 0 if ammo!=0, 1 if ammo==0).
  - dbl: captions {"idle_both_barrels_empty","idle_one_barrel_loaded","idle_two_barrels_loaded"};
    index = ammo_in_magazine() directly (asm: movzx ax; store).
- new_object: placement new with class-specific size (pistol 0x158, dbl 0x168) - same shape as ref.

## OPEN: double_barreled extra leading assert eaters
Both DBL ctor (srcline 21, before ASSERT_CMP_U at 22) and DBL get_weapon_lexeme_pair
(srcline 44, before the caption stores) have an EXTRA eater: `mov byte[slot],0; lea; call
identity(=finalize_impl)` with NO conditional/push-args and the identity result discarded.
Pistol has NO such leading eater. First guess: `ASSERT_U( UNKNOWN_EXPRESSION )`. To be
confirmed/iterated against `--view diff`.

## weapon_and_hands_expression - PARTIAL 85.65% (same as ref #151)
inline-vs-call of operator+<animation_lexeme,animation_lexeme> + ASSERT_U eater `push 0`
exactness. SIDE (verified against `--view diff` at rva 0x79bcd0): the TARGET keeps the
out-of-line `call operator+`; OUR BASE inlines it (addition_lexeme ctor + cloned_in_buffer
+ ~addition_lexeme). operator+<animation_lexeme,animation_lexeme> is a STANDALONE symbol in
BOTH rich indexes (target 0x0b42f0, base 0x08c9f0), so forcing the out-of-line call IS
source-steerable - a deliberate forward-decl device (cf. computed_*_animation_time_scale,
inline in a header yet forward-declared out-of-line in a .cpp to force the target's call).

### STRUCTURE-VERIFIER-v2 (batchA) - operator+ steer ATTEMPTED, REVERTED (85.65 -> 85.65, no effect)
Added an explicit-specialization DECLARATION in both idle_state .cpp, after the includes:
```cpp
namespace vostok { namespace animation { namespace mixing {
    template < >
    addition_lexeme& operator+< animation_lexeme, animation_lexeme >( animation_lexeme& left, animation_lexeme& right );
} } }
```
Rebuilt: NO change (still 85.65% both classes; report-changes shows no movement on either
weapon_and_hands). MSVC8 still inlines the primary inline template because its DEFINITION is in
scope (mixing_addition_lexeme.h includes _inline.h at its bottom); an explicit-specialization
*declaration* does not suppress inlining of an already-visible inline template at this call site.
The PROVEN forward-decl device (computed_*_animation_time_scale) only works because that function's
inline definition is NOT in the consuming TU - the .cpp forward-declares it without including the
inline header. To replicate here I would have to DROP `#include <vostok/animation/mixing_addition_lexeme.h>`
from the idle_state .cpp and instead forward-declare BOTH `operator+` and the `addition_lexeme`
type (the return is consumed by `animation::mixing::expression(...)`). That is a deep restructure
shared across 7 sibling .cpp (weapon_core_idle_state, pistol/dbl idle, pistol/dbl aimed_idle,
show_state...) in the OUT-OF-SCOPE animation module's header surface - collateral risk too high for
this pass. REVERTED both files to comment-only. Remains 85.65% PARTIAL: source-steerable IN PRINCIPLE
(a TU-include restructure) but the simple spec-decl device is ineffective. Do not bank as
"non-steerable LTCG"; it is "steerable only via an include restructure not yet done".

## Build #1 results (first approximation)
report.json fuzzy_match_percent (None = perfect, omitted by objdiff):
- pistol::ctor 100% , pistol::new_object 100% (BUT see below: initially folded/absent)
- pistol::get_weapon_lexeme_pair 92.52% , pistol::weapon_and_hands 85.65%
- dbl::ctor 100% , dbl::new_object 100%
- dbl::get_weapon_lexeme_pair 77.41% , dbl::weapon_and_hands 85.65%

### CRITICAL fix found: anchors not dispatched
Build #1's ctor/new_object did NOT appear on the base side (folded away) because I
defined `use_game_core_pistol_..` / `use_game_core_double_..` anchors but forgot to ADD
CALLS to them in the temp_include_all dispatcher (~line 1359). Added the two calls.

### get_weapon_lexeme_pair diff (both classes): ammo_in_magazine inline-vs-call
The whole-function register cascade + frame-size diff (pistol sub esp 0x20 target vs 0x24
base; dbl 0x28 vs 0x2c) ALL stem from a single divergence: the TARGET emits
`call ?ammo_in_magazine@weapon_core@survarium@@QBEGXZ`, but our base INLINED it
(`mov ax,[ecx+0x47a]; mov [ebp-..],ax; movzx`) because `weapon_core::ammo_in_magazine`
was defined in the class body (implicitly inline). The inlined temp grows the frame and
clobbers a different register, shifting eax/ecx/edx through the rest of the function.
FIX: declared `ammo_in_magazine` out-of-line (decl in weapon_core.h, body in
weapon_core.cpp). It is used ONLY by these two files, so the change is localized.
(The trailing `playback_enum` vs `playing_type_enum` in the impl-call mangling is a benign
typedef-name alias, same as the reference idle getter at 92%.)

### dbl leading assert eater (srcline 21 in ctor, srcline 44 in getter)
DBL (not pistol) has an extra leading eater `mov byte,0; lea; call empty_stub` with NO
movzx/test/je/push. Build #1 used `ASSERT_U(UNKNOWN_EXPRESSION)` which WRONGLY added a
`movzx;test;je;push 1;push 0;call;add esp` block (the `_U` eater body). Changed to plain
`ASSERT(UNKNOWN_EXPRESSION)` (= VOSTOK_EMPTY_EXPRESSION `if(identity(false)){}`) for build #2.

## Build #2 results
- pistol::ctor 95.56%, pistol::new_object 100% DONE, pistol::get_weapon_lexeme_pair 99.92%, pistol::weapon_and_hands 85.65%
- dbl::ctor 95.76%, dbl::new_object 100% DONE, dbl::get_weapon_lexeme_pair 99.92%, dbl::weapon_and_hands 85.65%
- ammo out-of-line fixed the getters (77/92 -> 99.92). DBL leading plain ASSERT matched its lone eaters.

### ctor residual = trailing ASSERT
BOTH ctors' ONLY remaining diff: a trailing lone eater `mov byte,0; lea; call empty_stub`
(NO branch) right before the final `mov eax,[ebp-..]` (return this). Pistol carcass line 30,
dbl line 32. Added `ASSERT( UNKNOWN_EXPRESSION )` after the loops. CONFIRMED plain (non-_U)
`ASSERT(UNKNOWN_EXPRESSION)` reproduces the lone-eater shape exactly (MSVC elides the
empty-`if` body's branch) - this is how build #2's DBL leading asserts already matched.

### getter residual (99.92%) = 1 register + 2 benign relocs
The only remaining getter diffs are:
1. `mov eax,[eax+0x128]` (target) vs `mov ecx,[eax+0x128]` (base) - the m_weapon-pointer load
   for the `m_weapon.ammo_in_magazine()` __thiscall this. A single register-allocation choice.
2. `movss xmm0,[s_aim_transition_time]` reloc + `get_weapon_lexeme_pair_impl` callee mangling
   (`playback_enum` vs `playing_type_enum` typedef alias). These TWO are IDENTICAL to the
   reference weapon_core_idle_state::get_weapon_lexeme_pair which is marked 100%|DONE (it has
   the exact same 2 ARG_MISMATCH relocs). So the getters are effectively matched; the 0.08%
   is the lone ammo-this register nit (LTCG-level reg alloc).

## Build #3 (FINAL) results - no regressions
- pistol::ctor 100% DONE, pistol::new_object 100% DONE
- pistol::get_weapon_lexeme_pair 99.92% DONE (ammo-this reg + 2 benign relocs)
- pistol::weapon_and_hands 85.65% PARTIAL (operator+ inline-vs-call: target out-of-line call, base inlines; source-steerable, not yet attempted)
- dbl::ctor 100% DONE, dbl::new_object 100% DONE
- dbl::get_weapon_lexeme_pair 99.92% DONE, dbl::weapon_and_hands 85.65% PARTIAL
report-changes.json: 0 regressed; ctors improved 95.5->100.

## Reusable learnings
1. A lone assert eater `mov byte,0; lea; call empty_stub` with NO movzx/test/je/push is a
   compiled-out NON-_U `ASSERT( ... )` (= VOSTOK_EMPTY_EXPRESSION `if(identity(false)){}`);
   MSVC /Od elides the empty-`if` branch, leaving just the guard call. Use
   `ASSERT( UNKNOWN_EXPRESSION )`. The _U forms (`ASSERT_U`/`ASSERT_CMP_U`) instead add the
   `movzx;test;je;push..;call expression_eater;add esp` body - do NOT use them for a lone eater.
2. A trivial accessor (`u16 ammo_in_magazine() const { return m_x; }`) defined in the class
   BODY is implicitly inline -> base inlines it at call sites while the target emits a `call`.
   When the target diff shows `call ?accessor@...` but base shows the inlined field load, move
   the accessor out-of-line (decl in header, body in .cpp). Here this single change took both
   getters 77/92% -> 99.92% by removing the inlined temp (frame shrank) and the cascading
   eax/ecx/edx register shuffle it caused.
3. temp_include_all anchors must be BOTH defined AND called from the dispatcher (~line 1359);
   defining the `use_..` fn without adding `vostok::use_..();` to the dispatcher leaves the
   target functions unreferenced -> folded away -> absent on the base side -> scored as 0 in
   report.json (shows as `fuzzy_match_percent: null` AND absent from the diff `right` side).

## Commands run
- pdb_rich_query --index binaries/rich/target/index.jsonl --list  (RVAs above)
- pdb_fetch --target-index binaries/rich/target/index.jsonl --rva <rva> --view target  (per-fn asm)
