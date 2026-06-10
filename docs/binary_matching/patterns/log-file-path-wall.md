# LOG_* __FILE__ residual is permanent on logging-heavy functions (path differs)
tags: cpp:macro | asm:push | topic:logging
symptoms: push path-string never matches, C:\survarium\sources vs Z:\home, functions capped 40-52%
confidence: 9/10
variants: log-line-verbosity-immediates.md

Every LOG_* bakes `__FILE__` into the call. The target's is `C:\survarium\sources\
vostok\...`; our Wine base build is `Z:\home\...`, so the `push <path-string>` never
matches, capping logging-dominated functions well below 100 even with control flow fully
matched. Not source-steerable. EXCEPTION: network_core's `__FILE__` is the RELATIVE
`.\udp_match_client.cpp` and DOES match - the cap does not apply there.

Evidence: tcp_packet_socket::on_packet_received/on_packet_size_received ~40%, on_packet_has_been_sent ~52%; http_client's LOG lines (same wall).
