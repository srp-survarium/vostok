# A redundant `static_cast_resource_ptr` value-loads the source where a DIRECT pass copy-constructs it (drop the cast when the param type is the same underlying resource_ptr)

tags: cpp:cast cpp:template cpp:member | asm:mov asm:lea asm:call | topic:inline-vs-call topic:convention
symptoms: passing a member resource_ptr to a by-value param, base does `mov eax,[this+off]; push eax` (value-load) into the temp where the target does `lea edx,[this+off]; push edx` (address) before the resource_ptr copy/`::set`; the only diff is value-vs-address of the same member, ~43% over a structurally-correct call
confidence: 8/10
variants: static-cast-resource-ptr-copy.md, template-param-byvalue-vs-constref.md

When a facade method takes a resource_ptr BY VALUE (or by const-ref) and the argument is a
member resource_ptr, check whether the param's declared typedef and the member's typedef are
the SAME underlying `resource_ptr<object, base>`. Two differently-named typedefs (e.g.
`resources::unmanaged_resource_ptr` and `render::particle::particle_system_instance_ptr`) are
frequently the identical type (`resource_ptr<unmanaged_resource, unmanaged_intrusive_base>`).
If so, `static_cast_resource_ptr< P >( m_member )` is REDUNDANT: the cast template returns its
arg as a prvalue, which the compiler materialises by VALUE-LOADING the member (`mov eax,[this+off]`)
and the by-value param is then constructed from that prvalue. The target, passing the member
DIRECTLY, copy-constructs the by-value param from the member's ADDRESS (`lea edx,[this+off]`;
the resource_ptr copy-ctor / `::set` reads through it). Same statement, different operand
class -> a low score over a correct structure.

Fix: drop the cast and pass the member directly.

```cpp
// 43.6% - cast value-loads the member into the prvalue:
play_particle_system( render_scene( ), static_cast_resource_ptr< P >( m_particle_fired ), m_transform );
// 100% - direct pass copy-constructs the by-value param from m_particle_fired's address:
play_particle_system( render_scene( ), m_particle_fired, m_transform );
```

Confirm the types are identical first (grep the two typedefs); only then is the cast removable.
This is the inverse of static-cast-resource-ptr-copy.md (there the cast is REQUIRED because the
source is a different-typed accessor result).

Evidence: booby_trap::play_fired_effects 43.6% -> 100% and the ~booby_trap dtor
remove_particle_system_instance call (game/booby_trap.cpp); `unmanaged_resource_ptr` ==
`render::particle::particle_system_instance_ptr` == `resource_ptr<unmanaged_resource,unmanaged_intrusive_base>`.
