# `if ( cfg.value_exists("k") ) m = (T)cfg["k"]` looks the SAME key up TWICE (guard does not memoize)
tags: cpp:operator cpp:if cpp:member | asm:call asm:test asm:jcc asm:push | topic:codegen-idiom
symptoms: call value_exists / movzx eax,al / test / je / push <same key literal again> / call operator[]
confidence: 9/10

The guarded conditional-load idiom (pervasive in `*_params` ctors). `value_exists` is its own
out-of-line call (CRC32 the key then lower-bound the sorted children); `movzx al`/`test`/`je` is
the branch. The guard does NOT memoize — the **same string literal is pushed again** for the
`operator[]` inside the `if`, so each guarded field is two key lookups. Emit one `value_exists`
+ one bool test per field, in source order.

```cpp
if ( cfg.value_exists( "base_dispersion" ) )
    base_dispersion = (float)cfg["base_dispersion"];
```
```asm
push  ??_C@_0BA@OKBBDLEB@base_dispersion?$AA@
mov   ecx, [ebp+8]
call  ...binary_config_value::value_exists
movzx edx, al
test  edx, edx
je    short ..
push  ??_C@_0BA@OKBBDLEB@base_dispersion?$AA@   ; SAME key, pushed again
mov   ecx, [ebp+8]
call  ...binary_config_value::operator[]
call  ...binary_config_value::operator float
movss [..], xmm0
```
Steerable: write the `if ( cfg.value_exists("k") ) m = (T)cfg["k"];` shape; the doubled key push is forced. cite: weapon_dispersion_params ctor weapon_dispersion_params.cpp:32-33.
