# Inlined state getter: `cmp [m+off], K; sete` with NO temp slot = positive `return m_x == k;`
tags: cpp:bool cpp:inline cpp:return | asm:cmp asm:sete asm:movzx | topic:codegen-idiom
symptoms: xor ecx ecx cmp dword [eax+11Ch] K sete cl movzx test je, no [ebp-N] bool store
confidence: 8/10
variants: inlined-bool-helper-in-condition.md

The sete/movzx/test chain with no bool stack slot is an inlined POSITIVE one-liner
(`return m_state == connected;`). A NEGATED spelling (`if ( !has_disconnection_initiated() )`
over `return m_state != connected;`) emits setne + inverted jcc - wrong bytes. The compare
constant K identifies the getter (udp_match_connection::state: 0=connected ->
is_connected, 3=disconnected -> is_disconnected).

```cpp
if ( m_connection.is_connected( ) )   // getter: return m_state == connected;
```
```asm
mov eax,[this]; xor ecx,ecx
cmp dword ptr [eax+11Ch], 0
sete cl; movzx edx,cl; test edx,edx; je <else>
```
Evidence: udp_match_client::{enqueue,process_incoming_packet,send_queued_packets} (94-100%).
