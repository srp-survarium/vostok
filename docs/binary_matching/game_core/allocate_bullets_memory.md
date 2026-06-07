# bullet_manager::allocate_bullets_memory (0x592590)

~85.75% PARTIAL (marker was stale 92.30%). Structure MATCH (11/11 stmts), one SIZE.

## Residual (non-steerable wall)
SIZE on the `query_create_resources(_and_wait)( &request, 1, bind, g_allocator )` call.
`query_create_resources` has 7 params; source passes 4 and relies on 3 defaults
(`variant<32> const** = NULL`, `query_result_for_cook* = NULL`, `assert_on_fail_bool`).
The 5 pushed dwords (request, count, alloc, 2x NULL, bool) differ in ORDER/FORM:
- TARGET: `push 1; push 0; push 0; push g_allocator; push &request; mov ecx,1;
  lea eax,[ebp-10h]; call ...; add esp,14h`
- BASE:   `push 1; push g_allocator; push &request; push 1; lea edx,[ebp-10h];
  push edx; xor eax,eax; xor ecx,ecx; call ...; add esp,14h`
Both clean up 0x14 (5 dwords). The boost::function arg is by const-ref (`lea
ecx,[ebp-30h]`) in both. This is LTCG default-argument materialization/order at the
call boundary - the one residual class MATCHING.md allows stopping on. Everything
after is cascading reg renames. Source correct.

## Commands
pdb_fetch --function allocate_bullets_memory --view structure-diff/diff/target/base
pdb_rich_query --function query_create_resources --list (7-param signature)
