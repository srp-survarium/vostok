# weapon_recoil_calculator::fire - STRUCTURE MATCH (size)

Verdict: STRUCTURE MATCH after a real source-precedence FIX. 79.80% -> 91.69%.

## The fix (steerable - applied)
The reconstruction had:

    float recoil = m_player_recoil_multiplier * force_koef * first_shoot ? a : b;

`*` binds tighter than `?:`, so this parses as `(mult*koef*first_shoot) ? a : b` -
the WHOLE product becomes the ternary condition. The target asm tests `first_shoot`
ALONE as the condition:

    0x7e: movzx ecx, byte ptr [first_shoot]
    0x82: test  ecx, ecx
    0x84: je    .else          ; pick first_shoot_side_recoil vs shoot_side_recoil
    ...then multiply the selected value

So the target source parenthesizes the ternary:

    float recoil = m_player_recoil_multiplier * force_koef * ( first_shoot ? a : b );

Applied to both `recoil` and `recoil_amount`. After rebuild the two `ONLY base` /
`ONLY target` rows on those statements disappeared (aligned 18 -> 19), 0 regressions.

## Residual (non-steerable)
- `bool first_shoot = ... && ...` : target stores the && result temp as a BYTE
  (`mov byte[ebp-39h],1`), base as a DWORD (`mov dword[ebp-30h],1`) + a frame-slot
  shift ([ebp-2Ch] vs [ebp-38h]). /Od bool-temp materialization width, not source.
- `math::deg2rad` and `math::min` inline-vs-call SIZE rows (LTCG).

report.json: 91.69% (was 79.80%).
