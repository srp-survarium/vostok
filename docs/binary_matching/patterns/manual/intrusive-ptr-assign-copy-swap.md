# `ptr = other;` (intrusive_ptr operator=) = copy-and-swap: two `call ::dec` + one interlocked_increment + slot swap, `ret 4`
tags: cpp:operator cpp:template | asm:call asm:mov | topic:codegen-idiom topic:inline-vs-call
symptoms: mov [ebp-4],0 / cmp ecx,[eax] / call intrusive_ptr<...>::dec (x2) / call interlocked_increment / mov [edx],eax / ret 4
confidence: 8/10

`intrusive_ptr::operator=` is `self_type(object).swap(*this); return *this;`. /Od inlines the
temp's `set` (equal-guard, dec, store, increment) AND the swap as plain `mov` slot-juggling, but
leaves **two `call ...::dec`** (the temp ctor's inlined dec, then the temp dtor's dec) plus one
`interlocked_increment`. Net: a ~0x7c body ending `ret 4`. The dec/c_ptr callee names are
ICF-folded — ignore them.

```cpp
m_model = model;          // both render::static_model_ptr
```
```asm
mov   dword ptr [ebp-4], 0          ; temp.m_object = 0
cmp   ecx, [eax]                    ; set guard: equal(object)?
call  intrusive_ptr<...>::dec       ; temp.set -> dec(old)
call  vostok::threading::interlocked_increment   ; inc new
mov   [edx], eax                    ; swap stores temp.m_object into *this
call  intrusive_ptr<...>::dec       ; ~temp dec
ret   4
```
Steerable: keep the idiomatic `lhs = rhs;` — the two-`dec` + one-`increment` skeleton is forced by copy-and-swap; do NOT hand-roll a manual inc/dec. cite: weapon.cpp:110 `m_model = model;`, weapon.cpp:252 `animation = m_idle;`; symbol intrusive_ptr<...weapon_ammunition...>::operator=.
