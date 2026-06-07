# booby_trap_core.cpp - structure verification + fixes

Structure-verifier pass over the matched-but-<100% functions in
`sources/vostok/game_core/sources/booby_trap_core.cpp`. Diagnosed each with
`pdb_fetch --view structure-diff --condensed` then `--view diff`. report.json
numbers below are post-rebuild.

## Summary of report.json (markers were stale; corrected in-source)

| function       | old marker | report.json | new STATE | structure |
|----------------|-----------|-------------|-----------|-----------|
| ctor           | 94.90%    | 100.0%      | 100% DONE | exact (1/1) |
| ~dtor          | 14.86%    | 14.86%      | 14.86% PARTIAL | MATCH, SIZE wall |
| load           | 84.87%    | 90.68%      | 90.68% PARTIAL | MATCH, SIZE wall |
| set_transform  | 97.38%    | 97.38%      | 97.38% PARTIAL | MATCH, SIZE wall |
| apply_damage   | 99.85%    | 83.06%      | 83.06% PARTIAL | MATCH, SIZE+EMPTY wall |
| on_enter       | 98.75%    | 91.77%      | 91.77% PARTIAL | MATCH, SIZE wall |
| insert         | 97.44%    | 97.44%      | 97.44% PARTIAL | MATCH, SIZE wall |
| use_initialize | 99.84%    | 99.84%      | 99.84% PARTIAL | MATCH exact (0/0 diffs) |
| use_execute    | 97.35%    | 82.75%      | 82.75% PARTIAL | MATCH, SIZE wall |
| can_defuse     | 95.27%    | 81.57->86.04% | 86.04% PARTIAL | MATCH, SIZE wall (FIXED ternary) |

The markers were broadly stale (a prior delinker/rebuild shifted several down).
All ten functions are STRUCTURE MATCH: same statement count and shape on both
sides, with one recurring non-steerable cause.

## The one fix (source-steerable): can_defuse return ternary

Original source:
```cpp
return user != owner ? user->team( ) != owner->team( ) : true;
```
emitted `xor ecx,ecx; cmp esi,eax; setne cl` for the inner `!=`. The target
instead uses a branch: `je .1` (user==owner) and `jne .1` (teams !=) both jump
to a shared `mov [slot],1`, fall-through `mov [slot],0`. That is the
short-circuit `||` shape:
```cpp
return user == owner || user->team( ) != owner->team( );
```
Truth value identical, codegen now branch-based. Lifted can_defuse 81.57% ->
86.04%. The residual SIZE on the `holder()` line is the wall below.

## The recurring wall (non-steerable): inline-vs-call of header `inline` helpers

Every remaining SIZE diff is the same phenomenon: the TARGET emits an
out-of-line `call` to a function declared `inline` in a header, while our base
INLINES it. This is an LTCG cost-based inline-vs-call decision at link time, not
source-steerable from the call site (the source statement is identical and the
statement count matches). Per call site:

- ~dtor: `delete_helper<doug_lea_allocator,collision_geometry>` out-of-line
  (pointer in edi) vs the whole null-check+free_impl inlined. Dominates the body,
  hence the low 14.86%. (Same class noted in `bullet_manager.cpp::free_bullet`.)
- load: `new_helper<collision_geometry>::call<doug_lea_allocator>` out-of-line vs
  `malloc_impl` inlined one layer deeper (sushi's original note). x2 sites.
- apply_damage: `collision::bone_collision_data::bone_collision_data(pcstr, _, body_part)`
  ctor out-of-line (name in edx, this in esi) vs its two `fixed_string` members
  built inline. The `EMPTY only target` <0> is the ctor call as its own stmt
  boundary.
- on_enter / can_defuse: `inventory::holder()` out-of-line vs inlined `[+0x158]`
  member read.
- set_transform / insert / use_execute: `booby_trap_set_core::config()`
  out-of-line vs inlined member read (sushi's "config didn't inline" note).
- use_execute also: `math::min<u32>` out-of-line vs inlined branchless
  `sbb/neg/neg/and/add` min.

use_initialize is a structural EXACT match (0 size-diffs, 0 quantity-diffs); its
99.84% residual is pure register/stack-slot LTCG noise.

ctor is a clean 100% (1/1 stmt) - marker corrected, carcass deleted, bare
`// STATE[100%|DONE]`.

## Commands used
```
pdb_fetch --target-index binaries/rich/target/index.jsonl \
  --base-index binaries/rich/base/index.jsonl \
  --function booby_trap_core::<fn> [--rva 0x..] \
  --view structure-diff --condensed \
  --objdiff-base-dir binaries/objdiff/base --objdiff-target-dir binaries/objdiff/target
# then --view diff at the divergent statement.
```
Target rvas: ctor 0x58bd90, ~dtor 0x58be80, load 0x58bf60, set_transform
0x58b3c0, apply_damage 0x58b9c0, on_enter 0x58bb10, insert 0x58ba60,
use_initialize 0x58b5c0, use_execute 0x58b460, can_defuse 0x58b270.
