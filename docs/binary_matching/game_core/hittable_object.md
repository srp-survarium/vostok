# game_core::hittable_object - structure-verifier pass

Verifier-matcher pass (2026-06-07, chain/game_core-sv-item_object_batch). Diffed every
non-100% function with `--view structure-diff --condensed` after a clean rebuild. No logic
changes; markers synced to report.json.

### hittable_object (ctor)  (72.88% PARTIAL)
target 0x589f30 / base 0x4570b0, 0/0 stmts, 0 diffs. Empty body, init-list only. The
target emits an out-of-line constructor for the `hit_receiver` member sub-object while base
inlines it - a whole-program LTCG decision driven by the base class's declaration, not this
ctor's source. STRUCTURE MATCH; non-steerable. (Same class as the base-class-ctor inline
entry in assembly_patterns.md.)

### ~hittable_object  (69.59% PARTIAL)
target 0x589ee0 / base 0x457050, 4/4 stmts, 0 diffs. Two ASSERTs + 
`destroy_static_rigid_body(m_rigid_body)`. The `hit_receiver` member destructor is emitted
out-of-line in target, inlined in base. STRUCTURE MATCH; non-steerable.

### load  (marker 86.74% [stale] -> 98.14% PARTIAL)
target 0x589f80 / base 0x457110, 29/29 stmts, aligned 26, size-diffs 3, quantity-diffs 0.
SIZE on:
- `bt_collision_shape_ptr shape = create_compound_shape(...)` - the intrusive resource_ptr
  construct/assign.
- `shape->set_no_delete()`.
- `info.m_collisionShape = shape` - resource_ptr assign.
All three are whole-program inline-vs-call of the resource_ptr ops. STRUCTURE MATCH;
non-steerable.

## Verdict
All three non-100% functions are STRUCTURE MATCH. Residuals: inlined member ctor/dtor
(ctor/dtor) and resource_ptr inline-vs-call (load). None source-steerable.

(`get_transform` is marked STATE[MISSING] - a trivial forwarder, not in report's scored set;
left untouched, out of scope for this percentage-driven batch.)
