# structure: character_dispersion_calculator::get_target_koef

## Verdict
STRUCTURE MISMATCH (size) - target 21 / base 21 statements (0 quantity-diffs),
2 SIZE-diffs, both tracing to ONE source cause: the `switch` bounds check.

`float survarium::character_dispersion_calculator::get_target_koef(const survarium::weapon_user_state_enum, const bool, const bool) const`
target rva 0x585ee0 | base rva 0x4543c0
report.json fuzzy_match_percent: 95.74% (INPROGRESS). The % is high, the SHAPE is
right (statement counts and block structure align); the residual is a codegen-size
divergence on two statements, both from the same dispatch fix.

## Condensed structure-diff (--view structure-diff --condensed)
```
target: 0x585ee0            base: 0x4543c0
; ... get_target_koef(...) const ; target 21 stmts / base 21 stmts
.. same ..
0x015 <0x10> | 0x015 <0x1a> | switch ( character_state )   SIZE
.. same ..
0x103 <0x4>  | 0x10d <0x2>  | return 1.0f;                 SIZE
.. same ..
; aligned 19, size-diffs 2, quantity-diffs 0
```

## Divergences
1. SIZE - `switch ( character_state )`: base 0x1a vs target 0x10 (+0xa). The base's
   dispatch emits a range check `cmp dword ptr [ebp-0Ch], 3; ja default` ahead of the
   `jmp dword ptr [edx*4+table]`; the target has NO check (contiguous [0..4] table).
   Confirmed via `--view diff`: the `cmp .. ,3` / `ja` rows are `ONLY base`.
2. SIZE - `return 1.0f;` (the default/`fld1` case): base 0x2 vs target 0x4. On target
   the default body needs a `jmp short` to the epilogue (the table is contiguous so the
   default sits mid-body); on base it falls straight through. This is a knock-on of (1):
   the same table-layout difference.

Both are SIZE only - the statement skeletons line up 1:1 (19 aligned + the 2 SIZE rows
= 21 each). No quantity divergence, no order divergence.

## Audit - what the % hid AND what the prior notes got wrong
- The 95.74% is honest about shape: 0 quantity-diffs means the per-case bodies, the
  recovered `ASSERT( UNKNOWN_EXPRESSION_T( m_params ) )`, and the control structure all
  match. The only open item is the dispatch-table SIZE.
- CORRECTION (audit caught): the STATE line and the older per-function .md (lines 18-22)
  claimed a second fix - "add `{ }` braces to type_stand/type_crouch because the carcass
  shows `+0x002` jmp-`}` at 0x5d / 0xc0". The structure-diff shows 0 quantity-diffs, so
  there is NO missing brace-block: those `+0x002` rows are the case FALL-THROUGH thunks
  (the `jmp .crouch` / `jmp .sprint` between cases) and they are present on BOTH sides.
  Adding braces is unnecessary and would not change the alignment. The per-function .md
  already self-contradicted this on line 111 ("Cases stay brace-less"); the structure-diff
  settles it: brace-less is correct.

## Concrete source fix (needs rebuild to confirm - NOT done here)
ONE change, resolving both SIZE-diffs: cover all enum values and make the default
unreachable so MSVC emits the contiguous table with no bounds check:
- add `case type_preview: return 1.0f;`
- change `default: return 1.0f;` -> `default: NODEFAULT();` (`__assume(0)`,
  `sources/vostok/debug_macros.h`).
No brace changes. Rebuild + re-diff to confirm.

## State / labeling
Already correctly labeled `// STATE[95.74%|INPROGRESS]`. No downgrade needed (it was
never a banked DONE). This pass: embedded the condensed structure-diff in place of the
one-sided `// FUNCTION BODY` carcass, and corrected the STATE-line/report brace claim.
