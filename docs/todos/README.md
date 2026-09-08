# Deferred non-game surfaces

These notes record code intentionally left outside the game-only Release and
Debug graphs. They are recovery tasks, not prerequisites for compiling and
linking the game.

## Authoring subsystems

- [Animation authoring](animation_authoring.md)
- [Maya animation conversion](maya_animation_authoring.md)
- [Navigation graph generation](navigation_authoring_debug.md)
- [Render live editing and conversion](render_authoring.md)
- [Animation cooker self-checks](nongold_verification.md)

## Runtime diagnostics

- [Bullet debug visualization](bullet_debug_visualization.md)
- [Sound propagator snapshots](sound_authoring_debug.md)

The runtime-diagnostic items are not part of the SDK/editor surface. They are
listed here because `MASTER_GOLD` historically enabled them together with the
authoring code and their exact non-Gold implementations are incomplete.
