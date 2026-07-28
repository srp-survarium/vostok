# Constructed-argument temp: inline `T(NULL)` (0 locals, 1 stmt) vs named `T tmp; f(tmp)` (+1 local, ctor-first)
tags: cpp:ctor cpp:local cpp:temporary | asm:push asm:call | topic:source-fidelity topic:arg-eval-order
symptoms: target 0 named locals + 1 statement, a constructed temp argument, SIZE -0x3 ctor reschedule, named-local form is byte-100 but 2 statements
confidence: 7/10
variants: array-walk-zero-locals-is-for-each.md

A call passing a constructed temporary: `f( a, b, T( NULL ) )`. The target records 0 named
locals and ONE statement. A named local `T tmp( NULL ); f( a, b, tmp );` matches the target's
ctor-BEFORE-arg-push order (byte-100%) but is +1 local AND a 2-statement structure - WRONG on
both. The inline temp is 0 locals + 1 statement (correct structure), but MSVC's right-to-left
arg evaluation pushes `this`/later args before constructing the inline temp - a ctor
reschedule (SIZE -0x3) that no 1-statement spelling fixes. Take the inline form: the
1-stmt/0-local STRUCTURE beats the ctor-order bytes (recoverable later).

```cpp
f( a, b, resources::managed_resource_ptr( NULL ) );   // inline temp: 0 locals, 1 statement
```
Steerable to the correct STRUCTURE (not to 100% bytes). Evidence:
`weapon_core::set_animation_callback` x2, 1->0 local, 100 -> 81/80% with only `SIZE -0x3`
(ctor scheduling) remaining; structure 1 stmt + 0 locals == target (#372).
