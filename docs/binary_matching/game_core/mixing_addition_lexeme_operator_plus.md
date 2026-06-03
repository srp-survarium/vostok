# mixing operator+ (expression-returning) + expression::is_empty

Unit: the animation::mixing expression-returning `operator+` overloads + the real
`expression::is_empty()` body. These live in animation headers but are compiled into
game_core (the weapon-state family instantiates them). Goal: unblock the
weapon_and_hands_expression family (capped ~83%).

## Target functions (rich/target)

mixing_addition_lexeme_inline.h:
- 0x099150  `template<T> expression operator+(expression&, T&)`  (T=animation_lexeme)  L41-46
- 0x0ac540  `expression operator+(expression&, expression const&)`  L81-90
- 0x0b4180  `expression operator+(expression const&, expression const&)`  L93-103
- 0x0bce60  `expression operator+(expression&, expression&)`  L58-66

mixing_expression_inline.h:
- 0x098ad0  `bool expression::is_empty() const`  L47-49

The existing `template<T1,T2> operator+(T1&,T2&) -> addition_lexeme&` (L33-37) stays;
it serves lexeme+lexeme. The NEW expression-returning overloads serve expr+expr / expr+lexeme.

## Recovered shapes

is_empty (0x098ad0):
```
cmp [eax],0   ; m_node @0
je  true
cmp [eax+4],0 ; m_lexeme @4
je  true
xor eax,eax   ; return false
true: mov eax,1
```
=> `if ( m_node && m_lexeme ) return false; return true;` (m_node intrusive_ptr @0, m_lexeme* @4)

operator+(expression&, expression&) (0x0bce60):
```
if ( left.is_empty() ) return right;       // call is_empty; copy-ctor right -> result
if ( right.is_empty() ) return left;
return expression( *addition_lexeme(left,right).cloned_in_buffer() );  // tmp lexeme, cloned, expression ctor, ~tmp
```

operator+(expression&, expression const&) (0x0ac540): const right copied to a local
`expression` first (addition_lexeme ctor needs non-const T&), then same as above.

operator+(const&, const&) (0x0b4180): BOTH copied to locals first.

template<T> operator+(expression&, T&) (0x099150): is_empty INLINED on left (cmp[ecx],0..),
`if(left.is_empty()) return right;` (expression<T> ctor), then the addition_lexeme path.
No `if(right.is_empty())` because right is a lexeme, not an expression.

## Source written

mixing_addition_lexeme.h: declared the 4 new overloads after the addition_lexeme& template.
mixing_addition_lexeme_inline.h: defined them (lines positioned at target L41/58/81/93 opens).
mixing_expression.h: is_empty stub `{return false;}` -> declaration only.
mixing_expression_inline.h: defined out-of-line is_empty.

## Commands / results

(below, appended as iterated)

## Results (after rebuild, report.json top-level fuzzy)

- operator+(expression const&, expression const&)  0x0b4180  -> 45.98% PARTIAL
- operator+<animation_lexeme>(expression&, animation_lexeme&)  0x099150 -> 12.09% PARTIAL
- operator+(expression&, expression&)  0x0bce60  -> None (no base ODR-use; caller STUB)
- operator+(expression&, expression const&)  0x0ac540 -> None (no base ODR-use; caller STUB)
- expression::is_empty()  0x098ad0 -> None (body byte-correct/20B but inlined whole-program,
  no standalone base symbol -> unpaired)

## Why PARTIAL (proven wall)

Two non-source-steerable causes (see assembly_patterns.md "mixing expression operator+"):
1. Build-config divergence: target compiles operator+<animation_lexeme> with FULL optimization
   (FPO, reg alloc, deep inlining) while base is /Od. Confirmed by the diff: target has no ebp
   frame, sub esp,24h, ebx/edi/esi, is_empty as cmp[ecx],0.
2. inline-vs-call LTCG x3 on the /Od-both overloads: target keeps expression::is_empty,
   addition_lexeme::cloned_in_buffer, ~addition_lexeme as out-of-line calls; our /GL inlines all
   three (byte temps; add ecx,1Ch -> base_lexeme::cloned_in_buffer; explicit ~expression dtors).
   is_empty has ZERO standalone symbol in base obj (strings|grep empty).

The structure (carcass srcline comments) matches the target exactly for the /Od-both overload.

## Caller impact

weapon_core_idle_state::weapon_and_hands_expression (and idle twins) UNCHANGED at 85.65% - they
use lexeme+lexeme (-> addition_lexeme&), not these new expression-returning overloads. The reload/
fire/aimed weapon_and_hands_expression bodies that WOULD select these are still STUB (None) - their
implementation is a separate unit; this unit only supplies the overloads + is_empty they need to
compile/resolve. No caller regressed.

## Commands

nix develop -c pdb_rich_query --index binaries/rich/target/index.jsonl --function "operator+" --list
nix develop -c pdb_fetch --target-index ... --rva 0x0bce60/0x0ac540/0x0b4180/0x099150/0x098ad0 --view target,structure
touch sources/vostok/game_core/sources/weapon_core_idle_state.cpp ; nix develop -c python3 scripts/rebuild.py
nix develop -c pdb_fetch ... --rva 0x0b4180 --view diff   (showed the x3 inline-vs-call residual)
strings binaries/objdiff/base/.../mixing_expression_inline.h.obj | grep is_empty  -> empty (proves inlined)
