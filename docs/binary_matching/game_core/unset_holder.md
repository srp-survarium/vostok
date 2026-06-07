# inventory::unset_holder

See inventory.md for the full structure-verification trail (shared root cause:
item_in_slot() / intrusive_ptr operator inline-vs-call LTCG).

Verdict: STRUCTURE MATCH (shape ok). Residual = per-call-site LTCG inline-vs-call,
non-steerable from the caller. See the embedded `// STRUCTURE DIFF` / `// VERDICT`
in inventory.cpp for the condensed diff.
