# Legacy `survarium` game module implementation (parked)

The pre-rebuild contents of `sources/vostok/game/sources/` (minus the keeper
infra files: `game.vcproj`, `pch.{h,cpp}`, `game_memory.h`,
`game_entry_point.cpp`, `game_library_linkage.cpp`), moved here verbatim during
the canonical carcass rebuild of the module (see
`docs/binary_matching/library_carcass_rebuild.md`).

This is the 2011/2012-era GSC game code. The shipped game evolved it heavily
(hud/menu/inventory/match flow rewritten, the simulation core split out into
`game_core`), so much of it has no counterpart in the original PDB. But unlike
the network legacy, a large share of type/file names DID survive into the
shipped build, so these files are useful as body references when matchers fill
in the rebuilt carcass - treat them as priors, not verified matches.

The rebuild work-list lives in `temp/structure_queue/` (canonical headers +
compiland carcasses; a file is deleted from the queue when its real counterpart
is reproduced in the tree). `temp/triage_log.md` records which `survarium::`
headers were excluded from the queue because the type lives in `game_core` /
elsewhere.

`game_net_client.{h,cpp}` here are the game-side wiring of the parked legacy
network client (see `temp/network_legacy/README.md`); the shipped game talks to
`vostok::network` (`login_client`/`match_client`) instead.

Nothing here is compiled. Delete this directory once the game module is
matched.
