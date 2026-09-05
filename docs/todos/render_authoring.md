# Render authoring and live-edit recovery

The non-gold render projects contain SDK-only live editing and conversion paths
whose v0.100b implementation is absent from the recovered game tree. Game-only
Release and Debug graphs define `VOSTOK_GAME_BUILD` and omit these paths:

- texture source conversion and Lua texture-options cooks referenced by
  `texture_cook_register.cpp`;
- `res_effect::used_shaders` tracking used only by shader live recompilation;
- material-source watcher callbacks used by the editor's live reload;
- construction of the missing `render::editor::renderer` facade in DLL builds.

The binary texture cooker, normal effect compilation/loading, material runtime,
game renderer, and render engine stay enabled. `shader_source` uses the ordinary
render-core allocator in game builds instead of the absent editor debug
allocator.

When SDK work resumes, recover and audit the texture converter/options cooker,
effect dependency tracking, material watcher callback, editor renderer, and its
draw/terrain command families. The `sdk/v0100b-editor` branch has compatibility
implementations based partly on STK2; they are leads, not exact v0.100b source.
