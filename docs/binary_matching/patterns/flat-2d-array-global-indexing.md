# A `[idx*sizeof + symbol]` base load with a `PAY0<N>` mangled global = a 2D array `T g[M][N]`

tags: cpp:array cpp:global | asm:lea asm:cmp | topic:structure-shape topic:data-symbol

## Symptom

A global data symbol indexed with a single flattened offset, e.g.

```asm
lea  edi, [ecx+eax*2]                      ; flat = i + row*N  (row*N precomputed)
lea  edi, [edi*8+?g_graphic_presets@survarium@@3PAY09Ugraphic_preset@1@A]
cmp  [eax*8+?g_graphic_presets@...@A], ebp ; element.field
```

The mangling `3PAY0<N>U<type>` decodes as a **global array whose innermost
dimension is `[0..N]` (= N+1 elements)** - here `Y09` = `[10]`. The code treats
the symbol as a flat contiguous base (`base + flat_index * sizeof(T)`), with the
row stride folded in by an earlier `row*N` computation (`lea r,[r+r*4]` = `*5`,
`add r,r` = `*10`).

## Source

A 2D array global with that inner dimension:

```cpp
graphic_preset g_graphic_presets[ 5 ][ 10 ];          // [M][10]
...
graphic_preset& preset = g_graphic_presets[ row ][ i ];   // -> base + (row*10+i)*8
```

`g[row][i]` decays to `&g[0][0] + (row*10 + i)` and the compiler emits exactly the
flat `[flat*sizeof + symbol]` base load - no pointer-to-array indirection. The
outer dimension `M` is NOT in the mangled name (only the innermost shows); pick it
from the guarding bound (`row < M`, here `cmp al, 5` -> `M = 5`). Verify the chosen
`[M][N]` reproduces the exact mangled symbol after a build - `PAY0<N>` pins N, the
flat-base usage confirms it is a real array (not a `T(*)[N]` pointer, which would
load the pointer first).

The array's own element bytes score as a SEPARATE data symbol; the code match only
needs the symbol to exist and relocate (define it once at namespace scope, even
zero-initialized, if the real contents are not recoverable).

Seen: `survarium::options_item_int::call` / `options_graphics_quality_selector::call`
(options_items.cpp) walking `g_graphic_presets[quality][i]`.
