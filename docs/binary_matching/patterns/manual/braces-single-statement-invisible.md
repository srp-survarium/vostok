# Braces around a SINGLE statement are invisible — `if(c) s;` == `if(c){ s; }` byte-for-byte
tags: cpp:if cpp:for cpp:while | asm:cmp asm:jmp | topic:wall topic:convention
symptoms: identical bytes with and without braces, no line-table or size difference, brace presence unrecoverable
confidence: 10/10
variants: lexical-block-relocates-dtor.md, rel8-rel32-brace-exit.md, sibling-braced-scopes.md

A `{ }` that wraps a single controlled statement (an `if`/`else`/`for`/`while` body) is
PURELY lexical — no object lifetime, no scope cost — so codegen is byte-identical with or
without it. **You cannot recover brace presence from the asm here.** Probe-verified: the two
functions below compiled to identical bytes (only the base address + auto label suffix
differ):

```cpp
void nobrace( int a ) { if ( a ) g = 1; }
void brace  ( int a ) { if ( a ) { g = 1; } }
```
```asm
?nobrace@@YAXH@Z PROC                  ?brace@@YAXH@Z PROC
  83 7d 08 00  cmp [a],0                 83 7d 08 00  cmp [a],0
  74 0a        je  $LN2@nobrace          74 0a        je  $LN2@brace
  c7 05 .. 01..  mov g,1                 c7 05 .. 01..  mov g,1
$LN2@nobrace: 5d/c3  pop ebp/ret        $LN2@brace:   5d/c3  pop ebp/ret
;  ^ identical opcodes & sizes          ^ identical opcodes & sizes
; (for(...) g+=i;  ==  for(...){ g+=i; }  likewise byte-identical)
```
Wall: braces become VISIBLE only when they bound a local with a non-trivial destructor (the dtor moves to the `}` — lexical-block-relocates-dtor.md) or change slot aliasing (sibling-braced-scopes.md). For a single plain statement there is no tell.
