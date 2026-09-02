# Retail/candidate PDB comparison

The current measured snapshot is
[`pdb_comparison_audit.md`](pdb_comparison_audit.md). It records the Git commit,
PDB hashes, exact commands, filters, summary counts, and unresolved evidence.
Do not copy an old count from this file into a review.

## Evidence owners

No single flattened report is the PDB truth. Use the channel that owns the
question:

| Question | Evidence owner | Strength |
|---|---|---|
| complete class shape and declaration order | `pdb_topology --classes` | direct field-list evidence |
| same-name class record variants/multiplicity | `pdb_topology --classes` | direct raw-TPI inventory |
| enum values/underlying type | `pdb_divergence` plus raw variant inspection | semantic after variant ambiguity is excluded |
| source definition order and constants | `pdb_divergence` | semantic when both files/functions pair |
| function locals/statements/lexical blocks | `pdb_fetch --view structure-diff` | direct procedure evidence |
| DBI/TPI/global/module-symbol sequence | `pdb_topology --order` | physical/linker-derived diagnostic |
| out-of-line function presence | `pdb_divergence --list-presence-fns` and the ledger | scheduling/reachability evidence |
| emitted member use | target/base disassembly at a real consumer | direct for that access path |

`pdb_divergence` remains useful as a broad normalized compatibility view. Its
class/enum model collapses same-name records, so a row involving a multi-variant
name is not a source verdict by itself. `report.json` owns emitted byte results;
it does not explain every record retained in the linked PDB.

## Reproducible audit

1. Start from a clean worktree and run a successful full
   `python3 -m vostok build -j6`. A module-only build or `derive refresh` is not
   current PDB evidence.
2. Record the commit and SHA-256 of both PDBs. Use `base` for the reconstructed
   candidate and `target` for retail consistently.
3. Run the unfiltered comparison first. Then run the campaign-filtered view with
   every `--skip` value written out; never describe a hidden filter as “our
   types.”
4. Run `pdb_topology --classes --json` and preserve these categories separately:
   `identical`, `record-multiplicity`, `variant-overlap`, disjoint `different`,
   target-missing-base, and base-only names.
5. Run `pdb_topology --order --json`. Order claims use only keys that occur once
   on both sides. Report changed unique records, one-sided records, multiplicity,
   excluded non-unique keys, and inversions separately.
6. Run target-vs-target self-checks when the comparator changes. They must have
   no semantic or order differences; duplicate keys may still be listed as
   excluded from pairing.
7. Derive review counts from the uncapped JSON and summarize them in the
   measured snapshot; attach the raw JSON externally when individual rows need
   review. Do not paste a capped terminal view and call it complete.

## Honest classifications

Use only these conclusions until stronger evidence exists:

- **exact** — the compared semantic sets agree;
- **record multiplicity** — equal shape, unequal repeated-record count;
- **variant overlap** — at least one equal shape plus unmatched shapes;
- **disjoint variants** — the same name exists but no complete shape agrees;
- **one-sided** — the named entity exists in only one compared scope;
- **consumer-bound** — assembly/procedure evidence binds one emitted consumer to
  one shape;
- **unresolved provenance** — the PDB contains competing records and they have
  not been bound to compilands or consumers.

Do not call a record `phantom`, `stale`, `unused`, or `canonical` because a
same-name sibling exists, because one selector chose another record, or because
one consumer uses another layout. Those terms require record provenance or an
equivalent direct demonstration. See
[`divergence-phantom-duplicate-type-record.md`](patterns/divergence-phantom-duplicate-type-record.md).

## Campaign filters

The historical campaign view excluded third-party and deferred subsystems with:

```text
bullet opcode stlport vorbis ogg zlib render sound scaleform flash
```

That filtered view is useful for scheduling but is not the total retail/candidate
PDB difference. Always publish both totals and filtered totals, and identify
which remaining rows are vendor/deferred rather than subtracting them in prose.
