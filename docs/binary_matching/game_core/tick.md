# bullet::tick (0x582a20) - structure verification

Reported fuzzy: ~89.91%. Verdict: STRUCTURE MATCH (shape ok), residual is
non-steerable LTCG.

## Root cause of the SIZE cascade
`pdb_fetch --view diff` at the prologue:

    target: sub esp, 90h ; mov [ebp-88h], ecx   (this @ -0x88, needs disp32)
    base  : sub esp, 8Ch ; mov [ebp-80h], ecx   (this @ -0x80, fits disp8)

The high_time statement calls `m_bullet_manager->get_bullet_time_factor()`:

    base  : fstp dword ptr [ebp-8Ch] ; call bullet_manager::get_bullet_time_factor ; fmul [ebp-8Ch]
    target: fmul st, dword ptr [?s_bm_bullet_time_factor@survarium@@3MA]

The target INLINED `get_bullet_time_factor()` and reads the file-static
`s_bm_bullet_time_factor` directly (defined `static float` in bullet_manager.cpp,
line 16). With the call gone there is no FP-spill temp and the frame/local layout
differs: target `this` lands at `[ebp-88h]` (disp32, 6-byte loads), base at
`[ebp-80h]` (disp8, 3-byte loads). Every `mov reg, [ebp-this]` is therefore 3 bytes
shorter in base -> all 21 SIZE diffs (each `m_start_velocity = zero_velocity;`,
each `this`-read) are this one cascade.

Not source-steerable: `get_bullet_time_factor` is an out-of-line method in another
TU and `s_bm_bullet_time_factor` is TU-local to bullet_manager.cpp. The original
kept the static TU-local and let LTCG inline the accessor into tick; we cannot read
that static from bullet.cpp, and header-inlining the accessor would require moving
the static to a header - a structural change the original did NOT make. sushi@MATCH
already notes "target inlined get_bullet_time_factor".

## math::min reg-arg order (one of the quantity diffs)
`math::min( g_bullet_tracer_exposition, m_life_time )` is called with both float
operands pre-loaded into xmm0/xmm1 (LTCG custom calling convention):

    target: xmm0 = g_bullet_tracer_exposition (arg0), xmm1 = m_life_time (arg1)
    base  : xmm0 = m_life_time, xmm1 = g_bullet_tracer_exposition (reversed)

Which register each argument occupies is chosen by LTCG at link time -> bankable
LTCG (argument-in-register), not a source order issue. Source order already matches
the target's natural order.

## Statement-attribution quantity diffs (ONLY base/target rows)
`if ( time != 0 )` + `if ( !update_bullet_position(...) )` are two nested source
statements in our base; the target attributed the failure body to a single source
line (L181) and shows L183/L200 where base has none. These are physical-source-line
attribution differences in the original layout (not byte-affecting); the underlying
asm shape (ucomiss/jnp guard -> update_bullet_position call -> store/return) matches.
Merging the two ifs into `if ( time != 0 && !update_bullet_position(...) )` would not
change bytes nor the frame cascade, so it was not pursued.

## Conclusion
Shape matches. Residual = LTCG inline-vs-call (get_bullet_time_factor) frame cascade
+ LTCG min reg-arg order + register/slot allocation noise. Non-steerable.
