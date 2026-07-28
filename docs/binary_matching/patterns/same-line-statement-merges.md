# Same-line merges: ONE line-table statement covers everything on the line (size-sum tell)
tags: cpp:local cpp:assert cpp:if | asm:jmp | topic:pdb-locals topic:structure-shape
symptoms: TRGT_ONLY/BASE_ONLY pair whose sizes ADD UP exactly to the other side's single row
confidence: 9/10
variants: string-array-brace-init-lines.md, for-head-statement-merges.md, nested-if-vs-multiline-and.md

The line table emits ONE statement per source LINE, so the original layout is recoverable
from exact size sums:
- `else if ( cond ) call;` on ONE line = ONE statement (0xbd = 0x64 test + 0x59 call).
- `float3 start, finish;` = ONE statement of 2*0xb; two `math::color a(..), b(..);` on
  one line = ONE of 2*0x19.
- `ASSERT( ... ); <statement>;` on ONE line = the eater's 0xc + the statement's bytes in
  one record (booby_trap_set_core dtor: eater + begin(), 6 -> 5 stmts;
  weapon_core_shotgun_reload_start_substate::finalize: eater + reset(), 3 -> 2;
  weapon_core_fire_state_base initialize/finalize + aimed sibling: eater 0xc +
  `*m_is_firing_ptr = true/false` 0xc/0xf = one 0x18/0x1b record, 5 -> 4 / 7 -> 6 stmts).

```cpp
ASSERT( m_x ); m_damage_parameters.begin( );   // one line = one record
```
Evidence: legs_ik process (23-vs-24 from a two-line else-if), get_foot_fixed_transform; the array-brace-init entry is the inverse direction.
