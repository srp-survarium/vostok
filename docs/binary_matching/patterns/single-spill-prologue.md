# Single-`this`-spill prologue: `push ecx` (4-byte frame) vs `sub esp,0Ch`
tags: cpp:member | asm:push asm:sub-esp | topic:convention
symptoms: push ecx prologue vs sub esp 0Ch, this at [ebp-4] vs [ebp-0Ch], trailing alignment nops
confidence: 6/10
variants: od-frame-slot-noise.md

A small method with NO declared locals (only `this` spilled): identical instruction
stream apart from the prologue - target `push ecx` vs base `sub esp,0Ch`. MSVC's
single-slot `push reg` frame vs `sub esp,N` is a prologue/frame-allocation quirk not
expressible in source. Non-steerable; mark DONE/PARTIAL with the residual noted.

Evidence: damage_model::on_broken_limb_affect (98.95%).
