<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Menu inline and callback-boundary recovery

Source-only continuation above `7326c76c5` in PR 571. **62 additional register
entries** cover all 18 in-class bodies in the six menu/handler classes, the
separately defined inline lobby login getter, generated operations and retained
consumer anchors. No build, ledger refresh or post-edit matching claim.
Addresses below are retail RVAs unless labeled frozen base.

The real behavior correction is login input priority **0 → 100**. Three lobby
external-handler null-reference getters gain the existing game/network-client
forwarding paths; eight lobby and five messaging accesses in its callback now
use those seams. Two network notifications similarly call the public lobby
methods instead of duplicating their private-field logic through friendship.
Original named boundaries remain explicit questions, not proof from equivalent
operations alone. The missing carry predicate and main-handler callback remain
individually managed open bodies.

## Individual register

Ranges enumerate distinct generated methods, not new recovered algorithms.

| # | Function | Evidence and disposition |
|---:|---|---|
| 1 | `login_menu::input_priority` | Constructor-installed retail input slot returns 100; independently selected frozen base returns zero. Correct the literal, not input ordering elsewhere. |
| 2 | `action_blocked() const` | Existing unmarked nonzero test reads this class's private timer at `+0xc0`. Login callback `0x993c0 +0x4d` checks exactly that field, not the shadowed base timer. Preserve predicate, const and early-return consumer. |
| 3 | `get_bullet_manager() const` | Target `0x327c0` is `xor eax,eax; ret`. Preserve the null-reference result; do not route through game merely because that seems useful. |
| 4–5 | Implicit copy constructor; assignment | Raw generated public nonvirtual records `0x20f52/0x20f60`, attributes `0x103`. Keep implicit base/member operations, no new copying API. |
| 6 | Explicit constructor | `0x6f2e00`: base scene, derived vptrs, own timer zero, status 4, two empty resource handles and resource query. Existing initialization retained. |
| 7 | Explicit virtual destructor | `0x6f2be0`: generated cleanup releases both movie handles, then base scene. Existing empty source body is appropriate; do not duplicate member cleanup. |
| 8 | `set_status` | `0x6f29f0` writes current time plus 5000/10000 to own timer for selected failures. Confirms predicate meaning; preserve the actual status-dependent policy. |
| 9 | `tick` | `0x6f2920 +0x3f..0x5b` tests timer, clears expired timer and reenables button. Existing base tick/movie advancement remains; do not replace deadline comparison with a different clock test. |
| 10 | `on_activate` | Retained base activation then registration of input subobject `+0xbc`. Priority correction is live through this consumer. |
| 11 | `on_deactivate` | Existing base deactivation and removal of the same input subobject retained. No added timer reset. |
| 12 | `on_resources_ready` | `0x794a20 +0x289`: allocate 16-byte handler, call base constructor, install derived vptr and bind game/menu references, then register with movie. Verifies handler initialization in an actual expansion. |
| 13 | `login_menu_external_handler` inline constructor | Remove stale marker from the two-reference initializer list; consumer above stores game at `+8` and login menu at `+0xc`. No new ownership or manual vptr store. |
| 14–16 | Handler implicit copy; assignment; virtual destructor | Raw types `0x20f69/0x20f70/0x20f6e`; attributes `0x103/0x103/0x107`. Keep generated borrowed-reference/base cleanup, not explicit deletion of game/menu. |
| 17 | Handler `callback` | `0x993c0`: sign-in branch uses the timer predicate and real network client; exit branch delegates to engine exit. Existing dispatch retained; it is evidence for this handler, not the missing main-menu dispatch. |
| 18 | `main_menu::input_handler` | Target `0x82e90` and independent frozen base both perform nullable pointer adjustment to input subobject `+0xbc`. Existing `return *this` retained. Introducing virtual, not an input-interface override. |
| 19 | `input_priority` | Installed target slot `0x82ea0` and frozen base `0x9ba40` both return 100. Existing body retained. |
| 20 | `get_bullet_manager() const` | Same target null result `0x327c0`; remove false “real body reaches manager through m_game” comment and stale marker. No functional change. |
| 21–22 | Implicit copy; assignment | Generated types `0x18de4/0x18dee`, attribute `0x103`. Preserve compiler-generated base operations. |
| 23 | Explicit constructor | `0x5c9cb0`: base scene, three derived vptrs, query resources. No external-handler construction added. |
| 24 | Explicit virtual destructor | `0x5c9aa0` installs derived vptrs and tail-calls base destruction. Existing empty source body retained. |
| 25 | `on_resources_ready` | `0x5c9ac0` installs render scene/view resources; current source has no main-menu movie or callback registration. This narrow consumer does not establish whole-program unusedness. |
| 26 | `query_resources` | Existing scene/view request pair and callback retained. No guessed SWF request introduced to make the unobserved handler live. |
| 27–28 | `on_activate`; `on_deactivate` | Existing base lifecycle and input registration/removal retained; same `+0xbc` handler subobject and priority contract. |
| 29 | `main_menu_external_handler` inline constructor | Keep game-reference/base-construction model supported by private field `+8` and both live sibling constructors. Original construction/registration consumer remains open. |
| 30 | Main handler `callback` | OPEN method-name/argument dispatch. No addressed class procedure or current construction/registration found; sibling login/lobby commands do not identify this UI's contract. Do not mark editor/server-only or invent a live registration. |
| 31–33 | Main handler implicit copy; assignment; virtual destructor | Raw types `0x4ad9c/0x4ada2/0x4ada0`, generated attributes `0x103/0x103/0x107`. Preserve generated cleanup despite missing addressed procedures. |
| 34 | `lobby_menu::input_handler` | Target `0x82e90` performs input-subobject adjustment `+0xbc`; existing body retained. |
| 35 | `input_priority` | Installed target and frozen-base slots both return 100. The 2012 lobby header independently supplies the same older source form. |
| 36 | `get_bullet_manager() const` | Target returns null at `0x327c0`; remove false m_game-forwarding narration from the existing unmarked body. |
| 37 | `selected_profile` | `callback 0x83aa0 +0xc9` reads byte `+0xe0`; relocation callback `0x833d0 +0x3dd` uses the same byte with profile stride `0x1b8`. Existing u8 getter verified, marker removed. |
| 38 | `can_player_carry(u32,u32,u32,float)` | OPEN slot-aware weight-delta/limit hypothesis. Layout and producers establish capacity/current weight, but not three integer argument roles, replacement/count policy, equality limit or actual invocation. False is not verified. |
| 39 | Private inline `login_client` | Definition already exists at the top of `lobby_menu_ui.cpp`. `update_status 0x733190 +0xa3` follows game/network pointer and virtual slot `+0x38`; keep the existing definition and sole-TU consumer. |
| 40–41 | Implicit copy; assignment | Raw `0x18958/0x1898c`, generated `0x103`. No custom copying of owned camera/UI/character pointers added. |
| 42 | Explicit constructor | `0x78c100`: base construction, member initialization, owned lobby camera/director switch, physics initialization and query. Existing ownership path retained. |
| 43 | Explicit virtual destructor | `0x78b0d0`: unregister active scheduler entries, destroy owned camera/character, release project, destroy UI/physics and bases. Preserve actual cleanup, not generic deletion of borrowed handler game references. |
| 44 | Private retained `lobby_client` | `0x6f1e70`: scene game `+0xa8`, game network `+0x3b8`, virtual slot `+0x3c`. Existing body in `lobby_menu_input.cpp` is a real sibling donor. |
| 45 | Private retained `messaging_client` | `0x6f1e50`: same path, slot `+0x44`. Keep distinct named client and reference return. |
| 46 | `on_render_scenes_ready` | `0x737070 +0x6d5` allocates 12-byte external handler, calls base construction, stores game `+8` and derived vptr. Earlier configuration supplies capacity; handler registered with lobby/match-making movies. |
| 47 | `player_parameters_ready` | `0x7352c0 +0x76` stores resource total weight to menu `+0x100`; later publishes total and capacity to UI. Producer evidence does not establish the missing carry predicate's argument contract. |
| 48 | `on_ui_destroy` | `0x733aa0` destroys owned external/relocation handlers and clears pointers. Generated virtual handler destruction is real; game references remain borrowed. |
| 49 | `on_connected_to_lobby` | Retained query-lobby-info followed by guarded flag-set/show-connected operations. Public method supplies precisely the operations duplicated inside network notification. Keep body and access. |
| 50 | `on_disconnected_from_lobby` | Retained guarded flag-clear/show-disconnected operations; public method supplies network notification's expansion. Keep body and access. |
| 51 | `update_status` | `0x733190 +0xa3` selects the private login getter path to publish account/server information. This is not a consumer of the external handler's separate login getter. |
| 52 | `lobby_menu_external_handler` inline constructor | Actual resource callback verifies game-reference/base construction. Remove stale marker; preserve order and generated cleanup. |
| 53 | Handler `lobby_client` | Replace null with game→network→virtual lobby client. Callback `+0x4d` verifies the path; use helper at eight existing accesses. Original named boundary stays indexed. |
| 54 | Handler `login_client` | Replace null with corresponding virtual login-client model; sibling menu consumer verifies path semantics but no invocation selects this handler's getter. No new call site invented. |
| 55 | Handler `messaging_client` | Replace null with game→network→virtual messaging client. Callback `+0x592` verifies the path; propagate at five existing accesses. Named boundary stays indexed. |
| 56–58 | Handler implicit virtual destructor; copy; assignment | Raw `0x4c796/0x4c78f/0x4c798`, generated `0x107/0x103/0x103`. Keep base destruction and borrowed game reference. |
| 59 | Handler `callback` | `0x83aa0`, 83 statements, 18 named locals. Thirteen existing client paths now use declared helpers, preserving dispatch, argument widths and operations. Login getter is deliberately not substituted into unrelated menu code. |
| 60 | `relocate_item_func::call` | `0x833d0` parses item moves, resolves weapon/ammo counterpart and checks category/slot compatibility. Target has no observed capacity/current-weight read here; do not invent an extra carry rejection merely because this is a plausible consumer. |
| 61 | `network_client::on_connected_to_lobby` | `0x5b6b20` expands the public lobby method's exact operations; restore one forwarding call, remove private access/local reconstruction. Original boundary and zero target line records remain open. |
| 62 | `network_client::on_disconnected_from_lobby` | `0x5b6af0` attributes lobby lookup plus entire guarded operation to one statement. Restore public forwarding call; original named boundary remains inferred, not a current byte claim. |

## Raw class and installed-slot evidence

Complete retail class/field-list pairs: lobby `0x1898f/0x1898e`,
`0x1a0e6/0x1a0e5`, `0x46ce7/0x46ce6`; main `0x18df1/0x18df0`,
`0x46ce3/0x46ce2`; login `0x20f63/0x20f62`, `0x46ceb/0x46cea`.
Keep variant records distinct: some method parameter type indices differ.
This pass does not claim whole-record or enum-variant equality.

All three menus publicly inherit the scene at zero and input handler at
`+0xbc`; own data is private. Login has a separate timer at `+0xc0`, even
though the base also has a timer. Both main/lobby input-handler getters are
introducing virtuals (`0x013`), while normal overrides and explicit menu
destructors carry `0x007`.

Handler pairs: login `0x20f73/0x20f72`, main `0x4ada5/0x4ada4`,
lobby `0x4c79a/0x4c799`. All have public flash base at zero, private
noncopyable at `+8`, private borrowed game reference at `+8`; login additionally
has private menu reference `+0xc`. Handler destructor attribute `0x107`
means generated **virtual**, not explicit empty source and not nonvirtual.

| Menu | Target input table / priority target | Frozen-base table / priority target |
|---|---|---|
| Login | `0x95d308 / 0x82ea0` (100) | `0x950488 / 0xbc470` (0) |
| Main | `0x95c520 / 0x82ea0` (100) | `0x94f6a8 / 0x9ba40` (100) |
| Lobby | `0x95d154 / 0x82ea0` (100) | `0x9502d4 / 0x9ba40` (100) |

Tables come from actual constructor stores; priority is slot `+0x18`.
The login structure-only report says MATCH with zero statements on both sides,
yet target is six bytes versus base three and the return values differ.
That result cannot exonerate a literal. All three bullet-manager procedures
instead share an actual null-return body; preserve it without claiming null
references are valid general-purpose C++ objects.

## Private access exposes a missing source boundary

The network notification reconstruction duplicated the lobby methods and
introduced `friend class network_client` to reach private query/flag/UI state.
Both public methods survive in retail with the exact same operations. Restoring
their invocation explains the single saved lobby pointer without fabricating a
caller local or broadening visibility. It also preserves query-before-flag-test
ordering and the false/true disconnected-message arguments.

Frozen connected notification has three body statements versus target zero;
disconnected has one on each side, but target's statement includes the lookup
that frozen base attributes elsewhere. No original-boundary certainty is
claimed from equivalent instructions. Both questions receive source TODOs and
deferred structure checks. Do not add forced inlining to make this assumption
self-fulfilling.

Remove both reconstructed friendships: network no longer needs private access;
game-world's existing project-loaded path already uses public
`show_match_making(false)`. This is consumer/access evidence, not an inference
from PDB omitting friendship. Remove stale comments asserting missing Scaleform
implementations or fictitious game-backed bullet managers.

## Two open bodies and a constructor model

`can_player_carry` is declared in the raw class, with three u32s and a float.
Capacity comes from `player/stamina_params/max_carried_weight`; current total
comes from the player-parameter resource. A likely contract predicts weight
after a move, accepting moves out of carried slots and testing additions or
replacements against the limit. Which integer denotes source slot, destination,
item/profile/count, whether the float is unit or total weight, and the exact
replacement/equality policy are not established. The retained relocation
consumer instead checks slot/category and compatibility. Keep the body open,
not an invented always-false rule or a newly inserted weight filter. The 2012
legacy lobby files have no corresponding method or weight fields.

Main handler's layout supports the existing borrowed-game initializer, and
both sibling constructors show base construction plus reference binding. No
current construction/registration or addressed main-handler procedure was
found; main resource setup only obtains scene/view. That supports a bounded
constructor model, not a verified empty callback or editor/server-only label.
The callback hypothesis is method-name dispatch to game/menu actions, but the
actual commands, argument schema and guards require a real consumer/UI contract.

## Checkpoint

Nine literal markers removed; two marked bodies stay open in this family.
Eight new source TODOs are indexed. Ten source files changed; all stored layout,
access and declaration order preserved apart from removing invented friendship.
No build, tool rebuild, SDK edits, new external-handler registration, README or
ledger refresh. Reviewed coverage increases by 62; this is not 62 new or matched
implementations. Recheck affected callers and generated cleanup together in the
eventual combined build.
