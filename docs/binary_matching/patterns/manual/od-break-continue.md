# `break`/`continue` = an inverted guard whose taken branch `jmp`s to the loop exit / increment
tags: cpp:for cpp:while | asm:jmp asm:cmp asm:jcc | topic:structure-shape
symptoms: cmp/jne .skip then jmp .end (break) or jmp .incr (continue), an extra jmp target inside the loop body
confidence: 8/10
variants: od-loop-skeletons.md, od-early-return-guard.md, closing-brace-jmp-rows.md

Inside a loop, `if (c) break;`/`continue;` lower to the guard shape: the `if` inverts to
`jne .skip`, then an unconditional `jmp` to a loop label. The **target label** is the only
difference — `break` -> loop-EXIT label (the one the false test targets); `continue` ->
the **increment block** of a `for` (the `.test` of a `while`). Probe-verified, both in a
`for`:

```cpp
for ( int i=0; i<n; ++i ) { if ( i==5 ) break;    g += i; }   // -> jmp .end
for ( int i=0; i<n; ++i ) { if ( i==5 ) continue; g += i; }   // -> jmp .incr ($LN3)
```
```asm
; break:                              ; continue:
  83 7d fc 05  cmp [i],5               83 7d fc 05  cmp [i],5
  75 02        jne SHORT $LN1@brk      75 02        jne SHORT $LN1@cont
  eb 11        jmp SHORT $LN5@brk      eb e7        jmp SHORT $LN3@cont
                    ; ^ loop EXIT               ; ^ the for-INCREMENT block ($LN3), not the body
$LN1: ...body...                      $LN1: ...body...
$LN5: (exit)                          $LN3: (++i) -> falls into the test
```
Steerable: a `jmp` from mid-body to the loop's exit vs its increment label names break vs continue; `continue` in a `for` lands on `++i` then falls into the test.
