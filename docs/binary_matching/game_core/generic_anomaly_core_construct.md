# game_core::generic_anomaly_core_construct - structure-verifier pass

This TU holds `generic_anomaly_core::load`, the free `state_prio`, and
`generic_anomaly_core::resolve_links`. Their structure-verifier findings live alongside the
rest of the class in `generic_anomaly_core.md` (2026-06-07,
chain/game_core-sv-item_object_batch) - see that file for the per-function diffs.

Summary:
- load          96.31% [stale] -> 96.07% PARTIAL  (vectora::resize / math::min / VOSTOK_NEW inline-vs-call LTCG)
- state_prio    BLOCKED [obsolete] -> 99.63% DONE  (fully reconstructed, 5/5 stmts clean; residual is frame-slot)
- resolve_links 99.00% -> 100% DONE  (24/24 stmts clean; carcass stripped, bare STATE[100%|DONE])

All STRUCTURE MATCH; no source-steerable defect.
