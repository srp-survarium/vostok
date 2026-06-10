# Speculative ASSERT in an inline ctor multiplies into every inline site
tags: cpp:assert cpp:inline cpp:ctor | asm:call | topic:assert-eater
symptoms: eater bytes at every inlining consumer, ~0x10 per site, several consumers capped at once
confidence: 9/10
variants: assert-eater-recover.md

A header-inline ctor/method expands at every call site, so a guessed
`ASSERT( UNKNOWN_EXPRESSION )` in it emits the eater at EVERY consumer and silently caps
each one's %. Before adding an ASSERT to an inline body, check the target bytes at EACH
inline site (the expansion is delimited by the member-address temp reload before and the
member store after).

```cpp
inline handler_allocator::handler_allocator( )   // ASSERT here hits every inlining ctor
```
Evidence: network_core/handler_allocator() - both target sites (udp_match_connection ctor 0x205-0x219, udp_match_client ctor 0x191-0x1ad) show only `call <folded sub-ctor>; mov byte[this+400h],0`; removing the speculative ASSERT moved async_connector ctor 81.38->91.79, connection ctor 95.58->98.52, client ctor 77.47->79.20.
