# Retail/candidate PDB comparison audit — 2026-09-02

This is a measured inventory of the current linked PDBs. It is not a count of
source bugs. The comparison deliberately separates semantic source evidence,
compiler/linker record order, hash-table serialization, and the physical MSF
container. A difference in one layer does not automatically explain a
difference in another.

The short answer is that the candidate PDB does **not** follow retail order for
everything. Five high-confidence source-definition-order differences have been
fixed, and their filtered comparison is now clean. Much broader and stable
differences remain in stream placement, module extraction, type insertion,
string tables, section contributions, line/checksum records, public-address
order, FPO, and frame data. Most of those are diagnostics of different link
inputs or emitted code, not instructions to reorder source blindly.

## Current reproducible snapshot

The candidate is commit `6d00b67e6fc9a4e68e1908096dce13f824b3f03c`,
built and then clean-HEAD rebuilt in an isolated worktree and Wine prefix with
`python3 -m vostok build -j6`. Both full builds measured 75.66% code and 37,041
/ 44,600 exact functions, with zero regressions or improvements. The comparison
uses pinned `vostok-pdb-parser` commit
`1eee4a0a155f9ec911638f639c00818b2a84070e`.

| PDB | bytes | SHA-256 |
|---|---:|---|
| candidate `binaries/Win32/survarium-dx11-win32-gold.pdb` | 104,352,768 | `a88cad5f439648d145bb4da1bf41adae0c27ec7a0153a2a65baa2d4a86c694e6` |
| retail `survarium.pdb` | 101,673,984 | `0ffe85c27f8b95f23a65d91866af3384ab24ca343b3865a57f71a08902d5a238` |

Both PDBs record the engine tree under `c:\survarium\sources`, so the same
normalization prefix is used on both sides:

```text
pdb_divergence \
  --base-pdb binaries/Win32/survarium-dx11-win32-gold.pdb \
  --base-engine-path 'c:\survarium\sources' \
  --target-pdb <retail>/survarium.pdb \
  --target-engine-path 'c:\survarium\sources'

pdb_topology --target-pdb <retail>/survarium.pdb \
  --base-pdb binaries/Win32/survarium-dx11-win32-gold.pdb --classes --json

pdb_topology --target-pdb <retail>/survarium.pdb \
  --base-pdb binaries/Win32/survarium-dx11-win32-gold.pdb \
  --order --limit 0
```

The JSON form is uncapped and owns the individual rows. `--limit 0` prints the
complete channel summaries without millions of record details.

## What is now compared

The topology sweep inventories every present MSF stream slot and compares every
decoded order-bearing layer in these PDBs:

| layer | compared evidence |
|---|---|
| MSF 7.00 container | superblock, active free-page-map blocks and bits, directory/map pages, stream slots, sizes, page lists, allocation runs, fragmentation, identified roles, unidentified slots |
| PDB Info and `/names` | live named-stream order, bucket positions, deleted buckets, feature order, strings, metadata, hash buckets, trailing name count |
| DBI | header and substream offsets/sizes, modules, library grouping, per-module source files, section contributions, section map, image section headers, EC strings/metadata/hash buckets, optional stream references |
| TPI and IPI | every raw record kind in insertion order, named complete records, enum value order, per-record hashes, index-offset checkpoints, hash-adjustment buckets |
| global and public symbols | every raw symbol record, stable recognized identities, GSI and PSI hash-record and bucket order, public address map, thunk and section maps |
| module streams | every raw/top-level symbol record and stable symbol kind, C13 subsection order, local strings, file checksums, line programs, frame data, inlinees, cross-scope maps, and raw-word fallback for unknown payloads |
| optional debug streams | legacy FPO, frame data, OMAP, fixup, xdata/pdata, token/RID map and section-header presence; payloads are decoded when present |

For this pair all present stream slots receive a role; there are no unidentified
streams. IPI, OMAP, fixup, xdata/pdata, token/RID, C13 inlinee/cross-scope, and
several other optional kinds are absent or empty in both inputs. Absence is
reported, not treated as unmeasured content. A future unknown C13 payload is
still preserved and compared as ordered raw words rather than silently skipped.

This is complete for the observable serialization supported by these VS2008
PDBs; it is not a claim that physical allocation has source semantics or that
the PDB records information the compiler/linker discarded.

## Semantic compatibility view

The normalized categories overlap: one type can differ in size, members,
declaration order, and visibility. “Diverged” is not their sum.

| scope | compared | diverged | size | member | member-function order | visibility | candidate-only | retail-only |
|---|---:|---:|---:|---:|---:|---:|---:|---:|
| all non-external types | 16,650 | 125 | 30 | 50 | 90 | 21 | 929 | 965 |
| campaign-filtered types | 6,570 | 57 | 6 | 19 | 32 | 17 | 196 | 182 |

| current campaign-filtered scope | compared | diverged | candidate-only | retail-only |
|---|---:|---:|---:|---:|
| normalized enums | 598 | 2 | 49 | 57 |
| paired source files | 701 | 4 | 2 | 0 |

The four source rows are anonymous local constants in
`mixing_n_ary_tree.cpp`,
`mixing_n_ary_tree_animation_event_iterator.cpp`, `math_quaternion.cpp`, and
`lobby_menu_ui.cpp`. There are now zero high-confidence definition-order rows
in the filtered view.

Out-of-line presence remains large: the preceding audit snapshot reported 272
candidate-only and 219 retail-only filtered functions. Presence is
scheduling/reachability evidence under LTCG, not by itself proof of a missing or
fabricated source body.

## What was fixed

The original source comparison found five files whose attributed line order and
independent compiland procedure-symbol order both inverted. Each was reordered
as a separate measured, code-neutral change:

| source | measured commit |
|---|---|
| `vostok/core/sources/core_entry_point.cpp` | `3ca48737d` |
| `vostok/engine/sources/engine_world.cpp` | current commit `6d00b67e6` |
| `vostok/game/sources/game_world.cpp` | `4a35a62cf` |
| `vostok/game/sources/weapon.cpp` | `acd9c8340` |
| `vostok/ui/sources/ui_text_edit.cpp` | `9054e72f5` |

The earlier line-only view also listed `login_menu.cpp`, `player_tick.cpp`, and
`particle_emitter_instance.cpp`. Their compiland procedure order does not
invert. `login_menu.cpp` contains a retained `#line` directive that directly
demonstrates why attributed line order alone can lie about physical source
order. Those three were comparator false positives, not source fixes.

Fixing all five definition-order rows is real PDB progress, but it resolves only
one semantic channel. It does not make the physical PDB layout equal.

## Complete class variants

The raw view retains every complete class/struct/interface record, groups equal
semantic shapes, and preserves every PDB-local type index. It does not select a
supposedly canonical same-name record.

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

Retail-vs-retail reports all 29,428 class names identical. This validates the
comparison without pretending retail has one shape per name: it contains 267
multi-variant names and 642 names with repeated equal records.

### The six filtered size rows

| normalized row | raw evidence | honest disposition |
|---|---|---|
| `packets_in_list_predicate` | retail sizes `0x1` and `0x2`; candidate `0x2` exactly shares one retail shape | variant overlap; provenance of the unmatched retail form is unresolved |
| `udp_match_connection` | retail `0x530`/`0x538`; candidate `0x538`, but complete nested/type/order shape is disjoint | consumer assembly supports candidate offsets on inspected paths; whole-record provenance remains unresolved |
| `udp_match_client` | retail `0xb20`/`0xb28`; candidate `0xb28`, with other complete-shape differences | unresolved disjoint variants, not a proven duplicate phantom |
| `udp_match_client_session` | retail `0x568`/`0x570`; candidate `0x570`, with property/typedef differences | unresolved disjoint variants |
| `match_client_impl` | retail `0x96ba0`, `0x258ba0`, `0x258ba8`; candidate `0x258ba8`, with nested typedef/order differences | inspected constructor supports the 8192-element candidate arena; other records remain unresolved |
| `btKinematicCharacterController` | one retail `0xe0` record versus candidate `0xd0` | real Bullet/vendor layout difference |

The previous description of five rows as documented duplicate-record
“phantoms” was too strong. A shared shape or one agreeing consumer cannot prove
that every other retained record is stale or unused.

## Full stream/layout result

The physical files are materially different before semantic record pairing:

| MSF inventory | candidate | retail |
|---|---:|---:|
| file pages | 101,907 | 99,291 |
| present stream slots | 2,165 / 2,165 | 2,408 / 2,410 |
| declared stream bytes | 102,622,317 | 99,206,885 |
| stream pages | 101,296 | 98,115 |
| allocation runs | 2,268 | 2,519 |
| fragmented streams | 46 | 44 |
| active free pages | 4 | 740 |

The candidate directory occupies 405 pages in one run; retail uses 393 pages in
seven runs. This is real byte/container layout, but page-number equality is not
a source reconstruction target.

Representative record-order channels show where the difference comes from:

| channel | candidate | retail | shared unique | inversion rate | key result |
|---|---:|---:|---:|---:|---|
| identified stream roles/slots | 2,166 | 2,409 | 2,110 | 19.1443% | 2,095 moved; 2,107 changed sizes/pages; 56 candidate-only, 299 retail-only |
| DBI modules | 2,387 | 2,396 | 2,331 | 19.6655% | 2,329 moved; 56 candidate-only, 65 retail-only |
| DBI EC strings | 958 | 1,139 | 687 | 42.4086% | 675 moved; 1,259/1,597 hash buckets differ |
| global `/names` strings | 8,708 | 9,623 | 6,907 | 16.2759% | 6,905 moved; 10,514 candidate buckets differ and retail has 6,069 more buckets |
| all TPI record kinds | 457,630 | 460,370 | 457,580 | 1.3820% | occurrence pairing finds 148,913 changed records; insertions make this a physical diagnostic |
| named complete TPI records | 33,706 | 33,896 | 30,926 | 16.8261% | 30,926 moved; 4 changed; 262 multiplicity; 877 non-unique keys excluded |
| global symbol stream | 160,408 | 155,692 | 145,083 | 4.7647% | 145,081 moved; 158 multiplicity; 1,279 non-unique keys excluded |
| GSI serialized hashes | 81,844 | 77,304 | 67,240 | effectively zero | only 3 pair inversions; resolved identity order is almost exact |
| PSI serialized hashes | 78,564 | 78,388 | 77,843 | 0% | shared unique hash-record order is exact |
| PSI public address map | 78,564 | 78,388 | 77,843 | 3.6975% | 77,048 moved, reflecting different final addresses/order |
| legacy FPO | 13,906 | 13,992 | 729 | 0% order | 721 shared keys have changed payload/address data |
| frame data | 60,101 | 58,810 | 1,823 | 0% order | 1,822 shared keys have changed payload/address data |

“Moved” is deliberately sensitive: moving one intact block marks records in
both blocks. The report therefore also gives pair inversions, LIS, retained and
reversed adjacency, contiguous runs, increasing runs, and displacement. For
example, DBI retains a 457-object contiguous run despite 2,329 moved records;
named TPI retains a 1,715-record run; the global symbol stream retains a
2,629-record run.

### Modules, source files, and C13

Of 2,331 uniquely paired module scopes:

| per-module channel | differing scopes |
|---|---:|
| source-file reference order | 1,679 |
| all raw symbol records | 1,689 |
| recognized top-level symbol sequence | 1,470 |
| file-checksum records | 2,294 |
| line-program records | 2,105 |
| C13 subsection sequence | 1,201 |
| C13 local string tables | 0 |
| C13 frame/inlinee/cross-map/other payloads | 0 |

The zero rows are meaningful only for the 2,331 paired scopes; the 56
candidate-only and 65 retail-only modules remain one-sided.

### Enum declaration order

Raw TPI contains 2,804 uniquely paired complete enum scopes. Twelve differ and
111 same-name scopes are ambiguous because at least one side contains duplicate
records. Only one of the twelve is a pure order mismatch:
`vostok::render::mesh_type_enum` has all 12 comparable header/value records and
10 pair inversions. Retail places `mt_user_mesh_editable` and
`mt_user_mesh_wire` before the five skinned-mesh enumerators. This is direct,
actionable source evidence and is handled in the follow-up measured commit.

The other eleven are value/name/multiplicity differences: the parked
`resources::class_id_enum`, renamed `udp_match_connection` values, six
Scaleform/vendor scopes, `math::convex`'s unnamed collision, and an
`animation_player` unnamed target-only value. The 111 ambiguous scopes must not
be resolved by arbitrarily selecting a record.

## What the large physical difference means

DBI library grouping explains part of the movement. Of 51 multi-member library
groups, 27 preserve relative order for all shared objects. Several other groups
are intact rotations. VFS, for example, contains the same 34-object and
26-object blocks in opposite order. Neither PDB's DBI order is the archive
member order: it reflects linker extraction/demand order.

The final response file already follows `RETAIL_LINK_LIBRARY_ORDER` in
`scripts/vostok/build/ninja_regen.py`, derived independently from retail section
contributions. Commit `86e53d1a0` moved the then-current build from 9,097 to
10,004 exact functions and from 88.38% to 91.72% fuzzy. The current DBI
interleaving follows neither side's response-file library sequence. Changing
that established order merely to improve a PDB prefix would discard stronger
code and section evidence.

The stream-slot gap is also a roster difference, not just order: the candidate
and retail link different sets of object/library streams, including candidate
zlib/libjpeg objects versus retail LIBCMT-origin objects. Header inclusion can
affect TPI and per-module UDT emission, but the stable unique module-local UDT
subsequence is already close (369 inverted pairs among 7,732 shared unique
records). That supports targeted declaration/PCH investigation, not wholesale
include reordering.

## Stability and limits

A forced identical-input relink in the discovery audit deleted the output PDB
first and rebuilt no object or library. The PDB changed in identity/checksum
bytes, but all then-decoded module, named-TPI, global, and per-module symbol
orders were identical. The expanded comparator's retail-vs-retail self-check
reports zero moves, changes, one-sided rows, or differing scopes in all 58
summary channels. Its unit and integration suite also checks synthetic stream
reordering and malformed payloads.

That establishes reproducibility for fixed inputs and comparator sanity. It
does not turn linker-derived order into source-order proof. In particular:

- type indexes and raw unnamed-record occurrence are insertion-sensitive;
- contribution, public, FPO, and frame records are address-sensitive;
- hashes and buckets change when their input set changes;
- LTCG, COMDAT selection, type merging, and archive extraction affect record
  presence and order;
- absent optional streams contain no recoverable order evidence;
- compiler-discarded source spelling cannot be reconstructed from physical page
  allocation.

## Current conclusion

The candidate is semantically close enough to pair most named entities, but its
PDB is not structurally or physically equal to retail. Five verified
source-definition-order problems are fixed; one newly exposed enumerator-order
problem is actionable; 57 filtered type rows, two normalized enum rows, four
anonymous-constant source rows, raw same-name variants, one-sided entities, and
large linker/container differences remain.

Those figures are an evidence inventory, not one flat work queue. Make a source
change only when the PDB channel actually owns source semantics or when a
physical cluster is corroborated by class structure, function structure,
assembly, section layout, or controlled link experiments. Unmatched same-name
records remain unresolved provenance until they are bound to a compiland or
consumer; they are not honestly described as duplicate-record phantoms.
