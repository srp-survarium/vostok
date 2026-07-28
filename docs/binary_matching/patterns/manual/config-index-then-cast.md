# `(T)config["key"]` = `call binary_config_value::operator[]` then `call operator T`
tags: cpp:operator cpp:cast cpp:member | asm:call asm:movss asm:push | topic:codegen-idiom
symptoms: push ??_C@_0..key / mov ecx,cfg / call binary_config_value::operator[] / call ...operator float (or u32/...) / store
confidence: 9/10

`operator[](pcstr)` is out-of-line but `operator T` casts are inline-but-still-emitted. So a
scalar `(float)config["k"]` is one `operator[]` call (key pushed, `cfg` the `this` in ecx,
returns the child node in eax) followed by the cast call on that node; float/int casts return in
xmm0/eax and store directly. Do NOT cache the returned node.

```cpp
m_spending_threshold = (float)config["spending_threshold"];
```
```asm
push  ??_C@_0BD@CINAKECL@spending_threshold?$AA@
mov   ecx, [ebp+8]            ; cfg (this for operator[])
call  vostok::configs::binary_config_value::operator[]
call  vostok::configs::binary_config_value::operator float
mov   eax, [ebp-74h]
movss [eax+3Ch], xmm0         ; -> member
```
Steerable to 100% as the natural `(T)config["k"]` spelling; the inline-vs-call cut on `operator T` itself is the residual (config-value-operator-cast.md). cite: player_stamina::load player_stamina.cpp:66.
