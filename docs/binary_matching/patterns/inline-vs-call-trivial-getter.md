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

CASCADE sub-case (severe %, structure-faithful source still right): when the inlined accessor returns an object that the statement immediately CONSUMES (e.g. `m_inventory->holder().damage_model()` returning a `resource_ptr`, or `container.size()`/`container[i]`), inlining the accessor cascades into inlining the WHOLE downstream machinery - the resource_ptr copy ctor (`dec`/`interlocked_increment` instead of an out-of-line `intrusive_ptr` ctor), `operator*`, `~intrusive_ptr`, even `math::min`/`vectora::size`/`vector::operator[]`. Target keeps the accessor out-of-line so the smart-ptr local is a real `intrusive_ptr` and every deref is a real `operator*` call. You CANNOT recover this from the consuming TU. Still write the faithful source the target's asm proves: deref the smart-ptr local via `( *dm ).member(...)` (operator*) NOT `dm->member(...)` (operator->) - the target calls `intrusive_ptr<...>::operator*`; `dm->` would be operator-> and is wrong even though both read 12-15% while the holder() inline masks it. Confirm the accessor is in the TARGET rich index but ABSENT from the base index (proof our build never emits it standalone), note + park.
Evidence: dispersion_calculator::get_dispersion (is_aimed); set_animation_to_wait (get_user @0x9b330 vs `mov eax,[m_weapon+44Ch]`, 77.33%); weapon_core batch3: is_trying_to_aim 66.75, on_user_sprint 89.72, has_animation_ended predicates 85-87, is_ready_to_be_deactivated 84.77 (is_double_handed @+48A, has_animation_ended @+135). game_core items: artefact_lifebone_core activate_impl 12.4 / switch_passive_mode_impl 46.3, medkit/oxygen_tank set_active/active_tick/remove_affects (all inventory::holder()+inventory_holder::scheduler(), both target-only standalone); scheduler::on_frame(u32) 46.4 (vectora::size + vector::operator[] target-only), on_frame(record&) 82.2 (math::min(s32) + boost::function copy ctor's assign_to_own target-only).
