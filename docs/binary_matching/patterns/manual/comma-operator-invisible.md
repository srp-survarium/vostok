# An expression-statement comma is invisible: `(e1, e2);` == `e1; e2;` byte-for-byte (a for-increment comma is NOT)
tags: cpp:comma | asm:call asm:mov | topic:wall topic:convention
symptoms: no distinguishing bytes between a discarded-value comma expression and the equivalent statement sequence (the for-increment comma is the EXCEPTION - its ops live in the loop increment block)
confidence: 10/10
variants: for-head-statement-merges.md, od-loop-skeletons.md

The built-in comma operator only sequences: `e1` is evaluated for side effects and
discarded, the value is `e2`. So `g = ( side(), 7 );` and the two-statement `side(); g = 7;`
compile to the SAME bytes. **You cannot tell a discarded-value comma expression from a
statement sequence in the asm.** Probe-verified — identical:

```cpp
void comma_expr( ) { g = ( side(), 7 ); }
void two_stmt  ( ) { side(); g = 7; }
```
```asm
?comma_expr@@YAXXZ PROC                  ?two_stmt@@YAXXZ PROC
  e8 .. call ?side@@YAHXZ                  e8 .. call ?side@@YAHXZ
  c7 05 .. 07000000 mov g,7                c7 05 .. 07000000 mov g,7
  5d / c3  pop ebp / ret                   5d / c3  pop ebp / ret
;        ^ byte-identical                  ^ byte-identical
; (a for-increment comma is the EXCEPTION - NOT invisible; see Wall)
```
Wall: this invisibility is ONLY for a discarded-value comma. A **for-increment comma is NOT
invisible** — `for(;;++a,f())` and `for(;;++a,--b)` lower BOTH ops into the /Od loop
**increment block** (above the test, reached by the back-edge), in source order; the same
op written at the body bottom (`for(;;++a){ …; f(); }`) lands in the **body block** instead.
That increment-block-vs-body-block placement is recoverable AND steerable, so pick the form
the target shows (probe-verified; od-loop-skeletons.md owns the /Od loop block layout). The
for-head merge into one line record is for-head-statement-merges.md.
