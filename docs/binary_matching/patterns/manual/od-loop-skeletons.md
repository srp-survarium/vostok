# The three /Od loop skeletons: for vs while vs do-while are distinguishable
tags: cpp:for cpp:while cpp:do-while cpp:loop | asm:jmp asm:cmp asm:jcc | topic:structure-shape
symptoms: jmp short .test entry, increment block ABOVE the test, test-at-top with no entry jmp, body-first bottom-test non-inverted back-edge
confidence: 9/10
variants: for-head-statement-merges.md, loop-comparator-je-vs-jae.md, closing-brace-jmp-rows.md

The loop keyword is recoverable from layout under /Od. **for**: `init` -> `jmp .test`
(entry skips the increment) -> **increment block** -> inverted compare exits -> body ->
`jmp .incr` back. **while**: test at TOP, no entry jmp. **do-while**: body first, bottom
compare NOT inverted (`jg` back-edge), no top test. Probe-verified, all three:

```cpp
void counted( int n ) { for ( int i=0; i<n; ++i ) g += i; }
void w      ( int n ) { while ( n>0 ) { g += n; --n; } }
void dw     ( int n ) { do { g += n; --n; } while ( n>0 ); }
```
```asm
?counted@@YAXH@Z PROC                       ; FOR
  c7 45 fc 00..  mov  DWORD PTR _i$[ebp], 0          ; init
  eb 09          jmp  SHORT $LN3@counted             ; <-- ENTRY jmp straight to the test
$LN2@counted:                                        ; increment block sits ABOVE the test
  8b 45 fc / 83 c0 01 / 89 45 fc                     ; ++i
$LN3@counted:
  8b 4d fc / 3b 4d 08   cmp  ecx, _n$[ebp]
  7d 11          jge  SHORT $LN4@counted             ; inverted exit
  ... body (g += i) ...
  eb de          jmp  SHORT $LN2@counted             ; back-edge to the INCREMENT
$LN4@counted:

?w@@YAXH@Z PROC                             ; WHILE  (no entry jmp; test at top)
$LN2@w:
  83 7d 08 00 / 7e 18   cmp [n],0 / jle .end         ; test at TOP, inverted exit
  ... body (g+=n; --n) ...
  eb e2          jmp  SHORT $LN2@w
$LN3@w:

?dw@@YAXH@Z PROC                            ; DO-WHILE (body first; bottom test, NOT inverted)
$LN3@dw:
  ... body (g+=n; --n) ...
  83 7d 08 00 / 7f e4   cmp [n],0 / jg .top          ; back-edge, non-inverted; no top test
```
Steerable: the `jmp .test` entry + a separate increment block = a `for` (not a `while`); a `while(c){...;++i;}` puts `++i` INLINE at the body bottom. for-head merges into one line record (for-head-statement-merges.md); `i != N` exits on `je`, `i < N` on `jge`/`jae` (loop-comparator-je-vs-jae.md).

**What lands in the increment block** (the codebase's lockstep idiom): whatever you put in the for-increment goes there, ABOVE the test, in source order — `++a`, `++a,--b`, or `++a,f()` all emit into that block (the comma is NOT invisible here, cf. comma-operator-invisible.md). The SAME op written at the body bottom — `for(;;++a){ …; --b; }` or `{ …; f(); }` — lands in the body block instead (below the body, before the back-edge `jmp .incr`). So increment-block-vs-body-block placement distinguishes `for(;;++a,f())` from `for(;;++a){…;f();}` (probe-verified: `for(a=0;a<10;++a,hello())` puts `call hello` between `add a,1` and the `cmp`, while end-of-body puts it between `call body` and the back-edge).
