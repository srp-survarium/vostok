# Retail/candidate PDB comparison audit — 2026-09-02

This is a measured inventory, not a claim that every reported row requires a
source edit. It replaces the stale qualification that five network layout rows
were known duplicate-record “phantoms.” The retail PDB does contain competing
same-name records, but the available evidence does not prove that the unmatched
records are stale, unused, or otherwise disposable.

## Snapshot and reproduction

The candidate used the reconstructed C++ state at commit
`5ebab38e1b213efd6e38c376efa3c0c16a2db41b` and was built in an isolated
worktree and Wine prefix with a successful full `python3 -m vostok build -j6`.
No reconstructed C++ was changed for this audit. The comparison used
`vostok-pdb-parser` commit
`ee1cfd5ee969e3975215949a5a5c4e95bb5ed7ef`, which is pinned by this branch.

| PDB | bytes | SHA-256 |
|---|---:|---|
| candidate `binaries/Win32/survarium-dx11-win32-gold.pdb` | 105,073,664 | `3ebd4bd5c0624d392e32761ca5792bc9f3b949adfb51a3554ee158695a2dde70` |
| retail `survarium.pdb` | 101,673,984 | `0ffe85c27f8b95f23a65d91866af3384ab24ca343b3865a57f71a08902d5a238` |

Both PDBs record the engine source tree under the canonical
`c:\survarium\sources` prefix, so the normalized commands use that prefix on
both sides:

```text
pdb_divergence \
  --base-pdb binaries/Win32/survarium-dx11-win32-gold.pdb \
  --base-engine-path 'c:\survarium\sources' \
  --target-pdb <retail>/survarium.pdb \
  --target-engine-path 'c:\survarium\sources'

pdb_divergence <paths-above> \
  --skip bullet --skip opcode --skip stlport --skip vorbis --skip ogg \
  --skip zlib --skip render --skip sound --skip scaleform --skip flash

pdb_topology --target-pdb <retail>/survarium.pdb \
  --base-pdb binaries/Win32/survarium-dx11-win32-gold.pdb --classes --json

pdb_topology --target-pdb <retail>/survarium.pdb \
  --base-pdb binaries/Win32/survarium-dx11-win32-gold.pdb --order --json
```

`pdb_divergence` warned that four Scaleform modules ended early while their
symbol streams were read. The raw class inventory does not apply the campaign
filters. These two facts are part of the result and prevent treating the tables
as interchangeable counts.

## Normalized compatibility view

The category counts overlap: one type can differ in size, members, declaration
order, and visibility. “Diverged” is therefore not the sum of the following
columns.

| scope | compared | diverged | size | member | member-function order | visibility | base-only | target-only |
|---|---:|---:|---:|---:|---:|---:|---:|---:|
| all non-external types | 16,650 | 125 | 30 | 50 | 90 | 21 | 929 | 965 |
| campaign-filtered types | 6,570 | 57 | 6 | 19 | 32 | 17 | 196 | 182 |

| scope | compared | diverged | base-only | target-only |
|---|---:|---:|---:|---:|
| all normalized enums | 825 | 3 | 51 | 68 |
| campaign-filtered enums | 598 | 2 | 49 | 57 |
| all paired source files | 1,183 | 16 | 3 | 2 |
| campaign-filtered source files | 701 | 12 | 2 | 0 |

The filtered source result consists of eight definition-order differences and
four constant differences:

| kind | engine-relative source |
|---|---|
| constant | `vostok/animation/sources/mixing_n_ary_tree.cpp` |
| constant | `vostok/animation/sources/mixing_n_ary_tree_animation_event_iterator.cpp` |
| order | `vostok/core/sources/core_entry_point.cpp` |
| constant | `vostok/core/sources/math_quaternion.cpp` |
| order | `vostok/engine/sources/engine_world.cpp` |
| order | `vostok/game/sources/game_world.cpp` |
| constant | `vostok/game/sources/lobby_menu_ui.cpp` |
| order | `vostok/game/sources/login_menu.cpp` |
| order | `vostok/game/sources/player_tick.cpp` |
| order | `vostok/game/sources/weapon.cpp` |
| order | `vostok/particle/sources/particle_emitter_instance.cpp` |
| order | `vostok/ui/sources/ui_text_edit.cpp` |

The out-of-line presence diagnostic reports 339 candidate-only and 294
retail-only functions without filters, and 272 candidate-only and 219
retail-only functions with the campaign filters. Presence is scheduling and
reachability evidence; it is not by itself a missing-source verdict under LTCG.

The filtered label is literal. For example, `--skip bullet` does not exclude a
type merely because its C++ name starts with `bt`; the remaining six size rows
still include `btKinematicCharacterController`.

## Raw complete-class variants

The previous comparator selected one “best” record per qualified class name.
The current raw view retains every complete record, groups equal semantic
shapes, preserves all PDB-local type indexes, and classifies the full variant
sets.

| class-name status | names |
|---|---:|
| identical semantic set and record multiplicity | 28,202 |
| only repeated-record multiplicity differs | 122 |
| at least one shared shape plus unmatched variants | 130 |
| both sides present, no complete shape shared | 413 |
| retail name missing from candidate | 561 |
| candidate-only name | 382 |

| raw inventory | retail | candidate |
|---|---:|---:|
| complete class records | 30,421 | 30,299 |
| distinct semantic variants | 29,759 | 29,640 |
| names with more than one semantic variant | 267 | 322 |
| names with repeated equal records | 642 | 651 |
| unresolved referenced types | 2 | 2 |

This raw inventory includes vendor/external classes. It is intentionally not a
filtered replacement for the normalized table.

A retail-vs-retail self-check reports all 29,428 retail class names identical,
with no multiplicity, overlap, disjoint, or one-sided differences. This checks
the comparison, not the proposition that retail contains only one shape per
name: retail has 267 multi-variant names and 642 names with repeated equal
records.

## Reclassification of the six filtered size rows

| normalized row | retail complete variants | candidate | raw-set status | supported conclusion |
|---|---|---|---|---|
| `packets_in_list_predicate` | sizes `0x1`, `0x2` | size `0x2`, equal to one retail shape | variant overlap | one exact shape is shared; the retail `0x1` variant has unresolved provenance |
| `udp_match_connection` | sizes `0x530`, `0x538` | size `0x538` | disjoint | size agrees with one retail variant, but nested types, enum labels, and declaration order still disagree |
| `udp_match_client` | sizes `0xb20`, `0xb28` | size `0xb28` | disjoint | size agrees with one retail variant, but generated/member and nested-declaration details still disagree |
| `udp_match_client_session` | sizes `0x568`, `0x570` | size `0x570` | disjoint | size agrees with one retail variant, but properties and nested typedef/declaration details still disagree |
| `match_client_impl` | sizes `0x96ba0`, `0x258ba0`, `0x258ba8` | size `0x258ba8` | disjoint | the largest retail size is closest, but nested typedef presence and declaration order still disagree |
| `btKinematicCharacterController` | size `0xe0` | size `0xd0` | disjoint | a real single-variant layout difference remains; this is Bullet/vendor work despite surviving the substring filters |

`sequence_id_predicate` is another relevant network member row: both sides have
the name, but no full shape agrees. The candidate is closest to the retail
`u16` form and still differs in `boost::noncopyable` inheritance access; retail
also retains a `u8` form.

Assembly previously inspected for `udp_match_connection` and
`match_client_impl` supports the candidate layout at those consumers. That is
consumer-bound evidence. It cannot establish that the other retail records are
phantoms, nor can it erase the remaining whole-record differences. The honest
state of the unmatched variants is unresolved provenance.

## Enum result

The two filtered normalized enum rows are qualitatively different:

- one is an anonymous-name collision between unrelated third-party enumerator
  sets; it is not an engine enum source task;
- `vostok::resources::class_id_enum` is a real normalized difference, with 11
  candidate-only enumerators tied to still-present cooks and consumers.

Top-level enum comparison still collapses same-name `LF_ENUM` records. This
audit therefore does not declare any selected enum record canonical. Raw enum
variant-set and provenance reporting remains a tooling gap.

## Whole-PDB order diagnostics

Order comparison pairs only keys that occur exactly once on both sides. It
reports insertions/deletions, multiplicity differences, equal duplicate keys,
semantic changes, and inversion participants separately. An insertion cannot
manufacture a move, and PDB-local type indexes or source paths cannot
manufacture a semantic change.

| physical channel | candidate total | retail total | shared unique | inversion participants | changed unique | candidate-only | retail-only | multiplicity | excluded non-unique |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| DBI module stream | 2,387 | 2,396 | 2,331 | 2,329 | 0 | 56 | 65 | 0 | 0 |
| named complete TPI records | 33,722 | 33,896 | 30,844 | 30,844 | 4 | 458 | 677 | 278 | 877 |
| global symbol stream | 163,527 | 155,692 | 140,998 | 140,996 | 0 | 13,974 | 7,480 | 154 | 1,278 |

The four changed unique named TPI records are
`btKinematicCharacterController` and three Scaleform classes. Module streams
pair in 2,331 scopes; 1,472 have at least one symbol-stream difference. Across
those differing scopes there are 13,825 inversion participants, 0 changed
unique symbols, 97,361 candidate-only symbols, 16,434 retail-only symbols,
10,583 multiplicity differences, and 8,786 excluded non-unique keys.

Retail-vs-retail has zero moves, semantic changes, one-sided records, or
multiplicity differences in every channel and zero differing module scopes. It
still excludes 1,021 non-unique named-type keys and 1,451 non-unique global
symbol keys from order pairing, as intended.

These figures show that the candidate and retail PDBs have profoundly different
physical/linker ordering. They do **not** show that 30,844 type declarations or
140,996 source declarations are misplaced: DBI, TPI, global, and module symbol
streams are affected by compilation, LTCG, COMDAT selection, type merging, and
linker processing. Use the order report to locate clusters, then prove source
declaration or definition order with the higher-confidence class/source views.

## Current answer

Retail and candidate are close enough to pair most named entities, but they are
not structurally equal. The campaign-filtered normalized view has 57 type names,
2 enum rows, and 12 source files with at least one reported difference, plus
large one-sided function-presence sets. The raw class stream additionally
exposes hundreds of multiplicity/variant-set differences, and the linker-derived
record ordering is almost entirely different.

Those numbers are an evidence inventory, not an actionable queue. Before a row
becomes a source task, exclude vendor/deferred scope, preserve all same-name
variants, bind ambiguous records to a compiland or consumer where possible, and
use function structure/assembly to decide what the shipped code actually uses.
