# +0xC frame, byte-equal code = ELIDED named-return temp; the spelling is proven where the copy SURVIVES
tags: cpp:return cpp:local cpp:template | asm:sub-esp | topic:convention topic:inline-vs-call
symptoms: frame exactly +0xC, instruction stream identical, only bottom-of-frame disp constants shift
confidence: 5/10
variants: od-frame-slot-noise.md

A target frame exactly +0xC over base with an otherwise identical stream is NOT a missing
ASSERT (that adds ~0xc of CODE) - it is a dead 12-byte temp the front-end materialized
and the backend elided code-free: the original used a NAMED return value. The named local
reserves its slot at EVERY inline emission; MSVC8 LTCG elides the return copy
per-emission, so one emission can keep the copy (byte-proof of the spelling) while others
keep only the slot.

```cpp
custom_alloc_handler< H > const result( a, h );
return result;            // NOT: return custom_alloc_handler< H >( a, h );
```
Evidence: make_custom_alloc_handler - copy kept in tcp_packet_client::start_reading (100.00% only with the named spelling); elided (slot kept) in the standalone udp/tcp start_receiving COMDATs where our LTCG does not elide (+0x12 residual, 99.8 -> 87-91). Keep the byte-proven spelling; book other emissions as backend copy-prop variance.
evidence-basis: one byte-proven emission; same spelling regresses sibling emissions
