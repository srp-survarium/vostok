# Inline accessor leaves a +0xc call-result temp; direct member access does not
tags: cpp:inline cpp:member | asm:mov | topic:inline-vs-call
symptoms: mov ecx [member] mov [ebp-TEMP] ecx mov eax [ebp-TEMP], +0xc per accessor use
confidence: 8/10
variants: inline-vs-call-trivial-getter.md, per-call-ref-return-spill.md

At /Od (/Ob0) an accessor is a real CALL whose result lands in a temp; LTCG inlines the
call but the temp survives (+0xc per use). A direct member read folds inline with NO
temp. If the accessor header is OURS, make the member protected and access it directly -
the target provably did.

```cpp
m_skeleton   // direct protected access, NOT get_skeleton()
```
```asm
; accessor: mov ecx,[member]; mov [ebp-TEMP],ecx; mov eax,[ebp-TEMP]    ; vs direct: flows straight into the consumer
```
TELL for arg lists: /Od schedules CALLS before the push sequence (temp); a direct member read evaluates IN the push sequence.
Evidence: legs_ik_processor get_skeleton() -> m_skeleton (process 92.60->98.96, process_leg +3, get_foot_fixed_transform +4).
