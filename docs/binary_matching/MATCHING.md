# MATCHING.md - writing matched source (non-optimized modules)

Naming conventions:
    * `target` - original executable we are trying to match against.
    * `base`   - our own executable we compiled with decompiled source code.

Your goal is to match `base` as close to `target` as possible. This doc is the
*shape of the code*; the *process* (queue, rebuild/diff, commit-per-function) is
in [agentic_loop.md](agentic_loop.md).

## Reproduce the target EXACTLY - never "fix" anything (the #1 rule)
The `target` binary is the **only ground truth**. Your job is to write source that
reproduces *its* instructions byte-for-byte - **including bugs, dead code, odd
logic, off-by-ones, and anything that looks wrong.** You are NOT improving the
code; you are recreating it.

- **Never fix a bug.** If the target does something that looks broken, the original
  source did that, so your source must do it too. "Correcting" it makes your bytes
  diverge from the target - that is a *worse* match, not a better one.
- **The disassembly decides, not your judgment.** If the target reads member
  `m_foo` (a wrong-looking field), reproduce `m_foo`. Pick the source that emits
  the target's actual instructions; never pick source because it reads "more
  correct" or because it nudges the fuzzy % when the asm says otherwise.
- **Exactness beats every other consideration** - readability, idiom, perceived
  correctness. A faithful reproduction of a buggy original is the goal.

Do not describe your work as "fixing" - you are *matching*. (Caught on
`scheduler::on_frame`: a worker called a member-selection change a "bug fix"; the
correct framing is that the target's asm reads that member, so the source must.)

The modules we match are the ones the shipped `Master Gold` build compiled with
optimizations **disabled** (`/Od`). That means the binary is very close to the
original source and the `.pdb` carries a lot of useful info: every statement
location, local definition and function argument. The `/Od` set is read from the
**target** codegen (a `/Od` function keeps the `push ebp; mov ebp, esp` frame,
which optimized/FPO code drops) — *not* from the `.vcproj`, which is base config
and can differ (e.g. `network`'s vcproj says `/Oy` omit-frame-pointer, but the
target kept the frame).

Actively being matched now:
    * `game_core`       lives in `namespace survarium`.
    * `network_core`    lives in `namespace vostok { namespace network_core {`.
    * `logging`         lives in `namespace vostok { namespace logging {`.

Matchable too (also `/Od` in the target), not started yet:
    * `sound`, `network`, `vfs`, `particle`, `ai`, `ai_navigation`, `fs`, `debug`.

Everything else (`physics`, `collision`, `game`, `render`, `core`, `engine`,
`animation`, `ui`, `input`, `scaleform`, `survarium`/EXE) was built **optimized**
and does **not** map 1:1 — out of scope for this approach.

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
- Names - **everything is `snake_case`, all-lowercase. NO CamelCase / PascalCase anywhere** (not types, not functions, not variables, not enum values). Agents get this wrong - it's a hard rule, verified against matched code:
    - **Types** (class / struct / enum / typedef): `snake_case`, e.g. `weapon_dispersion_calculator`, `udp_match_packet`, `ik_processor`, `bt_character_controller`, `body_part_parameters`. Never `WeaponDispersionCalculator`.
    - **Acronyms / initialisms are lowercased and treated as ordinary words**: `udp`, `ik`, `id`, `ai`, `bt`, `http`, `2d`/`3d`, `cc` (console command), `perc` (percent). So `udp_match_packet`, `get_initiator_id`, `ammo_id_enum` - never `UDP`, `IK`, `ID`.
    - **Members** `m_` + snake_case (`m_air_resistance`); **globals** `g_` (`g_allocator`); **file statics** `s_` (`s_aim_transition_time`), console-command statics `s_<name>_cc` (`s_dispersion_enabled_cc`).
    - **Functions / methods / locals / params**: snake_case (`time_delta_in_sec`, `get_target_koef`).
    - **Enum types** end `_enum` (`weapon_user_state_enum`); enum **values** are snake_case.
    - Functor helpers `<verb>_predicate : boost::noncopyable`.
  When unsure, copy the exact spelling from the target PDB symbol / the nearest matched file - do not invent a casing.
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


## Comment hygiene - keep matched code lean
In-code comments are minimal; verbose rationale lives in the per-function `.md`
(section 7), never inline.
- **The carcass `// <full signature>` line is a TYPE reference**, kept while matching
  because vostok-structure's generated argument *names* can be wrong while the
  *types* are right. Once you have confirmed the signature, **delete it** - a
  confirmed match does not keep it.
- **A clean `100%|DONE` keeps ONLY its `// STATE[100%|DONE]` line** - no explanation
  block above the function (the why-it-matched detail goes in the `.md`). The single
  exception is the non-100% carcass-structure rule below (keep structure inline).
- Reserve inline comments for the genuinely unexpected/unique: a `claude@MATCH:` for
  an odd shape chosen to reproduce bytes, or a `claude@NOTE:` for a surprising target
  fact. Do NOT narrate routine mechanics (anchors, "empty body", ICF/linker folding)
  inline - and never state something you have not verified (e.g. "no symbol / not
  scorable": empty functions DO get a standalone symbol and appear in objdiff).


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
between. Use these as scratch while matching.

**Keep the target structure inline when the match is NOT 100%.** If a function
ends `PARTIAL` / `BLOCKED` / `SKIPPED`, leave the `// FUNCTION BODY` block (and,
for the diverging region, the relevant `pdb_fetch --view target` asm as a comment)
in the source above/inside it, so the next reader has the full divergence context
in place - not buried in a PR or a log. This is the existing house style; see
`collision_sensor.cpp`, `player_stamina.cpp`, `damage_model.cpp`. Only a clean
**100% DONE** match may delete the carcass for tidiness.

**A multi-line statement carries its `<VA>` only on its LAST line.** The structure
/ carcass annotates by statement, not by physical source line, and the address is
attached to the statement's *final* line. So a `<>`-less line is not necessarily
inlined-away or a comment - it may just be the head of a multi-line statement
(a wrapped call, a multi-line `if (...)`, a member-init list) whose address sits a
few lines below. Read the whole statement before concluding a line was optimized
out.

**Carcass `<VA>` addresses are from the BASE build, not the target.** They differ
from the target rvas that `pdb_rich_query --list` / `--rva` report (two different
binaries; often off by ~0x10000). Pasting a carcass address into the *target*
index will miss. Use the carcass addresses only as scratch for the base build;
get the target asm by function name / target rva via `pdb_fetch --view target`.

**Local `[ebp-N]` slot *numbers and ordering* are allocation noise** - MSVC `/Od`
does NOT assign slots in declaration order (observed e.g. four bools at
`[ebp-2],[ebp-4],[ebp-1],[ebp-3]`). Match the source *statement order* (which is
reliable); never reorder declarations to chase a slot number.
