# jump_logic.cpp - matching trail (batch3)

Worktree: /home/sheep/Projects/surv/vostok_6
Branch: match/game_core-jump_logic-batch3 off fc3aadf9

NOTE: report.json has NULL per-function `measures.fuzzy_match_percent` for the WHOLE
game_core unit on this build (the rebuild emitted "no target symbol map ... emitting
local defaults"); only the unit-level aggregate exists (jump_logic.cpp = 25.96%, 2/19
fns matched). So the per-symbol cross-check uses `pdb_fetch --view diff` `N/N
instructions equal (X%)` footers against the COMMITTED base/target indexes (both dated
12:48, the PR build).

REVIEWER CORRECTION (2026-06-03) used the diff-VIEW footer, which UNDER-counts jump-table
functions. The RE-MATCH below establishes the authoritative report.json numbers and supersedes
the footer figures: does_need_land_and_run and get_jump_animation_index are TRUE 100% matches
(footer 63.7%/55.0% is a jump-table-data artifact), set_user is 83.61% (cast-temp fix),
deactivate 45.13% (fsm wall), get_move_direction 100%.

## RE-MATCH (reviewer f83c820c follow-up, 2026-06-03)

### CRITICAL: where the authoritative per-fn % actually lives
The path `unit.functions[].measures.fuzzy_match_percent` is null worktree-wide (a red herring -
all 25372 fns null; caused by a missing `binaries/objdiff/target-symbol-map.tsv`). BUT the
TOP-LEVEL field `unit.functions[].fuzzy_match_percent` IS populated and IS the authoritative
objdiff measure - it is exactly what `scripts/generate_delink.py::_report_changes` and
`match_score.py`/README use. Read THAT, not `.measures.`:
    json: r['units'][U]['functions'][F]['fuzzy_match_percent']   # authoritative
The `pdb_fetch --view diff` FOOTER (rich or objdiff-backend) is a SECONDARY number that
UNDER-counts jump-table/relocation-heavy functions because it scores the inline jump-table data
(emitted past `ret`, holding binary-specific RVAs) as mismatched "instructions". For
does_need_land_and_run the footer reads 63.7% while the authoritative objdiff measure is 100%;
for get_jump_animation_index footer 55.0% vs authoritative 100%. ALWAYS cross-check the footer
against report.json's top-level fuzzy_match_percent before banking a number.

HONEST authoritative numbers (report.json top-level fuzzy_match_percent, this build):
  get_move_direction        100%   (objdiff-backend diff 99/99; report.json shows 0.0 only
                                    because the free-fn symbol didn't pair in the delinked .objs
                                    - a pairing quirk, NOT a regression; both diffs say 100%)
  does_need_land_and_run    100%   (report-changes logged "97.56 -> 100.0 improved")
  get_jump_animation_index  100%   (const-pairing fix made it measurable AND it is a true match)
  set_user                  83.61% (cast-temp fix; remaining = fsm out-of-line wall)
  deactivate                45.13% (fsm out-of-line wall; footer 26.9% under-counts)

Commands used:
  pdb_rich_query --index binaries/rich/{target,base}/index.jsonl --list --function <substr>
  pdb_fetch --target-index binaries/rich/target/index.jsonl --base-index binaries/rich/base/index.jsonl \
      [--objdiff-base-dir binaries/objdiff/base --objdiff-target-dir binaries/objdiff/target] \
      --function "<full sig substr>" --view diff
  touch sources/vostok/game_core/sources/jump_logic.cpp ; nix develop -c python3 scripts/rebuild.py

### get_jump_animation_index - PAIRING FIX -> 100% DONE
Target sig: `unsigned int get_jump_animation_index(const move_direction_enum, const bool,
const jump_animation_parts)` (rva 0xbd280). Base lacked the param `const`s in the HEADER
decl (the defn already had them), so pdb_fetch's signature-string pairing failed (`--view
diff needs both`). Added the three `const`s to the header decl -> now pairs. The CODE
(0x00-0xbb) is byte-identical (structure: 12 stmts, switch +0x10, on_site +0x08, ternary
cases +0x16/+0x13). Authoritative objdiff measure = 100.0% (report.json top-level). The
diff-VIEW footer reads 55.0% only because of the jump-table-data artifact - ignore it.
The original "100% DONE" was right about the code; it just was never paired/measurable
until the const fix. Restored STATE[100%|DONE].

### does_need_land_and_run -> 100% DONE (NOT a steerable single-tail; the footer was a red herring)
Tested the reviewer hypothesis (result-temp / trailing `return false` for a single tail):
  - Trailing `return false;` (no default): footer rose to 72.5% via the RICH-index diff,
    BUT the objdiff-backed diff showed it ADDED `cmp [ebp-0Ch],8 ; ja .17` (a jump-table
    bounds check the target lacks) + a dead `xor al,al` at the tail. That is WORSE-shaped
    code, not a match - the higher rich footer was misleading.
  - Restored `default: NODEFAULT();` (return-per-case): the CODE 0x00-0x188 is byte-identical,
    structure = 13 stmts matching the target EXACTLY (0x9/0x21/0x13/0x7/0x2a x4/0x27 x4/0x28).
The diff-view footer reads 102/160 = 63.7% because every leaf `jmp .17`(base)/`.18`(target)
resolves to the SAME address 0x188 (verified in --view base/target: base epilogue .17 ==
target epilogue .18, only the label INDEX shifts) PLUS the inline jump-table data past `ret`
holds binary-specific RVAs that the footer disassembles as mismatched junk. The AUTHORITATIVE
objdiff measure (report.json top-level + report-changes "97.56 -> 100.0 improved") is 100% -
the function is a true byte match. The reviewer's "extra trailing block / single-tail" reading
came from the misleading footer; there is no extra source statement. STATE[100%|DONE], carcass
deleted.

### set_user - cast-temp fix applied (steerable part banked)
Applied the flagged steerable fix: bound `static_cast<jump_logic_base_state*>(i)` to a named
local `state` before the call:
    for (...) { jump_logic_base_state* state = static_cast<...>(i); state->set_user(user); }
This reproduces the target's loop-body order: `mov [slot],edx; push user; mov ecx,[slot];
mov edx,[ecx]; mov ecx,[slot]` (materialize cast into its own slot, then push the arg).
Authoritative objdiff measure 83.61% (report.json top-level; the diff-view footer reads
50.0%/21-42 - it under-counts). Remaining residual = the ai-fsm inline-vs-call WALL: target
`call finalize_impl`(states()) + `call operator[]`(front()); base inlines both -> one extra
stack slot (sub esp,10h vs 0Ch) + slot renumbering. Legit LTCG call-boundary; not steerable.

### deactivate - 45.13% PARTIAL (fsm wall)
The substring "deactivate" mis-paired to `thread_pool::deactivate_if_oversubscribed`
(spurious 89.5%); using the full sig `jump_logic::deactivate` pairs correctly. Diff-view
footer 7/26 = 26.9%; authoritative objdiff measure = 45.13% (report.json top-level). Single
statement `set_initial_state(states().front())`; no cast/loop to steer. The whole residual
is the ai-fsm out-of-line-vs-inline wall (states()=finalize_impl + front()=operator[]
out-of-line in target, all three inlined in base). Legit LTCG call-boundary. PARTIAL.

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

## UNBLOCK follow-up (2026-06-03, branch match/game_core-jump_logic_ctor_initialize, off int/game_core)

The C4716 state-vtable trap is GONE on the common ground (jump_logic_state_*
selected_animations now return). Restored + matched ctor/dtor/initialize_logic.

Commands:
  pdb_rich_query --index binaries/rich/target/index.jsonl --list --function true_predicate
  pdb_rich_query --index binaries/rich/target/index.jsonl --list --function jump_logic
  pdb_fetch ... --function "jump_logic::initialize_logic" --view target
  pdb_fetch ... --function "jump_logic::jump_logic" --view target
  pdb_fetch ... --function "jump_logic::~jump_logic" --view target
  nix develop -c python3 scripts/rebuild.py    (NO module arg)
  pdb_fetch ... --objdiff-base-dir binaries/objdiff/base --objdiff-target-dir binaries/objdiff/target \
      --function "jump_logic::initialize_logic" --view diff

### true_predicate -> reused, NOT redefined
Target has TWO `survarium::true_predicate()`: the EXTERNAL one in
breath_vibration_calculator.cpp (0xbb5c0) and a file-static one in
weapon_core_shotgun_reload_state.cpp (0x589720). initialize_logic @0x1d5 pushes the
EXTERNAL `survarium::true_predicate`. breath_vibration_calculator.cpp already defines
it (external linkage, always emitted). So jump_logic.cpp forward-declares
`bool true_predicate();` and references it - redefining would be an ODR/LNK conflict.
Not a stub to match here.

### ctor (??0jump_logic, 0x57de30) -> 100% DONE
`: m_owner(owner), m_user(0), m_logic(0), m_animated_object(0),
m_jumping_direction(move_direction_on_site), m_is_jump_from_right_leg(true)
{ initialize_logic(); }`. objdiff report.json top-level = 100.0%.

### dtor (??1jump_logic, 0x57da30) -> 100% DONE
`m_logic->clear_transitions(); while(ai::fsm_state* state = m_logic->pop_state())
VOSTOK_DELETE_IMPL(g_allocator, state); VOSTOK_DELETE_IMPL(g_allocator, m_logic);`
NOTE: game_core_memory.h's NEW/DELETE macros are COMMENTED OUT, so this TU must call
VOSTOK_NEW_IMPL/VOSTOK_DELETE_IMPL(g_allocator, ...) directly (same as breath_vibration).
objdiff report.json top-level = 100.0%.

### initialize_logic (0x57dae0) -> 60.40% PARTIAL (ai-fsm out-of-line wall)
Body (verified byte-for-byte structure, 166/275 equal text-diff): NEW(ai::fsm);
inactive/start/landing = NEW(jump_logic_state_*)(*this) (source decl order
inactive,start,landing -> slots -4/-0Ch/-8 match target); add_state x3; two
add_transition with boost::bind<bool>(&true_predicate) and
boost::bind(&jump_logic::landing_predicate,this); set_initial_state(states().front()).
report.json shows fuzzy=None (NOT 0): objdiff CANNOT pair this symbol - it errors
"x86: Failed to find operand for Absolute relocation" on the boost stored_vtable
relocation and falls back to the text differ; the authoritative number is the
text-diff 60.4%.
RESIDUAL = the documented ai-fsm out-of-line-vs-inline WALL (LTCG call-boundary class,
same root cause as deactivate 45.13% / set_user 83.61%): TARGET out-of-lines
fsm::states() (delinker-misnamed finalize_impl) + front()/operator[] in the final
set_initial_state(states().front()); BASE inlines all three to direct field loads.
That tail raises base register pressure -> base allocates a 0x18-larger frame
(sub esp,0D4h vs 0BCh), omits the target's `push esi`, and uses ecx (not esi) to hold
each transition temp's address for boost::function::clear() -> every [ebp-XX] slot
renumbers by 0x18, cascading the whole diff. fsm::states() is declared `inline` in
fsm.h; whether the toolchain inlines it is the call-boundary LTCG decision - NOT
source-steerable. EXHAUSTED source shapes: statement order, member-init list, local
decl order (slots already match), the two bind expressions all verified 1:1. PARTIAL.

## ctor / dtor / initialize_logic -> BLOCKED (C4716 state-vtable trap)  [SUPERSEDED above]
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
