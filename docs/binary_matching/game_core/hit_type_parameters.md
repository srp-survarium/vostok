# hit_type_parameters.cpp - structure verification (item/material batch)

| fn | old marker | report.json | structure | wall |
|---|---|---|---|---|
| ctor(pcstr,f,f,f,u32) | 78.14% DONE (STALE) | 97.62% | MATCH 0/0 | m_type fixed_string ctor inline-vs-call frame-slot (LTCG) |
| apply_damage | 100% DONE | 100% | - | - |
| set_parameters | 100% DONE | 100% | - | - |

## ctor (97.62%, marker was wildly stale at 78.14%)
structure-diff: target 0 / base 0 stmts, 0 size, 0 quantity.
The only asm diff is the frame slot: target `push ecx` (4-byte frame, `this` at
`[ebp-4]`) vs base `sub esp,0Ch` (`[ebp-0Ch]`). Base reserves extra frame because the
`m_type( type )` fixed_string member ctor materializes its capacity temp inline; the
member stores (next/m_type call, m_absorption_amount/m_armor/m_reduce movss,
m_bdb_count) are byte-exact apart from the `[ebp-4]` vs `[ebp-0Ch]` rename. Same
fixed_string inline-vs-call frame class as game_material ctor. Non-steerable.

VERDICT: STRUCTURE MATCH. Marker synced 78.14% -> 97.62%, embed added. No logic change.
