# Compiler-generated dtor epilogue: missing member-dtor `this` setup is ICF folding
tags: cpp:dtor | asm:add asm:call | topic:fold-icf
symptoms: target add ecx 0xNN before each member ~T() call, base omits the add ecx setups
confidence: 6/10
variants: virtual-dtor-explicit-call.md

A destructor where the target sets `mov ecx,[this]; add ecx,0xNN` before each trivial
member `~T()` call but the base omits those setups (still issuing the calls) is
ICF/codegen folding the identical member-dtor `this` adjustments - not source-steerable.
The only real source is the explicit body (e.g. `DELETE(m_drawer)`); the member-dtor
epilogue is auto-emitted.

```asm
mov ecx,[this]; add ecx,0xNN     ; target only - this-setup for a folded member ~T()
call <member dtor>
```
Evidence: game_core/legs_ik_processor::~legs_ik_processor (85.71%).
