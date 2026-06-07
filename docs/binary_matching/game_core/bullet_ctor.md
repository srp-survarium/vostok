# bullet::bullet(...) full ctor (0x5814b0) - structure verification

Reported fuzzy: ~97.92%. Verdict: STRUCTURE MATCH (shape ok). target 4 / base 4
stmts, size-diffs 0, quantity-diffs 0 (the member-init list collapses to a handful
of attributed statements; both sides agree).

## Residual cause (non-steerable)
`--view diff`:
- Frame: base `sub esp, 58h` (this @ [ebp-58h]) vs target `sub esp, 54h` (this @
  [ebp-54h]). Base carries one extra 4-byte temp slot; both this-slots are disp8 so
  there is NO size cascade (unlike tick) - the structure-diff is clean.
- Commutative FP-multiply operand/register order, e.g.:
  - `m_air_resistance = wa->air_resistance( ) * air_resistance`:
    base `mulss xmm0,[ebp+18h]` (keeps call result in xmm0, mul by param) vs
    target `movss xmm0,[ebp+18h]; mulss xmm0,[ebp-0Ch]` (param into xmm0, mul by
    stored call result).
  - `m_max_distance = wa->distance( ) * 1000.f`:
    base `movss xmm0,[ebp-18h]; mulss xmm0,[1000]` vs
    target `movss xmm0,[1000]; mulss xmm0,[ebp-18h]`.

Both are the compiler choosing which operand of a commutative `a*b` lands in xmm0 -
a register-allocation/LTCG decision, not source order (swapping operands would
reorder the side-effecting `wa->...()` call and not match). The const-pool label
diffs (out_of_range_reward/offset, unreferenced_parameter_helper vs the
fixed_size_allocator::finalize_impl ASSERT stub) are delinker naming, same bytes.

## Conclusion
Member-init order and statement shape match the target exactly. Residual = LTCG
commutative-mul operand order + a 4-byte frame temp. Non-steerable.
