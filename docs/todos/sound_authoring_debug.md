# Sound diagnostic snapshot recovery

The non-gold sound project exposes source-config generation and debug snapshot
facilities in addition to the game runtime. The WAV/SPL config helpers have
complete bodies in this tree and only needed their Lua-config declarations
included explicitly for non-gold compilation.

`new_sound_propagator::dump_debug_snapshot`, however, is absent. Game-only
Release and Debug builds retain proxy snapshot generation but omit the nested
propagator callback under `VOSTOK_GAME_BUILD`.

When diagnostic recovery resumes, recover and validate the propagator-level snapshot schema
and implementation. A declaration exists on the historical
`sdk/v0100b-editor` branch, but the corresponding exact v0.100b behavior is not
established by the shipped game.
