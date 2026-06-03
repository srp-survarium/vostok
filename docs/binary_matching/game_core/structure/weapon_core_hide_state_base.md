# Structure verification: weapon_core_hide_state_base {ctor, initialize, finalize, on_animation_end_impl}

Unit: sources/vostok/game_core/sources/weapon_core_hide_state_base.cpp (PR #126)

Verifier scope: SOURCE STRUCTURE only (statement QUANTITY + per-statement SIZE),
target vs base, independent of the byte/fuzzy %. No rebuild, no logic change.

This is the sibling of PR #125 (weapon_core_show_state_base). initialize and
on_animation_end_impl recovered from PARTIAL to 100% via a compiled-out ASSERT
recovery (the target's `call empty_stub`) plus the inherited
animation_playback_state::reset() fix - so this verification is EXTRA rigorous
that the 100% sits over the CORRECT structure (a big PARTIAL->100 jump is exactly
where a wrong-structure 100% could hide).

## Verdict (combined)

ALL FOUR: STRUCTURE MATCH. Statement counts and per-statement sizes are identical
target-vs-base for every function. The four are genuine 100% over the correct
structure - nothing to flag, nothing to downgrade, no carcass to restore.

| function               | target rva | base rva | target stmts | base stmts | bytes | report.json % |
|------------------------|------------|----------|--------------|------------|-------|---------------|
| ctor                   | 0x6ed280   | 0x44afd0 | 3            | 3          | 0x6b  | 100.0         |
| initialize             | 0x6ed2f0   | 0x44afa0 | 5            | 5          | 0x2f  | 100.0         |
| finalize               | 0x6ed250   | 0x44af70 | 4            | 4          | 0x21  | 100.0         |
| on_animation_end_impl  | 0x6ed220   | 0x44af40 | 5            | 5          | 0x2d  | 100.0         |

report.json (binaries/objdiff/report.json, built locally at HEAD; read directly,
no rebuild) carries fuzzy_match_percent 100.0 for each of the four mangled
symbols, confirmed individually:
- `??0weapon_core_hide_state_base@survarium@@IAE@AAVweapon_core@1@AA_N@Z` size 107 -> 100.0
- `?initialize@weapon_core_hide_state_base@survarium@@MAEXXZ` size 47 -> 100.0
- `?finalize@weapon_core_hide_state_base@survarium@@MAEXXZ` size 33 -> 100.0
- `?on_animation_end_impl@weapon_core_hide_state_base@survarium@@EAEXAA_N@Z` size 45 -> 100.0

(Sizes 107/47/33/45 == 0x6b/0x2f/0x21/0x2d, byte-identical to the show sibling.)

## The four skeletons (target vs base, statement by statement)

Format: `offset  <size>  statement`. Target prints `Lnn` source-line tags; base
prints the rendered source statement (same skeleton, source form). The Lnn tags
line up 1:1 with the .cpp source lines.

### ctor  (3 statements, 0x6b bytes) - MATCH
```
        TARGET                          BASE
0x00  <0x55>  L20                0x00  <0x55>  {   (decl + member-init list)
0x55  <0x0d>  L21                0x55  <0x0d>  m_body_part_mask_for_user = animation::body_part_whole_body_but_hands;
0x62  <0x09>  L22                0x62  <0x09>  }
```
Sizes identical (0x55, 0x0d, 0x09). L20 = the ctor decl line, to which the
member-init list `weapon_core_animation_end_aware_state( weapon, true ),
m_is_shown( is_shown )` is attributed (collapses into the one 0x55-byte opener:
base-ctor delegation + compiler-emitted vtable stores + the m_is_shown ref store).
L21 = the body assignment that OVERWRITES the inherited m_body_part_mask_for_user
(-1 -> -3). L22 = scope exit. The init-list-vs-body split is correct: the one body
assignment is its own statement, everything else is on the decl line. QUANTITY
match, SIZE match. (Identical shape to the show ctor.)

### initialize  (5 statements, 0x2f bytes) - MATCH (a PARTIAL->100 jumper)
```
        TARGET                          BASE
0x00  <0x09>  L25                0x00  <0x09>  {
0x09  <0x08>  L26                0x09  <0x08>  weapon_core_animation_end_aware_state::initialize( );
0x11  <0x0c>  L27                0x11  <0x0c>  ASSERT( UNKNOWN_EXPRESSION );   <-- recovered ASSERT
0x1d  <0x0e>  L29                0x1d  <0x0e>  m_weapon.instant_toggle_start( );
0x2b  <0x04>  L30                0x2b  <0x04>  }
```
Sizes identical (0x09, 0x08, 0x0c, 0x0e, 0x04). The load-bearing statement is L27
at offset 0x11, size 0x0c: that is exactly the compiled-out-ASSERT sequence
(`mov byte[ebp-1],0; lea eax,[ebp-1]; call empty_stub`, ~0x0c bytes). The ASSERT
recovery genuinely emits this statement - it is present on BOTH sides with a
byte-exact 0x0c delta. This is NOT a high-%-over-wrong-structure fluke: the
statement quantity is 5-vs-5 and the ASSERT slot has the right size. The earlier
PARTIAL was the pre-ASSERT base (missing that one statement); recovering the
ASSERT (a real compiled-out assert, NOT "LTCG") closed it. QUANTITY match, SIZE
match.

### finalize  (4 statements, 0x21 bytes) - MATCH
```
        TARGET                          BASE
0x00  <0x07>  L33                0x00  <0x07>  {
0x07  <0x08>  L34                0x07  <0x08>  weapon_core_animation_end_aware_state::finalize( );
0x0f  <0x0e>  L36                0x0f  <0x0e>  m_weapon.instant_toggle_end( );
0x1d  <0x04>  L37                0x1d  <0x04>  }
```
Sizes identical (0x07, 0x08, 0x0e, 0x04). No ASSERT here (the empty_stub sequence
appears only in initialize/on_animation_end_impl) - finalize is correctly a 4-statement
body with no extra slot. QUANTITY match, SIZE match.

### on_animation_end_impl  (5 statements, 0x2d bytes) - MATCH (a PARTIAL->100 jumper)
```
        TARGET                          BASE
0x00  <0x09>  L40                0x00  <0x09>  {
0x09  <0x0c>  L41                0x09  <0x0c>  ASSERT( UNKNOWN_EXPRESSION );   <-- recovered ASSERT
0x15  <0x0c>  L42                0x15  <0x0c>  m_is_shown = false;
0x21  <0x06>  L43                0x21  <0x06>  animation_player_tick_result = true;
0x27  <0x06>  L44                0x27  <0x06>  }
```
Sizes identical (0x09, 0x0c, 0x0c, 0x06, 0x06). The load-bearing statement is L41
at offset 0x09, size 0x0c: the same compiled-out-ASSERT sequence, here as the FIRST
body statement (before the two bool stores). Present on both sides, byte-exact 0x0c.
The earlier PARTIAL framed this as "LTCG" - that framing was WRONG: it was a
compiled-out ASSERT blocked behind the missing statement, recoverable from source,
not a call-boundary argument-passing artifact. With the ASSERT recovered the
quantity is 5-vs-5 with matching sizes. Note the hide body assigns `m_is_shown =
false` (the show sibling assigns `true`); the store size is the same 0x0c either way,
so it does not perturb structure - and this is the correct hide value. QUANTITY
match, SIZE match.

## Divergences

None. No QUANTITY divergence (every function's statement count matches), no SIZE
divergence (every per-statement +delta matches). No added/missing brace block, no
early-return-vs-wrapping-if mismatch, no member-init-vs-body-assignment mismatch,
no spurious `[n]` lexical block on either side.

## Cross-check against the show sibling (PR #125)

The hide unit is byte-for-byte the same shape as the verified show unit
(weapon_core_show_state_base.md): identical statement counts (3/5/4/5) and identical
per-statement sizes for every function. The only source-level differences are the
non-virtual member targets (instant_toggle_start/end, same as show) and the
on_animation_end_impl bool value (`false` for hide vs `true` for show) - neither
changes structure. This corroborates the 100%.

## State sync

No state change required. At HEAD on this branch:
- weapon_core_hide_state_base.cpp: all four marked `// STATE[100%|DONE]`, carcasses
  stripped, ASSERT statements present with a one-line "compiled-out ASSERT" note.
  Confirmed correct by this verification (clean 100% DONE over matching structure ->
  STATE line only, no carcass).
- docs/binary_matching/game_core/weapon_core_hide_state_base.md: records all four at
  100% DONE (with the pre-ASSERT PARTIAL figures kept as the historical iteration
  trail + the CORRECTION that the residual was a compiled-out ASSERT, not LTCG).
- docs/binary_matching/game_core/PROGRESS.md line 97: the ledger reads
  `weapon_core_hide_state_base::{ctor,initialize,finalize,on_animation_end_impl} ->
  STATE[100%|DONE] -> PR #126 (regressions: none)`.

This verifier confirms those are CORRECT - each 100% is genuine over a matching
structure - so nothing is downgraded and no carcass is restored.

## Bottom line

All four functions: STRUCTURE MATCH (quantity + size). PR #126 is a genuine
all-100%-DONE unit; structure does not block the merge.
