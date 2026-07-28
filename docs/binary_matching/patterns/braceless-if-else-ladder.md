# Nested brace-less if/else ladder: each 2-byte `jmp` carries the bare `else` line record
tags: cpp:if | asm:jmp | topic:structure-shape
symptoms: +0x002 stmt carrying the else keyword line, consecutive source lines no room for } lines, chained 2-byte jmps
confidence: 8/10
variants: else-block-double-jmp.md, closing-brace-jmp-rows.md

`if (a) if (b) x; else y; else z;` (dangling-else binds inner; outer else is legal)
compiles to two chained 2-byte jmps: then-of-inner jumps to a label that itself holds
the jmp over the outer else. In the carcass each jmp is its own `+0x002` statement
carrying the `else` keyword's line. Byte-wise identical to the braced form - pick the
ladder when the line records sit on consecutive source lines with no room for `}` lines.

```cpp
if ( a )
	if ( b )
		x( );
	else        // its own +0x002 record at this line
		y( );
else            // ditto
	z( );
```
Evidence: string_order::execute lines 62-68 = if/if/call/else/call/else/call on 7 consecutive records ('65'/'67' carry the elses), 100%.
