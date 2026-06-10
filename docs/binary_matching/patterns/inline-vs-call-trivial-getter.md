# Trivial header getter: target `call`s it, our /GL inlines the member read (wall)
tags: cpp:inline cpp:member cpp:bool | asm:call asm:mov | topic:inline-vs-call
symptoms: call is_aimed vs mov al [obj+488h], getter standalone in target index only, no inline keyword
confidence: 7/10
variants: inline-vs-call-template-comdat.md, outline-accessor-to-recover-call.md, two-accessors-one-and-split.md, accessor-call-result-temp.md

A one-line header accessor (`{ return m_member; }`) emits a real `call` in TARGET but is
inlined in BASE to a direct member read. Whether a trivial COMDAT inline member is
emitted-and-called or inlined is a whole-program /GL decision, not steerable from the
caller; the inline also shifts frame/slots. Match the member access; mark PARTIAL.

```asm
; target: call survarium::weapon_core::is_aimed     ; base: mov al, [obj+488h]
```
Getters defined in the header WITHOUT the `inline` keyword are a recurring sub-case: target compiles per-TU (COMDAT standalone, every caller `call`s), our /GL inlines - out-lining them changes COMDAT placement and risks other matches; bank PARTIAL [LTCG getter inline-vs-call].
Evidence: dispersion_calculator::get_dispersion (is_aimed); set_animation_to_wait (get_user @0x9b330 vs `mov eax,[m_weapon+44Ch]`, 77.33%); weapon_core batch3: is_trying_to_aim 66.75, on_user_sprint 89.72, has_animation_ended predicates 85-87, is_ready_to_be_deactivated 84.77 (is_double_handed @+48A, has_animation_ended @+135).
