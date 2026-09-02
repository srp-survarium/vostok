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
`50a8f49664178f56d2f665c923c9c5748ef9d747`, which is pinned by this branch.

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
| all paired source files | 1,183 | 12 | 3 | 2 |
| campaign-filtered source files | 701 | 9 | 2 | 0 |

The filtered source result consists of five definition-order differences and
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
| order | `vostok/game/sources/weapon.cpp` |
| order | `vostok/ui/sources/ui_text_edit.cpp` |

The earlier line-only comparison also listed `login_menu.cpp`, `player_tick.cpp`,
and `particle_emitter_instance.cpp`. Their attributed line orders invert, but
their independent compiland procedure-symbol orders do not. `login_menu.cpp`
demonstrates the failure directly: its physical source already places
`clear_resources()` before `set_status()`, while a retained `#line 95` makes the
later function appear to start on an earlier reported line. Source definition
order now requires both evidence channels to invert, so these three rows are no
longer presented as source edits.

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

“Inversion participant” is deliberately sensitive: moving one intact block can
label nearly every record in both blocks. The comparator now also reports exact
pair inversions, the longest ordered subsequence (LIS), adjacency retained after
one-sided records are removed, the longest contiguous run, increasing runs, and
rank displacement.

| physical channel | inverted pairs | inversion rate | LIS | retained adjacency | longest contiguous run |
|---|---:|---:|---:|---:|---:|
| DBI module stream | 534,038 / 2,715,615 | 19.6655% | 738 / 2,331 (31.66%) | 1,127 / 2,330 (48.37%) | 457 |
| named complete TPI records | 80,313,846 / 475,660,746 | 16.8847% | 10,589 / 30,844 (34.33%) | 21,805 / 30,843 (70.70%) | 1,693 |
| global symbol stream | 469,719,551 / 9,940,147,503 | 4.7255% | 85,614 / 140,998 (60.72%) | 116,107 / 140,997 (82.35%) | 2,629 |

This is still a large physical difference, but it is not randomized or
“almost entirely different.” Long runs survive, especially in TPI and global
symbols. The old participant count hid that locality.

### Stability check

An identical-input relink was forced by touching only the generated final-link
response file. The build wrapper deleted the old output PDB before linking, so
the test did not reuse a prior type stream. No object or library was rebuilt.

The fresh files had different raw identities:

| artifact | preserved baseline SHA-256 | forced-relink SHA-256 | differing bytes |
|---|---|---|---:|
| EXE | `c29d8295f0f749d57b3cf0bd81b3f91e0583424699df09de43289458f1655ad7` | `9735990ce3b426e3ad6e53808de835840419b3a31f86df0c569b85e95d57b30c` | 22 |
| PDB | `3ebd4bd5c0624d392e32761ca5792bc9f3b949adfb51a3554ee158695a2dde70` | `fb99f6fd84f7b65d49ab72409e88c9cb9ac6e3180090fe0d615c49be45080633` | 39,536 |

Despite those identity/checksum bytes, baseline-vs-relink comparison was exact:
all 2,387 DBI modules, 33,722 named complete TPI records, 163,527 global
symbols, and all 2,387 paired module symbol streams had zero moves, changes,
one-sided records, or multiplicity changes. PDB record order is reproducible for
fixed link inputs. It is stable enough to use diagnostically; stability does
not make a linker-derived channel direct source-order proof.

### Causal grouping

DBI order was split by contributing library. Of 51 multi-member library groups,
27 have zero relative inversion among their shared objects. The engine groups
for `debug`, `engine`, `input`, `libfoundation`, `ogg`, `opcode`, and `vorbis`
are fully equal in both presence and order. Several groups that label every
object “moved” are intact rotations:

| library | shared objects | retained adjacency | increasing runs | inverted pairs |
|---|---:|---:|---:|---:|
| `vostok_ai-static-gold.lib` | 79 | 77 / 78 | 2 | 690 |
| `vostok_collision-static-gold.lib` | 44 | 42 / 43 | 2 | 123 |
| `vostok_fs-static-gold.lib` | 14 | 12 / 13 | 2 | 13 |
| `vostok_logging-static-gold.lib` | 9 | 7 / 8 | 2 | 8 |
| `vostok_physics-static-gold.lib` | 12 | 10 / 11 | 2 | 35 |
| `vostok_ui-static-gold.lib` | 17 | 15 / 16 | 2 | 60 |
| `vostok_vfs-static-gold.lib` | 60 | 58 / 59 | 2 | 884 |

VFS is the clearest example. Candidate DBI order contains a 34-object block
followed by a 26-object block; retail contains the same two blocks in the
opposite order. Neither DBI sequence is archive member order:
`vfs_lib.rsp` begins with `library_linkage.obj`, `pch.obj`, and the `find_*`
objects, whereas candidate DBI begins with `mount_physical_path.obj` and retail
begins with `mount_ptr.obj`. DBI is recording link extraction/demand order, not
the `.vcproj` file list verbatim.

The final response file already follows `RETAIL_LINK_LIBRARY_ORDER` in
`scripts/vostok/build/ninja_regen.py`. That order was derived independently
from retail section contributions. Commit `aa7724d34` introduced it and moved
the then-current build from 9,097 to 10,004 exact functions and from 88.38% to
91.72% fuzzy. The current DBI interleaving also does not follow the response
file's library sequence on either side. Changing that established order merely
to make the DBI prefix look closer would discard much stronger code and section
evidence.

Module-local symbols were also grouped by record kind after scopes were ordered
by their stable module key. This removes DBI module extraction order from the
measurement:

| module-local kind | candidate total | retail total | shared unique | candidate-only | retail-only | multiplicity | inverted pairs | retained adjacency | LIS |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| constant | 2,911 | 3,894 | 2,396 | 426 | 1,415 | 6 | 20 / 2,869,210 (0.000697%) | 2,369 / 2,395 (98.91%) | 99.54% |
| data | 9,029 | 8,253 | 6,873 | 2,148 | 1,372 | 0 | 50,860 / 23,615,628 (0.215366%) | 3,632 / 6,872 (52.85%) | 70.03% |
| procedure | 48,155 | 47,652 | 43,825 | 4,312 | 3,809 | 0 | 45,398 / 960,293,400 (0.004728%) | 40,403 / 43,824 (92.19%) | 95.81% |
| thunk | 864 | 860 | 279 | 558 | 554 | 0 | 15 / 38,781 (0.038679%) | 269 / 278 (96.76%) | 98.92% |
| UDT | 622,900 | 428,241 | 7,732 | 89,917 | 9,284 | 10,577 | 369 / 29,888,046 (0.001235%) | 7,290 / 7,731 (94.30%) | 97.85% |

The shared unique module-local UDT sequence—the channel most plausibly affected
by header inclusion and declaration order—is already extremely close. The huge
UDT total, one-sided, and multiplicity gap also means that only a small unique
subset can make an order claim. It is evidence to investigate PCH/type-emission
and missing/extra declarations, not evidence to reorder headers wholesale.

By contrast, `pdb_divergence` reports five high-confidence source-definition
order rows. The updated report prints both PDB sequences with attributed source
lines. Those rows can be fixed directly, one translation unit and measured
commit at a time; physical DBI/TPI/global rows cannot.

Retail-vs-retail has zero moves, semantic changes, one-sided records, or
multiplicity differences in every channel and zero differing module scopes. It
still excludes 1,021 non-unique named-type keys and 1,451 non-unique global
symbol keys from order pairing, as intended.

These figures show a stable, materially different physical/linker order with
substantial preserved locality. They do **not** show that 30,844 type
declarations or 140,996 source declarations are misplaced: DBI, TPI, global,
and module symbol streams are affected by compilation, LTCG, COMDAT selection,
type merging, and linker processing. Use the order report to locate clusters,
then prove source declaration or definition order with the higher-confidence
class/source views.

## Current answer

Retail and candidate are close enough to pair most named entities, but they are
not structurally equal. The campaign-filtered normalized view has 57 type names,
2 enum rows, and 9 source files with at least one reported difference, plus
large one-sided function-presence sets. The raw class stream additionally
exposes hundreds of multiplicity/variant-set differences. Linker-derived record
order is stable and measurably different, but much of it consists of retained
or rotated blocks; the established final library order and shared unique
module-local UDT order are already strong.

Those numbers are an evidence inventory, not an actionable queue. Before a row
becomes a source task, exclude vendor/deferred scope, preserve all same-name
variants, bind ambiguous records to a compiland or consumer where possible, and
use function structure/assembly to decide what the shipped code actually uses.
