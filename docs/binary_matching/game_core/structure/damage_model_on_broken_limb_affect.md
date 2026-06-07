# damage_model::on_broken_limb_affect - structure verification

Verdict: STRUCTURE MATCH (shape ok). fuzzy 98.95% (report.json). Marker correct.

## Structure diff (target 0x6ef660 | base 0x45cfd0), target 15 / base 15 stmts

    .. same ..
    ; aligned 15, size-diffs 0, quantity-diffs 0

The four `if / else if` branches (each `strings::equal` + `( type ==
affect_applying ) ? 1 : 0` store) all match in count and size.

## Cause: prologue this-slot frame + trailing nop padding

Identical instruction stream apart from:

    target: push ecx       ; this-spill in a 4-byte frame, this at [ebp-4]
    base:   sub esp, 0Ch   ; 12-byte frame, this at [ebp-0Ch]
    target: 2 trailing nops (alignment); base has none

The function declares no locals; only `this` is spilled. MSVC's choice of `push
ecx` (single-slot frame) vs `sub esp,0Ch` is a prologue/frame-allocation quirk not
expressible in C++ source, and the trailing nops are inter-function alignment
padding. Non-steerable. STRUCTURE MATCH.
