# A reference parameter and a pointer parameter generate IDENTICAL bodies — only the mangle differs
tags: cpp:member cpp:const | asm:mov | topic:mangling topic:wall
symptoms: T& and T* produce the same load-slot-then-deref body, AAH vs PAH (AAV/PAV) in the mangled name is the sole difference
confidence: 9/10
variants: param-pointer-const-mangling.md, call-convention-cdecl-vs-thiscall.md

A `T&` parameter is passed and used exactly like a `T*`: the slot holds an address, every
use dereferences it. The ONLY signal of reference-vs-pointer is the **mangled name** —
`AAH` (`A` ref) vs `PAH` (`P` ptr) for `int` (`AAV..`/`PAV..` for class types). A by-value
parameter, by contrast, works directly on its slot with no indirection. Probe-verified:
`byref` and `byptr` bodies are byte-identical.

```cpp
void byref( int& r ) { r = 5; g = r; }   // ?byref@@YAXAAH@Z
void byptr( int* p ) { *p = 5; g = *p; } // ?byptr@@YAXPAH@Z
void byval( int v )  { v = 5; g = v;  }  // ?byval@@YAXH@Z
```
```asm
?byref@@YAXAAH@Z PROC                     ?byptr@@YAXPAH@Z PROC
  8b 45 08 mov eax,[r]                      8b 45 08 mov eax,[p]
  c7 00 05.. mov DWORD PTR [eax],5          c7 00 05.. mov DWORD PTR [eax],5
  8b 4d 08 mov ecx,[r]                      8b 4d 08 mov ecx,[p]
  8b 11    mov edx,[ecx]                     8b 11    mov edx,[ecx]
  89 15 .. mov g,edx                         89 15 .. mov g,edx
;  ^ identical body                          ^ identical body (AAH vs PAH is the ONLY diff)
; byval: operates on [v$ebp] directly -> mov DWORD PTR _v$[ebp],5  (no indirection)
```
Wall (body) / steerable (signature): never decide ref-vs-ptr from the body — read the `A` vs `P` in the mangled name. Top-level const adds the const letter and gates pairing (`QAV` vs `PAV`) — param-pointer-const-mangling.md.
