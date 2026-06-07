# STRUCTURE: damage_model_cook::delete_resource

VERDICT: STRUCTURE MATCH (residual non-steerable)

target 0x7510a0 | base 0x5661d0 | report.json 93.26%
target 27 stmts / base 30 stmts; aligned 26, size-diffs 1, quantity-diffs 3.

This is NOT the 31% cook-base `delete_resource` wall (that one frees the raw resource
param via a register-arg delete_helper). This cook frees ONE allocation: the whole
damage_model is a single buffer, so the per-element work is just dtor calls
(`~hit_type_parameters` etc.) followed by one `VOSTOK_FREE_IMPL`.

- The 3 `EMPTY only base` quantity diffs are collapsed source-line gap markers (`<0>`),
  benign - they are not real statements.
- The single SIZE diff is the final `VOSTOK_FREE_IMPL( g_allocator, resource )`: the
  target keeps `free_helper` OUT-OF-LINE (calls `free_helper_impl<doug_lea,resource_base>`
  with an allocator null-check), while base INLINES `free_helper`. Same inline-vs-call
  LTCG class; the `sub esp,50h` vs `4Ch` frame delta cascades from it.

STATE downgraded 93.26% DONE -> PARTIAL (the residual is an inline-vs-call codegen diff,
not pure argument passing, so PARTIAL is the honest tag). Stale sushi@TODO "remove once
ctor matched" comments cleaned up (ctor is now 100%).
