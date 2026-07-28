# Mid-body call to a stripped/folded empty callee with a REAL source line (unmatchable)
tags: cpp:assert | asm:call asm:lea | topic:fold-icf topic:inline-vs-call
symptoms: mov byte[ebp-1],0 lea eax call mid-function, callee is a bare ret, arg in EAX no push, no symbol in either index
confidence: 4/10
variants: assert-eater-recover.md, fold-misname-empty-fn.md

The prologue-eater shape appearing MID-function with a genuine source-line number: a call
to a helper that is empty/stripped/COMDAT-folded whole-program. NOT reproducible from the
function's own source - match every other statement, mark PARTIAL. Verify the class by:
(1) the call target disassembles to a bare `ret`; (2) the arg register/convention matches
no writable C++ convention; (3) pdb_rich_query finds no standalone symbol in target OR base.

```asm
mov byte[ebp-1],0; lea eax,[ebp-1]; call <fn>   ; callee = single ret, misnamed finalize_impl
```
Evidence: weapon_core_show_state_base::{initialize (L28, 75.13%), on_animation_end_impl (L42, 69.93%)} - every other statement byte-exact.
