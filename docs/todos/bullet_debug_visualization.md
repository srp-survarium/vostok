# Bullet debug visualization recovery

The non-gold game-core project enables a game diagnostic bullet visualization layer
that is incomplete in the recovered v0.100b game tree. The missing surface
includes `bullet_manager.h` from the original debug implementation, trajectory
and collision-point storage, debug-render state fields, `toggle_is_fixed`, and
the colored-vertex drawing path.

Game-only Release and Debug graphs omit `bullet_manager_debug_render.cpp` in
their temporary project view and use `VOSTOK_GAME_BUILD` around the diagnostic
calls embedded in runtime translation units. Bullet simulation, collision,
ricochet, tracer, and lifetime logic remain enabled.

When diagnostic recovery resumes, recover the bullet-manager debug state and renderer as a
single subsystem. Do not infer its layout from the surviving calls: the current
header intentionally lacks those fields, and the shipped game does not contain
the non-gold authoring implementation needed to prove them.
