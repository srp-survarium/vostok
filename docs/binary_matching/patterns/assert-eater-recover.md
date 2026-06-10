# TRGT_ONLY 0xc/0x12 row = compiled-out ASSERT - recover it
tags: cpp:assert | asm:mov asm:lea asm:call | topic:assert-eater
symptoms: TRGT_ONLY 0xc, TRGT_ONLY 0x12, mov byte ptr [ebp-N],0, lea eax, call empty_stub, finalize_impl, call_constructor_helper
confidence: 10/10
variants: assert-u-eater.md, unreferenced-params-eater.md, unused-bool-false-store.md, assert-misname-scored-matched.md, speculative-assert-inline-ctor.md

Master Gold does NOT delete asserts: every non-evaluating assert (`ASSERT`/`R_ASSERT`/`FATAL`
= `VOSTOK_EMPTY_EXPRESSION` = `if(::vostok::identity(false)){}else(void)0`) leaves a lone
eater that bumps the frame and shifts the `[ebp-N]` slots. A TRGT_ONLY structure row of
exactly 0xc (or 0x12) between otherwise-matching statements is a missing assert, not a
stripped/LTCG call.

```cpp
ASSERT( UNKNOWN_EXPRESSION );
```
```asm
mov byte ptr [ebp-N], 0    ; the false bool
lea eax, [ebp-N]           ; identity(false), ref in eax (promoted convention)
call empty_stub            ; folded-empty; delinker misnames: finalize_impl / call_constructor_helper / ...
; plain ASSERT: NOTHING follows (no movzx/test/je, no arg pushes)
; size 0xc = 4+3+5 (slot within disp8); 0x12 = 7+6+5 (slot past -0x80, disp32) - same construct
```
Steerable house style; never use a `_U` form for a lone eater (adds test/je + pushes).
Evidence: booby_trap_core.cpp/inventory_cook.cpp carcasses; booby_trap_core::serialize 60.3->77.2 + deserialize 15.6->18.1; get_target_koef/get_broken_hands_penalty corrections; pistol_weapon_core_fire_state::weapon_and_hands_expression (the 0x12 row); double_barreled idle ctor (leading lone ASSERT + ASSERT_CMP_U pair, only the second tested).
