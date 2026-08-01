# A gap in an /Ox line map is often a SUNK store, not compiled-out code - and you must not "fix" it by moving the statement

tags: cpp:local cpp:member cpp:assignment | asm:mov asm:inc | topic:line-records topic:optimized-target topic:structure-shape topic:scheduling
symptoms: the target line map has no record where a store/increment/`= 0` initialiser must exist; `--view structure-diff` reports BASE_ONLY for a plain assignment the target never records; a statement's `mov`/`inc` shows up inside the asm range of a LATER record
confidence: 9/10
variants: pdb-file-line-map-recovers-tu-layout.md, statement-line-gap-recovers-zero-byte-statements.md, multiline-statement-closing-line-record.md

## The two reasons a line has no record

`statement-line-gap-recovers-zero-byte-statements.md` covers the first: the line emitted
**no code at all** (a `MASTER_GOLD` assert, a compiled-out `#ifndef` span, a comment).

The second is easy to mistake for it: at `/Ox` the scheduler **sinks (or hoists) a
statement whose entire body is a store** into a neighbouring statement's basic block.
The store is still there in the asm - it just sits inside another line's address range,
and MSVC never opens a record for its own line. So the map shows a hole while the bytes
show the store.

`device::create_d3d` (release, `/Ox`, `MASTER_GOLD`) has five of them at once:

| source statement                       | its asm                       | record it landed in |
| -------------------------------------- | ----------------------------- | ------------------- |
| `m_adapter = 0;`                        | `mov [ecx], edi` @0x76        | line 100 (`EnumAdapters` fallback) |
| `m_use_perfhud = false;`                | `mov byte [esi+14Dh],0` @0x78 | line 100            |
| `g_num_monitors = 0;`                   | `mov [g_num_monitors], edi` @0x8d | line 106 (the clear loop body) |
| `u32 num_display_modes = 0;`            | `mov [ebp-10h], ebx` @0x10b   | line 124 (`GetDisplayModeList`) |
| `++g_num_monitors;`                     | `inc [g_num_monitors]` @0x344 | line 154 (`GetDesc`) |

## How to use it when reconstructing

1. Locate the store in the asm, note which **record's address range** contains it.
2. Put the statement in the record-free gap that *precedes* that record, in the natural
   source order. It will sink there again in your build, and the base line map will
   reproduce the target's hole for free.
3. Confirm with `--view structure-diff`: if the statement is placed right, the target's
   hole and your hole coincide and the row disappears. If you placed it too late (after
   the absorbing statement) MSVC opens a record for it and the diff shows **BASE_ONLY**
   at that line.

## Do NOT chase the merge by reordering (measured)

A tempting inference is the reverse: "the target merges GetDesc + `++monitor_index` +
the loop latch into ONE 0x56-byte record at line 154, my base splits them into 0x18 at
154 and 0x3b at 155, so I should move `++monitor_index` above `GetDesc` and let it
sink." That was tried on `create_d3d`:

```cpp
    ++g_num_monitors;                          // both counters first ...
    ++monitor_index;
    DXGI_OUTPUT_DESC output_desc;
    output->GetDesc( &output_desc );           // ... call last
```

**97.40% -> 90.90%.** Moving the increment ahead of the call changed the induction-variable
schedule of the whole loop tail and cost 6.5 points; it was reverted. The record *layout*
is a consequence of scheduling, not a knob you turn by permuting statements - it only
tells you which gap a statement belongs in, never that two statements must share a line.
Write the natural order (`GetDesc` then `++monitor_index`, the order the loop reads in),
and let a residual line-table split stand: `SIZE -0x3e` + `BASE_ONLY` on a loop-latch row
is a scheduling artifact, not a source-shape error.

Evidence: `render/core/dx11/sources/device.cpp::create_d3d` 13.5% -> 97.4% (25/25
statements, base 0x3a7 vs target 0x3aa bytes) with every sunk store placed by rule 2;
the one deliberate re-order to force a record merge is the 90.90% data point above.
