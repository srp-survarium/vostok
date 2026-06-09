# Legacy `vostok::network` implementation (parked)

The pre-rebuild contents of `sources/vostok/network/` (minus the keeper infra
files: `api.h`, `library_linkage.h`, `sources/pch.{h,cpp}`,
`sources/network_memory.h`, `sources/network_entry_point.cpp`,
`sources/network.vcproj`), moved here verbatim during the canonical carcass
rebuild of the module (see `docs/binary_matching/library_carcass_rebuild.md`).

This is the 2012-era GSC client/server/packet web (`client`, `server`,
`packet`, `packet_socket`, `io_service`, readers/writers). The shipped game
replaced it with `login_client`/`match_client` + `network_core::tcp_packet`,
so none of these types exist in the original PDB - they cannot be matched and
were dropped from the build graph.

Why parked instead of deleted: the orders/responses/channel lineage survived
into the shipped code (the legacy versions were producing partial matches:
`functor_response.h` 100%, `network_entry_point.cpp` 74.8%, `send_order.h`
68.9%, ...), so these files remain useful as body references when matchers
fill in the rebuilt carcass. Game-side consumers (`game_world.h` had a value
member `network::client m_client`) were commented out and must eventually be
redone against the canonical API - "deal with them later".

Nothing here is compiled. Delete this directory once the network module is
matched and the game/engine consumers are reworked.
