# CRTP packet<T> writer/reader need `friend` access to base_packet's private buffer
tags: cpp:template cpp:member | topic:structure-shape
symptoms: private m_buffer/m_buffer_size access errors from packet/packet_reader/tcp_packet
confidence: 8/10

base_packet keeps m_buffer/m_buffer_size private with only getters (const buffer() stays
private - PDB-matched). The CRTP writer packet<T>, the reader packet_reader, and
tcp_packet (reallocate) all touch those privates - befriend exactly those three (no
layout change, STATIC_SIZE_ASSERT stays 0x8).

```cpp
template < typename T > friend class packet;
friend class packet_reader;
friend class tcp_packet;
```
reallocate lives in tcp_packet::reallocate (allocator + 3-byte length-prefix dance, mirrors the old non-CRTP network::packet::reallocate); packet<T>::reallocate forwards implementation().reallocate(n) and COMDAT-folds onto packet<tcp_packet>::reallocate. append/reserve/resize/allocated_size reach T-specifics purely through implementation() so udp_match_packet (empty/int3 reallocate stub) still compiles.
