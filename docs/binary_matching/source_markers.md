# Source markers — the matching-triage vocabulary (greppable)

In-source comments that record matching state. Keep them consistent so a later
sweep can `rg` the live set and know what each means. Counts are indicative
(tip ~2026-06-25).

## Un-bodied / unmatched function markers

| marker | ~count | meaning | grep |
|---|---|---|---|
| `/* no source */` | ~988 | UN-TRIAGED: an inline declaration with no out-of-line body in the PDB — there is no symbol to recover a body from. The starting point; refine it (below) as you learn why. KEEP it even after refining (it records that the body was never an out-of-line symbol). | `rg "/\* no source \*/"` |
| `STATE[STUB]` | ~315 | A function present but bodied as a stub (empty/minimal) awaiting real reconstruction. | `rg "STATE\[STUB\]"` |
| `STATE[INLINED]` | ~9 | REFINED from `/* no source */`: the body was reconstructed from a site where the target INLINES it. Annotate with the source: `// STATE[INLINED]: body from <fn> <va> (...)`. Keep the `/* no source */` too. | `rg "STATE\[INLINED\]"` |
| `STATE[REMOVED]` | (new) | REFINED: exists in headers but was NOT compiled — no TU references it, so the inline is never instantiated and it's absent from BOTH binaries. The empty stub is CORRECT (matches the target); no reconstruction needed. This is the counterpart to `STATE[INLINED]`: referenced→reconstruct, unreferenced→`REMOVED`/leave. | `rg "STATE\[REMOVED\]"` |
| `STATE[UNMATCHABLE]` | (new) | REFINED: a genuine tooling/structural wall (e.g. name-representation gap, optimized-COMDAT from an out-of-scope consumer) — real but not source-steerable. Document the wall. | `rg "STATE\[UNMATCHABLE\]"` |

**Refinement workflow (the `/* no source */` triage):** start at `/* no source */`
→ determine if any TU references it: unreferenced → `STATE[REMOVED]` (empty stub
is correct, done); referenced → reconstruct the body from the inlined call sites +
semantics → `STATE[INLINED]` (or `STATE[UNMATCHABLE]` if no decodable site),
**keeping** the
`/* no source */` marker. The `STATE[...]` form is canonical (consistent with
`STATE[STUB]`); earlier notes called the categories "inlined/dead/unmatchable" —
those are these `STATE[...]` tags, not `/* inlined */`-style comments. `/* no
source */` count is the progress signal — it shouldn't drop (kept), but the
fraction carrying a `STATE[...]` refinement should rise.

## Note markers (not function-state — context for reviewers/future matchers)

| marker | ~count | meaning | grep |
|---|---|---|---|
| `claude@NOTE` | ~733 | A matching limitation / failed-match rationale recorded at the function (inline-vs-call wall, FPO frame, cross-module block). Not an open question — a recorded conclusion. | `rg "claude@NOTE"` |
| `sushi@TODO` | ~358 | An OPEN review question (unresolved/suspect/investigate-later). MUST also have a row in `review_todos.md` (the curated index). | `rg "sushi@TODO"` |
| `sushi@NOTE` | ~142 | A reviewer note/observation (not necessarily an open question). | `rg "sushi@NOTE"` |

## Related

- `review_todos.md` — curated index of `sushi@TODO` rows (survives squash-merges).
- `divergence_queue.md` — the pdb_divergence diff→0 worklist.
- patterns/INDEX.md — recovered matching patterns/idioms.
- See [[no-source-triage-goal]]: reconstruct+wire real inlined-only fns (helps
  callers), tag DEAD artifacts, UNMATCHABLE walls; marker = progress signal.
