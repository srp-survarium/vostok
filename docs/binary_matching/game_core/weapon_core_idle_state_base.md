# game_core::weapon_core_idle_state_base::{ctor,initialize,finalize}

Group of three members of `survarium::weapon_core_idle_state_base`
(derives from `weapon_core_base_state` -> `ai::fsm_state` + `resources::unmanaged_resource`).

Target rvas: ctor 0x6ea680, initialize 0x6ea660, finalize 0x6ea640.

## Target asm

### ctor (0x6ea680)
```
push ebp; mov ebp,esp; push ecx; mov [ebp-4],ecx
push 0                                  ; serialize_animation_state = false
mov eax,[ebp+8]; push eax               ; weapon
mov ecx,[ebp-4]
call survarium::weapon_core_base_state::weapon_core_base_state   ; (weapon, false)
mov ecx,[ebp-4]; mov [ecx],   ??_7...@6Bfsm_state@ai@vostok@@      ; vtable @0x00
mov edx,[ebp-4]; mov [edx+18h],??_7...@6Bunmanaged_resource@...@   ; vtable @0x18
mov eax,[ebp-4]; mov esp,ebp; pop ebp; ret 4
```
=> base-ctor delegation (init list `weapon_core_base_state(weapon, false)`) + two
compiler-emitted vtable stores. Source body is empty.

### initialize (0x6ea660)
```
mov eax,[ebp-4]; mov ecx,[eax+128h]; call survarium::weapon_core::instant_idle_start
```
=> `m_weapon.instant_idle_start();` (m_weapon @ 0x128 in weapon_core_base_state).

### finalize (0x6ea640)
```
mov eax,[ebp-4]; mov ecx,[eax+128h]; call survarium::weapon_core::instant_idle_end
```
=> `m_weapon.instant_idle_end();`

## Commands run (verbatim, in order)
- pdb_rich_query --index binaries/rich/target/index.jsonl --function weapon_core_idle_state_base --list
- pdb_fetch --target-index ... --function weapon_core_idle_state_base --rva 0x6ea680 --view target/callees
- pdb_fetch --target-index ... --function initialize --rva 0x6ea660 --view target/callees
- pdb_fetch --target-index ... --function finalize  --rva 0x6ea640 --view target/callees
- git log --all / gh pr list  -> found PR #121 (match/game_core-weapon_core_base_state_ctor_deserializing)
  holds the fsm_state stopgap + base ctor + derived-stub anchor scaffolding I depend on.
  Branched off THAT branch (loop_performance.md "scaffolding in an open PR" rule), stacking on #121.
- python3 scripts/rebuild.py   # NO module arg
- pdb_fetch ... --view diff (per fn)

## Scaffolding inherited from PR #121 branch
- temp_include_all.cpp: `vostok::ai::fsm_state::~fsm_state(){}` stopgap (pure-virtual dtor, no body in sources).
- weapon_core_base_state.cpp ctor filled + access specifiers; type_definitions animation_playback_state ctor.

## Added this unit
- weapon_core_idle_state_base.cpp: ctor body empty; initialize = m_weapon.instant_idle_start();
  finalize = m_weapon.instant_idle_end(). Added `#include <vostok/game_core/weapon_core.h>`.
- temp_include_all.cpp: `use_game_core_weapon_core_idle_state_base` anchor - concrete derived
  stub (overrides pure weapon_and_hands_expression with VOSTOK_UNREACHABLE_CODE), construct,
  qualified non-virtual `initialize()`/`finalize()` calls, escape &state.

## Iterations
1. INPUT: bodies as above + anchor; ctor/initialize/finalize public (default access).
   BUILD: code 27.09% / functions 33.38%. All three present in base obj but
          fuzzy_match_percent = None (objdiff can't pair).
   DIFF:  COFF symbol dump - mangling mismatch (access specifier):
          ctor   base ??0...@@QAE (public)  vs target IAE (protected)
          init   base ?...@@UAE   (pub vrt) vs target EAE (private virtual)
          final  base ?...@@UAE   (pub vrt) vs target EAE (private virtual)
   FIX:   ctor -> protected:, initialize/finalize -> private:. Anchor's qualified
          non-virtual calls now need access -> befriended
          ::vostok::use_game_core_weapon_core_idle_state_base (friend decl, no bytes).
2. INPUT: ctor protected:, initialize/finalize private:, friend the anchor + forward-decl it.
   BUILD: code 27.10% / functions 33.39%. report-changes: 0 regressed, 3 improved.
          ctor 100%, initialize 100%, finalize 100%. Mangled names now match target
          (IAE ctor, EAEXXZ virtuals).
   DIFF:  none needed - all three exact.

## Outcome
STATE: all three DONE 100%. Blocker: none. Regressions caused: none (0 regressed, 3 improved).
Inlining: none relevant. The ctor body is just base-ctor delegation + the two
compiler-emitted vtable stores; initialize/finalize each are a single out-of-line
call to weapon_core::instant_idle_start/instant_idle_end on m_weapon (@0x128).

Key learning (access specifiers from the COFF symbol): the target ctor is PROTECTED
(IAE) and the initialize/finalize virtual OVERRIDES are PRIVATE (EAE), not public.
Reading the mangled access char from the target obj up front would have saved the
first rebuild. Stacked on PR #121 (branch match/game_core-weapon_core_base_state_ctor_deserializing)
to inherit the fsm_state::~fsm_state stopgap + weapon_core_base_state scaffolding.
