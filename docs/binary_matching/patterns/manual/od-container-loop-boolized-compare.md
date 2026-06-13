# /Od container loop re-derives `end()` EVERY turn and boolizes `it != end` (xor/cmp/setne/movzx/test/je)
tags: cpp:for cpp:iterator | asm:setne asm:movzx asm:cmp | topic:codegen-idiom topic:stlport
symptoms: add eax,<container off> then mov [tmp],eax re-derived per turn, xor/cmp/setne/movzx/test/je chain instead of a bare cmp/jcc
confidence: 8/10

Unlike the plain-integer /Od skeleton (a bare `cmp/jcc`), an STL iterator `it != c.end()` under
/Od **re-derives `end()` into a temp on every turn** and **boolizes** the comparison through
`xor; cmp; setne; movzx; test; je`. That long per-turn preamble (~7 rows) is the tell that the
loop bound is a container `end()` call, not a cached integer.

```cpp
for ( const_iterator iter = m_filtered_items.begin(); iter != m_filtered_items.end(); ++iter )
```
```asm
mov   eax, [ebp-40h]
add   eax, 1Ch          ; re-take &container (end()) EACH iteration
mov   [ebp-14h], eax
mov   ecx, [ebp-14h]
mov   [ebp-8], ecx
mov   edx, [ebp-4]
xor   eax, eax
cmp   edx, [ebp-8]       ; iter != end
setne al
movzx ecx, al
test  ecx, ecx
je    short ..
```
Steerable: caching `end()` in a named local removes the per-turn re-derivation — match the source's exact choice (cached local vs inline `c.end()`). cite: vostok::ai::planning::sound_filter::contains_object sound_filter.cpp:32 (cached form: math_convex.cpp:263).
