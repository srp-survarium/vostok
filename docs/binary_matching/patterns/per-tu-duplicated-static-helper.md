# One short helper name, several INDEPENDENT per-TU definitions with DIFFERENT bodies - attribute each COMDAT to its TU by RVA adjacency
tags: cpp:static cpp:inline cpp:header | asm:call | topic:file-layout topic:pairing topic:data-recovery
symptoms: same demangled name repeated in the index under several `file:` values, identical sizes for some copies and different sizes for others, a header-inline copy whose `file:` is the HEADER so the owning TU is unknown, base emits the helper from the wrong set of TUs
confidence: 9/10
variants: inline-header-split-pairing.md, static-plain-name-pairing.md

## Symptom

`render/engine`'s `frac` appears **nine** times in the target rich index:

| `file:` | size |
|---|---:|
| `help_math.h` | 45 (x4) |
| `light.cpp` | 45 |
| `render_model_static.cpp` | 68 |
| `render_particle_emitter_instance.cpp` | 118 |
| `system_renderer.cpp` | 13 |

The naive reading - "one header inline, COMDAT-duplicated per TU" - is **wrong**, and a
previous batch had written exactly that into the source as
`// frac: COMDAT copy of the help_math.h inline`.

## The rule

**A `file:` that names a `.cpp` is a definition IN that `.cpp`.** MSVC files an inline
header function's line records under the HEADER; a same-named function filed under a
`.cpp` is a separate, file-local definition in that translation unit. So the sizes may
legitimately differ - they are different source bodies that happen to share a name.

Corollary: those `.cpp` TUs **cannot also include the header**, or the two declarations
would collide. Our tree had the include set exactly inverted: `help_math.h` was included
by `light.cpp` / `render_model_static.cpp` / `system_renderer.cpp` (the three TUs that
own their own copy) and by none of the four that really include it.

## Attributing a header COMDAT to its owning TU: RVA adjacency

The four `help_math.h` copies all report `file: help_math.h`, so the index alone does not
say which TU emitted them. The linker lays a TU's COMDATs out contiguously, so **sort the
whole index by `rva` and read the neighbours**:

```python
recs = [json.loads(l) for l in open('binaries/rich/target/index.jsonl')]
recs = sorted((r for r in recs if r.get('rva')), key=lambda r: r['rva'])
i = next(k for k,r in enumerate(recs) if r['rva'] == 6280272)
for r in recs[i-3:i+4]:
    print(r['rva'], r.get('file'), r['mangled'][:70])
```

```
6280240 .../stage_rain.cpp  ?is_effects_ready@stage_rain@...
6280272 .../help_math.h     vostok::render::frac        <-- owned by stage_rain.cpp
6280320 .../stage_rain.cpp  ??1stage_rain@...
```

That pinned the four to `cloud_noise.cpp`, `stage_rain.cpp`, `stage_clouds.cpp` and
`clouds.cpp` - which is also where `call vostok::render::frac` shows up, so the include
set is confirmed twice over.

## Recovering the bodies

Each variant is a different one-liner and each is fully legible from ~15 instructions;
`math::abs(float)` is the `and eax,7FFFFFFFh` bit trick and `math::abs(int)` is
`sar/add/xor`:

| owner | asm tell | source |
|---|---|---|
| `help_math.h`, `light.cpp` | `and eax,7FFFFFFFh` + `cvttss2si` + `sar/add/xor` | `math::abs( f ) - math::abs( static_cast< int >( f ) )` |
| `render_model_static.cpp` | adds `call vostok::math::floor` (the **int**-returning one) | `math::abs( f ) - math::abs( math::floor( f ) )` |
| `render_particle_emitter_instance.cpp` | `__real@4b000000` magic-constant round + `andps/orps` sign copy | `math::abs( f ) - math::abs( floorf( f ) )` |
| `system_renderer.cpp` | bare `cvttss2si` / `cvtsi2ss` / `subss`, no frame | `f - static_cast< int >( f )` |

## Free confirmation: the header's own line numbers

`help_math.h`'s records are `{ 14, 15, 16 }` (prologue / body / epilogue), i.e. the
signature is on line 13. Our reconstructed header had it on line 7 - the six missing lines
are exactly the standard 5-line GSC copyright banner plus one blank. Line numbers do not
move bytes, but they corroborate that the body is a SINGLE line.

## Do not over-read

A per-TU copy the target keeps but nothing in our tree calls stays target-only: the
shipped link retains inlined-away static COMDATs (`screen_factor` has **zero** call sites
in the whole image and is still emitted), ours does not. Restore such a definition only
together with the caller that uses it - otherwise it is dead code plus a C4505 warning.
Record the recovered body in a `claude@NOTE` at the site instead.
