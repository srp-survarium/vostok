# damage_model::hit_body_part - structure verification

Verdict: STRUCTURE MATCH (shape ok). fuzzy 99.90% (report.json, post-rebuild).
Marker was stale at 99.82%; corrected to 99.90%.

## Structure diff (target 0x6efaf0 | base 0x45d6e0), target 18 / base 18 stmts

    .. same ..
    ; aligned 18, size-diffs 0, quantity-diffs 0

## Cause: frame size + slot numbers (allocation noise)

Identical instruction stream. The only differences:

    target: sub esp, 60h ; this at [ebp-5Ch] ; temps at [ebp-60h]
    base:   sub esp, 64h ; this at [ebp-60h] ; temps at [ebp-64h]

Base reserves 4 extra bytes; every `[ebp-N]` slot is shifted by 4. Per
MATCHING.md, `[ebp-N]` slot numbers and ordering are explicitly allocation noise.
No statement, brace, or local controls this 4-byte frame choice. Non-steerable.
STRUCTURE MATCH.
