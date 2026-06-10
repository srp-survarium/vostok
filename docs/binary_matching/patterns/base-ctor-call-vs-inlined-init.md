# Derived ctor: base-ctor `call` vs inlined init is decided by the BASE class declaration
tags: cpp:ctor | asm:call asm:mov | topic:inline-vs-call topic:structure-shape
symptoms: call Base::Base one call no args, inlined grand-base ctor + mov dword ptr [eax],0 vtable store
confidence: 9/10
variants: state-ctor-vtable-stores.md, fold-misname-empty-fn.md

Whether a derived ctor emits one `call Base::Base` or inlines the grand-base init +
vtable store is decided by the base CLASS's declaration, not the derived body: a
user-declared out-of-line base ctor -> a real call; no declared ctor (compiler-generated
trivial init) -> MSVC inlines. Not steerable from the derived ctor's source.

```cpp
derived::derived( )    // base has an out-of-line ctor -> one call, no args
```
```asm
; target: mov esi,[ebp-4]; call survarium::interactive_object::interactive_object
; base build (base has NO declared ctor): push 1; mov ecx,[ebp-4]
;          call vostok::resources::unmanaged_resource::unmanaged_resource; mov dword ptr [eax],0
```
COROLLARY: a base with no declared ctor but a declared virtual gets an implicit ctor that can survive ICF-folded out-of-line and be `call`ed identically in BOTH binaries - check the base index before banking BLOCKED.
Evidence: inventory_item ctor (base interactive_object out-of-line @ target rva 0x9ccb0; base build inlines unmanaged_resource(1)); player_logic_base_state ctor 100% (ai::fsm_state implicit ctor folded, `call fsm_state::fsm_state` in both).
