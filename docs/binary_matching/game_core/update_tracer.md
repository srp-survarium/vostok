# bullet_manager::update_tracer (0x5914a0)

58.56% -> 89.38% PARTIAL.

## Fix applied
Same boost::bind by-value-vs-reference_wrapper issue as play_particle. Target bind:
`list5<value<bullet_manager*>, value<unsigned short>, reference_wrapper<float3 const>,
reference_wrapper<float3 const>, value<float>>` - so m_tracer_idx (u16) and length
(float) stay by value, but position/direction are cref.

Changed:
```
functor->position  -> boost::cref( functor->position )
functor->direction -> boost::cref( functor->direction )
```
The bind region now matches.

## Residual (non-steerable wall)
- functor alloc SIZE (target 0x4d vs base 0x4b) + `EMPTY only target`, plus the
  whole frame is target `sub esp,0C0h` vs base `sub esp,0BCh` (4 bytes), cascading
  `[ebp-0ACh]` vs `[ebp-0A8h]` slot renames everywhere. Cause: same malloc_impl
  inline-vs-call as add_decal/play_particle (target keeps malloc_impl out-of-line
  @0xae5c0; base inlines).
- The `);` bind SIZE is now just the cascading slot shift, structurally identical.

## Commands
pdb_fetch --function update_tracer --rva 0x5914a0 --view structure-diff/diff/target
