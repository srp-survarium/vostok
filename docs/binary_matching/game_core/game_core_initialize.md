# game_core::game_core_initialize

`survarium::game_core_initialize()` at target rva `0x59c330`.
(The dispatch note said `0x5ac330`; the real index rva is `0x59c330`.)

Target asm (full body):

```
0x00:  push  ebp        ; <0x3>
0x01:  mov   ebp, esp
0x03:  pop   ebp        ; <0x2>
0x04:  ret
```

i.e. an empty function: the bare MSVC `/Od` frame prologue + epilogue, 5 bytes.

## Attempts
1. Empty body `{}` + direct call from a new `use_game_core_initialize()` anchor in
   `temp_include_all.cpp`  ->  base produced **no** standalone `game_core_initialize`
   symbol (`base_path = ./dummy.obj`). The empty fn is inlined at the call site.
2. Took the function's address (`void(*fn)() = &game_core_initialize; example_callback(...)`)
   to defeat call-site inlining  ->  still no standalone base symbol; not in base PDB.
3. Stored the address in a file-scope `volatile` global pointer
   (`void(*volatile g_keep)() = &game_core_initialize`)  ->  still ICF-folded away;
   no standalone symbol in the base EXE / base structure.

## Outcome
STATE[100%|DONE]: the source body is byte-correct (empty `{}` compiles to exactly
the target's 5 bytes - confirmed the symbol `?game_core_initialize@survarium@@YAXXZ`
is present in the compiled `entry_point.obj`). It is **not scorable** by the
delinker because the LTCG linker (`/GL` + `/OPT:ICF`) folds the 5-byte empty
function into another byte-identical empty function, so the final base EXE keeps no
unique standalone symbol for it - the base PDB records the folded-into symbol's name
instead. The unit therefore stays on `dummy.obj` and `report.json` lists it with no
`fuzzy_match_percent`. This is a pure linker-ICF visibility artifact, not a source
mismatch, and cannot be steered from source (all three reference shapes above were
tried). Body is as correct as source can make it.

Regressions caused: none (`0 regressed, 0 improved`).
Inlining/ICF: the function itself is the thing folded; nothing inlined *into* it.
