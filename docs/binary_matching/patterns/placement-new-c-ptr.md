# Placement new on a mutable_buffer = `new ( buffer.c_ptr() ) T(...)`
tags: cpp:new-delete | asm:call asm:cmp asm:push | topic:codegen-idiom
symptoms: C2665 operator new overloads, call c_ptr misnamed operator*, post-new cmp [p] 0 je null guard
confidence: 8/10

`mutable_buffer` has NO conversion to void* (only operator bool), so `new ( buffer ) T`
fails C2665 - the standard form is `new ( buffer.c_ptr() ) T(...)`. The `call operator*`
in the asm is `mutable_buffer::c_ptr()` kept out-of-line (LTCG) - don't read it
literally. The post-`operator new` `cmp [p],0; je` is the standard placement-new null
guard.

```cpp
return new ( buffer.c_ptr( ) ) weapon_core_idle_state( params->weapon, animations, animations_count );
```
```asm
lea &buffer; call <c_ptr, misnamed operator*>; push; push sizeof(T); call operator new
cmp [p],0; je .skip; <T ctor>
```
Evidence: weapon_core_state_cook_template<weapon_core_idle_state>::new_object (100%).
