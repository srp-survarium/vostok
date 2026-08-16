# Overloaded free function: delinker drops the param list, both overloads share one bare demangled name

tags: cpp:overload cpp:static | topic:mangling topic:scoring-artifact topic:tooling
symptoms: free-function overloads listed in report.json under ONE bare demangled name (no `?@@` mangle, no param list), every overload unpaired/None despite byte-correct source, base obj has the proper `?fn@ns@@YA...@Z` mangle while the target obj symbol is `vostok::ns::fn`
confidence: 8/10
variants: static-plain-name-pairing.md, mangled-join-failure.md

A NAMESPACE-scope free function that is OVERLOADED can come back from the delinker with
its parameter list stripped: the target `.obj` records every overload under the SAME bare
demangled name (e.g. both `fill_x3daudio_vector(_D3DVECTOR&, float, float, float)` and
`fill_x3daudio_vector(_D3DVECTOR&, float3 const&)` print as just
`vostok::sound::fill_x3daudio_vector`). objdiff pairs by symbol-name string, so it cannot
map either demangled, param-less target symbol to the parameter-distinguished base symbol
(`?fill_x3daudio_vector@sound@vostok@@YAXAAU_D3DVECTOR@@MMM@Z` etc.) — both overloads stay
`unpaired`/`None` in report.json no matter how correct the bytes are. The match database
recovers these functions from the rich PDB indexes: one target-side signature retains the
bare identity, while every additional signature receives a stable signature-derived key.
The base index must use the target's primary signature rather than its own RVA order;
otherwise link-order differences can cross-pair the overloads.

Distinguish from `static-plain-name-pairing.md`: there a plain name means the target fn has
INTERNAL linkage and the fix is to restore `static`. Here the fn is genuinely extern
(`YA` in the base mangle = `__cdecl` non-member, public linkage) and there is more than one
overload — making it `static` does NOT bring the param list back. This is a target-side
delinker limitation, NOT a source/matching gap.

Confirm cheaply:
```
llvm-nm binaries/objdiff/target/<unit>.obj | grep <fn>   # 'U ... \n T vostok::ns::fn'  (no ?@@, no params)
llvm-nm binaries/objdiff/base/<unit>.obj   | grep <fn>   # '?fn@ns@@YA...@Z'           (proper mangle)
```
Two `T vostok::ns::fn` lines that are byte-identical text = two overloads collapsed onto one
name on the target side.

Verify the source is still correct WITHOUT objdiff, by RVA (name-independent):
```
pdb_fetch --target-index ... --base-index ... --function <fn> --view diff --rva 0x<base-rva-of-this-overload>
```
The two-sided diff identifies each overload independently even though objdiff's report
cannot. Exact complete rich instruction streams are correctness-facing evidence and are
recorded by `vostok derive`; a non-exact overload remains without a fuzzy score rather than
borrowing the ambiguous bare-name score.

Evidence: sound_scene.cpp `fill_x3daudio_vector` x2 (both 100% via RVA diff) +
`closest_point_on_segment` / `compare_propagator_info_by_distance` — all byte/structure-
correct, all `unpaired` in report.json because the target obj lost their overload param
lists. Same delinker behaviour family as the D3D1x_HAL/Shader "exact-by-source" target-obj
skips.
