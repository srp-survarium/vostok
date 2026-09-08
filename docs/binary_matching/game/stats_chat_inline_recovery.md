<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Stats, damage-panel and chat inline recovery

Source-only continuation above `f352639fb` in PR 571. **63 additional entries**
cover all 15 in-class bodies in these four classes, generated operations, the
graph sample record and retained owner methods. External consumers and NPC
donors are evidence below, not additional counted entries. No build or new
byte/structure score claim.

Seven placeholder bodies gain explicitly bounded models: four stats setters
and the damage panel's draw/populate/group helpers. Two formatting-dependent
stats setters remain open. Existing chat and graph getters are verified through
consumers; seven chat-mode reads now use the declared const getter. No new
live call is invented to force an otherwise unobserved diagnostic helper into
the executable.

## Individual register

Generated-method ranges identify separate declarations, not recovered bodies.

| # | Function | Evidence and disposition |
|---:|---|---|
| 1 | `stats::set_active_scene` | Restore older `string64` / `active scene: %s` / set-text body as a legacy model. Exact retail prefix, buffer contract and consumer remain unverified; no current caller is invented. |
| 2 | `set_player_stamina` | OPEN numeric-label contract. Allocated text field `+0x28` and float parameter support a formatted stamina display, but not units, precision, prefix or original caller. Empty body is not verified. |
| 3 | `set_can_use_value` | OPEN boolean-label contract. Allocated text field `+0x2c` and bool parameter support a textual status, but not literal spelling, hide-versus-text policy or original caller. |
| 4 | `set_dispersion_components` | Direct-text forwarding model into field `+0x30`, supported by the existing resources-text setter and allocated text object. Original formatting/visibility and caller remain open; draw's separate hide operation stays unchanged. |
| 5 | `set_player_logic_state` | Direct-text forwarding model into field `+0x34`; do not invent a prefix or notification. Original format and caller remain open. |
| 6 | `set_weapon_logic_state` | Corresponding model into field `+0x38`; preserve separate player/weapon widgets, with original format and caller still open. |
| 7–8 | Implicit copy constructor; assignment | Raw `0x3fcc8/0x3fcd1`, generated attributes `0x103`. Private noncopyable base and const colors remain; generated declaration does not imply assignment is usable. |
| 9 | Explicit constructor | `0x5caf20`: borrowed UI world, crosshair zero, two row colors, create call. Existing ownership/initializers retained. |
| 10 | Explicit nonvirtual destructor | `0x5ca320`: destroy root window through borrowed UI world; children are root-owned. No individual child double-deletion added. |
| 11 | Private `create` | `0x5ca330` owns the root/text hierarchy, including all six missing setters' destinations. Existing field/order setup retained; absence of setters' calls does not mean these fields are absent. |
| 12 | `draw` | `0x5ca2f0`: draw root, then hide dispersion widget through text's window. Preserve order and the hide operation rather than silently enabling it in a setter model. |
| 13 | `set_fps_stats` | `0x5caee0`: retained numeric formatting/text update. Consumer is game stats update; donor for general numeric-label shape only. |
| 14 | `set_player_linear_speed` | `0x5cae60`: existing speed format, buffer and destination retained. Player update uses graph average; not evidence for stamina units. |
| 15 | `set_player_angular_speed` | `0x5caea0`: separate angular label retained, with caller converting radians to degrees. Do not copy that conversion to stamina. |
| 16 | `set_camera_stats` | `0x5cad80`: existing position/direction/crosshair labels retained. Stored crosshair distance and separate buffers are not replaced with a new aggregate string model. |
| 17 | `set_resources_stats` | `0x5ca2d0`: field `+0x18`, virtual `set_text` at `+8`, tail call. Concrete direct-string donor; does not prove every missing string setter omitted a prefix. |
| 18 | `set_crosshair_info` | `0x5ca2e0`: existing float store to `+0x3c`, consumed during camera-label formatting. No immediate text update added. |
| 19 | `stats_graph::cumulative_count() const` | Existing u32 at `+0x20` verified in row and FPS consumers. Remove stale marker; do not return a fixed pool capacity or count only non-invalid samples. |
| 20 | `cumulative_value() const` | Existing float sum at `+0x18` verified in both network-stat consumers. Remove stale marker; not the neighboring weighted-average field or an already divided average. |
| 21–22 | Implicit copy constructor; assignment | Raw `0x19870/0x19882`, generated `0x103`; retain private noncopyable base and const parameters. No invented deep copy of cyclic nodes. |
| 23 | Explicit constructor | `0x5c9d80`: null active/pool pointers, input fields, sum/count zero, color. Retail does not initialize `m_weighted_average_value` at `+0x1c`; do not add a safety store. |
| 24 | Explicit nonvirtual destructor | `0x5c9e30`: free active ring by stored count, then pooled chain. Sample records own no separate data allocation. Existing cleanup retained. |
| 25 | `add_value` | `0x5c9e90`: reuse oldest/pool nodes, subtract/add sum, update count and ring links. These writers establish getters' meaning; preserve recycling and time-window conditions. |
| 26 | Private `adjust_time_interval` | `0x5c9d00`: evict old samples into pool while adjusting sum/count/links. Existing UNKNOWN_EXPRESSION assertion models remain; no null/empty guard invented here. |
| 27 | `set_time_interval` | `0x5c9dd0`: store interval and call adjustment. Preserve the existing required nonempty graph precondition implied by its callee, not a new ignored update. |
| 28 | `cumulative_time() const` | `0x5c9cf0`: newest time minus next/oldest time. Existing assertion models remain. No replacement with count or configured interval. |
| 29 | `average_value() const` | `0x5c9de0`: zero-time tolerance, then `(sum - oldest.value) / elapsed`. Keep this distinct from the raw cumulative sum getter and from count-based averaging. |
| 30 | `render` | Retained 40-statement owner consumes average/time, samples and invalid sentinel. Existing source left unchanged; this getter audit is not complete matching of the plotter. |
| 31 | `stop_rendering` | Target `0x5c9ce0` is a bare return. Existing empty body retained, not a guessed clear/pause flag. |
| 32 | `stats_value` trivial operations | Raw `0x19893/0x19892` has only four public fields at `0/4/8/0xc`, size `0x10`. Preserve aggregate/trivial behavior and explicit producer initialization, not a fabricated nulling constructor or per-sample ownership. One record anchor, not four claimed emitted methods. |
| 33 | `damage_model_stats::draw` | Root-window forwarding model from retained `npc_stats::draw`; recover this panel's own caller and guard policy. No new draw invocation inserted into the game frame. |
| 34 | `set_stats(const damage_model_ptr,u32)` | NPC-style clear/null-check, player-stat collection and caption/content iteration model. Preserve by-value const resource pointer and supplied time. Original reset/null/format policy and consumer remain open. |
| 35 | Private `create_new_group` | NPC first-column text creation/adoption/layout model, adapting away its explicit column enum. Original u32 role, width/placement and invocation remain open; no assertion that first-column layout is proven retail. |
| 36–37 | Implicit copy constructor; assignment | Raw `0x12f8e/0x12f97`, generated `0x103`; keep private noncopyable and const layout values, not copyable UI ownership. |
| 38 | Explicit constructor | `0x6f2500`: UI world, colors `0xff80ffff/0xffffff80`, height 20, widths 180/360, owned root visible at zero with 1280×720 size. Actual game load constructs this panel; not an editor-only class. |
| 39 | Explicit nonvirtual destructor | `0x6f24f0`: UI world virtual `destroy_window` at `+0x24` on root `+4`. Existing deletion path retained. |
| 40 | `chat_handler::input_priority` | Constructor-installed target table `0x95bbc4`, slot `+0x18`→`0x4c80`, returns 5. Frozen base table `0x94ed4c` also returns 5. Existing body verified; no priority change. |
| 41 | `is_active` | Game-world hide-UI consumer `0x5d0710 +0x78` tests chat `+0x15`. Remove stale marker, preserve distinction from focused and match mode. |
| 42 | `in_match() const` | Byte `+0x16` verified at seven owner reads and messaging's existing channel parser. Propagate at the seven owner reads; original named boundaries remain indexed. No change to assignment or field timing. |
| 43 | `get_movie` | Existing mutable handle reference at `+0x1c`; add-message `+0xf` directly follows the stored resource without an extra owning copy. Remove stale marker; initialization remains the writer. |
| 44–45 | Implicit copy constructor; assignment | Raw `0x1514b/0x15167`, generated `0x103`. Preserve private noncopyable base and generated semantics, not a custom copied chat registration. |
| 46 | Explicit constructor | `0x5cc4f0`: flash function/external bases, three false bytes at `+0x14..0x16`, game reference and empty movie handle. Keep all three flags separate. |
| 47 | Explicit virtual destructor | `0x5cc480`: release movie handle then both flash-handler bases/implementations. Empty source body relies on this generated cleanup; no extra UI destruction added. |
| 48 | `initialize` | `0x5cc570`: assign actual movie resource, configure it, register this through both flash interfaces and set send-function proxy. Retain resource and callback ownership. |
| 49 | `on_gamepad_action` | `0x5cba60`: return mode byte, not focused/active or unconditional true. Use `in_match()`. Frozen structure/extent already agrees; no new byte claim. |
| 50 | `on_mouse_key_action` | `0x5cba50`: same mode-byte result, own stack cleanup. Use getter without inventing mouse processing. |
| 51 | `on_mouse_move` | `0x5cba40`: same mode-byte result. Preserve argument/event-consumption behavior. |
| 52 | `on_keyboard_action` | Existing bind lookup, send/escape/tab-channel behavior and movie input translation retained. No delegation of this distinct behavior to the three simple mode-return handlers. |
| 53 | `callback` | Existing start/cancel-focus and mouse-cursor dispatch retained. Focus flag is deliberately not replaced with match-mode getter. |
| 54 | `call` | Existing Flash send-function path reads text and checks network bandwidth before messaging. No invented call to the separate channel-parameter overload. |
| 55 | `focus` | `0x5cbab0 +0x2a` tests match mode or requested focus before GUI update, then stores focused flag and adds/removes input handler. Restore only mode predicate seam. |
| 56 | `show` | `0x5cca50`: scene movie show then active byte true. No automatic focus/mode change. |
| 57 | `hide` | `0x5cca00`: hide movie when available, then active byte false. Preserve distinction from input-handler focus registration. |
| 58 | `tick` | `0x5cba70`: retained movie advancement. Remove obsolete blanket comment alleging this and its callees are empty stubs. |
| 59 | `set_mode` | `0x5cbee0`: retain channel tables/selection, mode writer at `+0x15b` and final mode read at `+0x508`. Only final inverse read becomes `!in_match()`; do not substitute stale argument value across callbacks. |
| 60 | `on_message_typed` | Existing bandwidth-guarded messaging forwarder preserves supplied channel. Distinct from Flash send-function's fixed channel. |
| 61 | `add_message` | `0x5cc720 +0x113` gates match-channel team presentation on mode byte. Restore getter seam; retain sender conversion, team handling and message data unchanged. |
| 62 | `add_to_recent_list` | `0x5cbc20 +8` returns early in match mode. Restore getter; no inversion of lobby-only recent-list update. |
| 63 | `set_local_player_name` | Existing narrow-to-wide conversion and UI invocation retained; no separate local-name storage added to this class. |

## Raw layout, access and reference distinctions

Complete target class/field-list pairs: stats `0x3fcd4/0x3fcd3` (size `0x48`),
graph `0x19885/0x19884` (`0x28`), damage panel `0x12f9a/0x12f99` (`0x1c`),
chat `0x1516a/0x15169` plus `0x1ae85/0x1ae84` (`0x20`). Keep chat's multiple
records distinct; no blanket record-multiplicity equality claim.

Stats fields are protected and `create` private; graph/panel fields and helper
methods are private. All derive privately from noncopyable. Chat publicly
inherits input at zero, flash-function handler at `+4`, flash-external handler
at `+0xc`, with private noncopyable/flags at `+0x14`. Its data is private.
Public getters and private helper access are preserved; no added friendship.

`in_match` is const; `is_active` and the movie getter are not. The movie result
is a mutable resource-pointer **reference**, not a fresh ownership increment.
Damage panel's `set_stats` instead takes a **const by-value** resource pointer;
the model keeps that parameter ownership instead of changing it to a reference.

Network row `0x773e70 +0x10e..0x119` loads a graph and its u32 count `+0x20`,
then converts the same count for division. Sum reads are at row `+0xdb/+0x15a/
+0x16e` and stream `0x773800 +0x10c/+0x168`, all field `+0x18`. Game's FPS
consumer uses count/elapsed time, not sum/count. These distinguish what the
two getters actually mean. Preserve graph's existing boundary assertions and
uninitialized weighted-average field; this pass adds neither safety guards
nor a different statistical estimator.

## Models, donors and evidence gaps

The older `7798342a7^:sources/vostok/game/sources/stats.cpp:36` has the exact
active-scene formatter with a 64-byte buffer. It is not in the later
`temp/game_legacy` snapshot, so that snapshot alone would miss this donor.
Restore it as a **legacy model**; an ASCII retail search found no copy of
`active scene: %s`, and no current caller/retained expansion establishes its
retail spelling. This is not evidence that the PDB-declared method was absent.
Other old camera labels differ from current retail, so the entire old stats
file must not be imported wholesale.

The three direct string setters use the already allocated destination widgets
and retained `set_resources_stats` as a donor. Whether their original strings
were prefixed, formatted or accompanied by visibility changes remains open.
Do not manufacture a stamina unit/precision or true/false label: their two
individual TODOs record a formatted-display hypothesis and the missing caller/
literal evidence. None is labeled editor/server-only.

The damage panel is constructed by current game-world load and has a retained
retail constructor/destructor. Its missing methods have no current call sites.
Retained `npc_stats::draw` forwards to its root; `npc_stats::set_stats` clears
children, checks owner, collects structured stats, then renders caption/content
groups. `npc_stats::create_new_group` at `0x5bbb60` supplies real text creation,
font/mode/color, parent adoption and below-previous-item placement. Adapt the
first-column form as an explicit **layout model**, not a proven width choice.
The panel's one u32 argument is modeled as font color; its original role and
possible column/wrapping policy remain indexed.

New population code uses the existing player `damage_model::fill_stats` and
`damage_info_type` record, preserving supplied time, by-value owner, caption
color and content color. That producer was itself reconstructed as a model in
[the damage register](../game_core/damage_inline_recovery.md). **This new caller
is not independent retail evidence for that model.** Its heading/format TODOs
remain open, and the new reset/null/group loop must be checked against an
original consumer before either side can be called recovered source.

Window adoption uses the existing UI implementation: `add_child(...,true)`
marks ownership, `remove_all_children` removes/deletes adopted children, and
panel destruction deletes the root. Do not add separate child deletion or
draw calls to make this model live. All seven new bodies remain reviewable
source priors/models rather than measured byte wins.

## Chat getter propagation and checkpoint

The seven reads are the three simple event returns (`+0` each), focus
`+0x2a`, recent-list guard `+8`, add-message guard `+0x113`, and set-mode's
final inverse test `+0x508`. All read mode `+0x16`; source now names the same
declared getter. Constructor/set-mode writes stay direct. Existing external
messaging parsing already calls the getter and is unchanged. Active byte
`+0x15` and focused byte `+0x14` are not interchangeable with mode.

Twelve literal markers removed; two formatting bodies remain open in this
family. Ten new TODO anchors are indexed. Five source files changed; no
layout/order/access widening, script or SDK edits, compilation, tool rebuild,
README/ledger refresh or claim of class-wide byte/statement closure. The index
now accounts for 1353 reviewed entries; remaining literal markers number 200,
including 57 inactive SDK mirrors (143 active marked lines), not 143 proven
missing functions.
