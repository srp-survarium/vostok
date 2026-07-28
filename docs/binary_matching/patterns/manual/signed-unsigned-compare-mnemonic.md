# Signed vs unsigned comparison: the branch mnemonic IS the operand signedness (jl/jge vs jb/jae)
tags: cpp:if cpp:for | asm:cmp asm:jcc | topic:codegen-idiom topic:mangling
symptoms: jl/jge/jg/jle (signed) vs jb/jae/ja/jbe (unsigned) after a cmp, same source `<`/`>` shape
confidence: 9/10

The same `<`/`>` source picks its conditional-jump by operand TYPE: signed -> `jl`/`jge`/`jg`/`jle`
(plus a bare `test r,r; jg/js` for `>0`); unsigned -> `jb`/`jae`/`ja`/`jbe`. So a `u32` counted
loop exits on `jae`/`jb`; an `int` clamp uses `jg`/`jle`. The mnemonic is a one-byte opcode
difference and is the cheapest signedness tell in the diff.

```cpp
math::clamp( x, 0, dimension-1 );   // int  x  -> signed jumps
... months > 1 ...                  // u32     -> unsigned jumps
```
```asm
; signed int clamp                  ; unsigned compare
test  eax, eax / jg  short ..        cmp   eax, 1
cmp   eax, esi / jle short ..        jbe   short ..   ; jbe = unsigned <=
```
Steerable: pick the type (int vs u32) to flip `jg`/`jle` <-> `ja`/`jbe`; if the target branches `jb`/`jae` your operand must be unsigned. cite: terrain get_row_col terrain_collision_utils_inline.h:106-112 (signed); core day_count build_extensions.cpp (unsigned).
