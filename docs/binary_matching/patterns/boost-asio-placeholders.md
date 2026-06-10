# asio completion binds use boost::asio::placeholders::error/bytes_transferred, not _1/_2
tags: cpp:template | asm:mov asm:push asm:movzx | topic:codegen-idiom
symptoms: mov eax [?error@...placeholders@asio@boost@@] vs movzx ecx byte[_1], ~94% head divergence
confidence: 8/10
variants: boost-bind-cref.md

An asio async_read/async_write completion bind scores ~94% with a head divergence: target
reads the placeholder OBJECT from a global (`?error@...placeholders@asio@boost@@`), base
uses boost's global `_1` literal - different placeholder objects, both compile.

```cpp
boost::bind( &on_xxx, this, boost::asio::placeholders::error,
             boost::asio::placeholders::bytes_transferred )   // ::iterator for resolvers
```
Member-callback binds that are NOT asio handlers (m_on_error = bind(&on_error, this, _1, _2)) correctly keep boost _1/_2.
Evidence: network_core/tcp_packet_socket::start_receiving (94 -> 99.88%).
