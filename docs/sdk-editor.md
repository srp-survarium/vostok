# SDK editor recovery

The SDK editor is not part of the supported game build. Release and Debug now
compile and link the game-only runtime without requiring the editor or content
pipeline; see [game-builds.md](game-builds.md).

The original solution coupled these products through `MASTER_GOLD`. Disabling
that macro exposed editor adapters and authoring checks in the same engine
projects as runtime code, which made an ordinary game build look like an SDK
build. The game-specific solution view now separates those concerns. Deferred
authoring surfaces are listed in [todos/README.md](todos/README.md).

A full editor build still has two independent requirements:

1. Its managed and C++/CLI assemblies need the CLR v2 and .NET 3.0/3.5/WPF
   authoring payload. The pinned toolchain currently provides MSVC, the Windows
   SDK, and the DirectX SDK, but no `netfx` payload.
2. The native editor adapters and content-authoring implementations must be
   recovered and validated as SDK code. They include the editor renderer,
   animation authoring, navigation generation, and their managed host boundary.

The historical `sdk/v0100b-editor` branch is useful as an inventory and contains
compatibility implementations derived partly from the older STK2 SDK. It is not
proof of exact v0.100b behavior and should be ported subsystem by subsystem when
SDK recovery is explicitly in scope.
