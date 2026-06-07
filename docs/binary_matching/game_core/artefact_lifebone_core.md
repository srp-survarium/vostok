# artefact_lifebone_core - structure-verifier (v2) trail

Worktree: vostok_3, branch chain/game_core-sv-artefact_lifebone_core.
Diagnosed each matched-but-<100% function with
`pdb_fetch --view structure-diff --condensed` (+ `--view diff` / `--view target|base`).

report.json fuzzy_match_percent at start:
- ctor      ??0... 99.81%
- dtor      ??1... 92.12%
- load_core         92.12%
- activate_impl     12.375%
- switch_passive_mode_impl 46.30%
- action            96.14%   (source STATE said 75.57% - stale, corrected)

## ctor  (99.81% DONE)
structure-diff: 5/5 stmts, 0 size-diffs, 0 quantity-diffs -> STRUCTURE MATCH.
asm diff: every instruction identical except the frame size: target `sub esp,0FCh`
(this at `[ebp-0FCh]`) vs base `sub esp,0F4h` (`[ebp-0F4h]`). An 8-byte extra temp
slot in the target, almost certainly a boost::bind temporary the target keeps live.
No `empty_stub`/ASSERT in the body. Frame-size / slot noise on an otherwise perfect
match - not source-steerable here. Banked DONE.

## dtor  (92.12% PARTIAL -> STRUCTURE MATCH, wall)
Empty source body (0/0 stmts) so structure is trivially matched. The byte diff is
entirely compiler-generated MI-destructor code: the target re-writes the primary
vtable pointer a SECOND time before chaining to ~artefact_base:
  0x45 mov eax,[ebp-Ch]; 0x48 mov dword ptr [eax],0
base emits only the first vtable store (0x0c). This second store is the MI-dtor
vtable re-establishment MSVC inserts between destroying the secondary base
(damage_protector) sub-object and the primary base (artefact_base). It is driven by
the class hierarchy/vtable layout, not by the (empty) dtor source, so it is not
steerable from this file. The old STATE note ("there is also an artefact_lifebone
class") could NOT be confirmed - no such symbol in the target index; the real cause
is MI-destructor vtable management. CONFIRMED WALL.

## switch_passive_mode_impl  (46.30% PARTIAL)
structure-diff before: target 14 / base 16 stmts, quantity-diffs 2 - base had an
EXTRA `}` after register_body_part_damage_protector (the `if (switch_on) { ... }`
outer braces). The target's `else` (L66) sits directly after register with no
separate `}` statement -> the target did NOT brace the if/else branches (each branch
is a single `for` statement).
FIX: dropped the outer `{ }` on both the `if` and `else` branches (the for-loops are
each a single statement). Rebuilt: now target 14 / base 13, quantity-diffs 1, and the
remaining quantity-diff is only an `EMPTY only target` blank-line run (cosmetic). The
spurious block is gone.
Residual (unchanged %): the SIZE diffs on `damage_model_ptr dm = ...holder()...` and
each `dm->...` call. Target keeps `inventory::holder()` (out-of-line symbol at target
0x86b70), the `intrusive_ptr` copy-ctor and `operator*` OUT-OF-LINE; our base inlines
them. This is the same accessor inline-vs-call LTCG wall the codebase already banks
(see inventory_holder::inventory() note). Forcing them out-of-line would require
de-inlining shared header accessors across all of game_core - out of scope for this
unit and against the established convention. STRUCTURE MATCH, wall.

## activate_impl  (12.375% PARTIAL)
structure-diff: 6/6 stmts, quantity-diffs 0 -> STRUCTURE MATCH. Two SIZE diffs:
- `damage_model_ptr dm = m_inventory->holder().damage_model();` target 0x2c / base 0x76
- `body_part_parameters* bp = dm->get_body_part(...)`            target 0x1d / base 0x28
Both are the same accessor inline-vs-call wall as switch_passive. The target
(--view target) is clean: `call inventory::holder` (out-of-line) + virtual
damage_model() + `call intrusive_ptr::intrusive_ptr` (out-of-line copy-ctor) + later
`call intrusive_ptr::operator*`. Base inlines holder(), inlines the intrusive_ptr
copy/refcount (the dec/inc block) and operator*, blowing up the byte size. The low %
is just the small function being dominated by inlined refcount code. Not steerable
without de-inlining shared accessors. STRUCTURE MATCH, wall.

## action  (96.14% DONE)
structure-diff: 12/12 stmts, quantity-diffs 0 -> STRUCTURE MATCH (braces correct).
Sole SIZE diff: `inventory_item::set_amount( amount()-1 )`. Target out-of-line call
with thiscall+stack: `... ; push eax ; mov ecx,[ebp-4] ; call set_amount`. Base
passes the arg in a register (LTCG custom calling convention): `mov eax,[ebp-4]; call`
with the value already in ecx. This is the bankable call-boundary LTCG arg-passing
exception (register vs stack slot). STATE corrected 75.57% -> 96.14% DONE.

## load_core  (92.12% DONE)
structure-diff: 4/4 stmts, quantity-diffs 0 -> STRUCTURE MATCH. Sole SIZE diff:
`inventory_item::set_amount( (u16)amount )`. Same LTCG arg-passing as action: target
`movzx eax, word ptr [ebp-4]; push eax; mov ecx,[ebp-28h]; call set_amount` (stack
arg + u16 truncation at the call site), base passes in register. There IS an
out-of-line set_amount in the target (0x869d0, inventory_item.h) so both CALL it; only
the calling convention differs. Bankable LTCG arg passing. STATE 95% -> 92.12% DONE.

## Summary of edits
- switch_passive_mode_impl: removed spurious if/else outer braces (real structure fix).
- All functions: deleted `// FUNCTION BODY` carcasses; embedded condensed structure-diff
  + one VERDICT each; corrected stale/mislabeled STATE lines to report.json.
- No body-logic change beyond the switch_passive brace drop.
