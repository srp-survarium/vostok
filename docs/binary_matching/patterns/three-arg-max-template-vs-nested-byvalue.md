# `math::max(a,b,c)` on floats binds the by-const-ref template and never inlines: retail spelled nested 2-arg max
tags: cpp:template cpp:float cpp:call | asm:call asm:comiss asm:fdiv | topic:inline-vs-call topic:overload-resolution topic:structure-shape
symptoms: call vostok::math::max<float> with three pushed POINTERS, ??$max@M@math@vostok@@YAMABM00@Z and ??$max@M@math@vostok@@YAMABM0@Z base-only symbols, x87 fdiv tail after the call where target is all-SSE divss, return statement SPLIT into a BASE_ONLY second record
confidence: 8/10
variants: min-max-operand-order.md

A 3-arg `math::max( x, y, z )` on floats has only the `T const&` template as a
candidate, and inside that template the nested unqualified `max(value2, value3)`
resolves at point-of-definition (floats have no ADL) to the 2-arg `T const&`
template - a two-level by-ref chain MSVC8's inliner leaves out-of-line (same
mechanism as the documented `min`->`min_integral` threshold note in
math_functions_inline.h). The call forces address-taken spills and an x87
return, splitting the statement. A nested 2-arg spelling instead binds the
by-value non-template `max(float, float)` overload, which always inlines to a
`comiss`/`ja`/`movaps` tree.

```cpp
math::max( extents.x, math::max( extents.y, extents.z ) )  // by-value overload, inlines
// NOT: math::max( extents.x, extents.y, extents.z )        // by-ref template chain, calls
```
```asm
comiss xmm1, xmm2      ; max(y, z)
ja    .1
movaps xmm1, xmm2
.1: comiss xmm0, xmm1  ; max(x, .)
ja    .2
movaps xmm0, xmm1
```
Evidence the nested spelling is retail's: the target binary has NO `max<float>`
template instantiation at all (2- or 3-arg) while `max<unsigned int>` template
instantiations survive; screen_factor and render_model_static::select_lod are the
only base emitters of both float instantiations, and both target bodies inline the
tree. Fixed render/engine renderer.cpp screen_factor; select_lod has the same bug.
