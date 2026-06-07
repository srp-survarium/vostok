# booby_trap_set_core.cpp - structure verification (v2 verifier-matcher)

Targets handed: `~booby_trap_set_core()`, `remove_trap_if_active`,
`remove_trap_impl`, `insert_trap`. All four have CORRECT source structure;
one fix applied (the dtor's missing ASSERT), the rest are non-steerable
inline-vs-call walls.

Diagnosis command (per function):
```
nix develop -c pdb_fetch --target-index binaries/rich/target/index.jsonl \
  --base-index binaries/rich/base/index.jsonl --function <name> \
  --view structure-diff --condensed \
  --objdiff-base-dir binaries/objdiff/base --objdiff-target-dir binaries/objdiff/target
```

## ~booby_trap_set_core() : 77.75% -> 83.57% (FIX applied)
The target opens with `mov byte ptr [ebp-5], 0` / `lea eax, [ebp-5]` /
`call empty_stub` BEFORE `m_damage_parameters.begin( )` - the classic
compiled-out ASSERT prefix (assembly_patterns.md "mov byte[ebp-N],0; lea;
call empty_stub"). Our base had no ASSERT there. Added `ASSERT( UNKNOWN_EXPRESSION );`
as the first statement; rebuild improved 77.75 -> 83.57% (objdiff), 0 regressions.

Residuals (non-steerable):
- `VOSTOK_FREE_IMPL( g_allocator, damage_parms_buffer )` (type `pcvoid` =
  `void const*`): target calls `free_helper` OUT-OF-LINE (`lea edi,[ebp-4]`;
  `mov ecx,eax`; `call free_helper`), base INLINES `free_helper`'s body
  (`if(!pointer)return;` -> `cmp [ebp-4],0` / `jne` + `free_helper_impl` +
  `pointer=0`). Same free_helper inline-vs-call wall as
  damage_model_cook::delete_resource. The `push edi` / `sub esp,44h` (vs 3Ch)
  / extra slot are all downstream of this one inline choice.
- `m_traps_buffer` free is inlined on BOTH sides (matches).
- Structure parser shows quantity-diffs 2 only because the target PDB
  attributes the ASSERT to the same source line as `begin()` (it splits them
  into ASSERT + EMPTY + ONLY-base begin). The byte improvement confirms the
  ASSERT is the right instructions - this is a source-line attribution artifact,
  not a real structural divergence.

## remove_trap_if_active : 79.71% (STRUCTURE MATCH, non-steerable)
target 4 / base 4 stmts, quantity-diffs 0. Sole SIZE on `remove_trap_impl( *trap )`:
target inlines `resource_ptr::operator*( )` (deref `mov eax,[edx]` + its internal
ASSERT `byte[ebp-9]` prefix), base calls `operator*` out-of-line. inline-vs-call wall.

## remove_trap_impl : 96.21% (STRUCTURE MATCH, non-steerable)
target 5 / base 5 stmts, quantity-diffs 0. Two SIZE diffs:
- `get_inventory( ).holder( )`: target inlines `inventory::holder( )` as
  `mov edx,[ecx+158h]` (reads `*m_holder`); base calls `holder( )` out-of-line.
- `holder.scheduler( )`: target inlines `inventory_holder::scheduler( )` as
  `mov edx,[.+4]`; base calls `scheduler( )` out-of-line.
Both accessors have visible inline bodies in headers (inventory.h:29,
inventory_holder.h:41); the out-of-line emission is the optimizer's choice,
not source-steerable.

## insert_trap : 97.38% (STRUCTURE MATCH, non-steerable)
target 9 / base 9 stmts, quantity-diffs 0. Same two SIZE diffs as
remove_trap_impl: `holder( )` and `scheduler( )` inlined on target vs
out-of-line in base. Non-steerable.

## Net
Only the dtor was source-steerable (missing ASSERT, +5.82pp). The other three
are byte-residual-only inline-vs-call walls (holder/scheduler/operator*/free_helper)
with perfect statement structure. No further source fix available.
