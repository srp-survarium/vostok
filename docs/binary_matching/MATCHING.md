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

Keep digging until the ONLY remaining difference is argument passing; only then is
the function done (the ledger notes the residual: a `tried --note` cause, e.g.
`LTCG arg passing`). If you cannot finish, park it: a terse `claude@NOTE:` above the
function plus a park whose cause is the concrete next step (parks are recorded
via `vostok ledger park` - by the orchestrator for dispatched matchers) - do
**not** bank it as matched and call the residual "LTCG". (History: `empty_stub` calls and a
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
`_N.h` note under "The carcass"), (2) match B against its own target shape - fetch its
statement structure and rich asm like any unit (`pdb_fetch --function <B> --view
structure` / `--view target`), and (3) track B's OWN `fuzzy_match_percent` - it shows
up in `report.json` / the ledger like any function. A call that resolves but whose callee body is unverified
is a half-match hiding an unmatched function.

**A cross-TU helper is DECLARED in a header with the module's `VOSTOK_<MODULE>_API`
macro - never ad-hoc forward-declared in the consuming `.cpp`.** When B is defined
out-of-line in one `.cpp` and called from another, put its declaration in a shared
header (create the module's `api.h` if it lacks one, mirroring `vfs`/`ai`/`physics`)
prefixed with `VOSTOK_<MODULE>_API`, and include that header in both the defining and
the consuming `.cpp` so the symbol carries the correct DLL scope. The gold build defines
`VOSTOK_STATIC_LIBRARIES`, so the macro expands to nothing and this is byte-neutral for
the match - but the forward-decl-in-`.cpp` shortcut breaks DLL configs and is not how the
engine is structured. EXCEPTION: a helper that is `inline` in a header yet *deliberately*
forward-declared out-of-line in a `.cpp` to force the target's out-of-line `call` (a
matching device - e.g. `computed_*_animation_time_scale`, inline in
`weapon_animations_timescale_inline.h`) stays a forward-decl; header-izing it would inline
the call and change the match.


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
- `ASSERT( UNKNOWN_EXPRESSION_T( your_guess ) )` - **almost always use this.** The `_T`
  form holds your *guess* of the asserted condition (discarded, so byte-identical and
  risk-free) and documents intent. ALWAYS try to infer the guess from the function
  name / params / context - a non-null `this`/arg, a valid index, an in-range enum, a
  non-empty container, `value_exists(cfg[...])`, etc.
- `ASSERT( UNKNOWN_EXPRESSION )` - the bare fallback ONLY when you genuinely cannot
  guess; it alone still reproduces the `call empty_stub` bytes.
When you see the `empty_stub` sequence at a statement, place an `ASSERT(...)` there.

`UNKNOWN_EXPRESSION` / `_T` are already **defined** and pch-provided (in
`vostok/debug/macros.h`: `UNKNOWN_EXPRESSION` = `true`, `UNKNOWN_EXPRESSION_T( e )`
= `( true ? true : !!e )`) - just USE them, never `#include`, redefine, or "fix"
them. Add `STATIC_SIZE_ASSERT( type, 0xNN )` after each reconstructed struct to pin
its PDB size.

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

## Match status (the ledger; the only in-source marker is `STATE[STUB]`)
In-source `%`/status markers go stale the moment a rebuild moves the score, so they
are NOT written anymore. The source carries exactly two things:
- `// STATE[STUB]` on a body that is not matched yet (still the carcass / a
  placeholder) - the ONLY `STATE` marker allowed in source - plus that stub's
  signature/carcass comments (matcher input);
- `sushi@` / `claude@` `@MATCH/@NOTE/@TODO` comments (see "Comment tags").

Everything else is DERIVED and lives outside the source
(design: `ledger_design.md`):
- **current %s**: `report.json` / `vostok ledger readme` - the only live numbers.
- **bulk status, queues, reports**: `config/match_state.tsv`, regenerated
  from report.json + the rich indexes + the PDB declaration dump by `vostok build`
  at the end of every build (or, regen-only, by `python3 -m vostok derive
  refresh`). Per paired function it derives the structure class
  (`MATCH | SIZE | SPLIT | QUANTITY`), tracks pairing history ("matched at NN%
  before; vanished/regressed without a source touch -> out of scope"), and
  classifies base-only symbols (`NEAR_MISS` mangling mismatches, declared-but-
  inlined-in-target, the fabricated-symbol lint). Query it with
  `vostok ledger list / report / queue`.
- **structure-diffs**: run on demand (`pdb_fetch --view structure-diff`), never
  embedded in source.

Globals, constants, pointer tables, and BSS use the data-reconstruction lane;
see [`data_matching.md`](data_matching.md) and `python3 -m vostok data --help`.
Its linked-image audit never feeds the function score; only definitions split
into target COFF through the delink manifests enter objdiff's data denominator.

The only hand-written records are ledger parks and attempt notes:
- `vostok ledger park <mangled> --cause "..."` - stop working the function; the
  cause is the concrete next step (name the blocker in it). `--cause` is the
  verb's only flag.
- `vostok ledger open <mangled>` - undo a park, so queues offer it again.
- `vostok ledger tried <mangled> --note "..."` - record an attempt without
  parking (a DONE-with-residual explanation, inline-site evidence for a
  target-inlined body, ...).
The ledger is committed; the ORCHESTRATOR is its single writer (regenerate via
vostok build / regen-only refresh + park + commit at run milestones) - dispatched
matchers/verifiers never edit it, they report parking/causes in their result
lines instead.

A function "counts as DONE" when report.json reads 100% and the structure class
is MATCH; an under-100 function is done ONLY when the residual is understood and
non-steerable (LTCG argument passing / CRT noise) and a `NOTE` flag says why.
A function the target inlined everywhere emits no standalone target symbol -
nothing to pair or score; reconstruct it only from a matched consumer's bytes
and record that evidence as its `NOTE` cause (e.g. `body from
udp_match_client::enqueue's else-branch bytes`). A body with no consumer
evidence yet stays a `/* no source */` sham (with its `sushi@TODO`).


## Comment hygiene - lean code, verbose commit message
**Code is not the place to be noisy.** Keep inline comments minimal. ALL the
explanation, exploration, attempts, and rationale belong in the COMMIT/PR MESSAGE
(agentic_loop.md section 7) - be as detailed as you like *there*, never in the
source. A reader of the `.cpp` should see matched code, not a narration.
- **The carcass `// <full signature>` line is only a TYPE reference**, kept while
  matching because vostok-structure's generated argument *names* can be wrong while
  the *types* are right. **Once the arguments/types match the target, DELETE it** - a
  confirmed match does not keep the signature line.
- **A clean 100% match carries NO marker or explanation block at all** - the % lives
  in `report.json`, the why-it-matched detail goes in the commit message.
- Reserve inline comments for the genuinely unexpected/unique: a `claude@MATCH:` for
  an odd shape chosen to reproduce bytes, or a `claude@NOTE:` for a surprising target
  fact. Do NOT narrate routine mechanics (anchors, "empty body", ICF/linker folding)
  inline - and never state something you have not verified (e.g. "no symbol / not
  scorable": empty functions DO get a standalone symbol and appear in objdiff).
- **Strip unnecessary logs.** Remove any logging / diagnostic statement (`LOG_*`,
  `LOGF_*`, `printf`, `OutputDebugString`, a trace call) or commented-out debug/log
  line that was added while matching and that the TARGET does not actually emit - it is
  not part of the byte-match and only clutters the source. (A log the target's bytes
  genuinely contain stays.) Any diagnostic you needed belongs in the commit message,
  never as a leftover log line in the `.cpp`.


## Comment tags (where matching knowledge lives - keep them)
Three suffixes, author-prefixed. Existing notes are `sushi@...` (the original
matcher) - never drop them. Prefix your own with `claude@...`.
- `@MATCH:` a deliberate, often odd-looking shape chosen to reproduce the bytes (`fld1 used instead of movss`, `target does an extra copy`, `-> inlined in target`). Most valuable; write one whenever you do something unexpected to match.
- `@NOTE:` an observation about the target.
- `@TODO:` an open question.

**@-comments carry FACTS, never SCORES.** A match % (or fuzzy number, or
report figure) in a comment goes stale the moment a rebuild moves it - that
is the same disease the old `STATE[NN%]` markers died of; the numbers live in
`report.json`/the ledger. Describe the WALL, not the percentage:
- BAD:  `claude@NOTE: 94% wall - target keeps vectora_allocator ctor out-of-line`
- GOOD: `claude@NOTE: inline-vs-call wall - target keeps the
  vectora_allocator<void*>(base_allocator*) ctor out-of-line at the
  m_victory_items init site, base inlines it (LTCG per-site cut; see
  patterns/inline-vs-call-template-comdat.md)`
Byte sizes, instruction names, pattern refs, target shapes: all fine - they
are facts about the binary. Anything that re-measures on rebuild is not.

"Why it didn't match / why I stopped" = a terse `claude@NOTE:` above the function
(why stuck, what was tried - the non-matching note the next matcher reads first)
plus the same conclusion as the ledger park cause or `tried --note`; long rationale
goes in the commit message, never a comment block.


## The carcass (generated stub comments)
Stubs arrive with `// FUNCTION BODY` / `// LOCALS` / `// TYPEDEFS` blocks. A body
line is `<absoluteVA>|offset|+delta:'srcline'`: paste the VA into IDA (`G`);
`<N>` = no address (inlined/comment); a large `+delta` = something inlined
between. Use these as scratch while matching - and remember the same information
(and more) is available LIVE from the rich indexes: `pdb_fetch --view structure`
(the statement skeleton), `--view target` (the rich asm), and `--address 0x<va>` /
`--offset 0x..` / `--index N` (the asm of one specific statement). The carcass is
a generation-time snapshot; `pdb_fetch` is the source of truth.

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

**Carcass handoff (NOT 100%): the matcher DELETES the carcass.** The matcher reads
the STUB's `// FUNCTION BODY` carcass for the shape clues below, then deletes it when
done - it does not preserve or annotate it. The structure-verifier then checks the
shape with the two-sided condensed structure-diff (next), run **on demand** - the
diff and its verdict are NEVER embedded in source; the verdict goes in the commit
message (the ledger re-derives the structure class on every regen). A clean 100%
match carries nothing.

**Preferred for a non-100% function: the two-sided condensed structure-diff** (it
supersedes the one-sided `// FUNCTION BODY` carcass). Run `pdb_fetch ... --view
structure-diff --condensed` and read it: it shows target-vs-base aligned with the
matched runs collapsed to
`.. same ..` and each divergence as `NN: 0x{toff} <0x{tsize}> | 0x{boff} <0x{bsize}> |
{stmt}   {SIZE|ONLY base|ONLY target}` (the `NN:` is a monotonic statement index; blank-
line gaps are suppressed, tallied as `blank-gaps` in the summary). That way the reader sees exactly
where our build diverges from the target, not just the target shape. It is rerun on
demand whenever needed - never pasted into the source. **The verdict (`STRUCTURE
<MATCH|MISMATCH (size|quantity|both|order)> - <terse cause>`) goes in the commit
message** (and, when the residual is permanent, a ledger park cause or
`tried --note`). (The structure-verifier produces and owns these.)

**Each addressed statement IS a real source statement - that is the whole point of the
structure.** The compiler emits one line-table entry (a debugger BREAKPOINT) per source
statement, so the structure is a faithful, ORDERED list of the source's statements: an
addressed `<0xsize>` row means that statement *was* in the original source, full stop. A
QUANTITY divergence is therefore literally a missing or extra SOURCE statement, and
reproducing the target's statement count + order reproduces the original structure - which
is why structure beats the byte %. (The `<N>` no-address lines below are the COMPLEMENT: a
sub-expression the compiler set no breakpoint on - inlined, optimized out, or a continuation.)

This evidence comes from the aligned `pdb_fetch --view structure-diff` rows, not
from an aggregate line-table count. `pdb_divergence --raw-line-table-counts`
counts raw CodeView entries and is deliberately opt-in: optimization attribution
and source-line packing can change it without changing the semantic statement
shape. Never combine statements or tune whitespace merely to make that count
agree.

For whole-PDB structure, keep the evidence channels separate. Use
`pdb_topology --classes` for raw complete class variant sets and declaration
order, `pdb_divergence` for source definition order and the normalized enum
view, and `pdb_topology --order` for complete observable MSF/PDB topology. That
last view covers container pages/stream slots, DBI and source scopes, TPI/IPI
records and hashes, global/public indexes, module symbols and C13 records, and
optional FPO/frame streams; each channel labels whether its order is semantic,
physical, hash-, type-index-, address-, or linker-derived. A same-name duplicate
record is not a canonical-source verdict. The reproducible audit and
classifications live in
[`divergence_queue.md`](divergence_queue.md); raw-order confidence limits live in
[`pdb_topology.md`](pdb_topology.md).

**Named LOCALS are structure too, and structure beats the byte % for them as well.** The
PDB records the source's named-local set; `pdb_fetch --view structure` prints a
`locals (N): <name> <type>` block per side (or none). It is ground truth - "locals don't
lie" (sushi): if the target records 0 named locals and your base records 1, your source is
structurally wrong, so REMOVE the local (inline a single-use temp; for a multi-use value
find the 0-local construct the target used). Take the byte hit when they conflict (100% ->
90% is fine - the byte residual is recoverable later; a wrong local set is not), exactly as
for any other structure divergence. NEVER keep a load-bearing temp to hold a high %, and
NEVER "fix" a QUANTITY by collapsing decl+use onto one source line while keeping the local -
that preserves the wrong structure. `match_db` encodes this: a local-count mismatch demotes
a would-be MATCH to the `LOCALS` struct_class (kept in the queue), and `diff` reports a
MATCH->LOCALS flip as a REGRESS. Precedent: `weapon_and_hands_expression` - the chained
0-local form (90%) is correct over the 2-named-local 100% form.

**Names are NOT elided in this build** - the recorded local set is ground truth, so 0 target
locals means the SOURCE had 0 named locals. A 0-target-local where your base has one is
almost always an **INLINE HELPER or inline temp**: the local belongs to an inlined callee,
not this function's scope. Don't reach for an ugly hack (a raw for-loop, an embedded
assignment inside a call, a collapsed decl) to reproduce the bytes - find the helper the
target used and write it normally. An array walk is usually `std::for_each(begin, end, fn)`
(grep the SIBLING functions in the same .cpp - e.g. `inventory::remove` is a `std::for_each`
over `m_slots`, exactly like `inventory::serialize`/`deserialize` beside it, NOT a raw loop
with a named iterator); a clamp is `math::min(a, b)` with the temp inline; a constructed
argument is the temporary spelled inline (`T(NULL)`). If the inline spelling can't reproduce
the exact bytes (a ctor re-schedule), the 1-statement / 0-local STRUCTURE is still correct -
take the % hit, the byte residual is the recoverable part.

**Reading the STUB carcass (before you delete it)** - its markers are shape clues you
need for the match. A `<N>` (no address) line is a statement/sub-expression
the compiler set no breakpoint on (inlined, optimized out, or a continuation); its
count and grouping between two addressed lines are a *structural clue* (an inlined
call, a nested scope, a fall-through `jmp` thunk). Do NOT annotate the carcass -
it is deleted whole when the function is matched, and a per-statement question is
answered live (`pdb_fetch --view target --address 0x<va>`, or `--offset`/`--index`)
instead of with margin notes. While the body is still a STUB, leave its generated
blocks intact (they are the matcher input); mine `// LOCALS` for the rule below.

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
