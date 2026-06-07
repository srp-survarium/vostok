# bullet::compute_max_error (0x581b80) - structure verification

Reported fuzzy: ~99.94%. Verdict: STRUCTURE MATCH (shape ok). target 14 / base 14
stmts, size-diffs 1, quantity-diffs 0.

## The single SIZE diff
Statement: `float cosine_alpha = math::max( -1.0f, math::min( start_to_max_error |
start_to_target, 1.0f ) );`  (target <0x32> vs base <0x53>).

`--view diff` at 0xc1:

    base  (-): movss xmm0,[ebp-20h]; mulss xmm0,[ebp-44h]   ; x*x
               movss xmm1,[ebp-24h]; mulss xmm1,[ebp-48h]   ; y*y
               movss xmm2,[ebp-28h]; mulss xmm2,[ebp-4Ch]   ; z*z
               addss xmm2,xmm1; addss xmm2,xmm0             ; INLINED dot product
    target(+): lea eax,[ebp-4Ch]; lea ecx,[ebp-28h]; call vostok::math::operator|

Base INLINED the float3 dot product `start_to_max_error | start_to_target` into
mul/add; the target emitted an out-of-line `call vostok::math::operator|`. The base
inline is shorter, hence the 0x53 vs 0x32 size delta on this one statement.

This is the same float3 operator inline-vs-call LTCG wall recorded for
get_additional_length (assembly_patterns: `operator|` out-of-line vs inlined). Not
source-steerable from bullet.cpp: forcing the call would require forward-declaring
`vostok::math::operator|` out-of-line for the whole TU, which would change every
other `|` use (and other matched functions). The remaining const-pool label diffs
(scale_speedtree_to_vostok / clear_value, etc.) are delinker naming, same bytes.

## Conclusion
Shape matches; sole residual is the LTCG dot-product inline-vs-call. Non-steerable.
