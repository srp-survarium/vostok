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

## STATEMENT-COUNT recovery: `result = result + get_X(...)` is a lexical-scope block with a NAMED temp
When `--view structure-diff` shows a `result = result + get_X(...)` accumulate site with TWO
TRGT_ONLY statements clinging to it (target N+2 stmts) AND the target `--view info` lists an
extra `expression` local in `scope: 1` per such site, the original source did NOT inline the
call into the `+`. It declared a block-local named `expression` and added it on its own line:
```cpp
if ( cond )
{
    animation::mixing::expression  expression  = get_X( ... );  // line A: temp at [ebp-T], constructed here
    result = result + expression;                               // line B: operator+ -> ~intrusive_ptr of operator+ temp
}                                                               // line C: ~intrusive_ptr of [ebp-T] temp
```
The two TRGT_ONLY are line B's operator+ temp destructor and line C's block-local temp destructor.
Recovering the named `expression` local + the two-statement spelling collapses BOTH TRGT_ONLY
AND adds the missing scope-1 local (locals-are-structure): player_logic_stand_state /
player_logic_crouch_state::look_expression went 17->23 stmts / 10->13 locals (3 recoil blocks x2
recovered stmts). The remaining per-site `SIZE +0x43` is the operator+ inline-vs-call residual
above (banked).
