# Recover an INLINE ctor body from its consumer expansions (never guess NULL)
tags: cpp:ctor cpp:inline cpp:init-list | asm:mov asm:call | topic:inline-vs-call
symptoms: header-inline ctor no standalone symbol, mov [reader+4] eax after folded buffer() call
confidence: 8/10

A header-inline ctor with no standalone symbol leaves its body unrecorded - but every /Od
consumer EXPANDS it, so the consumers are ground truth: rich-view 1-2 consumers and read
the expansion between the member stores before banking an init list. A folded `call`
feeding a member store means a real getter init, not NULL (a NULL init would be a plain
`mov [member], 0`).

```cpp
packet_reader::packet_reader( base_packet const& packet )
	: m_packet( packet ), m_pointer( packet.buffer( ) ) { }   // NOT m_pointer( NULL )
```
```asm
mov [reader+0], &packet
lea/call <folded base_packet::buffer() const>
mov [reader+4], eax
```
Evidence: udp_match_connection::is_low_level_packet stmt 1 + process_incoming_packet L141; the fix moved udp_match_client::process_incoming_packet 88.12 -> 99.86.
