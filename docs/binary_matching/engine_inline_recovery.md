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

The eleven inline bodies and `can_prone` still open in PR 569 retain their
[individual evidence gaps](pr569_semantic_reconstruction.md); this follow-up
does not erase or relabel them as completed.

Next work includes the player/history consumers' own inline methods, the
game-core weapon/object families, and Scaleform carcasses versus the pinned
SDK source. Do not call a client-unemitted helper editor-only without checking
actual consumers. Do not edit the fenced SDK to resolve a project carcass.
