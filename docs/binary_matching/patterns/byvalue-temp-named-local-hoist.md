# By-value temp built BEFORE the trailing arg pushes: hoist it to a NAMED local
tags: cpp:local cpp:ctor | asm:push asm:lea asm:call | topic:structure-shape
symptoms: temp ctor before push this in target, inline rvalue pushes this first in base
confidence: 9/10
variants: cxyz-reference-hoist.md, chained-temporary-params-object.md

When the target materializes a by-value argument temp FIRST (`push 0; call ctor` writing
[ebp-4]) and only THEN pushes the remaining args (recomputing &temp via lea), but the
inline-rvalue spelling reorders (pushes this first, then builds the temp), the cause is
temp-scheduling - not LTCG. A named local declared on the line above pins the ctor ahead
of the pushes; its dtor still fires at end-of-scope matching the target.

```cpp
managed_resource_ptr tmp( NULL );
set_animation_callback( ..., tmp, ... );    // not the inline rvalue
```
Evidence: weapon_core::set_animation_callback both overloads - inline temp 80.52/81.17% -> named local 100/100.
