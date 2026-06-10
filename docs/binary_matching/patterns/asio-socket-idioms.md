# boost::asio socket open/bind/endpoint idioms (all match from plain source)
tags: cpp:member cpp:ctor | asm:cmp asm:lea asm:rep-movsd | topic:codegen-idiom
symptoms: cmp dword[socket+4] -1 setne, mov dword[ebp-4] 2 protocol temp, zero 7 dwords endpoint, sub esp 51Ch gap
confidence: 9/10

The plain asio source reproduces byte-perfect: `is_open()` inlines to
`cmp dword[socket+4],-1; setne` (impl.socket_ != invalid_socket); `open(udp::v4())`
materializes the protocol temp (`mov dword[ebp-4],2; lea/push; call basic_socket::open`);
`bind(udp::endpoint())` INLINES the default endpoint ctor (zero 7 dwords = 0x1c union,
family=2 word, port=0 word, addr=0 dword); `m_server_endpoint = udp::endpoint(
address::from_string(host), port )` = sret from_string + out-of-line detail::endpoint
ctor + `rep movsd` 7 dwords into the member. The huge /Od frame (sub esp,51Ch with a
~0x4b4 unused gap) reproduces by itself - don't chase it.

Evidence: network_core/udp_match_client::connect (100% first build).
