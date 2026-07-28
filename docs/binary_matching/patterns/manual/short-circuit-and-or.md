# `&&` and `||` short-circuit: && is all `je .end`; || flips the first to `jne .body`
tags: cpp:if cpp:bool | asm:test asm:jcc | topic:codegen-idiom
symptoms: two sequential test/je for &&, first operand test/jne to the body for ||, second operand only evaluated when needed
confidence: 9/10
variants: and-chain-third-bool-temp.md, nested-if-vs-multiline-and.md

`if (a && b)`: eval `a`, `test/je .end` (false short-circuits past the body); eval `b`,
`test/je .end`; body. `if (a || b)`: eval `a`, `test/jne .body` (TRUE short-circuits INTO
the body); eval `b`, `test/je .end`; body. The second operand's evaluation (here a `call`)
physically sits AFTER the first test — proof it is conditional (genuine `&&`/`||`, not a
bitwise `&`/`|` which evaluates both). Probe-verified:

```cpp
void andsc( ) { if ( a() && b() ) g = 1; }
void orsc ( ) { if ( a() || b() ) g = 1; }
```
```asm
?andsc@@YAXXZ PROC                       ?orsc@@YAXXZ PROC
  e8 .. call ?a@@YAHXZ                      e8 .. call ?a@@YAHXZ
  85 c0 test eax,eax                        85 c0 test eax,eax
  74 13 je   $LN2@andsc   ; a false -> end  75 09 jne  $LN1@orsc   ; a TRUE  -> body
  e8 .. call ?b@@YAHXZ                      e8 .. call ?b@@YAHXZ
  85 c0 test eax,eax                        85 c0 test eax,eax
  74 0a je   $LN2@andsc   ; b false -> end  74 0a je   $LN3@orsc   ; b false -> end
  c7 05 .. 01.. mov g,1   ; body          $LN1@orsc:
$LN2@andsc:                                 c7 05 .. 01.. mov g,1   ; body
                                          $LN3@orsc:
```
Steerable: a second-operand `call`/load guarded by the first operand's branch = short-circuit `&&`/`||`; a bool TEMP materialized before the branch instead = an inlined predicate, not a bare `&&` (and-chain-third-bool-temp.md).
