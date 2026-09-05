# Navigation authoring and debug recovery

The non-gold AI-navigation project includes an interactive navmesh generator,
fusion/tessellation tests, collision picking, and debug drawing alongside the
game's compiled-navmesh loader and path finder. The authoring side depends on
SDK-only state that is absent or incompatible in the recovered v0.100b tree:
the mutable model collision geometry, older debug-render overloads, and several
graph-generator test contracts.

Game-only Release and Debug graphs omit the graph-generator authoring units in
their temporary project view and use `VOSTOK_GAME_BUILD` around its few calls
from the mixed runtime world. Compiled navmesh loading,
spatial-tree construction, node lookup, channel search, funnel path finding,
standability checks, and shared serializer/triangulation helpers remain enabled.

When SDK work resumes, recover and validate the generator as one subsystem:
model/collision resource ownership, renderer overloads, fuser geometry helper,
interactive controls, serializer, and generator path tests. The compatibility
changes on `sdk/v0100b-editor` are useful leads but are not exact v0.100b
authoring evidence.
