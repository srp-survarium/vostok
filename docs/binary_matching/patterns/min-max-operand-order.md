# math::min/max xmm operand order reveals (and is steered by) SOURCE argument order
tags: cpp:float cpp:operator | asm:movss asm:call | topic:codegen-idiom
symptoms: movss xmm0/xmm1 swapped vs target, movss xmm1 xmm0 shuffle before call min
confidence: 9/10

Which operand lands in xmm0 vs xmm1 at a `math::min`/`max` call is decided by SOURCE
operand order - never bank an xmm-operand-order diff (~3 instrs, ~84%) as LTCG; swap the
operands (min/max commute) and rebuild. The promoted float `vostok::math::min` takes
xmm0=SECOND arg, xmm1=FIRST arg in both builds, so a base `movss xmm1,xmm0` shuffle (a
prior call's result moved out of xmm0) means the source order is swapped vs the original.

```cpp
m_energy_current -= math::min( m_energy_current, amount );   // member FIRST -> arg in xmm0, member in xmm1
```
Evidence: generic_anomaly_core::dec_energy (100% reference); legs_ik_processor set_{heel,toe}_transition_time 83.69 -> 100 swapping to member-first; bullet::tick wanted min(m_life_time, g_bullet_tracer_exposition), collide_front_face min(1.0f, max(...)) - both recovered from the register shuffle alone.
