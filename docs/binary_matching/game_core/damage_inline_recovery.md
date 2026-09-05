<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Damage-model inline recovery

Scope: all 42 explicit/implicit inline declarations in `damage_model.h`,
`body_part_parameters.h`, `hit_type_parameters.h`, `damage_protector.h` and
`affects_threshold.h`, plus the 17 predicate declarations in the two damage
implementation files: **59 declarations**. Existing unmarked bodies are
included. The three UI methods in `game/sources/damage_model_stats.h` are a
separate, still-open owner, not counted here. No build or score refresh ran.

“Verified operations” below means inspected retail operations, not a new
candidate byte/structure match. “Model” means a reconstruction with a stated
evidence limit, not recovered original source. A missing procedure or current
source caller does not establish that a helper was never used.

## Two data defects and one const-local repair

### The NPC heading belongs to body state

Retail `damage_model::fill_stats(ai::npc_statistics&,u32)`, RVA `0x6ef7f0`,
pushes its `"damage status: "` heading through `stats + 0x2798` in statement 3.
That is `body_state`, not `selectors_state` at `+0x13cc`. The following retained
predicate dispatches each body part's NPC `dump_state`. Correct the source's
wrong destination; the old TODO had already suspected this. No standalone
frozen-base `damage_model::fill_stats` was found, so this is a source-versus-
retail finding, not a claim of a newly measured candidate assembly change.

### The affect count is one byte, not four

Retail `body_part_parameters::serialize`, RVA `0x5871f0`, statement 3 computes
the number of eight-byte affect entries and calls the one-byte append helper.
Raw PE decoding of the `E8` at RVA `0x587259` resolves to RVA `0x7d750`.
That helper passes `size = 1` to the buffer append. Both `append(bool)` and
`append(u8)` have symbols at that same address; its first printed name does
not identify the overload the caller used. The caller passes the count itself,
without converting it to a boolean.

The existing source cast the count to `u32`, selecting a four-byte writer.
Restore `(u8)m_affects.size()`. Retail `deserialize`, RVA `0x587b90`, records
the local as **u8** and calls RVA `0x7e950`: a byte load followed by a one-byte
pointer increment. `r<bool>` and `r<u8>` again share this address. Restore
`reader.r<u8>()`, rather than treating the first displayed alias as type proof.
Counts above one are meaningful: the container has capacity eight and the
reader decrements the count while reading each affect. The old four-byte writer
would leave three count bytes in front of the first affect payload.

The enclosing damage-model writer below is still a model. Fixing this retained
per-part writer does not establish the parent producer's framing or clock policy.

### Const traversal does not need a container API change

Retail `damage_model::dump_stats`, RVA `0x6ef920`, records the local as
`body_part_parameters const*`. Statement 5 at `+0x4b` is a direct next-pointer
load from offset zero. `next` is public. Restore the const local and traverse
`body_part = body_part->next`; do not invent a const overload in the shared
intrusive list. Remove the old claim that the PDB type cannot compile and the
derived callback-copy wall comment. Caller byte effects remain unmeasured.

## Recovered ownership and explicitly modeled paths

The player statistics types already existed in a game UI header. Move exactly
those `survarium::statistics_item<ItemSize,ItemsCount>` and `damage_info_type`
declarations into the data-only `game_core/damage_info_type.h`. This preserves
their names and members; the PDB size for `damage_info_type` is `0x4f18`.
The header location is our dependency organization, not proven retail provenance.
Game-core must not depend on a game UI class just to access this existing schema.

The player `dump_state` model reuses the private `fill_new_stats_item` template
and appends to `damage_info`. Its retained NPC specialization is RVA `0xba3c0`;
the template stays in its original `.cpp`. Add an explicit instantiation for
the existing player statistics item alongside the NPC instantiation so the
new inline caller has a definition. **This additional instantiation is model
support, not a target-confirmed emitted procedure.** Original player formatting,
heading/reset policy and the UI consumer are still open.

For graph removal, `hit_type_parameters` owns a fixed trailing array at
`this + 0x30` of `(body_part_parameters*,float)` pairs, with a PDB-const count
at `+0x2c`. Retail `apply_damage`, RVA `0x586bc0`, only dereferences a destination
when its coefficient is positive. The removal model disables every matching
edge by zeroing its coefficient, without mutating the const count or deleting
the body part. `body_part_parameters::remove_edges` traverses its hit types
using the existing PDB-named `remove_vertex_from_hit_parameters_predicate`.
Whether retail also nulled the stored pointer remains unknown. No surviving
removal caller establishes that detail; duplicate matches are all disabled.

The body-part index/name models use the existing public next chain and insertion
order. Failed lookup returns `u8(-1)` / null. That index identity, failure policy,
possible assertions and more-than-255-part behavior have no verified consumer.
Do not present the sentinel choices as retail facts.

The damage-model writer traverses its body parts and invokes their existing
writer with the supplied signed client offset. Retail parent `deserialize`
at `0x6efc10` binds the reader and traverses this same list without reading a
part count. That supports the symmetric traversal model, but does not prove
the original writer had no other framing or assertions. The retained child
writer preserves zero timestamps before offset subtraction.

## Per-declaration register

### `damage_model.h` — 18

| # | Declaration | Disposition and evidence |
|---:|---|---|
| 1 | `check_health_predicate::operator()` | Existing inversion of the part's `is_healthy`; supports find-first-unhealthy composition. Leaf equality/affect policy remains a model. |
| 2 | `dump_player_body_part_state_predicate` ctor | Existing capture of stats reference and time matches PDB fields at +0/+4; player consumer not retained in the inspected evidence. |
| 3 | `dump_player_body_part_state_predicate::operator()` | Existing forwarding seam to player `dump_state`; restored parent uses it. NPC sibling dispatch provides the model, not proof of player formatting. |
| 4 | `affect_subscriber(callback const&)` | Existing callback copy and null next: ownership model; no observed consumer for this overload. |
| 5 | `affect_subscriber()` | Verified in damage-model ctor `0x6efeb0`: default callback construction and null next, then bind assignment in the body. Unlike the stamina subscriber, no explicit null callback argument. |
| 6 | `booster_damage_protector` ctor | Retained at `0xab200`: base construction, reduce/absorb, bound callback and string copy. Preserve the absence of a store to derived next at +0x68. |
| 7 | `booster_damage_protector::reduce_damage` | Retained at `0xab320`: compare hit type; on match clamp `amount * reduce - absorb` at zero, otherwise return amount. Unused body-name/armor-piercing args do not justify additional effects. |
| 8 | `damage_model::fill_stats(damage_info_type&,u32)` | New player-statistics model described above; existing predicate owns per-part traversal. Heading/format/reset alternatives open. |
| 9 | `damage_model::is_healthy` | New composition model: `find_if(check_health_predicate()) == NULL`. Empty list consequently healthy; predicate leaf policy and original empty-list assertion remain unverified. |
| 10 | `broken_legs_count` | Existing sum of +0x338/+0x339; retail player jump consumers checked in the weapon-core register. |
| 11 | `broken_hands_count` | Existing sum of +0x33a/+0x33b; limb update/weapon consumers retain these two counters. |
| 12 | `get_parts_count` | New direct list-size model; PDB-owned body list at +0x108. No evidence for filtering or an original assertion. |
| 13 | `get_last_aggressor_id` | New direct u8 getter model; retained `hit_body_part` writes initiator at +0x2e0 and reset restores 255. |
| 14 | `get_affects_applying_type` | Verified load +0x2c8 inside body-part `regenerate` (`0x587860`, statement 10) and hit processing. Remove stale “Needs getter” comment. |
| 15 | `get_body_part_index` | New insertion-order model; name comparison uses public `get_name`. Failure/sentinel/overflow policy open. |
| 16 | `get_body_part_name` | New inverse list-index model; no private field access required. Failure policy open. |
| 17 | `get_body_part_with_min_health` | Open body: likely minimum scan, but raw/normalized health, death-eligible subset, empty result and tie policy are unobserved. `get_total_health` uses death-eligible percentages and is not proof of this helper's rule. |
| 18 | `serialize(packet&,s32)` | New per-part bind/foreach model; retained reader and child writer constrain ordering, not all producer framing. |

### `body_part_parameters.h` — 11

| # | Declaration | Disposition and evidence |
|---:|---|---|
| 19 | `remove_vertex_from_hit_parameters_predicate` ctor | Existing pointer capture; target type names the owned vertex. No proof of an original removal consumer. |
| 20 | `remove_vertex_from_hit_parameters_predicate::operator()` | Existing forwarding to `remove_vertex`; propagated through reconstructed `remove_edges`. |
| 21 | `dump_state(damage_info_type&,u32)` | New private-template-based statistics model; same item shape as NPC sibling, original player formatting still open. |
| 22 | `remove_edges` | New hit-list traversal through existing predicate; follows fixed-array edge-disable model. |
| 23 | `get_name` | Existing +0x70 fixed-string access; expanded in retained name `find_if` at `0xab780`. |
| 24 | `is_healthy` | Existing exact `m_health == m_max_health` kept as model, not proven from name. Threshold/affect/empty-parent policy remains open. |
| 25 | `relative_health` | Existing raw-health return explicitly unverified. A normalized fraction is plausible; do not silently change it without a consumer, especially for zero max health. |
| 26 | `get_max_health` | Existing +0x8c getter; `player_parameters_modifyer::apply` uses it when setting modified body-part parameters. |
| 27 | `damage_group` | Existing const byte getter at +0xa5; retained `hit_body_part` compares groups and the 255 sentinel. |
| 28 | `get_regeneration_speed` | Existing +0x94 getter; parameter modifier and regen-scale predicate consume it in current and retained paths. |
| 29 | `get_health_level_color` | Open body: likely health-ratio-to-color conversion, but endpoints, thresholds, interpolation, alpha and zero-max-health behavior lack a verified draw consumer. No guessed color constants added. |

### Hit parameters, protectors and thresholds — 13

| # | Declaration | Disposition and evidence |
|---:|---|---|
| 30 | `hit_type_parameters::get_bdb_coefficients` | Existing implicit inline returns trailing array at +0x30; verified inside `apply_damage` at `0x586bc0`. |
| 31 | `get_type` | Existing fixed string at +4; retained hit-type `find_if` at `0xba310` resolves the accessor and string comparison. |
| 32 | `get_armor` | Existing +0x24 getter; retained body-part hit processing loads it. |
| 33 | `get_reduce` | Existing +0x28 getter; retained hit damage formula consumes it. |
| 34 | `get_absorption` | Existing +0x20 getter; retained hit damage formula consumes it. |
| 35 | `get_bdb_coeffs_count` | Existing const count at +0x2c; retained apply-damage iteration and parameter modifier use this bound. |
| 36 | `remove_vertex` | New fixed-array coefficient-disable model; pointer clearing is an explicitly unresolved alternative. |
| 37 | `damage_protector` ctor | Retained at `0xab170`: vptr, two default callback members at +8/+0x28 and null next at +0x48. Existing body correct. |
| 38 | `damage_protector` virtual dtor | Retained at `0xab1c0`: automatic callback cleanup and base destruction; empty user body does not mean no generated work. Keep class polymorphic. |
| 39 | `affects_threshold::value` | Existing float at +4; `check_affects` statement 2 multiplies it by max health before comparison. |
| 40 | `get_affects_count` | Existing const count at +8; `apply_affects` statement 2 computes end = begin + count*4. |
| 41 | `get_affects` | Existing trailing enum array at +0x10; `apply_affects` statement 1 forms this exact address. |
| 42 | `bodypart` | Existing pointer at +0xc; `check_affects` statement 3 calls that owner's `apply_affects`. |

### Implementation-file predicates — 17

| # | Declaration | Disposition and evidence |
|---:|---|---|
| 43 | `regenerate_body_parts_predicate` ctor | `damage_model::tick` statement 4 captures delta/current time at +0/+4. |
| 44 | Its `operator()` | Expanded in foreach `0xabac0`: loads both captures and calls the part's regenerate. |
| 45 | `find_body_part_by_name_predicate` ctor | `get_body_part` statement 1 captures the name after empty-base construction. |
| 46 | Its `operator()` | Expanded in `find_if` `0xab780`, using the part's fixed string at +0x70. |
| 47 | `find_by_damage_type_predicate` ctor | `hit_body_part` statement 9 copies damage type into its 16-byte owned buffer, then passes predicate to find_if. |
| 48 | Its `operator()` | `find_if` `0xab830` compares protector +0x50 with predicate buffer. Field is public and no PDB getter is declared; remove unsupported “is getter” TODO instead of inventing a symbol. |
| 49 | `dump_npc_body_part_state_predicate` ctor | `fill_stats` statement 4 captures stats reference/current time at +0/+4. |
| 50 | Its `operator()` | Expanded in foreach `0xabb50`; passes both captures to NPC `dump_state`. |
| 51 | `affect_event_predicate` ctor | `notify_on_affect_event` statement 2 captures name/type/event at +0/+4/+8. |
| 52 | Its `operator()` | Expanded in foreach `0xab8d0`: invokes subscriber callback with those three values, without an invented null guard. |
| 53 | `reset_predicate::operator()` | Foreach `0xab980` directly invokes part reset. |
| 54 | `find_hit_parameters_by_type_predicate` ctor | `get_hit_parameters` statement 1 captures the type string. |
| 55 | Its `operator()` | `find_if` `0xba310` compares the captured type with hit parameters' +4 fixed string. Remove TODO asking to inspect this very expansion. |
| 56 | `protect_damage_predicate` ctor | `hit_by_type` statement 10 captures body/type/armor/amount at +0/+4/+8/+0xc. |
| 57 | Its `operator()` | Retained `0xb9ef0`: positive amount and present callback guard; passes four captures, stores returned amount. |
| 58 | `protect_affect_predicate` ctor | Retained `has_affect_protector` constructs body-name/type and initializes result false. |
| 59 | Its `operator()` | Retained `0xb9f80`: only invoke present callback while result is false, storing returned bool. |

## Visibility, support code and next verification

Raw two-sided class topology for damage model, body part, hit parameters,
booster protector and threshold shows identical semantic variants; extra equal
retail records for body part/threshold are multiplicity, not different layouts.
Keep private data private. The retained parameter modifier traverses the private
damage-model body list; its existing friendship stays with an open original-
friendship-versus-helper question. Replacing that list traversal with guessed
index helpers would obscure, not recover, the observed caller.

The booster string-copy question is resolved: `strings::copy(dst,16,src)` calls
`strcpy_s`, where 16 is destination capacity, not a 16-byte source read. A shorter
terminated string is valid. The derived protector's link at +0x68 is not initialized
by its retained constructor; do not propagate null initialization mechanically
from the base link at +0x48 or from earlier subscriber fixes.

Source-only checks cover existing helper signatures, complete data types,
template definition availability, and unchanged PDB-owned fields/access. New
models have not been compiled or byte-validated. Deferred checks must include
the two data fixes, const-local partition, player template instantiation and
caller expansion. Remaining health/color/minimum/producer questions stay live
in `review_todos.md`, not relabeled as server-only or editor-only.
