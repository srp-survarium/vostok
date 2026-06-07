# weapon_recoil_calculator unit

Branch: match/game_core-weapon_recoil_calculator off origin/int/game_core

## Setup
- Added `#include <vostok/game_core/weapon_recoil_calculator.h>` to temp_include_all.cpp
- Added `use_game_core_weapon_recoil_calculator()` anchor + dispatcher call.
- Header access-specifier fix: moved `process_compensation`, `get_random_angle`,
  `get_random_amount`, `reset` under `private:` (mangled `AAE` = private). Before
  this they were public and objdiff wouldn't pair them.

## Mangled access chars (from target obj strings)
- ctor QAE, set_weapon QAE, fire QAE, reload QAE, chamber_a_round QAE, tick QAE,
  pseudo_random::random_f QAE  -> public
- reset AAE, get_random_amount AAE, get_random_angle AAE, process_compensation AAE -> private

## Baseline after access fix + anchor (report.json)
- set_weapon 100, reset 100, chamber_a_round 100, reload 100, ctor 100
- get_random_amount 80.75, process_compensation 86.90, get_random_angle 89.02,
  fire 79.80, tick 93.37
- random_f 21.57
- dynamic initializer for s_recoil_use_pseudo_random_cc 0.00

## get_user inline WALL (cross-unit, weapon_core.h owned by v7)
get_random_angle / get_random_amount / fire / tick all call
`m_weapon->get_user()`. In the TARGET this is a real `call weapon_core::get_user`;
in our base it INLINES (weapon_core.h line 177 declares get_user as an in-class
inline returning m_user, reading offset 0x44C). The inline shifts the frame size
(+4 temp) and replaces the call with mov chains, so these functions cannot reach
100% without making weapon_core::get_user out-of-line in weapon_core.h. That file
is owned by v7 (forbidden). => these 4 are PARTIAL, cross-unit wall.
Aside from the get_user inline, get_random_angle/amount match instruction-for-
instruction.

## process_compensation fixes (was 86.90)
Target vs base diffs found:
1. line 293: target `addss` not `mulss` -> `math::sqr(vert) + math::sqr(horiz)`
   (source had `*`).
2. line 294: eval order `( side_compensation_speed + additive ) * dt_sec *
   m_player_compensation_multiplier` (target loads param-sum first).
3. lines 296/299: abs comparisons use `comiss` (SSE) in target but x87 in base;
   swapped operands to `math::abs(x) > compenstion_amount` so the abs() result is
   the comiss register operand.
4. line 304: same eval-order fix as #2 for back compensation.
Note process_compensation does NOT call get_user.
Result: 86.90 -> 93.74. Residual: the two `math::abs(float)` comparisons emit x87
(fld/fcomip) in our build vs SSE (comiss) in the target. abs() returns its float in
ST0 here but xmm0 in the target LTCG build -> return-register diff at the abs() call
boundary. Tried swapping comparison operands (`abs(x) > amount`): no change. Accept
as call-boundary residual.

## fire fixes (was 79.80 -> 95.10)
1. line 195 PRECEDENCE BUG: `m_player_recoil_multiplier * force_koef * first_shoot
   ? a : b` parsed as `(prod ? a : b)`. Target is `(first_shoot ? a : b) * force_koef
   * m_player_recoil_multiplier`. Fixed (and same for recoil_amount line 209).
2. line 201 sqr order: target does sqr(vert) + sqr(horiz) (vert/0x34 first); source
   had horiz first. Fixed.
Tried line 210 `math::min`->`std::min` to match target's `stlp_std::min<float>`:
REGRESSED 95.10->92.93 (std::min grew the frame). Reverted to math::min (higher %).
Residual: +0xC frame size + `first_shoot` && temp stored as dword (base) vs byte
(target) - temp-width codegen, not steerable.

## tick fixes (was 93.37 -> 93.90)
1. line 82 `!= 0.0` (double) -> `!= 0.0f`: target uses float compare (ucomiss), base
   used cvtss2sd/ucomisd. Fixed.
2. line 99 sqr order vert-first (same as fire). Fixed.
Residual: std::min<float> at line 80 is INLINED in our build (comiss/jbe ternary
returning a ptr) but is an out-of-line `call stlp_std::min<float>` in the target.
LTCG inlining-decision difference, not steerable from this unit.

## get_random_amount: also fixed line 344 `0.25` (double) -> `0.25f` (target uses
float max, movss [__real@3e800000]). Score unchanged (get_user wall dominates) but
the literal is now correct.

## random_f WALL (LTCG pow inlining)
Target references pow_impl(float,u32) as an out-of-line call and INLINES
pow(float,int) (no pow(float,int) symbol in any target obj). Our build emits
pow(float,int) as an out-of-line `call` (symbol ?pow@...@YAMMH@Z present) and does
NOT inline it -> the whole x87 schedule of the 'k' statement diverges (base 'k' is
133 bytes, target 213 - the ~80-byte integer-pow branch set). pow(float,int) is in
shared math_functions_inline.h; this is an LTCG inlining decision, not steerable
from this unit. Stays 21.57%.

## dynamic initializer for s_recoil_use_pseudo_random_cc (0%)
The cc_bool ctor is called with several args in registers (eax=1, ecx=0, esi=this)
- an LTCG custom calling convention at the cc_bool ctor boundary. Not steerable from
this unit.

## FINAL SCORES
- set_weapon 100, reset 100, chamber_a_round 100, reload 100, ctor 100 (5x DONE)
- fire 95.10 PARTIAL (frame/bool-temp residual)
- tick 93.90 PARTIAL (std::min LTCG inline wall)
- process_compensation 93.74 PARTIAL (abs return-register x87/SSE)
- get_random_angle 89.02 / get_random_amount 80.75 PARTIAL (get_user inline wall, cross-unit weapon_core.h)
- random_f 21.57 PARTIAL (pow(float,int) LTCG inline wall)
- dynamic initializer 0 (cc_bool ctor LTCG calling convention)
No regressions anywhere in the build.
