<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Animation-analysis methods: producer and individual gaps

PR 571 source-only audit. This family is **managed but not reconstructed**:
the eight algorithm bodies remain open. No fabricated sample rate, indexing
scheme, timing values or callable fake algorithm is introduced to make the
inventory appear finished. No build or score refresh.

## What the retail producer actually does

`game::register_cooks` contains the analysis-result cook registration in source.
The retained cook `translate_query`, RVA `0x7534b0`, has exactly eight statements
and 0x172 bytes, matching the frozen candidate's statement structure/extent.
It checks/extracts user data, logs and returns on failure, constructs
`animation_analyzer`, allocates `animation_analysis_result`, publishes it and
finishes the query. There is no animation evaluation, sample loop or timing
write hidden between those operations.

The target call graph reports one retained caller of analyzer construction:
this cook at `+0xd7`. Its outgoing calls confirm analyzer/result construction,
query publication/completion, logging and cleanup. No analysis helper is
called, and the full statement assembly contains no corresponding inline
operations. The user-data structure includes `animation` at +0xc, but this
path only constructs/destroys that resource reference; it does not evaluate it.
The analyzer constructor stores the legs/count/skeleton and zero ground height.

Result construction at RVA `0x797070` allocates `28 * legs_count` bytes and
sets the buffer-vector start/end; it does **not** populate the seven float
timings. Destruction at `0x796fc0` clears the vector then frees the buffer.
Thus filling result timings in this cook would add behavior absent from the
retained target. Do not change failure handling or zero all timings to make
this incomplete-looking retail path more useful.

## Individual register: 14 declarations/producer anchors

| # | Function | Disposition |
|---:|---|---|
| 1 | `animation_analyzer` constructor | Existing owner-pointer/count/reference stores retained. RVA `0x6ecbd0`; direct operand read at +0x2c resolves RVA `0x964044` to float 0.0 for ground height. Remove only stale disassembly narration. |
| 2 | `animation_analyzer` destructor | Existing empty body retained. RVA `0x6ecbc0` has only empty noncopyable-base cleanup. Inputs are borrowed; no legs/skeleton deletion. |
| 3 | `analyze` | OPEN pipeline: likely create sample storage, prepare samples and analyse into per-leg key times. Original allocation/sample-count policy, resource evaluation and any guards are unobserved; retained cook does not call it. |
| 4 | `prepare_samples` | OPEN sampler: likely evaluate a managed clip over the skeleton, derive heel/toetip from bone transforms and leg offsets, then populate sample records. Sampling rate, start/end inclusion, frame-major versus leg-major layout, ground-height reduction and foot-base geometry need evidence. |
| 5 | `analyse_samples` | OPEN per-leg event extraction: likely use range/stance/center and directional searches to fill seven event times. The integer argument could describe sample count/stride rather than a leg number; no consumer fixes its role or event thresholds. |
| 6 | `get_max_range` | OPEN extrema reduction: plausible motion-axis range or maximum spatial displacement of heel/toe/middle/foot-base. Which field/axis, integer bounds versus count/stride and empty-input behavior are unknown. No arbitrary distance formula is bodied. |
| 7 | `get_stance_index` | OPEN index selection: plausible low-foot/stationary candidate selected relative to a height/range tolerance. Float argument meaning, arg1/arg2 roles, tie handling and cyclic search boundaries are unknown. |
| 8 | `get_cycle_center` | OPEN center reduction: possible mean position, endpoint midpoint, extrema center or stance-based center. Return float3 alone cannot choose among them or determine coordinate space and sample range. |
| 9 | `find_contact_time` | OPEN directional threshold search: signed argument suggests forward/backward scan; bool may choose heel versus toe or contact versus release. Starting-index/count roles, both float parameters, wrapping and fractional-time interpolation remain unobserved. |
| 10 | `find_swing_change_time` | OPEN directional crossing search: could seek velocity/sign change relative to supplied center/direction. Signed scan, integer bounds, float time/tolerance role, reference-vector meaning and wrap/interpolation rules remain unresolved. |
| 11 | `animation_analysis_result::key_times` | Existing +0x10c field-reference model retained; constructor/destructor establish the buffer owner, but no current source or retained getter consumer was found. Source TODO records that limitation. |
| 12 | `animation_analysis_result` constructor | Existing 28-byte-per-leg allocation/buffer construction retained. No target timing initialization observed. Allocation debug-description argument is optimized away here and is not recovered by these bytes. |
| 13 | `animation_analysis_result` destructor | Existing vector-clear/free order retained. The later implicit vector destruction sees an empty range; no double resource owner is added. |
| 14 | `animation_analysis_result_cook::translate_query` | Existing producer retained after full target statement/call review. The absence of sampling is actual evidence, not inferred merely from absent standalone symbols. No new analyze call or output initialization is added. |

These algorithm sketches are **hypotheses**, not source recovery. Unlike the
wire inverses elsewhere in this campaign, there is no retained output-producing
consumer from which to choose a unique compatible algorithm here. The scalar
return placeholders are not validated for execution. Every method has its own
source TODO and review-table row so it cannot disappear behind one generic
“unused/editor” classification.

## Data and access constraints for eventual reconstruction

`leg_info` records hip/ankle/toe bone indices and two float3 offsets:
ankle-to-heel and toe-to-toetip. Existing IK transform helpers demonstrate how
bone positions can be obtained, but are not an implementation of this sampler.
`leg_cycle_sample` has four float3 values (heel, toetip, middle, foot_base),
size 0x30. The record does not specify the array's frame/leg layout or how
middle/foot_base are derived. These distinctions affect every integer argument
in the private algorithms; guessing one indexing convention would compound
the uncertainty across the whole pipeline.

`leg_key_times` has seven floats, in order: pre_land, strike, land, stance,
lift, liftoff, post_lift. Their names suggest ordered contact/swing events,
but do not establish normalized phase versus seconds, loop wrap handling or
sentinel values. No 30/60 Hz assumption, zero fallback or monotonic clamp is
justified by the retained record.

Both queried analyzer/result class variants are semantically identical between
the frozen PDBs. Analyzer entry/constructor/destructor are public; algorithm
helpers/data and the nested sample type are private. Result's mutable-reference
getter is public and its buffer members private. Keep the missing sampling
and reduction seams inside the analyzer rather than inventing a cook friendship
or accessing private sample fields externally.

The generated analyzer header prints public noncopyable inheritance while the
current source is private and topology says identical: the existing raw
base-access projection question applies here too. Preserve it as an explicit
question instead of claiming whole-class access verification or guessing a
change. No class layout, const member or method order is changed in this pass.

## Next evidence and verification

Find an original source/binary/configuration that actually invokes the analysis
pipeline. An editor/debug build is a candidate, not proven ownership. Recover
the sampler's loop/indexing first, then the range/stance/center reductions and
the two directional searches, and only then wire orchestration/output events.
Do not label the registered client cook editor-only or treat its unfilled
output as a reason to redesign the retail-matching path.

Local source searches, rich method searches (only analyzer constructor and
destructor retained), target callers/callees, raw PE constants, full producer
assembly, frozen structure/class comparisons and `git diff --check` were used.
No build, new compiler measurement or original algorithm recovery is claimed.
The missing active-producer evidence is a specific open family, not a blocker
for continuing the many other unreviewed engine functions.
