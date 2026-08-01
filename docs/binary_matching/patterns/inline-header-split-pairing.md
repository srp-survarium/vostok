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

## Three variants beyond `<x>.h` -> `<x>_inline.h` (batch B6)

1. **Header -> `.cpp`.** The target may file a template's bodies under
   `sources/<x>.cpp`, not a header at all: `xs_descriptor<T>::reset/set_texture/
   use_texture` and the implicit `vs_data::operator=` they instantiate all live in
   `dx11/sources/xs_descriptor.cpp`. Reconstruct as out-of-line template definitions in a
   NEW `.cpp` plus whole-class explicit instantiation (`template class xs_descriptor<vs_data>;`
   - the tree already uses that idiom in `res_xs.cpp`). Members the target does not show are
   NOT evidence against this: an explicit instantiation emits every member and `/OPT:REF`
   strips the unreferenced ones. Remember to add the new `.cpp` to the module `.vcproj`.
2. **`<x>_impl.h`, not `_inline.h`.** Same mechanism, different suffix
   (`res_xs_hw.h` -> `res_xs_hw_impl.h`). Read the target file name, don't assume.
3. **N of our headers were ONE of theirs.** `dx11/res_effect.h` holds what we had split
   across `core/res_effect.h`, `core/effect_compile_data.h`,
   `dx11/binary_shader_{source,key_type,cook_data}.h`. Fold them into the target-named
   header and delete the extra files (repoint the includers); a compiler-generated ctor/dtor
   is attributed to the file that physically holds the CLASS, so the class itself has to move.

## Two traps

* **ICF folds poison the attribution.** The rich index is RVA-keyed, so several mangled
  names can share one record and inherit the file of whichever symbol won the fold. Before
  moving anything, check the target RVAs are DISTINCT and the sizes non-trivial. Small
  compiler-generated dtors sitting at an RVA adjacent to a real function in some unrelated
  header (`shader_constant_table::~shader_constant_table` filed under `res_geometry.h`) are
  usually fold/neighbour artifacts - do not chase them.
* **Include cycles cap the fold.** `res_pass::res_pass` is attributed to `dx11/res_effect.h`,
  but defining it there needs `res_xs` complete, and `res_effect.h -> res_xs.h ->
  res_xs_hw.h -> resource_manager.h -> res_effect.h` closes a cycle. Leave the body where it
  compiles and note it; a partial relocation that breaks the build costs a whole cycle.

## What is NOT a split

A target row whose mangled name is absent from the WHOLE base index is a missing
instantiation, not a misplaced body. All 70 remaining `effect_manager_inline.h` rows are
`effect_manager::create_effect<effect_xxx>` for engine effect types our build never
instantiates - relocating anything cannot pair them. Check `mangled in BASE` before
adding a file to the worklist.

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

## Payoff (render/core, 2026-08-01, batch B6)

| unit | before | after |
|---|---|---|
| `dx11/res_xs_hw_impl.h` | 0 paired | 9 (ctor/dtor 100%, create_hw_shader 87-93%) |
| `dx11/sources/xs_descriptor.cpp` | 0 | 7 distinct (gs_data::operator= 91.9%) |
| `dx11/res_effect.h` | 0 | 7 (cook_data ctor 100%, effect_compile_data 92.2%) |
| `dx11/effect_options_descriptor.h` | 0 | 3 (two at 100%, pcstr 99.5%) |
| `dx11/shader_constant_buffer_inline.h` | 0 | 1 |

Render fuzzy 32.0% -> 32.9%, exact 490 -> 496; unmeasured render rows 208 -> 143.

## Related
- `address-taken-anchor-emits-header-comdat.md` - why a header COMDAT exists at all.
- The reverse failure mode (base emits nothing) is an inline-vs-call wall, not this.
