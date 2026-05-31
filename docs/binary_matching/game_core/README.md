# game_core - matching notes

- Namespace: `survarium` (the pch does `using namespace vostok;`).
- Build: `/Od` (optimization off) + LTCG, `Master Gold` - see [../MATCHING.md](../MATCHING.md).
- Sources: `sources/vostok/game_core/sources/`.
- Reachability anchor: every matched function must be referenced from
  `temp_include_all.cpp` (a `use_*` driven by `IncludeAll`, instantiated in
  `game/sources/game_world.cpp`) or it is dead-stripped - see
  [../agentic_loop.md](../agentic_loop.md) section 3.
- Best reversed references: `player_stamina.cpp`, `collision_sensor.cpp`,
  `damage_model.cpp`, `bullet.cpp`.

## Module-wide notes
Shared types, recurring gotchas, and asm quirks that span functions - add as you
find them.

## Per-function logs
One `<function>.md` in this folder per function that needed real effort (see
[../agentic_loop.md](../agentic_loop.md) section 7). Live status is in the
`STATE[...]` markers in the sources; these files hold the deeper write-ups.
