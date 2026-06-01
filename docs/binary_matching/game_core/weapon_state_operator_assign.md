# game_core::weapon_state::operator=

Match `survarium::weapon_state& survarium::weapon_state::operator=(weapon_state const&)`.

- Source: `sources/vostok/game_core/sources/weapon_state.cpp`
- Branch: off `feature/agentic-matching-loop` (fresh independent class, no PR chain)

## Target

`weapon_state` is a 3-byte POD (`weapon_state.h`):
- `/* 0x0 */ u8 slot_id`
- `/* 0x1 */ u8 ammo_slot_id`
- `/* 0x2 */ u8 state`

### Commands

```
pdb_rich_query --index binaries/rich/target/index.jsonl --function "weapon_state::operator=" --list
# -> 0x590c00  vostok/game_core/sources/weapon_state.cpp

pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x590c00 --view target
```

### Target asm (0x590c00)

```
0x00: push ebp
0x01: mov  ebp, esp
0x03: push ecx
0x04: mov  [ebp-4], ecx          ; this
0x07: mov  eax, [ebp-4]          ; line 27
0x0a: mov  ecx, [ebp+8]          ; other
0x0d: mov  dl, [ecx]
0x0f: mov  [eax], dl             ; slot_id      = other.slot_id
0x11: mov  eax, [ebp-4]          ; line 28
0x14: mov  ecx, [ebp+8]
0x17: mov  dl, [ecx+1]
0x1a: mov  [eax+1], dl           ; ammo_slot_id = other.ammo_slot_id
0x1d: mov  eax, [ebp-4]          ; line 29
0x20: mov  ecx, [ebp+8]
0x23: mov  dl, [ecx+2]
0x26: mov  [eax+2], dl           ; state        = other.state
0x29: mov  eax, [ebp-4]          ; line 31: return *this
0x2c: mov  esp, ebp
0x2e: pop  ebp
0x2f: ret  4
```

Plain member-wise byte copy in declaration order, **NO self-assignment guard**
(carcass FUNCTION BODY has lines 27/28/29 copies, `<0>` blank line 30, then
line 31 return - no `if` line). Matched EXACTLY: three direct member assignments
then `return *this`.

## Source written

```cpp
weapon_state& weapon_state::operator=( weapon_state const& other )
{
    slot_id      = other.slot_id;
    ammo_slot_id = other.ammo_slot_id;
    state        = other.state;
    return *this;
}
```

## Anchor

Added `use_game_core_weapon_state()` to `temp_include_all.cpp`:
default-construct `a`,`b`; `b = a`; escape `&a`/`&b` through the opaque
`example_callback` sink so the member stores are observed (LTCG cannot DSE).
Also `#include <vostok/game_core/weapon_state.h>` and call it from `IncludeAll::IncludeAll`.

NOTE: weapon_state's default ctor is `explicit` and still a STUB (separate unit);
its emptiness is fine for the anchor (objects only need to be observed, not
initialized to specific values).

## Iterations

### Rebuild 1 (`python3 scripts/rebuild.py`, no module arg)

```
improved 0.00% -> 100.00%  weapon_state::operator=(weapon_state const&)
improved 0.00% -> 100.00%  weapon_state::weapon_state(void)   # free bonus from the anchor
```

`weapon_state::operator=` -> **100% DONE** on the first rebuild. The anchor also
pulled the empty default ctor (`weapon_state::weapon_state()`) to 100% for free
(same effect player_stealth saw) - its empty STUB body already byte-matched
0x5a0c40, so I updated its marker but did not touch its body. `deserialize`
stays BLOCKED (packet_reader cluster, README) - untouched.

### Regressions (NONE of mine - stale-baseline artifact)

report-changes.json shows 3 `player_stealth` fns dropping 100% -> 0%
(ctor, copy ctor, operator=). Triage per loop_performance.md:
- My `git diff` touches only `temp_include_all.cpp` + `weapon_state.cpp`; nothing
  in player_stealth or its reachability.
- `git show HEAD:.../player_stealth.cpp` -> all `STATE[STUB]`, empty bodies; no
  `player_stealth` reference in the anchor.
- Therefore player_stealth was never reachable/matched on THIS branch's committed
  source; the previous report.json was produced on a tree with PR #114's
  scaffolding merged. Stale-baseline artifact, not a regression from this unit.

## Result

`game_core::weapon_state::operator= -> STATE[100%|DONE]` (default ctor incidentally 100%).

