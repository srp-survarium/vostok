# 3+ operand `&&`/`||` chains and mixed precedence: the jump TARGETS encode the grouping
tags: cpp:if cpp:bool | asm:test asm:jcc | topic:structure-shape
symptoms: N-1 jne .body + 1 je .end for an N-way ||, N je .end for an N-way &&, mixed forms route the && group's failure to the next || operand
confidence: 9/10
variants: short-circuit-and-or.md, nested-if-vs-multiline-and.md

Extending short-circuit-and-or.md to chains. An N-operand `&&` is N × `test/je .end`
(all fall through on true). An N-operand `||` is (N-1) × `test/jne .body` + a final
`test/je .end`. **Mixed precedence is readable from where each operand's branch lands**:
`&&` binds tighter, so in `a && b || c` operand `a`'s false-branch jumps to the `c`
evaluation (the `||` alternative), while `b`'s true-branch jumps to the body. Parentheses
re-route accordingly. Probe-verified:

```asm
; a()&&b()||c()  ==  (a&&b)||c :
  call a / test / je  .EVAL_C     ; a false -> skip to the || alternative (c)
  call b / test / jne .BODY       ; a&&b true -> body
.EVAL_C: call c / test / je .end
.BODY: g=1

; a()||b()&&c()  ==  a||(b&&c) :
  call a / test / jne .BODY       ; a true -> body
  call b / test / je  .end        ; the b&&c group: either false -> end
  call c / test / je  .end
.BODY: g=1

; (a()||b())&&c() :
  call a / test / jne .EVAL_C     ; a true -> still must check c (group then &&)
  call b / test / je  .end
.EVAL_C: call c / test / je .end / g=1
```
Steerable: count `je .end` vs `jne .body` and follow the targets to recover operator count AND grouping; a `&&` operand whose false-branch skips to a LATER operand (not `.end`) means an `|| ` follows it at lower precedence. Reproduce the exact parenthesization the jump routing implies.
