# A multi-line statement's line record lands on its CLOSING line - reconstruct the source layout from the record deltas

tags: cpp:call cpp:array cpp:initializer | asm:none | topic:structure-shape topic:pdb-locals topic:line-records

## Symptom
A function's `--view structure` shows line records with wide, irregular gaps -
`... 340 | 347 | 349` - and no obvious statement to put in the hole. Guessing
"the call is at 344 and there are three blank lines" produces a source whose
statement count and record spacing never line up with the target.

## The rule
For a statement spelled over several source lines, MSVC files the line record at
the line holding the **closing `);`** (or `};`), not at the line that opens it.
The same holds for a braced array initializer: the records land on the
individual **element lines**, and everything from the last recorded element to
the closing `);` of a following multi-line call is one contiguous stretch of
source you can count out exactly.

Calibrated on `system_renderer::draw_aabb`, whose repo source is already
line-for-line faithful:

| | `if` row | first element row | last element row | call row |
|---|---:|---:|---:|---:|
| base (source lines known) | 803 | 807 | 812 | **823 = `);`** |
| target | 576 | 580 | 585 | **595** |

Base source 813-823 is: elements 7-8, `};`, blank, `draw_lines\t(`, five
argument lines, `);`. The target's 586-595 is the same shape one line shorter.
The record is on `);` on both sides - not on `draw_lines (`.

## Using it
`--view structure` deltas are enough to lay the source out line-for-line before
the first build. Worked example, `portal_sector_system::draw_portals`
(target records 323,324,325,326 | 329,330,331 | 333,334 | 338,339,340 | 347 | 349):

```
322	u16 const frustum_indices[]		= {   <- no record: the decl line emits nothing
323		0, 1, 2,
324		0, 2, 3,
325		0, 2, 1,
326		0, 3, 2
327	};
328
329	spatial_sector const& s			= m_structure->get_sectors( )[active_sector_id];
330	u32 const* const portals_end	= s.get_portals( ) + s.get_portals_count( );
331	for ( u32 const* i = s.get_portals( ); i != portals_end; ++i )
332	{
333		portal const& p				= m_structure->get_portals( )[*i];
334		if ( !p.is_visible( ) )
335			continue;
336
337		vertex_colored const vertices[] = {
338			vertex_colored( p.get_points( )[0], math::color( 0x6464c864 ) ),
339			vertex_colored( p.get_points( )[1], math::color( 0x6464c864 ) ),
340			vertex_colored( p.get_points( )[2], math::color( 0x6464c864 ) ),
341			vertex_colored( p.get_points( )[3], math::color( 0x6464c864 ) ),
342		};
343		renderer.draw_triangles	(
344			vertices, vertices + array_size( vertices ),
345			frustum_indices, frustum_indices + array_size( frustum_indices ),
346			false
347		);
348	}
349	draw_quads( renderer );
```

That source compiled to a base whose `--view structure` reproduced **every one of
the target's line deltas** (`+6, +1, +1, +2, +1, +4, +1, +1, +7`), i.e. the
whitespace was recovered, not merely the semantics. Two constraints do the work:

* **the last element of an initializer usually has NO record** - its code folds
  into the following call's block, so `};` sits one line past the last recorded
  element (here 340 -> element[3] at 341, `};` at 342);
* **the delta from the last element record to the call record is the call's own
  line count**: +7 = `};`, open paren, 2 argument lines, `false`, `);` with no
  blank line before the call. A +10 would have bought a blank line and one
  argument per line.

## Carcass `<N>` markers give the gaps for free
A STATE[STUB] carcass writes the no-code lines out explicitly:

```
// <0x648908>|0x048|+0x021:'1623'
// <0>  <1>  ... <10>            <- eleven source lines emitting nothing
// <0x648929>|0x069|+0x03c:'1635'
```

Counting those against the shape above pinned `renderer::draw_frame_histogram`'s
loop exactly: `<0>..<4>` (five lines) between the `for` row 1636 and the body's
first row 1642 is not five blank lines - it is `{` plus a **five-line
`lines_time[count] = float3( ... );`** whose record lands on its `);` at 1642.

## Do not over-read
Line numbers do not affect codegen, so a wrong guess costs nothing in bytes -
but a right one is free evidence about statement COUNT and grouping, which do.
Use it to choose between "one statement per component" and "one float3
constructor", never to invent a statement that emits no code.

## Related
- `nested-if-vs-multiline-and.md` - the same last-line rule for a split `&&` condition.
- `closing-brace-jmp-rows.md` - when a closing brace gets its own record.
