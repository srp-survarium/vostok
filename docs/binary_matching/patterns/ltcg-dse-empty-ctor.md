# LTCG dead-store elimination: an anchored-only ctor compiles EMPTY
tags: cpp:ctor | topic:anchoring topic:codegen-idiom
symptoms: ctor body with no member stores, push ebp mov ebp esp push ecx mov [ebp-4] ecx mov eax leave ret
confidence: 7/10
variants: dced-static-helper-anchor.md

Under /Od + /GL (LTCG), a constant-only ctor whose ONLY caller is a synthetic
temp_include_all anchor gets every member store proven dead and emits an empty body.
Escaping `&params` to an opaque external (example_callback -> printf("%s")) does NOT
count as observing the float members. The body is right; the base bytes are
LTCG-emptied - PARTIAL until a real game caller is matched.

```asm
push ebp; mov ebp,esp; push ecx; mov [ebp-4],ecx; mov eax,[ebp-4]; leave; ret   ; init list gone
```
Real matched value-struct ctors (animation_analysis_result, 85.98% DONE) survive only because a real reachable caller observes them.
Evidence: game_core/weapon_recoil_params::weapon_recoil_params() (18.18%).
