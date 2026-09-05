<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Engine-wide inline reconstruction follow-up

Stack base: PR 569 at `855dc1283`. This follow-up keeps the larger engine census
and subsequent source batches separate from that PR's reviewed work. No build
is scheduled during the reconstruction pass.

## Baseline, not a missing-function count

At this baseline, the literal `/* no source */` scan under `sources/vostok`
finds 598 marked lines in 136 files:

| Source subtree | Marked lines |
|---|---:|
| game | 324 |
| game_core | 205 |
| scaleform | 57 |
| physics | 6 |
| animation | 5 |
| render | 1 |

These are deliberately **not** reported as 598 missing implementations.
Some functions already have working bodies but stale annotations; multiline
signatures, overloads, inactive source, explicit `STATE[STUB]` markers and
unmarked fake-return bodies also require a declaration-level census. The wider
marker scan must include REMOVED/STUB/UNMATCHABLE as well. Neither scan proves
complete semantic coverage.

Each reviewed class receives a per-function register covering **all** its
inline declarations, not merely the grep hits, plus related retained methods
where they supply evidence. For each method, check PDB access and
layout, retained procedures, source consumers, target caller operations and
historical/sibling code. Mark source priors and semantic models separately
from verified retail expansions. Explicitly retain any unresolved alternative.

## Reviewed bundles

| Bundle | Declarations reviewed | Result | Evidence |
|---|---:|---|---|
| `circular_buffer<T>` | 22 | 13 stale markers; 3 new models; 3 open bodies; 3 existing unmarked methods reviewed | [individual register](game_core/circular_buffer_inline_recovery.md) |
| `player` inlines | 33 | 13 stale markers; 1 forwarding seam; 1 stamina getter model; 8 open bodies; 10 existing unmarked overrides reviewed | [individual register](game/player_inline_recovery.md) |
| Scaleform reference mirrors | 71 marked declarations | All have existing SDK/generated-source owners; not active missing bodies. Includes 57 literal markers and 14 other distinct STUB declarations | [ownership register](scaleform/inline_reference_ownership.md) |
| `player_input_handler` + `game_camera` inlines | 14 | Real input-priority fix (0 → 10); 10 stale markers; 3 existing setters; no missing bodies remain in these classes | [individual register](game/camera_input_inline_recovery.md) |
| `weapon_ammunition` + `empty_hands` inlines | 31 | Real empty-hands deactivation fix; remove two unsupported ammunition bool results; seven stale literal markers; explicit unreachable/boundary questions | [individual register](game_core/ammunition_empty_hands_inline_recovery.md) |
| `weapon_core` + animation-selector inlines | 65 | Replace unmarked zero ammo total; restore/propagate getter and predicate seams; remove invented derived-class friend; classify all 19 literal markers; retain two getter-model and caller-boundary questions | [individual register](game_core/weapon_core_inline_recovery.md) |
| Weapon/player state bases + inventory/interactive/world-object bases | 39 | Restore firing setter in four callers and two unmarked argument-copy hooks; classify nine literal markers; three flag models and shared-trap extent remain open | [individual register](game_core/state_object_inline_recovery.md) |
| Trap set + individual trap inlines | 24 | Restore removal/configuration seams; remove invented matrix-access friendship; explicit count/filter and unreachable models; raw bytes resolve non-returning inventory-hook extents | [individual register](game_core/trap_inline_recovery.md) |
| Missile/spore family + artefact-base constructor | 21 | Two simple models; every remaining method has a semantic hypothesis and specific ownership/configuration/timing evidence gap. No observed current construction path; not claimed server/editor-only | [individual register](game_core/missile_spore_inline_recovery.md) |
| Player input + normal RNG + shotgun substates | 33 | Fix live RNG seed restore; recover single-input/zero-local caller seams; replace three private playback accesses with base finalization; walking and eight RNG contracts remain open | [individual register](game_core/input_random_reload_inline_recovery.md) |
| Breath-state family and calculator inlines | 40 | Seven real 0-versus-1 float fixes including the live vibration multiplier; consolidate seven duplicate header bodies onto existing owners; recover empty lifecycle defaults; interpolation model and two reset/delay bodies remain open | [individual register](game_core/breath_recoil_inline_recovery.md) |
| Stealth + ladder/landing-point inlines | 23 | Restore null link/resource initialization and direct-pointer predicates in their existing cook; restore three const locals; four stealth getter models and four explicit detection/selection gaps | [individual register](game_core/stealth_ladder_inline_recovery.md) |
| Stamina + subscriber/dispatch inlines | 15 | Restore live subscriber null initialization; review jump/medkit/weight getter expansions; implement four-field writer model with explicit clock-sentinel uncertainty | [individual register](game_core/stamina_inline_recovery.md) |
| Damage model, body parts, hit parameters, protectors and thresholds | 59 | Two real data fixes (stats destination and one-byte affect count); const-local repair; ten literal placeholders reconstructed as bounded models; all 17 implementation-file predicates checked in retained consumers; health/color/minimum and producer details remain open | [individual register](game_core/damage_inline_recovery.md) |
| Medkit, oxygen tank and lifebone inlines | 38 | Constructor-installed vtables and retained bodies refute stale missing-symbol claims; restore six argument-use bodies, remove six unsupported bool constants, preserve silent-action/scheduled-tick ownership and distinct lifebone writer; assertion/result models remain open | [individual register](game_core/medical_item_inline_recovery.md) |
| Damage zone, hit receiver and local predicates | 23 | Restore armor-piercing caller seam; distinguish empty subscription defaults from live player overrides; verify unusual loose-payload comparison; cleanup/equality models plus individual query/notification/hit gaps, including a declaration-only missing body | [individual register](game_core/damage_zone_inline_recovery.md) |
| Anomaly core, state and group inlines | 12 | Restore declared state notification forwarder and caller; verify owner/getter/setter expansions and empty hook default; hit/explosion event models and hook/source-boundary questions remain explicit | [individual register](game_core/anomaly_inline_recovery.md) |
| Collision sensor, geometry, subscriber and predicates | 19 | Real inverted removal-predicate fix; preserve live virtual notifications, null cast defaults and generated subscriber vptr stores; three getter/debug-forwarding models with explicit consumer/guard gaps | [individual register](game_core/collision_sensor_inline_recovery.md) |
| Scheduler inlines and both frame consumers | 12 | Real per-frame mode-bit fix and const-local repair; recover unmarked status-transfer helper and guarded activation models; preserve verified registration/unregistration and callback-copy semantics, with transition timing explicitly open | [individual register](game_core/scheduler_inline_recovery.md) |
| Collision user, usable-object cast, hit initiator and death subscribers | 20 | Real missing death-subscriber next initialization; propagate three existing factor seams and repair three const locals; preserve generated vptr stores and independently checked virtual defaults; default-constructor/source-boundary models remain open | [individual register](game_core/collision_user_inline_recovery.md) |
| Material manager, material and pair inlines | 23 | Three bounded name/ordered-pair lookup models; verify retained getters and by-value resource setters; preserve generated destruction and record missing consumers plus unresolved noinline/friend relationships separately | [individual register](game_core/material_inline_recovery.md) |
| Bullet manager, callback pool, predicates and bullet getters | 28 | Real particle-normal destination fix; restore allocator assertion and typed/raw forwarding, tracer and cleanup seams; three const-pointer repairs; explicit decal-clear/accessor models and preserved retail loop hazards | [individual register](game_core/bullet_inline_recovery.md) |
| Update/hit/relocation/options packets and update-queue consumer | 22 | Six bounded inverse-wire models; repair byte-ID read types, remove undefined extra copy declaration, preserve generated memberwise operations, and fix queue construction/assignment order; original missing-direction consumers remain open | [individual register](game_core/update_packet_inline_recovery.md) |
| Profile, item, slot and booster inlines | 11 | Three inverse-wire/forwarding models; fix positive mode checks in the live reader expansion; verify constructors, raw slot-mode table and id-only equality; preserve distinct compact-profile/shop/raw-lobby formats, with sender/sparse/narrowing policy open | [individual register](game_core/profile_packet_inline_recovery.md) |
| Leg-IK predicates/timers/drawer and consumer anchors | 21 | Real elapsed-timer initializer fix (two 0.1f → 0.0f stores); recover rotation/line wrapper seams and five placeholder bodies; retain verified ground/timer operations and generated cleanup, with support/ownership/const questions open; raw base access subsequently verified | [individual register](game_core/legs_ik_inline_recovery.md) |
| Locomotion/jump state hooks, readiness and destruction | 25 | Eight stale markers removed after raw installed-vtable/dispatch verification; keep landing false readiness distinct from callback completion, preserve generated resource cleanup and abstract jump-base vptr stores; inactive selection assertion spelling remains open | [individual register](game_core/locomotion_state_inline_recovery.md) |
| Animation analyzer, result buffer and retained producer | 14 | All eight missing algorithms have individual hypotheses/gaps; source/xrefs/full retained cook prove no sampling there; preserve unfilled retail result path, borrowed analyzer inputs and buffer cleanup; original active producer and getter questions remain open; raw base access subsequently verified | [individual register](game_core/animation_analysis_inline_recovery.md) |
| Cook helpers, registration owners and generated cleanup | 11 | Selected hit-type getter and core-shotgun constructor models; repair callback parent constness; keep core registration/skeleton-config seams open and distinct from retained game/resource pipelines | [individual register](game_core/cook_inline_recovery.md) |
| Player-owned history/state/profile records and input predicate | 13 | Verify implicit member construction/destruction and profile deletion through its owner; remove three stale markers; preserve timestamp/scene-removal ownership, with generic predicate spelling open and raw base access subsequently verified | [individual register](game/player_owned_inline_recovery.md) |
| Dead/preview state construction, hooks and FSM consumers | 15 | Real dead-state readiness false→true fix; six stale markers removed through live constructors and installed vtables; verify IDs/seeds/borrowed cleanup, retaining const-RNG spelling and preview partition questions | [individual register](game/dead_preview_state_inline_recovery.md) |
| Game match-client wrappers and queue/callback consumers | 20 | Verify queue/options/timestamp getters and delegates; restore order-allocator deletion model; remove stale invented network-client friendship; private connect hook and original delete consumer remain open | [individual register](game/match_client_inline_recovery.md) |
| Network-client remaining inlines, descriptors and packet consumers | 24 | Model retail's non-returning load hook; restore connection flag getter/respawn seam and byte-ID deletion model; verify virtual defaults and expose false deserialize comment; creation/death wire contracts and signature variant remain open | [individual register](game/network_client_inline_recovery.md) |
| Messaging-client and account-list inlines/generated ownership | 13 | Restore sign-in-state predicate at ten guards and name getter at two consumers; verify list/reference getters, ID comparison and generated copies; setter copy/notification policy and named boundaries remain explicit models | [individual register](game/messaging_client_inline_recovery.md) |
| Lobby-client inline interface and inventory/UI consumers | 32 | Restore by-value inventory insertion seam and faction-points lookup model; verify callbacks/getters and fix two UI u32-count truncations; preserve u8 indices and record remaining search/element/name-consumer questions | [individual register](game/lobby_client_inline_recovery.md) |
| Game inline interface and world portal forwarding | 35 | Verify getters and resource return ownership; restore public spectator scene-switch seam, remove two invented friends, recover legacy debug toggle and clock/portal models; window-query bool/async contract remains individually open | [individual register](game/game_inline_recovery.md) |
| World inline interface, tracer ownership and NPC predicates | 50 | Consolidate duplicate tracer onto generated-method owner; remove three invented friends and restore public camera/physics seams; recover legacy NPC/visibility and predicate models, verify getter/resource-copy operations, and keep selection/hit/debug/query-producer gaps explicit | [individual register](game/game_world_inline_recovery.md) |

The eleven inline bodies and `can_prone` still open in PR 569 retain their
[individual evidence gaps](pr569_semantic_reconstruction.md); this follow-up
does not erase or relabel them as completed.

Checkpoint after the world/NPC inline audit: the registers account for 971
declarations, including existing bodies and explicitly open models/extent
questions. The literal scan is now 298 marked lines: game 177, game_core 52,
Scaleform 57, physics 6, animation 5, render 1. Excluding the 57 inactive SDK
mirror lines leaves 241 active marked lines, **not** a remaining-function
count. Unmarked placeholders and nonliteral STUB markers remain in scope.

The [raw-base access audit](pdb_base_access_rendering.md) closes the six-class
public/private discrepancy: both actual PDBs encode private; the generated
headers hardcode public. The flake renderer patch awaits a later tool build;
engine inheritance stays unchanged. This does not increase the function count.

Next work includes the remaining game logic/state and object families, plus
the still-open body models. SDK mirror ownership is already recorded,
not a remaining body-reconstruction queue. Do not call a client-unemitted helper editor-only without checking
actual consumers. Do not edit the fenced SDK to resolve a project carcass.
