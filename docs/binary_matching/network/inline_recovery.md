<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Network inline-body audit

PR 569 contained three inert packet-allocation stubs in `network::match_client`.
All three are now restored through the allocators already owned by the class.
They have no standalone procedures in the client target, so the evidence is the
surviving ownership graph and the corresponding live allocation paths rather
than a callable symbol.

| Declaration | Recovered behavior | Evidence |
|---|---|---|
| `match_client::delete_packet` | destroy through `*m_order_packets_allocator` | `new_packet` and the ordering connection use the order-packet allocator; `udp_match_packet` is allocator-owned |
| `match_client::new_response_packet` | allocate through `*m_response_packets_allocator` | `create_responses_packets_allocator` establishes the distinct response-packet pool and response handlers consume packets from it |
| `match_client::delete_response_packet` | destroy through `*m_response_packets_allocator` | inverse of `new_response_packet`; using the order allocator would violate the pool ownership encoded by the two members |

No `/* no source */`, `STATE[STUB]`, or fake constant-return body remains in
`sources/vostok/network` from this audit.
