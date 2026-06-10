# Per-call ref-return spill = uniform `[ebp-N]` shift, not a brace bug
tags: cpp:member | asm:mov asm:push | topic:inline-vs-call topic:pdb-locals
symptoms: uniform slot deltas all the SAME constant, zero TRGT_ONLY/BASE_ONLY rows, frame +4 per spilled site
confidence: 8/10
variants: accessor-call-result-temp.md, od-frame-slot-noise.md

When a ref-returning accessor is passed repeatedly (`f(*ptr, accessor(), ...)`) and the
BASE spills it into a fresh stack temp per call while the TARGET inlines the deref
(`mov ecx,[this]; mov edx,[ecx]; push edx`), the frame grows 4 bytes per site and EVERY
later `[ebp-N]` shifts by the total. Distinguish from a genuine brace-scope storm: zero
target-only/base-only rows + constant slot deltas = temp-spill/LTCG artifact, not a
missing brace.

Evidence: game_core/legs_ik_processor::process.
