# `and 0x7FFFFFFF` through a stack slot = `math::abs(float)`; the compare's polarity names the guard

tags: cpp:float cpp:if cpp:inline | asm:and asm:comiss asm:jbe | topic:codegen-idiom topic:condition-shape
symptoms: movss [esp+N],xmm / mov reg,[esp+N] / and reg,7FFFFFFFh / mov [esp+N],reg / movss xmm,[esp+N] / comiss / jbe; a `.rdata` slot the delinker names `epsilon_5`

`vostok::math::abs( float )` is not an intrinsic - it is the bit-mask inline

```cpp
inline float abs( float value )
{
	u32 bit_value = ( *(u32*)&value ) & 0x7fffffff;
	return ( *( float* )&bit_value );
}
```

so every call site round-trips the value through a stack slot to do an INTEGER
`and`. Under /O2 MSVC happily reuses the dead incoming-argument slot for that
temp, which is why the sequence often reads `[esp+<frame size + 4>]` - above the
frame, not a local. Do not mistake it for a spill of a named local: the PDB
records no local there.

The negation mask (`xorps xmm, [__mask@@NegFloat@]`) is the *other* float-sign
idiom and means a source-level `-x`, not `abs`.

## Which guard produced the compare

`math_functions_inline.h` gives three shapes that all end in a compare against
the epsilon constant; the JUMP CONDITION separates them, so read it rather than
guessing:

| source | semantics | asm at the site |
|---|---|---|
| `math::abs( a - b ) > math::epsilon_5` | run when strictly greater | `comiss xmm_abs, xmm_eps` + `jbe <skip>` |
| `!math::is_similar( a, b )` (= `!(abs(a-b) < eps)`) | run when `>=` | `comiss` + `jb <skip>` |
| `math::is_zero( d )` (= `abs(d - T(0)) < eps`) | run when strictly less | `comiss` + `jae <skip>` |

`jbe` is the tell for the hand-written `abs(...) > eps` form - `is_similar` /
`is_zero` can never produce it. Note also that the NON-absolute difference is
usually CSE'd and reused by the guarded statement (`divss` by the same register
that fed the `and`), which confirms the source computed `a - b` once and wrapped
only the guard in `abs`.

Recovered with this in `temporal_projection_matrix_modifier::push_jittering`
(render/engine), where two such guards protect the division that rebuilds
`p.e20` / `p.e21`:

```cpp
if ( math::abs( jittered_left - jittered_right ) > math::epsilon_5 )
	p.e20 = ( jittered_left + jittered_right ) / ( jittered_left - jittered_right );
```

**Corollary for optimized (render) TUs:** the whole `abs` inline emits no
`empty_stub` call even though its body carries `R_ASSERT( valid(value) )` - in
this build asserts in render fold to zero bytes AND zero line records, so an
assert-carrying math inline is byte-free. Do not add eater bytes for it.
