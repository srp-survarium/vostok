# Structure audit: dispersion_calculator::get_dispersion

- Target rva 0x586970, base rva 0x45c300
- Signature: `float survarium::dispersion_calculator::get_dispersion() const`
- report.json fuzzy_match_percent: 87.49152%
- STATE in source: `87.49%|PARTIAL`

## Verdict

STRUCTURE MATCH (source shape mirrors the target; the residual byte/quantity
delta is LTCG + COMDAT inline-vs-call only, not a source-structure defect).

The function is correctly labeled PARTIAL, not a mislabeled DONE. No downgrade,
no restructure required. Nothing new caught by the audit beyond what the existing
claude@NOTE comments already document.

## Condensed structure-diff

```
target: 0x586970            base: 0x45c300
; float survarium::dispersion_calculator::get_dispersion() const ; target 6 stmts / base 9 stmts
.. same ..
0x010 <0x7b> | 0x010 <0x87> | if ( !( m_weapon && m_weapon->ammunition( ) && s_dispersion_enabled_value ) )   SIZE
.. same ..
--          | <0>         |    EMPTY only base
0x092 <0xf> | 0x09e <0xe> | weapon_dispersion_params const& weapon_params = m_weapon->get_dispersion_params( );   SIZE
--          | <0>         |    EMPTY only base
--          | 0x0ac <0x3d> | : weapon_params.from_the_hip_multiplier;   ONLY base
.. same ..
0x0a1 <0xef> | 0x0e9 <0xc1> | + ( m_weapon_calculator.get_value( ) + m_character_calculator.get_value( ) ) * m_shooting_skill_coeff;   SIZE
; aligned 3, size-diffs 3, quantity-diffs 3
```

## Divergence analysis

Statement skeletons line up one-to-one in QUANTITY of *source* statements; the
6-vs-9 stmt count is the diff parser counting base's extra address-bearing lines
that come from a different codegen lowering, not from a different source shape.

1. SIZE @ L27 `if ( !( m_weapon && m_weapon->ammunition() && s_dispersion_enabled_value ) )`
   target 0x7b vs base 0x87. Same guard statement. The `ammunition()` safe-bool
   test (`operator unspecified_bool_type`) lowers via an extra bool stack slot in
   base vs a direct `cmp/sete` in target. LTCG materialization. Not source-steerable.

2. SIZE @ L29 `weapon_dispersion_params const& weapon_params = ...` target 0xf vs
   base 0xe. One byte, slot/register allocation noise.

3. ONLY base `: weapon_params.from_the_hip_multiplier;` (0x3d). This is the else
   branch of the source ternary
   `m_weapon->is_aimed() ? weapon_params.aim_multiplier : weapon_params.from_the_hip_multiplier`.
   The target carcass collapses the whole ternary region into inlined no-address
   lines `<0>..<9>`; the base, having INLINED `is_aimed()` as `[this+0x488]`
   (m_aimed) instead of calling `weapon_core::is_aimed` out-of-line, materializes
   the else branch as its own address-bearing source line. This is the COMDAT
   inline-vs-call decision, confirmed in the `--view diff` (target: `call
   survarium::weapon_core::is_aimed`; base: `mov al,[edx+488h]`). The source
   ternary is already written in the target's shape; the extra base statement is a
   codegen artifact, not a missing/added source statement.

4. SIZE @ L40 final return expression, target 0xef vs base 0xc1. Same statement;
   size delta is downstream of (1) and (3) (frame size 58h vs 4Ch -> [ebp-XX]
   slot reassignment) plus the inlined is_aimed body folded into this region.

## Source-shape conclusion

The target carcass (binaries/structure/target/.../dispersion_calculator.cpp)
shows statements at source lines 27 (guard), 28 (`return 0.0f`), 29
(`weapon_params` ref), inlined `<0>..<9>` for the ternary, then 40 (final return).
Our source (lines 45-54) reproduces exactly that order and shape: flat
early-return guard (no wrapping `if` block), reference binding, a single ternary
expression, single return expression. No body-assign-vs-init-list issue, no
brace/early-return mismatch, no definition-order issue.

No source fix is warranted. The residual to 100% is the inline-vs-call COMDAT
decision on `is_aimed()` and the safe-bool slot materialization - neither is
steerable from this translation unit. PARTIAL is the correct label.
