# MATCHING.md - writing matched source (non-optimized modules)

Naming conventions:
    * `target` - original executable we are trying to match against.
    * `base`   - our own executable we compiled with decompiled source code.

Your goal is to match `base` as close to `target` as possible. This doc is the
*shape of the code*; the *process* (queue, rebuild/diff, commit-per-function) is
in [agentic_loop.md](agentic_loop.md).

The current modules being matched are:
    * `game_core`       lives in `namespace survarium`.
    * `network_core`    lives in `namespace vostok { namespace network_core {`.
    * `logging`         lives in `namespace vostok { namespace logging {`.


All of those modules were compiled with optimizations disabled (specifically `/Od` flag) in the shipped `Master Gold` build. That means binary is very close to the original source code and .pdb contain a lot of useful information: all statement location, local definitions and function arguments.

When unsure, copy the nearest reversed file - good references are
`game_core/sources/{player_stamina,collision_sensor,damage_model}.cpp` and
`network_core/sources/http_client.cpp`. The existing code wins over this doc.


## LTCG (link-time codegen you cannot control)
All three modules are built with LTCG (`/GL`): some codegen is deferred to link
time with whole-program info, and you cannot steer it from source. Known effects:
arguments passed in different registers / stack slots (or dropped when proven
constant call-site-wide), a different **frame layout** (`[ebp-XX]` slot
assignment), and **inlining** chosen across module boundaries - plus other
effects we have not fully enumerated. It does not reorder the body.

When base and target differ only by these, it is an LTCG artifact, not a source
bug: recognize it, leave a `claude@NOTE:`, and do not contort the source to chase
it. A diff that is only register / `[ebp-XX]` slot differences is a **match** -
mark `DONE`; when inlining or the frame diverges and the body is as close as
source can make it, mark `PARTIAL`. Trust the operand-aware match %
(`agentic_loop.md` section 2a) over a raw instruction-difference count. e.g.
`// STATE[97%|DONE]: LTCG arg passing`, `// STATE[88%|PARTIAL]: LTCG inlined get_x()`.


## Scope
If a function exists in target but its matching source file is not provided in base - skip it. Currently we only try to match skeletons that were already defined.


## Style
- Hard tabs; align member-init lists and `=` columns with tabs.
- Space inside parens: `foo( a, b )`, `if ( cond )`, empty `( )`, `for ( u32 i = 0 ; i < n ; ++i )`.
- Function brace on its own line; single-statement bodies brace-less. (Brace style is free.)
- Engine typedefs only: `u8/u16/u32/u64`, `s8..s64`, `f32/f64`, `pstr/pcstr`, `pbyte/pcbyte`, `pvoid/pcvoid`, `float3/float4x4`. `NULL`, not `nullptr` (VS2008 / C++03).
- Casts: `static_cast<>`, `static_cast_checked<T>()`, `static_cast_resource_ptr<>`; C-style only where the target did one (e.g. `(float)config["x"]`).
- Names: members `m_snake`, everything else `snake_case`; enum types end `_enum`; functor helpers `<verb>_predicate : boost::noncopyable`; globals `g_`, file statics `s_`.
- Memory: `VOSTOK_NEW_IMPL( g_allocator, T )( args )`, `VOSTOK_DELETE_IMPL`, `VOSTOK_MALLOC_IMPL( g_allocator, n, "tag" )`, `VOSTOK_FREE_IMPL`. Stack vectors: `buffer_vector<T> v( ALLOCA( n * sizeof( T ) ), n )`.
- Prefer the STL the target used (`erase( remove_if(...), end() )`, `std::find/sort/unique`) over a hand loop.


## Asserts (compiled out in Master Gold)
`Master Gold` is a release config, so `ASSERT` / `NODEFAULT` / `UNREACHABLE_CODE`
/ `VOSTOK_UNREFERENCED_PARAMETERS` expand to nothing - the argument is never
parsed and never affects bytes. The original assert conditions are gone, hence:
- `ASSERT( UNKNOWN_EXPRESSION )` - a condition was here but you don't know it.
- `ASSERT( UNKNOWN_EXPRESSION_T( your_guess ) )` - the `_T` form holds your *guess* (risk-free, it is discarded).

`UNKNOWN_EXPRESSION` / `_T` are intentionally **undefined** - never define or
"fix" them. Add `STATIC_SIZE_ASSERT( type, 0xNN )` after each reconstructed
struct to pin its PDB size.

## STATE markers
One per function, line above it: `// STATE[<percent>%|<tag>]: short reason`.

| tag | meaning |
|---|---|
| DONE | matched (may be <100% if the remaining diff is LTCG/CRT noise - say why) |
| PARTIAL | mostly matched, remaining diff understood |
| STUB | skeleton only, body still the carcass |
| BLOCKED | needs another function/type first |
| SKIPPED | tried, deferred |
| INLINED | inlined at all call sites; no standalone body |
| UNCHECKED / UNVERIFIED | written, not yet diffed / not confirmed |

e.g. `// STATE[94.32%|DONE]: LTCG for mutex`, `// STATE[97.67%|PARTIAL]: target didn't xor after std::find`.


## Comment tags (where matching knowledge lives - keep them)
Three suffixes, author-prefixed. Existing notes are `sushi@...` (the original
matcher) - never drop them. Prefix your own with `claude@...`.
- `@MATCH:` a deliberate, often odd-looking shape chosen to reproduce the bytes (`fld1 used instead of movss`, `target does an extra copy`, `-> inlined in target`). Most valuable; write one whenever you do something unexpected to match.
- `@NOTE:` an observation about the target.
- `@TODO:` an open question.

"Why it didn't match / why I stopped" = the reason on the `STATE` line plus a
`@MATCH`/`@NOTE` comment at that statement (multi-line rationale goes in a comment
block above the function).


## The carcass (generated stub comments)
Stubs arrive with `// FUNCTION BODY` / `// LOCALS` / `// TYPEDEFS` blocks. A body
line is `<absoluteVA>|offset|+delta:'srcline'`: paste the VA into IDA (`G`);
`<N>` = no address (inlined/comment); a large `+delta` = something inlined
between. Use these as scratch while matching and delete them as you account for
each statement.
