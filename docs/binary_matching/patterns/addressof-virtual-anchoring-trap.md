# `&Class::virtual_member` emits a vtable thunk, NOT the body (anchoring trap)
tags: cpp:virtual | topic:anchoring
symptoms: zero Class::member symbols in obj despite anchor, C4716, LNK1257 on instantiating
confidence: 8/10
variants: pure-virtual-anchor-overrides.md, template-address-anchors.md

A pointer-to-member of a virtual is a vtable index/thunk - it does not ODR-use the body,
so the anchor emits NOTHING for it. Instantiating the class would emit the vtable ->
codegen of every still-STUB sibling virtual -> C4716/LNK1257. Force the body with a
QUALIFIED (devirtualized) call on a fabricated null object instead.

```cpp
Class& s = *reinterpret_cast< Class* >( NULL );
s.Class::member( );   // ODR-uses the exact body, never touches the vtable
```
Caveat: under /GL the linker may then INLINE the trivial one-liner into the anchor (no standalone body in the EXE; the target keeps it standalone only because it sits in a live vtable) - the source is still byte-correct; confirm by disassembling the anchor.
Evidence: jump_logic_state_{landing,start}::{execute,is_ready_for_transition}.
