# booby_trap_core

Unit: `sources/vostok/game_core/sources/booby_trap_core.cpp`. Already partially
matched on arrival (many 100% lifecycle/accessor fns). This pass drove the
source-steerable shapes; the rest are blocked on a single recurring LTCG wall.

## Commands

- `nix develop -c python3 scripts/rebuild.py` (no module arg; relinks EXE)
- Per-fn %: `binaries/objdiff/report.json` -> units[].functions[].fuzzy_match_percent
- Diffs:
  `nix develop -c pdb_fetch --target-index binaries/rich/target/index.jsonl \
    --base-index binaries/rich/base/index.jsonl --rva <rva> --view diff \
    --objdiff-base-dir binaries/objdiff/base --objdiff-target-dir binaries/objdiff/target`
- RVAs (target index, rebased; differ from carcass addrs):
  on_state_timer_finished 0x58b090, can_defuse 0x58b270, set_transform 0x58b3c0,
  use_execute 0x58b460, switch_to_state 0x58b800, apply_damage 0x58b9c0,
  on_enter 0x58bb10, ~booby_trap_core 0x58be80, load 0x58bf60,
  unregister_tick 0x58b7e0

## Fixes landed (no regressions anywhere)

### on_state_timer_finished  86.20% -> 99.67%  DONE
Target loads `m_trap_state` ONCE into a temp slot (`mov ecx,[eax+170h];
mov [ebp-8],ecx`) and compares the temp twice; base re-read the member on each
compare. Cached it in a local:
```cpp
booby_trap_state const state = m_trap_state;
if ( state == booby_trap_state_armed || state > booby_trap_state_disarmed )
```
Remaining diff is a pure stack-slot swap (target: this@-4, state@-8; base
reversed) on a byte-identical instruction stream -> acceptable DONE.

### can_defuse  81.57% -> 86.04%  PARTIAL
Boolean form: target merges both true-paths into one `mov 1` block
(`je .true; ...; jne .true; mov 0; jmp; .true: mov 1`). Rewrote the ternary as a
short-circuit OR to get the branch-merge:
```cpp
return user == owner || user->team( ) != owner->team( );
```
Residual = the `holder()` LTCG wall (below).

### switch_to_state  90.55% -> 93.08%  PARTIAL
Target jump table has NO bounds check (`jmp [reg*4+table]`, no `cmp 3; ja`),
so the source covered the full contiguous case range and ended
`default: NODEFAULT();`. Added it. Residual = the `config()` LTCG wall (below).

## Walls (deferred) - LTCG per-callsite no-inline of trivial header getters/helpers

Base build uses `/Od /Ob2 /Oi /GL` (verified in
`binaries/ninja/rsp/game_core_cl_0.rsp`). Under LTCG the original toolchain chose
NOT to inline several trivial header inlines at these call sites, emitting them as
standalone COMDAT symbols + a `call`; our base inlines them. No source construct
controls per-callsite inlining of a `{ return m_x; }` getter, so these are not
recoverable from this TU.

- `booby_trap_set_core::config()` (`{ return m_config; }`, header inline, real
  symbol 0x9b8e0 in target) kept as a call: blocks `set_transform` (97.38%),
  `insert` (97.44%), `switch_to_state` (93.08% residual), `use_execute`,
  `load`.
- `inventory::holder()` (`{ return *m_holder; }`, real symbol 0x86b70) kept as a
  call: blocks `can_defuse` (86.04% residual), `on_enter` (91.77%),
  `apply_damage` (83.06%).
- `math::min(u32,u32)` (real symbol 0x3fbb0) and `new_helper<>::call` /
  `delete_helper<>` kept as calls: blocks `use_execute` (82.75%),
  `load` (90.68%), `~booby_trap_core` (14.86% - whole VOSTOK_DELETE_IMPL inlined
  vs target's `delete_helper<doug_lea_allocator,collision_geometry>` by-value
  call; non-_MANAGED only declares the by-ref inline overload, so the by-value
  `T const*` overload the target uses is unreachable from this source).
- `register_tick` (74.26%): `scheduler::register_on_frame` + `boost::bind` LTCG.
- `serialize` (16.17%) / `deserialize` (11.13%): network_core packet
  serialize/deserialize wall (explicitly deferred by task scope).
