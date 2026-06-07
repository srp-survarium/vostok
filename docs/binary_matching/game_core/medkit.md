# game_core::medkit - structure-verifier pass

Verifier-matcher pass (2026-06-07, chain/game_core-sv-item_object_batch). Refreshed
report.json + rich indexes via `nix develop -c python3 scripts/rebuild.py`, then diffed
every non-100% function with `--view structure-diff --condensed` (and `--view diff` to
name causes). Stale/UNCHECKED markers synced to report.json. No logic changes - all
residuals are non-steerable whole-program LTCG inline/arg-passing walls.

## Per-function findings

### ~medkit  (marker UNCHECKED -> 67.06% PARTIAL)
target 0x750180 / base 0x44bc60, 3/3 stmts, aligned 0, size-diffs 3, quantity-diffs 0.
All three `VOSTOK_DELETE_IMPL( g_allocator, m_* )` statements align; each inlines the
array element-destructor + free to a different byte size (whole-program inline form).
STRUCTURE MATCH; non-steerable.

### load  (marker 91.02% DONE [stale] -> 96.66% PARTIAL)
target 0x74fc20 / base 0x44b480, 48/48 stmts, aligned 42, size-diffs 6, quantity-diffs 0.
SIZE on the `for` loop counter slot and on 4x `strings::copy( ..., 0x10, cfg[i][...] )`:
the config-value -> pcstr conversion arg is inlined to a different size. STRUCTURE MATCH;
non-steerable. (sushi@NOTE: the allocations look off but reproduce the target.)

### set_active  (marker UNCHECKED -> 47.04% PARTIAL)
target 0x74f730 / base 0x44b0d0, target 23 / base 25 stmts, aligned 9, size-diffs 10,
quantity-diffs 10. `--view diff`: frame 0xC0 (base) vs 0x84 (target). The ONLY base /
ONLY target rows are multi-line-statement attribution boundaries of the inlined
`register_for_update( ..., boost::bind( &medkit::active_tick, this, _1 ), ... )` and the
`m_inventory->holder().scheduler()/.damage_model()` resource_ptr accessor calls. Base
inlines these one layer deeper than the target, which enlarges the frame and shifts every
[ebp-N] slot. STRUCTURE MATCH (same control shape); non-steerable whole-program LTCG.

### action  (marker 96.17% PARTIAL -> 96.17% PARTIAL, format only)
target 0x74f970 / base 0x44b420, 10/10 stmts, aligned 9, size-diffs 1, quantity-diffs 0.
Sole SIZE on `set_amount( amount() - 1 )`: inline-vs-call of set_amount. STRUCTURE MATCH.

### active_tick  (marker UNCHECKED -> 53.50% PARTIAL)
target 0x74fa50 / base 0x44ba50, target 39 / base 36 stmts, aligned 29, size-diffs 6,
quantity-diffs 5. 2 SIZE on `math::min(...)` (template inlined to a tighter cmov form in
target, 0x14 vs 0x2c bytes); the rest + the ONLY/EMPTY rows are the inlined
`apply_med_kit(...)` / `holder()` resource_ptr accessor multi-line attribution. Base
inlines deeper, shifting slots. STRUCTURE MATCH; non-steerable.

### remove_affects  (marker UNCHECKED -> 60.46% PARTIAL)
target 0x74f9d0 / base 0x44b9b0, 4/4 stmts, aligned 3, size-diffs 1, quantity-diffs 0.
`--view diff`: frame 0x20 (base) vs 0x14 (target). Sole SIZE on
`m_inventory->holder().damage_model()->cancel_affect(...)`: base inlines `damage_model()`
(returns a resource_ptr by value) with the temp-construct/test/destroy materialization
(the `mov byte[ebp-0Dh],0; lea; call` sequence) while the target out-of-lines it.
STRUCTURE MATCH; non-steerable (same class as the intrusive_ptr-by-value getter entry in
assembly_patterns.md).

### find_damage_protection  (marker 99.90% DONE -> 99.90% DONE, format only)
target 0x74f660 / base 0x44b000, 8/8 stmts, aligned 8, size-diffs 0, quantity-diffs 0.
Structure byte-clean; sole residual is a 4-byte frame-size / slot-allocation difference.
STRUCTURE MATCH; non-steerable.

## Verdict
Every medkit non-100% function is a STRUCTURE MATCH. No source-steerable defect (no
missing ASSERT, brace, early-return, access-specifier, or switch-shape issue). All
residuals are whole-program LTCG: boost::bind/register_for_update inline depth, resource_ptr
accessor inline-vs-call (returns-by-value temp), math::min template inline, strings::copy /
set_amount / VOSTOK_DELETE_IMPL inline-vs-call, and frame-slot allocation.
