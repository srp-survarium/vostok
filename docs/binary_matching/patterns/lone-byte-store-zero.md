# Lone 4-byte `mov byte[ebp-N],0` with no lea/call - a bool member of a local object
tags: cpp:bool cpp:member cpp:inline | asm:mov | topic:codegen-idiom topic:frame-layout
symptoms: <0x4> statement, mov byte ptr [ebp-N],0 with no lea/call, no PDB local for the slot
confidence: 8/10
variants: assert-eater-recover.md, bool-const-local-codegen.md

A statement of size <0x4> whose only instruction is a byte-store of zero, with no
following `lea eax; call <eater>` and NO PDB local at that slot. It is not an unused
`bool b = false;` (the PDB would list it). Resolve the slot against the PDB locals'
`fp_off` ranges instead: `[ebp-N]` falls INSIDE a class-typed local, and N minus that
local's start is a member offset. The statement is an inlined bool setter with no
assert (`inline X& flag( bool const v ) { m_flag = v; return *this; }`), called on that
object with the returned reference discarded.

```asm
mov byte ptr [ebp-5], 0    ; offset_lexeme_parameters lives at [ebp-58h]; 0x58-5 = 0x53 = m_can_generate_events
```
Resolved site: game_core get_weapon_lexeme_pair_impl line 40 =
`offset_lexeme_parameters.can_generate_events( false );` (the class layout in
`binaries/structure/target/headers` or the reconstructed header gives the member).
Check `pdb_topology --function F` for the locals' `fp_off` before calling any such byte a temp.
