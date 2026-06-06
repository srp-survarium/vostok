# Structure verification: survarium::get_additional_length

Function: `float survarium::get_additional_length(math::float3 const&, math::float3 const&, float)`
Mangled: `?get_additional_length@survarium@@YAMABVfloat3@math@vostok@@0M@Z`
Unit: `sources/vostok/game_core/sources/legs_ik_processor.cpp`
Size: 151 bytes (target 0x97, base 0xc8)
report.json fuzzy_match_percent: 65.375 (PARTIAL)

## Verdict

STRUCTURE MISMATCH (size)

QUANTITY matches (4 statements vs 4 statements). One statement diverges in SIZE:
the dot-product line. The PARTIAL/LTCG label is NOT justified - the residual is a
source-steerable inline-vs-call codegen decision on an inline COMDAT, which the
project owner's standing rule explicitly classifies as a matching problem, NOT
bankable LTCG (LTCG banks only dropped/register-passed ARGUMENTS at a call
boundary). A matcher should continue.

## The two skeletons

TARGET ; 4 statements, 0x97 bytes
```
0x00  <0x6>   L112   prologue
0x06  <0x18>  L113   knee_angle_cos = upleg_dir | -leg_dir;   (24 bytes)
0x1e  <0x75>  L114   return is_similar(...) ? ... : sqrt(...) (117 bytes)
0x93  <0x4>   L115   epilogue
```

BASE ; 4 statements, 0xc8 bytes
```
0x00  <0x6>   {                                               prologue
0x06  <0x49>  knee_angle_cos = upleg_dir | -leg_dir;          (73 bytes)  <-- diverges
0x4f  <0x75>  : sqrt( sqr(knee_len) * 0.5f / (1.0f - cos) );  (117 bytes) byte-exact
0xc4  <0x4>   }                                               epilogue
```

Statement count, the ternary control flow, all three constants (0.5f `half`,
`epsilon_5`, 1.0f) and the `is_similar` / `sqr` / `sqrt` call sequence in the L114
statement are byte-exact (both `<0x75>`). The entire residual lives in ONE
statement: the `upleg_dir | -leg_dir` dot product. Target 0x18, base 0x49 -
delta of 0x31 (49 bytes). The frame also grows sub esp,20h -> sub esp,24h and an
extra `[ebp-18h]` temp appears; both are direct consequences of the same single
divergence.

## The divergence, statement by statement

SIZE diff on L113 (`float const knee_angle_cos = upleg_dir | -leg_dir;`):

TARGET 0x06..0x1e (24 bytes) - operator| emitted as a CALL:
```
0x06  lea   eax, [ebp-10h]
0x09  mov   ecx, [ebp+0Ch]
0x0c  call  vostok::math::float3_pod::operator-     ; -leg_dir
0x11  mov   ecx, [ebp+8]
0x14  call  vostok::math::operator|                 ; <-- OUT-OF-LINE CALL
0x19  movss [ebp-4], xmm0
```

BASE 0x06..0x4f (73 bytes) - operator| INLINED to the raw dot product:
```
0x06  lea   eax, [ebp-10h]
0x09  mov   ecx, [ebp+0Ch]
0x0c  call  vostok::math::float3_pod::operator-     ; -leg_dir (still a call - matches)
0x11  mov   [ebp-18h], eax                          ; extra temp for the negated vec
0x14..0x46  movss/mulss/addss x3 + sums             ; x*x + y*y + z*z spelled out inline
0x4a  movss [ebp-4], xmm2
```

This is `operator |` (math_float3_inline.h:295) forwarding to
`float3_pod::dot_product` -> `x*other.x + y*other.y + z*other.z`
(math_float3_inline.h:118). The target keeps `operator|` as an out-of-line COMDAT
call; our base inlines it down to the arithmetic. `operator-` is a CALL on BOTH
sides, so the divergence is isolated to operator| alone.

Note: at base 0x4f / 0x9f the 1.0f constant is reloc'd through the symbol
`?scale_speedtree_to_vostok@speedtree_forest@render@vostok@@2MA` whereas the
target uses `clear_value`. Both are the same float 1.0f value picked up by a
different COMDAT-folded .rdata symbol; this is linker data-folding noise, not a
structure diff, and it sits inside the already byte-exact L114 statement.

## Is the "operator| LTCG" claim justified?

No. The in-source STATE comment (legs_ik_processor.cpp:153-159) calls this a
"per-call-site whole-program LTCG inline-vs-call of a trivial COMDAT ... Not
steerable from this function's source." Under the project owner's standing rule
this is mislabeled:

- LTCG is a bankable excuse ONLY for function-ARGUMENT passing (dropped or
  register-passed args at a call boundary). This residual is NOT about argument
  passing - both sides pass the same args to the same calls. It is an
  INLINE-vs-CALL decision on a header-defined inline COMDAT (`operator |`).
- Inline-vs-call is explicitly named in the rules as a SOURCE-STEERABLE matching
  problem, not LTCG. The compiler chose to inline operator| at this site; the
  target chose not to. That choice is driven by how the expression is written and
  by the inline/COMDAT shape - it is steerable.

The cascading effects the comment cites (sub esp,24h vs 20h; the extra [ebp-18h]
temp) are not independent residuals - they all follow from forcing operator| back
to a call. Fix the one inline decision and they disappear together.

## Concrete source fix (for the matcher - needs a rebuild to confirm, NOT done here)

The target wants `operator |` emitted as a CALL, not inlined. Levers, in order of
preference:

1. Write the dot product so the call survives: replace `upleg_dir | -leg_dir`
   with the explicit operator-call / free-function form that the target's source
   used, e.g. route through the same spelling that keeps `vostok::math::operator|`
   un-inlined (match the exact expression the original wrote - the target proves
   it called `operator|`, so the original source used the `|` operator, but the
   compiler kept it out of line at this site).
2. The inline-vs-call swing here is sensitive to the surrounding statement: the
   target negates via a `call operator-` and then a `call operator|`; confirm the
   negation and the dot are written as two separate operator expressions (not
   folded), matching `upleg_dir | -leg_dir` with the negation forced to its own
   call - which it already is. The remaining lever is the inline budget at this
   site, steerable by how knee_angle_cos is typed/used or by not taking the
   result through a `float const` temp that lets MSVC see straight through the
   COMDAT.
3. If the operator| body itself is what MSVC keeps inlining, check that the
   `operator |` definition in math_float3_inline.h is presented to this TU the
   same way it was to the target's TU (declaration vs inline-definition
   visibility). A call survives when the inline def is not visible at the call
   site in the same form.

The right next step is for a matcher to experiment with the call-site expression
until `vostok::math::operator|` is emitted as a call at offset 0x14, collapsing
L113 from 0x49 back to 0x18 and the frame from 0x24 to 0x20.

## Bottom line

- STRUCTURE MISMATCH (size), isolated to the L113 dot-product statement.
- 65.375% is genuine PARTIAL, but the residual is SOURCE-STEERABLE
  (inline-vs-call of an inline COMDAT), NOT bankable argument-passing LTCG.
- The in-source "LTCG / not steerable" justification does NOT hold; PARTIAL should
  stay open for a matcher to push on, not be banked as an LTCG excuse.
