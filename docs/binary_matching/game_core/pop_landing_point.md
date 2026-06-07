# ladder::pop_landing_point

target rva 0x586450 | base rva 0x45b8c0
report.json fuzzy_match_percent: 99.7

## Verdict
STRUCTURE MATCH (shape ok). Single statement both sides, no quantity or size diffs.
Sole residual is a stack-frame-size difference, non-steerable (LTCG).

## Body
```
landing_point* ladder::pop_landing_point( )
{
    return m_landing_points.pop_front( );
}
```

## structure-diff (condensed)
```
target: 0x586450            base: 0x45b8c0
; survarium::landing_point* survarium::ladder::pop_landing_point() ; target 1 stmts / base 1 stmts
.. same ..
; aligned 1, size-diffs 0, quantity-diffs 0
```

## asm diff (the residual)
```
~ 0x03: sub esp, 44h                 -> sub esp, 1Ch
~ 0x06: mov [ebp-44h], ecx           -> mov [ebp-1Ch], ecx
return m_landing_points.pop_front( );
~ 0x09: mov ecx, [ebp-44h]           -> mov ecx, [ebp-1Ch]
  0x0c: add ecx, 128h
~ 0x12: call 0000007Fh               -> call 00000063h
```

Same class as add_landing_point: ONLY the prologue `sub esp, 44h` (target) vs
`sub esp, 1Ch` (base), the renamed `this` slot, and the relative call displacement
to pop_front differ. The `add ecx, 128h` (m_landing_points @0x128) and the call are
identical.

## Cause
Single delegation to `m_landing_points.pop_front()` (container at this+0x128). The
target reserves a 0x44-byte frame, base 0x1c - extra space for pop_front's inlined
temporaries under whole-program LTCG. No missing local (1/1 statements) and no
ASSERT (no empty_stub sequence). Callee-inline/whole-program LTCG frame artifact at
the call boundary, not source-steerable. Match the body, mark DONE at 99.70%.
