# network_core - inline structure annotations & legend

This file documents the `// <addr>` inline annotation pattern that the carcass
generator and matchers left scattered across the network_core sources
(`sources/tcp_packet.cpp`, `tcp_packet_inline.h`, `udp_match_stats.h`,
`sources/network_core_entry_point.cpp`), and how to read it.

These annotations are TRANSIENT scaffolding - a per-statement map produced from
the TARGET PDB line table while a function was being matched. Once a function is
verified they are deleted (the standard `// STRUCTURE DIFF` + `// VERDICT:` embed
replaces them for non-100% functions; 100% functions carry nothing).

## The `// <VA>|off|delta:'srcline'` row

A single statement's row looks like:

    // <0x6e7975>|0x08f|0x00b:'27'

Fields, left to right:

- `<0x6e7975>` - the TARGET virtual address (a breakpoint-able instruction
  address) where this source statement begins. These are TARGET-build VAs; the
  base-build VA is offset by ~0x10000, so never compare the absolute value -
  compare the SHAPE (which `'srcline'` are present, their sizes, their order).
- `0x08f` - the statement's byte OFFSET from the start of the function body.
- `0x00b` - the byte DELTA to the next statement (i.e. this statement's size in
  bytes). Written `+0xNNN` in some rows; the `+` is cosmetic.
- `'27'` - the SOURCE line number (1-based) in the owning `.cpp`/`.h` this
  statement maps to.

A bare `// <N>` row (no `<addr>`, just a small integer) is a no-address /
secondary line - a continuation of a multi-line statement, or a synthetic
compiler line that emitted no standalone breakpoint. A `[n]` token in a row
(e.g. `0x02b|[1]:'144'`) marks a lexical BLOCK-open at that statement (a `for` /
`while` / `if` / braced scope opening); a `// LOCALS` entry tagged `<n>` is a
local declared INSIDE block `n`, not at function scope.

## KEY pattern: a loop/`if` CLOSING `}` carries the breakpoint, the OPENING `{` does not

The most load-bearing thing to know when reading these rows against C++ source:

- The OPENING brace `{` of a `for` / `while` / `if` body does NOT get its own
  instruction address - control simply falls into the body, so the opening brace
  line has no `<addr>` row (or shares the condition's row).
- The CLOSING brace `}` of that body DOES carry a breakpoint address - it is
  where the loop back-edge / scope-exit / fall-through `jmp short` lives. So a
  `+0x002` step (a 2-byte `jmp short`) on a `}`-line is the signature of a
  braced block CLOSING.

Worked example from `~tcp_packet` (`tcp_packet_inline.h`):

    if ( buffer( ) )                                   // <0xa72b0>|0x000|0x000:'21'   <- if-condition, opens block; brace `{` itself: no row
    {
        pbyte real_buffer = buffer( ) - 3;             // <0xa72b6>|0x006|0x006:'23'
        VOSTOK_FREE_IMPL( m_allocator, real_buffer );  // <0xa72b9>|0x009|0x003:'24'
    }                                                  //   <- closing `}`: this is where the breakpoint/scope-exit lands

Read it as: the `{` opening the `if (buffer())` block is invisible in the line
table; the statements inside carry rows; the `}` closing it is where codegen
emits the exit. So when you decide whether a `case`/`if`/loop in source was
BRACED, you look for the `+0x002` closing-brace step in the carcass, NOT for the
presence of a local (a brace-less single statement can still declare nothing).

## `FUNCTION BODY[0xVA]: N` / `******` blocks

Some functions carried a full dumped body block, e.g.

    // FUNCTION BODY[0x91fa0]: 15
    // <0x91fa8>|0x008|+0x007:'212'
    // ...
    // ******

`[0xVA]` is the function's target entry VA; `: N` is the target statement count;
each row is a statement (same `<VA>|off|delta:'srcline'` grammar); `******`
closes the block. `// LOCALS` ... `******` similarly lists the function's locals,
each `name<n>` tagged with its owning block.

## Disposition

All of the above is scaffolding. After verification:
- A 100% function carries NO annotation block at all.
- A non-100% function carries exactly the standard condensed `// STRUCTURE DIFF`
  block + one `// VERDICT:` line, embedded at the END of the body (before the
  closing `}`), per the structure-verifier format in MATCHING.md.

The raw `// <addr>` rows documented here were deleted from the network_core
sources once this legend was recorded.
