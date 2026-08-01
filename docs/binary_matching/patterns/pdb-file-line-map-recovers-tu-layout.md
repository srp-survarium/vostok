# The rich index's per-function line map recovers a whole TU's source layout, not just its order
tags: cpp:file-layout cpp:macro | asm:push | topic:line-records topic:structure-shape topic:logging topic:data-recovery
symptoms: log call pushes a __LINE__ immediate that cannot be reached by our source; functions match in order but every __LINE__/__FILE__ site is off; want to know how many lines a compiled-out block occupied
confidence: 9/10
variants: log-line-verbosity-immediates.md, multiline-statement-closing-line-record.md, statement-line-gap-recovers-zero-byte-statements.md

`binaries/rich/target/index.jsonl` carries `file` and a `statements[]` array of
`{off,size,line}` for **every** function. One `jq` pass therefore prints the shipped
source layout of a whole translation unit - definition ORDER *and* the line each body
opens and closes on:

```sh
jq -r 'select(.file|test("render/core/dx11/sources/device\\.cpp$"))
       | "\(.rva) lines=\([.statements[].line]|min)-\([.statements[].line]|max) \(.name)"' \
   binaries/rich/target/index.jsonl | sort -n
```

```
5594800 lines=23-25    device::device(bool)
5593744 lines=28-30    device::~device()
5592544 lines=33-53    device::on_device_removed()
5591488 lines=56-64    is_resolution_already_exists(...)
5591600 lines=67-159   device::create_d3d()
...
8188048 lines=12-12    `dynamic initializer for 'g_debug_render_device''
```

What it gives you, in order of usefulness:

1. **Definition order** - the RVA order and the line order agree, so a TU whose functions
   are in the wrong source order shows up immediately (COMDAT order follows source order).
2. **The exact opening/closing line of every body**, which is the only way to reproduce a
   `__LINE__` immediate. `min` is the `{` record (the prologue), `max` is the `}` record
   (the epilogue) - both exist even at `/Ox`.
3. **The size of compiled-out regions.** A 29-line hole inside a function body with no
   records is an `#ifndef MASTER_GOLD` / `#ifdef DEBUG` block; a hole between two functions
   is a comment banner or a file-scope definition with no dynamic initializer. You cannot
   recover their *content*, but you know their extent - which is what pins the lines of
   everything below them.
4. **Where a global with no dynamic initializer lives.** A POD-ish file-scope array leaves
   no symbol in the line map, so the only in-file room for it is a records-free gap between
   two functions.

Cross-check the layout you write with the per-function `--view structure` line column, and
against the base side of the same query after the build (`binaries/rich/base/index.jsonl`)
- if the base's `dynamic initializer for 'X'` files on the closing `);` of a multi-line
definition (it does, see `multiline-statement-closing-line-record.md`), the same rule holds
for the target.

## Relationship to "do NOT pad to land __LINE__"
`log-line-verbosity-immediates.md` records sushi's rule: do not insert filler purely to
shift one call site onto its `__LINE__`. That rule is about a *single* immediate with no
other evidence - padding there is a guess and stays fragile forever. The line MAP is a
different situation: the PDB pins every body in the file at once, so reproducing the file's
geometry is recovering structure, not guessing at one number. Still keep the honest
boundary - reproduce holes you can *prove* from the map, annotate them with a
`claude@NOTE`, and never invent geometry a record does not demand.

Evidence: `render/core/dx11/sources/device.cpp` - `device::on_device_removed` pushes
`26h..2Bh` for its six `LOG_ERROR` sites; the map put the body at 33..53, which forced the
ctor's five-entry initializer list onto 18..22 and the `g_debug_render_device` definition
onto line 12, and revealed a 29-line compiled-out `#ifndef MASTER_GOLD` block inside
`create_d3d` (71..99) plus a 13-line file-scope gap (299..311) that is the only room the
`g_num_monitors` / `g_monitor_resolutions` definitions can occupy.
