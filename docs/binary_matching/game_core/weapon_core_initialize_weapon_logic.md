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

## Reviewer audit (claude, no rebuild — review-only)
Re-ran the five checks against `--view target`/`--view diff` and report.json
(95.69019 → 95.69%, confirmed in `.cpp` STATE, here, and PROGRESS).
- **Form A / Form B residual diagnosis HELD UP.** Counted in the target asm:
  exactly **56** `behaviour_cook_params`(folded function0 default ctor) + 56
  `assign_to` (Form A, transitions 0-55) and **16** `boost::function<bool(void)>::
  function<bind_t>` converting ctors (Form B, transitions 56-71), 56+16 = 72.
  `add esp` tally: 41×`14h` + 15×`10h` (Form A) vs 11×`4` (Form B). The `--view
  diff` tail shows BASE emitting Form A (`add esp,14h` + function-ctor + `assign_to`)
  where TARGET emits Form B (`add esp,4` + converting ctor) — base/target the right
  way round. The residual is the boost::function instantiation shape, NOT a wrong
  predicate / missing-extra transition / wrong enum / missing ASSERT.
- **72 add_transition / 10 add_state verified.** Extracted the target's ordered
  (predicate, enum) sequence for all 72 transitions and diffed it against the source:
  ZERO mismatches once the Form-B `push 0`(bind int arg) is excluded from the enum
  read. Enum map confirmed from the target pushes: idle=0, fire=1, aim=2, aim_fire=3,
  reload=4, inactive=5. Predicate counts match (30 target_predicate, 22
  target_and_animation_ended, etc.). `finalize_impl` ASSERT recovered correctly
  (`mov byte[ebp-29h],0; lea eax,[ebp-29h]; call ...finalize_impl`).
- **Carcass restored** from the matcher's malformed `/* ... */` duplicate-signature
  + LOCALS scratch block to a proper inline `// FUNCTION BODY` comment block (all
  `<0>`/`<1>` marker lines verbatim), per MATCHING.md non-100% rule. Comment-only.
- PROGRESS rva corrected (was the base-region `0x5a3130`; target rva is `0x597020`).
- No stray logs; the chamber-a-round flag guards (`byte[+48Dh]`, `cmp [ebp-4],0`,
  nested `m_chamber_a_round_on_reload`) match the target's branch structure.

## Structure-verifier pass (2026-06-07)
- report.json now reads **97.94%** (the STATE marker was stale at 95.69%); synced.
- `--view structure-diff --condensed`: target 141 / base 128 stmts; aligned 126,
  size-diffs 1, quantity-diffs 15. ALL the quantity rows are line-attribution
  artifacts: the conditional `add_state(chamber_a_round)` (L168/L169) and the nested
  `if(m_is_there_chamber_a_round_state) if(!m_chamber_a_round_on_reload)` at L205-207
  emit the SAME instructions on both sides - `--view diff` shows them byte-identical
  modulo register naming (target attributes both nested tests to one source line at
  0x1871 <0x2a>, base splits them into two L-rows of 0x15). No real structural
  divergence, no missing/extra transition.
- Residual to 100% is the boost::bind/boost::function transition-temp REGISTER
  allocation: base inserts a `push esi` in the prologue and uses esi where the target
  uses ecx, cascading the `~` register-rename across the 72 transitions (the Form-A vs
  Form-B boost::function construction shape documented above).
- NEXT STEP: localize the FIRST transition whose temp-ctor register choice diverges
  and steer the boost::function construction form there; the rest cascade from it.
- Carcass deleted and replaced with the standard condensed `// STRUCTURE DIFF` embed.

## BLOCKED packet cluster (re-checked, still blocked)
The orchestrator asked whether any BLOCKED serialize/deserialize is now compilable.
Re-checked: `network_core/udp_match_packet.h` still does NOT compile standalone -
`udp_match_client_session` is entirely undefined, the private base
`packet<udp_match_packet>` hides `append()`, and the header's own note still lists
the missing pieces. report.json scores these (e.g. reload serialize 57%,
deserialize 56.7%) only because the EMPTY stub bodies pair by mangled name against
the real target bodies - that is the stub-vs-body partial artifact, NOT a compiled
match. `weapon_core_reload_state_base::serialize/deserialize` targets are tiny
(`packet.append(m_animation_has_been_ended)` @+0x135 and `reader.r<bool>()` into
@+0x135), but writing those bodies requires including the broken header, which would
break the whole game_core build. Left BLOCKED per game_core/README.md; unblocking is
a separate header-cluster work item.
