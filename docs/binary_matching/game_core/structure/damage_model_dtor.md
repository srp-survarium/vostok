# damage_model::~damage_model - structure verification

Verdict: STRUCTURE MATCH (shape ok). fuzzy 83.38% (report.json). Marker correct.

## Structure diff (target 0x6efdd0 | base 0x45d880), target 5 / base 5 stmts

    .. same ..
    ; aligned 5, size-diffs 0, quantity-diffs 0

All 5 source statements (the `while ( pop_front )` delete loop and the two
`unsubscribe_from_affect` calls) match in count and size.

## Cause: compiler-generated member-destructor epilogue

The divergence is entirely AFTER the closing `}` at offset 0x73 - the auto-
generated destruction of the embedded members (`m_hand_damaged_subscriber`,
`m_leg_damaged_subscriber`, the affect-subscription lists, the body-parts list).
Target inlines a member dtor through `esi`:

    target: lea esi,[..] ; call dtor ; ... (esi reused)
    base:   mov [ebp-2Ch],reg ; mov reg,[ebp-2Ch] ; call ; ... (routed through slots)

plus `push esi`/`pop esi` and the usual ICF-folded call-target offsets. This is
compiler-generated cleanup code, not source-authored; no source statement
controls it. Non-steerable. STRUCTURE MATCH.
