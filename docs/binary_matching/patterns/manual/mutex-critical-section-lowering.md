# `threading::mutex` lock/unlock = Enter/LeaveCriticalSection; the tiny `unlock` inlines as `__imp__LeaveCriticalSection@4`
tags: cpp:member | asm:call asm:push | topic:codegen-idiom topic:convention
symptoms: call mutex::lock (spins on TryEnterCriticalSection), push <cs>; call dword ptr [__imp__LeaveCriticalSection@4], no matching Enter call
confidence: 9/10

`threading::mutex` wraps a `CRITICAL_SECTION`. `lock()` is a spin loop over
`TryEnterCriticalSection` (kept out-of-line: `call mutex::lock`); `unlock()` is a 7-byte
`push ecx / call LeaveCriticalSection / ret`, so the RAII dtor inlines the release directly as
`push <cs> / call __imp__LeaveCriticalSection@4` rather than a `call mutex::unlock`. Contrast a
`simple_lock` raii, which keeps a real `call ...::clear` (scoped-lock-raii.md).

```cpp
threading::mutex_raii raii( m_name_registry_mutex );
```
```asm
call  vostok::threading::mutex::lock              ; acquire (spins on TryEnterCriticalSection)
...
push  edi
call  dword ptr [__imp__LeaveCriticalSection@4]   ; release inlined at the scope's }
```
Steerable: a `LeaveCriticalSection` with no matching `EnterCriticalSection` call = the inlined `mutex::unlock` dtor — pick `mutex` (not `simple_lock`) when you see the CS lowering. cite: resources_manager::finalize_name_registry resources_manager.cpp:305.
