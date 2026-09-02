---
name: vostok-match-unit
description: Binary-match one complete Vostok translation unit, or a small related bundle of tiny units, against the shipped Survarium target. Use when implementing target-only or low-match bodies, reconstructing a caller and its non-render callees, or closing genuine QUANTITY, SPLIT, LOCALS, layout, const, or symbol-presence mismatches. Do not use for ordinary feature development or broad module orchestration.
---

# Match one Vostok unit

1. Read `AGENTS.md`, `docs/binary_matching/MATCHING.md`,
   `docs/binary_matching/agentic_loop.md`, and the relevant module notes.
   Skim `docs/binary_matching/patterns/INDEX.md`; open only patterns matching the
   observed symptom.
2. Confirm the worktree and branch, the complete owned TU, explicit open
   functions, and disjoint file scope. Do not touch another worker's files.
3. Inspect target evidence first:
   - disambiguate with `pdb_rich_query --list`;
   - fetch target `structure`, `target`, `callees`, and `info` views;
   - slice dense statements by address.
4. Navigate source with `vostok tool clangd`. Reconstruct the normal C++
   form implied by target statements, locals, calls, layout, and mangling.
5. Descend into stub callees when required, except into `render`. Own and report
   the entire callee subtree. Do not fabricate a helper or edit another unit to
   force inlining.
6. Delete implemented carcasses and obsolete historical comments. Remove
   `STATE[STUB]` only after reconstruction. Keep live blockers and required
   `sushi@TODO` entries synchronized with `review_todos.md`.
7. Run the full `python3 -m vostok build` without a module argument. Inspect
   `report-changes.json` and query only the relevant slices of `report.json`.
8. Compare `structure-diff` before assembly `diff`. Fix source shape before
   chasing bytes. Repeat until structure matches or a concrete blocker remains.
9. Run the final full rebuild after the last edit. Include the generated
   `README.md` and `config/match_state.tsv` with the source in one commit.
   Never commit a source state whose ledger came from a different build.
10. Report per-function results, regressions, blockers with next actions,
    verification, and the commit hash. Do not push or merge unless authorized.

Always reproduce the target, including bugs and odd structure. Faithful
initializer lists, braces, locals, const qualifiers, access and definition
order outrank fuzzy percentage.
