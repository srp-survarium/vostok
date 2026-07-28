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

### Type/declaration lookup in the target structure
WANT: "give me the declaration of `class foo`" from `binaries/structure/target`
as a command.
WHY: the missing-types step (loop section 4) is currently a manual grep through
PDB-generated headers.
IDEA: a small lookup over the target structure keyed by symbol name, returning
the decl plus its source location.
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

## Closed

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

## Why does generate_structure skip vostok/scaleform?

The shipped PDB has 10 `vostok/scaleform/sources/*.cpp` compilands (+4 headers
with addressed inlines) - the rich index sees all ~275 functions - but
`binaries/structure/target/sources/` contains no `vostok/scaleform/` dir at
all; every other of the 1,187 compilands is emitted. Found 2026-06-12 during
the game carcass rebuild (the flash/scaleform types were initially mistaken
for all-inlined because of this). Parser-side question: an exclusion list, a
path-classification bug, or something about that lib's debug info?
