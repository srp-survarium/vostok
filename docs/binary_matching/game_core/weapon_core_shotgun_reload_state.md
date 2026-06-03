# weapon_core_shotgun_reload_state - matching log

Unit: the non-BLOCKED STUBs in
`sources/vostok/game_core/sources/weapon_core_shotgun_reload_state.cpp`.
serialize/deserialize stay BLOCKED (udp_match_packet/packet_reader never-compiled
cluster, see game_core/README.md).

Worktree: /home/sheep/Projects/surv/vostok_2 ; base commit fc3aadf9.

## Target RVAs (binaries/rich/target/index.jsonl)
- 0x589730 initialize
- 0x589770 finalize
- 0x5898b0 execute
- 0x5898d0 ~dtor
- 0x589980 finish_reload_predicate
- 0x589ad0 initialize_logic
- 0x589d40 ctor
- 0x589db0 weapon_and_hands_expression
- 0x589720 true_predicate

## Layout
- weapon_core_base_state: m_animation_playback_state@0x120, m_weapon@0x128,
  m_body_part_mask_for_user@0x130, m_animation_has_been_ended@0x135.
- subclass: m_logic (ai::fsm*) @0x138, m_delete_substates_on_destruction (bool) @0x13c.

## Commands
- pdb_rich_query --index binaries/rich/target/index.jsonl --function shotgun_reload_state --list
- pdb_fetch --target-index binaries/rich/target/index.jsonl --rva <rva> --view target/structure/callees
- nix develop -c python3 scripts/rebuild.py

## RESULTS (diff-header fuzzy %, this PR)
- execute    100%   DONE       m_logic->tick();
- ctor       100%   DONE       byte-exact; objdiff pairs `call initialize_logic` by symbol name (report.json: 100.0), so it scores DONE even while initialize_logic's body is a STUB.
- finalize    78.57% PARTIAL   LTCG elides animation_playback_state::reset() (header-inline stub); 3 reset instrs missing. Documented unsteerable inline-vs-call (animation/type_definitions.h:40).
- initialize  50.88% PARTIAL   LTCG inlines fsm_state_list::front(); target keeps states()/front() out-of-line (1 local vs our 3). Unsteerable inline-vs-call.
- weapon_and_hands_expression  100% DONE (objdiff `None`, see below)  byte-identical to target 0x589db0.
- true_predicate    INPROGRESS  `return true;` correct; removed as unreferenced (C4505) until initialize_logic references it.
- initialize_logic  INPROGRESS (STUB)  heavy: operator new fsm, add_state x3, two add_transition with boost::bind(true_predicate)/(&finish_reload_predicate,this), behaviour_cook_params, function0/1 assign_to.
- finish_reload_predicate  INPROGRESS (STUB)  ammo vs capacity, intrusive_ptr/inventory amount(), get_target() checks.
- ~dtor   INPROGRESS (STUB, scored ~32%)  vtable restore + pop_state loop + delete_helper_impl. Now vtable-anchored via the construct anchor.
- serialize/deserialize  BLOCKED  udp_match_packet/packet_reader never-compiled cluster.

## Matched (first approximations)
- execute:    m_logic->tick();
- finalize:   m_animation_playback_state.reset(); m_logic->set_initial_state(0);
- initialize: m_logic->set_initial_state( m_logic->states().front() ); m_animation_has_been_ended = false;
- true_predicate: return true;  (asm: mov al,1)
- weapon_and_hands_expression:
    current = static_cast<substate*>( m_logic->current_state() );  // m_current_state @ fsm+0x10
    return current->weapon_and_hands_expression( buffer, is_third_view, user_state_id, weight );
- ctor (100% DONE): init list m_logic(0), m_delete_substates_on_destruction(true);
    body: m_body_part_mask_for_user = body_part_whole_body_but_hands (-3);  initialize_logic(...);

## Hard / deferred
- initialize_logic (0x589ad0): builds fsm via operator new, add_state x3, two
  add_transition with boost::bind(true_predicate) and boost::bind(&finish_reload_predicate,this),
  behaviour_cook_params, function0/function1 assign_to. Heavy. INPROGRESS.
- finish_reload_predicate (0x589980): ammo_in_magazine vs get_magazine_capacity,
  intrusive_ptr dance over inventory items, amount(), get_target() checks. Mid-complex.
- ~dtor (0x5898d0): vtable restore, pop_state loop + delete_helper_impl, list delete.

## Anchoring problem (the hard part of this unit)
- The class's virtual overrides (execute/initialize/finalize/weapon_and_hands_expression)
  cannot be anchored by member-function-POINTER: a pointer to a virtual member yields a
  vtable thunk/index, NOT the out-of-line body, so nothing is kept (base index showed 0
  scored methods).
- Constructing an instance emits the class vtable, which references
  base::weapon_and_hands_expression, which CALLS
  weapon_core_shotgun_reload_base_substate::weapon_and_hands_expression - a STUB with no
  return -> C4716 -> LNK1257 (LTCG codegen failed). So construction is blocked until that
  base_substate override returns a value (it lives in a sibling file/unit).
- WORKING anchor for execute/initialize/finalize: a *qualified* (non-virtual) call through
  a never-dereferenced static volatile pointer:
      static state_t* volatile state = 0;
      state->state_t::execute();  // forces the standalone body, no vtable emission
  Link-bait only, never executed.
- ctor: no function-pointer exists for ctors and a real construction hits the vtable/substate
  wall, so it stays INPROGRESS until the substate override is matched.
- true_predicate: only referenced by initialize_logic (STUB) -> removed as unreferenced
  (C4505) until initialize_logic is matched -> INPROGRESS.

## weapon_and_hands_expression - re-match (PR #188 review FIX 1)

The override is now UNBLOCKED (the base_substate override is matched, so the real body
links). The override body was 41%/single-slot; the target 0x589db0 materializes the
downcast into TWO stack slots within one statement:

```
sub esp,0Ch
mov [ebp-0Ch],ecx            ; this
mov eax,[ebp-0Ch]
mov ecx,[eax+138h]           ; m_logic
mov edx,[ecx+10h]            ; current_state() (m_current_state @ fsm+0x10, inlined)
mov [ebp-8],edx              ; SLOT A: the current_state() result (a named local)
mov eax,[ebp-8]
mov [ebp-4],eax              ; SLOT B: the downcast `current`
...
mov ecx,[ebp-4]
call weapon_core_shotgun_reload_base_substate::weapon_and_hands_expression
```

The two slots are TWO distinct named locals, NOT a static_cast-to-reference temp:

```cpp
ai::fsm_state* state = m_logic->current_state( );
weapon_core_shotgun_reload_base_substate* current = static_cast< ... >( state );
return current->weapon_and_hands_expression( ... );
```

Attempts (each + rebuild + rich-index byte compare):
1. `current = static_cast<...>( m_logic->current_state() )` (single statement, named ptr):
   `sub esp,8`, single slot `mov [ebp-4],edx` - the static_cast folds current_state() into
   the cast operand. WRONG (one slot).
2. `static_cast<substate&>( *m_logic->current_state() ).method(...)` (reference cast):
   still `sub esp,8`, single slot. WRONG.
3. `fsm_state* state = current_state(); substate* current = static_cast<...>(state);`:
   `sub esp,0Ch`, `mov [ebp-8],edx; mov eax,[ebp-8]; mov [ebp-4],eax` - EXACT.
   The separate `state` local forces the extra slot; the static_cast is then a no-op copy.

Result: base RVA 0x44eba0, size 68 = target size 68, 26/26 instructions byte-identical.
objdiff report.json lists `fuzzy_match_percent: None` for this symbol - the delinked
target obj has no COMDAT for it to pair against, so objdiff cannot score it. Verified
100% by direct rich-index instruction comparison (base == target). Marked 100%|DONE.
