# damage_zone_core shape-distance helpers

Unit: the four free geometric helpers in `damage_zone_core.cpp`, plus the virtual
`dz_bone_data_contact_test_predicate::add_single_result` that dispatches to them.

- `survarium::distance_from_sphere_center_to_point_on_shape(float)` (rva 0x587d50)
- `survarium::distance_from_box_center_to_point_on_shape(float4x4 const&, float3 const&, float3 const&)` (rva 0x5884d0)
- `survarium::distance_from_capsule_center_to_point_on_shape(float4x4 const&, float, float, float3 const&)` (rva 0x5882a0)
- `survarium::distance_from_cylinder_center_to_point_on_shape(float4x4 const&, float, float, float3 const&)` (rva 0x588040)
- `dz_bone_data_contact_test_predicate::add_single_result` (rva 0xb7be0)

## Commands
```
pdb_rich_query --index binaries/rich/target/index.jsonl --list | grep distance_from
pdb_rich_query --index binaries/rich/target/index.jsonl --rva 0x5884d0   # box
pdb_rich_query --index binaries/rich/target/index.jsonl --rva 0x5882a0   # capsule
pdb_rich_query --index binaries/rich/target/index.jsonl --rva 0x588040   # cylinder
pdb_rich_query --index binaries/rich/target/index.jsonl --rva 0x0b7be0   # add_single_result
pdb_fetch --target-index binaries/rich/target/index.jsonl --base-index binaries/rich/base/index.jsonl --function distance_from_box_center --view diff
```

## Key facts learned
- `float4x4` (math_float4x4.h) is a union of `{ float4_pod i,j,k,c }` / `lines[4]`.
  Offset `+0x10` = `j`, `+0x30` = `c`. The delinker misnames the inlined
  `float4_pod::xyz() const` accessor (returns `float3 const&`) as
  `fixed_size_allocator<...>::finalize_impl` (target) or
  `boost::function1<...>::dummy::nonnull` (base) - both are the same inlined
  accessor folded into different COMDATs. So `transform.c.xyz()` ->
  `mov eax,[ebp+8]; add eax,0x30; call <xyz>`.
- `transform.lines[i].xyz()` indexes a matrix row.

## Anchoring
These are free functions with external linkage but no header declaration. Anchored
in `temp_include_all.cpp`: forward-declared in the `namespace survarium` block and
their addresses taken via `volatile` function pointers in `use_damage_zone_core()`.
After anchoring, the base PDB has them (rich index rva 0x45eae0 for box) and
`pdb_fetch --view diff` works.

## NOTE on report.json plumbing
report.json shows these free functions with NO `fuzzy_match_percent` (listed as
target-only). The delinked COFF base obj contains the mangled symbols
(`?distance_from_box_center...@@YAM...`) but objdiff isn't pairing them with the
target's demangled entry. The authoritative signal here is therefore the
`pdb_fetch --view diff` percentage (rich-index based), not report.json.

## box - per-iteration
- v1 `axis.dot_product(dir)`: 77.9% (88/113). Diffs: (a) the 3 `xyz()` accessor
  calls resolve to differently-named folded thunks (cosmetic delinker naming,
  byte-identical call site); (b) base INLINED `axis.dot_product(dir)` to scalar
  `mulss` while target kept a real `call float3_pod::dot_product` @0x9b; (c) the
  tail `( result - transform.c.xyz() ).length()` `operator-` swaps its two register
  operands (base `mov ecx,eax; lea edx,[ebp-18h]` vs target `lea ecx,[ebp-18h]; mov
  edx,eax`) - a call-boundary arg-passing diff, NOT a frame-size cascade (box frame
  is 5Ch on BOTH sides; the single inline reuses existing slots and does not grow
  it - corrected in review, the box rationale earlier said "size delta").
- v2 `dir.dot_product(axis)`: 77.9%, same. Call object does NOT steer the inline.

## ROOT CAUSE (claude, 2026-06-06): /Od /Ob2 /Oi /Oy /GL whole-program inline wall
`damage_zone_core.cpp` compile flags (from `binaries/ninja/rsp/game_core_cl_1.rsp`):
`/Od /Ob2 /Oi /Oy /GL`. `/Ob2` lets the compiler inline ANY suitable function (not
just `inline`-marked), and `/GL` makes inline-expansion a *whole-program* link-time
(LTCG) decision. Our base sees only a partial program, so it inlines the tiny
`inline float3_pod::dot_product` (math_float3_inline.h:118, body `x*o.x+y*o.y+z*o.z`)
right at each call site as scalar `mulss/addss`. The shipped target's full-program
LTCG link instead kept `dot_product` OUT-OF-LINE (a real `call`).

Proof: the standalone `float3_pod::dot_product` differs structurally between sides:
```
pdb_fetch ... --function "float3_pod::dot_product" --view diff   # 0/30 (0.0%)
  base  (rva 0x36d10): /Od thiscall, push ebp; x87 fld/fmul; ret 4
  target(rva 0x08130): register args eax/ecx, SSE movss/mulss, ret  (optimized)
```
The target call sites pass args in `eax`/`ecx` (the optimized convention), confirming
the target dispatches to the optimized out-of-line copy. Our base both emits a /Od x87
standalone copy AND inlines the body where /Ob2 finds it cheap.

Consequence cascade (why % is low, not just a couple of lines): each inlined
dot_product spills an extra `[ebp-0Fxh]` temp, which (1) grows the frame
(`sub esp,0F8h` base vs `0F4h` target for capsule; `10Ch` vs `108h` cylinder) and
(2) shifts every subsequent stack slot, which (3) re-orders the `ecx`/`edx` operand
passing of the downstream `operator-` calls. All of it is downstream of the inline.

This is the SAME documented wall as `weapon_core_reload_state` /
`pistol_weapon_core_reload_state` (PROGRESS.md): under `/Ob2 /GL`, base inlines
in-class/inline-marked helpers the target kept out-of-line. It is NOT a call-argument
LTCG diff (no arg dropped/reg-substituted at a boundary) and is NOT de-inlinable from
this TU - the only lever would be `#pragma auto_inline(off)`, which the original
source did not use (it would change which functions inline program-wide, not match the
target's specific link-time choice). Classified PARTIAL / proven build-config wall.

## report.json pairing (unchanged, confirmed)
The four free functions are listed in report.json with NO `fuzzy_match_percent`
(target-only, `-1.00`). objdiff pairs by exact symbol-name string; the TARGET obj
records them DEMANGLED (`survarium::distance_from_box_center_to_point_on_shape`) while
the MSVC-built BASE obj records the decorated name
(`?distance_from_box_center_to_point_on_shape@survarium@@YAMABVfloat4x4@math@vostok@@ABVfloat3@34@1@Z`).
Verified with `strings` on both objs. No `target-symbol-map.tsv` aliasing exists for
free functions (the map only reconciles folded COMDAT groups). So the authoritative %
for these four is `pdb_fetch --view diff`, not report.json.
