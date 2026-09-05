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

The eleven inline bodies and `can_prone` still open in PR 569 retain their
[individual evidence gaps](pr569_semantic_reconstruction.md); this follow-up
does not erase or relabel them as completed.

Checkpoint after the missile/spore review: the registers account for 320
declarations, including existing bodies and explicitly open models/extent
questions. The literal scan is now 510 marked lines: game 294, game_core 147,
Scaleform 57, physics 6, animation 5, render 1. Excluding the 57 inactive SDK
mirror lines leaves 453 active marked lines, **not** a remaining-function
count. Unmarked placeholders and nonliteral STUB markers remain in scope.

Next work includes the player/history consumers' own inline methods, the
game-core weapon/object families, and Scaleform carcasses versus the pinned
SDK source. Do not call a client-unemitted helper editor-only without checking
actual consumers. Do not edit the fenced SDK to resolve a project carcass.
