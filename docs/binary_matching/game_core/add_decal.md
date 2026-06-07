# bullet_manager::add_decal (0x591980)

~89.81% PARTIAL (marker was stale 94.92%). Structure MATCH (12/12 stmts), two SIZE.

## Residual (non-steerable wall)
SIZE at the functor alloc + at `m_functors.push`. Both trace to ONE cause:
- TARGET calls `bullet_functor_mt_allocator::malloc_impl` out-of-line (rva 0xae5c0).
- BASE inlines malloc_impl to `intrusive_mpmc_stack::try_pop` (no standalone
  malloc_impl symbol in base index), so base has an extra `push 58h` and the frame is
  target `sub esp,0ACh` vs base `sub esp,0A8h` (4 bytes). The `m_functors.push` SIZE
  is purely the cascading slot rename.
Whole-program LTCG inline-vs-call. Source correct. add_decal's bind is just
`boost::bind( &add_decal_impl, this, functor )` (pointer arg, no cref needed - unlike
play_particle/update_tracer which bound float3s).

## Commands
pdb_fetch --function add_decal --rva 0x591980 --view structure-diff/diff/target/base
pdb_rich_query --function malloc_impl --list
