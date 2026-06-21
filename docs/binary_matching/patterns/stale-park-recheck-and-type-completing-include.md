# Stale cross-unit PARK notes go stale; a type-completing include can newly pair a whole forward-declared subsystem

## Symptom
A function carries a `claude@NOTE: PARKED - blocked on cross-unit symbol absent from
our tree` and a recovered body. The block was real WHEN WRITTEN, but the blocking
member/accessor (or the header) may have landed since. Two separate things:

1. **A PARK cause can expire.** `lobby_client::ping_server` / `read_ping_server_answer`
   were parked on `game::m_current_time_in_ms` (offset 0x3f4) and `lobby_menu::set_ping`
   being absent from the carcass `game.h` / `lobby_menu.h`. Both had since been added
   (`game.h` gained `m_current_time_in_ms` @0x3f4 + `game_time_ms()` accessor and
   `m_lobby_menu` @0x374 + `lobby_menu()` accessor; `lobby_menu.h` gained
   `set_ping(u32)`). Re-checking unblocked both: `unpaired`/2.86% -> STRUCTURE MATCH
   (76%/52%, residual = packet_reader inline wall).

2. **Calling a method on a FORWARD-DECLARED type needs a type-completing include.**
   The carcass `game.h` only forward-declares `class lobby_menu;`. `m_game.lobby_menu()`
   returns `lobby_menu&`, but calling `.set_ping(...)` requires the complete type, so a
   bare rebuild fails with the incomplete-type error (not a warning - the build STOPS).
   Add `#include "lobby_menu.h"` to the `.cpp`. Side effect: pulling that header into the
   build NEWLY PAIRED ~993 functions (the entire `lobby_menu`/`lobby_camera` UI subsystem
   that was unpaired only because the carcass forward-decl kept its codegen out of the
   base). The `report-changes` "regressed 100->0" entries that accompany this are all
   `null`-named / dtor / `vcall` / template COMDAT fold-rep churn (objdiff ICF re-attributes
   the folded representative when a new unit pairs), NOT real regressions - filter them out
   and read the `improved` count + per-function structure-diff.

## Cure
- On entering a unit, re-verify every cross-unit PARK cause against the CURRENT headers
  (`grep` the offset / accessor / method) before trusting the note - causes expire as the
  carcass fills in.
- When the recovered body calls a method on a forward-declared member type, add the
  type-completing `#include` to the `.cpp`; expect (and welcome) a large `improved` count
  if that header gates a whole subsystem. Confirm "regressions" are fold-rep noise, not
  source.
