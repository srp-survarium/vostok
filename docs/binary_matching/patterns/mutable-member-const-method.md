# A const method that assigns a member: the member is `mutable`
tags: cpp:const cpp:member | asm:call asm:mov | topic:structure-shape
symptoms: C2678 binary '=' const left-hand operand, const method storing into this+off, @@IBE mangling
confidence: 9/10

When a `... method() const` takes a member's address and mutates it (or stores into
`[this+0xNN]`), declare that member `mutable`. Do NOT drop the method's const - the
target's mangled name carries `B` (const this); removing it mis-mangles and scores None.

```cpp
mutable resources::managed_resource_ptr m_animation_to_wait_for;   // assigned in a const setter
```
```asm
mov ecx,[this]; add ecx,138h; call intrusive_ptr::operator=   ; inside a const (@@IBE) method
```
Evidence: weapon_core_animation_end_aware_state::set_animation_to_wait(...) const (mangles @@IBE) assigning m_animation_to_wait_for @0x138.
