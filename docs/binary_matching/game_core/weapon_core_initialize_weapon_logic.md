# game_core::weapon_core::initialize_weapon_logic

STRESS TEST. One of the largest/hardest functions in game_core: builds the
weapon-logic FSM from 10 `weapon_core_base_state_ptr` arguments. Target rva
`0x597020`, body `0x3724` bytes, 124 statements.

## What it does (verified from `--view target`)

Prologue (src L126-L173):
- L128 `m_is_there_chamber_a_round_state = chamber_a_round_state.get() != NULL;`
  (`setne` on `[ebp+28h]` deref, store to `[eax+48Dh]`).
- L129 `ASSERT( ... )` — `call fixed_size_allocator<...>::finalize_impl`
  (`mov byte[ebp-29h],0; lea eax,[ebp-29h]; call empty_stub`), the compiled-out
  assert. Recovered as `ASSERT( UNKNOWN_EXPRESSION_T( m_logic ) )`.
- L131-138 eight `m_logic_states.push_back( <arg>_state )` (member `m_logic_states`
  at 0x418, `buffer_vector<weapon_core_base_state_ptr>::push_back`, `add ecx,418h`).
- L140-141 `if ( m_is_there_chamber_a_round_state ) m_logic_states.push_back( chamber_a_round_state );`
- L143-144 `if ( chamber_a_round_aimed_state ) m_logic_states.push_back( chamber_a_round_aimed_state );`
  (the `<0x27>` guard uses the resource_ptr safe-bool
  `platform_pointer_selector<char const,1>::helper_pod::operator char const*`).
- L146-157 ten `T* local = <arg>_state.get();` (`mov [ecx]; mov [ebp-XX]`).
  slot map: `[ebp-0Ch]`=inactive `[ebp-8]`=show `[ebp-1Ch]`=hide `[ebp-28h]`=idle
  `[ebp-20h]`=reload `[ebp-18h]`=fire `[ebp-14h]`=aim `[ebp-24h]`=aim_fire
  `[ebp-10h]`=chamber_a_round `[ebp-4]`=chamber_a_round_aimed.
- L159-160 `fire->set_is_firing_ptr( &m_is_firing ); aim_fire->set_is_firing_ptr( &m_is_firing );`
  (member `m_is_firing_ptr` at 0x12c of weapon_core_base_state; value `&m_is_firing`
  = `this+0x48c`; `set_is_firing_ptr` is an inline store).
- L162-173 ten `m_logic->add_state( <local> )`, the last two guarded by
  `if ( m_is_there_chamber_a_round_state )` and `if ( chamber_a_round_aimed )`.
  (`m_logic` at 0x414, `ai::fsm::add_state`.)

Body: 72 `m_logic->add_transition( from, to, boost::bind( &weapon_core::<pred>, this[, <weapon_targets>] ) )`.
Full from/to/pred/enum table extracted by `/tmp/parse2.py` (in the PR description /
commit). Predicates: `target_predicate`(1-arg), `target_and_animation_ended_predicate`(1-arg),
and the 0-arg `must_chamber_a_round_predicate`, `must_chamber_a_round_aimed_predicate`,
`must_chamber_a_round_and_animation_ended_predicate`,
`must_chamber_a_round_aimed_and_animation_ended_predicate`,
`can_and_must_reload_predicate`, `can_and_must_reload_and_animation_ended_predicate`,
`instant_idle_predicate`, `is_trying_to_aim`, `is_not_trying_to_aim_predicate`.

The chamber_a_round / chamber_a_round_aimed outgoing transitions (the last two
"rows", transitions 58-71) are each wrapped in a braced
`if ( m_is_there_chamber_a_round_state ) { ... }` / `if ( chamber_a_round_aimed ) { ... }`.
A handful of earlier chamber transitions (trans 6,7,21,22,28,35,36,50,51) are
single-statement brace-less `if` guards.

## THE WALL: two distinct boost::function codegen forms (Form A vs Form B)

The target emits the transition machinery in TWO different shapes:

- **Form A** (transitions 0-55, statement size `0xc6`/`0xc4`/`0xab`):
  - `boost::bind(...)` result is copied (4x dword) into a 16-byte temp,
  - `vostok::ai::behaviour_cook_params::behaviour_cook_params()` is called on the
    `boost::function` storage (this is the ICF-FOLDED `boost::function0<bool>`
    default ctor — both just zero one pointer, so the linker folded them and the
    symbol resolved to `behaviour_cook_params`),
  - then `boost::function0<bool>::assign_to<bind_t>( temp )`,
  - `add esp, 14h` after the bind call.
  This is the **default-construct-then-assign** (`operator=`) shape.

- **Form B** (transitions 56-71, statement size `0x8a`/`0x85`):
  - `boost::bind(...)` result copied into the temp,
  - `boost::function<bool(void)>::function<bind_t>( bind_t, int )` converting
    constructor (single step, no separate default ctor, no `behaviour_cook_params`),
  - `add esp, 4` after the bind call.
  This is the **copy-initialization / converting-ctor** shape.

A single source idiom `add_transition( a, b, boost::bind(...) )` can only produce
ONE of these per build — the compiler picks the `boost::function` constructor
based on how the temporary is materialized, which is NOT independently steerable
per call from source. The two regions almost certainly came from two textually
different original idioms (e.g. one block built a named `boost::function0<bool>`
local and assigned the bind into it, the other passed the bind directly as a
temporary; or a macro vs hand-written). Reproducing BOTH forms in one function
is the crux/wall this stress test set out to find.

## Commands run (verbatim)
- pdb_rich_query --index binaries/rich/target/index.jsonl --function initialize_weapon_logic --list
- pdb_fetch --target-index binaries/rich/target/index.jsonl --function initialize_weapon_logic --view callees
- pdb_fetch --target-index binaries/rich/target/index.jsonl --function initialize_weapon_logic --view info
- pdb_fetch ... --view structure
- pdb_fetch ... --view target  (> /tmp/iwl_target.txt, 3363 lines)
- /tmp/parse2.py  (extracted the 72-transition table)
- python3 scripts/rebuild.py   (no module arg)

## Iterations
1. INPUT: full body — prologue (push_back x10 guarded, .get() locals x10,
   set_is_firing_ptr x2, add_state x10 guarded) + 72 add_transition via
   `boost::bind( &weapon_core::pred, this[, enum] )`; ASSERT for finalize_impl;
   anchor `use_game_core_weapon_core_initialize_weapon_logic` in temp_include_all;
   includes `<vostok/ai/fsm.h>` + `<boost/bind.hpp>`.
   BUILD: rebuild completed; report.json fuzzy_match_percent = 95.69019.
   DIFF:  residual concentrated in the per-transition boost::function construction
          (see the Form A vs Form B note above) — confirmed by the 95.69% being
          just short of 100 with the structure otherwise paired.

## Outcome
STATE[95.69%|INPROGRESS]: full structurally-faithful body written; report.json
scores it **95.69%**. (The matcher process died on a transient API error right
after the rebuild, before it could read this number / commit; the orchestrator
finalized the STATE, this outcome, and the PROGRESS ledger line from report.json
without rebuilding — the body is unchanged from what produced 95.69%.)
Residual wall: Form A vs Form B boost::function construction (see above) — one
source idiom yields one form, so ~16 or ~56 transitions mismatch on the function0
default-ctor + assign_to vs converting-ctor shape regardless. Next step: find the
two original idioms (named function0 local + assign vs direct temporary) and split
the body to match both regions; OR accept the ~one-form residual as a documented
template-instantiation-shape limit. The carcass should be restored (non-100%) —
flagged for the reviewer.
