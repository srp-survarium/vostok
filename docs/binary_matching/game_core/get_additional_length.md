# game_core::get_additional_length

`float survarium::get_additional_length(vostok::math::float3 const&, vostok::math::float3 const&, float)`
Free function in `namespace survarium`, `sources/vostok/game_core/sources/legs_ik_processor.cpp`.
Target rva 0x0bb1f0.

## Target asm (pdb_fetch --view target)

```
0x00: push ebp; mov ebp,esp; sub esp,20h
0x06: lea eax,[ebp-10h]; mov ecx,[ebp+0Ch]; call float3_pod::operator-   ; -leg_dir -> [ebp-10h]
0x11: mov ecx,[ebp+8]; call operator|                                    ; upleg_dir | (-leg_dir)
0x19: movss [ebp-4],xmm0                                                 ; knee_angle_cos = dot
0x1e: movss xmm0,[clear_value]; movss [ebp-14h],xmm0                     ; 1.0f -> temp slot
0x2b: push ecx; fld [epsilon_5]; fstp [esp]                              ; 3rd arg = epsilon_5
0x35: lea ecx,[ebp-14h]; lea eax,[ebp-4]; call is_similar<float>         ; is_similar(&cos, &1.0f, eps)
0x48: je .1
0x4a: movss xmm0,[ebp+10h]; mulss xmm0,[half]; movss [ebp-20h],xmm0; jmp .2  ; knee_len * 0.5f
.1:
0x5e: lea eax,[ebp+10h]; call sqr<float>                                 ; sqr(knee_len)
0x66: mulss xmm0,[half]                                                  ; * 0.5f
0x6e: movss xmm1,[clear_value]; subss xmm1,[ebp-4]                       ; 1.0f - cos
0x7b: divss xmm0,xmm1                                                    ; / (1-cos)
0x7f: push ecx; movss [esp],xmm0; call sqrt                              ; sqrt(...)
0x8d: fstp [ebp-20h]
.2:
0x90: fld [ebp-20h]; mov esp,ebp; pop ebp; ret                          ; return result
```

Structure: 4 statements (L112 prologue, L113, L114, L115 epilogue). L114 is one
statement => the whole if/else is a single ternary `return`.

## Constants (read from target obj .text relocs -> .rdata/.data)
COFF: `binaries/objdiff/target/vostok/game_core/sources/legs_ik_processor.cpp.obj`,
fn at .text+0x2468.
- `epsilon_5` -> .rdata 0x3727c5ac = 9.999999e-06 = `vostok::math::epsilon_5` (`.00001f`)
- `half`      -> .rdata 0x3f000000 = 0.5f  (bare `0.5f` literal; delinker-named "half")
- `clear_value` -> .data +0x44/+0x48 = 0x3f800000 = 1.0f  (bare `1.0f` literal; in .data)

`operator-` = unary `float3_pod::operator-() const` (negate), `operator|` = dot product
(`float operator|(float3_pod const&, float3_pod const&)`). `is_similar<float>` =
`math_functions_inline.h` template (3rd arg `float const epsilon` by value, but the two
floats by reference -> 1.0f materialized to a slot). `sqr<float>`, `sqrt` from math.

## Source written
```cpp
float const knee_angle_cos = upleg_dir | -leg_dir;
return math::is_similar( knee_angle_cos, 1.0f, math::epsilon_5 )
    ? knee_len * 0.5f
    : math::sqrt( math::sqr( knee_len ) * 0.5f / ( 1.0f - knee_angle_cos ) );
```

Anchor: `use_game_core_get_additional_length` in temp_include_all.cpp (process_leg,
its real caller, is still STUB). Forward-declared in the survarium decl block.

## Commands run (verbatim, in order)
- nix develop --command pdb_rich_query --index binaries/rich/target/index.jsonl --function get_additional_length --list
- nix develop --command pdb_fetch --target-index binaries/rich/target/index.jsonl --function get_additional_length --view target|structure|info|callees
- python3 (COFF parse of legs_ik_processor.cpp.obj) -> constant values
- python3 scripts/rebuild.py

## Iterations
1. INPUT: ternary form above + anchor `use_game_core_get_additional_length`.
   BUILD: 65.375% (report.json).
   DIFF (--view diff, objdiff fuzzy 88.43%): the ONLY divergence is the dot product.
     TARGET: `mov ecx,[ebp+8]; call operator|` (out-of-line dot).
     BASE:   inlines the whole dot (`mov [ebp-18h],eax; movss xmm0,[a+8]; mulss
             [b+8]; ...; addss; addss; movss [ebp-4],xmm2`).
   Everything else byte-exact: operator-, is_similar(&cos,&1.0f,eps), sqr, sqrt, the
   1.0f/0.5f/epsilon_5 constants, the ternary je/jmp control flow. The frame shift
   (sub esp,24h vs 20h) and the [ebp-24h] vs [ebp-20h] result slot all CASCADE from
   the single inlined dot.
   ROOT CAUSE: per-call-site inline-vs-call LTCG of a trivial COMDAT. pdb_rich_query
   shows operator| present out-of-line in BOTH indexes (target 0x8160, base 0x371e0),
   so it is NOT inlined-everywhere - this one call site was inlined by whole-program
   codegen. Not steerable from this function's source (assembly_patterns.md, the
   vectora::size/is_aimed/fixed_string inline-vs-call class). Source is correct.

## Re-investigation (structure-verifier challenge to the LTCG label)

The structure verifier (docs/.../structure/get_additional_length.md) claimed the residual is
caller-source-steerable (inline-vs-call of an inline COMDAT) and pushed for a rewrite. I ran the
experiments. Verdict: the verifier's premise is EMPIRICALLY FALSE - no caller-source form both keeps
the correct `operator|` symbol AND emits it as the target's out-of-line call. Detail:

Forms tried (each a full rebuild, watchdog --stall 150, this worktree's report.json):
1. `upleg_dir | -leg_dir`                  -> 65.375%. operator| INLINED at the call site.
2. `math::operator |( upleg_dir, -leg_dir )` (explicit operator-call syntax, SAME symbol)
                                            -> 65.375%. Byte-identical to form 1; still inlined.
3. `math::dot_product( upleg_dir, -leg_dir )` -> 90.4%. BUT binds to the TEMPLATE
   `vostok::math::dot_product<vostok::math::float3>` (math_functions_inline.h:16), which overload
   resolution prefers over the non-template free `dot_product(float3_pod const&, float3_pod const&)`
   (float3 is an exact `T` match; the non-template needs a float3->float3_pod base conversion). That
   template is emitted as a `__cdecl` call (push/push, x87 `fstp` return) - NOT operator|.

DECISIVE: the TARGET binary's rich index has NO `dot_product<float3>` template symbol at all - only
`operator|` (rva 0x8160) and the member `float3_pod::dot_product` (rva 0x8130). So the original source
did NOT write `dot_product(a,b)` (that would instantiate+emit the template). It wrote `a | b`. Form 3's
90.4% is a coincidentally-higher score over a FABRICATED function that does not exist in the target -
forbidden by the "reproduce exactly, never coincidentally-higher %" rule. Rejected; reverted to form 1.

Why the correct form can't be steered to the target bytes:
- TARGET `operator|` (0x8160) is a `__fastcall`-style COMDAT (ecx=left, eax=right, returns in xmm0),
  emitted out-of-line and CALLED at this site. Its sibling member `dot_product` (0x8130) is likewise a
  custom-convention COMDAT (eax=this/left, ecx=right). These custom calling conventions are LTCG
  whole-program convention promotions on internal/COMDAT helpers.
- BASE `operator|` standalone (0x37000) is plain `__cdecl` x87 `/Od` - our base build did NOT apply the
  same LTCG convention promotion, and at this call site `/Ob2` INLINES operator| (SSE arithmetic).
- The two divergences (per-call-site inline-vs-call + the fastcall-vs-cdecl convention) are both
  whole-program/linker decisions. No caller-source spelling reaches them: every form that keeps the
  `operator|` symbol inlines (forms 1,2), and the only form that calls (form 3) is a different function.

Build config note: game_core `Master Gold|Win32` is `Optimization="0"` (/Od) BUT
`InlineFunctionExpansion="2"` (/Ob2) + `EnableEnhancedInstructionSet="2"` (SSE2) + intrinsics +
WholeProgramOptimization (/GL). So despite "/Od", /Ob2 DOES inline `inline` helpers - which is why
operator| inlines in our base. (README's "/Od" is shorthand; /Ob2 is on.)

## Outcome
STATE[65.38%|PARTIAL]: residual = `operator|` (dot) inlined in base, called in target -
per-call-site whole-program LTCG inline-vs-call of a trivial COMDAT; both binaries keep
the standalone operator|. Source `upleg_dir | -leg_dir` is correct; every other statement,
all constants, and the ternary control flow are byte-exact.
Regressions caused: none (report-changes shows only `basic_streambuf::imbue` 100->0, an
unrelated CRT/STL streambuf function I did not touch - a build-ordering report flake).
Inlining: the single `operator|` call site (whole-program, unsteerable).

## Deep pass (anchor-removal, match/game_core-legs_ik_processor-deep)
Hypothesis: the direct fake-observation anchor `use_game_core_get_additional_length`
(NULL args, escaped float result) might have forced the `operator|` inline. process_leg
(its real caller) is now anchored transitively via the real `processor` instance in
`use_game_core_legs_ik_processor`, so the direct anchor + its IncludeAll dispatch line
were removed (temp_include_all.cpp).
- COMMAND: python3 scripts/rebuild.py (no module arg); python3 scripts/legs_scores.py;
  pdb_fetch --base-index binaries/rich/base/index.jsonl --target-index binaries/rich/target/index.jsonl --function get_additional_length --view diff
- RESULT: 65.38% UNCHANGED. The diff is byte-identical to the PR #159 baseline - base still
  inlines the one `operator|` call site, target still emits `call vostok::math::operator|`,
  same `sub esp,24h` vs `20h` cascade. Function still SCORES (not dead-stripped) - reached
  transitively through process_leg.
- CONCLUSION: the residual is GENUINE per-call-site whole-program LTCG inline-vs-call, NOT an
  anchor-observation artifact. The fake anchor was redundant clutter (correctly removed) but
  was never the cap. Stays 65.38% PARTIAL.
