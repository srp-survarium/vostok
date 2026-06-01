# game_core::weapon_dispersion_calculator::{set_reload_dispersion_amount, set_one_shoot_dispersion_amount, set_aiming_speed}

Grouped as ONE unit of work (three trivial same-class setters sharing scaffolding),
per the accessor-grouping rule in agentic_loop.md / matcher.md.

Branch based on `match/game_core-weapon_dispersion_calculator_get_value` (PR #112,
OPEN, base `feature/agentic-matching-loop`) so we inherit that PR's scaffolding:
the class member layout, the `weapon_dispersion_calculator.h` private members, and
the `use_weapon_dispersion_calculator` anchor in temp_include_all.cpp.

## Target asm (pdb_fetch --view target)

set_reload_dispersion_amount @ 0x57f860:
```
push ebp ; mov ebp,esp ; push ecx ; mov [ebp-4],ecx
mov   eax,[ebp-4]
movss xmm0,[ebp+8]
movss [eax+4],xmm0        ; -> offset 0x4 = m_reload_dispersion_amount
mov esp,ebp ; pop ebp ; ret 4
```

set_one_shoot_dispersion_amount @ 0x57f840:
```
...
movss [eax],xmm0          ; -> offset 0x0 = m_one_shoot_dispersion_amount
ret 4
```

set_aiming_speed @ 0x57f800:
```
...
movss [eax+0Ch],xmm0      ; -> offset 0xC = m_aiming_speed
ret 4
```

Member layout (weapon_dispersion_calculator.h):
- 0x00 m_one_shoot_dispersion_amount
- 0x04 m_reload_dispersion_amount
- 0x08 m_growth_speed
- 0x0C m_aiming_speed
- 0x10 m_max_value
- 0x14 m_target_coeff
- 0x18 m_current_coeff (read by get_value)
- 0x1C m_current_time

Each setter writes the member whose name matches its argument; the disassembly
offsets confirm this exactly (no "wrong-looking" field, all clean 1:1).

## Commands run (verbatim, in order)
- git checkout -b match/game_core-weapon_dispersion_calculator_setters match/game_core-weapon_dispersion_calculator_get_value
- pdb_rich_query --index binaries/rich/target/index.jsonl --function set_reload_dispersion_amount --list
- pdb_rich_query --index binaries/rich/target/index.jsonl --function set_one_shoot_dispersion_amount --list
- pdb_rich_query --index binaries/rich/target/index.jsonl --function set_aiming_speed --list
- pdb_fetch --target-index binaries/rich/target/index.jsonl --function set_reload_dispersion_amount --rva 0x57f860 --view target
- pdb_fetch --target-index binaries/rich/target/index.jsonl --function set_one_shoot_dispersion_amount --rva 0x57f840 --view target
- pdb_fetch --target-index binaries/rich/target/index.jsonl --function set_aiming_speed --rva 0x57f800 --view target
- python3 scripts/rebuild.py   # NO module arg (bare name skips EXE relink = stale score)

## Iterations
1. INPUT: each setter body `m_<member> = <arg>;` matching the asm offset; signature
   changed to `const float` to match the header. Anchor in temp_include_all
   `use_weapon_dispersion_calculator`: take address of each setter member-fn into a
   table escaped through example_callback (forces standalone bodies to be kept),
   then call all three setters on `calc` and escape `&calc` through example_callback
   so the stores are observed (defeats LTCG dead-store elimination).
   BUILD: report.json fuzzy_match_percent = 100.0 for all three:
     ?set_aiming_speed@...@@QAEXM@Z              100.0
     ?set_one_shoot_dispersion_amount@...@@QAEXM@Z 100.0
     ?set_reload_dispersion_amount@...@@QAEXM@Z  100.0
     (?get_value@...@@QBEMXZ still 100.0 - inherited scaffolding not regressed)
   report-changes.json: +2 functions matched, 19 regressed / 21 improved - all
   regressions in UNRELATED modules (btGhostPairCallback / Scaleform / vostok::ui /
   vostok::render / vostok::threading / vostok::collision / vostok::memory /
   vostok::network / boost / buffer_string `scalar deleting destructor' & template
   helpers). These are stale-baseline / COMDAT-fold non-determinism artifacts (many
   of the same destructor names also appear in the "improved" list); my git diff
   touches only weapon_dispersion_calculator.cpp + the one anchor block, so none of
   the regressions are mine (see loop_performance.md baseline-artifact note).
   DIFF:  `--view diff` refused ("needs both indexes") because --function matches
   >1 index entry (dispersion_calculator sibling / caller-in-callees); report.json's
   100.0 is byte-exact and authoritative (loop_performance.md note).

## Outcome
STATE[100%|DONE] for all three setters. The anchor's address-table + &calc escape
through example_callback is what kept the stores observed; without it the trivial
setters would have dead-store-eliminated (the README "constant-only ctor" elision).
Single rebuild, no iteration needed - offsets read from asm up front mapped 1:1 to
the argument-named members. Regressions caused: none (unrelated stale-baseline
artifacts only). Inlining: none; standalone bodies kept via address-of in anchor.
