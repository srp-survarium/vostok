# jump_logic state subclasses (jump_logic_state_{start,landing,inactive})

Worktree: /home/sheep/Projects/surv/vostok_6
Branch: match/game_core-jump_logic_state_subclasses off origin/int/game_core

Unit: the three `jump_logic_base_state` derivatives plus the base header itself. The
priority was the no-return STUB `selected_animations` overrides that BLOCKED
`jump_logic::{ctor,dtor,initialize_logic}` (constructing the subclasses emits their
vtable -> force-codegens the no-return STUBs -> C4716/LNK1257, the documented #148 trap).

Files:
- jump_logic_base_state.h        (ctor + set_user + data-member visibility)
- jump_logic_state_inactive.cpp/.h
- jump_logic_state_start.cpp/.h
- jump_logic_state_landing.cpp/.h
- temp_include_all.cpp           (anchors: now CONSTRUCT the subclasses)

## Commands

```
nix develop -c pdb_rich_query --index binaries/rich/target/index.jsonl --function "jump_logic_state_start"  --list
nix develop -c pdb_rich_query --index binaries/rich/target/index.jsonl --function "jump_logic_state_landing" --list
nix develop -c pdb_rich_query --index binaries/rich/target/index.jsonl --function "jump_logic_state_inactive" --list
nix develop -c pdb_fetch --target-index binaries/rich/target/index.jsonl --rva <rva> --view target,structure,callees,info
nix develop -c python3 scripts/rebuild.py     # no module arg
strings binaries/objdiff/target/.../<file>.cpp.obj | grep -oE "...@@[A-Z]{3}"   # access chars
```

Target RVAs (rich index):
- inactive::selected_animations 0x57ce30 (image 0x6e... vary)
- landing: ctor 0x57cef0, finalize 0x57ce40, initialize 0x57cf30, on_interval_end 0x57ce50,
  get_look 0x57d020, get_main 0x57d110, selected_animations 0x57d3e0
- start:   ctor 0x6e8bd0, finalize 0x6e8a00, initialize 0x6e8c10, on_interval_end 0x6e8a50,
  on_jump_event 0x6e8b00, get_look 0x6e8d80, get_main 0x6e8f00, selected_animations 0x6e9210
- base_state: ctor 0xbd230, set_user 0xbd210

## KEY FINDING: every override is PRIVATE (objdiff pairs by access char)

The PDB type-info default put these under `public:`, but the EMITTED symbols mangle:
- `selected_animations`, `initialize`, `finalize` -> `EAE` (private virtual)
- `get_main_lexeme`, `get_look_lexeme`, `on_interval_end`, `on_jump_event` -> `AAE` (private nonvirtual)
- `is_ready_for_transition` -> `EBE` (private virtual const)
- ctor -> `QAE` (public), execute -> folds to public empty rep -> stays public.
- base ctor -> `IAE` (PROTECTED); base data members are read directly by derived overrides
  (no accessor in the target asm) -> made `protected:`.

Moved every override except the ctor + execute under `private:`; befriended the anchor
where the qualified call needs a private member (inactive selected_animations).

## UNBLOCK mechanism

Giving each subclass's no-return STUBs a returning body (`UNREACHABLE_CODE();`, which is
`__assume(0)` in release -> no C4716, emits the empty target frame) lets the class be
INSTANTIATED without the #148 trap. The anchors `use_game_core_jump_logic_state_{start,
landing}` now CONSTRUCT a fabricated instance (never runs), which emits the ctor + the
full vtable (all virtual overrides codegen'd + paired) and the befriended anchor calls the
private non-virtual helpers so their bodies are emitted too. `inactive` is anchored by a
qualified devirtualized call (its empty selected_animations needs no construction).

## Results (report.json fuzzy_match_percent)

100% DONE:
- jump_logic_base_state::jump_logic_base_state (ctor)   -- richer body (5 member inits)
  matches @0xbd230; the richer body also stops MSVC inlining it into the derived ctors,
  which is why the derived ctors lifted to 100% (target CALLS the base ctor, does not inline).
- jump_logic_base_state::set_user
- jump_logic_state_inactive::selected_animations        -- empty UNREACHABLE frame @0x57ce30
- jump_logic_state_landing::jump_logic_state_landing     -- base ctor + m_landing_type=4 (land_run)
- jump_logic_state_landing::finalize                     -- empty body @0x57ce40
- jump_logic_state_start::jump_logic_state_start         -- base ctor + m_preface_interval_ended/
  m_jump_interval_ended cleared; m_physics_jumped (+0x2C) deliberately left uninitialised
- jump_logic_state_start::finalize                       -- m_jump_logic.owner().remove_animation_callback("jump",this);
  remove_animation_callback(channel_id_on_animation_interval_end=2, this)
- jump_logic_state_start::is_ready_for_transition (kept from prior unit)

PARTIAL (UNREACHABLE/empty stubs that UNBLOCK; real bodies recorded inline, blocked on deep
machinery in separate units):
- start/landing selected_animations  9.43% : `make_pair(expression(main_lexeme)+look_lexeme,
  main_lexeme)` where main=get_main_lexeme(buffer,is_third_view, is_aimed?body_part_whole_body
  :body_part_mask), look=get_look_lexeme(buffer,is_third_view,look_calculator,main). Blocked:
  animation_lexeme has no default ctor + the expression/operator+ (addition_lexeme) machinery
  needs the get_main/get_look bodies and the addition_lexeme include resolved (same wall as
  pistol_weapon_core_fire_state). Tried writing the body verbatim -> C2512/C2678; reverted.
- start/landing get_main_lexeme 3.3/3.22% , get_look_lexeme 7.07/10.04% : large lexeme /
  lexeme_parameters / interpolator machinery (jump_logic::get_animation/get_move_look_*,
  fixed_vector<animation_interval,2>, linear_interpolator).
- start/landing initialize 5.83/8.38% : boost::bind + set_animation_callback callback wiring.
- start/landing on_interval_end 11.77/12.81% , start on_jump_event 10.52% : read
  animation_callback_params fields (need that type's layout) + intrusive_ptr::operator==.

## jump_logic::{ctor,dtor,initialize_logic} -- now UNBLOCKABLE, NOT yet matched

These are no longer C4716-blocked (the subclass vtables emit cleanly now). Restoring them
needs: ctor (small: ASSERT + 5 member stores + initialize_logic() call @0x57de30); dtor
(clear_transitions / while(pop_state) DELETE / DELETE m_logic @0x57da30); initialize_logic
(@0x57dae0: NEW(fsm); NEW each state; add_state x3; add_transition x2 via boost::bind<bool>
of survarium::true_predicate and boost::bind(&jump_logic::landing_predicate,this);
set_initial_state(states().front())). initialize_logic depends on the boost::bind machinery
+ a not-yet-present `survarium::true_predicate` free fn -> left BLOCKED for a follow-up
boost::bind unit. The C4716 blocker itself is GONE.

## Rebuild log (report.json deltas, this worktree)
1. inactive UNREACHABLE + start/landing UNREACHABLE stubs, access still public:
   base obj emitted but UNPAIRED (base `UAE` vs target `EAE`). Discovered via obj `strings`.
2. moved overrides to private:, fixed base ctor body + protected ctor/members, matched ctors:
   inactive::selected_animations 100%; base ctor/set_user 100%; landing ctor/finalize 100%;
   start ctor 100%; start::is_ready 100%.
3. constructing anchors: all subclass virtuals/helpers emitted + paired (the partials above).
4. start finalize via m_jump_logic.owner().remove_animation_callback -> 100%.

## Regressions
report-changes.json: ~14 regressed / ~22 improved, all whole-program ICF fold-rep churn
(boost storage thunks, vector-deleting dtors, empty_stub, resource_ptr ctors) flipping
100<->0 as the empty/return-true fold classes reshuffled when the new bodies joined. NONE are
functions this PR's source touches (no jump_logic logic fn regressed).
