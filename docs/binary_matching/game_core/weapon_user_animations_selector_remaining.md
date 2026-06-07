# weapon_user_animations_selector - remaining hub functions + container_cook batch

Worktree: `/home/sheep/Projects/surv/vostok_5`. Base: `origin/int/game_core`.
Branch: `match/game_core-selector_remaining_and_container_cook`.

Unit: selector ctor/dtor/tick/selected_animations/activate/deactivate plus the
7 `weapon_user_animations_container_cook.cpp` STUBs. (serialize/deserialize stay
BLOCKED on the udp_match_packet/packet_reader cluster.)

Target RVAs (int target index, `binaries/rich/target/index.jsonl`):
- selector ctor 0x585610, dtor 0x5850b0, tick 0x585480, selected_animations 0x584e50,
  activate 0x584f90, deactivate 0x584e90.
- cook ctor 0x752910, delete_resource 0x7529b0, create_requests_for_animations 0x752950,
  translate_query 0x7532e0, on_config_loaded 0x752f50, on_animations_loaded 0x752c20,
  get_animations_from_request_results<27|6|2|100>.

NOTE: the `// FUNCTION BODY` carcass RVAs in the .cpp (0x59xxxx / 0x76xxxx) are STALE
vs the int target index (0x58xxxx / 0x75xxxx). Use the index RVAs above with pdb_fetch.

## Commands run (verbatim, key ones)
- pdb_rich_query --index binaries/rich/target/index.jsonl --list | grep ...
- pdb_fetch --target-index binaries/rich/target/index.jsonl --rva <rva> --view target|structure
- nix develop -c python3 scripts/rebuild.py   (NO module arg)

## Anchoring
- Selector tick/deactivate/selected_animations: added member-fn-pointer anchors p4/p5/p6
  in `use_game_core_weapon_user_animations_selector` (a direct call would LTCG-inline them).
- Cook: new `use_game_core_weapon_user_animations_container_cook` constructs a static
  cook + `resources::register_cook(&s_cook)`. The vtable keeps translate_query/delete_resource;
  translate_query/on_config_loaded reach on_config_loaded -> create_requests_for_animations
  + on_animations_loaded via boost::bind, so all survive.

## Decoded bodies
- selected_animations: `return current_state().selected_animations( buffer, weapon_parameters, is_third_view );`
  (virtual at player_logic_base_state vtable+0x18, sret).
- deactivate: `m_logic.set_initial_state(NULL); m_user->unsubscribe_animation_player(
  channel_id_on_animation_interval_end, this ); (*m_user->damage_model()).unsubscribe_from_affect(
  affects_type_leg_damage, &m_leg_damaged_subscriber );`
- tick: `if (!(m_user->input().actions_mask & 0x200)) m_forced_not_to_sprint=false;
  else if (!(m_user->input().is_sprinting() && is_weapon_in_idle()) && current_state().id()==type_sprint)
  m_forced_not_to_sprint=true; m_logic.tick();`
- delete_resource: `VOSTOK_DELETE_IMPL( g_allocator, resource )` (target out-of-lines delete_helper<doug_lea_allocator,resource_base>).
- create_requests_for_animations: `ASSERT(...); for(u32 i=0;i<requests_count;++i)
  requests.push_back( resources::create_request( cfg[i], resources::animation_class ) );`
  (animation_class == 0x3d; delinker misnames create_request as finalize_impl).

## Iterations
1. base_player was only fwd-declared in selector.cpp -> tick/deactivate failed C2027.
   FIX: #include <vostok/game_core/base_player.h>.

## Re-match (PR #187 review): deactivate vtable slot
Reviewer corrected the earlier "hard wall" framing for deactivate's L122 call: the
slot delta was SOURCE-STEERABLE. base_player.h declared its two overloaded virtuals
`unsubscribe_animation_player(reserved_channel_ids_enum, pcvoid)` then
`unsubscribe_animation_player(pcstr, pcvoid)`. MSVC assigns vtable slots to a group of
overloaded virtuals in reverse declaration order, so the (enum, pcvoid) overload landed
at +58h while the target uses +54h.
- FIX: swapped the two declaration lines (pcstr overload first, enum overload second) in
  sources/vostok/game_core/base_player.h. Touched weapon_user_animations_selector.cpp
  (header-edit gotcha) and ran `python3 scripts/rebuild.py` (no module arg).
- RESULT: deactivate now emits `mov edx,[ecx+44h]; mov edx,[edx]; mov eax,[edx+54h]; call eax`
  matching the target exactly. report.json top-level fuzzy_match_percent 34.88% -> 34.91%.
  report-changes.json: 0 regressed, 1 improved.
- The small overall % bump is expected: the slot fix is one statement; the dominant
  remaining divergence is the L124 deref idiom (target derefs an
  `intrusive_ptr<booby_trap_core,...>::operator*` after pushing the args, frame `sub esp,8`;
  base derefs first via a `dummy::nonnull` fold, frame `sub esp,10h`). That is the same
  deref-idiom / arg-eval-order wall as current_state() and is out of scope for this fix.
- base_player.h is shared; the overload-order edit is minimal and reconciles at land.

## batch2 (resume of crashed session) - branch match/game_core-weapon_user_animations_selector-batch2

Worktree /home/sheep/Projects/surv/vostok_5. Resumed an uncommitted in-progress state.

### Build was BROKEN on resume (the first thing to fix)
The crashed session had set the predicate / is_weapon_* / on_* members to `private` (correct
per their `ABE`/`AAE` mangling) but anchored them in `temp_include_all.cpp`'s FREE function
`use_game_core_weapon_user_animations_selector` via member-fn pointers -> C2248 (can't take
`&self::is_weapon_firing` etc. from a non-member). FIX: added
`friend void ::vostok::use_game_core_weapon_user_animations_selector( );` to the class (the
established house pattern - see booby_trap_core.h, weapon_core_*_state.h). Build then linked.

### Access chars (pdb mangled, target index)
- predicates broken_legs/crouch/jump/stand + is_weapon_firing/in_idle/toggling: `ABE` private const.
- sprint_predicate, look_time_factor: `QBE` public const.
- on_interval_ended/on_broken_limb_affect/set_sprint_callbacks: `AAE` private.
- set_animation_callback x2, remove_animation_callback x2: `QAE` public.
Header already matched these except the predicate block (was `public:`) -> moved to `private:`.

### Commands (verbatim, key)
- pdb_rich_query --index binaries/rich/target/index.jsonl --list | grep weapon_user_animations
- grep -oE '"\?[^"]*weapon_user_animations_selector@survarium@@[^"]*"' binaries/rich/target/index.jsonl
- pdb_fetch --target-index binaries/rich/target/index.jsonl --rva <rva> --view target|structure|callees
- pdb_fetch --target-index ... --base-index binaries/rich/base/index.jsonl --function ... --view base
- nix develop -c python3 scripts/rebuild.py   (no module arg)

### Results this session (report.json fuzzy %)
- on_interval_ended         100% DONE  (ASSERT(UNKNOWN_EXPRESSION_T(&params)); if(animation_user_data==1)
                                         m_right_leg_is_supporting = animation_interval_id != 0; return call_me_again).
- remove_animation_callback(pcstr)         100% DONE  (m_user->unsubscribe_animation_player(channel_id,callback_uid)).
- remove_animation_callback(enum)          100% DONE.
- set_animation_callback(pcstr)   78.28% PARTIAL  body byte-correct subscribe(...,managed_resource_ptr(),0xff,NULL) @vtable+50h.
- set_animation_callback(enum)    84.79% PARTIAL  subscribe(...,managed_resource_ptr(),NULL) @vtable+4Ch.
  WALL: base emits one fewer pushed arg + `mov ecx,eax` this-load vs target's direct ecx; rooted in
  base_player.h virtual overload/vtable layout (owned by another matcher, same class as the
  deactivate +54h slot fix). Not steerable from this TU.
- broken_legs_predicate           76.76% PARTIAL  (*m_user->damage_model()).broken_legs_count()==2;
  WALL: intrusive_ptr operator* out-of-lining - target `call intrusive_ptr<booby_trap_core,...>::operator*`
  (a `mov eax,[eax];ret` COMDAT, NO assert), base inlines our intrusive_ptr_inline.h operator* (carries
  ASSERT, folds to dummy::nonnull). Same deref wall as current_state/get_current_state_id/deactivate.
- is_weapon_in_idle 36.41% / is_weapon_firing 8.96% / is_weapon_toggling 31.17%  PARTIAL.
  WALL 1: current_active_object() returns intrusive_ptr<inventory_item,...> by value (copy-ctor'd temp,
  operator* out-of-lined) in target; base's interactive_object_ptr root is game_world_object and base
  inlines the copy-ctor. Rooted in base_player.h / interactive_object.h typedef + the deref idiom.
  WALL 2 (reviewer, was missing here): weapon_core::is_idle() ITSELF diverges. The target inlines a
  3-field COMPOUND at the call site - `byte[+492]==0 && byte[+488] && byte[+48C]` (diff: test [+492],jne;
  test [+488],je; test [+48C],je; set 0/1) - whereas the weapon_core.h is_idle() we filled is a single
  `return m_is_idle;` (one byte[+492] read). So even past the ptr-typedef wall the predicate body is wrong.
  is_idle's real body lives in weapon_core.h (owned by another worker); the byte[+492]/[+488]/[+48C] reads
  give the exact expression to fill there. Both walls cross-unit, neither steerable from this TU.
- on_broken_limb_affect           20.12% PARTIAL  3 statements decoded: L341 m_user->force_animation_selection();
  L339 = ASSERT_CMP_U(affect,==,4) (push affect/4/0); L337 = a 2-arg eater (push type,bodypart; NO push 0)
  - NOT a standard ASSERT_U/ASSERT_CMP_U; exact debug macro unidentified. Deferred.
- Still STUB (not decoded, deref/predicate-logic walls): crouch/jump/stand/sprint predicate,
  look_time_factor (needs named float consts clear_value/epsilon_5/half/period + vtable+2Ch virtual),
  look_time_factor_calculator, set_sprint_callbacks, ctor/dtor/activate. serialize/deserialize stay BLOCKED.

### weapon_core.h LTCG gotcha (LNK1257)
Reverting the crashed session's weapon_core.h guesses produced `LINK : fatal error LNK1257: code
generation failed` (NO compile error). Cause: weapon_core::is_idle() was `{ /* no source */ }` (a
value-returning inline with NO return path); is_weapon_in_idle is its FIRST real consumer (baseline had
`return false`), and inlining a no-return value-returning fn under LTCG fails codegen. FIX (minimal,
scoped): `inline bool is_idle() const { return m_is_idle; }` (mirrors is_firing/is_toggling; m_is_idle
@+0x492). Left is_toggling at its baseline `return m_is_toggling`. This is the only weapon_core.h edit.

### Regressions
report-changes vs prior in-worktree build: 2 regressed, both MY OWN wall-blocked partials
(is_weapon_in_idle 41.56->36.41, is_weapon_toggling 34.89->31.17, from the weapon_core.h is_idle
minimal body / is_toggling baseline revert). No function outside the selector unit regressed; no ICF
thunk churn. Per MATCHING.md the structurally-correct body is kept over a higher-scoring `return false`.
