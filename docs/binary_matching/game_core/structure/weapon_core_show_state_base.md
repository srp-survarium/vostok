# Structure verification: weapon_core_show_state_base {ctor, initialize, finalize, on_animation_end_impl}

Unit: sources/vostok/game_core/sources/weapon_core_show_state_base.cpp (PR #125)

Verifier scope: SOURCE STRUCTURE only (statement QUANTITY + per-statement SIZE),
target vs base, independent of the byte/fuzzy %. No rebuild, no logic change.

## Verdict (combined)

ALL FOUR: STRUCTURE MATCH. Statement counts and per-statement sizes are identical
target-vs-base for every function. The four are genuine 100% over the correct
structure - nothing to flag, nothing to downgrade.

| function               | target rva | base rva | target stmts | base stmts | bytes | report.json % |
|------------------------|------------|----------|--------------|------------|-------|---------------|
| ctor                   | 0x6ed380   | 0x44b030 | 3            | 3          | 0x6b  | 100.0         |
| initialize             | 0x6ed3f0   | 0x44b000 | 5            | 5          | 0x2f  | 100.0         |
| finalize               | 0x6ed350   | 0x44afd0 | 4            | 4          | 0x21  | 100.0         |
| on_animation_end_impl  | 0x6ed320   | 0x44afa0 | 5            | 5          | 0x2d  | 100.0         |

report.json (binaries/objdiff/report.json, built locally at HEAD) shows the whole
unit at fuzzy_match_percent 100.0, 4/4 matched functions, .text 100.0%. Each
function's per-function entry is 100.0 (confirmed by reading report.json directly;
no rebuild performed). The .rdata section is 75.0% but that is not a function's
structure (data, not code) and does not affect any of the four.

## The four skeletons (target vs base, statement by statement)

Format: `offset  <size>  statement`. Target prints `Lnn` source-line tags; base
prints the rendered source statement (same skeleton, source form).

### ctor  (3 statements, 0x6b bytes) - MATCH
```
        TARGET                          BASE
0x00  <0x55>  L21                0x00  <0x55>  {   (decl + member-init list)
0x55  <0x0d>  L22                0x55  <0x0d>  m_body_part_mask_for_user = animation::body_part_whole_body_but_hands;
0x62  <0x09>  L23                0x62  <0x09>  }
```
Sizes identical (0x55, 0x0d, 0x09). L21 = the ctor decl line, to which the
member-init list `weapon_core_animation_end_aware_state(weapon, true), m_is_shown(is_shown)`
is attributed (collapses into the one 0x55-byte opener: base-ctor delegation +
compiler-emitted vtable stores + the m_is_shown ref store). L22 = the body
assignment that OVERWRITES the inherited m_body_part_mask_for_user (-1 -> -3).
L23 = scope exit. The init-list-vs-body split is correct: the one body assignment
is its own statement, everything else is on the decl line. QUANTITY match, SIZE match.

### initialize  (5 statements, 0x2f bytes) - MATCH (the PARTIAL->100 jumper)
```
        TARGET                          BASE
0x00  <0x09>  L26                0x00  <0x09>  {
0x09  <0x08>  L27                0x09  <0x08>  weapon_core_animation_end_aware_state::initialize( );
0x11  <0x0c>  L28                0x11  <0x0c>  ASSERT( UNKNOWN_EXPRESSION );   <-- recovered ASSERT
0x1d  <0x0e>  L30                0x1d  <0x0e>  m_weapon.instant_toggle_start( );
0x2b  <0x04>  L31                0x2b  <0x04>  }
```
Sizes identical (0x09, 0x08, 0x0c, 0x0e, 0x04). The load-bearing statement is L28
at offset 0x11, size 0x0c: that is exactly the compiled-out-ASSERT sequence
(`mov byte[ebp-1],0; lea eax,[ebp-1]; call empty_stub`, ~0x0c bytes). The ASSERT
recovery genuinely emits this statement - it is present on BOTH sides with a
byte-exact 0x0c delta. This is NOT a high-%-over-wrong-structure fluke: the
statement quantity is 5-vs-5 and the ASSERT slot has the right size. The earlier
75.13% PARTIAL was the pre-ASSERT base (missing that one statement); recovering the
ASSERT (a real compiled-out assert, NOT "LTCG") closed it. QUANTITY match, SIZE match.

### finalize  (4 statements, 0x21 bytes) - MATCH
```
        TARGET                          BASE
0x00  <0x07>  L34                0x00  <0x07>  {
0x07  <0x08>  L35                0x07  <0x08>  weapon_core_animation_end_aware_state::finalize( );
0x0f  <0x0e>  L37                0x0f  <0x0e>  m_weapon.instant_toggle_end( );
0x1d  <0x04>  L38                0x1d  <0x04>  }
```
Sizes identical (0x07, 0x08, 0x0e, 0x04). No ASSERT here (the empty_stub sequence
appears only in initialize/on_animation_end_impl). QUANTITY match, SIZE match.

### on_animation_end_impl  (5 statements, 0x2d bytes) - MATCH (the PARTIAL->100 jumper)
```
        TARGET                          BASE
0x00  <0x09>  L41                0x00  <0x09>  {
0x09  <0x0c>  L42                0x09  <0x0c>  ASSERT( UNKNOWN_EXPRESSION );   <-- recovered ASSERT
0x15  <0x0c>  L43                0x15  <0x0c>  m_is_shown = true;
0x21  <0x06>  L44                0x21  <0x06>  animation_player_tick_result = true;
0x27  <0x06>  L45                0x27  <0x06>  }
```
Sizes identical (0x09, 0x0c, 0x0c, 0x06, 0x06). The load-bearing statement is L42
at offset 0x09, size 0x0c: the same compiled-out-ASSERT sequence, here as the FIRST
body statement (before the two bool stores). Present on both sides, byte-exact 0x0c.
The earlier 69.93% PARTIAL framed this as "LTCG" - that framing was WRONG: it was a
compiled-out ASSERT blocked behind the missing statement, recoverable from source,
not a call-boundary argument-passing artifact. With the ASSERT recovered the
quantity is 5-vs-5 with matching sizes. QUANTITY match, SIZE match.

## Divergences

None. No QUANTITY divergence (every function's statement count matches), no SIZE
divergence (every per-statement +delta matches). No added/missing brace block, no
early-return-vs-wrapping-if mismatch, no member-init-vs-body-assignment mismatch.

## Reconciliation of stale states (already applied on this branch)

The state-sync the task calls for was already done in commit 51616bb2 on this
branch (before this verification). At HEAD:
- weapon_core_show_state_base.cpp: all four marked `// STATE[100%|DONE]`, carcasses
  stripped, no "LTCG"/PARTIAL rationale comments. Confirmed correct by this
  verification (clean 100% DONE over matching structure -> STATE line only, no carcass).
- docs/binary_matching/game_core/weapon_core_show_state_base.md: Outcome section
  reads "ctor 100% DONE, finalize 100% DONE, initialize 100% DONE,
  on_animation_end_impl 100% DONE"; the 75.13%/69.93% figures are retained only as
  the historical iteration trail, with the CORRECTION that the residual was a
  compiled-out ASSERT, not LTCG.
- docs/binary_matching/game_core/PROGRESS.md line 90: the ledger reads
  `STATE[100%|DONE] -> PR #125 (regressions: none)`.

This verifier confirms those syncs are CORRECT - each 100% is genuine over a
matching structure - so nothing is downgraded and no carcass is restored. This
report records the verification on the merge path.

## Bottom line

All four functions: STRUCTURE MATCH (quantity + size). PR #125 is a genuine
all-100%-DONE unit; structure does not block the merge.
