# A qualified `obj->Base::f()` devirtualizes to a direct `UAE` thiscall (vs vtable dispatch)
tags: cpp:virtual cpp:call | asm:mov asm:call | topic:codegen-idiom
symptoms: direct call ?f@...@UAE.. with no vtable deref when qualified, vs register-indirect call after a double-deref of this when not
confidence: 8/10
variants: virtual-call-through-vtable.md, mangled-access-virtual.md, call-convention-cdecl-vs-thiscall.md

A normal virtual call dispatches through the vtable (the established
virtual-call-through-vtable.md: load `[this]` -> vtable, load the slot, `call <reg>`, no
named callee; slot displacement / 4 = the index). The matching-relevant twist:
**qualifying the call with the class name suppresses dispatch**. `obj->Base::f()` emits a
*direct* thiscall to `?f@Base@@UAE..` — the callee keeps its `UAE` virtual-member mangling
(mangled-access-virtual.md) but is reached by a plain `call <symbol>`, no vtable load.
Probe-verified, side by side:

```cpp
void call_virt          ( B* b ) { g = b->f( 7 ); }       // dispatch
void call_nonvirt_explicit( B* b ) { g = b->B::f( 7 ); }  // qualified -> direct
```
```asm
?call_virt@@YAXPAUB@@@Z PROC                  ; VTABLE DISPATCH
  6a 07     push 7
  8b 45 08  mov eax,[b]
  8b 10     mov edx,[eax]      ; edx = vtable ptr (at object offset 0)
  8b 4d 08  mov ecx,[b]        ; this
  8b 02     mov eax,[edx]      ; eax = vtable[0]  (slot 0; a later slot = [edx+N])
  ff d0     call eax           ; register-indirect, NO named callee

?call_nonvirt_explicit@@YAXPAUB@@@Z PROC      ; DEVIRTUALIZED
  6a 07     push 7
  8b 4d 08  mov ecx,[b]        ; this
  e8 ..     call ?f@B@@UAEHH@Z ; DIRECT call, UAE mangling, no vtable load
```
Steerable: a `call <reg>` after a double-deref of `this` = a virtual call (read the slot off the displacement); a direct `call ?..@UAE..` = the source qualified the call (or holds a statically-known type) — reproduce the qualification. Taking `&Base::f` instead emits a thunk (addressof-virtual-anchoring-trap.md).
