# A 2-byte `jmp short $+2` (to the literal next instruction) can be KEPT by the target and DROPPED by the base
tags: cpp:switch | asm:jmp | topic:scoring-artifact topic:convention
symptoms: 1-row TRGT_ONLY <0x2>, jump target is the next instruction, last case break before the join
confidence: 6/10
variants: folded-case-break-dead-jmp.md, closing-brace-jmp-rows.md

A trailing `break;` whose jump target is the literal next instruction (last case of a
switch falling into the join) survived as a real 2-byte `jmp short $+2` in the target
while our /Od build emitted nothing - a 1-row `TRGT_ONLY <0x2>` residual that resists
source steering. Bank it; do not reshape the switch for 2 bytes.

Evidence: on_user_name_answer_received's default-case break (login_client_impl_sign_in.cpp).
