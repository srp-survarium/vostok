---
name: vostok-review-match
description: Review one Vostok matching commit, PR, batch, or linear stack for target faithfulness, structure, build safety, regressions, fabricated symbols, stale comments, and per-commit measurement integrity. Use before landing matching work or when asked for a correctness review plus cleanup. Do not reduce review to conventional style or fuzzy percentage.
---

# Review matching work

1. Establish the exact base and head. Verify the intended range is linear and
   each commit carries a README/ledger snapshot generated for that source.
2. Read `AGENTS.md` and the relevant matching rules. Inspect the complete diff,
   commit sequence, generated score changes, open `sushi@TODO` ledger entries,
   and regression reports.
3. Review for:
   - incorrect or invented behavior relative to target evidence;
   - wrong layout, const, locals, visibility, initialization, control flow, or
     definition order;
   - fabricated symbols or cross-unit codegen hacks;
   - guessed constants without a documented open question;
   - accidental logs, traces, debug code, or generated artifacts;
   - stale carcasses, percentages, verdicts, and comments narrating code as
     formerly removed/missing/stubbed after reconstruction;
   - unresolved review comments missing either the source marker or ledger row.
4. Preserve comments that explain a current non-obvious constraint, live state
   markers, grounded optimizer walls, and unresolved `sushi@TODO` evidence.
5. Validate suspicious structure with `$vostok-verify-structure`; validate
   implementation fixes with `$vostok-match-unit`.
6. Make focused corrections on a new review branch above the stack. Prefer one
   review commit. Do not rewrite the reviewed stack.
7. Run a successful full `python3 -m vostok build` after the final edit.
   Include its `README.md` and ledger in the review commit. If it cannot
   succeed, do not commit source changes.
8. Report findings by severity with file/line evidence, changes and hashes,
   measured score/regression effects, unresolved risks, and a clear
   ready/not-ready-to-fast-forward verdict. Do not merge or push unless asked.
