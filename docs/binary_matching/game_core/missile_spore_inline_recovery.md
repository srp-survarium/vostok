<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Missile and spore inline contracts

Scope: every `missile_weapon_core` inline (11), every `artefact_spores_core`
inline (9), and the one `artefact_base` constructor: 21 declarations total.
This is an individual semantic review, **not** a claim that all 21 bodies have
been recovered. Two straightforward models are implemented; complex open
contracts remain explicitly unbodied rather than being relabeled as no-ops.

## Evidence and access constraints

Both complete missile/spore class records and the referenced
`physics::bt_static_rigid_body` compare identically in the frozen raw target/base
PDB topology. The static body is defined in `physics/static_rigid_body.h`, not
in a filename matching its `bt_` class spelling. Its real interface supplies
`get_transform`, `set_transform` and `apply_impulse`; `physics::world` supplies
add/remove and contact subscribe/unsubscribe operations.

Target procedure queries for both complete class-qualified prefixes return
no functions. An engine-wide source scan finds the two headers, the spore
class's base-constructor call, and an include in `game/sources/player_cook.cpp`.
The current cook never constructs or calls this class. No separate `.cpp`,
further derived spore class, or active construction path was found. Neither
the include nor the retained type records proves the methods were unused in
the original source: they may have been inlined, conditionally used or part
of an unshipped consumer. Do not label them server/editor-only from this scan.

The overload distinction is important: spore `tick(u32,matrix)` and
`activate(u32,matrix)` override the **missile** interface. They do not implement
the `interactive_object` `tick()` or `activate(base_player&,engine&)` slots
inherited through `artefact_base`. This spore class therefore remains abstract
under that interface. Do not invent extra overrides just to instantiate it.

The callback field exactly matches the physics world's contact callback type.
That gives a plausible subscription seam, but no retained caller determines
when registration happens, whether injected bodies transfer ownership, or
which contact argument is the other object. There is no separate world-membership
flag. None of those details can safely be derived from a raw pointer's type.

## Every missile function

| Function | Current disposition and candidate contract | Evidence still needed |
|---|---|---|
| Constructor | Retain existing world/body pointer stores and automatic callback construction. Possible scalar initialization and callback binding remain open. | A constructor expansion or source prior for impulse, dispersion and activation defaults; do not assume zero from the member names. |
| Destructor | Unbodied ownership contract. Likely reverses contact registration and physics attachment; may also destroy the body. Automatic callback-member destruction already occurs. | Determine borrowed versus owned body and unsubscribe/destroy ordering from creation/removal callers. |
| `load` | Unbodied configuration contract. Candidate loads impulse, dispersion and activation policy, perhaps binds contact callback. | Real config subtree/key spelling, conversions/defaults, and callback-registration site. Literal key-name searches in the current source/config tree do not establish these. |
| `tick` | Unbodied time/transform contract. Candidate updates an unthrown/attached body or advances activation; matrix may be an attachment frame. | Distinguish attached tracking from free flight and establish time units/meaning of the matrix argument. |
| `activate` | Unbodied activation contract. Candidate installs transform, adds body to world, and registers contact callback according to immediate/delayed mode. | Collision group/mask, activation timing, whether subscription is already established, and whether registration belongs here or in throw. |
| `throw_weapon` | Unbodied launch contract. Candidate sets transform and applies an impulse derived from direction, impulse scalar and dispersion. | Third vector's meaning (direction, velocity or point), random distribution, application point, time role and add/activate ordering. Do not choose a random distribution merely because a dispersion field exists. |
| `on_contact_callback` | Unbodied base response. Could be an empty notification hook, a delayed-activation trigger or object removal. | A derived/base call path that distinguishes those materially different behaviors and identifies contact argument roles. |
| `get_activation_type` | Retain field getter for `m_type`; interface preserves immediate/delayed enum values 0/1. | Loading/default initialization remains unresolved; the getter does not repair it. |
| `get_transform` | Implement direct output assignment from `m_rigid_body->get_transform()`, using the real complete physics type. | Ownership-based model, not a verified expansion; confirm any null guard/assert and behavior after removal. No identity fallback is invented. |
| `remove_physics_object` | Unbodied removal contract. Candidate unsubscribes callback and removes body from world; destruction/nulling depends on ownership. | Match constructor/activation lifetime. Do not turn removal into deletion solely because both operations exist in the physics API. |
| `physics_world` | Retain nonconst member pointer getter. | Existing ownership-based accessor; no signature widening. |

## Every spore function

| Function | Current disposition and candidate contract | Evidence still needed |
|---|---|---|
| Constructor | Retain explicit missile-base delegation and implicit artefact-base construction. Cloud fields are not initialized by the current body. | Original defaults, allocation site and initialization ownership for ghost, owner, hit descriptors and clocks. |
| Destructor | Unbodied ghost/cloud lifetime. Likely removes/releases any remaining cloud before base destruction. | Ghost ownership, physics removal versus destroy operation, and whether `cloud_destroyed` is notified during destruction. |
| `load` | Unbodied configuration contract. Candidate delegates missile loading and sets cloud duration/hit descriptors and ghost shape. | Concrete configuration schema, string ownership, time conversion and allocation/geometry factory. |
| `set_owner` | Implement non-owning `m_owner = owner` pointer setter. | Field model; original assertion or propagation to physics user data remains unproven. |
| `tick` | Unbodied timed-cloud behavior. Candidate ticks flight until explosion, infects overlaps on a cadence and expires the cloud. | Whether timestamps are absolute/deltas, meaning of zero, expiry comparison, damage cadence/scaling and base-tick ordering. |
| `activate` | Unbodied initial activation. Candidate delegates missile activation and sets/reset clocks or creates an immediate cloud. | Distinguish immediate versus delayed explosion, clock initialization, ghost registration and transform ownership. |
| `on_contact_callback` | Unbodied explosion trigger. Candidate filters contacts, removes the projectile and creates/positions an infection cloud. | Which objects qualify, whether owner/self is excluded, repeated-contact handling and source of the cloud shape. |
| `cloud_destroyed` | Unbodied notification hook; empty default is plausible but not established. | Further derived class or caller expansion to distinguish notification-only from required cleanup. |
| `infect_objects_in_cloud` | Unbodied overlap/damage traversal. Candidate walks ghost overlaps, resolves hit receivers and applies owner/body-part/hit/amount/piercing parameters. | Target traversal/filtering, hit overload, repeated damage policy and whether amount is per tick or per second. |

## Artefact-base constructor

`artefact_base()` delegates to `inventory_item(inventory_item::use_silent)`;
retain that existing body. It contributes the inventory base to the spore
object and does not supply missing interactive-object virtual implementations.

## Next evidence, not a deferred-build cure

No build of the same unconsumed declarations can establish these missing
lifetime/configuration contracts. Look for original caller/source/config
artifacts or a concrete derived implementation; then follow world-add/remove,
contact callbacks and ghost-hit consumers as one lifetime. Existing physics
and trap implementations supply available operations, not proof of which
sequence this family used. Do not fabricate a reachability anchor or delete
the type/include records to hide the open work.

All 21 functions are individually managed. The two new bodies are models;
14 literal no-source bodies remain genuinely open across the pair, along with
the existing constructor-default questions. No C++ build or ledger update was
run, and the campaign remains incomplete.
