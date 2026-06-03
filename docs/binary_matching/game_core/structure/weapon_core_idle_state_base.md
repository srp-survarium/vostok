# Structure verification: weapon_core_idle_state_base (PR #123)

Unit: `sources/vostok/game_core/sources/weapon_core_idle_state_base.cpp`
Grouped functions verified: ctor, initialize, finalize.
Method: `pdb_fetch --view structure` on the TARGET and BASE rich indexes, compared
statement by statement (QUANTITY = statement count; SIZE = per-statement +delta).
report.json fuzzy_match_percent read for context only.

Resolved RVAs:

| function   | target rva | base rva |
|------------|------------|----------|
| ctor       | 0x6ea680   | 0x44af40 |
| initialize | 0x6ea660   | 0x44af20 |
| finalize   | 0x6ea640   | 0x44af00 |

## 1. weapon_core_idle_state_base::weapon_core_idle_state_base(weapon_core&)

Verdict: STRUCTURE MATCH.

```
TARGET (0x6ea680)                       BASE (0x44af40)
; 2 statements, 0x31 bytes              ; 2 statements, 0x31 bytes
0x00  <0x28>  L15                       0x00  <0x28>  {
0x28  <0x9>   L16                       0x28  <0x9>   }
```

Statement count 2 == 2; sizes 0x28 == 0x28 and 0x9 == 0x9; total 0x31 == 0x31.
The single body statement (0x28 bytes) is the init-list base delegation
`: weapon_core_base_state( weapon, false )` plus the two compiler-emitted vtable
pointer stores for the multiply-inherited subobjects (fsm_state @0x00,
unmanaged_resource @0x18); these are never written in source - MSVC emits them
after the base ctor returns (see assembly_patterns.md "derived state ctor"). The
body is empty `{ }` - NO member-init body assignments and NO extra lexical block,
so no QUANTITY or SIZE divergence. Source shape is correct as written.

fuzzy_match_percent: 100.0 (mangled `??0...@@IAE@...` = protected, matches target).

## 2. void weapon_core_idle_state_base::initialize()

Verdict: STRUCTURE MATCH.

```
TARGET (0x6ea660)                       BASE (0x44af20)
; 3 statements, 0x19 bytes              ; 3 statements, 0x19 bytes
0x00  <0x7>   L19                       0x00  <0x7>   {
0x07  <0xe>   L21                       0x07  <0xe>   m_weapon.instant_idle_start( );
0x15  <0x4>   L22                       0x15  <0x4>   }
```

Statement count 3 == 3; sizes 0x7 == 0x7, 0xe == 0xe, 0x4 == 0x4; total 0x19 ==
0x19. One single-statement body (the `m_weapon.instant_idle_start( )` call at
0x07), brace-open and brace-close. No early-return guard, no nested `[n]` block,
no extra statement. Source shape is correct as written.

fuzzy_match_percent: 100.0 (mangled `?initialize@@...@@EAEXXZ` = private virtual,
matches target).

## 3. void weapon_core_idle_state_base::finalize()

Verdict: STRUCTURE MATCH.

```
TARGET (0x6ea640)                       BASE (0x44af00)
; 3 statements, 0x19 bytes              ; 3 statements, 0x19 bytes
0x00  <0x7>   L25                       0x00  <0x7>   {
0x07  <0xe>   L27                       0x07  <0xe>   m_weapon.instant_idle_end( );
0x15  <0x4>   L28                       0x15  <0x4>   }
```

Statement count 3 == 3; sizes 0x7 == 0x7, 0xe == 0xe, 0x4 == 0x4; total 0x19 ==
0x19. Same shape as initialize: one single-statement body (the
`m_weapon.instant_idle_end( )` call), no guard, no block. Source shape is correct
as written.

fuzzy_match_percent: 100.0 (mangled `?finalize@@...@@EAEXXZ` = private virtual,
matches target).

## Summary

All three functions: STRUCTURE MATCH. No QUANTITY (count) divergence and no SIZE
(+delta) divergence on any statement; totals and per-statement sizes are identical
target vs base. The 100.0% fuzzy_match_percent for each is a genuine match over the
correct structure, NOT a high-percent-over-wrong-structure trap. No restructure
needed; STATE[100%|DONE] stands for all three. No source change, no downgrade.
