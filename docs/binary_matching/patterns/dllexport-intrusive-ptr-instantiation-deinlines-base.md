# An API-exported `template class intrusive_ptr<...>` de-inlines c_ptr/operator=/refcount in the BASE only - target inlines, base CALLs
tags: cpp:template cpp:member | asm:call asm:inc asm:dec | topic:codegen-idiom topic:inlining topic:residual
symptoms: a faithful body over an intrusive_ptr member pairs only ~25-70%; the BASE asm has `call ...intrusive_ptr<...>::c_ptr`, `call ...::operator=`, or an out-of-line `operator unspecified_bool_type`, while the TARGET inlines the same op (`cmp [reg+off],0` for the safe-bool, raw `mov [reg]` for c_ptr, an inline `inc [obj+10h]` / `add [obj+10h],-1` refcount pair for the assignment); structure-diff shows STRUCTURE MATCH or tiny SIZE deltas (the statements/order are right), the residual is purely the de-inlined helper
confidence: 8/10
variants: anchor-construct-call-volatile-args.md, intrusive-ptr-byvalue-bool-chain.md, icf-fold-names-noise.md

A header that declares `template class VOSTOK_<MODULE>_API intrusive_ptr< T, Base, policy >;`
(an explicit instantiation marked dllexport - e.g. in mixing_binary_tree_animation_node.h
for `binary_tree_animation_node_ptr`) forces the smart-ptr's member functions to be emitted
as exported out-of-line symbols. In the BASE build those exported copies are then *called*
from consumers instead of inlined, so:

- `if ( node.m_ptr_field )` (safe-bool) compiles to `lea ecx,[obj+off]; call ::operator T*`
  / `::c_ptr` in the base, but the TARGET inlines it to `cmp [obj+off],0; je ...; mov edx,c_ptr; test edx; je` (the `operator unspecified_bool_type` body, inlined).
- a loop step `i = i->m_ptr_field.c_ptr()` is `call ::c_ptr` in the base, `mov eax,[i+off]`
  (raw field read) in the target.
- a field assignment `p->m_ptr_field = raw_ptr` is `call ::operator=` (172-byte symbol) in the
  base; the target inlines the full `self_type(object).swap(*this)` -> `inc [new+10h]` /
  `mov old; add [old+10h],-1; jne; call dtor` refcount sequence (often ~0x28 bytes vs the
  base's ~0xc plain store when the base additionally proves the old value null and elides the
  dec).

This is a TARGET-vs-BASE inlining characteristic of the dllexported instantiation, NOT a source
structure problem - the statement set, order, locals, and member references are already correct
(confirm with `--view structure-diff`: STRUCTURE MATCH or only `SIZE` deltas, no TRGT/BASE_ONLY
once the body is faithful). Do NOT contort the source (no hand-inlined c_ptr, no raw-pointer
member access to dodge the safe-bool) to recover the bytes - that would be unfaithful. Take the
% hit; the residual is recoverable only by changing the build's export/inline policy, which is
out of a single function's scope.

Evidence (animation n_ary_tree_weaver, all anchored via anchor_animation_weaver.cpp): every
sub-100% member is gated by this - is_unique_animation_lexeme STRUCTURE MATCH at 68% (safe-bool +
c_ptr de-inlined in the loop), join_animations 23% / update_weights 36% / visit(animation) 28%
(operator= refcount de-inlined + base refcount-elision), visit(addition/subtraction/multiplication)
65-73% (inlined propagate carries the same de-inlined intrusive_ptr ops). The 7 members touching
no intrusive_ptr op (ctor, add_interpolator, 4x clean<T>, visit(weight_node)) pair at 100%.
