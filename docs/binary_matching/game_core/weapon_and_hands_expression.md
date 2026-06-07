# weapon_and_hands_expression (pistol / double_barreled idle / aimed_idle / show variants)

Covers the 5 `*_weapon_core_*_state::weapon_and_hands_expression` overloads in the
pistol/double_barreled idle, aimed_idle (4x identical shape) and pistol show (1x) files.

## Outcome
- 4 idle/aimed_idle variants: report.json 85.65%, PARTIAL.
- pistol show variant: report.json 83.52%, PARTIAL.
STRUCTURE MATCH (the statement skeleton reproduces the target); the residual is a
whole-program LTCG inline decision in the shared animation mixing header - non-steerable
from these `.cpp` files.

## The wall (idle / aimed_idle, 85.65%)
Body is `ASSERT_U; get_weapon_lexeme_pair; return expression( main + offset );`. The
structure-diff has two SIZE diffs:
1. ASSERT_U (0x36 target vs 0x38 base) - the assert-eater shape differs slightly.
2. the `return expression( main + offset )` (0x59 target vs 0x38 base).

`--view diff` confirms: the target INLINES `operator+<animation_lexeme,animation_lexeme>`
(the addition_lexeme ctor + cloned_in_buffer) into the return, needing extra stack
temporaries -> frame `sub esp, 170h` vs base `158h`. Base keeps the out-of-line
`call operator+` (`+ lea ecx; + call; + mov ecx,eax` extra lines in the diff). operator+
is a STANDALONE symbol in BOTH rich indexes, so whether it is inlined here is a
whole-program decision, not a source bug. Recovering it would require changing the shared
mixing header's inline policy - out of this file's scope.

## The wall (pistol show, 83.52%)
Body is `get_weapon_lexeme_pair; get_user_hands_expression; return hands_expression + main + offset;`.
Sole SIZE diff is the chained `+` return (0x7a target vs 0x53 base). The target uses dedicated
expression-returning operator+ overloads (`expression operator+( expression&, animation_lexeme& )`,
`operator+( expression&, expression& )`) that are NOT declared in the current shared
mixing_addition_lexeme_inline.h - only the generic `addition_lexeme& operator+(T1&,T2&)` exists.
So base emits `operator+<expression,animation_lexeme>` + `operator+<addition_lexeme,animation_lexeme>`
+ `expression(addition_lexeme)` where the target inlines the expression overloads. Adding those
overloads to the shared header is out of this file's scope and would shift other operator+ matches.

Same class of residual across all variants and the reference
weapon_core_idle_state::weapon_and_hands_expression (#151).
