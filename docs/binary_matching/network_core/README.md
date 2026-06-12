# network_core - matching notes

- Namespace: `vostok::network_core`.
- Build: `/Od` (optimization off) + LTCG, `Master Gold` - see [../MATCHING.md](../MATCHING.md).
- Sources: `sources/vostok/network_core/sources/`.
- Reachability anchor: referenced from the game module's `temp_include_all.cpp`
  (under game_core until the game carcass rebuild)
  (`use_network_core_*`, e.g. `use_network_core_http_client`) - see
  [../agentic_loop.md](../agentic_loop.md) section 3.
- Best reversed reference: `http_client.cpp`. Heavy Boost.Asio - expect the body
  to lean on asio / `boost::bind` shapes.

## Module-wide notes
Shared types, recurring gotchas, and asm quirks that span functions - add as you
find them.

## Per-function logs
One `<function>.md` in this folder per function that needed real effort (see
[../agentic_loop.md](../agentic_loop.md) section 7). Live status is in the
`STATE[...]` markers in the sources; these files hold the deeper write-ups.
