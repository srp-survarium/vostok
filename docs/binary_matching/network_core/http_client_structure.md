# http_client.cpp - inline `// <addr>` annotation legend

This documents the inline statement annotations that the http_client matcher left
in `sources/vostok/network_core/sources/http_client.cpp` while reversing the file.
They were scratch markers (now deleted from the source, recorded here for future
matchers). Two forms appeared.

## Form 1 - the addressed annotation: `// <0xVA>|0xOFF|0xDELTA[|[n]]:'LL'`

Example (from `http_client::get`):

    request_stream << "GET " << path << " HTTP/1.0\r\n";   // <0x78ac21>|0x052|0x014:'40'

Fields, left to right:

| field      | example     | meaning |
|------------|-------------|---------|
| `<0xVA>`   | `<0x78ac21>`| BASE-build virtual address of this statement (the base PDB's text VA). NOT a target rva - it is off the target by ~0x10000 (target `get` is 0x77abc0, base 0x57c260). Pasting it into the *target* index misses; it is only scratch for the base build. |
| `0xOFF`    | `0x052`     | byte offset of this statement from the FIRST annotated statement of the function (i.e. `<VA> - <VA of the function's first body statement>`). It is cumulative; it grows monotonically down the body. |
| `0xDELTA`  | `0x014`     | the running delta = `OFF(this) - OFF(previous annotated statement)` = how many bytes the PREVIOUS statement occupied (the gap from the previous statement to this one). The first statement has `0x000` (no previous). A large delta means code was inlined/elided into the gap. |
| `[n]`      | `[1]`       | (optional) this statement OPENS lexical block `n` - a `for`/`while`/`if`/braced scope. Seen on `handle_resolve` L70 `tcp::endpoint endpoint = *endpoint_iterator;` (`[1]`, the `if ( !err )` block) and `handle_read_status_line` L122 (`[1]`). |
| `:'LL'`    | `:'40'`     | the ORIGINAL TARGET source line number of this statement (from the target PDB's line table). These `'LL'` labels are why the source keeps bare `// N` placeholders (Form 2) - so our physical line numbers stay aligned with the target's. |

Worked check on `read_lines_from_stream` (first statements):

    ASSERT( prefix );                       // <0x789e63>|0x000|0x000:'15'
    std::istream response_stream( &buff );  // <0x789e88>|0x025|0x025:'16'
    std::string str;                        // <0x789ed6>|0x073|0x04e:'17'

- `OFF`(16) = 0x789e88 - 0x789e63 = 0x025 (matches).
- `DELTA`(16) = OFF(16) - OFF(15) = 0x025 - 0x000 = 0x025 -> the ASSERT statement is 0x25 bytes.
- `OFF`(17) = 0x073; `DELTA`(17) = 0x073 - 0x025 = 0x04e -> the `std::istream` ctor statement is 0x4e bytes.

## Form 2 - the bare placeholder: `// N`

Example (from `http_client::get`, between L33 and L39):

    m_on_content_downloaded = callback;   // <0x78abf8>|0x029|0x029:'33'
    // 1
    // 2
    // 3
    // 4
    // 5
    std::ostream request_stream ( &m_request_buff );   // <0x78ac0d>|0x03e|0x015:'39'

The target's statement list jumps straight from source line 33 to line 39 (the
target structure shows L33 then L39 with no statement in between - those source
lines emitted no standalone breakpoint because the `boost::function` copy / setup
on lines 34-38 was fully inlined / LTCG-elided). The bare `// 1`..`// 5` are
no-address PLACEHOLDER lines: they hold the physical line count so that line 39 of
our `.cpp` lines up with the target's `'39'` srcline label. They carry no address
because the compiler set no breakpoint there. They are pure scratch and emit
nothing.

## KEY pattern - a loop/if `}` carries the breakpoint, the `{` does not

In a `/Od` loop the CLOSING brace gets a real breakpoint address (the back-edge /
scope-exit `jmp short`, ~2 bytes) while the OPENING brace gets none. Seen in
`read_lines_from_stream`:

    while ( std::getline( response_stream, str ) && str != "\r" )   // <0x789ef8>|0x095|0x022:'19'
    {   // 1                       <- opening brace: bare placeholder, NO address
        // 2
    }                              // <0x789f72>|0x10f|0x07a:'22'   <- closing brace: HAS an address

and in `add_result_content`:

    for ( ; std::getline( response_stream, str ) && str != "\r" ; )  // <0x789d31>|0x078|0x022:'163'
    {
        m_result_content.append( str );                             // <0x789dab>|0x0f2|0x07a:'165'
    }                                                               // <0x789ddc>|0x123|0x031:'167'

The base structure dump confirms it: `read_lines_from_stream` shows a statement at
L22 with size `<0x2>` (the `jmp short` back-edge of the `while` = the `}`), then
L23. So the `}`-with-an-address line in the annotations marks where a braced loop
body closed - the same `+0x002`/`jmp short` signal that `MATCHING.md` /
`assembly_patterns.md` describe for switch braces, here for the loop back-edge. The
opening `{` is brace-only and gets no line-table entry, hence its bare `// N`
placeholder.

## Relation to the two-sided structure-diff

These inline annotations are the ONE-SIDED base-build carcass. The authoritative
structure check is the two-sided `pdb_fetch ... --view structure-diff --condensed`
(target vs base aligned), which is what the per-function `// STRUCTURE DIFF` embeds
in this file now use for the non-100% bodies. The inline `// <addr>` / `// N`
scratch has been removed in favor of those embeds.
