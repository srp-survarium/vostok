# Render shared-core-header audit

The render campaign no longer has an open shared-core-header ticket.  This file
keeps only the conclusions that prevent a later matcher from reviving obsolete
diagnoses.

## Closed findings

- `math_functions_inline.h`: closed by `fac29d0a6`.  The target-faithful plain
  inline declarations are present, and representative dependent functions have
  reached byte-exact.  Do not change `min`/`max` force-inlining from the old T1
  hypothesis.
- `math_color.h`: closed by `529335937`.  The retail channel order is restored;
  the `backend::clear_render_targets` family is byte-exact.  The old T2
  contradiction no longer describes the source.
- `threading_policies.h`: closed without a source change.  Keep the ASSERTs in
  `single_threading_policy::increment/decrement`; the current base and retail
  emitted implementations are byte-exact.  The old T3 claim that the ASSERTs
  must be removed was based on stale compilation evidence.
- Inline-header ownership: closed for the paired render surface.  The remaining
  target-only render translation units are documented linker-retention walls,
  not misplaced inline bodies.
- `buffer_string::operator=(value_type const*)`: the retail self-assignment
  guard is target-proven by the inlined pointer comparison and skip edge in the
  `render_cc_*::fill_macro` family.  The source now carries that guard.

Raw CodeView line-table entry counts are not a ticket source.  Use
`structure-diff`, named locals, declaration/layout evidence, and emitted
assembly; whitespace or source-line packing is never a matching fix.
