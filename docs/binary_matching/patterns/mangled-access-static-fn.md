# Static member function access codes after @@: C=private, K=protected, S=public
tags: cpp:static cpp:member | topic:mangling
symptoms: @@CAI, @@KAI, @@SAI, scores None for a static member in the base obj
confidence: 9/10
variants: mangled-access-instance.md, mangled-access-virtual.md, mangled-access-static-data.md, mangled-join-failure.md

Same pairing-failure class as the instance Q/A/I letters, but for STATIC member
functions: the char after `@@` is C=private static, K=protected static, S=public static
(then `A`=__cdecl-ish, return type, args).

```cpp
protected:    // ?movement_animation_index@@KAI... - K = protected static
	static u32 movement_animation_index( ... );
```
Evidence: movement_animation_index - public gave S, private gave C, both scored None; `protected:` -> KAI == target -> 100%.
