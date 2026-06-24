# Inlined matrix re-orthogonalisation = set up-row world-up + i = cross(j, k)

## confidence
8/10 - verified on `free_fly_camera::on_activate` (game): a one-statement
`m_matrix = src` followed by a block that sets the j row to (0,1,0) and rebuilds
the i row reconstructed exactly as two member-assign statements; 38.77% -> 79.75%,
STRUCTURE MATCH.

## tags
cpp:member cpp:inline cpp:assignment | asm:movss asm:mulss asm:subss asm:rep-movsd |
topic:codegen-idiom topic:math-inline

## symptom
- A `rep movsd ecx=10h` copying a 16-float `float4x4` into a member (often appears
  TWICE back-to-back - the second copy is the start of the re-orthogonalise block,
  not a second source statement).
- Then 3 lone `movss` writing `(0, 0, 1.0)` into consecutive matrix slots in the
  order x, z, y (the optimiser reorders the float3 store) - this is the **up row
  set to world-up (0,1,0)**. The `1.0` comes from a `clear_value` / `__real@3f800000`
  constant, the `0`s from `xorps xmm,xmm`.
- Then a cross-product expansion: reads `k.y` and `k.z` (matrix `+0x24/+0x28` from the
  matrix base), computes `xmm = j.y*k.z - j.z*k.y` etc. With `j = (0,1,0)` this folds to
  `i = (k.z, 0, -k.x)` - a 6-mul / 3-sub schedule writing only x,y,z of the i row.

## cause / source
The author copied a view matrix then **levelled the camera roll**: forced the up
(j) row to world-up and rebuilt the right (i) row from `cross(up, forward)`:

```cpp
m_inverted_view_matrix          = cd->get_inverted_view_matrix( );
m_inverted_view_matrix.j.xyz( ) = float3( 0.f, 1.f, 0.f );
m_inverted_view_matrix.i.xyz( ) = math::cross_product( m_inverted_view_matrix.j.xyz( ),
                                                       m_inverted_view_matrix.k.xyz( ) );
```

Confirm the cross direction against `math::cross_product(left,right)`:
`result.x = l.y*r.z - l.z*r.y; result.y = l.z*r.x - l.x*r.z; result.z = l.x*r.y - l.y*r.x`.
With `left = j = (0,1,0)` this is `(r.z, 0, -r.x)` = exactly the inlined schedule, so
`left` is the **up** row and `right` is the **forward (k)** row.

## residual (non-steerable)
The `float3( 0,1,0 )` RHS may materialise as a stack temp (3 stores + `movq` load +
`movq`/`mov` copy into the row) in the base while the target writes the 3 components
straight to the matrix; the cross row can differ only by xmm register-allocation order.
Both are constructor-temp / register scheduling, not source shape - take the hit.
