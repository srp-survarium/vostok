# `delete p` on a polymorphic type = `push 1` + a virtual call to the `??_G` scalar deleting destructor
tags: cpp:dtor cpp:virtual cpp:new-delete | asm:call asm:mov asm:push | topic:codegen-idiom topic:fold-icf
symptoms: push 1 then a vtable-dispatched call (no ??1/operator delete at the site), callee is ??_G/??_E, slot loaded off [this]
confidence: 8/10
variants: explicit-dtor-vs-delete-flag.md, virtual-call-vtable-dispatch.md

When the static type's destructor is **virtual**, `delete p` does NOT emit a dtor call or
`operator delete` at the site. It pushes the deleting flag `1`, loads the vtable, and makes a
**virtual call** to the compiler-generated *scalar deleting destructor* (`??_G<Class>`, or
`??_E` for the vector form) — which internally runs the dtor and frees. So the site shows a
`push 1` + register-indirect call and nothing else. This is the `??_G`/`??_E` symbol you see
in vtables. Probe-verified:

```cpp
void del_poly( Poly* p ) { delete p; }   // Poly has a virtual ~Poly()
```
```asm
?del_poly@@YAXPAUPoly@@@Z PROC            ; (after the cmp $T,0 / je null-check)
  6a 01     push 1                        ; deleting-destructor flag = 1 (free)
  8b 55 fc  mov edx,$T
  8b 02     mov eax,[edx]                 ; vtable ptr
  8b 4d fc  mov ecx,$T                    ; this
  8b 10     mov edx,[eax]                 ; vtable[0] = ??_GPoly (scalar deleting dtor)
  ff d2     call edx                      ; virtual call; dtor + free happen INSIDE
```
Steerable: a `push 1` before a vtable-dispatched call with no site-level `??1`/`operator delete` = `delete p` through a virtual dtor (the callee is `??_G`); don't expect a direct dtor call. A non-virtual dtor instead inlines the dtor + guarded free (explicit-dtor-vs-delete-flag.md).
