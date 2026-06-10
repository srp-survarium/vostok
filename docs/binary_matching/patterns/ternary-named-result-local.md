# Ternary returned via a NAMED result local: extra slot + movss/movss/fld epilogue
tags: cpp:ternary cpp:return cpp:local cpp:float | asm:movss asm:fld | topic:pdb-locals
symptoms: movss xmm0 [result] movss [ebp-N] xmm0 fld [ebp-N] epilogue, extra stack slot
confidence: 8/10
variants: ternary-parens-precedence.md

A float-returning function ending `movss xmm0,[result_slot]; movss [ebp-N],xmm0;
fld dword ptr [ebp-N]` (an EXTRA slot + SSE round-trip) assigned the ternary to a named
local and returned it; a bare `return cond ? a : b;` emits only a plain `fld`. The local
is its own PDB statement - prefer it whenever the carcass shows the extra slot. const on
the local does NOT change the slot.

```cpp
float const r = cond ? a : b;
return r;
```
Evidence: get_hand_coefficient 95.54 -> 99.90.
