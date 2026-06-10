# Small-struct factory: eax:edx register-pair return / promoted convention vs base sret (wall)
tags: cpp:return cpp:cast | asm:call asm:mov asm:push | topic:convention topic:fold-icf
symptoms: mov [tmp] eax mov [tmp+4] edx copy chain, lea eax [esp+10Ch] sret, callee body is one ret byte, const_buffer::const_buffer bogus name
confidence: 8/10

`T r = factory( args );` for a small (<=8B) POD (resources::request): the target returns
in the eax:edx pair then copies into the local (or LTCG promotes the callee to a custom
write-through-pointer convention, ICF-folding the promoted body onto a byte-identical
two-store ctor and leaving the original mangled symbol as a dead 1-byte `ret` stub); the
base lowers the same source as hidden-sret/RVO writing directly into `r`. Call-boundary
return-ABI under LTCG - not steerable; `__declspec(noinline)` does not reproduce it.

```cpp
resources::request r = resources::create_request( name, class_id );
```
```asm
; base:   push 0Fh; lea eax,[esp+10Ch]; call create_request                      ; sret/RVO
; target: push 0Fh; push edx; call create_request; add esp,8
;         mov [tmp],eax; mov [tmp+4],edx; mov [r],eax; mov [r+4],ecx             ; pair + copy
```
The extra temp/copy enlarges the frame so ONE root cause shows as many SIZE/quantity rows downstream - diagnose one site, attribute the cascade, mark PARTIAL.
Evidence: game_material_manager_cook::create_game_material_pairs (57.81%, every create_request site).
