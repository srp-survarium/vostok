# Template param by-value vs const&: mangled V?$ vs ABV?$ + caller temp-destruction shape
tags: cpp:template cpp:const | asm:push asm:lea | topic:mangling topic:convention
symptoms: V?$ vs ABV?$, push ecx mov esi esp, add esp 4 no dtor, lea ecx call intrusive_ptr::dec
confidence: 9/10
variants: param-pointer-const-mangling.md

A by-value class param is constructed straight into the arg slot (`push ecx; mov esi,esp`)
and callee-destroyed (caller just `add esp,4`); a const& param materializes a frame local
the CALLER destroys (`lea ecx,[temp]; call intrusive_ptr::dec`). The mangled letters
(`V?$...` by value vs `ABV?$...` const&) gate COMDAT pairing - grep both rich indexes.

```cpp
template < typename P >
P static_cast_resource_ptr( src_ptr const& source );   // target was const& (ABV)
```
Engine-wide signatures: check cross-module fallout after such a fix.
Evidence: static_cast_resource_ptr header had by-value `const src_ptr`; const& fix paired all instantiations 0->100 and moved game_material_manager_cook::on_configs_loaded 90.20->98.53.
