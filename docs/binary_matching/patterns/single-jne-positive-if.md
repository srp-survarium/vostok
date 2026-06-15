# A single `jne` straight over a call = positive `if ( !x ) f( );`, NOT `if ( x ) return;`
tags: cpp:if cpp:return | asm:jcc asm:jmp | topic:structure-shape
symptoms: one jne over the guarded call vs je + jmp pair, 2-byte one-jump delta
confidence: 8/10
variants: inverted-guard-early-return.md

For a guard whose taken branch lands on the function end, `if ( is_disconnected( ) )
return; f( );` emits `je <over-the-return>; jmp <end>` (two jumps), while a single
`jne <end>` straight over the call is the no-early-return spelling. The mirror of
inverted-guard-early-return.md: count the jumps to pick the spelling.

```cpp
if ( !is_disconnected( ) )
	send_queued_packets_impl ( );    // jne <end> straight over this call
```
Evidence: match_client_impl::send_queued_packets 91.6 -> 100 (header COMDAT 0x0dad60).
