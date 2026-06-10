# An /Od module unit can carry an OPTIMIZED LTCG COMDAT - correct source, unpairable bytes
tags: cpp:template cpp:member | asm:mov asm:movss | topic:convention topic:scoring-artifact
symptoms: no push ebp frame, this in eax, flat unrolled stores, xmm movq pairs, args in edi/esi
confidence: 8/10
variants: eax-this-convention.md, frameless-this-unused-leaf.md

A COMDAT instantiated from BOTH /Od and optimized TUs survives as whichever emission the
linker kept - sometimes the OPTIMIZED one (frameless, custom regs, unrolled), filed under
the /Od module's unit. Your /Od body emits framed per-statement code and scores
None/single digits NO MATTER WHAT. Write the source the gold LINE TABLE proves (the
optimized emission still carries statement lines in the PDB), mark PARTIAL citing the
emission, do not chase the %.

```asm
; tells: no `push ebp`; `this` in eax; 32 flat dword zero-stores; movq xmm pairs
```
Evidence: udp_match_stats::udp_match_stats() (0x62 bytes flat stores), udp_match_stats operator- (xmm pairs, args promoted to edi/esi; 10 statements at L212-224 in the PDB), udp_match_packet::header_size (8-byte mov/sub/sub/ret).
