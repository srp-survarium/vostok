# redundant_bullet_predicate (bullet_manager.cpp)

## ctor (inlined)
`redundant_bullet_predicate( bullet_manager& )` - no standalone symbol in the target
index; inlined into tick's `std::remove_if`. Marked STATE[INLINED]. (The old marker
claimed 90.15% DONE "LTCG for float3"; there is no separately-scored body.)

## operator()(bullet*) (0xae610)
~87.98% PARTIAL. Structure MATCH (5/5 stmts), one SIZE.

### Residual (non-steerable wall)
SIZE on `if ( bullet->get_start_velocity( ) != float3( 0, 0, 0 ) )`.
- TARGET binary has NO float3 `operator!=` symbol (only `operator==` @0x14b90). It
  inlines `a != b` into `!( operator==(a,b) )`: `call vostok::math::operator==` then a
  `je`/`jne` polarity flip vs base.
- BASE keeps a standalone float3 `operator!=` (@0x6f250 in math_float3_inline.h) and
  emits `call vostok::math::operator!=` directly (no negate; opposite branch polarity,
  plus a `mov ecx,eax` for the temp's this).
The float3(0,0,0) temp build (`fldz; fstp` x3) is byte-identical. This is the
whole-program LTCG inline-vs-call class (operator!= folded to !(==) at the call site).
Source `a != b` correct.

### Commands
pdb_fetch --function "redundant_bullet_predicate::operator()" --view structure-diff/diff/target/base
pdb_rich_query --function "operator!=" --list | grep float3  (target: none; base: 0x6f250)
pdb_rich_query --function "operator==" --list | grep float3  (target: 0x14b90)
