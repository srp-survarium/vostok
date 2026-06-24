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

Same knob, boost::function::operator(): the login-auth flow (network/login_client_impl_
sign_in/up/out.cpp) invokes a `boost::function4`/`function5` callback once per error/answer
arm (`callback( a, b, c, d )`). The TARGET keeps `function4<...>::operator()` OUT-OF-LINE -
a 5-byte `call` (real COMDAT symbols ??R?$function4@... present in the target index for the
login/lobby instantiations); the BASE INLINES the `if(empty()) throw; get_vtable()->invoker`
body at every site (function_template.hpp:761, no symbol of the kind in the base index).
Each inlined site is base +0x3d (4-arg) / +0x49 (5-arg) bigger, dragging the small handler
functions to 47-90% even though the structure is byte-shape-identical (t_stmts==b_stmts, zero
TRGT_ONLY/BASE_ONLY). De-inlining = strip the implicit-inline from `operator()` in boost
function_template.hpp - a global flip hitting every boost::function call in the engine,
off-limits from these three consumer TUs. Two companion residuals at the same wall: the
LOG_* macro's boost::function1 log-callback ctor scheduling (log-callback-ctor-schedule.md,
±a few bytes/LOG) and the bind-copy lowering at each async_write/handshake `)` (the function5
copy + boost::ref slot vs the target's folded-ctor schedule, +0x15/0x20). Evidence:
network/login_client_impl_sign_in.cpp on_user_name_answer_received 47.1% (4 callback sites),
26/27..16/16 STRUCTURE MATCH across all 19 handlers (2026-06-15).

Confirmed casualty, ai-planning: action_instance.cpp (2026-06-24). The DebugPolicy-typedef
fix below paired the 4 filter functions, but two of them sit on the INLINED-front minority of
this knob: action_instance::is_object_suitable 95.5% and parameters_suitability_checker_predicate
::operator() 95.8% each have a single `for(...; it = front(); ...)` loop-init statement that is
STRUCTURE MATCH except `SIZE +0x2` - the target inlines `intrusive_list<intrusive_list_item<
base_filter*>,...,no_debug_policy>::front()` (`mov ecx,[eax+8]`) while the base CALLs the standalone
(the knob keeps front out-of-line for the 16 fsm-style instantiations). Flipping would regress the
fsm family per the CAVEAT above; not steerable from action_instance.cpp - PARK at the ceiling.

PREREQUISITE that gated all four (separate from the front knob): the planning filter intrusive_list
typedefs (base_filter.h parameter_filters_type/parameters_filters_type/subfilters_type, action_instance.h
filters_set_type) defaulted DebugPolicy to `debug_policy`, but MASTER_GOLD targets instantiate every
intrusive_list with `no_debug_policy` (target 273 no_debug : 1 debug; base was 222 debug : 76 no_debug -
our intrusive_list.h default is wrong for MASTER_GOLD). The two policies are empty (layout-identical), so
this changed only the MANGLED NAME, leaving the 4 functions TARGET_ONLY/BASE_ONLY (unpaired) despite
correct bytes. Spelling `no_debug_policy` explicitly as the 6th template arg in the planning typedefs
paired them (pop_filter_list/add_filters_list to 100/99.85, the two front sites to 95.5/95.8) with ZERO
regressions (no paired AI fn used a `debug_policy` intrusive_list name). The faithful global fix is the
intrusive_list.h default, but that is a core cross-module header touched by many workers - out of a
single planning TU's scope; explicit per-typedef args produce identical bytes/structure.
