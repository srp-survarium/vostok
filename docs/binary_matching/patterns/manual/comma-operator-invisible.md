# The comma operator is invisible: `(e1, e2);` == `e1; e2;` byte-for-byte
tags: cpp:comma | asm:call asm:mov | topic:wall topic:convention
symptoms: no distinguishing bytes between a comma expression and the equivalent statement sequence, comma in for-increment just sequences
confidence: 10/10
variants: for-head-statement-merges.md

The built-in comma operator only sequences: `e1` is evaluated for side effects and
discarded, the value is `e2`. So `g = ( side(), 7 );` and the two-statement `side(); g = 7;`
compile to the SAME bytes. A comma in a `for`-increment just emits the increments back to
back. **You cannot tell a comma expression from a statement sequence in the asm.**
Probe-verified — identical:

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
; for(...; ++i,--j): the increment block emits ++i then --j sequentially
```
Wall: the only place the comma is forced is where the grammar needs one expression (a `for`-head, a macro arg, an `if`-condition); even there it is the plain sequence. The for-head merge into one line record is for-head-statement-merges.md.
