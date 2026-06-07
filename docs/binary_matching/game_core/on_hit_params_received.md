# damage_model_cook::on_hit_params_received

target 0x7511f0 | base 0x566340 | report.json: 87.38% -> 89.41% (this pass)

`?on_hit_params_received@damage_model_cook@survarium@@AAEXAAVqueries_result@resources@vostok@@@Z`

## Two real source bugs recovered this pass

### 1. m_hit_types_strings is a PUBLIC static (mangling `@@2V`, base had `@@0V`)
The two `fixed_vector` statics had DIFFERENT access in the original:
- `m_hit_types`         -> `?m_hit_types@...@@0V...`  (private, `0`) - matched on BOTH sides.
- `m_hit_types_strings` -> target `?m_hit_types_strings@...@@2V...` (PUBLIC, `2`),
  base was `@@0V` (private).

For static DATA members the access char after `@@` is `0`=private, `1`=protected,
`2`=public (distinct from the member-FUNCTION codes Q/A/I and the virtual U/M/E).
Fix: add a `public:` before `m_hit_types_strings` in the header so it mangles `@@2V`.
Order preserved (`m_hit_types` then `m_hit_types_commands` stay private, then a
`public:` opens for `m_hit_types_strings`).

### 2. The new cc_token is stored into a missing static `m_hit_types_commands`
Target asm right after the `cc_token::cc_token` ctor + the new-null-check join:

    mov   edx, [ebp-114h]
    mov   [?m_hit_types_commands@damage_model_cook@survarium@@0PAVcc_token@console_commands@vostok@@A], edx

i.e. `m_hit_types_commands = VOSTOK_NEW_IMPL( g_allocator, cc_token )( ... );` where
`m_hit_types_commands` is a private static `cc_token*` (`@@0PAV`). Our source used the
`VOSTOK_NEW_IMPL(...)` as a discarded bare expression. Added the static member (header
+ .cpp definition, declared private to keep its `@@0PAV` code) and the assignment.

## Residual (non-steerable, all LTCG inline-vs-call)
`--view structure-diff --condensed`: aligned 39, size-diffs 5, quantity-diffs 4.
- `0x047` static_cast_resource_ptr config: resource_ptr by-value temp materialization
  (construct + `dec` on the temp) - target inlines a different intrusive_ptr shape.
- `0x0ce` / `0x0f5` for-loop + push_back: the `fixed_string<24>("...")` ctor is INLINED
  in target (`buffer_string::buffer_string` 3-arg, 0x18 capacity to a slot) but a
  `call fixed_string<24>::fixed_string<24>` in base - the documented fixed_string ctor
  inline-vs-call LTCG wall (assembly_patterns.md). The `je short`->`je near` is a cascade.
- `0x182` cc_token new `);` : new-null-check materialization.
- `model_buffer`/L528 quantity + `0x2c0` damage_model new: malloc_helper / operator-new
  materialization, inlined differently.

None steerable from this function's source; the call-boundary/inline decisions are
whole-program LTCG. STRUCTURE MATCH.
