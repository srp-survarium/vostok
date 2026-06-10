# A fat early-`return;` row carries the inlined dtor of an in-scope local/by-value param
tags: cpp:return cpp:dtor | asm:jmp asm:call asm:cmp | topic:structure-shape topic:pdb-locals
symptoms: ~0x26 return row, sp_counted_base::release on the return line, dtor walk then jmp epilogue
confidence: 9/10
variants: for-head-statement-merges.md, closing-brace-jmp-rows.md

Before any non-trivial local exists, `return;` is a bare 5-byte jmp; once an
alloca-backed container or a by-value shared_ptr-holding param (callee-destroyed) is
live, the SAME `return;` row carries the inlined destruction attributed to the return's
line. Two identical ~0x26 rows = two early returns, NOT codegen layout.

```asm
; buffer_vector in scope: walk begin..end by elem size; end = begin; jmp epilogue   (~0x26)
; by-value resolver iterator: lea ecx,[ebp+14h]; cmp [ecx],0; je .skip; call sp_counted_base::release; jmp
```
Evidence: udp_network_flow_emulator::tick (14-vs-15 misaligned -> 15/15, 48.44 -> 60.55); async_connector::on_resolved (both error-arm returns).
