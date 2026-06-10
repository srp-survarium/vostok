# Qualified call to an EMPTY base virtual: target keeps the call, base inlines the {} away
tags: cpp:virtual cpp:inline | asm:call | topic:inline-vs-call
symptoms: + call Base::execute only on target side, ~3 bytes, empty body defined inline in header
confidence: 4/10
variants: outline-accessor-to-recover-call.md, inline-vs-call-trivial-getter.md

A derived override calling the empty base implementation (`Base::execute();` where the
base defines `{}` inline in the header) shows target `call Base::execute` while /GL base
inlines the no-op away. Both rich indexes can still list a standalone Base::execute -
a PER-call-site whole-program inline decision.

```cpp
// header: decl only;  .cpp: void weapon_core_base_state::execute( ) { }
weapon_core_base_state::execute( );   // qualified call from the derived override
```
```asm
mov ecx,[ebp-4]; call survarium::weapon_core_base_state::execute   ; target only
```
STEERABLE in the single-call-site case: out-lining the empty `{}` to the .cpp took weapon_core_fire_state_base::execute 80.91 -> 99.09 - but it is regression-free ONLY with a single qualified call site (else it changes other derived classes' codegen); a later sweep banked the same function PARTIAL rather than out-line. Filling the body with real bytes is always worse.
Evidence: weapon_core_fire_state_base::execute (both outcomes recorded); animation_playback_state::reset() in aimed_state_base::finalize (same class).
evidence-basis: contradictory outcomes (out-lined 99.09 vs later sweep banked PARTIAL)
