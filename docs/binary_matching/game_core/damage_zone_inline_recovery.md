<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Damage-zone and hit-receiver inline audit

Scope: 12 declarations in `hit_receiver.h`, eight in `damage_zone_core.h`,
and three inline predicate declarations in `damage_zone_core.cpp`: **23 total**.
This includes two predicate call operators that had declarations but no
definitions, not merely literal no-source markers. The collision-sensor and
generic-anomaly classes are inspected consumers/dependencies, not claimed fully
reviewed by this register. No build or score refresh ran.

## Subscription defaults are used through a real override boundary

Both base `hit_receiver` subscription functions are retained `ret 4` bodies
at RVA `0x12c50`. Their emptiness is real, not evidence that subscriptions
are unused. `player` overrides both: retail `subscribe_on_actions` at
`0x5d2e40` pushes into the player-owned subscriber vector, and unsubscribe at
`0x5d24b0` removes from that vector. Do not put a new list in the eight-byte
base class or devirtualize calls onto its no-op defaults.

Retail damage-zone entry at `0x588760`, statement 9, adjusts `this` to its
`player_actions_subscriber` base at +0x30 and calls the receiver's virtual
subscription slot at +0x1c. Statement 10 then calls the owning anomaly's
`on_hit_receiver_enter`. Leave/deactivate similarly unsubscribe. Preserve this
dispatch and remove only the two misleading base no-source comments.

The separate `hit_receiver::{on_enter_damage_zone,on_leave_damage_zone}`
helpers take a resource-pointer reference. The observed zone callbacks use raw
zone/receiver pointers and do not establish that these helpers were inlined
there. The likely semantic bundle is subscription plus owner enter/leave
notification, but their owner guard, notification order, repeated-entry policy
and reference lifetime are unproven. The core itself is not the resource-owning
base: concrete `damage_zone` combines `unmanaged_resource` and `damage_zone_core`.
Do not manufacture a `damage_zone_core_ptr(this)` temporary or new friendship
just to force this possible seam into an already observed raw-pointer path.
These two bodies stay explicitly open, not “server-only.”

## Pointer equality is not a style choice

Retail `hit_receiver_info::operator==(hit_receiver_info const&)`, RVA
`0x587d80`, loads each receiver, then the receiver's +4 loose-pointer data,
then compares the two payload pointers at data +0. This exactly supports the
existing double-indirection expression. Keep it and resolve its generic
“think about this” TODO with the actual evidence. Do not replace it with
ordinary receiver-pointer equality.

The raw-receiver overload has no inspected retained consumer. Implement simple
raw-pointer equality as an explicit model; whether it originally performed the
same loose-payload comparison, and its null/expired-object policy, remain open.
The distinction must not be silently hidden by calling one overload from the
other. `hit_receiver` destruction at `0x9b9c0` really decrements/frees the loose
data or clears its payload: an empty user destructor still has generated work.

## Cleanup models and an important contact-test limitation

`remove_left_receivers_predicate` stores a pointer to a supplied buffer of
`hit_receiver_info`. Supply its previously missing body as “not found in that
buffer,” using the existing info-to-info comparison. Missing-membership
polarity, whether the buffer describes current or departed receivers and
whether erasure also notifies remain explicit questions. Its constructor and
buffer ownership are not changed.

`damage_zone_core::remove_null_receivers` is a raw-null erase/remove model,
using the existing `erase_null_ptrs` functor. This does not pretend a null
receiver and a cleared loose payload are the same event. Its private inline
definition lives in the owning `.cpp`, after `hit_receiver_info` is complete;
all current zone callback definitions are in that TU. No header include cycle
or public traversal API is added, and no unobserved cleanup call is inserted
into a live callback.

The predicate's PDB noncopyable base is preserved. Use a copyable
`boost::bind<bool>(boost::ref(predicate), _1)` adapter for the standard
algorithm rather than copying that functor. The pinned Boost headers were
inspected: this older `reference_wrapper` has no call operator, so passing
`boost::ref(predicate)` directly to `std::remove_if` would not be sufficient.
This source model is unbuilt and its exact original loop/algorithm is unknown.

`erase_old_receivers::operator()` is still a missing definition with an
individual hypothesis: test contact/membership, notify leaving receivers and
return erasure eligibility. Its zone reference alone does not select that
policy. In particular, retained `collision_sensor::contact_test(object)` at
`0x58a640` ignores its object argument and calls each geometry's zero-argument
contact test. Therefore a guessed `!zone->contact_test(info.m_rigid_body)` is
not established as a receiver-specific overlap test. Do not blindly implement
that tempting expression. A surviving cleanup caller/another target is needed
to distinguish contact, list-age and notification behavior.

## Recover and propagate the armor-piercing seam

Retail `hit_on_inside`, RVA `0x588bf0`, statement 23 at +0x273 performs exactly
`lerp(min_armor_piercing, max_armor_piercing, hit_coeff)`. The prior statements
evaluate/clamp the curve coefficient; the named `armor_piercing_value` local
is const. Implement the declared private `calc_armor_piercing(float)` with
that arithmetic and use it in this existing caller. Keep the caller's local
and earlier clamp; do not add another clamp, reinterpret the float as raw
damage, or rewrite the two other hit-mode bodies. The helper-input contract
is still an inference from this consumer; its own procedure is not retained.
The frozen target/base structure diff reports `SIZE +0x12` at this armor
statement, alongside other arithmetic and tail differences. That is baseline
evidence for checking the boundary, not a post-edit result or a closed wall.

The standalone accessors also have real callers: generic-anomaly link resolution
stores false at core +0x108, and `simple_game_project::insert/remove` tests
concrete damage-zone +0x210 (the core base starts at +0x108). Preserve the
existing setter/getter. `owner()` becomes the direct pointer getter model for
the field assigned/reset by zone activation/deactivation; no new ownership.

`set_resources(queries_result&,u32)` and `hit(float)` remain open. The current
game cook collects query results and calls concrete `damage_zone::load`, while
core geometry ownership goes through the collision-sensor/link-resolver path.
Query ordering, whether u32 is a start index or count and resource lifetime
cannot be recovered merely from that signature. `hit(float)` likely packages
some common receiver/bone traversal, but the three retained hit modes use
different timing, speed/curve and armor policies. The float could be a scale,
time contribution or curve position. No arbitrary common body is installed.

## Every declaration

| # | Declaration | Disposition |
|---:|---|---|
| 1 | `hit_receiver` virtual destructor | Existing empty user body retained; observed generated loose-data cleanup at 0x9b9c0. |
| 2 | `cast_to_hit_receiver` | Retained 0x9b350 returns this; existing override correct. |
| 3 | `subscribe_on_actions` | Retained empty base default; live zone calls dispatch to player override. Remove marker. |
| 4 | `unsubscribe_from_actions` | Same base/default versus derived/owned-list distinction; remove marker. |
| 5 | `on_enter_damage_zone` | Open notification/subscription/resource-lifetime contract, not proved absent by missing procedure. |
| 6 | `on_leave_damage_zone` | Open symmetric leave/ownership contract; no invented reference temporary in zone callbacks. |
| 7 | `hit_receiver_info::operator==(hit_receiver const*)` | New raw-pointer equality model; loose-payload and null policies unresolved. |
| 8 | `erase_null_ptrs::operator()` | Existing raw-null predicate; retained as cleanup model, not proof of expired-pointer policy. |
| 9 | `erase_old_receivers` constructor | Existing reference capture matches PDB owner field; no added refcount copy. |
| 10 | `erase_old_receivers::operator()` | Missing definition remains explicitly open; contact-test argument omission defeats the obvious unverified receiver-overlap hypothesis. |
| 11 | `remove_left_receivers_predicate` constructor | Existing pointer to caller-owned buffer matches PDB field. |
| 12 | Its `operator()` | Supply missing not-in-buffer model, using observed info-to-info comparator; erasure polarity/notification questions retained. |
| 13 | `damage_zone_core::set_resources` | Open query-index/count and geometry ownership contract; inspected actual game cook/link resolver instead of declaring it unused. |
| 14 | `set_standalone` | Existing store observed in generic anomaly resolve_links statement 14. |
| 15 | `is_standalone` | Existing getter observed in simple_game_project insert +0xd0 and remove +0xe0, accounting for concrete-class base offset. |
| 16 | `owner` | New direct m_owner getter model; retained activate/deactivate and notification paths establish pointer ownership. |
| 17 | `remove_null_receivers` | New private raw-null erase model through existing noncopyable predicate; no unobserved live call inserted. |
| 18 | `remove_left_receivers(objects)` | Open current-versus-departed input-set contract, receiver conversion, duplicates and notifications; separate from existing observed on_leave. |
| 19 | `calc_armor_piercing` | New coefficient-based lerp and restored inside-hit caller seam; exact helper contract still inferred. |
| 20 | `hit(float)` | Open common-hit hypothesis; input role and timing/bone/curve policy not established by the three distinct retained hit modes. |
| 21 | `compare_body_parts_predicate` constructor | Existing name capture seen inside contact-result filtering statement 5. |
| 22 | Its `operator()` | Expanded string comparison in retained std find_if implementation 0xb8e70, statement 3; existing body retained. |
| 23 | `dz_bone_data_contact_test_predicate` constructor | Existing vptr/result-pointer/filter-pointer initialization at hit_on_inside statement 10; no new ownership or copy. |

Raw topology is identical for hit receiver, receiver info, damage-zone core and
the three cleanup predicate types. No visibility or layout correction is
justified in this bundle. Six literal markers are resolved and one additional
previously undefined predicate gets a model; several explicitly listed bodies
are still open. Deferred build must check the armor caller partition and new
template-based cleanup definition. Classification is not completed byte recovery.
