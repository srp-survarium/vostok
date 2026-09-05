<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Anomaly core, state and group inlines

Scope: all eight generic-anomaly-core inlines, both anomaly-state inlines and
both zone-group inlines: **12 declarations**. No builds or score refresh.
Raw target/frozen-base topology is identical for all three classes.

## Restore the declared state notification seam

Retail `zone_group::on_zone_act`, RVA `0x58d080`, statement 1 loads the state
owner at group +0x18, then the core owner at state +0x28, and forwards the zone
and receiver unchanged to `generic_anomaly_core::on_zone_act`. The latter's
retained body consumes its arguments and sets the zone-trigger flag. No state-
specific counter, energy adjustment or early return is present in this path.

Implement the existing `anomaly_state::on_zone_act` as owner forwarding, and
route the group notification through that declared helper. `zone_group::core`
still owns the same two-level lookup for its other consumers. Include the
existing core declaration in the state header so the inline call has a complete
owner type; core only forward-declares the state, so this introduces no cycle.
No field, friend, virtual or allocation is added.

The operations are supported, but the exact source seam is inferred: the old
direct `core()->on_zone_act` form and the new state forwarder can both produce
this chain. The frozen baseline already has a 9-statement, 0x10f-byte structure
match. This is not a claimed byte-score fix; deferred verification must ensure
the helper boundary preserves that result. The new forwarding TODO records
this distinction rather than asserting that its original spelling is known.

## Two event-entry models, not verified event consumers

The source/reference search finds no current call to `on_hit_anomaly` or
`on_explosion_in_anomaly`, and neither has a retained standalone procedure.
That does not establish server/editor exclusivity or prove their original
empty bodies. Both have individually bounded models:

- `on_hit_anomaly`: mirror the retained player **shoot** event's configured
  energy increase and shoot-trigger flag. A hit on the anomaly is interpreted
  as a shot at it, not damage to a character. This interpretation is unproven:
  character-hit energy, trigger-only behavior or other event distinctions
  remain alternatives. Do not route the live player hit/shoot cases through
  this new model merely because some operations look alike.
- `on_explosion_in_anomaly`: add the configured explosion energy via existing
  `inc_energy`. Retail load statement 18, RVA `0x58c9c0 + 0x1d6`, reads
  `energy_on_explosion` into +0x58. Whether this entry also sets a zone/shoot
  trigger is unknown. No guessed flag update is added.

Both use `inc_energy`, preserving its observed energy-enabled guard rather
than bypassing it through a direct field write. No assertion or clamping
policy beyond that existing helper is invented. These models need original
event consumers before their semantics can be called verified.

## State-change default and owner construction

`state_changed` has a retained bare `ret` at `0x3f210`. Remove its no-source
marker, but do not insert a speculative call into `tick`. The retained and
frozen-base tick already have a 12-statement, 0xd4-byte structure match; the
current concrete `generic_anomaly` does not override the hook. Missing emitted
call instructions cannot distinguish an originally omitted call from an
optimized-away empty hook. Invocation timing relative to state assignment,
finalization and initialization stays an explicit question.

Retail load also confirms both unmarked constructors:

- Anomaly state: statement 24 constructs its vector at +0x1c and stores core
  owner at +0x28; it does not initialize all the other scalar fields there.
- Zone group: statement 38 constructs its vector at +0xc and stores state
  owner at +0x18. Do not add charged-count or recharge-time initialization
  simply because their later initialization happens elsewhere.

Keep those constructor bodies and remove only their derived inlining notes.

## Every declaration

| # | Declaration | Disposition and evidence |
|---:|---|---|
| 1 | `generic_anomaly_core::physics_world` | Existing +0x70 pointer getter; expanded in zone-group recharge statement 3. |
| 2 | `scheduler` | Existing reference-returning +0x74 getter; same recharge statement passes the stored scheduler. |
| 3 | `on_hit_anomaly` | New shoot-energy/trigger model with explicit event-identity alternatives; no current consumer found. |
| 4 | `on_explosion_in_anomaly` | New configured-energy model, preserving inc_energy guard; extra trigger behavior unresolved. |
| 5 | `current_time` | Existing +0xc getter; anomaly-state initialize at 0x58d560 statement 4 reads it before adding timeout. |
| 6 | `current_energy` | Existing direct +0x30 getter retained as field-based source prior; no specific getter consumer found. The field itself is used in retained state selection. |
| 7 | `set_current_energy` | Existing assignment; state execute at 0x58d4a0 statement 4 converts exit energy to float and stores at core +0x30. |
| 8 | `state_changed` | Verified empty default; original invocation timing remains open, no new call inserted. |
| 9 | `anomaly_state` constructor | Existing owner/vector construction verified in load statement 24. |
| 10 | `anomaly_state::on_zone_act` | New owner forwarder, propagated to group callback; same observed owner chain, exact inline source boundary unproven. |
| 11 | `zone_group` constructor | Existing owner/vector construction verified in load statement 38. |
| 12 | `zone_group::core` | Existing group-owner/core-owner chain seen in recharge and zone-action consumers; no new cached owner field. |

Four literal markers are resolved. The event policies, forwarding source form
and state-hook timing remain synchronized source/review TODOs. This register
does not classify the separate collision-sensor, geometry or artefact-container
classes; those are the next related owners. No new candidate measurement is
claimed, and existing banked matching results are not treated as work to chase.
