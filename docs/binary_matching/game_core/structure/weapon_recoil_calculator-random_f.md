# pseudo_random::random_f - STRUCTURE MATCH (size)

Verdict: STRUCTURE MATCH. 60.74%. All 5 statements present (aligned 3, quantity-diffs 0).
Two SIZE rows on the `k` and `result` statements.

## Cause (non-steerable)
The `k` expression ends with `math::pow( math::sin( (t-pi_d2)/12 ), 5 )`. The literal
`5` is an `int`, so it binds `math::pow(float, int)`. The TARGET inlines that overload
at the call site; BASE keeps it an out-of-line `call vostok::math::pow`.

Both indexes carry `pow_impl(float,uint)` out-of-line. The `pow(float,int)` overload
exists out-of-line in BASE (rva 0x4aaa0) but NOT in TARGET (inlined whole-program).
Disassembling base `pow(float,int)` @0x4aaa0:

    test ecx,ecx; jne .1; movss xmm0,[1.0]; ret      ; pow(x,0)=1
    .1: jge .2; movss xmm1,[1.0]; divss xmm1,[esp+4]; neg ecx; jmp pow_impl
    .2: movss xmm1,[esp+4]; jmp pow_impl

is exactly the target's inlined block (0x6b-0xc0): `mov edx,5; test; jne; ...; call
pow_impl`. The inline also reorders operand evaluation (the second pow's sin operand is
computed first at target 0x39-0x5f) and shifts the `result` row from x87 to xmm codegen.

This is the documented per-call-site inline-vs-call LTCG class (same as operator| /
is_aimed / fixed_string ctor). Source `math::pow(sin(...), 5)` is already correct.

## Note
The prior STATE said "pow inlined in base" - that was backwards: the target inlines,
base calls out-of-line. Corrected.

report.json: 60.74%.
