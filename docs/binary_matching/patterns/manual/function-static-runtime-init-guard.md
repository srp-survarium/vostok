# `static T x = f();` (function-local, runtime init) = a first-call guard bit in the TU's `$S` flags word
tags: cpp:static cpp:local | asm:test asm:or asm:jcc | topic:codegen-idiom
symptoms: test [$S..],reg / jne over the init / or [$S..],reg, then the init body once, value read from the named static after
confidence: 9/10

A function-local `static T x = f();` with a runtime initializer emits a once-guard. In MSVC8 the
guard is a BIT in a per-TU `$S` flags word (NOT the C++11 thread-safe magic-statics machinery):
`test [$S],bit; jne done; or [$S],bit; <init>; done:`. The named static holds the result; the
guard lives in the separate `$S` symbol, shared across the TU's statics.

```cpp
static bool s_use_console_for_logging = s_use_console || testing::run_tests_command_line();
```
```asm
test  [$S3], bl          ; already initialized?
jne   short $LN..
or    [$S3], ebx         ; set guard bit
... <runtime init runs once> ...
mov   [s_use_console_for_logging], bl
mov   al, [s_use_console_for_logging]   ; later calls read straight through
```
Steerable: write the `static T x = f();` spelling; the `$S` guard word + bit ordering are emitted automatically. Distinct from a file-scope static's dynamic initializer (`??__E`/atexit, console-command-static-init.md). cite: use_console_for_logging logging_extensions.cpp:147.
