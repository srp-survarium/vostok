# bullet_manager::play_particle (0x5916e0)

67.87% -> 98.81% PARTIAL.

## Fix applied
The boost::bind bound its float3 / resource arguments BY VALUE; the target binds
them via boost::reference_wrapper. Target disasm shows `boost::addressof<...>` calls
(boost::cref) and the bind type
`list5<value<bullet_manager*>, reference_wrapper<resource_ptr const>,
reference_wrapper<float3 const>, reference_wrapper<float3 const>,
reference_wrapper<float3 const>>`.

Changed:
```
functor->resource  -> boost::cref( functor->resource )
functor->position  -> boost::cref( functor->position )
functor->direction -> boost::cref( functor->direction )
functor->normal    -> boost::cref( functor->normal )
```
The whole bind region collapsed to `.. same ..`. New boost
void_function_obj_invoker / bind_t / storage symbols delinked to 100%.

## Residual (non-steerable wall)
Only diff left: `bullet_manager::bullet_functor* functor = VOSTOK_NEW_IMPL(...)`
SIZE (target 0x49 vs base 0x4b) + an `EMPTY only base` slot. Cause: target calls
`bullet_functor_mt_allocator::malloc_impl` out-of-line (present at target rva
0xae5c0), base inlines it to `intrusive_mpmc_stack::try_pop` (no standalone
malloc_impl in base index). Same whole-program LTCG inline-vs-call class as
malloc_impl in add_decal/update_tracer. Source correct.

## Commands
pdb_fetch --function play_particle --rva 0x5916e0 --view structure-diff/diff/target/base
pdb_rich_query --function malloc_impl --list (target has 0xae5c0, base none)
