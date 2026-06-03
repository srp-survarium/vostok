# player_logic_jump_state (unit)

7 STUB functions in `sources/vostok/game_core/sources/player_logic_jump_state.cpp`.
A `player_logic_base_state` derivative holding a `jump_logic m_logic` at +0x28; most
methods delegate to `m_logic`.

## Target symbols (target index rva)
```
0x6e9320  set_user                  ?set_user@...@@EAEX...          private virtual
0x6e9350  finalize                  ?finalize@...@@EAEXXZ           private virtual
0x6e9370  initialize                ?initialize@...@@EAEXXZ         private virtual
0x6e93a0  is_ready_for_transition   ?is_ready_for_transition@...@@EBE_NXZ  private virtual const
0x6e9420  selected_animations       ?selected_animations@...@@EBE...  private virtual const
0x6e9450  execute                   ?execute@...@@EAEXXZ            private virtual
0x6e9470  ctor                      ??0player_logic_jump_state@...@@QAE@...  public
```

KEY visibility finding: every virtual override mangles `E` (private). The original
header had them `public`; moved all overrides to a `private:` section (ctor stays
public). Without this objdiff cannot pair the symbols.

## Commands
```
nix develop -c pdb_rich_query --index binaries/rich/target/index.jsonl --list | grep player_logic_jump_state
nix develop -c pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x6e9320 --view target,structure,callees,info   # (repeat per rva)
nix develop -c python3 scripts/rebuild.py
nix develop -c pdb_fetch --target-index ... --base-index binaries/rich/base/index.jsonl --function "...is_ready_for_transition" --view diff --objdiff-base-dir binaries/objdiff/base --objdiff-target-dir binaries/objdiff/target
```

## Results
- ctor 100%: `player_logic_base_state(owner, type_jump)`, `m_logic(owner)`. type_jump=3 (`push 3`).
- initialize 100%: `m_logic.activate(); ASSERT( UNKNOWN_EXPRESSION );`
  The `mov byte[ebp-1],0; lea eax,[ebp-1]; call finalize_impl` is a compiled-out ASSERT
  (assembly_patterns.md). Recovered it; 100%.
- execute 100%: `m_logic.tick();`
- finalize 100%: `m_logic.deactivate();`
- set_user 100%: `player_logic_base_state::set_user(user); m_logic.set_user(user);`
- is_ready_for_transition 79.48% PARTIAL (see below).
- selected_animations INPROGRESS (see below).

## is_ready_for_transition (79.48% PARTIAL)
Source:
```cpp
return m_logic.is_jump_finished()
    || (*m_user->damage_model()).broken_legs_count() == 2
    || !m_user->is_alive();
```
- `m_user` is private in base_state; made it `protected` (no byte change to base_state's
  own functions; verified base_state.cpp still 100%) so the derived override reads it
  directly as the target does (no accessor call).
- `damage_model.h::broken_legs_count()` was `/* no source */`; gave it the genuine body
  `return m_broken_legs_count[0] + m_broken_legs_count[1];`. This is reached only through
  this function. It matched the target's inlined `movzx [+339]; movzx [+338]; add; movzx cl`
  (u8 truncation) and the `==2` compare. Lifted 40% -> 79.48%.

Iteration history (objdiff fuzzy on the diff header):
- direct double-deref `(*dm()).m_broken_legs_count[0] + (*dm()).m_broken_legs_count[1]`
  -> 71% diff / 40.05 report (operator* emitted twice).
- accessor `(*dm()).broken_legs_count()` -> 83.7% diff / 79.48 report (single operator*).

Two RESIDUAL diffs, both upstream inline-vs-outline decisions outside this unit:
1. `intrusive_ptr::operator*` (`ASSERT(m_object); return *m_object;`, inline in
   intrusive_ptr_inline.h) is INLINED in our build but the target keeps it out-of-line;
   the inlined `return *m_object` adds a `mov edx,[ebp-8]; mov eax,[edx]` chain.
2. `base_player::is_alive()` is a real out-of-line symbol in the target (rva 0x86310)
   but the header declares it `inline { return m_is_alive; }`, folding to a direct
   `movzx [+119h]` read instead of `call is_alive`.
Fixing either requires editing shared/other-unit headers (intrusive_ptr_inline.h,
base_player.h/.cpp), out of this unit's scope. Left PARTIAL with full carcass + target
asm preserved inline.

## selected_animations (INPROGRESS)
Real body is a one-line delegation: `return m_logic.selected_animations(buffer,
weapon_parameters, is_third_view);` (target asm @0x6e9420 is a clean forwarding shim).
BLOCKED: `jump_logic::selected_animations` (jump_logic.cpp) is still a STUB with no
return value; once the real delegation references it, MSVC errors C4716 ("must return a
value") which is escalated to LNK1257 during LTCG. `mixing::expression` has no default
ctor so the stub cannot be trivially closed.

Placeholder body `VOSTOK_UNREACHABLE_CODE();` keeps the pair-returning override
compilable (it is force-instantiated by the temp_include_all anchor's jump_state vtable).
NEXT STEP: match `jump_logic::selected_animations` (separate unit; ~0x4f bytes, calls
weapon_user_animations_selector::look_time_calculator + a virtual on the fsm current
state) so it returns a value, then replace the placeholder here with the one-line
delegation.

## Anchor
Added `use_game_core_player_logic_jump_state()` in temp_include_all.cpp (registered in
the use list before `use_game_core_jump_logic_state_inactive`). Constructs a jump_state
(fabricated null owner/user; never runs) and calls set_user/initialize/execute/finalize/
is_ready_for_transition through base / fsm_state references (overrides are private).
selected_animations is intentionally NOT called from the anchor (see INPROGRESS).

## Regressions
report-changes.json shows ~14 generic COMDAT 100->0 (btHashMap dtor, speedtree, float3
ctor, resource_ptr template ctors, vector-deleting-dtor thunks, empty_stub) with ~16
improved — classic COMDAT re-attribution churn from adding a newly-anchored TU, not logic
regressions. No touched-file logic function (jump_state / base_state) regressed.
