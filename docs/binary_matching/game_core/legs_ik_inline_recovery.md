<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Leg-IK inline helpers and consumers

PR 571, source-only. No compiler/linker/tool build or ledger refresh.
Target/base references below are frozen artifacts, not rebuilt results.

## Individual register: 21 declarations/consumer anchors

| # | Function | Disposition and evidence |
|---:|---|---|
| 1 | `legs_ik_processor::set_drawer` | Replace empty body with pointer-install model. Constructor initializes +8 to null; destructor deletes that member. Original creation/setter caller, replacement cleanup and assertion policy remain unidentified; no automatic delete-on-replacement is invented. |
| 2 | `set_character_controller` | Existing non-owning setter retained. `weapon_core::activate` RVA `0x595f50`, `+0x7b9..+0x7ea`, calls the user's physics-controller virtual and stores its result at weapon `+0x1e4` = processor `+0xc`. |
| 3 | `is_right_supporting_leg` | Restore right-versus-left comparison forwarder model. No current consumer found. Original tie policy and the underlying comparison contract remain open. |
| 4 | `leg_params::set_rotation_axis` | Restore the three-component assignment and propagate into `process_leg`. Keep the PDB's const method; current model casts away receiver constness. The observed caller passes mutable leg parameters. Original const_cast versus mutable storage and exact inline boundary remain open. |
| 5 | `leg_params::is_heel_on_ground` | Existing byte getter at +0x2c retained; observed inside processor heel setter, ground predicates and foot processing. |
| 6 | `leg_params::is_toe_on_ground` | Existing byte getter at +0x2d retained; analogous toe/setter and foot consumers. |
| 7 | `leg_params::is_on_ground` | Existing short-circuit OR retained. The end of `process`, RVA `0x6ec640`, statement 23, explicitly expands left heel/toe and conditionally right heel/toe. |
| 8 | `leg_params::is_full_on_ground` | Existing short-circuit AND retained. `leg_params::set_heel_on_ground` RVA `0x6ea890` checks both bytes and resets +0x28 only if both are true. Toe setter and foot placement use the same predicate. |
| 9 | `leg_params::is_more_supporting_then` | Replace undefined bool return with a documented recent-full-stance model: this stance age < other stance age. Existing setters reset the private age on full contact and tick adds elapsed time; no comparison consumer was found. Direction, contact priority and ties remain unverified. |
| 10 | `transition_time_calculator` constructor | **Real data fix:** initial value 0.1f → 0.0f. Both embedded calculator initializers in retail processor construction load pooled zero; frozen base loads 0.1. |
| 11 | `transition_time_calculator::reset` | Existing zero store retained. Processor heel setter RVA `0x6eab70`, statement 6, stores zero at +0x88; its later reset stores at +0x8c. |
| 12 | `transition_time_calculator::tick` | Existing elapsed-time addition retained. Processor tick RVA `0x6ea9d0`, `+0x55` and `+0x73`, adds the computed seconds to both calculator fields. |
| 13 | `transition_time_calculator::get_value` | Existing value load retained. Heel setter statement 8 loads +0x88 into the heel duration; toe path analogously uses +0x8c. No extra clamp belongs in this getter. |
| 14 | `legs_ik_drawer` constructor | Existing renderer-reference/scene-copy model retained, including by-value scene parameter. Recorded fields/signature agree, but original construction consumer is unidentified. No claim of verified original ctor expansion. |
| 15 | `legs_ik_drawer::draw_line` | Restore scene-bound renderer forwarder and use it at all four draw_leg line sites. Actual renderer arguments/stores are retained at RVA `0x7a1d70`; the original choice of direct member call versus inline wrapper remains open. |
| 16 | `legs_ik_drawer` implicit destructor | Keep generated. Typed delete helper RVA `0xbb290` destroys the scene resource at +4, then frees allocation and clears owning pointer. Renderer is borrowed, not deleted. The printed inventory-item destructor label is an ICF alias, not a different stored type. |
| 17 | `ik_processor` implicit destructor | Keep generated/non-owning. Recorded base has borrowed skeleton pointer and scalar time; derived cleanup calls a folded empty base operation. No invented virtual destructor, skeleton free or extra store. |
| 18 | `legs_ik_processor` constructor | Retained initializer list otherwise unchanged. Both pointers null, two leg constructors, two fermi interpolators at 0.1f, two elapsed timers now 0.0f, durations obtained from interpolators. Do not zero the hip pointer absent an observed store. |
| 19 | `leg_params` constructor | Existing values retained: transition times and stance age 0, axis (1,0,0), contact flags false. Bone indices are not initialized here; activation owns them. |
| 20 | `legs_ik_processor::process_leg` rotation assignment anchor | Target RVA `0x6eafa0`, statements 24/25, tests direction similarity then copies normalized cross product into params+0x1c. Restore the declared setter seam at this one assignment, without moving normalization into it or changing the guard. This is not a new full-function matching claim. |
| 21 | `legs_ik_drawer::draw_leg` line anchors | Four existing line operations now go through draw_line. Retain endpoint/color order, depth=false, preceding four draw_origin operations, and renderer scene binding. This is a source-boundary reconstruction, not newly verified codegen. |

No source consumer of the two support-selection helpers was found. Do not
insert them into the end of `process` merely because its discarded ground
checks look unfinished: retail really emits those checks and no stance-age
comparison there. The helper model is kept separate from that verified oddity.

## Raw float proof

The processor constructor has **zero body statements** in both PDBs and the
same 0xe6-byte extent. `structure-diff` says STRUCTURE MATCH. It still contains
two wrong data operands in the frozen candidate:

| Site | Target operand/data | Frozen base operand/data |
|---|---|---|
| Heel elapsed timer | instruction RVA `0x6ea7b4` loads RVA `0x964044`: `00000000` = 0.0f | instruction RVA `0x6f4924` loads RVA `0x93a424`: `cdcccc3d` = 0.1f |
| Toe elapsed timer | instruction RVA `0x6ea7cf` loads the same zero | instruction RVA `0x6f493f` loads the same 0.1f |

Direct PE reads also verify the separate fermi-interpolator argument is 0.1f
in retail (RVA `0x8138b8`), not zero. Do not change it together with the timers.
The leg transition/stance initializers and stance reset all load retail zero.
This single unmarked inline constructor fixes both initially overcounted elapsed
timers; their reset/add/get operations were already correct.

## Const, access and model boundaries

`leg_params::set_rotation_axis` is genuinely const in the generated target
record, and its axis is a public float3 at +0x1c. The existing direct store in
`process_leg` proves what is written, not how the original const setter enabled
mutation. The explicit cast model is valid for that mutable caller; it must not
be advertised as permission to modify a genuinely const leg object. Mutable
storage is an alternative source spelling still requiring evidence.

The recent-stance model uses the private age field within its own class; the
outer selector calls the declared comparison instead of reaching into that
private field. Smaller versus larger age, heel/toe/full-contact priority and
tie handling cannot be settled from the age producer alone. The two restored
bool bodies are models, not byte-exact or proven gameplay behavior.

Frozen two-PDB topology reports identical semantic records for the processor,
both nested types, drawer and base. The base's two fields really are protected,
so derived accesses do not require invented getter methods. Remove stale
comments justifying protected access solely by its byte effect.

The raw generated base/drawer headers print public `core::noncopyable` bases,
while source uses private and topology reports identical. This is the same
unresolved base-access projection discrepancy noted in the bullet pool audit.
Do not quietly call it 100% verified visibility or change inheritance on this
inconsistent evidence. Both source anchors and the review table retain the
question; raw base-access encoding/parser handling still needs resolution.

`set_drawer` and drawer construction have no identified current creation site.
The processor owns deletion of its current drawer, while the drawer retains
its scene reference. Pointer replacement, ownership-transfer assertions and
allocation source are open; a trivial pointer-install model is not a proven
leak-safe replacement API.

## Validation boundary

Raw target/base operand reads, one-sided consumer assembly, class topology,
source consumer searches and `git diff --check` were run. No builds or score
claims. The five literal placeholders now have bodies, but the support-choice,
drawer ownership/creation, const-storage and inline-boundary questions remain
explicitly open. Whole `process`/`process_leg` matching and the physics foot
adjustment body are not declared closed by this inline batch.
