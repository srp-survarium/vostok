# `reader.r<float>()` = integer load to a stack slot, then `movss` reload (GPR has the bytes, FPU wants them)
tags: cpp:template cpp:member cpp:float | asm:mov asm:movss | topic:codegen-idiom topic:wire-io
symptoms: mov edx,[ptr]; mov [esp],edx; movss xmm0,[esp] for a 4-byte float read, this in ECX (vs EDX for r<u32>)
confidence: 8/10

`r<float>()` shares the integer body (the read is byte-identical to `r<u32>`: load m_pointer, deref
into a GPR, advance). To RETURN it as a float it stores the GPR to a 1-slot stack temp and reloads
with `movss` — a GPR->XMM bounce through memory, not a direct float load. The standalone `this`
arrives in ECX here (vs EDX for r<u32>/r<bool>): the per-instantiation register convention is
irregular.

```cpp
float yaw = reader.r< float >( );
```
```asm
mov   eax, [ecx+4]   ; m_pointer (this in ECX)
mov   edx, [eax]     ; deref into a GPR
add   eax, 4
mov   [esp], edx     ; spill to stack temp
movss xmm0, [esp]    ; reload as float for return
mov   [ecx+4], eax
```
Wall: the bounce is forced by an int-shaped read returning a float; do NOT "fix" it to a direct movss-from-m_pointer. cite: packet_reader_inline.h:39; target packet_reader::r<float>.
