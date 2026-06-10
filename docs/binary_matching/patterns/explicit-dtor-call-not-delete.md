# Destruct-without-freeing helper = `obj.~T()`, NOT `delete &obj`
tags: cpp:dtor cpp:new-delete | asm:call | topic:allocator
symptoms: ~0x2c body vs ~0x50 with inlined operator delete, call_destructor helper
confidence: 8/10
variants: virtual-dtor-explicit-call.md, allocator-new-delete-helpers.md

A helper that runs an object's destructor but leaves the storage (owned by an allocator)
is an EXPLICIT dtor call. `delete &obj;` compiles to the dtor PLUS an inlined
`operator delete` (~0x50 bytes vs the target's ~0x2c).

```cpp
packet.~udp_match_packet( );   // NOT: delete &packet;
```
Evidence: udp_match_packet::helper::call_destructor 43.2 -> 100.0 switching delete to the explicit dtor call.
