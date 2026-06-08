# Review TODOs (`sushi@TODO:`)

Open matching questions sushi raised in PR review, recorded in-source as `sushi@TODO:`
markers and indexed here so a later agent can sweep them in one pass.

- **Live set** (source of truth): `grep -rn "sushi@TODO:" sources/` — the markers travel with
  the code, so this is always current even if the table below drifts.
- **This table** is the curated index: one row per marker, with the function and the PR it came
  from. Append a row whenever you add a `sushi@TODO:` to the source; tick `Done` (and drop the
  marker) when the matching question is resolved.

Append-only ledger (union-merged across branches, like `PROGRESS.md` / `unanswered_questions.md`).

| Status | Function | File:line | PR | TODO |
|--------|----------|-----------|----|------|
| not done | `get_weapon_lexeme_pair_impl` | `game_core/sources/weapon_lexeme_pair.cpp` | #155 | Function is **not matched-done**: `STATE[None]` with no STRUCTURE DIFF — objdiff can't pair it (the LTCG inline-vs-call of the `animation_lexeme_parameters` setters shortens the body past the pairing threshold). Needs structure recovery before it earns a real % / embed. |
| open | `get_weapon_lexeme_pair_impl` | `game_core/sources/weapon_lexeme_pair.cpp` (`.bones_mask( 2 )`) | #155 | Is `2` a bare magic constant or a named value defined somewhere? |
| open | `get_weapon_lexeme_pair_impl` | `game_core/sources/weapon_lexeme_pair.cpp` (the `bool dummy` dead store) | #155 | Unlikely a `bool dummy` — if it were, `dummy` would appear in the locals (it does not). The lone 4-byte dead store needs further matching to recover the compiled-out structure (would let objdiff pair it: currently `STATE[None]`). |
