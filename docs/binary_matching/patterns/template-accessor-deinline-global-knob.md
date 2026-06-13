# De-inlining a class-template accessor (`intrusive_list::front`) is a GLOBAL knob; the target mixes inline-and-call per instantiation

tags: cpp:template cpp:inline cpp:member | asm:call asm:mov | topic:inline-vs-call topic:convention
symptoms: call <bare ret> then call <mov eax,[eax+8]; ret> with this in EAX, fsm states front family, sibling TUs stuck 45-94% on the same statement, regressions in OTHER modules after the flip
confidence: 7/10
variants: outline-accessor-to-recover-call.md, eax-this-convention.md, single-tu-anchor-inlines-helper.md

`m_logic->states( ).front( )` compiles in the target to TWO LTCG custom-convention calls
(`this` in EAX): `fsm::states()` (non-const overload; identity over `m_states` at offset 0
-> a bare `ret`, ICF-fold-misnamed) then `intrusive_list<fsm_state,...>::front()`
(`mov eax,[eax+8]; ret`). Both are REAL standalone target symbols. Steering:
- `fsm::states()` non-const: declare in fsm.h, define in ai/sources/fsm.cpp (pattern 69).
  The CONST overload stays inline - const call sites (selector::serialize) show NO call.
- `front()`: remove the `inline` keyword from the template decl+def
  (intrusive_list.h/_inline.h). Under /Ob2+LTCG the keyword biases per-site inlining;
  removing it made every front() site a call.
CAVEAT - the knob is GLOBAL but the target is MIXED per instantiation/site: the
fsm_state list front() is out-of-line everywhere, yet the ai-planning/sound/particle
lists' front() is INLINED in their (previously 100%) consumers. The flip traded ~60
functions at 93-97% (sound/particle/ai/vfs front sites) for the whole fsm-consumer
family (jump_logic 6 unpaired -> 100%, deactivate 45->93, set_user 75->96.6) - net
+0.27% overall, +53 exact. The per-site choice is whole-program LTCG budget, NOT
expressible in source; whichever spelling is chosen, the minority sites stay residual.
Collateral: the budget shift can flip OTHER inline-splits in the same function (the
free-fn `boost::bind<bool>` function0 assign_to collapsed from the generic
basic_vtable0 shape to a fused specialized call in initialize_logic, 100% -> 89.7 on
that statement).

```asm
; target call site (jump_logic::deactivate):
mov eax,[eax+8]      ; this->m_logic  (arg in EAX, custom conv)
call <ret>           ; fsm::states()  - identity, fold-misnamed finalize_impl
call <mov eax,[eax+8]; ret>  ; front() - fold-misnamed list3::operator[]
push eax
```
Evidence: jump_logic.cpp 2026-06-13 - deactivate 45.1->93.0, set_user 74.9->96.6,
initialize_logic unpaired->89.7 (sole stmt diffs = EAX-vs-ECX this convention +
the assign_to collapse); breath/shotgun/selector siblings moved toward target.

Same knob, packet_reader: hit_info::deserialize (game_core/hit_initiator.cpp) matches the
target STRUCTURE 1:1 (9 stmts, same lines) but stays objdiff-unpaired on size (base 0x117
vs target 0x88 bytes) because the base INLINES packet_reader::r<bool>/r<float>/
r_string<16> at every call while the target keeps them OUT-OF-LINE (real target symbols
??$r@_N@, ??$r@M@, ??$r_string@$0BA@@; base index has NO such instantiations). De-inlining
them = removing `inline` from the template decls in the already-matched
packet_reader_inline.h - a global cross-unit flip touching every read site engine-wide,
off-limits from a single consumer TU. The bigger base frame (sub esp 98h vs 58h) and each
statement's +0xN size all follow from the inlining; not steerable from hit_initiator.cpp.
