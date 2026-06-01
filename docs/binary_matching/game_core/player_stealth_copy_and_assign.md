# game_core::player_stealth::{player_stealth(const&), operator=}

Matched as ONE group (two trivial same-class members sharing scaffolding).

## Target asm

### copy ctor  survarium::player_stealth::player_stealth(player_stealth const&)  @ 0x59aab0
```
push ebp; mov ebp,esp; push ecx; mov [ebp-4],ecx
mov eax,[ebp+8]            ; other
push eax
mov ecx,[ebp-4]           ; this
call operator=
mov eax,[ebp-4]           ; return this
mov esp,ebp; pop ebp; ret 4
```
=> body is `*this = other;` (delegate to operator=); ctor returns this implicitly.

### operator=  survarium::player_stealth& operator=(player_stealth const&)  @ 0x59aa10
```
push ebp; mov ebp,esp; push ecx; mov [ebp-4],ecx
mov eax,[ebp-4]; cmp eax,[ebp+8]; je .1     ; if ( this != &other )
  fld [edx]    ; fstp [ecx]      member @0x00
  fld [ecx+4]  ; fstp [eax+4]    member @0x04
  fld [+8]     ; fstp [+8]       member @0x08
  ... 0x0c,0x10,0x14,0x18,0x1c,0x20,0x24,0x28   (11 floats total, decl order)
.1:
mov eax,[ebp-4]   ; return *this
mov esp,ebp; pop ebp; ret 4
```
11 float members copied in declaration/offset order. Self-assignment guard present.

Identical shape to the already-100% `player_stamina::operator=` and its `*this = other`
copy ctor (sources/vostok/game_core/sources/player_stamina.cpp).

## Commands run (verbatim, in order)
- pdb_rich_query --index binaries/rich/target/index.jsonl --function player_stealth --list
- pdb_fetch --target-index binaries/rich/target/index.jsonl --function player_stealth --rva 0x59aab0 --view target
- pdb_fetch --target-index binaries/rich/target/index.jsonl --function player_stealth --rva 0x59aa10 --view target

## Iterations
1. INPUT: copy ctor `*this = other;`; operator= self-guard + 11 member copies in decl
   order + `return *this`. Anchored in temp_include_all use_game_core_player_stealth():
   default-construct, load(cfg), copy-construct stealth2(stealth), escape &stealth and
   &stealth2 through example_callback opaque sink so dest stores are observed.
   BUILD: report.json fuzzy_match_percent for BOTH at 100.0% on the FIRST rebuild.
     ??4player_stealth@survarium@@QAEAAV01@ABV01@@Z  (operator=)   = 100.0
     ??0player_stealth@survarium@@QAE@ABV01@@Z       (copy ctor)  = 100.0
   (Bonus: the default ctor ??0...@XZ also went 0 -> 100 once anchored; its empty
    body already matched the target's empty default ctor. Left as a separate unit.)

   Rebuild commands:
   - nix develop --command bash -c 'python3 scripts/rebuild.py'   # bare, full EXE relink

## report-changes.json triage (stale-baseline artifacts, NOT my regressions)
61 functions show 100 -> 0 in report-changes.json. My git diff touches ONLY
player_stealth.cpp + the temp_include_all anchor. The regressed list is entirely
functions whose SCAFFOLDING is not committed on this branch:
- `weapon_dispersion_calculator::{set_*, get_value}` - HEAD commit d0599dbc claims
  these matched (PR #113), but `git show HEAD:.../temp_include_all.cpp | grep
  weapon_dispersion_calculator` = 0 hits: the source/anchor sits on PR #113's
  unmerged branch, only the ledger was forward-ported. The previous report.json was
  generated on a tree that had that branch's scaffolding present.
- scalar-deleting-dtors, boost::_bi storage helpers, float3/float4/float4x4 ctors,
  medkit overrides, btSoftBody/btPairSet dtors - same: reachable only via other
  branches' scaffolding, dropped to 0 on a clean build of THIS branch's committed source.
Per loop_performance.md "Distinguish real regressions from baseline-artifact
regressions": (1) my diff does not touch any of them; (2) they are unanchored on the
committed tree. => stale-baseline artifacts, not regressions I caused.

## Outcome
STATE[100%|DONE] for both copy ctor and operator=. Matched byte-exact on first rebuild.
Anchoring: use_game_core_player_stealth() default-constructs, copy-constructs (exercises
copy ctor -> operator=), does a direct `stealth2 = stealth` (exercises operator=), and
escapes &stealth/&stealth2 through example_callback (opaque sink) so LTCG keeps the
member stores observable. Regressions caused: none (the 61 in report-changes are
stale-baseline artifacts from unmerged sibling branches). Inlining: none relevant.
