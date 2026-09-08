# Animation authoring recovery

The non-gold animation project mixes the game runtime with SDK-only animation
authoring, validation, serialization, and editor-mixer code. The recovered game
tree does not contain the complete v0.100b authoring types. In particular, the
unbaked bi-spline implementation and several editor-only n-ary/binary tree
members are absent.

Game-only Release and Debug graphs omit whole authoring translation units in
the temporary project view and use `VOSTOK_GAME_BUILD` only for authoring code
that shares a translation unit with runtime code. The runtime animation player,
baked animation resources, mixer, cooks, and world remain enabled. Deferred
pieces are:

- `check_animation_data.cpp` and the absent unbaked bi-spline validation path;
- the project-listed but unrecovered `bi_spline_bone_animation.cpp` and
  `bi_spline_skeleton_animation.cpp` (the historical SDK branch has
  compatibility bodies, but they are not game runtime);
- `editor_mixer.cpp` and the editor factories in `animation_entry_point.cpp`;
- binary-tree serialization and n-ary-tree dump/callback checks that require
  `driving_animation`, `m_next_animation`, `playing_type`, or the dropped
  integration debug state.

The authoritative project and original bodies remain available for a future SDK build.
The historical `sdk/v0100b-editor` branch contains compatibility implementations
derived from the older STK2 SDK, but those should be audited against v0.100b
evidence before the game-build guards are removed.

Two additional target-only helpers have game-build compatibility bodies because
VC9 LTCG rejects their non-returning stubs even when no ordinary call survives:
`animation_player::has_object` delegates to a lookup over the mixing tree, and
the obsolete `n_ary_tree::computed_local_bone_matrix` returns identity. Recover
and validate the original SDK-only local-bone helper before using it for editor
preview; the v0.100b runtime uses `bone_matrices_computer` instead.
