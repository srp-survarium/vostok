# game_core::weapon_recoil_params::weapon_recoil_params() (default ctor)

Target RVA: 0x5830c0 (the carcass comment said 0x5930c0; the rich index is
authoritative -> 0x5830c0). Size 205 bytes, 2 statements (L25 `{`, L27 `}`),
all stores attributed to L25 -> member-initializer list.

The body is 12 `movss xmm0,[rdata]; movss [this+off],xmm0` pairs - a default ctor
initializing scalar floats from the constant pool. Member offset 0x20
(`additive_back_recoil`) is NOT stored -> that member is absent from the init list.

## Commands run (verbatim, in order)
- nix develop --command pdb_rich_query --index binaries/rich/target/index.jsonl --function weapon_recoil_params --list
  -> 0x5830c0 default ctor, 0x583190 binary_config_value ctor (separate unit).
- nix develop --command pdb_fetch --target-index binaries/rich/target/index.jsonl --function weapon_recoil_params --rva 0x5830c0 --view target
- nix develop --command pdb_fetch ... --view info   (locals: 0)
- nix develop --command pdb_fetch ... --view structure   (2 statements: L25, L27)
- nix develop --command pdb_fetch ... --view diff --objdiff-base-dir binaries/objdiff/base --objdiff-target-dir binaries/objdiff/target
  (STUB body empty -> 7.69%; target shows 12 `movss xmm0,[0]` stores, operands masked)
- python3 COFF parser over binaries/objdiff/target/vostok/game_core/sources/weapon_recoil_params.cpp.obj
  -> dumped .rdata floats and .text relocations.

## Constant map (from .text relocs -> .rdata values)
The first 12 .text relocations (text+0x00e .. text+0x0bd) are the default ctor;
they reference sequential .rdata slots 0x04..0x30 (each /Od float literal gets its
own slot, no dedup). rdata 0x1c is the symbol `epsilon_3` (= vostok::math::epsilon_3
= .001f); all other slots are 0.0f.

| member off | member                       | rdata | value           |
|-----------:|------------------------------|------:|-----------------|
| 0x00       | first_shoot_side_recoil      | 0x04  | 0.0f            |
| 0x04       | shoot_side_recoil            | 0x08  | 0.0f            |
| 0x08       | first_shoot_back_recoil      | 0x0c  | 0.0f            |
| 0x0c       | shoot_back_recoil            | 0x10  | 0.0f            |
| 0x10       | shoot_recoil_min_angle       | 0x14  | 0.0f            |
| 0x14       | shoot_recoil_angle_range     | 0x18  | 0.0f            |
| 0x18       | additive_recoil_time         | 0x1c  | math::epsilon_3 |
| 0x1c       | additive_side_recoil         | 0x20  | 0.0f            |
| 0x20       | additive_back_recoil         |  --   | (NOT stored)    |
| 0x24       | additive_recoil_min_angle    | 0x24  | 0.0f            |
| 0x28       | additive_recoil_angle_range  | 0x28  | 0.0f            |
| 0x2c       | side_compensation_speed      | 0x2c  | 0.0f            |
| 0x30       | back_compensation_speed      | 0x30  | 0.0f            |

`math::epsilon_3` already compiles inside game_core (used by booby_trap_set_core.cpp,
medkit.cpp), so the pch chain exposes it - no missing type.

Reachability: `weapon_recoil_params` was not anchored. Added
`use_weapon_recoil_params()` to temp_include_all.cpp (default-constructs an
instance) and called it from IncludeAll::IncludeAll().

## Iterations (one block per source variant)
1. INPUT: member-init list in declaration order, all 0.0f except
   additive_recoil_time( math::epsilon_3 ), additive_back_recoil omitted.
   Anchored via use_weapon_recoil_params() that builds a local and discards it.
   BUILD: report.json fuzzy = 18.18%. (regressions: 0 regressed / 0 improved)
   DIFF:  base ctor body is EMPTY (.text = 55 8b ec 51 89 4d fc 8b 45 fc 8b e5
          5d c3 90 90 -> push/mov/push/mov/mov[ebp-4]/mov eax/leave/ret, no
          stores, no .rdata). All 12 target movss stores are target-only (`+`).
          Root cause: base is /Od + /GL (LTCG). The anchor constructed a local
          `params` and discarded it, so LTCG proved every member store dead and
          elided the whole ctor body. (game_core.vcproj Master Gold: Optimization=0
          but WholeProgramOptimization=1.) Target keeps the stores because it has
          real observers.
2. INPUT: change anchor to escape the object - example_callback(
          reinterpret_cast<pcstr>( &params ) ).
   BUILD: report.json fuzzy = 18.18% (unchanged). (regressions: 0/0)
   DIFF:  base ctor STILL empty (.text 16 bytes, identical hex). example_callback
          does printf("%s",name) and use_weapon_recoil_params itself was inlined
          away (no symbol in base index). Escaping &params to a %s-printf did not
          force the float stores to survive LTCG.
   NOTE:  animation_analysis_result::animation_analysis_result (85.98% DONE) is NOT
          anchored in temp_include_all.cpp at all - it survives via a REAL caller
          in reachable game code, which is what forces its stores past LTCG. The
          default weapon_recoil_params ctor has no such real caller in matched/
          reachable code, so any synthetic anchor lets LTCG elide the stores.
3. INPUT: anchor reads every member through a volatile float sink, then escapes
          the sink. volatile reads are observations MSVC must not elide.
   BUILD: FAILED - ninja build stopped on a compile error in temp_include_all.cpp
          (reverted before re-diff; rebuild ERROR'd so it did NOT regenerate diff
          inputs - base/report.json still reflect iteration 2). Likely the
          reinterpret_cast<pcstr> of a `volatile float*` dropping volatile, or the
          sink loop. Not worth another rebuild to fix: the volatile read still
          could not introduce a *real* consumer of the constructed object, so it
          would not have forced the ctor's own stores to survive anyway.
4. INPUT: revert anchor to the iteration-2 form (escape &params via
          example_callback) with a clearer NOTE; ctor body unchanged. This is
          byte-identical to iteration 2's compiled output (comment-only delta), so
          no rebuild is needed - the 18.18% stands.
   BUILD: 18.18% (carried from iteration 2; current source is byte-equal to it).
   DIFF:  unchanged - base ctor empty, all 12 target movss are target-only.

## Outcome
STATE[18.18%|PARTIAL]: The source is correct and complete - the member-init list
maps 1:1 to the target's 12 `movss [this+off], xmm0` stores, in declaration order,
with the right constants (all 0.0f except additive_recoil_time = math::epsilon_3),
and additive_back_recoil (offset 0x20) correctly omitted (the target never stores
it). The remaining diff is pure LTCG: under /Od + /GL (Master Gold,
Optimization=0 + WholeProgramOptimization=1) the linker's whole-program dead-store
elimination drops every member store because the only caller is the synthetic
temp_include_all anchor, which the optimizer sees through (escaping &params to a
%s-printf does not count as observing the float members). Real matched value-struct
ctors (e.g. animation_analysis_result, 85.98% DONE) survive only because a *real*
reachable game caller observes them - this ctor has no such caller in matched code.
Blocker: needs a real consumer of weapon_recoil_params() in reachable game_core
code (will arrive when its callers are matched). Regressions caused: none (0
regressed / 0 improved; build report code 27.09% unchanged).
Inlining: use_weapon_recoil_params itself is inlined away; the standalone ctor
symbol is kept but its body is LTCG-emptied.
