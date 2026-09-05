<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Stealth and ladder inline reconstruction

Scope: all six player-stealth inlines, all nine landing-point inlines, and all
eight ladder/nested-occluder in-class bodies: 23 declarations. Four bodies
remain literally open; four stealth getter implementations are explicit
field-based models. No build or score refresh ran.

## Landing-point constructor and the missing predicate seam

Target `ladder_cook::on_animations_loaded` (RVA 0x6f0f90), statement 18,
inlines landing-point construction. After base construction it writes zero
to +0 at function offset 0x344, copies position to +4 and rotation to +0x10,
then explicitly calls the managed-resource pointer constructor with null for
+0x1c and +0x20. Our independently fetched base constructor omitted the +0
store and default-constructed its resource members through a different
no-argument path. Restore `next(NULL)` and both explicit null resource
initializers, retaining position/rotation order. This is a real uninitialized
link discrepancy, not evidence that the compiler just chose a different
inline boundary. No node ownership or destructor policy is added.

Statement 25 is the useful lost-inline seam:

- Target directly calls `c_ptr()` on the **stored** +0x1c resource, normalizes
  it to bool, and only when false repeats that operation for +0x20.
- The frozen base uses the by-value `get_start_animation` and
  `get_end_animation` interfaces. It creates reference-counted temporaries,
  records their cleanup flags, tests them and destroys them after the guard.

Restore `use_for_attachment` as start-animation presence and `use_for_landing`
as end-animation presence. Propagate the same two-predicate conjunction to
the existing cook guard. This removes the need for owning temporary copies
at a site that only asks whether the two stored resources exist. Preserve the
separate by-value getters and their PDB signatures for callers that actually
need resources; do not change their return types to force this caller.

Statements 21 and 24 confirm that the named start/end requests feed +0x1c and
+0x20 through the existing setters. The cook permits either animation alone
and rejects only points with neither. Selection policy is a separate question.

Restore the target's three const locals in the same consumer: `ladder* const
new_ladder`, `pcstr const start_animation` and `pcstr const end_animation`.
Remove its old blanket claim that constructor/copy differences are an
unsteerable global emission decision. Exact expansion and other resource
temporaries still require the deferred build; they are not declared matched.

## Usage and ownership do not prove the selector algorithms

The cook creates the points, adds them to the ladder, and drains/deletes every
point before deleting the ladder in `delete_resource`. The target delete
loop at RVA 0x6f0f20 confirms the pop/delete ownership arrangement. Do not add
a second point-deletion loop to `ladder::~ladder` or invent ownership inside
the selectors.

`ladder::use_initialize` calls the owner's virtual `use_ladder(this)` slot and
returns true. The reconstructed `player::use_ladder` uses `NOT_IMPLEMENTED`;
its target record is a single int3. This is the previously documented shared
trap/extent problem, not a normal recovered traversal consumer. No other
selector source use or retained selector procedure was found. That does not
prove neither inline ever existed or that a server/editor target is required.

Candidate selection models are individually recorded below. The point
position, rotation, linked list and ladder plane constrain possible inputs,
but not the distance metric, direction or tie-breaking rules. No fabricated
nearest-point implementation or new call is introduced solely for reachability.

## Every landing-point inline (9)

| Function | Disposition |
|---|---|
| Constructor | Restore null link and explicit null resource constructors from the target cook expansion; keep const position/rotation copies. |
| `set_start_animation` | Retain owning resource assignment at +0x1c; observed in cook statement 21. |
| `set_end_animation` | Retain owning resource assignment at +0x20; observed in cook statement 24. |
| `use_for_attachment` | Restore direct start-resource presence bool and use it in the cook guard. |
| `use_for_landing` | Restore direct end-resource presence bool and use it in the cook guard. |
| `get_position` | Retain const-reference field getter; no selector consumer establishes additional checks. |
| `get_rotation` | Retain const-reference field getter; do not substitute plane/orientation recomputation. |
| `get_start_animation` | Retain by-value resource getter and its ownership semantics; no longer misuse it for the cook's presence-only question. |
| `get_end_animation` | Retain corresponding by-value resource getter. |

## Every ladder inline (8)

| Function | Disposition / unresolved contract |
|---|---|
| `use_info` | Retain `st_use_ladder`, independently read through the target instruction's string address. |
| `select_landing_point` | Open. Candidate: filter end-animation points by direction indicated by the bool and choose a nearby exit. Bool could instead mean top/bottom or upward/downward traversal; Euclidean versus vertical/projection distance, thresholds, ties and fallback are unproved. |
| `select_attachment_point` | Open. Candidate: select a start-animation point near the supplied position. Nearest-anywhere versus plane-facing/reach-threshold filtering and first-versus-last tie rules need a real traversal consumer. |
| `get_animation` | Retain by-value main-resource getter at +0x138, matching its PDB type. No current traversal consumer establishes assertions. |
| Occluder `use_initialize` | Retain false with unreferenced parameter; explicitly fetched target RVA 0xba8c0. |
| Occluder `use_execute` | Retain same confirmed false return. |
| Occluder `use_finalize` | Retain same confirmed false return. |
| Occluder `use_info` | Retain `st_do_not_use_ladder_from_this_side`; full string read from retail PE, not guessed from truncated mangling. |

The three raw class variants (`landing_point`, `ladder`, nested occluder)
match the frozen candidate, including public/private boundaries. No field or
method visibility is widened and no friend is added.

## Stealth data ownership is known; detection semantics are not

The current player cook loads `initial_stealth` from `stealth_params`, then
player construction copies it. The target retains load, assignment, copy
construction and a genuinely empty default constructor (RVA 0x59aad0).
Retain that default constructor instead of adding invented safe defaults.

Target `load` at RVA 0x59aae0 has 11 statements, each mapping the corresponding
configuration key to the existing field: default visual/sound values at +0/+4;
stand/crouch/walk/sprint factors at +8 through +0x20; detection level at +0x24;
always-visible distance at +0x28. These are confirmed ownership facts, not
proof of a particular detection formula. Raw type variants agree; target one/
base two equal-record multiplicity is not a layout or access mismatch.

## Every player-stealth inline (6)

| Function | Disposition / candidate contract |
|---|---|
| `can_be_detected` | Open. Candidate: combine subject visual/sound defaults with stance/movement factors, compare to observer detection capability, and apply an always-visible range. Which object is observer, which input is described, whether the float is range or squared range, whether sound and visual channels use OR/max, and stance-factor precedence are all unproved. Do not invent a formula from names alone. |
| `get_armor_value` | Model: return configured `m_default_value`. A consumer is still needed to exclude transforms or assertions; no armor field is fabricated. |
| `get_optics_value` | Open. Returning or transforming `m_detection_level` is plausible, but the separate detection-level getter means that identity is not established. No separate optics field exists in this type. |
| `get_armor_sound_value` | Model: return configured `m_default_sound_value`. No stance argument exists on this getter. |
| `get_detection_level` | Model: direct +0x24 field getter. No observed consumer establishes normalization or clamping. |
| `get_always_visible_distance` | Model: direct +0x28 field getter. Keep its stored units; a squared-distance conversion is not established. |

The source and retained-procedure scans found no detection/helper consumer
beyond the configuration/copy path. This is a bounded negative finding, not
a claim that all retail inline expansions have been ruled out. Four models
and two open stealth contracts remain visible in source and the review index.

Patch checks and register counts pass. Target construction, resource setter/
predicate operations, owned deletion and virtual uses were inspected without
compiling. Deferred validation must check the restored null store, explicit
resource initialization, predicate expansion and caller const locals. No new
percentage or completion claim is derived from the stale base artifacts.
