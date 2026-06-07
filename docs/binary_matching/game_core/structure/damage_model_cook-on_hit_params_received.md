# STRUCTURE: damage_model_cook::on_hit_params_received

VERDICT: STRUCTURE MATCH (residual non-steerable)

target 0x7511f0 | base 0x566340 | report.json 89.41% (was 87.38%)
target 46 stmts / base 46 stmts; aligned 39, size-diffs 5, quantity-diffs 4.

Two real divergences were source-steerable and FIXED this pass (see
../on_hit_params_received.md):
1. `m_hit_types_strings` access: target `@@2V` (public static), base was `@@0V`
   (private). Added `public:` before it -> mangling matches.
2. The `cc_token` new-expression result is stored into a static `m_hit_types_commands`
   (`@@0PAV`, private `cc_token*`) the source lacked. Added the member + assignment.

Remaining divergences are all LTCG inline-vs-call materializations, non-steerable:
- static_cast_resource_ptr by-value temp (`0x047`).
- `fixed_string<24>("...")` ctor inlined (target) vs `call` (base) - drives the for-loop
  and push_back SIZE diffs (`0x0ce`, `0x0f5`).
- cc_token new `);` materialization (`0x182`).
- model_buffer VOSTOK_MALLOC_IMPL / damage_model operator-new materialization
  (model_buffer/L528 quantity pair, `0x2c0`).
