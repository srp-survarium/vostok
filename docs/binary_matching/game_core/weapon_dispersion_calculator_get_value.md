# game_core::weapon_dispersion_calculator::get_value

`float survarium::weapon_dispersion_calculator::get_value() const`
Target RVA: 0x57f820 (from `--list`).

## Target asm (pdb_fetch --view target)

```
0x00:    push  ebp        ; <0x7>
0x01:    mov   ebp, esp
0x03:    push  ecx
0x04:    mov   [ebp-4], ecx
0x07:    mov   eax, [ebp-4]   ; <0x6>
0x0a:    fld   st, dword ptr [eax+18h]
0x0d:    mov   esp, ebp      ; <0x4>
0x0f:    pop   ebp
0x10:    ret
```

Trivial getter: loads `dword ptr [this+0x18]` into st0 and returns it.
Member at offset 0x18 in `weapon_dispersion_calculator.h` is `m_current_coeff`.
So body = `return m_current_coeff;`.

This is the standard `/Od` getter prologue (push ebp / mov ebp,esp / push ecx /
mov [ebp-4],ecx ; this -> reload ; fld member). No inlining, no callees.

## Commands run (verbatim, in order)
- nix develop --command pdb_rich_query --index binaries/rich/target/index.jsonl --function get_value --list   (filtered to dispersion: 0x57f820)
- nix develop --command pdb_fetch --target-index binaries/rich/target/index.jsonl --function get_value --rva 0x57f820 --view target
- nix develop --command pdb_fetch ... --view diff  -> EMPTY (base symbol absent)
- report.json: fuzzy_match_percent = None for ?get_value@weapon_dispersion_calculator@survarium@@QBEMXZ
- COFF symbol dump of base/target weapon_dispersion_calculator.cpp.obj:
  base obj = 228 bytes, 1 symbol (file dead-stripped: no anchor in temp_include_all);
  target obj = 2030 bytes. Root cause of `None` = whole class is dead-stripped, not
  a mangling mismatch. Target mangling is QBE (public const) and the header already
  declares `float get_value() const;` public, so no access-specifier fix needed.

## Plan
1. Body: `return m_current_coeff;` (member @ 0x18).
2. Anchor: add `use_weapon_dispersion_calculator()` to temp_include_all.cpp +
   `#include <vostok/game_core/weapon_dispersion_calculator.h>`, call it from
   IncludeAll::IncludeAll(). Instantiate the class and call get_value() so the
   symbol survives the linker.

## Iterations

1. INPUT:
   - weapon_dispersion_calculator.cpp: `get_value()` body -> `return m_current_coeff;`
   - temp_include_all.cpp: `#include <vostok/game_core/weapon_dispersion_calculator.h>`,
     new `use_weapon_dispersion_calculator()` { instantiate + call get_value() },
     call it from IncludeAll::IncludeAll().
   BUILD: get_value -> 100.0% (report.json: ?get_value@weapon_dispersion_calculator@survarium@@QBEMXZ).
   Constructor became reachable via the anchor instantiation -> 25.8% (constant-only
   ctor under /Od+LTCG, expected low PARTIAL; left STUB, not this unit's work).
   DIFF:  n/a - already 100%, no diff needed.

## Outcome
STATE[100%|DONE]: `return m_current_coeff;` (member @ this+0x18) matches the target
`fld dword ptr [eax+18h]` getter byte-for-byte. No callees, no inlining.

Regressions caused: NONE attributable to this change. report-changes.json lists 49
regressed / 54 improved, but:
- The bulk (31 `scalar deleting destructor`, 3 dtors, ctors, thunks, `empty_stub`,
  intrusive_ptr/resource_ptr ctors) is the standard LTCG relink address-reshuffle
  churn that flips 100<->0 on every full relink; net code match rose +0.02% / +26
  functions.
- 4 `character_dispersion_calculator` entries (get_target_koef 88->0, get_broken_hands_penalty
  82->0, tick 25->0, set_character_dispersion_params 22->0) are a BASELINE ARTIFACT,
  not caused by this change: on `feature/agentic-matching-loop` HEAD those functions
  are all `STATE[STUB]` returning 0.0f and are NOT anchored in temp_include_all (verified
  via `git show HEAD:...`). Their 88%/82% in the previous report.json came from a build
  that had the open sibling PR branches' scaffolding merged in; my clean full rebuild on
  this branch produced the branch's true baseline where they are unreachable. My edits
  only touch weapon_dispersion_calculator.cpp + a get_value-only anchor; they do not
  reference character_dispersion_calculator at all.
