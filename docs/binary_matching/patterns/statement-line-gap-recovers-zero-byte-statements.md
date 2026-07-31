# Statement-LINE gaps recover byte-invisible source lines (`R_ASSERT` in an optimized TU)

tags: cpp:assert cpp:structure | asm:none | topic:structure-shape topic:optimized-target topic:pdb-lines

## Symptom
In an **optimized** (`/O2` / `MASTER_GOLD`) module a forwarder body reads as a single
addressed statement, so there is nothing in the bytes to tell you whether the source
also contained an `R_ASSERT( scene );`. In `MASTER_GOLD`, `R_ASSERT` expands to
`VOSTOK_EMPTY_EXPRESSION` = `if ( ::vostok::identity(false) ) { } else (void)0`, which
constant-folds to **zero bytes and zero line-table entries** - unlike the `/Od`
`ASSERT` -> `call empty_stub` eater (see `assert-eater-recover.md`).

## Cause / recovery
The PDB still records the `{` and `}` of the function as their own statements, and it
records the *absolute source line* of each. The **gap between the `{` line and the
body statement's line is the number of physical source lines the body occupied** - and
that count DOES see the zero-byte lines.

```
pdb_fetch --target-index ... --rva 0x<fn> --view structure
# or, for the whole TU at once, read `statements[].line` straight out of index.jsonl
```

`vostok::render::scene_renderer` (facade forwarders, all one `owner_push_back`
statement) gives a clean calibration table:

| shape                                                   | `{` -> stmt gap |
| ------------------------------------------------------- | --------------- |
| one-liner, no assert (`set_sky_material`, `reload_shaders`) | 1            |
| one-liner + `R_ASSERT` (`add_model`, `set_post_process`)    | 2            |
| expanded `boost::bind` w/ 3 bind args + `R_ASSERT` (`remove_decal`) | 11    |
| expanded `boost::bind` w/ 4 bind args + `R_ASSERT` (`update_decal`) | 13    |

A function whose measured gap is 13 but whose reconstruction is only 12 lines long is
**missing exactly one source line**, and in this family that line is the `R_ASSERT`.
That is how `update_ambient_volume` / `remove_ambient_volume` / `update_volume_fog` /
`remove_volume_fog` were shown to carry an `R_ASSERT( scene )` that the harvested
legacy ancestor had dropped: the change is byte-neutral (all four stayed at their
prior %), but the source shape is now right.

## Use it for
- deciding assert presence in optimized TUs (`R_ASSERT`, `VOSTOK_UNREFERENCED_PARAMETER`,
  any macro that folds to nothing),
- sizing a body you have not written yet: a 12-line gap on a 3-bind-arg command means
  the original spelled `boost::bind` expanded one-arg-per-line, not on one line,
- spotting the un-recoverable gaps: a jump of 137 lines between two neighbours (here
  `remove_tracer` 486 -> `set_portal_system` 623) is the block of sibling methods
  `/OPT:REF` stripped from the shipped exe - do not go looking for their bytes.

## Caveats
Only sound where each source line maps to at most one statement (the /Od-style
"one statement per line" house style). Blank lines and comments inside the body count
toward the gap too, so treat a 1-line discrepancy as *evidence*, not proof - confirm
against a sibling of the same shape that already matches 100%.
