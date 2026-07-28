# `threading::interlocked_*` = a `lock`-prefixed RMW: increment=`lock xadd; inc`, CAS=`lock cmpxchg`, exchange=bare `xchg`
tags: cpp:call | asm:lock asm:xadd asm:cmpxchg asm:xchg | topic:codegen-idiom
symptoms: threading::interlocked_increment/_decrement/_exchange_add/_compare_exchange/_exchange, lock xadd, lock cmpxchg, xchg
confidence: 9/10

The `_Interlocked*` intrinsics emit the locked RMW inline. `_InterlockedIncrement` is NOT `lock inc`
— MSVC lowers it to `mov eax,1; lock xadd [m],eax; inc eax` (returns the POST value, hence the trailing
`inc`). `exchange_add` is the plain `lock xadd`; CAS is `lock cmpxchg`; `interlocked_exchange` is a
bare `xchg` (implicitly locked on x86, no prefix). Under /Od a CALL SITE keeps `call
...interlocked_increment`; the `lock`-prefixed body shows in the /Ox out-of-line function.

```cpp
threading::interlocked_increment( s_global_unmounts_counter );
```
```asm
; interlocked_increment(long&)     ; interlocked_compare_exchange(long&,long,long)
mov  eax, 1                         mov  eax, [esp+4]
lock xadd [ecx], eax                lock cmpxchg [edx], ecx
inc  eax                            ret
ret                               ; interlocked_exchange: xchg [ecx],eax / ret
```
Wall (not steerable at the call site): operand register/order is fixed by the intrinsic; the `inc eax` AFTER the `xadd` is the return value, not a second op. cite: vostok::threading::interlocked_increment threading_functions_win_inline.h:38; unmount.cpp:129.
