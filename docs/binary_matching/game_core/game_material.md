# game_material.cpp - structure verification (item/material batch)

| fn | old marker | report.json | structure | wall |
|---|---|---|---|---|
| ctor() | 83.44% DONE (STALE) | 97.78% | MATCH 0/0 | m_name fixed_string("default") ctor inline-vs-call frame-slot (LTCG) |
| load_from_config | 88.10% PARTIAL (STALE) | 96.23% | MATCH 21/21 | binary_config_value access + buffer_string assign inline-vs-call (LTCG) |

## ctor (97.78%)
structure-diff: 0/0. Only asm diff is the frame slot: target `push ecx` (`[ebp-4]`) vs
base `sub esp,0Ch` (`[ebp-0Ch]`), driven by `m_name("default")` fixed_string ctor temp
materialized inline in base. All member stores byte-exact. Same fixed_string frame class
as hit_type_parameters ctor. Non-steerable.

## load_from_config (96.23%)
structure-diff: 21/21 stmts, 0 size-diffs, 0 quantity-diffs - perfect statement
alignment. Residual is the per-statement `val["..."]` binary_config_value operator[]/cast
and the `m_name = (pcstr)val["name"]` buffer_string assignment, which the target keeps
out-of-line and base inlines (sushi@MATCH: "Target uses stack for whatever reason"). The
trailing ASSERT( UNKNOWN_EXPRESSION ) reproduces the empty_stub. Non-steerable container/
string inline-vs-call LTCG.

VERDICT: both STRUCTURE MATCH. Markers synced, embeds added, carcass stripped. No logic change.
