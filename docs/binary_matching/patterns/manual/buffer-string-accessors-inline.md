# `buffer_string` accessors inline to 1-3 ptr ops: `c_str()`=`mov eax,[eax]`, `length()`=`m_end-m_begin`
tags: cpp:string cpp:inline | asm:mov asm:sub asm:ret | topic:codegen-idiom topic:fold-icf
symptoms: mov eax,[eax]; ret (c_str, this in EAX), mov eax,[ecx+4]; sub eax,[ecx]; ret (length), COMDAT-folded under a foreign symbol name
confidence: 8/10

The 3-pointer `buffer_string` layout (`m_begin`/`m_end`/`m_max_end`) makes its accessors trivial
and they almost always inline at the call site: `c_str() const` returns `m_begin` at offset 0
(`mov eax,[eax]; ret` — note `this` arrives in **EAX** for this leaf, not ECX); `length() const`
is `m_end - m_begin` (`mov eax,[ecx+4]; sub eax,[ecx]; ret`). The rare out-of-line copy is
ICF-folded and may report under an unrelated symbol name (a scoring artifact).

```cpp
inline pcstr name ( ) const { return m_name.c_str( ); }
```
```asm
; buffer_string::c_str() const          ; buffer_string::length() const
mov  eax, [eax]    ; m_begin (this=EAX)   mov  eax, [ecx+4]   ; m_end
ret                                       sub  eax, [ecx]     ; - m_begin
                                          ret
```
Wall-ish: when these inline the member offsets bake in — a wrong offset is a wrong layout, not a fixable spelling. `shared_string::c_str()` differs (null-check then `add eax,10h` past the profile header) and folds under an `intrusive_ptr<...>::c_ptr` name. cite: game_material.h:25 `m_name.c_str()`; buffer_string_inline.h:307 (length), :331 (c_str).
