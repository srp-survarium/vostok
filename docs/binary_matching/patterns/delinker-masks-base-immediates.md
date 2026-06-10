# Delinker masks BASE address-range immediates but keeps the target's (unmatchable operands)
tags: cpp:ctor | asm:mov | topic:tooling topic:scoring-artifact
symptoms: mov ecx imm32 zeroed in delinked base obj only, % stuck with structure 0 diffs
confidence: 5/10
variants: none-constant-pool-names.md

A `mov ecx, imm32` whose value falls in (or near) the image address range is treated as a
maybe-relocation: masked (zeroed) on the BASE side, kept on the TARGET side - permanent
operand mismatch even when the source constant is byte-correct. Verified the delinker,
not codegen: ANY seed literal (0xDEADBEEF, 0x12345) survives the ninja obj but vanishes
from the delinked base obj; no DIR32 reloc exists. The rich/structure view masks operands
too (scores IDENTICAL) - only the operand-aware objdiff % exposes the gap. Write the
source emitting the right ctor/call SHAPE (explicit `m_x( seed )` -> the random32(u32)
ctor) with the target literal, but do NOT expect the % to close.

Evidence: udp_network_flow_emulator ctor (87.59%) - the three random32(0x995a34/35/36) seeds.
