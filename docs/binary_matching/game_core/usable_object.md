# usable_object.cpp - structure verification (item/material batch)

| fn | old marker | report.json | structure | wall |
|---|---|---|---|---|
| ctor() | 84.24% PARTIAL | 84.24% | MATCH 0/0 | link_resolver base-subobject ctor inline-vs-call (LTCG) |
| ~usable_object | 100% DONE | 100% | - | carcass stripped |
| load | 94.15% DONE (STALE) | 100% | MATCH | now 100% |
| resolve_links | 98.15% DONE (STALE) | 99.9% | MATCH 5/5 | [ebp-N] slot-number noise only |
| insert | 100% DONE | 100% | - | carcass stripped |
| remove | 100% DONE | 100% | - | carcass stripped |
| get_transform | BLOCKED | n/a (no base symbol) | - | legit BLOCKED |

## ctor (84.24%)
structure-diff: 0/0. The link_resolver base-subobject (at offset +4) init differs: target
inlines the `mov dword[+4],0` store (frame `sub esp,10h`, extra `[ebp-8]` slot), base
emits an out-of-line `call 0x9E`. Whether the base subobject ctor is inlined is decided by
the base class declaration, not this ctor's source. Non-steerable.

## load: now 100%
Marker bumped 94.15% -> 100%. ASSERT(UNKNOWN_EXPRESSION) + binary_config_value size() +
VOSTOK_MALLOC_IMPL all byte-exact now.

## resolve_links (99.9%)
structure-diff: 5/5 stmts, 0 size, 0 quantity. The only asm diff is `[ebp-38h]` vs
`[ebp-3Ch]` / `[ebp-3Ch]` vs `[ebp-24h]` slot-number assignment - pure allocation noise
(MATCHING.md: slot numbers are noise, never reordered). Effectively a clean match;
bumped DONE. sushi@TODO about `i < m_collision_geometries` is a separate observation, not
a structure issue.

## get_transform: BLOCKED (correct)
Target has the symbol (0x5908e0) but our base emits NO symbol / no report.json entry - the
body `return m_collision_geometries[0]->get_transform()` is written but LTCG-eliminated
because its only caller (game_world_ui::update_minimap_objects) is not yet matched. Legit
BLOCKED; left as carcass stub - matcher work, not verification.

VERDICT: ctor MATCH (wall), resolve_links MATCH (noise), load now 100%. No logic change.
