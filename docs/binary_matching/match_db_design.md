# match_db - the queue/report database (design, agreed pre-implementation)

A sqlite database + CLI (`scripts/match_db.py`) that answers the bulk questions
the matching loop needs - build queues, roll up per-TU/module reports, find
unpaired functions - and replaces `status.jsonl` as the machine-readable status
store. It does NOT replace `pdb_fetch`: the parser stays the authoritative
per-function view (structure-diff, rich asm, statement slices); the DB exists
for queries across thousands of functions at once.

## Data sources

| source | gives |
|---|---|
| `binaries/objdiff/report.json` | TU roster (unit name = `vostok/<module>/sources/<file>.cpp`), per-function mangled name, size, `fuzzy_match_percent` |
| `binaries/rich/target/index.jsonl` | exe-level target inventory: rva, size, file, statement table (off/size/line), locals |
| `binaries/rich/base/index.jsonl` | same for our build; refreshed by every `rebuild.py` |
| target PDB **declaration records** (new parser dump, JSONL) | every function the ORIGINAL SOURCE declared - including methods the target binary inlined everywhere and emits no symbol for; class methods carry the true access/virtual/const |
| sources tree | the function's source extent for the "touched" fingerprint |

Pairing key = mangled symbol. Module = first path component of the unit;
vcproj <-> module is 1:1, so vcproject queries are module queries.

## Storage

`docs/binary_matching/match.db` - sqlite, **committed to git**.
- **Deterministic writes:** fixed page size, stable row ordering (sorted by
  rva/mangled), `VACUUM` before close - the same delink state always produces
  byte-identical files; a no-op refresh produces no git diff.
- **Merge strategy:** derived tables are recomputable, so a binary conflict is
  resolved by taking either side and re-running `refresh`; the persistent
  tables (`history`, `flags`) are merged with `match_db.py merge-flags
  <other.db>` (union, newest wins).
- **Commit cadence:** at the same points as the README score block (run start,
  before handing a stack back, after delinker/toolchain bumps) - not on every
  rebuild.
- `refresh` records delink timestamps + tool versions in `meta` and WARNS when
  the target side is older than the tools (the stale-target smear guard).

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
match_db.py refresh                       # ingest everything, reconcile history
match_db.py list   --module <m> [--unit <tu>] [--max-size 0x80]
                   [--class QUANTITY,SIZE] [--presence TARGET_ONLY]
                   [--queue-eligible] [--json]
match_db.py report --module <m> [--per-unit]   # totals / paired / 100% /
                                               # struct-MATCH / out-of-scope /
                                               # remaining + the BASE_ONLY lint
match_db.py queue  --module <m> --batch 12 [--small 0x100]
                   # matcher-ready batches, grouped by TU (a TU is never split
                   # across batches - the no-two-matchers-same-file rule),
                   # small-first, skipping out-of-scope/MATCHED_BEFORE
match_db.py sql "<query>"                 # escape hatch, read-only
match_db.py flag <mangled> --requeue|--out-of-scope --cause "..."
match_db.py merge-flags <other.db>
```

Every query takes `--json` for agents. Causes live in `flags.cause` and as
`claude@NOTE:` at the function - never in commit messages (narrative there is
fine, but it is not a system of record).

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
- **Single writer:** the ORCHESTRATOR runs refresh/flag and commits the DB at
  run milestones; dispatched matchers/verifiers never edit `match.db` - they
  report parking causes in their result lines.
- **Interned names:** symbols/units/files tables keep the committed DB at
  ~19MB; ids are sorted-dense and NOT stable across refreshes (persistent
  tables key on mangled TEXT).

## Implementation steps (one commit each, single PR)

1. this design doc
2. schema + `refresh` ingest of report.json + both rich indexes; `pairs` with
   struct classification
3. `list` + `report` (module/TU rollups) + `sql`
4. `queue` (TU-grouped batching)
5. `history` + fingerprints + out-of-scope derivation + `flag`/`merge-flags`
6. `declared_functions` ingest + BASE_ONLY taxonomy + fabricated-symbol lint
   (lands when the parser dump is available; graceful fallback before)
7. migration: import status.jsonl causes that justify flags, DELETE all
   status.jsonl, update MATCHING.md / agentic_loop.md / agent defs to point at
   match_db.py
