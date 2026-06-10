# objdiff None from differing delinker constant-pool symbol names (body byte-identical)
tags: cpp:float | asm:movss | topic:scoring-artifact topic:fold-icf
symptoms: structure-diff fully aligned 0 diffs yet scores None, same 0.0f reloc named differently per side
confidence: 8/10
variants: none-body-too-divergent.md, fold-misname-empty-fn.md

A byte-identical body can score None when each `movss xmm0,[0.0f]` reloc resolves to a
DIFFERENT delinker-assigned symbol for the same constant-pool slot (base
`out_of_range_reward` vs target `offset`) - the relocation compare bails. Non-steerable
delinker naming artifact; the `= 0.0f` source is correct.

Evidence: weapon_recoil_calculator::reset (7 stores to 0.0f, structure 7/7 aligned, None).
