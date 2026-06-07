# game_core::victory_item_core - structure-verifier pass

Verifier-matcher pass (2026-06-07, chain/game_core-sv-item_object_batch). Diffed every
non-100% function with `--view structure-diff --condensed` after a clean rebuild. No logic
changes; markers synced to report.json.

### victory_item_core (ctor)  (94.12% DONE)
target 0x58c450 / base 0x4495e0, 1/1 stmt, 0 diffs. Init-list + `m_transform.identity()`
aligns. Residual is the `unmanaged_resource` base ctor inline-vs-call (whole-program LTCG).
STRUCTURE MATCH; non-steerable.

### ~victory_item_core  (74.33% PARTIAL)
target 0x58c3f0 / base 0x449580, 1/1 stmt, aligned 0, size-diffs 1, quantity-diffs 0.
Sole SIZE on `VOSTOK_DELETE_IMPL( g_allocator, m_collision_geometries[0] )` (1 byte): the
inlined collision_geometry destructor differs by one byte in the whole-program inline form.
STRUCTURE MATCH; non-steerable.

### load  (marker 80.17% [stale] -> 85.05% PARTIAL)
target 0x58c4e0 / base 0x449670, 4/4 stmts, aligned 2, size-diffs 2, quantity-diffs 0.
SIZE on:
- `*m_collision_geometries = VOSTOK_NEW_IMPL( g_allocator, collision_geometry )` - the
  allocation/new inlined to a different size.
- `m_collision_geometries[0]->load( cfg["collision_geometries"][0] )` - the cfg[...]
  config-value index op inline form.
STRUCTURE MATCH; non-steerable whole-program LTCG.

## Verdict
All three non-100% functions are STRUCTURE MATCH. Residuals: base-ctor (unmanaged_resource),
element dtor, VOSTOK_NEW + config-value op inline-vs-call. None source-steerable.
