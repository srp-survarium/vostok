# Structure verification: survarium::ik_processor {ctor, activate}

PR #142, branch match/game_core-ik_processor-ctor-activate
Source: sources/vostok/game_core/sources/ik_processor.cpp

## 1. survarium::ik_processor::ik_processor()

Mangled: ??0ik_processor@survarium@@IAE@XZ (41 / 0x29 bytes)
report.json fuzzy_match_percent = 100.0

Verdict: STRUCTURE MATCH

### Skeletons

```
TARGET (rva 0x57eba0): ; 0 statements, 0x29 bytes
BASE   (rva 0x44f580): ; 0 statements, 0x29 bytes
```

Both sides report 0 statements at 0x29 bytes. A member-initializer-list ctor
attributes its inits to the ctor decl line and emits no separate body
statements, which is exactly the shape here:

```
ik_processor::ik_processor( )
    : m_skeleton        ( NULL )
    , m_last_time_in_ms ( 0 )
{
}
```

The 0x29 body is the two member stores plus the ICF-folded out-of-line
core::noncopyable base ctor (the `call ...finalize_impl` @0x0a folds to the
rva 0x3f210 fold target per assembly_patterns.md); none of this introduces a
distinct source statement on either side. No QUANTITY or SIZE divergence.

## 2. survarium::ik_processor::activate(animation::skeleton const&)

Mangled: ?activate@ik_processor@survarium@@IAEXABVskeleton@animation@vostok@@@Z (21 / 0x15 bytes)
report.json fuzzy_match_percent = 100.0

Verdict: STRUCTURE MATCH

### Skeletons (side by side)

```
offset  size   TARGET (rva 0x57eb80)   BASE (rva 0x44f560)
0x00    0x7    L20                     {
0x07    0x8    L21                     m_skeleton = &skeleton;
0x0f    0x6    L22                     }
```

Both sides: 3 statements, 0x15 bytes. Offsets and sizes are identical
statement for statement. Target lines L20/L21/L22 map cleanly onto the base's
brace-open / single body assignment / brace-close. The function is braced
(target carries three rows, matching the base's `{` and `}` scope-exit rows),
and the base source braces it accordingly. No QUANTITY or SIZE divergence.

## Summary

Both functions are genuine clean matches: the 100.0% fuzzy_match_percent is
backed by identical statement quantity and per-statement size on both sides.
This is NOT a high-percent-over-wrong-structure case. No downgrade, no source
fix required. STATE[100%|DONE] tags are accurate.
