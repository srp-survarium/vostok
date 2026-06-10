# Static DATA member access codes after @@: 0=private, 1=protected, 2=public
tags: cpp:static cpp:member | asm:mov | topic:mangling
symptoms: @@0V, @@1V, @@2V, @@2PAV, @@0IA, mov ecx ?m_x@ reloc differs
confidence: 9/10
variants: mangled-access-instance.md, mangled-access-static-fn.md, mangled-join-failure.md

For static DATA members the char right after `@@` is the access - 0=private,
1=protected, 2=public (then the type: PAV=pointer, V=class by value, IA=u32). objdiff
still pairs the functions (same name) but the relocation symbol differs, costing match %.
A class can interleave `private:`/`public:` to give two statics different access while
keeping declaration order.

```cpp
public:      // target reads @@2V - public
	static vectora< ... > m_hit_types_strings;
private:
	static vectora< ... > m_hit_types;          // @@0V both sides
```
Evidence: damage_model_cook::on_hit_params_received - m_hit_types_strings was target @@2V vs our private @@0V; a `public:` before it fixed the symbol.
