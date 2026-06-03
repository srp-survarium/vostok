# weapon_core_shotgun_reload_state_cook (cook/factory methods)

Unit: a batch of the simpler `weapon_core_shotgun_reload_state_cook` methods in
`sources/vostok/game_core/sources/weapon_core_shotgun_reload_state_cook.cpp`:
the destructor, `destroy_resource`, `deallocate_resource`, `allocate_resource`.
The heavy ones (`create_resource`, `on_subresources_ready`) are left STUB for a
later unit. Sibling of `weapon_core_cook` (PR #173) - same cook/factory shape.

## Setup / commands

Worktree: `/home/sheep/Projects/surv/vostok_3`. Branch off `fc3aadf9` (NOT a prior
match branch - non-stacked parallel setup).

```
git checkout -b match/game_core-shotgun_reload_cook fc3aadf9760101959c8bfda70566ba7b528c021b
nix develop -c pdb_rich_query --index binaries/rich/target/index.jsonl --function "weapon_core_shotgun_reload_state_cook" --list
nix develop -c pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x580150 --view target   # ~dtor
nix develop -c pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x580170 --view target   # deallocate_resource
nix develop -c pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x5801a0 --view target   # allocate_resource
nix develop -c pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x580110 --view target   # destroy_resource
```

NOTE: the carcass addresses in the `.cpp` (0x590xxx) are stale; the live target
RVAs are 0x580xxx. Offsets/structure in the carcass are still valid.

## Visibility (mangled access chars from target symbol list)
- `~weapon_core_shotgun_reload_state_cook ...@@UAE` = public virtual.
- `allocate_resource ...@@UAE` = public virtual.
- `deallocate_resource ...@@UAE` = public virtual.
- `create_resource ...@@UAE` = public virtual.
- `destroy_resource ...@@UAE` = public virtual.
- `on_subresources_ready ...@@AAE` = PRIVATE (header had it under `public:`;
  moved to `private:`).

The override signatures match `unmanaged_cook` (allocate takes `const_buffer, bool`),
so the header base stays `resources::unmanaged_cook`. The target dtor calls
`inplace_unmanaged_cook::~inplace_unmanaged_cook` - a COMDAT-folded identical
base destructor; keeping `unmanaged_cook` matched the dtor 100%.

## Anchoring
The cook was dead-stripped from base. Added
`use_weapon_core_shotgun_reload_state_cook()` to
`game_core/sources/temp_include_all.cpp` (construct a static + `register_cook`),
mirroring `use_victory_item_core`. Construction emits the vtable -> anchors all
the virtuals (dtor, allocate, deallocate, create, destroy).

## Results
- `~weapon_core_shotgun_reload_state_cook`: empty body `{}`. 100%.
- `destroy_resource`: `weapon_core_shotgun_reload_state* wpn_state =
  static_cast<...>(resource); wpn_state->~weapon_core_shotgun_reload_state();`.
  The `static_cast` from `unmanaged_resource*` adjusts -0x18 (the resource base
  subobject offset) with the null-checked downcast; the dtor call dispatches the
  virtual deleting-dtor vtable slot with flag 0. 100%.
- `deallocate_resource`: `VOSTOK_FREE_IMPL( g_allocator, (resources::resource_base*&)buffer )`.
  78.89% fuzzy / 55.6% code. Allocator is the `g_allocator` POINTER (no deref):
  target does `mov eax,[g_allocator]` (matches #173 weapon_core_cook learning).
  The reference-cast `(resource_base*&)buffer` gives the `free_helper(A&, T*&)`
  overload, T=resource_base, with `lea edi,[ebp+8]`=&buffer. RESIDUAL: target
  calls `free_helper<doug_lea_allocator,resource_base>` OUT-OF-LINE; base INLINES
  the wrapper (inlines the `if(!pointer) return` null check + calls
  free_helper_impl directly), spilling the allocator to an extra [ebp-4] temp
  (`sub esp,8` vs target `push ecx`). Inline-depth divergence - same class as
  weapon_core_cook::delete_resource (33% PARTIAL) and the other cook deallocates
  (inventory_cook/ladder_cook sit at 65-92%).
- `allocate_resource`: 37% fuzzy / 41% code. PARTIAL on two statements:
  - Statement 36 (malloc + return): shape correct
    `return mutable_buffer( VOSTOK_MALLOC_IMPL(g_allocator, sizeof(weapon_core_shotgun_reload_state)=0x140, ...), 0x140 )`.
    Target inlines the `mutable_buffer(pvoid,u32)` ctor as two field stores
    (`[edx]=ptr; [ecx+4]=0x140`) into the sret; base out-of-lines it to a
    COMDAT-folded `uint2::uint2` call. Same inline divergence.
  - Statement 35 (the ASSERT): NOT recovered. Target emits
    `bool x=false; ASSERT_check(&x); if(x){ push file_exist, raw_file_data.size,
    raw_file_data.ptr; copy in_query (96h dwords = 600 bytes) by value; ASSERT_check }`.
    This is a real assertion whose condition calls a validator taking `in_query`
    (query_result_for_cook) BY VALUE plus raw_file_data + file_exist. The exact
    UNKNOWN_EXPRESSION is unknown; only `ASSERT( UNKNOWN_EXPRESSION )` (the
    compiled-out first half) is emitted. NEXT STEP: identify the cook-input
    validator that takes query_result_for_cook by value (sibling
    weapon_core_inactive_state_cook::allocate at target 0x58ef60 has the
    byte-identical shape, only malloc size 0x138 differs - solving one solves both).

## Regressions
report-changes.json shows ~23 functions `100->0` (weapon_core_cook::*, empty_stub,
interlocked_*, etc.). These are NOT caused by this change: the worktree was
previously at b9f30c48 (PR #173, weapon_core_cook anchored); branching from
fc3aadf9 drops that anchor, so those symbols vanish from the base build. My diff
only touches the shotgun cook files + the temp_include_all anchor.
