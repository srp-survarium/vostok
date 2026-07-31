# `movss xmm, [const-pool]` where base emits `xorps` = the float literal is NOT `0.f`

tags: cpp:float cpp:ternary cpp:literal | asm:movss asm:xorps | topic:codegen-idiom topic:constant-recovery

## Symptom
A reconstructed statement is ~5 bytes SHORT of the target and the only instruction
difference is at the branch that supplies a default value:

```
base    0x4cc:  xorps xmm0, xmm0                 ; 3 bytes
target  0x4d1:  movss xmm0, dword ptr [<pool>]   ; 8 bytes  (delinker may name the
                                                 ;  pool slot after an unrelated
                                                 ;  global, e.g. `clear_value`)
```

## Cause
MSVC materialises the float **zero** with `xorps` and every OTHER float literal with a
load from the read-only constant pool. So the two-side difference is not scheduling -
it says the source literal you guessed as `0.f` is a different constant.

## Source mapping
`load_props_impl<binary_config_value>` (render facade light cook):

```
props.diffuse_influence_factor  = cfg.value_exists( "diffuse_influence_factor" )
                                  ? (float)cfg["diffuse_influence_factor"] : 0.f;   // xorps  - WRONG
props.diffuse_influence_factor  = cfg.value_exists( "diffuse_influence_factor" )
                                  ? (float)cfg["diffuse_influence_factor"] : 1.f;   // movss  - matches
```

Cross-check the VALUE, don't just pick "not zero": find another instruction in the same
function that reads the **same pool address** and whose meaning you already know. Here
the identical slot also feeds `m[3][3]` of the inlined `math::create_scale`, which is
`1.0f` by construction - confirming the influence-factor defaults are `1.f` (semantically
right too: full influence unless the config overrides it).

## Generalisation
- `xorps`/`pxor` present on only one side -> a `0.f` vs non-zero literal disagreement.
- an `fld1` on the target where base loads from the pool -> the literal IS `1.0f` but
  the source reached it through the x87 path (a `double`-typed argument), not SSE.
- a constant-pool slot the delinker names after a data symbol is usually a *pool entry*,
  not that global; read the other users of the same address before believing the name.
