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
