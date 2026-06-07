# player_parameters_cook unit

Survey + wall analysis. Branch `match/game_core-player_parameters_cook` off
`origin/int/game_core`. Unit already enabled/anchored before this work.

## Authoritative state (report.json fuzzy_match_percent, then re-checked via objdiff diff)

| function | report.json | objdiff diff | verdict |
|---|---|---|---|
| `get_booster_value` | 100.0 | - | DONE (carcass removed) |
| `player_parameters_modifyer_cook::ctor` | 100.0 | - | DONE (carcass removed) |
| `player_parameters_modifyer` default ctor (header unit) | 100.0 | - | DONE |
| `delete_resource` | 31.0 | 49.75 | PARTIAL/LTCG wall |
| `apply` | 85.01 | 81.10 | PARTIAL wall (copy-elision + order) |
| `translate_query` | 94.92 | 88.13 | PARTIAL wall (config-read copy-elision) |
| map `operator=` (header unit) | 0 | unpaired | compiler-generated, not source-steerable |
| `??_Gplayer_parameters_modifyer` scalar deleting dtor (header unit) | 0 | unpaired | compiler-generated thunk, attributed to a different TU |

The source `STATE[]` markers were stale (BLOCKED, 66.69%, 84.21%, 89.62%, 31.00%);
updated to the authoritative report.json/objdiff numbers.

## Commands
```
pdb_fetch --target-index binaries/rich/target/index.jsonl --base-index binaries/rich/base/index.jsonl \
  --function "<demangled>" --view diff --objdiff-base-dir binaries/objdiff/base --objdiff-target-dir binaries/objdiff/target
# (pdb_fetch --function matches the DEMANGLED name; "ns::class::fn" works, mangled "?fn@..." does not)
grep -n '?<mangled>' binaries/rich/{target,base}/index.jsonl   # raw instruction stream by mangled name
```

## delete_resource (49.75%, LTCG call-boundary wall)
One-liner `VOSTOK_DELETE_IMPL( g_allocator, resource )`.
- Target: `lea eax,[ebp+8]; push eax; mov eax,[g_allocator]; call <stub>; push eax; call delete_helper; add esp,8`
  -> pushes `&resource` (the `T*&` of the non-managed `delete_helper(A&, T*& pointer, ...)`
  overload) on the stack, evaluated first, and does NOT save edi.
- Base: `mov eax,[g_allocator]; call <stub>; push eax; lea edi,[ebp+8]; call delete_helper; add esp,4`
  -> passes `&resource` in register edi (saves/restores edi).
Same call target, same ASSERT stub, same source. Difference is purely whether the `T*&`
arg lands on the stack or in a register = LTCG call-boundary scheduling. Acceptable wall.

## apply (81.10%, copy-elision + documented order divergence)
- First statement `damage_model_ptr damage_model = player->damage_model()` copy-constructs an
  intrusive_ptr from a `damage_model_ptr const&` getter. Target calls the copy-ctor
  OUT OF LINE (`call 997h`, no `[ebp-4]=0` pre-zero); base INLINES it (`mov [ebp-4],0; ...; call F7h`).
  That inlining decision shifts the frame by 8 bytes (0x224 base vs 0x22C target) -> a
  whole-function `[ebp-XX]` slot cascade (e.g. `[ebp-178h]`<->`[ebp-180h]`, etc).
- Tail divergence: real `+`/`-` rows in the `add_damage_protector` loop (target-only
  `movss [ecx+24h]`, `call C46h/E8Bh/FF4h/102Ch`) - the prior author already flagged this as
  the biggest diff (sushi@MATCH "the order of things is different").
Statement skeleton otherwise pairs 1:1. Copy-elision + ordering wall; not a quick win.

## translate_query (88.13%, confirmed config-read copy-elision wall)
The documented ~91% cook config-read wall class. Statement skeleton pairs 1:1 with the
carcass. Residual is register allocation: target enregisters values into `esi`/`edi`
(`push esi; push edi` at top, used throughout) where base spills to stack, plus a frame-size
delta (54Ch base vs 4A4h target) producing a stack-slot cascade and a `[ebp-8]`<->`[ebp-0Ch]`
local swap. Not source-steerable.

## Header-unit 0% functions (not source-steerable here)
- `is_ammo@dictionary_item` (checked as an alternative): target is in `items_dictionary_cook.h`
  and is OPTIMIZED codegen (35 bytes, no prologue, `mov al,[eax+118h]`); base is `/Od`
  (107 bytes) in `dictionary_item.h` -> different TU + different flags -> unpaired (0%). A
  COMDAT/flags issue owned by the items_dictionary work, not this unit.
- The map `operator=` and `??_G` scalar deleting dtor are compiler-generated and attributed
  to a different TU than the base emits them in -> unpaired. Not source code we can steer.

## Conclusion
Unit is entirely wall-bound: 3 functions at 100%, the rest at genuine LTCG /
copy-elision / register-allocation walls. No tractable byte-win remained. The committed
change is: stale STATE-marker corrections + carcass cleanup for the two clean 100% DONE
functions (get_booster_value, ctor) + documented wall rationale on the three PARTIALs.
