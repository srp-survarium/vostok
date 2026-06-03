# Structure verification: weapon_user_animations_container_cook::weapon_user_animations_container_cook (ctor)

Verdict: STRUCTURE MATCH

Function: `survarium::weapon_user_animations_container_cook::weapon_user_animations_container_cook()`
- target rva 0x752910, base rva 0x44c930
- report.json fuzzy_match_percent = 100.0 (mangled ??0weapon_user_animations_container_cook@survarium@@QAE@XZ)

## Skeletons (pdb_fetch --view structure)

```
TARGET  ; 2 statements, 0x3c bytes        BASE  ; 2 statements, 0x3c bytes
0x00  <0x35>  L15                          0x00  <0x35>  {
0x35  <0x7>   L16                          0x35  <0x7>   }
```

Statement quantity: 2 vs 2 (match). Per-statement sizes: 0x35 / 0x7 on both sides
(match). No QUANTITY divergence, no SIZE divergence.

The ctor uses a member-initializer list and an EMPTY body:

```
weapon_user_animations_container_cook::weapon_user_animations_container_cook()
    : resources::translate_query_cook( resources::animation_container_class, reuse_true, use_current_thread_id )
{
}
```

The base-class init is attributed to the decl line (L15, 0x35 bytes - the whole
init collapses into the opening statement); L16 is the closing `}` (0x7). This is
exactly the target shape - the 2-statement skeleton is what an init-list-only ctor
with an empty body produces, NOT body assignments (which would split into N
statements). Correct structure.

100% in report.json over the CORRECT structure - not the trap. STATE[100%|DONE]
in weapon_user_animations_container_cook.cpp:10 is accurate; no change.
