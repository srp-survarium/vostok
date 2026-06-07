# Calculator tick functions - STRUCTURE MATCH (frame-slot / inline-vs-call walls)

Batch structure-verification of the dispersion/recoil/breath calculator ticks.
All verified via `pdb_fetch --view structure-diff --condensed`. None had a steerable
shape divergence; each carcass was replaced with the condensed structure-diff embed.

## character_dispersion_calculator::tick - 99.67%
target 22 / base 21 stmts; aligned 19, size-diffs 0, quantity-diffs 5 (all EMPTY
source-line gaps). STRUCTURE MATCH. Residual: /Od frame-slot (target sub esp,1Ch,
this@[ebp-10h] vs base sub esp,18h, this@[ebp-0Ch]) - one extra unused 4-byte slot
shifting every [ebp-N]. Non-steerable.

## character_recoil_calculator::tick - 99.65% (STATE was stale at 99.63%)
target 16 / base 18 stmts; aligned 16, size-diffs 0, quantity-diffs 2 (EMPTY gaps).
STRUCTURE MATCH. Residual: same /Od frame-slot allocation (+4 bytes, shifted offsets).

## breath_vibration_calculator::tick - 94.23%
target 19 / base 21 stmts; aligned 17, size-diffs 1, quantity-diffs 4. The lone SIZE
is `current_phase = m_user->local_time(...)` - frame-slot churn (target sub esp,38h,
this@[ebp-24h] vs base 30h, this@[ebp-1Ch]) swapping the two m_user-load registers.
The current_state ONLY base / ONLY target pair (L84) is a source-line attribution split
of the SAME statement, not a quantity divergence. STRUCTURE MATCH, non-steerable.

## weapon_recoil_calculator::tick - 93.37%
target 51 / base 55 stmts; aligned 49, size-diffs 2, quantity-diffs 4. The 2 SIZE rows
are inline-vs-call of `std::min`/`m_interpolator.transition_time()` and the FPU
0.0-compare lowering (`if (m_additive_recoil_timer != 0.0)`). LTCG, non-steerable.
The stale "Ghidra didn't generate / BLOCKED" note was wrong - it disassembles and scores.

## dispersion_calculator::get_dispersion - 87.49% (already verified earlier, left as-is)
The "dispersion_calculator ~87.49% tick" in the dispatch brief is actually get_dispersion;
it already carries its structure-diff embed + VERDICT (is_aimed inline-vs-call + safe-bool
materialization, LTCG). No change.
