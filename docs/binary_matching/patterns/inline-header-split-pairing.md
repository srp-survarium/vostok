# A whole objdiff unit that is 100% unpaired is a FILE-PLACEMENT bug, not a matching gap

tags: cpp:header cpp:inline cpp:template | asm:none | topic:pairing topic:objdiff-units topic:structure-shape topic:file-layout

## Symptom
An objdiff unit whose name is a HEADER shows every row unpaired / `fuzzy_match_percent == null`:

```
vostok/render/core/dx11/backend_inline.h            20 unpaired / 20
vostok/render/core/dx11/effect_manager_inline.h    114 unpaired / 114
vostok/render/core/dx11/state_cache_inline.h         8 unpaired / 8
vostok/render/core/utils_inline.h                    3 unpaired / 3
```

and `binaries/objdiff/objdiff.json` gives those units `"base_path": "./dummy.obj"`.
Meanwhile the base emits the *same mangled symbols* perfectly well - just filed under a
DIFFERENT header:

```
base  vostok/render/core/dx11/backend.h            10 fns   target  .../backend_inline.h        20
base  vostok/render/core/dx11/effect_manager.h     46 fns   target  .../effect_manager_inline.h 114
base  vostok/render/core/dx11/state_cache.h         8 fns   target  .../state_cache_inline.h      8
base  vostok/render/core/dx11/utils.h               3 fns   target  vostok/render/core/utils_inline.h  3
```

The base rows are INVISIBLE to the report (no unit is keyed on `backend.h`), so the
functions never pair, never score, and never show up in any queue.

## Cause
objdiff units are derived from the DELINKED TARGET objects, one per **source file** as
recorded in the PDB's line program. A function defined inline in a header is attributed
to the header that physically contains its body. Pairing is `unit + mangled symbol`, so
a body that lives in `backend.h` on our side and in `backend_inline.h` in the target
lands in two different units and can never be matched - no matter how byte-identical
the code is.

The original tree split every non-trivial inline body out of `<x>.h` into a sibling
`<x>_inline.h` (a convention already visible in-tree: `resource_intrusive_base.h` ends
with `#include <vostok/render/core/resource_intrusive_base_inline.h>`). Our
reconstructions kept the bodies in the main header.

## Detection (cheap, no build)
Diff the per-file function counts of the two rich indexes:

```sh
python3 - <<'EOF'
import json, collections
def counts(p, pref):
    c = collections.defaultdict(int)
    for ln in open(p):
        d = json.loads(ln); f = d.get('file') or ''
        if f.startswith(pref) and f.endswith('.h'): c[f] += 1
    return c
b = counts('binaries/rich/base/index.jsonl',   'vostok/<module>/')
t = counts('binaries/rich/target/index.jsonl', 'vostok/<module>/')
for f in sorted(set(b) | set(t)):
    print('%-58s base=%3d target=%3d' % (f, b.get(f,0), t.get(f,0)))
EOF
```

A `base=N target=0` line next to a `base=0 target=M` line with a related name is the
tell. Confirm with an exact **mangled-name** intersection (`d['mangled']`) before
moving anything - 44/46 identical mangled names is a certainty, not a guess.

## Fix
Move the inline BODIES (not the class declaration, not one-line accessors that are never
emitted) into the target-named file and include it at the very end of the main header,
after the namespaces are closed:

```cpp
// dx11/backend.h
};                      // end of class backend

} // namespace render
} // namespace vostok

#include <vostok/render/core/backend_inline.h>

#endif
```

* Platform-dispatched headers need BOTH files: the real `dx11/<x>_inline.h` and a
  guard-less shim `core/<x>_inline.h` containing only
  `#include VOSTOK_RENDER_CORE_FILE(<x>_inline.h)`.
* Where the target's path has no `dx11/` component (`core/utils_inline.h`,
  `core/shader_constant_inline.h`) the inline file is a REAL file at `core/` level.
* An in-class body becomes an out-of-class definition:
  `template < typename D, typename S > inline D* state_cache<D,S>::get_state( S const& d ) { ... }`.
  Order matters: an `_inline.h` whose bodies call another class needs that class complete,
  so include it from the *consumer* header after the class, not from the class's own header.
* Same trick for a header filed under the wrong DIRECTORY: the target had
  `core/res_xs.h` where we had `dx11/res_xs.h` (a shim). `git mv` the real file up and
  delete the shim.

## Payoff (render/core, 2026-08-01, batch B5)
Zero code changes, pure file relocation:

| unit | before | after |
|---|---|---|
| `backend_inline.h` | 0 paired | 8 paired (set_vb 98.9%, reset_render_targets 100%) |
| `backend_handlers_inline.h` | 0 | 11 paired (~53%) |
| `effect_manager_inline.h` | 0 | 44 paired, avg 86% |
| `state_cache_inline.h` | 0 | 8/8 paired, avg 95% |
| `utils_inline.h` | 0 | 3/3, avg 81% |
| `shader_constant{,_host,_slot}_inline.h` | 0 | 4/4 (slot ctor 100%, host compare 99.4%) |
| `custom_config_value_inline.h` | 0 | 3/4 (two at 100%) |
| `core/res_xs.h` | 0 | 3/3 at 77.1% |

Whole-EXE headline moved 36.32% -> 37.08% code / 53.02% -> 53.88% functions.

## Related
- `address-taken-anchor-emits-header-comdat.md` - why a header COMDAT exists at all.
- The reverse failure mode (base emits nothing) is an inline-vs-call wall, not this.
