# Runtime-loaded global vs const-fold, and FP reassociation of `x/180*pi*factor`
tags: cpp:global cpp:float cpp:fold cpp:console-command | asm:movss asm:mulss asm:fld | topic:codegen-idiom topic:fold-icf
symptoms: base folds a const expr into one `__real@` slot where target loads `[?global@@3MA]` + separate `[pi]`/`[1/180]`; base reorders multiply chain
confidence: 9/10

Two intertwined matching traps when a function multiplies a namespace `float` global by
degree/radian constants (camera FOV math; game_camera.cpp / camera_director.cpp).

## 1. A `float global = 57.5f;` reads as a CONSTANT unless its address escapes
A namespace-scope mutable `float` initialized to a literal in the SAME TU is treated by
MSVC as a known constant: reads get const-propagated and folded (e.g. `default_vertical_fov
/ 180.f * pi` collapses to one `__real@` slot, and `get_vertical_fov` returns a literal).
The TARGET instead loads `movss xmm0, [?default_vertical_fov@survarium@@3MA]` at runtime,
because a file-static `cc_float` console command binds `&default_vertical_fov` -> the address
escapes -> the optimizer can no longer prove it constant. Add the cc_float (its dynamic
initializer is byte-identical to the target's, min/max/serializable confirm it):
```cpp
float default_vertical_fov = 57.5f;  // extern in a shared header (referenced cross-TU)
static vostok::console_commands::cc_float
    cc_cam_fov( "fov", default_vertical_fov, 60.0f, 70.0f, true,
                vostok::console_commands::command_type_user_specific );
```
Defeating the fold flipped get_projection_matrix / get_vertical_fov to byte-exact.

## 2. Keep `1/180` and `pi` SEPARATE: match deg2rad's parenthesization, don't fold
`math::deg2rad(v) == ((v/180.f)*math::pi)` (math_functions_inline.h). When the target
INLINES it (`mulss [1/180]` then `mulss [pi]`, two slots), do NOT call `math::deg2rad()` -
under MASTER_GOLD it emits a real `call vostok::math::deg2rad` (worse). Write the body
inline, and group exactly so MSVC cannot reassociate the two constants into one
`pi/180` slot. A trailing runtime factor (`* m_fov_factor`) triggers the fold unless the
deg2rad part is its own parenthesized subexpression:
```cpp
// target: dv, *[1/180], *[pi], *[m_fov_factor]  (1/180 and pi are SEPARATE slots)
( ( default_vertical_fov / 180.0f ) * math::pi ) * m_fov_factor
// vs the ctor, where the target keeps pi/180 FOLDED then *0.75 separate:
//   dv, *[pi/180 = 0x3c8efa35], *[0.75]
( default_vertical_fov * ( math::pi / 180.0f ) ) * 0.75f
```
The outer parens force `(dv <op> const)` to evaluate as a runtime product first, so the
next constant stays a separate `mulss`. Same TU, two sibling FOV expressions, opposite
fold shapes - reproduce each exactly as the asm dictates, don't unify them.

Evidence: game_camera.cpp get_projection_matrix 87%->100%, get_vertical_fov unpaired->100%
(needed both the cc_cam_fov fold-defeat and the get_vertical_fov anchor call).
