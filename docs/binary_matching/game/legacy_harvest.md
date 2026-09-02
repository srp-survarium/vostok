# game legacy-body harvest — protocol

> Closed record. `temp/game_legacy/` was deleted from the tree after the harvest;
> every file it names is retrievable with `git show 3320ded27:temp/game_legacy/<file>`.

Bring the original game module's bodies (parked in `temp/game_legacy/`) into the
enabled carcass, **for the simple cases**. This is a SEEDING pass: replace stub
bodies with the real legacy logic, adapted to the canonical types. We do NOT
match/verify here — that's the next phase, once everything is in.

## The protocol (per function)

1. **Find the legacy ancestor.** Direct-name first (the carcass `.cpp` and a
   same-named `temp/game_legacy/*.cpp` share `class::method`). Obvious
   structural renames too (e.g. `actor`→`player`, the `game_world.cpp` split).
   No expensive content-rename hunting — if there's no simple ancestor, skip it.
2. **Port the body, adapted to canonical.** Replace the carcass stub body with
   the legacy body. The CARCASS header is ground truth: adapt the legacy body to
   the canonical signature, member names/types, and calls (game_core moved
   types, the network rewrite, signature/member drift). KEEP the addressed
   `// FUNCTION BODY[0xVA]` carcass comment — the matcher needs it next phase.
3. **Cut it from temp.** Delete the ported function from its
   `temp/game_legacy/*.cpp`. The temp tree IS the tracker: what remains is the
   un-ported / can't-port remainder (manual review, later). A fully-harvested
   legacy file gets deleted.
4. **Stay green.** The module must still compile + link after the batch.

## Rules

- **No `STATE[%|PARTIAL]` markers, no review doc.** Ported functions are still
  unmatched stubs — leave any existing `STATE[STUB]` as-is; add no percentages.
- **Simple ones only.** If a legacy body references removed subsystems, diverged
  control flow, or types that don't map cleanly, DON'T force it — leave it in
  `temp/game_legacy/` for manual review. Don't partial-port-and-flag; just skip.
- Adapt against the canonical carcass types, never the legacy headers (the legacy
  headers in `temp/game_legacy/*.h` are reference only — their members differ).
- Module stays green every batch; serial batches (shared headers); a legacy
  source file is owned by ONE batch (the batch owning its primary carcass target).

## Batches (direct-name manifest: 34 files, 228 fns)

| # | Subsystem | carcass files (fns) | Status |
|--|-----------|---------------------|--------|
| 1 | npc | human_npc, human_npc_cook, npc_stats, ai_sound_player (60) | DONE |
| 2 | game/world | game, game_initialize, game_world + the game_world*.cpp split (37+) | DONE |
| 3 | camera | free_fly_camera, game_camera, camera_director (21) | DONE |
| 4 | cooks/stats | animated_model_instance_cook, sound_player_cook, stats, stats_graph (26) | DONE |
| 5 | menus/input | key_binder, main_menu(+_input), lobby_menu(+_input) (34) | DONE |
| 6 | objects | object, object_{decal,environment,light,sky,solid_visual,volumetric_sound,weapon,wire} (31) | DONE |
| 7 | misc + anim | project_cooker_simple, weapon, weapon_cook (9) + animations_selector, simple/single_position_animation_controller | DONE |

**The direct-name harvest (batches 1-7) is COMPLETE.** ~125 functions seeded;
7 legacy `.cpp` fully consumed + deleted (animated_model_instance_cook,
camera_director, game_camera, game_initialize, object_weapon,
simple/single_position_animation_controller). What remains in `temp/game_legacy/`
is the can't-port remainder (removed subsystems / diverged control flow / rewritten
types) plus the renamed/split layer below — the manual-review bucket.

## ⚠️ Measurement gap — the game module is /OPT:REF-stripped (matching-phase blocker)

The harvested bodies compile + link but read **0%**, and NOT because the bodies
are wrong: **the entire game module is dead-stripped from the base exe**. Only 2
game base objs delink (`game_entry_point.cpp`, `temp_include_all.cpp` — the
reachable ones); the objdiff config points the other 139 game units at
`./dummy.obj`. Nothing instantiates the game object graph (the interim
`create_world` returns NULL), so `/OPT:REF` drops every game class method. The
base rich index has ZERO `human_npc`/`game_camera`/`camera_director` symbols.

So before the next-phase matching can measure/drive any game function (harvested
or not), the game functions must be made REACHABLE in the base exe — either:
1. **Reachability spine**: port/match `create_world → game ctor → game_world
   ctor → …` so the object graph is reachable naturally (as the shipped game is).
   The harvest SKIPPED these (heavily diverged), so this is real matching work; OR
2. **Anchors**: extend `game/sources/temp_include_all.cpp` with `use_game_*()`
   functions that instantiate each game class and call its methods (the exact
   pattern that keeps game_core matchable). This is the cheaper bridge.

This is the FIRST task of the game matching phase. Until then the harvest's
value is latent (bodies in source, invisible to the % tracker).

Renamed/split layer (after the direct passes, simple cases only): legacy
`actor*.cpp`→`player*.cpp`, legacy `game_world.cpp`→ the 5 shipped `game_world*.cpp`
TUs, legacy `human_npc_animation_controller*`/`human_npc_animations`→ shipped
animation controllers. The large new layer (network clients, lobby/login/chat
menus, scaleform, match flow, economy/profiles, damage/anomaly/booby-trap) has
NO legacy ancestor — it stays for the matcher loop; nothing to harvest.
