# `cmp K1; je THEN / cmp K2; jle ELSE` fall-through = `x == K1 || x > K2` (disjoint, NOT >=)
tags: cpp:if cpp:enum | asm:cmp asm:je asm:jle | topic:condition-shape
symptoms: two compares against different constants, je-to-then, jle/jg-to-else, fall-through to then, an apparently-dead `> K2` clause
confidence: 7/10

Two sequential compares with a fall-through encode a DISJOINT condition, not a single range.
`cmp x,K1; je THEN; cmp x,K2; jle ELSE; (fall through) THEN` is `if ( x == K1 || x > K2 )` -
the then-set is `{K1}` plus `{> K2}`. It is NOT `x >= K1` (that is ONE compare) and NOT
`x == K1 || x >= K2` (the `jle` puts K2 itself in the ELSE branch, so the source clause is the
STRICT `> K2`). The `> K2` clause can look dead when no enum value exceeds K2 - it is faithful
defensive source, keep it; dropping it changes the branch structure.

```cpp
if ( m_trap_state == booby_trap_state_armed || m_trap_state > booby_trap_state_disarmed )
```
```asm
cmp dword [x],1; je  .THEN      ; x == K1 (armed)
cmp dword [x],3; jle .ELSE       ; x <= K2 -> else (so K2/disarmed is in ELSE)
(fall through) .THEN             ; x > K2 -> then
```
Evidence: `booby_trap_core::on_state_timer_finished` (#372).
