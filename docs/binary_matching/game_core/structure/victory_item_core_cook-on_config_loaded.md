# structure verify: victory_item_core_cook::on_config_loaded

VERDICT: STRUCTURE MATCH (shape ok)

target 0x752150 | base 0x565d20 | report.json fuzzy 93.71%
(STATE marker was stale at 79.67%; synced to 93.71% PARTIAL.)

## condensed structure-diff
    ; target 10 stmts / base 10 stmts
    .. same ..
    0x015 <0x22> | 0x015 <0x1d> | configs::binary_config_ptr cfg = static_cast_resource_ptr<...>( data[0].get_unmanaged_resource() );   SIZE
    <0>         | --          |    EMPTY only target
    --          | <0>         |    EMPTY only base
    .. same ..
    ; aligned 8, size-diffs 1, quantity-diffs 2

## Divergence
- SIZE on the `cfg = static_cast_resource_ptr<...>(data[0].get_unmanaged_resource())`
  statement (0x22 target / 0x1d base). The base spills an extra by-value resource_ptr
  temp at [ebp-10h] (copy + dtor) that the target does not, bumping the frame 0x30->0x34
  and renaming the trailing [ebp-N] slots (a constant offset, not a structure diff).
- The two EMPTY-only rows are a collapsed source-line attribution gap (one per side),
  not a real statement count difference.

Real statement quantity is 10 == 10. No control-structure, brace, member-init, or
ordering divergence.

## Cause / fix
Non-steerable LTCG: the by-value resource_ptr returned through the inlined `data[0]`
operator[] + get_unmanaged_resource chain is materialized through an extra intermediate
temp in our base build. Documented intrusive_ptr-by-value materialization class. No
source restructure available. Detail: ../on_config_loaded.md
