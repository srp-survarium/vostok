# A module anchor cannot self-dispatch: an unreferenced `.obj` inside a `.lib` is never extracted

tags: cpp:static-init cpp:anchor | topic:anchoring topic:reachability topic:link-model

## Symptom
You add `anchor_<module>.cpp` to a module, wire it to the module's `.vcproj`, guard it
with a file-scope dynamic initializer so it needs no external caller:

```
namespace { struct holder { holder( ) { anchor_render_facade( ); } }; holder s_holder; }
```

The `.obj` is produced (`binaries/Win32/intermediates/Master Gold/<module>/anchor_*.obj`
exists) but **nothing changes**: the anchored functions stay `unpaired`, and
`grep -c anchor_<module> binaries/rich/base/index.jsonl` prints `0`.

## Cause
Every vostok module builds to a **static library**. The linker only extracts a member
`.obj` from a `.lib` when that member resolves an already-undefined symbol. An `.obj`
whose only content is a static initializer defines nothing anyone references, so it is
never pulled in - and `.CRT$XCU` never sees it. (In a directly-linked `.obj` the same
initializer *would* run; the static-library membership is what kills it.)

## Fix
Anchors need a reference from a TU that is already being linked. Use the existing
dispatch chain rather than inventing one:

- define `void anchor_<module>( );` in the module's own `anchor_<module>.cpp`,
- declare it in `sources/vostok/game_core/sources/anchor.h`,
- call it from `survarium::IncludeAll::IncludeAll( )` in
  `sources/vostok/game_core/sources/anchor.cpp` (instantiated from
  `game_entry_point.cpp::create_world`, the real startup root).

That is exactly how `anchor_scaleform` / `anchor_animation_*` reach their modules.

## Related: prefer ADDRESS-TAKE over CALL when the pin only needs the body kept
`anchor-construct-call-volatile-args.md` says a carcass anchor must construct+call.
That is right when the anchored body is currently EMPTY (an empty body folds and pairs
at 0). When the body is already written and only `/OPT:REF` is removing it, an
address-take through a `static pcvoid volatile` sink is enough and is **safer**: it
keeps the out-of-line body (and, transitively, every `&engine::world::*` it binds)
without handing LTCG a call site whose fabricated arguments would let it specialise
the body away from the target's bytes.

Cost of either form: the pin forces the function to keep a stock `__thiscall` entry
(`mov ecx`/`ret 4`), while the target's real cross-module callers let LTCG pick a
custom stack-slot convention (`sub esp,N; push ebx; push ebp; mov ebp,[esp+N]`). That
prologue/epilogue delta caps such functions around 79-95% and is the sanctioned
"LTCG argument passing" residual - it recovers when the real consumers are matched and
the pin is retired, so do NOT grind on it.
