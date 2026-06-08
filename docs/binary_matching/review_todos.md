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

## Provisional idiom candidates (NOT definitive — DO NOT promote to `assembly_patterns.md` yet)

These two readings were drafted from `game_core/get_weapon_lexeme_pair_impl`, but that function is
**unverified** (`STATE[None]` — it doesn't pair, so the match can't be confirmed) and the `bool
dummy` reading is exactly what was questioned in review above. They are recorded here only so the
future matcher has the candidate interpretation in hand; promote to `assembly_patterns.md` **only
after** a function that actually pairs confirms them.

**1. Conditional-pointer arg `val != u32(-1) ? &obj : NULL`**
```
push 0                  ; (an unrelated arg pushed first)
mov  edx, [ebp+20h]     ; val (a u32)
sub  edx, 0FFFFFFFFh    ; edx - (-1) == edx + 1
neg  edx                ; CF set iff edx+1 != 0  (i.e. val != -1)
sbb  edx, edx           ; edx = (val != -1) ? 0xFFFFFFFF : 0
lea  eax, [ebp-0E0h]    ; &obj
and  edx, eax           ; (val != -1) ? &obj : NULL
push edx
```
Source: write the ternary `val != u32(-1) ? &obj : NULL` (a pointer-or-NULL select on an unsigned
`!= -1` test) — NOT hand-rolled asm. MSVC /Od lowers `!= u32(-1)` to `sub r,-1; neg; sbb r,r` (mask
= all-ones iff non-equal) and ANDs it with the address. Candidate site: the offset lexeme's
`time_driving_animation` arg = `time_synchronization_group != u32(-1) ? &main_lexeme : NULL`.

**2. Lone 4-byte `mov byte[ebp-N],0` (no lea/call) = unused `bool b = false;`, not an ASSERT**
A structure statement of size `<0x4>` whose only instruction is `mov byte ptr [ebp-N],0` with NO
following `lea eax,[ebp-N]; call <empty_stub>` is a plain unused `bool` local dead-stored under /Od.
DISTINGUISH from a compiled-out `ASSERT` (`<0xc>` = the byte-store PLUS `lea+call`). Writing
`ASSERT(UNKNOWN_EXPRESSION)` for a `<0x4>` slot over-produces the lea+call. The 4-vs-7-byte disp is
just small-vs-large `[ebp-N]` offset (allocation noise). Candidate site: `get_weapon_lexeme_pair_impl`
L40, target `<0x4>` `mov byte[ebp-5],0`. **Caveat:** only valid if the surrounding match is correct
— here it is not yet (see the `not done` row).
