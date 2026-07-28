# pdb_divergence `[member]` flag may be a STALE/DUPLICATE type record — confirm against the emitted asm

- confidence: 8
- cpp tags: cpp:member cpp:struct cpp:template
- asm tags: asm:mov asm:cmp
- topic tags: topic:tooling topic:structure-shape topic:scoring-artifact

## Symptom

`pdb_divergence --headers-only` reports a `[member]` `changed` row where the base
and target differ in a member's TYPE/SIZE/OFFSET, e.g.

    class vostok::network_core::udp_match_connection
      [member]
        changed  m_local_sequence_id: base(sequence_number< u16 > @0x124)  target(sequence_number< u8 > @0x124)
        ...
      [size] base=0x538  target=0x530

…but the base value is the one a prior matcher already pinned against the
disassembly. Editing the header to the "target" value is WRONG.

## Cause

The original `survarium.pdb` type stream carries MORE THAN ONE record for the
same qualified type (per-TU type drift in the shipped build, or an incremental
relink). `pdb_divergence` joins by name and picks ONE of them — sometimes a
STALE / phantom record whose layout never shipped. The tell:

- the target structure-dump has BOTH `…/udp_match_connection.h` (the flagged one)
  AND `…/udp_match_connection_1.h` (or a `_1` sibling) with DIFFERENT member
  types/sizes, each ending in its own `STATIC_SIZE_ASSERT`;
- one record's layout matches the base; the other is what the divergence flagged.

This is the same root cause as the `[size]` "phantom timer record" trap in
`review_todos.md` (ai_world/search) — the structure extractor picks a
stale/duplicate record from the type stream, while `pdb_fetch` on the emitted
FUNCTION reads the real one.

### TWO different phantom sources — don't conflate them

1. **Target-side duplicate record** (the `_1` sibling above): the SHIPPED
   `survarium.pdb` genuinely carries two records; the base was already right.
2. **Base-side STALE record from incremental relink**: MSVC accumulates old type
   records in `survarium-dx11-win32-gold.pdb` across links, so after you EDIT a
   header and rebuild, `pdb_divergence` may still read the BASE's pre-edit layout
   and report the divergence as unchanged — even though your fix landed. The tell:
   `report.json` (objdiff vs the freshly-compiled `.obj`s) shows the consuming
   functions jumped to 100%, but `pdb_divergence` still prints the old `[member]`
   row. **`report.json` is authoritative for "did my layout fix land"; the PDB-vs-PDB
   divergence is only trustworthy after a CLEAN RELINK** (`rm` the exe + pdb [+ ilk]
   and rebuild) flushes the accumulated records. Verified 2026-06-25: particle_system_lod
   reorder / task_allocator `[4096]` / character_move_test_callback const-members /
   delayed_packets_predicate rename all read 100% in report.json with 0 regressions,
   while a dirty-PDB `pdb_divergence` still showed every pre-edit row.

## Resolution — the emitted function asm is the tie-breaker, NOT the type stream

Read the consuming function's asm with `pdb_fetch --view target` and look at how
the member is actually accessed:

- a `mov [eax], cx` (16-bit word store) / `mov cx, [ebp+8]` into the member, or a
  2-byte-spaced offset sequence (`+0x124`, `+0x126`, `+0x128`, `+0x12a`) ⇒ the
  member is `u16`/2 bytes → the `<u16>` record is real, the `<u8>` flag is phantom.
- a `mov [eax], cl` (byte store) / 1-byte-spaced offsets (`+0x124`, `+0x125`, …)
  ⇒ the member is 1 byte → the `<u8>` record is real.
- a `cmp edx, 1000h` loop bound / `add ecx, 60h` stride in a ctor proves an array's
  element count (e.g. `task_allocator::m_task_buffer[4096]`, granularity 0x60) over a
  flagged-but-phantom array size.

Also check the function's `--view info` recorded PARAM/LOCAL types: if the body's
params/locals are `<u16>` and the divergence claims the matching MEMBER is `<u8>`,
the member-flag is the phantom (the source bridges the two with a
`reinterpret_cast`, but a value STORE width never lies).

Verified network_core cluster (2026-06-25, divergence-member sweep): the
`sequence_number<u8>` flags on `udp_match_connection` (+ `udp_match_client_session`,
`udp_match_server` member_hook constant, `packets_in_list_predicate`,
`sequence_id_predicate`) and `udp_match_client`/`match_client_impl` `0xB20`/`2048`
flags were ALL phantom — the emitted `update_acknowledgements` stores 16-bit words
at 2-byte-spaced offsets, and `match_client_impl`'s ctor builds a `0x2000*0x12C`
arena (8192, not 2048). Base was already correct; every edit was reverted.

## Counter-example (a REAL `[member]` flag)

Not every flag is phantom. `particle_system_lod` had a genuine field REORDER
(`m_parent` first @0x0 in target vs @0x18 in base) with a SINGLE canonical record —
reorder the declarations to match. The discriminators: only ONE record exists for
the type, and a reorder/one-sided-field/offset-shift (not just a type-spelling
`enum X`/`const X`/`<u8>`-vs-`<u16>` swap) shows up with no contradicting asm.
