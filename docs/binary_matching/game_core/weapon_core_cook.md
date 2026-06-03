# weapon_core_cook (cook/factory methods)

Unit: a batch of the simpler `weapon_core_cook` methods in
`sources/vostok/game_core/sources/weapon_core_cook.cpp`:
ctor, `delete_resource`, `cooked_object_size`, `on_weapon_config_loaded`.
The heavy ones (`translate_query`, `load_weapon_parameters`,
`process_loading_weapon_core`, `on_core_subresources_ready`,
`query_weapon_states`, `on_weapon_states_ready`) are left STUB for a later unit.

## Setup / commands

Worktree: `/home/sheep/Projects/surv/vostok_3`. Branch off `fc3aadf9`.

Target index: `binaries/rich/target/index.jsonl`, base: `binaries/rich/base/index.jsonl`.

```
pdb_rich_query --index binaries/rich/target/index.jsonl --function "weapon_core_cook" --list
pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x58f0d0 --view target   # ctor
pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x58f0b0 --view target   # cooked_object_size
pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x58f120 --view target   # delete_resource
pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x5905c0 --view target   # on_weapon_config_loaded
```

## Visibility (mangled access chars from target symbol list)
- ctor `??0...@@QAE` = public.
- `delete_resource ...@@UAE` = public virtual.
- `translate_query ...@@UAE` = public virtual.
- `on_weapon_config_loaded ...@@MAE` = protected virtual.
- `load_weapon_parameters ...@@IAE` = protected.
- `process_loading_weapon_core ...@@IAE` = protected.
- `on_core_subresources_ready ...@@AAE` = private.
- `query_weapon_states ...@@AAE` = private.
- `on_weapon_states_ready ...@@AAE` = private.
- `cooked_object_size ...@@EBE` = private virtual const.

Header (`weapon_core_cook.h`) reordered into public / protected / private
blocks accordingly.

## Anchoring
The cook was dead-stripped from base (no `weapon_core_cook.cpp` symbols in the
base rich index until anchored). Added `use_weapon_core_cook()` to
`game_core/sources/temp_include_all.cpp` (construct a static + `register_cook`),
mirroring `use_victory_item_core`. Construction emits the vtable -> anchors the
virtuals (`translate_query`, `delete_resource`, `on_weapon_config_loaded`,
`cooked_object_size`); `on_weapon_config_loaded` calls
`process_loading_weapon_core` which transitively reaches the rest.

## Results
- `cooked_object_size`: `return sizeof( weapon_core );` (== 0x498). 100%.
- ctor: stub already correct (`translate_query_cook(weapon_class, reuse_false,
  use_current_thread_id)` + `register_cook(this)`). 100%.
- `delete_resource`: `VOSTOK_DELETE_IMPL( g_allocator, resource )`.
  - First tried `*g_allocator` -> 33%: the deref produced a `strip_pointer(T&)`
    path with an extra inlined call (`dummy::nonnull` in base) and esp+4 single
    push. Target is `mov ecx,[g_allocator]; push ecx` (no deref): pass the
    POINTER `g_allocator` directly so `strip_pointer(T* const)` returns `*t`
    inlined with no extra call.
- `on_weapon_config_loaded`: 93.27% with the `*g_allocator` version; switched
  NEW to `g_allocator` (target does `mov eax,[g_allocator]` no deref before
  malloc). Remaining gap: target frame is `sub esp,38h` vs base `34h` (one extra
  4-byte local) - target materializes the `data[0].get_unmanaged_resource()`
  result into a temp `resource_ptr` at `[ebp-14h]` that it `dec`s; base treats it
  as a raw pointer. The ASSERT empty-stub call is named `dummy::nonnull` in base
  / `finalize_impl` in target (same compiled-out ASSERT, byte-identical).
