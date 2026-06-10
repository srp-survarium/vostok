# Private LTCG-inlined helper has NO base symbol: None / "not in base index" = STATE[INLINED]
tags: cpp:member cpp:inline | topic:scoring-artifact topic:inline-vs-call
symptoms: not found in BASE index, None, exists out-of-line in TARGET only, private AAE helper
confidence: 6/10
variants: none-constant-pool-names.md, single-tu-anchor-inlines-helper.md

Under /Od+/GL the linker can inline a private helper whole-program into its only callers,
so no standalone base symbol survives to pair even though the target keeps one. Not
separately scorable: mark STATE[INLINED] and match its body as a callee of the caller.

Evidence: weapon_recoil_calculator process_compensation / get_random_angle / get_random_amount (inlined into tick/fire).
