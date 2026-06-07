# oxygen_tank.cpp - structure-verification trail

Verifier-matcher pass over the four flagged functions. RVAs are TARGET rvas from
`binaries/rich/target/index.jsonl` (note: the old carcass `<0x6f...>` addresses are
BASE-build addresses, ~0x10000 off the target).

report.json fuzzy_match_percent (synced into STATE this pass):
- set_active      48.62% (was labelled 50.42%)
- active_tick     75.86% (was labelled 92.30%)
- load            97.53% (was labelled 91.94%)
- find_influence  99.90% (matched)

Diagnosis command per function:
```
nix develop -c pdb_fetch --target-index binaries/rich/target/index.jsonl \
  --base-index binaries/rich/base/index.jsonl --rva <rva> \
  --view structure-diff --condensed \
  --objdiff-base-dir binaries/objdiff/base --objdiff-target-dir binaries/objdiff/target
```

## set_active (target 0x6e9dc0) - STRUCTURE MATCH, inline-vs-call wall

structure-diff: target 17 / base 20 stmts; aligned 12, size-diffs 5, quantity-diffs 3.
The 3 quantity rows are `EMPTY only base` (collapsed blank source-line gaps, cosmetic).
The 5 SIZE rows are all the same root cause.

Root cause (confirmed with `--view target` / `--view base`):
- TARGET emits and CALLS these out-of-line:
  - `survarium::inventory::holder()`            rva 0x86b70
  - `survarium::inventory_holder::scheduler()`  rva 0x82cc0
  - `survarium::scheduler::register_for_update` rva 0x82da0
- Our BASE INLINES all three at the call site:
  - `holder()` -> `[ecx+10Ch]`-ish field read, `scheduler()` -> `[..+158h]`/`[..+4]`
    materialized into stack temps (`[ebp-78h]`, `[ebp-7Ch]`, `[ebp-84h]`, `[ebp-88h]`).
  - `register_for_update` is `inline` in `scheduler_inline.h`; base inlines its whole
    body at the call site (the `or 80000000h` / `and 7FFFFFFFh` type+delta bit-twiddling
    that the target hides inside the out-of-line `register_for_update`).
- `pdb_rich_query --index binaries/rich/base/...` finds NO standalone copy of
  `inventory::holder`, `inventory_holder::scheduler`, or `register_for_update` in base -
  they exist only inlined. The target keeps standalone copies AND calls them here.

So this is purely the MSVC/LTCG inliner's inline-vs-call decision on trivial `inline`
accessors (`return *m_holder;`, `return m_scheduler;`) plus the `inline`
`register_for_update`. There is NO source-shape lever: the statement shape, the member
spelling `m_inventory->holder( ).scheduler( ).register_for_update( ... )`, the
`if(m_active)/else` and the `for` loop all match the target. De-inlining the accessors
in the header to force out-of-line calls would break every OTHER matched caller (and is
not the engine's source shape). medkit::set_active (target 0x74f730) has the IDENTICAL
divergence on the same call - it is not a usable reference, it has the same wall.

VERDICT: STRUCTURE MATCH (shape ok) - inline-vs-call inliner decision, non-steerable.
A real fix would require the scheduler unit to land `register_for_update` /
`holder` / `scheduler` as out-of-line (currently `register_for_update`/`unregister` are
STATE[BLOCKED] in scheduler_inline.h), which is out of scope for this file.

## active_tick (target 0x6ea0d0) - STRUCTURE MATCH, two walls

structure-diff: target 7 / base 7 stmts; size-diffs 2, quantity-diffs 0. Shape matches.
- `m_amount_ms -= math::min( m_amount_ms, frame_time_ms );` (SIZE 0x25 vs 0x3d):
  TARGET INLINES `math::min(u32,u32)` to the branchless `cmp/sbb/neg/and/add` sequence;
  our BASE out-of-line-CALLS it (`call math::min`). The target also keeps a standalone
  `math::min(u32,u32)` at rva 0x3fbb0. Inverse direction from set_active, same class of
  inliner decision, non-steerable.
- `LOG_INFO( "amount is: %dms", m_amount_ms );` (SIZE 0x81 vs 0x82): the usual
  logging-macro LTCG (extra string-temp destructor / register choice). Non-steerable.

Byte-neutral cleanup this pass: tightened the ASSERT from bare `UNKNOWN_EXPRESSION` to
`UNKNOWN_EXPRESSION_T( m_active )` (the tick only runs while the tank is active). The
`_T` guess is discarded by the macro, so it stays byte-identical - confirmed by rebuild.

VERDICT: STRUCTURE MATCH (shape ok) - math::min inline-vs-call + logging-macro LTCG.

## load (target 0x6ea1c0) - STRUCTURE MATCH, LTCG temp-spill

structure-diff: target 18 / base 18 stmts; size-diffs 2, quantity-diffs 0.
- Both `strings::copy( infl.X, 0x10, influences[i]["..."] );` lines (SIZE 0x38 vs 0x2c,
  base SMALLER): the TARGET spills the `influences[i][...]` binary_config_value temp
  into a stack slot before the copy (extra `mov [ebp-A0h],eax` / `mov eax,[ebp-A0h]`);
  base keeps it in a register. LTCG temp-materialization at the call boundary,
  non-steerable. Everything else is byte-identical.

VERDICT: STRUCTURE MATCH (shape ok) - binary_config_value temp-spill, LTCG.

## find_influence (target 0x6e9cf0) - STRUCTURE MATCH (effectively clean)

structure-diff: target 8 / base 9 stmts; size-diffs 0, quantity-diffs 1. Every statement
is byte-for-byte aligned (size-diffs 0). The single quantity row is `EMPTY only base` -
a collapsed blank source line between `return &infl;` (and its `}`) and `return NULL;`.
Cosmetic gap only; the control structure (for / inner if / two returns) matches exactly.

VERDICT: STRUCTURE MATCH - byte-identical, lone diff is a cosmetic blank-line gap.
