# Init + double-compare in ONE statement = assignment-in-condition `if ( ( x = m ) == A || x > B )`
tags: cpp:if cpp:local | asm:mov asm:cmp asm:jcc | topic:structure-shape topic:pdb-locals
symptoms: one record with member load + temp store + two compares, no user local recorded, slot order swapped
confidence: 8/10
variants: switch-2value-cluster.md

A target statement merging a member load into a temp AND two compares of that temp
(`mov ...; mov [tmp]; cmp [tmp],K1; je; cmp [tmp],K2; jle`) where a `T const x = m;` +
`if (x == A || x > B)` splits into two statements: declare the local uninitialized and
assign INSIDE the condition.

```cpp
T x;
if ( ( x = m ) == A || x > B )
```
Caveat: the no-user-local slot order smells like a switch, but 4-5 dense enum values lower to a JUMP TABLE on MSVC8 (booby_trap_core::switch_to_state) - the if-shape is the right reproduction here.
Evidence: booby_trap_core::on_state_timer_finished (99.67, 4/4 stmts, residual slot swap only; the switch experiment dropped it to 67).
