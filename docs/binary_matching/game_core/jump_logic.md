# jump_logic.cpp - matching trail (batch3)

Worktree: /home/sheep/Projects/surv/vostok_6
Branch: match/game_core-jump_logic-batch3 off fc3aadf9

NOTE: report.json shows 0% for all jump_logic symbols because the rebuild emits
"no target symbol map ... emitting local defaults". Ground truth is the
`pdb_fetch --view diff` per-function `N/N instructions equal (X%)` footer.

## get_move_direction (free fn) -> 100% DONE
Bools from actions_mask bits: fwd=bit1, bwd=bit2, left=bit4, right=bit8.
Stack slots (LOCALS decl order bwd,right,fwd,left): bwd=[ebp-4] right=[ebp-3]
fwd=[ebp-2] left=[ebp-1]. Assignment order in asm: fwd,bwd,left,right.
Two cancel-ifs (fwd&&bwd; left&&right), then nested dispatch.
diff: 99/99 instructions equal (100.0%).

## get_jump_animation_index (free fn)
Jump table switch on move_direction, NO bounds check -> full contiguous 0..8 +
default: NODEFAULT(). Identity table (case enum order = source order).
case on_site: on_site_jump(0) + animation_part.
each other case: (jump_from_right_leg ? X_from_right_leg : X_from_left_leg) + animation_part.
Constants from CONSTANTS carcass; left = right+6 (animations_per_two_legged=6).
anon enum with all 19 named constants. 22 no-address markers before L136 = const decls.

get_jump_animation_index -> 100% DONE (rebuild "improved 0% -> 100%").

## does_need_land_and_run -> 100% DONE
landing_direction = get_move_direction( m_user->input() ); switch(m_jumping_direction)
no-bounds jump table -> contiguous 0..8 + default:NODEFAULT(). each case returns a
3-way OR (self, prev-wrap, next-wrap). order verified vs asm cmp sequence. 100%.

## set_user -> 76.55% PARTIAL (ai-fsm wall)
m_user=&user; for(fsm_state* i=m_logic->states().front(); i; i=i->next)
  static_cast<jump_logic_base_state*>(i)->set_user(user);
diff: target out-of-lines states()/front() (call finalize_impl + call operator[]),
our in-class accessors fold to field loads -> frame slot [ebp-8] vs [ebp-0Ch] mismatch
+ cast-temp ordering. Same wall breath_vibration_calculator documented. Blocked on ai fsm.

## deactivate -> 45.13% PARTIAL (ai-fsm wall, same root cause)
m_logic->set_initial_state( m_logic->states().front() );
target out-of-lines states()/front(); base inlines. 7/26 equal in raw form.

## ctor / dtor / initialize_logic -> BLOCKED (C4716 state-vtable trap)
All three reconstructed and VERIFIED from asm (bodies kept as comments in the .cpp):
- ctor: `: m_owner(owner),m_user(0),m_logic(0),m_animated_object(0),
  m_jumping_direction(on_site),m_is_jump_from_right_leg(true) { initialize_logic(); }`
- dtor: `m_logic->clear_transitions(); while(fsm_state* s=m_logic->pop_state()) DELETE(s);
  DELETE(m_logic);` (same shape as breath_vibration_calculator::~, 100%).
- initialize_logic: m_logic=NEW(ai::fsm); inactive/start/landing=NEW(jump_logic_state_*)(*this);
  add_state x3; add_transition(inactive,start, boost::bind<bool>(&true_predicate));
  add_transition(start,landing, boost::bind(&jump_logic::landing_predicate,this));
  set_initial_state(states().front()). This COMPILES.
BLOCKER: anchoring the ctor -> constructs jump_logic -> initialize_logic constructs the
three jump_logic_state_* subclasses -> emits their vtables -> their STUB
`selected_animations` (no return value) force-codegen -> C4716 -> LNK1257. Those overrides
are matched only in PR #181 (absent from this base commit). The existing temp_include_all
state anchors deliberately AVOID constructing instances for exactly this reason.
UNBLOCK: match jump_logic_state_{inactive,start,landing}::selected_animations first
(or stack on PR #181), then restore the bodies above and anchor the ctor.

## get_animation_caption / get_move_look_caption / get_move_animation / get_move_look_caption
NOT attempted this batch. Need: extern decls for the global caption arrays
(?stand_animations_captions / ?jump_animations_captions, type pcstr const* const),
weapon_user_animations_selector::animations() (a `/* no source */` STUB), and an
unclear ASSERT( owner().animations() )-style side-effect whose value is discarded.
Higher risk (new global decls + STUB accessor), deferred.
