# `circular_buffer<T>` ring-buffer member shapes (`new_item`, dtor)
tags: cpp:template cpp:member cpp:new-delete | asm:div asm:je asm:call | topic:structure-shape topic:codegen-idiom
symptoms: lea x+x*2 then shl 5 (=*0x60 element stride), inc;xor edx,edx;div max_count remainder = next(), placement-new null-guard je on the slot pointer
confidence: 9/10

Engine ring buffer (`vostok/game_core/circular_buffer.h`): members are `T* m_history`
[0], `base_allocator& m_allocator` [4], `u32 m_max_count` [8], `u32 m_head` [0xC],
`u32 m_tail` [0x10]. `next(i) = (i+1) % m_max_count` compiles to `inc; xor edx,edx;
div m_max_count` (remainder in edx). The element-address `&m_history[i]` is
`lea reg,[i+i*2]; shl reg,5` (i*0x60 for a 0x60-byte T) `; add reg,[m_history]`.

`new_item()` — 4 body stmts, 0 named locals (the `result` reference elides):
```cpp
new ( &m_history[ m_head ] ) T( );      // slot ptr computed; je-skip ctor if NULL (placement-new guard)
T& result	= m_history[ m_head ];       // address kept in a reg, returned at the end
m_head		= next( m_head );
if ( m_head == m_tail )                   // buffer full -> drop oldest
	m_tail = next( m_tail );
return result;
```
The leading `add slot,[m_history]; je .skip; <T ctor calls>` is the standard
placement-`operator new` null guard (the address IS the placement operand, no
`operator new` call - the pointer is just `&m_history[m_head]`).

`~circular_buffer()` — 2 body stmts (drain loop + free), 100%:
```cpp
while ( !empty( ) )      // empty() == (m_head == m_tail); cmp m_head,m_tail; je .free
	pop_tail( );          // m_tail = next( m_tail ); loop while m_head != m_tail
VOSTOK_FREE_IMPL( m_allocator, m_history );  // free_helper: test ptr; call_free vtable[0x18]; m_history = 0
```
`VOSTOK_FREE_IMPL( m_allocator, ... )` (not `FREE`, which is g_allocator) frees
through the buffer's own `base_allocator&` member: the asm is `mov ecx,[m_allocator];
mov edx,[ecx]; push ptr; call [edx+0x18]` (= virtual `call_free`) then `mov [m_history],0`.

Inline-vs-call caveat: the compiler-generated `server_player_update` ctor (sub-objects
`player_input`+`weapon_state`) and `new_item` itself may be inlined by our LTCG where the
target keeps them out-of-line (target emits NO standalone `server_player_update` ctor) -
that residual is a whole-program inline-cost divergence, not source-steerable here.

Evidence: `~circular_buffer<client_player_history_item>` 100% (2/2 stmts);
`new_item` structure-faithful but LTCG-inlined into its sole caller.
