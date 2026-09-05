<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Scene and camera inline recovery

Source-only continuation above `eabb93c38` in PR 571. **45 additional entries**:
15 base-scene, six game-scene, seven director, four free-fly and 13 level-map
inline/generated/consumer operations. The nine previously reviewed
[`game_camera` inlines](camera_input_inline_recovery.md) supply evidence but
are not counted again. No build, ledger refresh or new match score.

Addresses are retail RVAs unless labeled frozen base. `Verified` means an
observed retail operation; an inlined field access alone does not establish
the original named wrapper boundary. `Model` means an explicitly provisional
implementation. Seven level-map bodies remain open, not recovered no-ops.

## Individual register

| # | Function | Evidence and disposition |
|---:|---|---|
| 1 | `base_game_scene(game&,scene const&,view const&)` | Model: use retained sibling's initialization and camera-director allocation, plus assignment of supplied resource handles to protected base members. Original consumer, initialization order and whether construction varied with supplied scenes remain open. No fabricated three-argument `game_scene` constructor. |
| 2 | `base_game_scene::show_ui(bool)` | Verified installed virtual slot `+0x14` selects `0x12c50`, `ret 4`. Existing empty default retained. |
| 3 | `is_mouse_over_ui()` | Verified installed slot `+0x18` selects `0xa2740`, false. Existing default retained; not inferred from absent direct callers. |
| 4 | `is_active() const` | Verified private `+0xb4` read in director `switch_to_camera`, `0x5bcc60 +0xcf` region. Keep current getter seam. |
| 5 | `get_game() const` | Verified director `apply`, `0x5bcda0 +0x1c` region: scene at director `+0x88`, game at scene `+0xa8`. Return borrowed reference. |
| 6 | `get_physics_world()` | Verified retained `player::get_physics_world`: load scene `+0x10f00`, then private scene field `+0xb0`. Current nonconst-scene consumers use this seam. |
| 7 | `get_physics_world() const` | Same-field getter model retained, but no consumer selecting this particular overload is established. A const player method with a nonconst scene reference is not proof. Explicit TODO. |
| 8 | `get_sound_scene()` | Verified NPC construction copies scene handle `+0x9c`; scene activation also forwards it. Preserve mutable handle reference, not a new getter-owned copy. |
| 9 | `get_camera_director()` | Verified `on_after_tick`, `0x5c73e0`, loads `+0xa0` and calls director apply; world constructor `0x5d0350 +0x35d` supplies the same pointer to free-fly construction. Preserve reference return. |
| 10 | `get_text_manager() const` | Verified network-client destruction accesses embedded scene's `+0xa4` through game `+0x13c` and checks null. Keep nullable pointer return. |
| 11 | `text_manager()` | Verified player insertion `0x5d4c90 +0x7d` region loads scene then `+0xa4` before `create_text_w`. Keep unchecked reference return distinct from the pointer getter. |
| 12 | Implicit base-scene copy constructor | Raw method `0x90ee`, attributes `0x103`. Keep implicit; PDB presence does not mean copying a noncopyable base is usable. |
| 13 | Implicit base-scene assignment | Raw `0x9119`, attributes `0x103`. Keep generated; no reference-rebinding or ownership policy invented. |
| 14 | Retained `base_game_scene(game&)` | Verified `0x5c6ea0`: zero resource handles and selected fields, store game reference, allocate `0x8c` director. Keep explicit source body and its initialization choices. |
| 15 | Explicit virtual base-scene destructor | Verified `0x5c6f20`: install derived vptrs, free/null director, release sound and then inherited view/scene handles. Raw `0x90f6`, attributes `0x013`, is explicit introducing virtual, not generated. |
| 16 | `game_scene::render_scene()` | Verified world `load`, `0x5d1a50 +0xd0`, checks resource at `+4`, matching existing getter/predicate consumer. Return const handle reference. |
| 17 | `render_scene_view()` | Verified base-scene `show_movie`, `0x5c7240 +0x51`, passes address `this+8`; `hide_movie` does likewise. Movie-argument copies are not scene-view getter copies. |
| 18 | Implicit game-scene default constructor | Raw `0x92c0`, attributes `0x103`; retained derived construction zeroes the two resource members. Keep implicit. |
| 19 | Implicit game-scene copy constructor | Raw `0x92bf`, attributes `0x103`. Preserve resource-pointer member copying; no handwritten raw copy. |
| 20 | Implicit game-scene assignment | Raw `0x92c3`, attributes `0x103`. Keep memberwise resource semantics. |
| 21 | Implicit game-scene destructor | Raw `0x92b8`, attributes `0x103`, nonvirtual. Derived destructor's folded callee releases `+8` then `+4`; do not add virtual destruction to this class. |
| 22 | `camera_director::load(config const&)` | Verified constructor-installed slot zero selects `0x12c50`, `ret 4`; remove stale no-source marker, retain empty default. |
| 23 | `get_active_camera() const` | Verified player tick `0x5c62b0 +0x6b0` and `+0x743` regions read director `+0x84`. Keep const camera pointer; older mutable-return spelling is not the retail signature. |
| 24 | `get_inverted_view_matrix() const` | Verified base-scene `apply_camera`, `0x5c72b0 +8`, copies director matrix `+4` to scene `+0x14`. Getter returns const reference. |
| 25 | `get_projection_matrix() const` | Verified same consumer `+0x1a`, director `+0x44` to scene `+0x54`. Getter returns const reference. |
| 26 | Implicit director copy constructor | Raw `0x90d0`, attributes `0x103`; keep implicit with noncopyable base. |
| 27 | Implicit director assignment | Raw `0x90e4`, attributes `0x103`; no new pointer ownership or scene-reference assignment. |
| 28 | Implicit director destructor | Raw `0x90df`, attributes `0x103`, nonvirtual despite other virtual methods. Scene owns/frees director storage; active-camera pointer is not deleted here. |
| 29 | `free_fly_camera::input_priority()` | **Real fix: 0 to 10.** Retail `0x868f0` returns 10; independently selected frozen base returns zero. Installed handler vtables confirm both identities. |
| 30 | Implicit free-fly copy constructor | Raw `0x1f2b5`, attributes `0x103`; keep generated, respecting base/member copy restrictions. |
| 31 | Implicit free-fly assignment | Raw `0x1f2c6`, attributes `0x103`; no hand-written queue/reference-copy policy. |
| 32 | Implicit virtual free-fly destructor | Raw `0x1f2be`, attributes `0x107`; deleting destructor `0x86df0` frees mouse/keyboard vector storage and optionally outer allocation. Do not invent focus unregistration or scene/director deletion. |
| 33 | `levelmap_camera(game_world&,camera_director&)` | World-binding model replaces null-reference base initializer with actual world upcast; retain invalid AABB initial value and bind both references. Original bounds initialization/construction consumer remain open. |
| 34 | `get_projection_matrix(float2 const&) const` | Open: likely top-down projection using bounds and window dimensions. Bounds producer, projected axes, fit/crop policy and clipping are unknown. Existing matrix placeholder is not verified behavior. |
| 35 | `on_keyboard_action(...)` | Open: investigate key/context selection and camera focus-switch bindings. No free-fly event-queue members exist here. False placeholder is not a recovered default. |
| 36 | `on_gamepad_action(...)` | Open: sibling policies differ (free-fly consumes some actions; lobby ignores). No basis for claiming false or copying one policy wholesale. |
| 37 | `on_mouse_key_action(...)` | Open: selection/focus/capture hypothesis needs bindings and owner. This class has no lobby-style capture flag; no field invented. |
| 38 | `on_mouse_move(...)` | Open: immediate pan/zoom versus ignored motion; axes, constraints and return policy need an actual consumer/expansion. |
| 39 | `input_priority()` | Sibling-camera model changes zero to 10, separately from the verified free-fly fix. Level-map's own constant and handler registration are unverified. |
| 40 | `tick()` | Open: static bounds-to-view update versus polling input; no timestamp fields justify transplanting free-fly timing logic. |
| 41 | `on_activate(camera_director*)` | Open: initial top-down pose and bounds producer may differ from base activation's director-state copy. Need actual bounds owner, orientation and depth. |
| 42 | `on_focus(bool)` | Sibling registration model: add/remove this input handler through world/game/input interfaces. Original invocation and optional empty base-hook delegation remain open. |
| 43 | Implicit level-map copy constructor | Raw `0x47e7a`, attributes `0x103`; keep implicit. |
| 44 | Implicit level-map assignment | Raw `0x47e89`, attributes `0x103`; no reference rebinding or invented bounds-copy API. |
| 45 | Implicit virtual level-map destructor | Raw `0x47e85`, attributes `0x107`; keep generated. Borrowed world/director references do not justify deletion or a custom unregistering destructor. |

## Raw PDB structure and ownership

Frozen target/base class topology is identical for base scene (`0xbc`), director
(`0x8c`) and level-map camera (`0x78`). This is existing-artifact evidence,
**not** a rebuild of this source batch. Raw class/field-list records:

| Class | Class TI | Field list | Important access/flags |
|---|---:|---:|---|
| `base_game_scene` | `0x911c` | `0x911b` | Public game-scene/engine bases; private physics-engine/noncopyable bases. Physics/active/block-time fields private; matrix/resource/game fields protected; `create_text_manager` protected. |
| `game_scene` | `0x92c6` | `0x92c5` | Public getter methods, protected resource handles at `+4/+8`; generated nonvirtual destructor. |
| `camera_director` | `0x90e7` | `0x90e6` | Private noncopyable base and all stored fields; getters public. No virtual destructor added. |
| `free_fly_camera` | `0x1f2c9` | `0x1f2c8` | Generated copy/assignment and virtual destructor, existing event-vector ownership. |
| `levelmap_camera` | `0x47e8c` | `0x47e8b` | Public game-camera and input-handler bases at `0/+0x54`; public AABB `+0x58`; private borrowed world/director references `+0x70/+0x74`. |

Raw constructor lists distinguish generated copies from explicit constructors:
base scene `0x90f5` has copy `0x90ee/0x103`, three-argument `0x90f2/0x003`,
one-argument `0x90f4/0x003`; director `0x90d5` has copy `0x90d0/0x103`
and explicit `0x90d4/0x003`; level-map `0x47e7d` has copy `0x47e7a/0x103`
and explicit `0x47e7c/0x003`. The generated bit is `0x100`, not the introducing
virtual property in `0x013`. No access was widened or friendship introduced.

The retained base-scene constructor leaves matrix default behavior and
`m_block_btn_time` alone. The alternate-constructor model does not add arbitrary
zero/identity initialization. The retained destructor's final call is printed
under an ICF-folded Boost `storage3` name, but its actual operations release the
handle at `+8` before `+4`. Treat this as inherited scene cleanup, not evidence
for a new Boost member or custom source destructor. Director destruction is
empty; ownership/freeing belongs to the scene, not the active camera.

## Virtual identities and the live priority bug

Table RVAs were recovered from constructor `mov [this+offset], immediate`
instructions and their pointer bytes read directly from each PE:

| Table | Target | Frozen base | Relevant slots |
|---|---:|---:|---|
| Base scene primary game-scene table | `0x95edb4` | `0x951f6c` | `+0x14`: empty show-ui; `+0x18`: false mouse-over; `+0x1c`: purecall |
| Director | `0x960ae8` | `0x953860` | `0`: empty load; `+4`: focus forwarding |
| Free-fly input handler | `0x95f854` | `0x9527ac` | `+0x18`: priority; `+0x1c`: deleting-destructor adjustor |

Retail priority slot targets `0x868f0` (`mov eax,0Ah; ret`); frozen base targets
`0xbc470` (`xor eax,eax; ret`). This independently confirms the mismatch rather
than trusting a two-sided ICF alias selection. `input_world::add_handler` calls
virtual `input_priority`, uses `handler_prio_less` in `lower_bound`, and inserts
at that position. Free-fly `on_focus`, `0x5bef80`, reaches input-world add/remove
slots `+0x10/+0x14`. Thus the constant can change live input dispatch order.

Director `switch_to_camera` remains a frozen structure match: 11 statements,
`0x135` bytes on both sides. Its scene-active read and focus-switch behavior do
not require replacing existing public getter seams with private-field access.

## Level-map evidence limits and static review

The current source search finds the declaration, project header listing and
`camera_director.cpp` include, but no construction/invocation. The rich target
index has no retained level-map procedures. Local history starts this header
with the generated reconstruction; the checked older `temp/game_legacy` trees
and exact-path history provide no original level-map implementation. This
does **not** prove absence of inline expansions or classify it editor/server-only.

Constructor, priority and focus use bounded sibling models. Free-fly calls the
empty base focus hook before registration; lobby does not. Original level-map
delegation is therefore still a question. Bounds/projection/input/timing choices
remain individual open bodies rather than invented keys, constants or fields.
No new call site is added just to make the class emit.

The alternate base-scene constructor now includes the existing allocator and
complete director definition. Level-map includes complete world/game/input
interfaces for the real derived-to-base conversion and registration calls;
there is no reverse include of level-map from those headers. Static review
checked member names, access, signatures and include direction only. Compilation
and post-change PDB/byte comparison are explicitly deferred by instruction.

This batch removes 17 literal no-source markers, but only the free-fly priority
change is a newly established retail behavior correction. Four implemented
models (alternate scene constructor and three level-map bodies), the const
physics-getter overload boundary, and seven level-map bodies remain explicit
open questions indexed in `review_todos.md`.
