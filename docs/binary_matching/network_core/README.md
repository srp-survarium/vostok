# network_core - matching notes

- Namespace: `vostok::network_core`.
- Build: `/Od` (optimization off) + LTCG, `Master Gold` - see [../MATCHING.md](../MATCHING.md).
- Sources: `sources/vostok/network_core/sources/`.
- Reachability anchor: referenced from `game_core`'s `temp_include_all.cpp`
  (`use_network_core_*`, e.g. `use_network_core_http_client`) - see
  [../agentic_loop.md](../agentic_loop.md) section 3.
- Best reversed reference: `http_client.cpp`. Heavy Boost.Asio - expect the body
  to lean on asio / `boost::bind` shapes.

## Module-wide notes
Shared types, recurring gotchas, and asm quirks that span functions - add as you
find them.

## Where per-function history lives
NOT here - per-function `.md` trails were dropped (see
[../agentic_loop.md](../agentic_loop.md) section 7). Live status is in the
`STATE[...]` markers in the sources; the run narrative is in each match's
commit/PR message. This README holds module-wide notes only.
