# /Od frame slots are assigned by local NAME, not by declaration order
tags: cpp:local cpp:name | asm:ebp-disp8 asm:ebp-disp32 | topic:frame-layout
symptoms: same statement +3/-3 bytes across a function, [ebp-40h] vs [ebp-2A0h] for the same object, identical frame size
confidence: 8/10
variants: lone-byte-store-zero.md

MSVC 9 at /Od hands frame slots to named locals in an order that depends on the
identifiers themselves (a hash-table walk), not on where they are declared or first
used: `int a; int b; int c; int d;` and `int d; int c; int b; int a;` both put `a` at
[ebp-4] and `c` at [ebp-16]; `w,x,y,z` lands as w,y,x,z; `alpha,beta,gamma,delta` as
alpha,gamma,delta,beta (Wine probe scratchpad/probe_va/fl3.cpp). Objects in nested
scopes and compiler temporaries follow after the outer-scope names.

So when a function pairs statement-for-statement but every access to one object is
3 bytes off (disp8 vs disp32) and the frame size is identical, the object is not the
one retail's PDB names at that slot. Compare `pdb_topology --function F` locals
(`fp_off`) on both sides: same name set means same slots per name - the code uses the
wrong name for the role.

```asm
; retail                            ; ours
lea ecx, [ebp-40h]  ; foot_center_transform.identity()     lea ecx, [ebp-2A0h]  ; result.identity()
```
legs_ik_processor::get_foot_fixed_transform builds `foot_center_transform` (retail
slot -0x40) and returns `result = foot_to_center_rel * foot_center_transform`; ours had
the two names swapped. Retail names come from the PDB, so use them verbatim - a renamed
local moves every slot after it in hash order, not just its own.
