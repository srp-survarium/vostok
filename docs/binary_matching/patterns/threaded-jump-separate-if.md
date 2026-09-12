# A jump recorded on the NEXT if's line is a threaded jump, not an else
tags: cpp:if cpp:else | asm:jmp asm:cmp | topic:control-flow topic:line-records
symptoms: jmp+cmp+je in one record on the second if's line, then-statement record without the jmp; our else-if puts the jmp with the then-statement
confidence: 9/10
variants: else-split-line.md

At /Ox, after `x = 2;` the optimiser knows `x == -1` is false and threads a `jmp` past
the following check. That jump belongs to the second `if` statement, so retail records
`jmp; cmp; je` together on that if's line and the then-statement keeps only its store.
Our `else if` produced the same bytes but attributed the jump to the then-statement -
and neither braces, a bare `else` line, nor line geometry moves it. Write two ifs.

```cpp
if ( s_debug_draw_sensor && ( group & 0x81 ) )
	color_idx = 2;

if ( color_idx == -1 )
	continue;
```
```asm
mov eax, 2            ; line 237 (5 bytes)
jmp short .5          ; line 239: threaded past the check
cmp eax, 0FFFFFFFFh   ;
je  short .6          ; (7 bytes together)
```
Evidence: bullet_physics_world::debug_draw_world (SPLIT -> MATCH 100, build 40).
