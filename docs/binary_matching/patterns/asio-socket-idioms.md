# boost::asio socket open/bind/endpoint idioms (all match from plain source)
tags: cpp:member cpp:ctor | asm:cmp asm:lea asm:rep-movsd | topic:codegen-idiom
symptoms: cmp dword[socket+4] -1 setne, mov dword[ebp-4] 2 protocol temp, zero 7 dwords endpoint, sub esp 51Ch gap
confidence: 8/10

The plain asio source reproduces byte-perfect: `is_open()` inlines to
`cmp dword[socket+4],-1; setne` (impl.socket_ != invalid_socket); `open(udp::v4())`
materializes the protocol temp (`mov dword[ebp-4],2; lea/push; call basic_socket::open`);
`bind(udp::endpoint())` INLINES the default endpoint ctor (zero 7 dwords = 0x1c union,
family=2 word, port=0 word, addr=0 dword); `m_server_endpoint = udp::endpoint(
address::from_string(host), port )` = sret from_string + out-of-line detail::endpoint
ctor + `rep movsd` 7 dwords into the member. The huge /Od frame (sub esp,51Ch with a
~0x4b4 unused gap) reproduces by itself - don't chase it.

Evidence: network_core/udp_match_client::connect (100% first build).

The protocol temporary matters for DNS queries too. In retail
`async_connector::connect(socket,host,port,...)` at RVA `0x5458a0`, the query
statement writes `2` into a protocol temporary and passes its address before
calling the four-argument resolver-query constructor. This is
`query(tcp::v4(), host, port)`, not `query(host, port)`: the latter's overload
sets `PF_UNSPEC`, whereas the former uses the given protocol's family.
The default `address_configured` flags (`0x400`) are unchanged. Inspect the
extra argument, not merely the demangled constructor family name.
