# ladder::add_landing_point

target rva 0x586470 | base rva 0x45b8e0
report.json fuzzy_match_percent: 99.76923

## Verdict
STRUCTURE MATCH (shape ok). Single statement both sides, no quantity or size diffs.
Sole residual is a stack-frame-size difference, non-steerable (LTCG).

## Body
```
void ladder::add_landing_point( landing_point* const new_point )
{
    m_landing_points.push_back( new_point );
}
```

## structure-diff (condensed)
```
target: 0x586470            base: 0x45b8e0
; void survarium::ladder::add_landing_point(survarium::landing_point* const) ; target 1 stmts / base 1 stmts
.. same ..
; aligned 1, size-diffs 0, quantity-diffs 0
```

## asm diff (the residual)
```
~ 0x03: sub esp, 38h                 -> sub esp, 0Ch
~ 0x06: mov [ebp-38h], ecx           -> mov [ebp-0Ch], ecx
m_landing_points.push_back( new_point );
  0x09: push 0
  0x0b: mov eax, [ebp+8]
  0x0e: push eax
~ 0x0f: mov ecx, [ebp-38h]           -> mov ecx, [ebp-0Ch]
  0x12: add ecx, 128h
~ 0x18: call 000000A1h               -> call 00000085h
```

The ONLY divergence is the prologue `sub esp, 38h` (target) vs `sub esp, 0Ch` (base),
which then renames the `this` slot (`[ebp-38h]` vs `[ebp-0Ch]`) and shifts the
relative `call` displacement to push_back. Everything else (push 0, push new_point,
`add ecx, 128h` to reach m_landing_points @0x128, the call) is byte-identical.

## Cause
The body is a single delegation to `m_landing_points.push_back( new_point )` (the
container at this+0x128). The target reserves a 0x38-byte frame, our base only 0x0c.
The extra reservation is space the target keeps for push_back's inlined temporaries
under whole-program LTCG; our base lowers the same call with a different (smaller)
reservation. There is no missing local (structure-diff = 1/1, identical statement
set) and no `empty_stub`/ASSERT (no zeroed-byte-local + call sequence). The frame
size here is a callee-inline/whole-program LTCG artifact at the call boundary, not a
source-steerable cause - same class as the inline-vs-call entries in
assembly_patterns.md. Match the body, mark DONE at 99.77%.
