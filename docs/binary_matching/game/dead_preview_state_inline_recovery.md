<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Dead and preview states: inline construction and real transition gate

PR 571 source-only batch. Six stale no-source markers are removed through
constructor/caller/vtable evidence. One real behavior discrepancy is corrected:
the dead state must return **true**, not false, from its readiness override.
No build or score refresh has been run.

## The readiness bug and its actual consumer

Independent exact-name target fetch for
`weapon_user_dead_state::is_ready_for_transition` gives RVA 0x12700,
`b0 01 c3` (`mov al,1; ret`). An independent frozen-base fetch gives
`xor al,al; ret`. This is not a two-sided ICF alias pairing verdict.

The retail constructor expansion installs vtable RVA 0x9629f8. Its slot 4
really points to RVA 0x12700, confirmed by raw PE reads. `ai::fsm::tick` calls
the current state's slot +0x10 and skips the outgoing-transition loop when
it returns false. `weapon::load_weapon` constructs outgoing dead-to-other-state
transitions bound to `is_alive`, in addition to incoming `is_dead` transitions.
Thus the false implementation suppresses those normal FSM exit transitions;
retail allows their predicates to be evaluated. This is a code-path conclusion,
not a claimed in-game reproduction after the unbuilt change.

## Individual register: 15 declarations/consumer anchors

| # | Function | Disposition |
|---:|---|---|
| 1 | `weapon_user_dead_state` constructor | Remove marker and obsolete placeholder warning. `load_weapon` RVA 0x5b1bd0, statement 5: base owner is weapon+0x278, ID argument is zero (`type_stand`), weapon reference at +0x28 and seed zero at +0x2c. Existing initializer/default random construction already does this; no invented dead enumerator. |
| 2 | Dead `initialize` | Remove marker; installed vtable slot 1 and exact-name procedure are bare return RVA 0x3f210. |
| 3 | Dead `execute` | Remove marker; installed slot 2 is the same bare return. No animation/recoil update is added here. |
| 4 | Dead `finalize` | Remove marker; installed slot 3 is the same bare return. No borrowed weapon cleanup is added. |
| 5 | Dead `is_ready_for_transition` | Fix false to target true. Direct method and installed slot independently agree; actual FSM/transition consumers establish why the gate matters. |
| 6 | Dead `selected_animations` | Existing operations retained. Target 0x9b400 selects first-view random index or first-view-count plus third-view random index, updating seed at +0x2c; uses trailing weapon array, freeze-at-end time callback, 0.5 time scale and inherited user pointer. Original mutable-versus-cast spelling remains open. |
| 7 | Dead implicit destructor | Keep generated. Its exact-name deleting wrapper at 0x1134a0 conditionally calls operator delete and returns this; no borrowed weapon or random-state destruction work. |
| 8 | `player_logic_preview_state` constructor | Existing initializer list retained. Standalone 0x767950 passes ID 4 (`type_preview`), stores borrowed array/count and zero seed; load_weapon independently emits the same fields. |
| 9 | Preview `initialize` | Existing unmarked empty body verified through installed vtable slot 1. |
| 10 | Preview `finalize` | Remove marker; installed slot 3 is bare return 0x3f210. |
| 11 | Preview `execute` | Remove marker; installed slot 2 is bare return 0x3f210. |
| 12 | Preview `selected_animations` | Existing retained method at 0x767980 advances seed at +0x30, selects from borrowed array/count, builds the interpolator/lexeme with user and body-part mask. Keep the real file-static s_aim_transition_time, verified 0.3f. Const-mutation spelling remains open. |
| 13 | Preview implicit destructor | Keep generated. Exact-name deleting wrapper at 0x1134a0 only conditionally frees this. It does not free the array owned by the weapon allocation. |
| 14 | `weapon::load_weapon` state construction/transitions | Both live construction sites and bidirectional death/alive bindings verified. Keep model/locator/state installation order. Preview argument/allocation statement partition remains an explicit source question, not a claimed LTCG wall. |
| 15 | `ai::fsm::tick` dispatch consumer | Verify readiness through +0x10 gates predicates; finalize/initialize/execute dispatch remain untouched. This explains the real dead-state bug without changing the shared FSM. |

## Installed virtual slots, not inferred empty bodies

The dead vtable comes from the immediate at load_weapon +0xf2; preview's comes
from standalone constructor +0x14. Raw first ten slots (RVAs):

| Slot | Dead table 0x9629f8 | Preview table 0x963af0 |
|---:|---|---|
| 0, deleting destructor | 0x1134a0 | 0x1134a0 |
| 1/2/3, initialize/execute/finalize | 0x3f210 | 0x3f210 |
| 4, readiness | 0x12700 | 0x12700 (inherited) |
| 5, set_user | 0x58c200 | 0x58c200 |
| 6, selected_animations | 0x9b400 | 0x767980 |
| 7, attachment transform | 0x112e20 | 0x112e20 |
| 8/9, serialization defaults | 0x12c50 | 0x12c50 |

RVA 0x3f210 starts `c3 cc cc ...`: it is a return followed by padding.
Readiness starts `b0 01 c3 cc ...`. This proves the six empty hooks independently
of their shared symbol spelling. The attachment-transform trap remains the
separate open base-state question; this audit does not turn its fake return
into a recovered body merely because the vtable points there.

## Data, structure and remaining source questions

Direct PE operand reads verify death time scale 0.5f at data RVA 0x948db4 and
preview transition time 0.3f at 0x92d484. Neither constant changes. Each random
step uses `seed = seed * 0x08088405 + 1` and unsigned high-half multiplication
by the selected range. Current mutable random fields support observed writes
from const methods; raw types do not decide mutable versus a source const_cast.
Both questions have lean source and review-table anchors.

Frozen topology reports one identical semantic variant for each state, with
public constructors and private overrides. No access change, new virtual,
handwritten destructor or friendship is introduced.

The frozen load_weapon structure comparison is target 16 statements/0x2c8,
base 15/0x2b9. Target statement 11 (+0x243) loads both death-animation counts;
statement 12 allocates/constructs preview, then statement 13 installs it. Current
source combines the array expression with constructor arguments. A separate
array-pointer or count expression is plausible, but no target named local
selects its spelling. Do not invent a local solely to satisfy the line count,
or label this source partition discrepancy a linker wall. Resolve the boundary
when further source evidence or the deferred batch comparison is available.

Later validation must check the corrected readiness method and callers,
constructor seed/owner/ID stores, installed vtables and preview partition.
This batch does not claim any new measured byte score or successful build.
