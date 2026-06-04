# player_logic_sprint_state (unit)

7 STUB functions in `player_logic_sprint_state.cpp` (+ `.h` execute). A
`player_logic_base_state` derivative that, unlike jump/crouch, holds two
`boost::function<void()>` callbacks (`m_initialize_callback` @0x28,
`m_finalize_callback` @0x48) plus a `player_stamina_subscriber m_stamina_subscriber`
@0x68. initialize/finalize (un)subscribe the stamina subscriber and fire a callback.

## Target symbols (mangled access chars)
```
0x6e9510  ??0player_logic_sprint_state@...@@QAE@...   ctor               public
0x6e94d0  ?set_callbacks@...@@QAEX...                  set_callbacks      public
0x6e94b0  ?on_stamina_depleted@...@@AAEXXZ             on_stamina_depleted private
0x6e95d0  ?finalize@...@@EAEXXZ                         finalize           private virtual
0x6e9620  ?initialize@...@@EAEXXZ                       initialize         private virtual
0x6e9670  ?selected_animations@...@@EBE...             selected_animations private virtual const
0x01a800  ??_G... (execute folds to empty stub)        execute            private virtual
```
Header rewritten: all virtual overrides + on_stamina_depleted moved to `private:`;
ctor + set_callbacks stay public.

## Commands
```
nix develop -c pdb_rich_query --index binaries/rich/target/index.jsonl --list | grep player_logic_sprint_state
nix develop -c pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x6e9510 --view target,callees   # per rva
nix develop -c python3 scripts/rebuild.py
nix develop -c pdb_fetch --target-index ... --base-index binaries/rich/base/index.jsonl --function "player_logic_sprint_state::player_logic_sprint_state" --view diff --objdiff-base-dir binaries/objdiff/base --objdiff-target-dir binaries/objdiff/target
```

## Reachability
Unit was NOT in the base index (compiled but link-discarded), so all 7 read 0%.
Added `use_game_core_player_logic_sprint_state()` anchor (+ include + entrypoint call)
in temp_include_all.cpp, mirroring the jump anchor: construct the state (ODR-uses
ctor + the bound on_stamina_depleted), reach private virtuals via the `ai::fsm_state&`
base, call public set_callbacks. Constructing forces selected_animations codegen
(C4716/LNK1257 on the no-return stub), so its body is a `VOSTOK_UNREACHABLE_CODE()`
placeholder (same trick as jump).

Needed `#include base_player.h` + `player_stamina.h` in the .cpp (base_player was
incomplete -> C2027 on `->force_animation_selection`/`->stamina`).

## Results
- set_callbacks 100% DONE: `m_initialize_callback = ...; m_finalize_callback = ...;`
- initialize 100% DONE: `m_user->stamina().subscribe_on_depletion(&m_stamina_subscriber); m_initialize_callback();`
- finalize 100% DONE: `m_user->stamina().unsubscribe_from_depletion(&m_stamina_subscriber); m_finalize_callback();`
- execute None DONE: empty `{ }`; 7/7 bytes equal in text diff but ICF folds the empty
  stub so objdiff pairs to a different fold-rep (reads None). Bytes correct.
- ctor 66.70% PARTIAL (was 43.47 -> 66.70). Iteration:
  - member-init `m_stamina_subscriber( boost::bind(...) )` -> 43.47%; the target
    DEFAULT-constructs the subscriber then assigns the bound callback (asm:
    `add edx,68h; push 0; call function(clear_type); [eax+20h]=0` then
    assign_to/swap/clear).
  - body-assign `m_stamina_subscriber.subscription_callback = boost::bind(&on_stamina_depleted, this);`
    -> 66.70%. Base/type (`push 2`=type_sprint) and the member-pointer bind match.
  Residual is boost-internal codegen: (1) the subscriber's default ctor uses
  `function(clear_type)` + `next=0` (our `{ }` default ctor emits `function()` w/o the
  `push 0` and never zeroes `next`); (2) `boost::function::operator=` expands to
  assign_to/swap/clear in the target, ours takes a shorter path. Both live in shared
  headers (player_stamina_subscriber.h / boost function_template.hpp), out of unit scope.
- on_stamina_depleted 47.50% PARTIAL: `m_user->force_animation_selection();`. Target
  makes an out-of-line `call base_player::force_animation_selection`; base_player.h
  declares it inline (`{ m_force_animation_selection = true; }`) so our build inlines the
  store (`mov byte[edx+118h],1`) and grows the frame (`sub esp,8` vs target `push ecx`).
  Inline-vs-outline wall, fixable only in base_player.h (other unit). Same class as the
  jump_state `is_alive()` residual.
- selected_animations 3.32% None: lexeme/expression wall (mixing infrastructure not
  matched). Placeholder `VOSTOK_UNREACHABLE_CODE()`; carcass preserved.

## Regressions
report-changes after the relink: 15 regressed / 11 improved, all (100->0)/(0->100)
COMDAT fold-representative churn on shared stubs (empty_stub, ~btHashMap, ~mutex,
network::world vcall thunks, resource_ptr::operator=, ~enemy_target_selector). None in
this unit or in crouch/jump/stamina/base_player. The 3 new 100% + the partials are the
real delta. Overall fuzzy 48.49%.
