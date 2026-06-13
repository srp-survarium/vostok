# An /Od module unit can carry an OPTIMIZED LTCG COMDAT - correct source, unpairable bytes
tags: cpp:template cpp:member | asm:mov asm:movss | topic:convention topic:scoring-artifact
symptoms: no push ebp frame, this in eax, flat unrolled stores, xmm movq pairs, args in edi/esi
confidence: 7/10
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

Milder variant - dead-guard fold (same frame both sides): a VOSTOK_UNREFERENCED_PARAMETERS /
VOSTOK_EMPTY_EXPRESSION body is `if ( ::vostok::identity(false) ) { eater(...); } else (void)0`.
The optimized COMDAT keeps the `identity(false)` lvalue-materialization (mov byte[ebp-1],0; lea;
call empty_stub) AND the eater call, but FOLDS the dead branch test (drops movzx[eax]/test/je),
so the eater runs unconditional. Our /Od build keeps the full movzx/test/je guard - an unavoidable
~7-byte residual. Do NOT mistake the single leading empty_stub for a recoverable ASSERT: a real
ASSERT would add a SECOND empty_stub call; here there is exactly one (the identity call). Verified
systemic across every survarium::weapon_*_state::weapon_and_hands_expression / on_specific_event /
on_animation_end override (64-88%, none 100% unless the kept COMDAT was the /Od one). A by-reference
param (animation_lexeme&) promoted through the variadic eater adds a second /Od-only artifact: copied
by value (sub esp,84h; rep movsd) in /Od vs passed as a pointer in the optimized emission.
