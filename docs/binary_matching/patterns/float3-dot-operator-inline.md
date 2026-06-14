# float3 dot `a | b`: per-call-site inline-vs-call; NEVER rewrite as dot_product
tags: cpp:operator cpp:float | asm:call asm:movss | topic:inline-vs-call
symptoms: call vostok::math::operator| vs inlined mulss/addss chain, sub esp 0x24 vs 0x20
confidence: 8/10
variants: inline-vs-call-template-comdat.md, mixing-expression-operator-plus.md

`a | b` (float3 dot) emits a real `call operator|` in TARGET but is inlined at the call
site in BASE. operator| is standalone in BOTH indexes (target 0x8160, base 0x371e0) - a
PER-call-site whole-program LTCG decision, not steerable. The inlined dot adds a frame
temp, bumps `sub esp` and shifts the result slot, all cascading from the one inline.
Source `a | b` is correct; mark PARTIAL.

```cpp
upleg_dir | -leg_dir   // keep the operator spelling
```
RE-CONFIRMED, do not re-litigate: `a|b` and `operator|(a,b)` both inline (65.375%); `dot_product(a,b)` scores 90.4% but binds the TEMPLATE dot_product<float3> (math_functions_inline.h:16, __cdecl x87) - a FABRICATED function absent from the target (only operator| 0x8160 + member dot_product 0x8130); forbidden. Second residual: target's operator| is LTCG-promoted __fastcall (ecx/eax, xmm0 return) vs our __cdecl x87 standalone. (game_core is /Od but /Ob2, which is why inline helpers inline at all.)
Evidence: game_core/survarium::get_additional_length (target rva 0x0bb1f0, 65.38% PARTIAL).
Also game_core/bullet.cpp whole TU: every `triangle_normal | direction` / `start_to_max_error | start_to_target` keeps operator| out-of-line in target, inlined in base - fix_collision_point_and_time (70.7%), compute_max_error (84.6%), check_collision (92.5%), process_ray_query (92.2%, also math::acos out-of-line in target). math::acos and linear_interpolation behave the same way (acos out-of-line target, linear_interpolation INLINED in target - opposite direction, same un-steerable LTCG cut). Source spellings all correct; PARK.
evidence-basis: rebuild-confirmed across one whole TU (bullet.cpp) and get_additional_length
