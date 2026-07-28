# `(this->*pmf)(args)` (single inheritance) = load pmf from its slot, `mov ecx,this`, `call eax` — not a vtable dispatch
tags: cpp:member cpp:call | asm:call asm:mov | topic:codegen-idiom
symptoms: (obj->*pmf)(...), mov edx,[obj+N]; mov eax,[edx+slot]; mov ecx,this; call eax, args pushed first
confidence: 9/10

A pointer-to-member-function call for a single-inheritance class is NOT a vtable dispatch: push
the args, load the pmf VALUE from its member slot (a plain code address for single inheritance),
set `ecx = this` (thiscall), `call` the register. The member-offset of the chosen pmf in its
holder struct selects which one. (boost::bind ultimately calls through such a pmf, but the direct
`(this->*pmf)()` form is this shape.)

```cpp
return ( this->*m_function_pointers->m_generator )( allocator, left );   // m_generator at +0x10
```
```asm
mov   eax, [ebp-8]       ; this
mov   edx, [eax+8]       ; -> m_function_pointers
mov   eax, [edx+10h]     ; the pmf value (m_generator)
mov   ecx, [ebp-8]       ; ecx = this  (thiscall)
call  eax
```
Steerable: the pmf-slot offset + thiscall `ecx` setup follow from the struct layout; single inheritance means a bare code address (no this-adjust). cite: ai base_lexeme::generate_permutations base_lexeme.cpp:94.
