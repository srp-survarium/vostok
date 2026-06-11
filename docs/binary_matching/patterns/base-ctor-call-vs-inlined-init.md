# Derived ctor: base-ctor `call` vs inlined init is decided by the BASE class declaration
tags: cpp:ctor | asm:call asm:mov | topic:inline-vs-call topic:structure-shape
symptoms: call Base::Base one call no args, inlined grand-base ctor + mov dword ptr [eax],0 vtable store
confidence: 8/10
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
COROLLARY 2: a base `ctor(){}`/dtor written IN-CLASS can BOTH inline into every derived ctor/dtor AND survive standalone (kept by another call site) - per-call-site LTCG. network::order's empty in-class `order(){}`/`~order(){}` show inlined in string/connect/send_order deriveds as `call <noncopyable ctor fold>` on this+4 (boost::noncopyable's declared `noncopyable(){}` stays an out-of-line folded call) + the `mov [this], ??_7order` vftable store, while standalone `order::order()` still exists at 0x49180; our /Od+LTCG base reproduced the inlining unprompted from the same in-class `{ }` (derived dtors 100%).
Evidence: inventory_item ctor (base interactive_object out-of-line @ target rva 0x9ccb0; base build inlines unmanaged_resource(1)); player_logic_base_state ctor 100% (ai::fsm_state implicit ctor folded, `call fsm_state::fsm_state` in both).
