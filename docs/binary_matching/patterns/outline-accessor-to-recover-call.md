# Target calls a trivial accessor our header inlines: move the body to the .cpp (steerable)
tags: cpp:inline cpp:member | asm:call asm:sub-esp | topic:inline-vs-call
symptoms: call accessor in target vs inlined mov ax [reg+off] in base, sub esp differs ~4, register cascade
confidence: 9/10
variants: inline-vs-call-trivial-getter.md, outline-empty-virtual.md

When the target emits `call accessor` for a trivial accessor defined IN the class body
(implicitly inline) and the accessor is a REAL standalone target symbol (pdb_rich_query
--list), declare it in the header and define it in the .cpp - /GL then stops inlining it
at call sites. Not one instruction: the inlined temp grows the frame and cascades the
whole function's register allocation.

```cpp
// header: u32 ammo_in_magazine( ) const;      .cpp: u32 weapon_core::ammo_in_magazine( ) const { return m_ammo_in_magazine; }
```
Evidence: out-lining ammo_in_magazine took pistol_/double_barreled get_weapon_lexeme_pair 77/92% -> 99.92%; fire_queue_length out-lined produced the two `call fire_queue_length` the target has (inline-in-header had inlined the array read @66%); same as the get_magazine_capacity NOTEs.
