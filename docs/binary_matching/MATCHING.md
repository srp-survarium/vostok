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


## LTCG is an excuse ONLY for function arguments - everything else is a matching problem
Every module we match is built with LTCG (`/GL`) - the active set (game_core,
network_core, logging) and the rest of the `/Od` set listed above (sound, network,
vfs, particle, ai, ai_navigation, fs, debug). It is tempting to wave off any leftover
diff as "uncontrollable LTCG"; that is almost always wrong and has hidden real,
source-steerable bugs. **The ONLY differences you may attribute to LTCG and
stop on are at the call boundary:**
- an **argument dropped** because the optimizer proved it constant call-site-wide, or
- an **argument passed in a register** instead of its stack slot (a custom calling
  convention chosen at link time).

**EVERYTHING ELSE IS A PROBLEM IN MATCHING - solve it from source.** Register choice,
`[ebp-XX]` slot assignment, frame size (`sub esp,N`), switch-dispatch shape, an extra
`cmp/ja` bounds check, a stray `fld1`, inline-vs-call of a real function, statement
order - none of these are "LTCG noise". Each has a concrete source cause:
- a frame-size / slot shift is usually a **missing `ASSERT`** (the `empty_stub` call,
  below) or a missing local;
- a switch bounds check (`cmp max; ja default`) is usually a **missing `case` plus a
  `default: NODEFAULT();`** (`__assume(0)`) - see the switch section;
- a `fld1` / const reached via the `default` may belong to an **explicit `case`**.

Keep digging until the ONLY remaining difference is argument passing; mark `DONE`
only then (e.g. `// STATE[97%|DONE]: LTCG arg passing`). If you cannot finish on this
machine, mark `INPROGRESS` with the concrete next step - do **not** bank it as a
matched `PARTIAL` and call the residual "LTCG". (History: `empty_stub` calls and a
switch bounds check were both wrongly written off as "LTCG" and were in fact a
recoverable ASSERT and a missing `default: NODEFAULT()`.) Trust the operand-aware
match % (`agentic_loop.md` section 2a) over a raw instruction-difference count.


## Scope
If a function exists in target but its matching source file is not provided in base - skip it. Currently we only try to match skeletons that were already defined.

**A helper you reconstruct to resolve a call is itself a match unit - it gets a
structure and a tracked %.** If, while matching function A, you discover A calls a
missing function B (e.g. an out-of-line `call vostok::math::get_relative_matrix`) and
you write B to make the link resolve, B is NOT invisible scaffolding: it is a real
function in the target binary with its own body and rva. You MUST (1) define B in its
ONE real source location (its real header, never the consuming `.cpp` - see the
`_N.h` note under "The carcass"), (2) give B its target carcass / `// FUNCTION BODY`
structure, and (3) track B's OWN `fuzzy_match_percent` and a `STATE`/ledger line, the
same as any matched function. A call that resolves but whose callee body is unverified
is a half-match hiding an unmatched function.


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


## Asserts (Master Gold drops the condition, but a CALL remains)
`Master Gold` never evaluates the assert *condition* - but `ASSERT` does NOT vanish.
It emits a real `mov byte[ebp-1],0; lea eax,[ebp-1]; call empty_stub` sequence
(~`0x0c` bytes). **So a `call empty_stub` in the target asm IS a compiled-out
`ASSERT` - recover it, do NOT write it off as an inlined/stripped/LTCG call.**
(The delinker may misname `empty_stub` as `finalize_impl` etc. - don't take it
literally.) Confirmed house style: `booby_trap_core.cpp` / `inventory_cook.cpp` map
`ASSERT( UNKNOWN_EXPRESSION );` to a real `+0x0c` slot.
- `ASSERT( UNKNOWN_EXPRESSION )` - a condition was here but you don't know it; this
  alone reproduces the `call empty_stub` bytes (the condition is discarded).
- `ASSERT( UNKNOWN_EXPRESSION_T( your_guess ) )` - prefer this: the `_T` form holds
  your *guess* of the condition (risk-free, discarded). Guess what was asserted.
When you see the `empty_stub` sequence at a statement, place an `ASSERT(...)` there.

`UNKNOWN_EXPRESSION` / `_T` are intentionally **undefined** - never define or
"fix" them. Add `STATIC_SIZE_ASSERT( type, 0xNN )` after each reconstructed
struct to pin its PDB size.

## Switch statements - case-body braces change codegen
The bracket style around a `case` body changes codegen/structure. **Read the carcass
to decide braces - not "does it have a local":**
- **A `+0x002` step (a 2-byte `jmp short` = a closing brace `}`) in the
  `// FUNCTION BODY` carcass marks a braced block.** Under `/Od` the `}` of a braced
  `case` (or a braced `if` inside one) emits that `jmp`. If the structure shows a
  `+0x002`/`jmp` for a case, brace it - whether or not it has a local; a case with no
  `+0x002` entry is brace-less. (We mis-handled this on `get_target_koef` and earlier
  switches by reading "no local -> no braces" instead of the structure; revisit prior
  switch matches against this.)
- The `{ }` is a real scope (it controls where temporaries are built/destroyed and the
  `[ebp-N]` layout). A case that declares a local or holds an `ASSERT`/temporary will
  show the `}` jmp and needs the braces - but the carcass is the signal, not a guess.
  Braced: `inventory_cook.cpp`, `items_cook.cpp`, `booby_trap_core.cpp`
  (`case X: { ...; ASSERT( UNKNOWN_EXPRESSION ); ...; break; }`); brace-less:
  `character_recoil_calculator.cpp`, `inventory.cpp`.
If a switch won't match, the case-brace scoping is a prime suspect.

**The `default:` shape controls the dispatch (bounds check vs none).** Compare the
jump-table dispatch:
- TARGET `jmp dword ptr [reg*4+table]` with **no** preceding `cmp max; ja default` =
  a contiguous jump table with **no bounds check**. MSVC emits this only when the
  source covers the full contiguous case range AND the `default:` is provably
  unreachable - i.e. `default: NODEFAULT();` (`NODEFAULT` = `__assume(0)` in Master
  Gold, `sources/vostok/debug_macros.h`; `NODEFAULT( return x );` / `UNREACHABLE_CODE`
  for the value-returning form).
- A bare `default: return X;` (a reachable default) makes MSVC emit `cmp max; ja
  default` and bound the table at the highest explicit `case` - an **extra** check the
  target won't have. If your base has a `cmp/ja` the target lacks, you are missing the
  top `case`(s) and a `default: NODEFAULT();`. A value the target reaches *through* the
  table (e.g. a final `fld1`) is an explicit terminal `case`, not the `default`.

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


## Comment hygiene - lean code, verbose `.md`
**Code is not the place to be noisy.** Keep inline comments minimal. ALL the
explanation, exploration, attempts, and rationale belong in the per-function `.md`
(section 7) - be as detailed as you like *there*, never in the source. A reader of
the `.cpp` should see matched code, not a narration.
- **The carcass `// <full signature>` line is only a TYPE reference**, kept while
  matching because vostok-structure's generated argument *names* can be wrong while
  the *types* are right. **Once the arguments/types match the target, DELETE it** - a
  confirmed match does not keep the signature line.
- **A clean `100%|DONE` keeps ONLY its `// STATE[100%|DONE]` line** - no explanation
  block above the function (the why-it-matched detail goes in the `.md`). The single
  exception is the non-100% carcass-structure rule below (keep structure inline).
- Reserve inline comments for the genuinely unexpected/unique: a `claude@MATCH:` for
  an odd shape chosen to reproduce bytes, or a `claude@NOTE:` for a surprising target
  fact. Do NOT narrate routine mechanics (anchors, "empty body", ICF/linker folding)
  inline - and never state something you have not verified (e.g. "no symbol / not
  scorable": empty functions DO get a standalone symbol and appear in objdiff).
- **Strip unnecessary logs.** Remove any logging / diagnostic statement (`LOG_*`,
  `LOGF_*`, `printf`, `OutputDebugString`, a trace call) or commented-out debug/log
  line that was added while matching and that the TARGET does not actually emit - it is
  not part of the byte-match and only clutters the source. (A log the target's bytes
  genuinely contain stays.) Any diagnostic you needed belongs in the `.md` trail, never
  as a leftover log line in the `.cpp`.


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

**`+delta` reads structure - and `+0x002` is almost always a closing brace `}`.** A
2-byte step is a `jmp short` (EB xx); inside a switch that `jmp` is the `break` /
scope-exit emitted for a `}`, so a carcass entry with `+0x002` marks where a braced
block closed. /Od emits it even when the block's last statement `return`s (dead, but
not removed). **So the carcass tells you where the braces were:** a `case` (or an
`if` inside one) that shows a `+0x002`/`jmp` entry was `{ }`-scoped, and your source
must brace it to match - do NOT decide braces from "has a local" alone, read the
structure. (Conversely, a `case` with no `+0x002` entry was brace-less - e.g.
`get_broken_hands_penalty`, whose cases show `+0x004`/`+0x2b` and no `}` jmp, matched
without braces.)

**`[n]` marks the start of lexical block `n`; a local tagged `<n>` lives in that block.**
A body line can carry a trailing `|[n]:` field (e.g. `<0x5934e2>|0x012|+0x021|[1]:'35'`) -
the `[n]` marks the statement that OPENS block `n`: a `for` / `while` / `if` / braced scope
(so that example is a loop opening at src line 35). Correspondingly, a `// LOCALS` entry
tagged `<n>` (e.g. `// ai::fsm_state*   state<1>`) is a local **declared inside block `n`**
- here `state` lives in that loop, not at function scope; declare it there, not at the top.
This block/scope mapping is reliable only in the non-optimized (`/Od`) target. (The rule is
more nuanced than this, but this is the working approximation.)

**Match the base structure to the target structure - not just the byte %.** After a build,
your function has its OWN structure in `binaries/structure/base/<unit>` (the base PDB's
statement/block layout); the target's is the `// FUNCTION BODY` carcass (the original
source's shape). COMPARE the two: the same set of `'srcline'` statements, the same `[n]`
block-opens, and the same `<n>` (no-address) lines should appear on BOTH sides.
**Compare each statement's SIZE too:** the per-statement `+delta` (bytes to the next
statement) should roughly agree between base and target - a statement whose `+delta`
differs is exactly where the codegen diverged, a fast way to localize the mismatch before
a byte diff. A block the
base has that the target lacks - a `[n]`/`<n>` from braces you added, a missing early
`return`, a different loop/`if` shape - means your CONTROL STRUCTURE diverges, even when the
fuzzy % is high. **A high % over the wrong structure is not a match;** fix the source's shape
to the target's. (Caught on `set_breath_holding_params`: an `if ( p ) { ...4 stores... }`
emitted a block-open the target structure lacked - the target was an early `if ( !p )
return;` guard, no braces - and the 76.80% hid it. The fix is a source restructure, not a %
to bank.)

**Keep the target structure inline when the match is NOT 100%.** If a function
ends `PARTIAL` / `INPROGRESS` / `BLOCKED` / `SKIPPED`, leave the `// FUNCTION BODY`
block (and, for the diverging region, the relevant `pdb_fetch --view target` asm as
a comment) in the source above/inside it, so the next reader has the full divergence
context in place - not buried in a PR or a log. This is the existing house style; see
`collision_sensor.cpp`, `player_stamina.cpp`, `damage_model.cpp`. Only a clean
**100% DONE** match may delete the carcass for tidiness.

**When the match is NOT 100%, PRESERVE the `// FUNCTION BODY` block verbatim -
including its `<0> <1> <2>` marker lines. Never strip them** (a clean 100% DONE deletes
the carcass entirely, per the rule just above). A `<N>` (no address) line is a statement/sub-expression
the compiler set no breakpoint on (inlined, optimized out, or a continuation); its
count and grouping between two addressed lines are a *structural clue* (an inlined
call, a nested scope, a fall-through `jmp` thunk). When you record which statement a
carcass line matched, write that annotation to the **RIGHT** of the line and leave the
addressed and marker lines intact - e.g.
`// <0x...>|0x025|+0x008:'106'\tcase type_stand: if ( is_moving )`. Of the generated
blocks, keep only `// FUNCTION BODY`; the `// STATICS` / `// OTHER SYMBOLS` /
`// LOCALS` / `// TYPEDEFS` comment blocks are scratch and may be dropped - but mine
`// LOCALS` for the rule below first.

**Every `// LOCALS` entry is a real source local - declare and use ALL of them.** If
the PDB recorded a variable as a local of the function, it WAS in the target source,
so your match must declare it. Under `/Od` every local is allocated a stack slot, so a
recorded local always shows up - a missing local is a missing statement; never drop one
because it "looks unused". (Slot number / ordering is still allocation noise - below.)

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

**A `<header>_N.h` filename in `binaries/structure/target` does NOT mean the header
was split into numbered files.** The structure generator emits one file per
*compiland instance* of a header and disambiguates duplicates with a `_1`/`_2`/...
suffix - so `math_float4x4_inline_2.h` is just the SAME real `math_float4x4_inline.h`
as it was included into a different `.cpp`. The shared include guard (all the `_N`
variants carry the base file's guard) is the tell. Put a reconstructed function in
its ONE real header (`math_float4x4_inline.h`), never invent a `_2.h` and never define
a math helper in the consuming `.cpp` just to resolve a call.

**Local `[ebp-N]` slot *numbers and ordering* are allocation noise** - MSVC `/Od`
does NOT assign slots in declaration order (observed e.g. four bools at
`[ebp-2],[ebp-4],[ebp-1],[ebp-3]`). Match the source *statement order* (which is
reliable); never reorder declarations to chase a slot number.
