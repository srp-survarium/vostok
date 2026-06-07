# bullet_manager::destroy_bullet (0x591330)

~97.20% PARTIAL (marker was stale 99.89%). Structure MATCH (4/4 stmts, 0 diffs).

## Residual (non-steerable wall)
Pure frame-size LTCG: target `sub esp,2Ch` vs base `sub esp,40h` (base 0x14 more),
cascading `[ebp-2Ch]` vs `[ebp-40h]` slot renames. No structural difference. The
`m_bullets.erase(iter)` + `VOSTOK_DELETE_IMPL` temporaries are lowered with a
different stack layout under whole-program LTCG. Source correct.

## Commands
pdb_fetch --function destroy_bullet --rva 0x591330 --view structure-diff/diff
