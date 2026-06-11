# animation::mixing `+` chains: keep the idiomatic spelling; the residual is a header gap
tags: cpp:operator cpp:template | asm:call | topic:inline-vs-call topic:structure-shape
symptoms: operator+ addition_lexeme cloned_in_buffer, expression::is_empty, caps ~83-85%
confidence: 8/10
variants: float3-dot-operator-inline.md

`expression( a + b )`: operator+ builds an addition_lexeme temp, clones it
(cloned_in_buffer), destroys the temp; expression's implicit template ctor clones AGAIN.
Keep the idiomatic source - hand-expanding to `expression( *addition_lexeme(a,b).
cloned_in_buffer() )` scores LOWER (85.65 vs 80.30: extra ~expression temps + inlined
base_lexeme::cloned_in_buffer<> instead of the out-of-line addition_lexeme one). Whether
operator+ is inlined or called per site is LTCG (standalone in both indexes).

```cpp
return expression( main_lexeme + offset_lexeme );   // never hand-expand
```
The ~83-85% cap on weapon_and_hands_expression chains is a CROSS-UNIT HEADER GAP: the target's `operator+(expression&, T&) -> expression` overload family + a real `expression::is_empty()` do not exist in our headers (mixing_addition_lexeme_inline.h has only `operator+(T1&,T2&) -> addition_lexeme&`; is_empty is a `return false;` stub). No reshaping in the consumer can select missing overloads (expression(...) wraps made it WORSE: 83.52 -> 77.48/56.65). Fix = match the mixing operator+ family + is_empty as their own unit; mark consumers PARTIAL.
Evidence: weapon_core_idle_state::weapon_and_hands_expression (85.65%); weapon_core_reload_state::weapon_and_hands_expression.
