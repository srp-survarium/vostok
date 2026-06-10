# malloc_impl / delete_helper<> wrapper inline-vs-call (VOSTOK_NEW/DELETE_IMPL)
tags: cpp:new-delete cpp:macro | asm:call asm:push | topic:inline-vs-call topic:allocator
symptoms: call malloc_impl vs call try_pop, call delete_helper vs delete_helper_impl with predicate bool, extra push 58h, frame +4
confidence: 8/10
variants: strip-pointer-delete-resource.md, free-vs-delete-impl.md

`VOSTOK_NEW_IMPL`/`VOSTOK_DELETE_IMPL` alloc/free rows that SIZE-diff a few bytes + bump
the frame by 4: TARGET calls the wrapper (`<allocator>::malloc_impl`, `delete_helper
<alloc,T>`), BASE inlines the wrapper to the leaf (`try_pop`; `delete_helper_impl<...,
call_destructor_predicate>` with the predicate bool materialized at the site). TELL:
pdb_rich_query finds the wrapper at a real rva in target only. Source macros correct;
mark PARTIAL.

Evidence: bullet_manager add_decal (malloc_impl @target 0xae5c0), free_bullet (delete_helper @0xae9f0, delete_helper_impl @0xaed80); bullet_functor_mt_allocator.
