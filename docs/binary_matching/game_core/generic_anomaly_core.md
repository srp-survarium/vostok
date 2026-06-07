# game_core::generic_anomaly_core - structure-verifier pass

Verifier-matcher pass (2026-06-07, chain/game_core-sv-item_object_batch). Diffed every
non-100% / SKIPPED / BLOCKED function across generic_anomaly_core.cpp and
generic_anomaly_core_construct.cpp with `--view structure-diff --condensed` (+ `--view diff`)
after a clean rebuild. No logic changes; markers synced to report.json and obsolete
SKIPPED/BLOCKED tags retired where the function is in fact reconstructed and matching.

## generic_anomaly_core.cpp

### generic_anomaly_core (ctor)  (marker SKIPPED -> 77.00% PARTIAL)
target 0x58d8d0 / base 0x456160, 0/0 stmts, 0 diffs. Init-list only, no body statements.
Residual is the compiler-emitted base-subobject ctor calls + vtable stores of the
multiply-inherited (NO_VTABLE + virtuals) hierarchy - decided by the base-class
declarations, not this ctor's source. STRUCTURE MATCH; non-steerable.

### ~generic_anomaly_core (dtor)  (marker SKIPPED -> 99.91% DONE)
target 0x58d880 / base 0x456110, 0/0 stmts, 0 diffs. `--view diff` (90.79% objdiff):
sole residual is two adjacent member sub-object destructor calls emitted in SWAPPED order -
target destroys the members at this+9 then this+8; base at +8 then +9. That is a
member-layout / destruction-order quirk; correcting it would need a header member reorder
(wide blast radius across the whole struct + other functions). At 99.91% it is out of scope
for this batch. STRUCTURE MATCH.

### activate  (marker 79.69% [stale] -> 83.49% PARTIAL)
target 0x58dc10 / base 0x4564b0, 14/14 stmts, aligned 5, size-diffs 9, quantity-diffs 0.
The `m_scheduler->register_on_frame( ..., boost::bind( &tick, this, _1, _2 ), true )` is
inlined one layer deeper in the target (0xce vs 0x10c bytes), which enlarges the frame and
cascades SIZE onto every trailing scalar store (the bool flags + spawn_artefacts). STRUCTURE
MATCH; non-steerable whole-program LTCG (boost::bind inline depth).

### deactivate  (99.68% DONE)
target 0x58d7c0 / base 0x456050, 11/11 stmts, aligned 11, 0 size/quantity diffs. Byte-clean
structure; sole residual a frame-size / slot-allocation difference. STRUCTURE MATCH.

## generic_anomaly_core_construct.cpp

### load  (marker 96.31% [stale] -> 96.07% PARTIAL)
target 0x58c9c0 / base 0x455330, 54/54 stmts, aligned 51, size-diffs 3, quantity-diffs 0.
SIZE on `m_artefact_containers.resize(...)` (vectora::resize), `math::min(...)` (template
inline), and `VOSTOK_NEW_IMPL( g_allocator, anomaly_state )( this )` (allocation inline).
STRUCTURE MATCH; non-steerable.

### state_prio  (marker BLOCKED -> 99.63% DONE)
target 0x58c630 / base 0x454fa0, 5/5 stmts, aligned 5, 0 size/quantity diffs. Fully
reconstructed and structurally clean; the old BLOCKED note ("update Ghidra script") is
obsolete. Sole residual is a frame-size / slot-allocation difference. STRUCTURE MATCH.

### resolve_links  (marker 99.00% -> 100% DONE)
target 0x58c6b0 / base 0x455020, 24/24 stmts, aligned 24, 0 diffs. 100.0 in report.json;
stripped the carcass and reduced marker to bare STATE[100%|DONE].

## Verdict
All non-100% functions are STRUCTURE MATCH. Residuals: multiple-inheritance vtable/base-init
codegen (ctor), one swapped member-dtor pair (dtor), boost::bind inline depth (activate),
vectora/math::min/VOSTOK_NEW inline (load), frame-slot (deactivate / state_prio). None
source-steerable in this batch. resolve_links reached 100%.
