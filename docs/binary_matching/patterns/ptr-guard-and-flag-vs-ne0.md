# A flag `bool f = p && (mask)` emits a branchy `mov dl,1/xor dl,dl`; `(mask)!=0` emits `setne`
tags: cpp:bool cpp:if | asm:cmp asm:je asm:setne | topic:codegen-idiom topic:structure-shape
symptoms: target conflict/flag bool built via cmp ptr,0 / je / mov dl,1 / jmp / xor dl,dl (branchy), base via setne al; an extra cmp ptr,<reg holding 0> the base lacks
confidence: 8/10

When a per-iteration flag guards a later deref, the original wrote a SHORT-CIRCUIT
`&&` whose left operand is a pointer NULL-check, e.g.
`bool f = p->member && ( p->member->field & q->field );`. The `&&` makes the compiler
materialize the bool through a BRANCH (`cmp p->member,0; je .false; ...; mov dl,1; jmp
.end; .false: xor dl,dl`), and the NULL-guard shows up as `cmp <ptr>, <reg-holding-0>`
where that reg is the same zero register reused for the later `= NULL` stores. Writing
the flag as a value-compare instead - `bool f = ( p->member->field & q->field ) != 0;` -
drops the guard and emits a clean `setne al`, byte-diverging and (worse) dereferencing a
member the original guarded. Match the `&&`: it both restores the bytes AND is the
faithful (NULL-safe) source.

```cpp
// target (branchy bool, NULL-guarded):
bool b_conflict = binding->m_action && ( binding->m_action->key_group & curr_pbinding->m_action->key_group );
```
```asm
mov edx,[ecx-14h]      ; binding->m_action
cmp edx,edi            ; edi == 0 (the held NULL, reused below for the = NULL stores)
je  short .false
mov ebx,[esi]          ; curr_pbinding->m_action
mov edx,[edx+8]        ; binding->m_action->key_group
test [ebx+8],edx       ; & curr->key_group
je  short .false
mov dl,1
jmp short .end
.false: xor dl,dl
.end:
```
The `!= 0` form instead is `mov eax,[ecx-14h]; mov eax,[eax+8]; mov esi,[edx]; test [esi+8],eax; setne al`.

Evidence: survarium::key_binder::bind_key 74.87 -> 100.0 (STRUCTURE MATCH, 0x1c1 bytes
both sides) by switching the unrolled conflict flag from `(...)!=0` to the `&&` guard.
