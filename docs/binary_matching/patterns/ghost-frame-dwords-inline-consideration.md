# Ghost frame dwords scaling +4 per call site = LTCG inline-consideration temps, not a missing local
tags: cpp:ctor cpp:local | asm:sub-esp | topic:convention
symptoms: sub esp delta is the ONLY diff, zero PDB locals on either side, +4 per call of the same callee, this home-slot rename
confidence: 6/10
variants: od-frame-slot-noise.md

When the base-vs-target diff is ONLY `sub esp, N` (+4 per call of one specific callee)
plus the `this` home-slot rename that follows - ZERO PDB-recorded locals on EITHER side,
every other instruction identical, and the callee itself 100% - the extra dwords are
compiler temps that link-time codegen reserved while CONSIDERING the callee for inlining.
Their count depends on the whole-program caller/callee context, not on this unit's
source: don't burn rebuilds restructuring init lists for it; revisit once the real
callers are matched.

```asm
sub esp, 14h     ; base
sub esp, 18h     ; target: +4 for the one strings::duplicate call in this ctor
```
Evidence: string_order's three ctors, +4 per strings::duplicate call (0x14->0x18, 0x1C->0x24, 0x24->0x30); callee strings::duplicate<base_allocator> itself 100%; real login/match callers were still stubs. Banked 99.70/99.75/99.77 PARTIAL.
