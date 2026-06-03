# weapon_user_animations_selector - state accessors batch

Unit: first small-accessor batch in `weapon_user_animations_selector.cpp`.
Functions: `current_state`, `get_current_state_id`, `is_in_jump`, `is_sprinting`,
`is_ready_to_be_deactivated` (a tight cluster - the latter three call `current_state()`).

Worktree: `/home/sheep/Projects/surv/vostok_5`. Base commit: fc3aadf9.

## RVAs / mangling (access chars)
- 0x584a00 `get_current_state_id`  `QBE` public const
- 0x584a30 `current_state`         `ABE` private const  -> moved to a `private:` block in header
- 0x584a70 `is_in_jump`            `QBE` public const
- 0x584bd0 `is_sprinting`          `QBE` public const
- 0x584c00 `is_ready_to_be_deactivated` `QBE` public const

## Layout facts used
- `ai::fsm::m_current_state` @ fsm+0x10 (`fsm::current_state()` inline returns it).
- `player_logic_base_state : ai::fsm_state` (single inheritance, offset 0).
  - `m_weapon_user_state_id` @ +0x20, accessor `id()`.
  - `m_is_ready_to_be_deactivated` @ +0x27, accessor `is_ready_to_be_deactivated()`.
- `weapon_user_state_enum`: stand=0, crouch=1, sprint=2, jump=3.

## Commands
- `pdb_rich_query --index binaries/rich/target/index.jsonl --list | grep weapon_user_animations_selector`
- `pdb_fetch --target-index binaries/rich/target/index.jsonl --rva <rva> --view target|structure|info`
- `llvm-objdump -dr binaries/objdiff/{target,base}/vostok/game_core/sources/weapon_user_animations_selector.cpp.obj`
- `nix develop -c python3 scripts/rebuild.py`  (no module arg)

## Anchoring
The selector ctor is still a STUB, so we never construct one. Existing anchor
`use_game_core_weapon_user_animations_selector` (temp_include_all.cpp) was extended:
direct calls on a null ref LTCG-inline into the anchor and emit NO standalone body
(first build: all five read 0%, only the `use_*` symbol in the obj). Switched to
member-function-pointer addresses (`&self::get_current_state_id` etc.) passed to
`example_callback`; non-virtual `&Class::member` ODR-uses the body as a standalone
COMDAT. `current_state` is private but is kept transitively (the three consumers call it).

## Results
- is_in_jump / is_sprinting / is_ready_to_be_deactivated: 100% DONE.
  Bodies: `current_state().id() == type_jump|type_sprint` and
  `current_state().is_ready_to_be_deactivated()`. Byte-identical incl. the
  `call current_state` reloc, +20h/+27h reads, `xor eax,eax; cmp ...; sete al`.

- current_state: 70.26% PARTIAL. Structure correct: empty_stub ASSERT @ L81 matched,
  `[+10h]` read, `return *result`. DIVERGENCE: target emits an extra
  `lea eax,[ebp-8]; call <operator*>; mov [ebp-4],eax` at L80 (frame 0x14 vs our 0x0C),
  where `<operator*>` ICF-folds to `vec_begin` (`mov eax,[ecx]; ret`) and takes
  `&(fsm_state* temp)`. The rich callee view names it
  `intrusive_ptr<booby_trap_core,...>::operator*` (a COMDAT fold rep). This is a
  smart-pointer/deref idiom on the cast result that a plain `static_cast<...*>(...)`
  collapses. Tried: pointer-form static_cast; reference-form `static_cast<T&>(*ptr)`
  with `&result` ASSERT - both collapse to the same 0x0C-frame bytes (the `*`/`&`
  round-trip is a no-op for single inheritance).

- get_current_state_id: 71.5% PARTIAL. Same root cause - target materializes the
  inlined `m_logic.current_state()` + cast through TWO extra temps (frame 0x14 vs 0x0C).
  Field reads (+10h/+20h) and control flow match; only the temp/`call` chain differs.

## Next step for current_state / get_current_state_id
Identify the exact deref idiom that emits the `call <operator*>` (ICF -> vec_begin)
on the cast result. Candidates to probe: an engine free helper that returns the
pointer unchanged (`mov eax,[ecx]; ret`) used to downcast `fsm_state*` ->
`player_logic_base_state*` while emitting an out-of-line conversion; or
`fsm::current_state()` returning a wrapper rather than a raw `fsm_state*` in the
shipped headers. raw_ptr/non_null_ptr are ruled out (gold build is non-DEBUG ->
`VOSTOK_USE_DEBUG_POINTERS=0`, those become plain `T*` typedefs).
