# jump_logic leaf methods (batch 1)

One PR matching the small `jump_logic` leaf accessors. All share the same
scaffolding (class decl, `m_logic`/`m_owner` member offsets, one anchor).

Functions in this unit:
- `jump_logic::landing_predicate() const`  -> None|DONE (folded, byte-correct)
- `jump_logic::tick()`                     -> 100% DONE
- `jump_logic::look_time_factor() const`   -> 100% DONE
- `jump_logic::is_jump_finished() const`   -> 100% DONE
- (cluster) `jump_logic_base_state::is_jump_finished() const` inline body filled
  (`return m_is_jump_finished;`) - forced by `jump_logic::is_jump_finished` inlining it.

## Commands

List/fetch target asm:
```
pdb_rich_query --index binaries/rich/target/index.jsonl --list --function jump_logic
pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x57d4f0 --view target  # landing_predicate
pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x57dac0 --view target  # tick
pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x57d9b0 --view target  # look_time_factor
pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x57d610 --view target  # is_jump_finished
```
(rich-index RVAs are 0x57dxxxx; the carcass comments use 0x58dxxxx - a section base
offset; use the rich-index RVA for pdb_fetch.)

Build: `nix develop -c python3 scripts/rebuild.py` (no module arg, relinks EXE).

## Per-function notes

### landing_predicate  (0x57d4f0)
```
mov al, 1 ; ret
```
=> `return true;` (carcass STUB had a wrong `return false;`). 8/8 bytes equal by
`--view diff` @0x57d4f0. objdiff reports the symbol unpaired (`None`) because
`/OPT:ICF` folds the `mov al,1;ret` body into a shared fold target (same situation
as `player_logic_base_state::is_ready_for_transition`, see temp_include_all.cpp note).
Byte-correct but not separately scorable -> `None|DONE`.

### tick  (0x57dac0)
```
mov eax,[ebp-4]      ; this
mov ecx,[eax+8]      ; m_logic (offset 0x8)
call vostok::ai::fsm::tick
```
=> `m_logic->tick();`  -> 100%.

### look_time_factor  (0x57d9b0)
```
mov eax,[ebp-4]   ; this
mov ecx,[eax]     ; m_owner (reference stored at offset 0x0)
call survarium::weapon_user_animations_selector::look_time_factor
```
=> `return m_owner.look_time_factor();`  -> 100%.

### is_jump_finished  (0x57d610)
```
mov eax,[this]; mov ecx,[eax+8]   ; m_logic
mov edx,[ecx+10h]                 ; fsm::current_state()  (m_current_state @ fsm+0x10)
mov [ebp-4],edx
mov eax,[ebp-4]; mov cl,[eax+25h] ; jump_logic_base_state::m_is_jump_finished @ +0x25
```
=> `return static_cast<jump_logic_base_state*>( m_logic->current_state() )->is_jump_finished();`
The two stack temporaries (`[ebp-4]`, `[ebp-5]`) are the inlined returns of
`fsm::current_state()` and `jump_logic_base_state::is_jump_finished()`. Filling the
latter inline (`{ return m_is_jump_finished; }`) was required - it was a `/* no source */`
STUB; calling it forced codegen and triggered C4716 until given a body. -> 100%.

## Includes added to jump_logic.cpp
`weapon_user_animations_selector.h` (look_time_factor), `ai/fsm.h`
(current_state/tick), `jump_logic_base_state.h` (cast + is_jump_finished member).

## Anchor
`use_game_core_jump_logic()` in temp_include_all.cpp: fabricates a `jump_logic&`
and makes qualified calls to the four methods, escaping the bool/float results via
`example_callback`. Listed in `include_all_game_core_functions()`.

## report-changes
9 net functions improved (is_jump_finished/tick/look_time_factor + ICF-paired
re-attributions). The 8 "regressed" entries are address-shift / ICF re-pairing noise
(empty_stub, speedtree_data ctor, boost storage thunks) matched by equal-named
improvements - not caused by this change.
