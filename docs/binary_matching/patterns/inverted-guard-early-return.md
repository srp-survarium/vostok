# A lone 2-byte TRGT_ONLY branch row after a guard = inverted guard + early `return;`
tags: cpp:if cpp:return | asm:jmp asm:jcc | topic:structure-shape
symptoms: TRGT_ONLY 0x2 bare branch with its own line, between guard test and body
confidence: 9/10
variants: early-returns-vs-else-join.md, flat-ifs-vs-else-if.md

Under /Od every line-table row maps to a SOURCE statement - codegen cannot add rows. An
extra TARGET row that is a tiny (2-byte) bare branch with its OWN line between a guard's
test and the guarded body is a real `return;`: the original spelled the guard INVERTED
(`if (!cond) return; body;` lowers as `jne .body; jmp .end`), while `if (cond) body;`
folds into a single `je .end` - one row fewer. Generalizes to any bare-branch ONLY row
with its own line (return/break/continue/else are all source statements).

```cpp
if ( !connected )
	return;          // the 2-byte row: a standalone jmp .end on its own line
disconnect( );
```
Evidence: tcp_packet_client::~tcp_packet_client (banked as a wall, then 3/3 stmts, 90.98 -> 94.88 via PR #291); same shape closed on_packet_received (17+1 -> 18/18) and on_packet_has_been_sent (9+1 -> 10/10).
