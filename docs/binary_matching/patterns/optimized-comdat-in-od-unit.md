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

Decisive tell - MAGIC-NUMBER DIVISION: /Od NEVER emits magic-multiply division, it emits
`idiv`. If the target divides a pointer-difference by sizeof (a container `.size()`) with
`imul 92492493h; sar 5` (signed /0x38) instead of `cdq; idiv`, the COMDAT is the OPTIMIZED
emission - even when it keeps a `push ebp` frame (the frame can survive while the inlined
callees come optimized). Companion tells: REFERENCE locals register-allocated (held in
edi/esi across statements) so the PDB records 0 named locals where your /Od base records the
references as stack slots, and a COLLAPSED line table (several source statements folded onto
one line entry). The /Od-faithful source is the READABLE multi-statement form (the optimizer
collapsed it); reproducing the optimizer's lower statement count via multiple-ops-per-line is
a fabricated structure - do NOT. The aggregate difference is available only through
`pdb_divergence --raw-line-table-counts`; it is not a closure criterion. Evidence:
survarium::scheduler::register_object (0x92cf0,
records.size() via imul magic, records/record refs in edi/esi, 4 collapsed line entries vs the
8-statement /Od readable form; sibling register_for_update is fully frameless FPO).

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
