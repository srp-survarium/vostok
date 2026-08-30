# The matching record - design

> **The sqlite database this document was written for is gone.** The record is
> now `config/match_state.tsv`, one text row per target function,
> and there is no cache behind it. Sections describing tables, interning, `sql`
> and blob-merge strategy are kept as HISTORY, clearly marked; the live design
> is what follows here. See CLAUDE.md for the day-to-day commands.

The derivation (`scripts/vostok/derive/`, run as `python3 -m vostok derive
refresh`) answers the bulk questions the matching loop needs - build queues,
per-TU/module rollups, unpaired functions - by writing the committed ledger
straight from the built artifacts. It does NOT replace `pdb_fetch`: the parser
stays the authoritative per-function view (structure-diff, rich asm, statement
slices); the ledger exists for questions spanning thousands of functions.

Queries live in `vostok ledger` and read the committed file directly, so they
need neither a build nor a database - a fresh clone can answer them.

`vostok build` re-derives the ledger at the end of every build (it is the
canonical build step and owns the regen); `vostok derive refresh` is the
regen-only path over an artifact set already on disk (run `vostok build` first
if sources moved). "refresh" below means that regen step regardless of trigger.

## Data sources

| source | gives |
|---|---|
| `binaries/objdiff/report.json` | TU roster (unit name = `vostok/<module>/sources/<file>.cpp`), per-function mangled name, size, `fuzzy_match_percent` |
| `binaries/rich/target/index.jsonl` | exe-level target inventory: rva, size, file, statement table (off/size/line), locals |
| `binaries/rich/base/index.jsonl` | same for our build; refreshed by every `vostok build` |
| target PDB **declaration records** (new parser dump, JSONL) | every function the ORIGINAL SOURCE declared - including methods the target binary inlined everywhere and emits no symbol for; class methods carry the true access/virtual/const |
| sources tree | the function's source extent for the "touched" fingerprint |

Pairing key = mangled symbol. Module = first path component of the unit;
vcproj <-> module is 1:1, so vcproject queries are module queries.

## Storage

`config/match_state.tsv` - tab-separated text, **committed to git**.
- **Deterministic writes:** fixed column order, sorted by mangled name, LF
  endings - the same delink state always produces a byte-identical file, so a
  no-op refresh produces no git diff.
- **Merge strategy:** it is text, so a conflict is a normal text conflict and
  `git diff` names the functions that moved. Resolve by taking either side and
  re-running the refresh; nothing needs a special merge tool.
- **Commit cadence:** with the work it measures - every build advances it, and
  the README score block moves with it.
- **Why not sqlite (HISTORY):** the original design committed a ~50 MB database.
  SQLite re-serialises its pages on every write, so git could not delta it and
  each commit stored a fresh multi-MB blob - ~3.5 GB of history, none of it
  diffable or mergeable. Measured on the same 60-row change, the text ledger
  grows ~200x slower. Roughly half the database was derived data that never
  belonged in git at all, and `source_maxima` was 99.99% degenerate because its
  MAX gate keyed on a composite hash that reset whenever any sibling in the same
  TU changed.

## Schema

```sql
-- inventories (fully derived, rebuilt by refresh)
declared_functions(class, name, signature, access, is_virtual, is_static,
                   is_const, kind)            -- from PDB type records
target_functions(rva INTEGER PRIMARY KEY, mangled, demangled, unit, file,
                 line, size, n_stmts)
base_functions  (rva INTEGER PRIMARY KEY, mangled, demangled, unit, file,
                 line, size, n_stmts)
unit_functions(unit, mangled, side, fuzzy_pct)  -- report.json per-TU scores
                                                -- (COMDATs appear in many TUs)

-- pairing + structure classification (derived; alignment computed in Python)
pairs(mangled, target_rva, base_rva,
      fuzzy_pct,            -- NULL = objdiff could not pair
      struct_class,         -- MATCH | SIZE | SPLIT | QUANTITY
      t_stmts, b_stmts, n_size_rows, n_trgt_only, n_base_only)

-- persistent (survives refresh; merged across branches)
history(mangled PRIMARY KEY,
        last_paired_at, best_fuzzy_pct, last_fuzzy_pct, last_struct_class,
        src_fingerprint)    -- hash of the function's source extent at pairing
source_maxima(mangled PRIMARY KEY,
        effective_hash, max_fuzzy_pct, exact_proven, state_id, module,
        source_file, source_lo, source_hi, origin, evidence)
source_maxima_epochs(mangled, effective_hash, max_fuzzy_pct, exact_proven,
        state_id, module, source_file, source_lo, source_hi, origin, evidence,
        PRIMARY KEY(mangled, effective_hash))
flags(mangled, flag, cause, set_at)   -- manual overrides only (requeue etc.)
meta(key, value)
```

Anti-join views `target_only` / `base_only` express presence; example join:

```sql
SELECT p.mangled, t.size, b.size, p.fuzzy_pct, p.struct_class
FROM pairs p
JOIN target_functions t ON t.rva = p.target_rva
JOIN base_functions   b ON b.rva = p.base_rva
WHERE t.size <= 0x80 ORDER BY t.size;
```

Pairing is by **mangled name** (`set(target) & set(base)`), PLUS a small
**cross-name pass** for the compiler-generated dynamic-init/atexit thunks the two
sides label differently: the original-game PDB stores a namespace-prefixed
demangled form (`vostok::sound::\`dynamic initializer for 's_debug_audio''`),
while the base index may store either the raw mangled form
(`??__Es_debug_audio@sound@vostok@@YAXXZ`) or a demangled form with the namespace
inside the quoted variable name
(`\`dynamic initializer for 'vostok::sound::s_debug_audio''`). Function-local
statics have a second safe spelling gap: one PDB may put the owner and numeric
compiler scope outside the quoted variable while the other puts them inside.
A by-name intersection misses both variants, so they double-list as `target_only` and
`base_only` even when the body is byte-identical. The pass canonicalizes the SAFE subset (fully-qualified plain
identifiers, plus local statics with a plain qualified owner, numeric scope, and
plain variable; anon-ns/`?A0x` hash, template, and cook scopes are deferred to a
Rust-side demangler fix in pdb-parser), pairs 1:1 + identical statement-shape
only (never onto the wrong variable), and records the pair keyed by the TARGET sym.
Because such a pair's base twin carries a *different* sym, the anti-join views
exclude by **RVA** (`p.target_rva`/`p.base_rva`), not by sym, so both sides drop
out of `*_only` cleanly. These thunks are absent from objdiff's report.json (it
hits the same name gap), so their `fuzzy_pct` is NULL - the win is roster
correctness (paired/`target_only` counts), not the weighted README %.

## Structure classification (`struct_class`)

Computed per paired function from the two statement tables via sequence
alignment (difflib over `(line-delta, size)` tuples; one in-memory pass over
all functions). Approximation for QUEUE RANKING only - the per-function verdict
stays `pdb_fetch --view structure-diff`.

| class | meaning |
|---|---|
| `MATCH` | same statement count, same per-statement sizes |
| `SIZE` | same count, >=1 statement size differs |
| `SPLIT` | counts and total bytes equal, but alignment yields paired TRGT_ONLY+BASE_ONLY rows (line-attribution split) |
| `QUANTITY` | statement counts differ - real missing/extra source statements |

Raw counts (`n_size_rows`, `n_trgt_only`, `n_base_only`) stored alongside.

## History: "seen before" does the classifying (requirements 2 & 8)

Every refresh that sees a function in BOTH base and target upserts its
`history` row ("paired, here's the %"). Rows are never deleted by a
disappearance - that persistence IS the flag. The verdicts fall out:

| current snapshot | history | verdict |
|---|---|---|
| paired | - | normal row: live %, struct class |
| TARGET_ONLY | no row | **queue it** (never written, or written but never anchored - either way a matcher must look once) |
| TARGET_ONLY | row, fingerprint unchanged | **out of scope automatically** - was written, paired at NN%, vanished without its source changing: external inline/link decision |
| paired again later | row | history updates; out-of-scope evaporates |
| anything | row, fingerprint CHANGED | function was touched -> history row reset, re-queued |

`MATCHED_BEFORE` is the special case `best_fuzzy_pct == 100 AND struct_class ==
MATCH` of the same row: it later regressing without a source touch means
something else inlined differently - non-steerable, skip it in queues.

The fingerprint hashes the function's SOURCE TEXT extent (file + statement
line range from the base PDB), NOT its bytes - so a matcher edit clears the
flag, while a header/other-unit change that only shifts bytes does not.

Known edge (accepted): a function pairs, then its `temp_include_all` anchor is
deleted - it vanishes with the source extent unchanged and is wrongly called
out of scope. The anchor removal is visible in the PR diff, and `flag
--requeue` overrides manually.

## Source-hash-scoped MAX

`history.best_fuzzy_pct` remains ordinary rebuild history for queue scheduling
and ICF/fold diagnosis. It is not correctness-facing MAX evidence.

`source_maxima` is the separate HoMM2-style ledger. A refresh records the
current fuzzy score and compiled-state identity under an effective hash made
from the function's source extent plus conservative module/compiler context:
the complete owning implementation file when the PDB identifies one, module
and shared headers, project files, anchor sources, and pinned build/delink
configuration. Hashing the complete translation unit is required because LTCG
can change a function after an edit to a different body in the same `.cpp`.

When objdiff omits a function score, refresh may recover only a strict exact
observation from the rich indexes: target and base must have equal size and an
identical non-empty ordered stream of normalized `(offset, length, instruction
text)` tuples. If ICF selected different canonical mangled identities for the
two RVAs, their rich aliases must also provide a unique unused base RVA with the
same demangled signature and source file. When ICF selects different source
records as the owner, both RVAs may instead expose the same complete multi-name
alias cluster; combined with identical complete instruction streams, that
proves the owner-file difference is linker attribution rather than a same-named
internal helper. Rich instruction comparison also canonicalizes the PDB's
demangled and decorated spellings of the same `operator delete` operand. This
is function-scoped evidence for COMDAT/source-tree pairing gaps,
not a fuzzy estimate; ambiguous, differently owned without shared-cluster
evidence, differing, or absent streams remain unscored. Structure classification
remains independent, so exact bytes do not suppress a `QUANTITY` warning.

- The same effective hash retains `max(old, current)`.
- A changed effective hash starts a new epoch at the current observation.
- Valuable inactive epochs are archived by `(mangled, effective_hash)`. If an
  identical source/compiler hash becomes current again, refresh restores that
  proof into the single active `source_maxima` row. Plain rebuild observations
  equal to their build are not archived, which keeps the database bounded.
- Ordinary `history` maxima are never imported.
- A same-hash maximum may survive temporary LTCG/ICF disappearance only while
  its retained source locator and context re-hash identically.
- `exact_proven` requires a measured byte-exact observation in that epoch.
- `state_id` identifies the observed size and ordered instruction stream.

Disposable compiler-state islands must restore all tracked source/context
inputs before recording their evidence. Generated probe source is not durable;
only the source-scoped maximum, state identity, origin, and evidence reference
belong in the database. README `exact-max` and `fuzzy-max` use this table and
take current measurements as a floor, so MAX can never display below current.

`vostok ledger list [--module <m>]` shows each row's `max`. An island
runner must first refresh from the real candidate artifacts, restore the source,
and then use `record-max <mangled> --evidence <path> --expected-hash <hash>`.
`record-max` accepts no score: it can annotate only the hash, score, exact bit,
and state identity already measured by the normal report/index pipeline.

When the island artifacts are deliberately kept outside the canonical build
tree, use `import-island --report <report.json> --base-index <index.jsonl>
--evidence <manifest.json>`. The tracked evidence manifest explicitly names
each reviewed function and pins the candidate report/index SHA-256, expected
score, module, and effective source hash. The importer updates only those
`source_maxima` rows; it never changes current pairings, ordinary history,
flags, or attempts, and refuses stale hashes, absent measurements, and
non-improvements. Run it once with `--dry-run` to obtain the current effective
hashes before pinning and importing the evidence.

## BASE_ONLY taxonomy (declaration-grounded)

The PDB declaration records list every method the original source declared,
including never-emitted (inlined-everywhere) ones - so legitimacy is a lookup,
not a judgment:

| BASE_ONLY case | signal | verdict |
|---|---|---|
| mangling mismatch (wrong access/const -> different mangled name) | TARGET_ONLY row with same demangled stem; declaration records give the TRUE access | **queue, high priority** - names the exact header fix |
| delink jitter (paired before under same mangled name; ICF/folded representative flipped) | history row exists | ignore |
| declared in structure, target emits no symbol (inlined everywhere in target) | found in `declared_functions` | legitimately out of scope, automatic |
| not declared anywhere | elimination | **fabricated symbol** -> `report` lint: "N unexplained base-only symbols in <module>" |

The lint makes the tool the fabricated-symbol detector (today the reviewer
greps PR diffs by eye). Caveat: free functions inlined everywhere leave no
type record and no symbol, so for them the check stays heuristic; nearly
everything we match is a class method.

## CLI

```
vostok derive refresh                    # regen-only: ingest the already-built
                                         #   report.json + indexes and rewrite the
                                         #   ledger (vostok build first if sources
                                         #   moved - it re-derives on its own)

vostok ledger report --module <m> [--per-unit]    # byte-weighted rollup:
                                                  #   done/open/park/blkd, held,
                                                  #   head, cur%, max%
vostok ledger list   [--module <m>] [--unit <tu>] [--class QUANTITY,SPLIT]
                     [--status parked] [--max-size 0x80] [--headroom] [--json]
vostok ledger queue  --module <m> [--limit N]
                     # ONE batch per TU - all its open functions, worst first;
                     # per-TU matching keeps small helpers in their callers'
                     # LTCG environment (no cross-TU small-function churn)
vostok ledger tried  <mangled>... [--note "..."]   # record a dispatch
vostok ledger park   <mangled> --cause "..."       # stop working it, with a reason
vostok ledger open   <mangled>                     # undo a park
```

Every query takes `--json` for agents. Causes live in the ledger's `note`
column and as `claude@NOTE:` at the function - never in commit messages
(narrative there is fine, but it is not a system of record).

## Parser dependency

A new vostok-pdb-parser dump: declaration records -> JSONL (class, name,
signature, access, virtual/static/const, kind), deterministic order. Until the
dump exists, `refresh` skips `declared_functions` ingestion with a warning and
the BASE_ONLY taxonomy falls back to "unexplained" for everything unpaired.

## Implementation deltas (as built; authoritative where they differ from above)

- **Declarations are transient.** The dump (vostok-pdb-parser PR #25,
  `pdb_declarations`; 222k rows / 85MB at `binaries/rich/target/
  declarations.jsonl`) is too big to store in the committed DB; `refresh`
  loads it in memory and persists only the per-function verdict in
  `base_only_status(mangled, status, detail)`. There is no `declared_functions`
  table.
- **BASE_ONLY statuses grew:** `COMPILER` (thunks, backtick names, `??__E/F`
  dynamic initializers, anon-ns), `ANCHOR` (our `temp_include_all`
  scaffolding), `TEMPLATE` (instantiation of a declared template - dump and
  demangler render template args differently, so the class/name STEM is
  matched), alongside `NEAR_MISS`/`JITTER`/`INLINED_IN_TARGET`/`UNEXPLAINED`.
  `NEAR_MISS.detail` carries the target-side mangled name, which encodes the
  exact access/const header fix.
- **Flags:** `SKIP` (queues skip; cause = next step), `NOTE` (informational
  only), plus `--requeue` (forgets history+flags). status.jsonl was migrated
  2026-06-12 - 620 causes imported as `[STATUS pct%] cause` flags (SKIP for
  BLOCKED/SKIPPED/INPROGRESS, NOTE otherwise); 48 rows referenced symbols
  absent from the current delink and live on in git history only.
- **Single writer:** the ORCHESTRATOR runs refresh/park and commits the ledger
  at run milestones; dispatched matchers/verifiers never edit it - they report
  parking causes in their result lines.
- **Interned names (HISTORY):** the database interned symbols/units/files to
  stay near ~19 MB. The ledger simply repeats the mangled name per row - it is
  66% of the file and irreducible, and at 19,645 rows that is ~5 MB of text.
- **Frameless = custom-conv, out of scope (2026-06-12, from the first
  orchestrator run):** a target function without the /Od `push ebp; mov ebp,
  esp` prologue is an LTCG-customized leaf (custom calling convention,
  this-in-eax) - never source-steerable as a standalone symbol; it only pairs
  inlined into callers. refresh stores `frameless` per side-table row; queue
  skips them (`--include-frameless` to override); report counts them as
  `custom_conv`.
- **Header pseudo-units fold into host TUs (same run):** the delinker
  attributes inline methods to per-header pseudo-units; they are not real TUs
  and matching them standalone hits the custom-conv/ICF walls. `queue` folds
  each header unit's functions into a host .cpp: stem match (`x.h` /
  `x_inline.h` -> `sources/x.cpp`), else the .cpp unit owning most functions
  of the same class, else a standalone batch labeled `(header-only)`. Rows
  carry their defining unit so the matcher knows it is editing the header.
- **Schema guard:** query commands refuse a DB whose `meta.schema_version`
  differs - after pulling a tool update, run `refresh` first (single-writer:
  the orchestrator).

## Implementation steps (HISTORY - the original sqlite build-out, all landed)

Kept as the record of how the database was built, and of what each verb below
was for. Every one of these verbs is now retired: the queries live in
`vostok ledger`, and `refresh` is derive's only verb.

1. this design doc
2. schema + `refresh` ingest of report.json + both rich indexes; `pairs` with
   struct classification
3. `list` + `report` (module/TU rollups) + `sql`
4. `queue` (one batch per TU)
5. `history` + fingerprints + out-of-scope derivation + `flag`/`merge-flags`
6. `declared_functions` ingest + BASE_ONLY taxonomy + fabricated-symbol lint
   (lands when the parser dump is available; graceful fallback before)
7. migration: import status.jsonl causes that justify flags, DELETE all
   status.jsonl, update MATCHING.md / agentic_loop.md / agent defs to point at
   vostok derive
