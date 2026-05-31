# Unanswered questions / tooling wishlist

Capabilities the matching agent would benefit from but does not have yet. When a
run hits a wall that better tooling would solve, add it here (or sharpen an
existing entry) instead of working around it silently. Keep each item: what is
missing, why it hurts, a sketch of the fix.

## Format
```
### <capability>
WANT: <what the agent wants to do>
WHY:  <the loop step it slows down or blocks>
IDEA: <a possible implementation, if any>
STATUS: missing | in-progress | partial
```

## Open items

### Semantic search of C++ source
WANT: "find the function/type that does X" or "where is this idiom used" across
all `sources/vostok`, by meaning, not exact tokens.
WHY: matching constantly needs the right overload, the existing helper, or a
prior example of a pattern; `rg` only finds literal strings.
IDEA: embed the tree, expose a query tool the agent can call.
STATUS: missing

### Single-function / incremental diff without a full rebuild
WANT: rebuild just the edited TU and read that one function's match in seconds.
WHY: a full `rebuild.py` is ~1 minute on the machine the agent runs on (longer
elsewhere) - fine per iteration, but a per-TU rebuild would still tighten the
loop over hundreds of functions.
IDEA: compile one `.obj` under Wine and run `objdiff-cli` on that unit only;
cache everything else.
CAVEAT: rarely viable - a single-TU compile does not run LTCG (a whole-program,
link-time step), so LTCG-affected functions will not match at all in such a diff
even when the full link does. Doable in theory for LTCG-insensitive code; expect
LTCG artifacts to diverge.
STATUS: nice-to-have, low value (full builds are ~1 min on the run machine)

### Type/declaration lookup in the target structure
WANT: "give me the declaration of `class foo`" from `binaries/structure/target`
as a command.
WHY: the missing-types step (loop section 4) is currently a manual grep through
PDB-generated headers.
IDEA: a small lookup over the target structure keyed by symbol name, returning
the decl plus its source location.
STATUS: missing

### STUB progress tracker
WANT: a live count of matched / parked / remaining functions per module.
WHY: to pick the next unit and to know when a run is done.
IDEA: derive it from the `STATE[...]` markers + `report.json`.
STATUS: missing

### Assembly of a (suspected) inlined function
WANT: when the diff suggests a call was inlined (missing/extra `call` + large
statement-offset delta, section 5), pull the assembly of the *inlined* function
itself - so the agent can see what it compiles to and reproduce that body inline in
the caller's source.
WHY: to match an inline cluster you need the inlinee's code, not just the knowledge
that something was inlined; and pragmas are unresolved (we don't steer inlining),
so reading the inlinee directly is the lever the agent does have.
IDEA: `pdb_fetch --view target --function <inlinee>` already prints any *standalone*
function's asm, so the common case works today. Gap: a function inlined
*everywhere* has no standalone copy in the target - nothing to fetch on that side.
Fall back to the base side's un-inlined copy (`--base-index ... --view base`) or its
`structure`/source. A helper that, given a caller + a suspected inlinee, returns
whichever exists (target standalone -> base standalone -> source) would close it.
STATUS: partial (standalone inlinees fetchable now; inlined-everywhere ones are the gap)

<!-- Append new wishlist items below, same format. -->

---

## Done / closed

Moved here once delivered or settled - kept for the rationale, out of the active list.

### `pdb_fetch --view diff` % vs the scoreboard  [done]
WANT: the diff view's headline % to mean roughly the same as `report.json`.
WAS: `pdb_fetch` printed objdiff-core 2.5.0's *strict* per-instruction match (every
differing instruction a full miss), e.g. 56% where `report.json` fuzzy said 89% -
a foot-gun.
DONE: `pdb_fetch` now computes a **target-byte-weighted fuzzy** match (partial
credit per instruction; base-only `-` rows don't penalize), header reads
`objdiff fuzzy match P%`. Tracks `report.json` closely but slightly conservatively
(89.6 vs 89.3 on `notify_objects_inside`) - not bit-identical, since `report.json`
comes from a different objdiff version. `report.json`/`STATE` stays authoritative.

### Target assembly on demand  [done]
WANT: a stable `vostok-pdb-parser` subcommand that prints a target function's
disassembly by name.
DONE: `pdb_fetch --view target` / `pdb_rich_query` over binaries/rich/target
(agentic_loop.md section 2). Ships via the flake binary.

### Instruction-level base-vs-target diff as text  [done]
WANT: the agent reads the exact diverging instructions, not just a percentage.
DONE: `pdb_fetch --view diff --objdiff-*` emits the per-function operand-aware
instruction diff (`~`/`+`/`-` rows) with base source interleaved (agentic_loop.md
section 2a). `report.json` stays the scoreboard.

### Cluster detection from PDB inline-site records  [closed - not possible]
WANT: auto-detect which source functions are inlined into one target span (the
"cluster" unit of work in agentic_loop.md).
WHY: when A is inlined into B and cannot be separated, they match as one unit.
FINDING: MSVC 8.0 / VS2008 PDBs carry NO inline-site info - 0 `S_INLINESITE`
records across all modules in both `survarium.pdb` (2396 modules, 47,792 procs)
and our base PDB, checked at the raw CodeView-kind level with 0 parse errors. That
debug info only appears in later toolchains. So clusters cannot be derived from the
PDB; infer inlining reactively from the diff (a missing/extra `call` plus a large
statement-offset delta), per section 5.
