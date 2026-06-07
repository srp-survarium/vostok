# Structure audit: character_dispersion_calculator::get_broken_hands_penalty

Verdict (RESOLVED): STRUCTURE MATCH (shape ok) - 10/10 aligned, 0 size-diffs, 0
real quantity-diffs (the sole remaining row is an EMPTY blank-line gap).

- target rva 0x585e50, base rva 0x44d700
- signature: `private: float __thiscall survarium::character_dispersion_calculator::get_broken_hands_penalty(unsigned char, bool) const`
- report.json fuzzy_match_percent: 100.0% (was 93.33% PARTIAL).

## How it was fixed (the verdict's suggested fold was WRONG)
The embed verdict said "fold the trailing `return 1.0f;` into case 0 so the switch
falls through to the shared epilogue fld1". Tried that two ways and both REGRESSED:
- `default: case 0: return 1.0f;` (case 0 last/merged) -> 95.29%, but DROPPED the
  target's explicit `cmp 0` (base switch shrank to 0x12 vs target 0x18) and the
  reorder shifted the jump table.
The target asm keeps THREE explicit comparisons (`cmp 0; cmp 1; cmp 2`) and the
no-match path falls into case 0's fld1 with NO separate trailing return. The form
that reproduces this exactly:
```cpp
switch ( broken_hands_count )
{
case 0:  return 1.0f;
case 1:  return ... ;
case 2:  return ... ;
default: NODEFAULT();          // __assume(0)
}
```
`default: NODEFAULT()` marks the no-match path unreachable, so MSVC emits no
spurious trailing `fld1` yet still keeps the explicit `cmp 0` for case 0 (whose
fld1 doubles as the shared exit). Result: 100%, 0 size-diffs.

## Condensed structure-diff (target 10 / base 13 stmts)

```
.. same ..
--          | <0>         |    EMPTY only base
0x015 <0x18> | 0x015 <0x1a> | switch ( broken_hands_count )                                                                                                  SIZE
.. same ..
0x05c <0x2b> | 0x05e <0x2d> | return using_double_handed_weapon ? m_params->injury_penalty_for_double_handed : m_params->injury_penalty_for_one_handed;     SIZE
--          | <0>         |    EMPTY only base
--          | 0x08b <0x2> | return 1.0f;                                                                                                                     ONLY base
.. same ..
; aligned 8, size-diffs 2, quantity-diffs 3
```

## Skeletons side by side

Target (10 stmts):
```
ASSERT( UNKNOWN_EXPRESSION_T( m_params ) );
switch ( broken_hands_count )                                  ; 0x18 bytes
case 0 / default: return 1.0f;     (fld1 at .1, shared)
case 1: return double_handed ? injury_double : 1.0f;
case 2: return double_handed ? injury_double : injury_one;     ; 0x2b bytes
(switch falls straight through to the epilogue - NO trailing return)
```

Base (13 stmts):
```
ASSERT( UNKNOWN_EXPRESSION_T( m_params ) );
switch ( broken_hands_count )                                  ; 0x1a bytes
case 0: return 1.0f;
case 1: return double_handed ? injury_double : 1.0f;
case 2: return double_handed ? injury_double : injury_one;     ; 0x2d bytes
return 1.0f;                                                    ; EXTRA fld1 at 0x8b
```

## Divergences

1. QUANTITY (ONLY base): base has a trailing `return 1.0f;` after the switch
   (an extra `fld1` at base 0x8b). The target has NO such statement - every case
   jump (`.1`, `.4`, `.7`) lands directly on the epilogue label `.8`
   (`mov esp,ebp / pop ebp / ret 8`). In the target, case 0's body IS the
   implicit-default fld1 at `.1`: after `cmp 2 / je .5` the no-match path falls
   through into `.1` (the same `fld1` that `cmp 0 / je .1` targets). Base instead
   emits a separate `fld1` for the trailing source-level `return 1.0f;`, and its
   case jumps go to that extra label.

2. SIZE: `switch ( broken_hands_count )` is 0x18 (target) vs 0x1a (base) - the
   jump-table / fall-through differs because the base has the extra trailing
   block to jump over.

3. SIZE: `case 2` return is 0x2b (target) vs 0x2d (base) - same root cause: the
   trailing `jmp` distance differs once the extra block exists.

All three divergences share ONE root cause: the spurious trailing `return 1.0f;`.

## Source fix (matcher job - needs a rebuild to confirm)

Remove the standalone trailing `return 1.0f;` and let case 0's body double as the
implicit default fall-through, so the no-match path and case 0 share a single
`fld1` (as the target does). Concretely the switch must be written so the
fall-through reaches case 0's `return 1.0f;` with no separate post-switch return
emitted (e.g. order case 0 last as the shared exit, or merge it with `default`
while preserving the explicit `cmp 0`). C++ all-paths-return must still be
satisfied without producing a second `fld1`.

This is a genuine restructure (not a byte tweak) and is left for a matcher; this
audit only flags it, embeds the diff, and downgrades STATE 93.33%|PARTIAL ->
INPROGRESS with the fix note. No logic changed, no rebuild run.
