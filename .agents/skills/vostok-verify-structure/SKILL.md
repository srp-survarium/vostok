---
name: vostok-verify-structure
description: Verify and fix whether reconstructed Vostok C++ has the target PDB statement structure, named locals, const types, access and definition order independently of fuzzy percentage. Use for high-score structural traps, batch audits after matching, or explicit structure verification. Do not use as a superficial byte-score review.
---

# Verify target source structure

1. Read `AGENTS.md`, `docs/binary_matching/MATCHING.md`, and relevant pattern
   entries. Start from current built indexes and reports.
2. Run `pdb_fetch --view structure-diff` first. For small functions, also compare
   target and base `structure` views. Pin ambiguous overloads by target RVA.
3. Compare:
   - statement quantity and order;
   - per-statement size;
   - named local sets and const qualification;
   - member initializer lists versus body assignments;
   - braces, lexical blocks, switch cases, and early-return shape;
   - declaration, definition, and access-specifier order.
4. Treat optimized-target PDB structure as a post-optimization projection. Do
   not delete real source solely because an optimized record omitted it, but
   continue steering every visible mismatch.
5. Classify each finding as `MATCH`, `QUANTITY`, `SIZE`, `LOCALS`, `ORDER`, or a
   combination. A high percentage does not excuse wrong structure.
6. Fix the mismatches using the `$vostok-match-unit` invariants. Never fabricate
   symbols, reorder for neatness, or modify a different unit to change inlining.
7. Remove stale carcasses, derived verdicts, percentages, and comments describing
   an obsolete removed/stub state. Preserve live blockers and `sushi@TODO` items.
8. Run a full `python3 -m vostok build`, re-run `structure-diff`, and inspect
   regressions. Iterate after every final source adjustment.
9. Commit the verified source together with that build's generated `README.md`
   and `docs/binary_matching/match.db`. Do not amend an unrelated historical
   commit or merge.
10. Report before/after structure classes and counts, remaining concrete
    residuals, regressions, verification, and the commit hash.
