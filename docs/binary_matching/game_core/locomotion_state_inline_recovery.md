<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Locomotion and jump state inline hooks

PR 571 source-only review: existing bodies, implicit destructors and actual
FSM dispatch sites, not merely standalone-symbol presence. No builds or new
score claims. This batch removes eight literal markers without inventing work
for verified empty hooks.

## Individual register: 25 declarations/consumer anchors

| # | Function | Disposition and evidence |
|---:|---|---|
| 1 | `player_logic_stand_state::initialize` | Empty body verified by its constructor-installed vtable slot 1 and method record, both selecting RVA `0x1a800`. Remove stale marker. |
| 2 | `player_logic_stand_state::finalize` | Same verified empty body in slot 3. Remove stale marker; do not invent stand_up here. Crouch finalization owns that operation. |
| 3 | `player_logic_stand_state::execute` | Same empty body in slot 2, reached by FSM dispatch. Remove stale marker; selection logic remains in selected_animations and state transitions. |
| 4 | Stand implicit destructor | Keep generated. Slot 0 is scalar-deleting wrapper `0xb6330`: empty base cleanup followed by flag-controlled operator delete. No new destructor body or state removal. |
| 5 | `player_logic_crouch_state::execute` | Verified empty slot 2 at `0x1a800`; remove marker. Existing initialize/finalize remain nonempty, calling player crouch/stand_up respectively. |
| 6 | Crouch implicit destructor | Keep generated; same deleting-wrapper fold as stand, backed by its own constructor vtable. |
| 7 | `player_logic_sprint_state::execute` | Verified empty slot 2; remove marker and stale 7-byte/objdiff narration. Its actual retained empty method is 11 bytes. Initialize/finalize still own stamina subscription and callbacks. |
| 8 | Sprint implicit destructor | Keep generated. Ordinary destructor RVA `0xbd010` clears subscriber callback at +0x68, finalize callback at +0x48, initialize callback at +0x28, then base cleanup. Scalar wrapper `0xbcfe0` owns conditional deletion. Do not add unsubscription here instead of existing finalize. |
| 9 | `jump_logic_base_state::~jump_logic_base_state` | Existing empty user body is correct: implicit member cleanup still destroys m_animation at +0x20 and runs FSM base cleanup. Remove marker, not the destructor declaration/definition. Retail also reinstalls this abstract base vptr. |
| 10 | `jump_logic_base_state::set_user` | Existing +0x1c pointer setter retained. RVA `0xbd210` stores the reference address; jump_logic::set_user iterates states and calls this virtual. |
| 11 | `jump_logic_base_state::is_jump_finished` | Existing +0x25 getter retained. `jump_logic::is_jump_finished`, RVA `0x57d610`, reads that byte from the current FSM state. It is distinct from transition readiness. |
| 12 | `jump_logic_base_state` protected constructor | Existing owner-reference/user-null/resource-default/interval-0xff/finished-false construction retained. RVA `0xbd230` explicitly installs its own vptr after FSM construction. Do not add NOVTABLE here. |
| 13 | `jump_logic_state_inactive` inline constructor | Existing base forwarder retained. `jump_logic::initialize_logic` RVA `0x57dae0`, statement 2, allocates 0x28, calls base construction, then installs the inactive vtable. |
| 14 | Inactive `initialize` | Existing unmarked empty body verified in slot 1 at `0x1a800`. |
| 15 | Inactive `finalize` | Empty slot 3 verified; remove marker. |
| 16 | Inactive `execute` | Empty slot 2 verified; remove marker. |
| 17 | Inactive `is_ready_for_transition` | Existing true retained. Slot 4 is RVA `0xd23e0`, explicitly returning AL=1. |
| 18 | Inactive implicit destructor | Keep generated. Scalar wrapper `0xbda80` reinstalls jump-base vptr and destroys the resource before conditional deletion. |
| 19 | Jump-start `execute` | Existing unmarked empty body retained; slot 2 points at `0x1a800`. Remove wrong comment claiming the folded public representative makes this override public. |
| 20 | Jump-start `is_ready_for_transition` | Existing m_jump_interval_ended getter retained. Slot 4 at `0xbd480` loads +0x2e, not physics-jumped or whole-jump-finished. |
| 21 | Jump-start implicit destructor | Keep generated. Scalar wrapper `0xbd780` destroys preface animation at +0x28, then reinstalls jump-base vptr and destroys base animation at +0x20, then handles deletion. |
| 22 | Jump-landing `execute` | Existing unmarked empty body retained; slot 2 points at `0x1a800`. Its override is private regardless of another symbol folded there. |
| 23 | Jump-landing `is_ready_for_transition` | Existing false verified. Slot 4 at `0xd2040` explicitly returns AL=0. Landing completion is an animation-callback flag consumed through is_jump_finished, not a transition back to inactive here. |
| 24 | Jump-landing implicit destructor | Keep generated. Same `0xbda80` deleting-wrapper fold as inactive/base, with one managed animation member to destroy. |
| 25 | Inactive `selected_animations` retained body | Keep existing shipping unreachable model. RVA `0x57ce30` returns with `ret 0x14` without writing hidden result storage; it does not construct a default expression pair. Original unreachable/assertion source spelling remains open. |

The jump_logic getters already recovered in earlier work are not counted
again. The detailed sprint subscriber constructor repair remains in the
[stamina register](stamina_inline_recovery.md), not claimed as a new fix here.

## Vtable proof, not an alias-name guess

Read the immediate table address installed by each retained constructor, then
read its first five dwords from the executable. These are RVA values:

| State | Table | deleting dtor | initialize | execute | finalize | ready |
|---|---:|---:|---:|---:|---:|---:|
| stand | `0x968928` | `0xb6330` | `0x1a800` | `0x1a800` | `0x1a800` | `0x12700` |
| crouch | `0x968954` | `0xb6330` | `0x57ed00` | `0x1a800` | `0x57ece0` | `0x12700` |
| sprint | `0x968988` | `0xbcfe0` | `0x6e9620` | `0x1a800` | `0x6e95d0` | `0x12700` |
| jump inactive | `0x9697f0` | `0xbda80` | `0x1a800` | `0x1a800` | `0x1a800` | `0xd23e0` |
| jump start | `0x969818` | `0xbd780` | `0x6e8c10` | `0x1a800` | `0x6e8a00` | `0xbd480` |
| jump landing | `0x969840` | `0xbda80` | `0x57cf30` | `0x1a800` | `0x57ce40` | `0xd2040` |

The table addresses come from constructor RVAs `0x7706e0`, `0x57ed20`,
`0x6e9510`, `0x6e8bd0`, `0x57cef0`, and inactive construction in
`0x57dae0 + 0xad`. The shared empty body bytes are exactly
`55 8b ec 51 89 4d fc 8b e5 5d c3` (11 bytes). Its first disassembler label
may name Boost shutdown_service; that label does not determine which state
uses it. The named method records and installed tables establish ownership.
An independent base fetch of stand initialize yields the same empty body.

Retail `ai::fsm::tick` calls ready through +0x10, finalize through +0xc,
initialize through +4 and execute through +8. These match the table slot
interpretation. Source state setup adds inactive/start/landing and the two
outgoing transitions inactive→start and start→landing; no landing outgoing
transition is introduced by this review.

## Lifecycle consequences

Crouch initialization calls the player virtual at +0x40 and finalization at
+0x44. Sprint initialization subscribes its embedded stamina listener before
the existing callback; finalization unsubscribes before its callback. These
are not empty just because execute is empty. Landing finalization itself is
a retained framed empty function at `0x57ce40` and remains so.

Jump-start initialization clears animation references/readiness flags and
registers interval/jump callbacks. Its interval callback sets +0x2e according
to the matching animation/interval. Landing initialization selects its landing
variant and clears +0x25; the matching completion callback sets +0x25 and
removes the interval callback. Keep these two state flags distinct.

The inactive selection body is an exceptional case, not a template for
implementing every empty method as unreachable. The observed FSM setup can
leave inactive before selecting an animation, and the retained selection
procedure leaves its output untouched. That is consistent with the existing
unreachable source model, but does not recover an exact assertion macro or
prove safety for every possible external call sequence. It stays an open
source-form question rather than gaining a fabricated return value.

The abstract jump base is **not** a NOVTABLE candidate: constructor RVA
`0xbd230 + 0x14` installs its table, and deleting wrappers reinstall it before
member cleanup. The FSM base's different attribute does not justify removing
these stores. Generated destructors preserve ownership; an empty user body
does not mean omitting implicit resource/callback destruction.

## Access and verification

All seven queried class names have the same semantic variant in frozen target
and base. Stand/crouch/start/landing each have two equal target records versus
one base record, so topology reports record multiplicity, not an access/layout
change. Sprint/base/inactive records are identical. Target class methods put
the overrides under private; jump-base constructor/data are protected. Remove
comments inferring access from an ICF representative or only from byte effects.

Validation: target/base one-sided method fetches, raw constructor/table/code
reads, dispatch and callback consumer inspection, frozen class topology and
`git diff --check`. No codegen changes or new byte percentages are claimed for
this annotation/source-classification batch; no build was run. The broader
animation-expression builders and existing callback/source uncertainties are
not declared closed by this register.
