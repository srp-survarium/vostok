# jump_logic.cpp - matching trail (batch3)

Worktree: /home/sheep/Projects/surv/vostok_6
Branch: match/game_core-jump_logic-batch3 off fc3aadf9

NOTE: report.json has NULL per-function `measures.fuzzy_match_percent` for the WHOLE
game_core unit on this build (the rebuild emitted "no target symbol map ... emitting
local defaults"); only the unit-level aggregate exists (jump_logic.cpp = 25.96%, 2/19
fns matched). So the per-symbol cross-check uses `pdb_fetch --view diff` `N/N
instructions equal (X%)` footers against the COMMITTED base/target indexes (both dated
12:48, the PR build).

REVIEWER CORRECTION (2026-06-03): the originally-banked footer percentages did NOT hold
on the committed build - re-measured with the committed indexes:
  get_move_direction        100.0% (99/99)   - confirmed DONE
  does_need_land_and_run     63.7% (102/160)  - was banked 100%, NOT 100% (see below)
  set_user                   41.9% (18/43)    - was banked 76.55%
  deactivate                 26.9% (7/26)     - was banked 45.13%
  get_jump_animation_index   could not pair via rich-diff (target params are const-
                             qualified, base not - top-level const not mangled so objdiff
                             still pairs; left DONE, body length matches target).
The banked numbers likely came from a pre-final/local build; STATE lines + this trail now
reflect the committed index.

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

## does_need_land_and_run -> 63.7% INPROGRESS (was banked 100%, REVIEWER downgrade)
landing_direction = get_move_direction( m_user->input() ); switch(m_jumping_direction)
no-bounds jump table -> contiguous 0..8 + default:NODEFAULT(). each case returns a
3-way OR (self, prev-wrap, next-wrap). order verified vs asm cmp sequence.
NOT 100%: re-measured 102/160 = 63.7% on the committed index. The body + per-case OR
expressions are byte-identical, but EVERY leaf does `jmp .17` (base) vs `jmp .18`
(target) - the target has ONE MORE block/statement. Target structure = 13 statements
ending at L259 (0x188, +0x28); the base's return-per-case shape stops at L255. This is
the documented "extra intermediate join jmp -> flatten to shared epilogue" class that
took movement_animation_index None->100%. ACTIONABLE re-match: the target almost
certainly wrote `bool result; switch(...){ case X: result = ...; break; } return result;`
(a result temp + single tail `return` at L259) rather than a direct `return` in each
case - giving one shared epilogue (.18) reached by every leaf. Carcass restored inline.

## set_user -> 41.9% PARTIAL (was banked 76.55%; ai-fsm wall + a steerable cast-temp residual)
m_user=&user; for(fsm_state* i=m_logic->states().front(); i; i=i->next)
  static_cast<jump_logic_base_state*>(i)->set_user(user);
Re-measured 18/43 = 41.9% on the committed index.
Residual (1) - the ai-fsm inline-vs-call wall (legit LTCG): TARGET out-of-lines
states() (folds to 0x03f210, delinker-misnames finalize_impl) + the front()/operator[]
accessor; BASE folds them inline (mov ecx,[eax+8]) - confirmed states() exists out-of-
line in TARGET and is absent in BASE. Shifts the loop-head `this` slot ([ebp-8] vs
[ebp-0Ch]).
Residual (2) - SOURCE-SHAPE, NOT the wall (flagged for re-match): in the loop body the
TARGET materializes the static_cast result into its own slot BEFORE pushing `user`
(mov [ebp-0Ch],edx; push user; mov ecx,[ebp-0Ch]; ...), base pushes user first and
derefs `i` directly with no cast temp. Steerable (bind the cast to a named local before
the call); distinct from the inline-vs-call wall. Try once the fsm type lands.
Blocked on the ai fsm type for the wall part.

## deactivate -> 26.9% PARTIAL (was banked 45.13%; ai-fsm wall, same root cause)
m_logic->set_initial_state( m_logic->states().front() );
Re-measured 7/26 = 26.9% on the committed index. TARGET out-of-lines states()
(0x03f210/finalize_impl) + front()/operator[]; base inlines all three (sub esp,0Ch +
[ebp-4]/[ebp-8] temps the target lacks). Permitted LTCG call-boundary class; blocked on
the ai fsm type.

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
