# weapon_recoil_params::weapon_recoil_params()  (default, no-arg ctor)

- Module: game_core
- Source: `sources/vostok/game_core/sources/weapon_recoil_params.cpp:12`
- Target RVA: `0x5830c0` (target index); FUNCTION-BODY comments show `0x5930c0`
  (pdb-recorded VA, different image base).

## History
- PR #107 (commit `1194b2d3`): body matched the target 1:1 but scored only
  **18.18%** because the anchor instantiated-and-discarded the object, so LTCG
  dead-store-eliminated all 12 member stores -> base ctor compiled empty.
- This retry (branch off `feature/agentic-matching-loop`): keep the proven body,
  fix ONLY the anchor (observed instance escaped through the opaque sink), per the
  revised README "Constant-only ctors" note + loop_performance "trivial setter".

## Commands run
```
nix develop --command pdb_rich_query --index binaries/rich/target/index.jsonl \
    --function weapon_recoil_params --list
# -> 0x5830c0 default ctor, 0x583190 binary_config_value ctor

nix develop --command pdb_fetch --target-index binaries/rich/target/index.jsonl \
    --view target --rva 0x5830c0
```

## Target asm (default ctor), member stores
12 `movss [this+off], xmm0` stores, all loading a float constant from rdata:
```
off 0x00 0x04 0x08 0x0C 0x10 0x14 0x18 0x1C   then  0x24 0x28 0x2C 0x30
```
Offset **0x20 (`additive_back_recoil`) is NEVER stored** (no `movss [this+20h]`).
Offset 0x18 (`additive_recoil_time`) loads the symbol the delinker named
`epsilon_3` = `math::epsilon_3` = `.001f`; every other store is `0.0f`.

This maps 1:1 to a declaration-order member-init list, all `0.0f` except
`additive_recoil_time( math::epsilon_3 )`, with `additive_back_recoil` omitted.
(Body unchanged from #107 - the disassembly confirms it was already correct, so
per the #1 rule it is NOT "fixed".)

## The fix (anchor only)
`temp_include_all.cpp`:
- `#include <vostok/game_core/weapon_recoil_params.h>`
- new `use_game_core_weapon_recoil_params()`:
  ```cpp
  survarium::weapon_recoil_params params;
  example_callback( reinterpret_cast< pcstr >( &params ) );
  ```
- called from `IncludeAll::IncludeAll()`.
Same observed-escape idiom proven by weapon_state::operator= (commit `4ffc36d8`).

## Rebuild / score
```
python3 scripts/rebuild.py        # bare name (no module arg) = full relink, fresh score
```
- fuzzy_match_percent (report.json): **100.0%** for
  `??0weapon_recoil_params@survarium@@QAE@XZ`
  (report-changes.json: `weapon_recoil_params(void)` 18.181818 -> 100.0).
  The binary_config_value ctor stays `None` (still STUB, out of scope).
- delink summary: code 27.07% -> 27.10% (+0.03), functions 8456 -> 8474 (+18).

### Regressions: none real (delinker symbol-reassignment churn)
report-changes.json lists 53 regressed / 61 improved. Adding a function to the
shared `temp_include_all.cpp` TU shifts COFF section layout, so the delinker
reassigns which object file owns each *byte-identical compiler-generated helper*
(scalar/vector deleting destructors, vtable `vcall`/thunk symbols, trivial inline
ctors like `size_policy`/`intrusive_ptr`/`resource_ptr`/`allocator<char>`/boost
`storage*`, STL locale destructors, bullet/Scaleform dtors). report-changes
matches by demangled name across two delinks, so the same body shows as both
regressed (in unit A) and improved (in unit B).

Every "regressed but not also improved" entry is one of those helpers - NOT a
hand-matched source function. In particular `weapon_state::operator=` and
`weapon_state::weapon_state(void)` are **STUBs on this branch** (empty bodies,
score `None` in report.json); their "100 -> 0" is the demangled name re-binding
to a different unit, not a real source regression. Net is +0.03% code, +18
functions, and my target +81.8 pp. No matched source dropped.
</content>
</invoke>
