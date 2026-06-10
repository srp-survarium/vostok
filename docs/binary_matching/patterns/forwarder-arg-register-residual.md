# Thin forwarder: float/int arg in xmm0/eax vs spilled to stack = LTCG call-boundary residual
tags: cpp:member cpp:float | asm:movss asm:fld asm:fstp | topic:convention
symptoms: movss xmm0 [ebp+..] vs fld fstp [esp], sibling overloads 100% same shape
confidence: 8/10
variants: small-struct-factory-abi.md

A one-line forwarder `member_ref.method( other_member, a, b, c )` can match 100% for some
overloads and stall 60-80% for siblings with the SAME shape: the target passes a float in
xmm0 (register) where the base passes on the stack (`fld; fstp [esp]`), or which int arg
lands in eax differs. Whole-program LTCG calling-convention specialization dictated by the
callee - not steerable from the forwarder; the 100% siblings prove the source right. Stop
at PARTIAL and name the cause.

Evidence: game_core/legs_ik_drawer - draw_cross/draw_line_capsule 100%, draw_origin 62.88%, draw_solid_capsule 79.43%, draw_leg 73.36% (same xmm0-vs-fld/fstp residual x4; member_ref @this+0, scene @this+4 via add ecx,4).
