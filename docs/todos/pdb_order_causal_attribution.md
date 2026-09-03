# PDB order causal attribution

Status: **open**. Small VS2008 experiments establish several causal rules. Three
bounded C13-guided corrections, two false-compiland removals, and one complete
library-local DBI order correction have now been accepted in the game. A new
section-contribution ownership pass also found and corrected one definition-order
pair whose module-symbol, FPO, and public order all disagreed with retail. The
large DBI/TPI/public residuals remain open; the current MSF slot gap is fully
attributed to the unequal semantic module roster, while physical equality must
wait for that roster and the stream sizes to converge.

## Honest progress statement

The expanded PDB comparison originally found large, stable order differences.
Those inventories were findings, not fixes. The production matching yield before
this causal campaign was limited to:

- two moved `engine_world` definitions from the source-definition comparison;
- two reordered `mesh_type_enum` entries from the complete-enum comparison.

The work recorded here added a reproducible 40-case toy harness, corrected two
false comparison signals, established the cause of several PDB channels, and
applied the first rule to the sound library. That production change corrected
header presence/order in 39 measured sound compilands, brought two small
compilands to zero source-file-order inversions, and left the authoritative code
comparison exactly neutral. A second application recovered a missing direct
header dependency and the target callee in `particle/evaluate_type.cpp`, taking
its only function from 86.25% to byte-exact 100%. These changes did not correct
VFS DBI order, the global named TPI sequence, or physical MSF allocation.

The next application produced the first broad code result from order evidence.
It traced a candidate-only sound PCH compiland to five false exports, then used
the PE export table to find three more false exports from the same reconstruction
mechanism. Removing all eight annotations dropped `sound\pch.obj` from the
linked PDB and moved the full comparison from 75.60% to 76.29%, with 297 more
exact functions. Removing a separate redundant linkage pin then dropped the
last candidate-only sound compiland, `sound_library_linkage.obj`; that second
link rotation cost 0.05 points and 120 exact functions. The last measurement
before the GFx artifact incident described below was 76.24%, 177 exact functions
above the starting point, with the sound module roster exactly 54 versus 54.

The next application established that sound is not an LTCG-order problem at
all: its Master Gold configuration disables whole-program optimization and
builds ordinary `/Od /Zi` objects. The candidate sound DBI sequence exactly
followed `sound_lib.rsp`, so the non-LTCG archive-member rule was directly
applicable. Normalizing the 54 retained members to the retail sequence removed
all 51 sound inversions, raised its LIS from 36 to 54, and preserved all 53
adjacencies. The whole-PDB DBI count fell by exactly 51 and named-TPI inversions
fell by 6,386. Public-address inversions rose by 2,060, while C13, FPO, and frame
counts did not move. The authoritative full build remained 76.24% with
37,103/44,600 exact functions and no changed function comparisons. This is one
complete PDB-order correction, not evidence that every physical order channel
improved with it.

The next pass assigned public, legacy-FPO, and frame rows to their unique DBI
section-contribution owner. That reduced the global address streams to
module-sized subsequences and exposed a seven-function
`object_volumetric_sound.obj` FPO population with one exact adjacent inversion.
Retail source and module symbols both define `resolve_links` before `insert`,
while the candidate source did the reverse. Swapping those two already-exact
empty definitions removed the inversion from the module-symbol stream and FPO
stream, and reduced the module's public inversions from two to one. Code stayed
exactly 76.24% and 37,103/44,600. The remaining public inversion is the class's
scalar/vector deleting-destructor pair; a new small probe found that merely
reversing scalar-delete and vector-delete demand does not steer that generated
order, with or without LTCG.

The latest C13 roster pass recovered three more retail source-file identities
in `game_core\entry_point.obj`. It split the match message enums and UDP orderer
out of two reconstruction-only combined headers, then restored the entry-point
TU's direct `network_messages.h` dependency. The module moved from 2 to 5 shared
unique retail file identities, while the executable comparison remained exactly
neutral. The five shared rows still contain four order inversions and two retail
stdlib rows remain absent, so this is a header-ownership result, not C13 order
closure.

Rebuilding the GFx suite from the current source and relinking moved the raw
whole-executable report from the pre-restage 76.24% and 37,103 exact functions
to 78.79% and 37,711 exact functions: +2.55 points and +608 exact. That is a
vendor-archive/link result, not yield from the C13 header split. It also rotated
many folded owners: the non-vendor ledger's current exact count moved from
10,439 to 10,430 while its hash-scoped exact-max count stayed 11,029. The
first direct-library repeat also showed that MSVC's librarian copied an
advancing clock into every archive-member header: all eight archive hashes
changed while the object cache was untouched. The builder now writes objects
through a fixed Wine path and freezes only the librarian clock. Two consecutive
cache-only passes then produced byte-identical output for all eight archives.
This proves stable re-archiving of one object set; it does not prove that a clean
recompile reproduces identical objects. A fresh-PDB game relink from the frozen
archive set reproduced 78.79% and 37,711/44,600 exact functions with zero
improved, regressed, added, removed, or fold-churn rows against the preceding
link. The ignored rebuilt archives must still be published and pinned before
this measured link is reproducible from a fresh checkout, so the raw increase
is not claimed as a landed source result.

Physical PDB order is not direct source-order proof. It is also not inherently
non-actionable. The useful classification is: **causally understood in the toy
model, still requiring validation on the real link**.

## Reproducible small-example layer

Run the whole matrix or one case inside the repository development shell:

```sh
python3 -m vostok tool pdb-order-probe
python3 -m vostok tool pdb-order-probe --list
python3 -m vostok tool pdb-order-probe --case archive-member-order
```

The harness compiles only tiny VS2008 fixtures, normally with `/Z7` and with a
targeted `/Zi` control where compiler-PDB behavior matters. It links without the
CRT and never builds game sources. It keeps source/output paths fixed and writes
exact commands, linker logs, PDB/EXE hashes, copied artifacts, full
`pdb_topology --order --json` reports, and compact channel summaries under
`binaries/gen/pdb-order-probes/`. That directory is generated and replaced by
the next run.

The current matrix contains 40 cases and emits 49 comparisons because clean,
incremental, and `/Z7`/`/Zi` control cases compare multiple states. Wine output
goes directly to each command's log file: VS2008's persistent `mspdbsrv.exe`
can inherit a pipe after `cl.exe` or `link.exe` has finished and otherwise make
a completed probe look hung.

Every conclusion below is scoped to these fixtures and the repository's
VS2008 SP1 toolchain.
It becomes a production rule only after the corresponding real objects,
libraries, logs, and PDB channels reproduce it.

## Results: DBI, TPI, publics, and frame data

The order counts below are pairwise inversions among shared unique identities,
not raw moved-row totals.

| one changed input | DBI modules | named TPI | public address map | frame data | conclusion in this fixture |
|---|---:|---:|---:|---:|---|
| direct object arguments `root alpha beta` vs `root beta alpha` | 1 | 1 | 1 | 1 | all four channels respond; the two direct non-root objects are serialized in reverse command-line order in DBI |
| archive members `alpha,beta` vs `beta,alpha` | 1 | 1 | 0 | 0 | DBI/TPI track archive-member/pass-2 order; placement does not move because pass-1 extraction demand is unchanged |
| root references `alpha+beta` vs `beta+alpha`, archive fixed | 0 | 0 | 1 | 1 | pass-1 extraction order reverses, while pass-2, DBI, and TPI remain fixed; placement follows the changed extraction demand |
| separate libraries `alpha.lib beta.lib` vs the reverse | 1 | 1 | 1 | 1 | search, load, pass-2, DBI/TPI, and placement all reverse together |
| shared-type contributor objects reversed | 1 | 2 | 1 | 1 | the shared named type moves from `0x1002` to `0x1001`; its earliest retained direct module reference and module-reference sequence reverse |
| first local use of two already-declared types reversed inside one TU | 0 | 0 | 0 | 0 | local use order does not control named TPI order in this case; only the source checksum changes |
| definitions of two `/Gy` functions reversed in one source file | 0 | 0 | 1 | 1 | lexical definition order controls final placement here without moving DBI, TPI, or C13 record order |
| linker `/ORDER` file reverses two functions | 0 | 0 | 1 | 1 | public and frame order follow final function placement independently of DBI/TPI/C13 |
| scalar-delete and vector-delete call order reversed | 0 | 0 | 0 | 0 | generated scalar/vector deleting-destructor order stays fixed in the defining object; only the root frame payload changes with its body size |
| one additional direct object/module | 0 among the three shared modules; one new module | 0 among the shared type | 0 among the shared publics | 0 among the shared frames | one semantic module role adds one stream slot, while downstream stream sizes and physical allocation change |

The same inputs under `/GL`/`/LTCG` do not obey all of the non-LTCG rules:

| one changed `/GL` input | DBI modules | named TPI | public address map | conclusion in this fixture |
|---|---:|---:|---:|---|
| direct object arguments reversed | 1 | 1 | 1 | the two objects reverse in DBI/TPI/placement; a synthetic `* CIL *` module is present |
| archive members reversed | 0 | 0 | 0 | archive member order does not control the surviving LTCG module order |
| unresolved-root demand reversed | 0 | 0 | 0 | pass-1 demand does not control the surviving LTCG module order or placement here |
| archive members reversed with a librarian `/LTCG` index | 0 | 0 | 0 | the librarian's CIL index does not make member order observable in the surviving order |
| unresolved-root demand reversed with a librarian `/LTCG` index | 0 | 0 | 0 | indexed-library demand order remains neutral |
| separate libraries reversed | 3 | 1 | 1 | library order changes the LTCG module sequence and downstream placement; the library modules appear in reverse library order in this fixture |
| compilation order reversed, archive and link fixed | 0 | 0 | 0 | object timestamps or compilation chronology do not control the result |
| source arguments reversed inside one compiler invocation | 0 | 0 | 0 | `/GL` batch source order is not retained when archive and link order stay fixed |
| separate `/Zi` compilations reversed in one shared compiler PDB | 0 | 0 | 0 | compiler-PDB contribution chronology is not retained |
| source arguments reversed inside one `/Zi` batch and shared compiler PDB | 0 | 0 | 0 | neither batch order nor shared compiler-PDB state changes the semantic or stable-stream order |
| direction of one cross-module call reversed, roots and archive fixed | 0 | 0 | 0 | a simple code-dependency edge does not schedule the surviving modules in this fixture |
| direction of one cross-module volatile-data reference reversed, roots and archive fixed | 0 | 0 | 0 | a simple data-dependency edge is likewise neutral |
| definitions of two `/Gy` functions reversed inside one `/GL` source file | 0 | 0 | 1 | final placement follows definition order in this fixture; legacy FPO has the same one inversion, while DBI, TPI, and C13 sequence order stay fixed |
| scalar-delete and vector-delete call order reversed | 0 | 0 | 0 | the LTCG link also preserves the defining object's generated destructor order |

These `/GL` controls invalidate simple production recipes of “put archive
members in retail DBI order” or “make the dependency graph point in retail
order.” The LTCG integration phase can discard both kinds of ordering evidence,
while direct-input and separate-library boundaries remain observable levers.

The archive experiment separates two linker phases that must not be conflated:

- full `/VERBOSE` pass 1 loaded `beta.obj` then `alpha.obj` in both archive
  variants because the unresolved-root demand was unchanged;
- pass 2 listed the archive members in archive order, and DBI plus named TPI
  changed with that order;
- changing only root expression/reference order reversed pass-1 loading and
  public/frame placement, but did not change pass 2, DBI, or TPI.

Therefore a real DBI rotation is not automatically an extraction-demand
rotation. Inspect pass 1, pass 2, direct-object arguments, archive member order,
and library order separately. Public/frame movement is the useful downstream
check for the placement path.

### PCH archive retention

The `pch-archive-retention` case compares a plain inline definition with an
otherwise identical `__declspec(dllexport)` inline definition in a PCH. Under
both `/Z7` and `/Zi`, the exported variant makes the consumer reference
`___@@_PchSym_...`; pass 1 then extracts `pch.obj` from the archive. The plain
variant extracts only `consumer.obj`. The exported link also creates the normal
`probe.exp` module, so its two added DBI modules must not both be attributed to
PCH retention.

The debug modes differ only in the consequence of omitting the creator object.
Plain `/Z7` emits LNK4206 and loses the consumer's precompiled debug types;
plain `/Zi` retains them in the compiler PDB without extracting `pch.obj`.
`/Yd` does not suppress the export-induced reference. Nor does `/Yl-` disable it
with VS2008: the hyphen is parsed as the requested PCH-symbol suffix. The usable
production rule is therefore to remove a false export, not to hide the PCH
dependency with a compiler switch.

### Named-type provenance

`pdb_topology --order` now reports `named_type_module_references`. It joins a
named complete TPI index to direct module symbol references from procedures,
locals, data, constants, UDT symbols, and related typed records, preserving DBI
module order. For the shared-type fixture it shows both modules and which is
earliest after the object-order change.

This is deliberately called the **earliest retained direct module reference**.
It is evidence about contribution order, not proof of an original lexical owner:
an indirect field/signature reference may not appear as a direct module symbol,
multiple modules may retain references, and merged duplicate variants remain
possible.

## Results: C13 source/checksum/line order

| one changed input | measured result | negative/control result |
|---|---|---|
| direct include order `alpha.h,beta.h` vs reverse | header checksums have 1 inversion; the two three-line header function blocks have 9 line-record inversions | DBI and named TPI stay fixed; header-defined public/frame records move with the emitted functions |
| include order inside `pch.h` | `pch.obj` header checksum order has 1 inversion; `pch.h` checksum changes in PCH and consumer scopes | no named TPI, line, public, or frame-order movement for these data-only header declarations |
| two nested function-bearing headers included after a fixed PCH | checksum and line streams each have 9 inversions; named TPI/reference, public, and frame sequences each have 1 | the fixed PCH and DBI module order do not move |
| declaration-only and function-bearing headers reordered after a fixed PCH | checksum streams have 14 inversions | named TPI, line, public, and frame sequences stay fixed because the reordered declarations do not contribute those records |
| a header already reached late through the PCH is explicitly pre-included before its umbrella | surrounding checksum records have 8 inversions | the transitive header itself retains its relative position; TPI, line, public, and frame sequences stay fixed |
| the same `#pragma once` PCH-owned header is redundantly included immediately before versus after `pch.h` | no semantic sequence moves | only the edited source checksum changes; this result does not cover traditional macro guards |
| the same traditionally guarded PCH-owned header is redundantly included after versus before `pch.h` | the post-PCH form retains the header before the source in the consumer checksum roster; the pre-PCH form omits it | the result is identical under `/Z7`, `/Zi`, and `/Zi /GL /O2`; DBI, TPI, line, public, and frame order stay fixed |
| harmless same-line comment changes alter only a header or source checksum | one changed checksum value under ordinary and `/GL` compilation | no checksum-order, line, TPI, public, or frame movement; checksum bytes are not an order lever |
| one PCH consumer moves from first to last in a fixed-link `/MP /Zi /GL` batch | no measured semantic or physical-order movement | batch argument position alone does not explain a consumer's local checksum sequence |
| `shared.h` supplied by PCH vs included directly after the PCH | the direct variant retains `shared.h` in the consumer module's checksum roster; the PCH-provided variant does not | named and raw TPI order remain the same; this is a roster/attribution change, not a TPI-order change |

These results make module-local C13 useful for include/PCH diagnosis. They do
not justify global include sorting. A real edit needs the first differing
adjacency in one compiland, dependency-valid source order, and agreement with
class/enum/function evidence.

A real negative control in `particle\help_functions.obj` reinforces that last
restriction. Retail has 22 inversions among ten shared checksum rows, with the
`math_randoms_generator.h` contribution before the local helper contribution.
Reversing the two direct includes in `help_functions.cpp` did not reproduce
that block: the disposable full link rose to 25 inversions and still placed the
math/random contribution after the helper. The edit was reverted. In this
case the checksum sequence reflects emitted inline/function contributions, not
the apparent lexical include adjacency alone.

### Why the smallest remaining C13 row is not yet a one-line edit

The compact whole-PDB pass selected `core\threading_event.obj` because it has
only one inversion among three shared first-party checksum rows. Retail records
`threading_event.h` before `threading_event.cpp`; the candidate records the
reverse. Both sides otherwise have the same 13 line records, the same C13
subsection sequence, and the same sole retained procedure, whose body is already
byte- and structure-exact.

That pair is real, but the isolated controls exclude the obvious edits:

- both retail and candidate core PCH compilands contain `threading_event.h`;
- moving a traditionally guarded redundant include before `pch.h` removes the
  header row instead of reversing it;
- harmless source/header checksum changes do not move the row;
- `/Zi`, `/GL`, optimization, and `/MP` batch position do not change the rule.

The remaining upstream difference is not small. The core PCH checksum streams
contain 663 retail and 685 candidate rows, with 613 shared unique paths and
70,090 pair inversions. `threading_event.h` is at retail position 309 and
candidate position 362. Therefore the one-inversion consumer row cannot safely
be treated as an isolated include-order instruction: it is downstream of a
substantially different PCH contribution history. The next source-level unit is
the core PCH roster/order, not a speculative swap in an already exact
`threading_event.cpp`.

The first-party roster delta narrows that upstream task further. Retail has two
unique Vostok paths in the core PCH (`logging_extensions.h` and
`fs/device_file_system_proxy_impl.h`); the candidate has 24, consisting of the
split/core logging topology, `list_inline.h`, and 16 optional math headers. This
is not permission to replace one include mechanically. Retail procedures and
types use the newer logging ABI even though the core PCH attributes its source
to the flat header path, while the current flat header still contains an older
API. The actionable reconstruction unit is therefore the logging/PCH header
ownership boundary (plus the missing FS implementation header), with build
errors and per-compiland target rosters used to restore direct dependencies.

## Production validation: public and frame/FPO placement

The source-definition and linker-`/ORDER` fixtures establish two independent
ways to move the same downstream channels. Real records must therefore be
checked against both source-line order and emitted address order.

Two exact adjacent swaps in `sound_scene.cpp` provide that check:

- source/PDB lines put `compare_propagator_info_by_distance` before
  `sound_scene::notify_listener` (retail lines 421 and 426), but retail address
  and frame order put `notify_listener` first; and
- source/PDB lines put `closest_point_on_segment` before
  `sound_scene::get_portal_nearest_point` (retail lines 1307 and 1315), but
  retail address and frame order again put the large member function first.

All four functions have matching statement counts and exact target/base byte
sizes. The candidate follows lexical order for both pairs, while retail
reverses it at placement. Reordering definitions would therefore make source
structure less faithful; these frame rows are downstream linker-placement
evidence, not a license to sort the source by RVA.

The same distinction appears under LTCG in `udp_match_connection.cpp`. Retail
places the constructor immediately before `is_low_level_packet` and the
destructor immediately before `instant_disconnect`; the candidate reverses
each pair even though the current lexical definition order already starts with
constructor then destructor then `is_low_level_packet`. The helper also has a
known inline-vs-call body mismatch. This makes source/body convergence and CIL
placement the next evidence, not a manual public-order edit.

### Section-contribution ownership

Global public, legacy-FPO, and frame sequences are now also grouped by the DBI
section contribution that contains each record's address. Publics retain their
original section/offset; FPO and frame RVAs are converted back through the PDB
address map first. A record is admitted only when exactly one half-open
contribution range contains it. Overlaps remain ambiguous and addresses outside
all contributions remain unowned instead of being guessed.

The current whole-game coverage is:

| channel | candidate owned | retail owned | differing module groups | differing library groups |
|---|---:|---:|---:|---:|
| frame data | 60,086 / 60,086 | 58,810 / 58,810 | 881 / 940 | 21 / 22 |
| legacy FPO | 13,912 / 13,912 | 13,992 / 13,992 | 476 / 706 | 23 / 24 |
| PSI public address map | 76,993 / 78,563 | 76,953 / 78,388 | 974 / 1,998 | 55 / 69 |

This is **placement ownership**, not proof of the original lexical source owner.
ICF aliases may resolve into the surviving contribution, and the global stream
still reflects linker placement. Its benefit is bounded attribution: a small
module can now be inspected as a small ordered subsequence instead of searching
millions of global pairs. A candidate source edit still requires agreement with
the target compiland symbols, source lines, declarations, and bodies.

## Results: MSF stream allocation and relinking

The allocation experiments distinguish output identity, semantic content, and
physical writer state:

- Two independent clean links with identical objects reproduce every decoded
  semantic sequence and every stable stream role/index/page binding. PDB and EXE
  hashes still differ; `llvm-pdbutil` confirms a different PDB GUID even though
  age and decoded topology agree.
- Clean `/INCREMENTAL:NO` versus fresh `/INCREMENTAL:YES` is not a pure allocator
  experiment. The incremental link changes section/contribution/linker metadata
  and three stream sizes even with the same objects.
- Successive no-input updates of one incremental PDB keep DBI, TPI, C13,
  public, and frame order fixed. The active free-page-map copy alternates, while
  old-directory and PDB-info pages move through the file. The first and third
  layouts differ, disproving a simple two-layout cycle.
- A second fresh three-update sequence reproduces the decoded MSF layout and
  every stable role/index/size/page binding at the corresponding first, second,
  and third updates. The files still have different GUID lineages and hashes.

This establishes deterministic physical allocation for a fixed fresh state and
update count in the toy environment, not byte-identical PDBs and not a universal
page formula. Whole-game page matching should wait until semantic stream rosters,
sizes, link mode, and incremental history agree.

The module-roster control makes that prerequisite concrete. A clean link with
`root.obj` and `alpha.obj` was compared with the same link plus one direct,
otherwise unreferenced `beta.obj` under `/OPT:NOREF`. The added object produced
exactly one additional DBI module and one additional identified module stream:
15 to 16 present stream slots. No shared DBI, named-TPI, public, or frame pair
changed order. Nevertheless, 14 of 17 stable roles changed in at least one of
stream index, size, page list, or run layout because the added module also
changed DBI/TPI/symbol/frame contents. This is direct evidence that page movement
can be a downstream consequence of a semantic-roster difference rather than an
independent writer-order defect.

The whole-game stream inventory has the same causal obstruction at production
scale:

- all 54 candidate-only and all 299 retail-only identified stream roles are DBI
  modules; neither PDB has an unidentified stream;
- the net semantic-role difference is `299 - 54 = 245`, exactly the difference
  between 2,408 retail and 2,163 candidate present streams;
- 234 of the retail-only module streams come from `LIBCMT.lib`; another 49 are
  retail `libjpeg.lib`, `libpng.lib`, and `zlib.lib` modules whose candidate
  counterparts appear under the reconstructed `libgfx_libjpeg.lib`,
  `libgfx_libpng.lib`, and `zlibN.lib` ownership;
- only 10 shared stable roles retain the same stream index, and only PDB-info
  retains the same index, size, and page count. Even that fixed stream occupies
  a different physical page (`101496` candidate versus `61511` retail).

Therefore the current 245-slot gap is completely explained as a semantic module
roster gap. It is not evidence for 245 independently missing allocator actions.
The gap grew by two when the false sound PCH and linkage modules were removed;
that is a correctness improvement, not an allocator regression.
MSF page/run comparison becomes a source/linker task only after module ownership,
CRT/vendor participation, stream sizes, and link history agree; before then it
is useful as a deterministic end-state check.

## Comparator defects exposed by the probes

The small cases found two false signals in the parser, missing attribution
channels, and one indexing trap in the new ownership implementation:

1. Some VS2008 C13 checksum records use offsets in global `/names` when the
   module-local string table is empty. Resolving those offsets only locally made
   unchanged files look different. The parser now falls back to global `/names`.
2. Frame/FPO rows were paired by RVA, so a function-placement swap appeared as
   changed payloads instead of an order inversion. Unique module procedure
   identities now key those records; unresolved or ambiguous RVAs still retain
   the weaker RVA key.
3. Named TPI rows lacked compiland evidence. The new direct-module-reference
   channel exposes reference sets, their earliest DBI-ordered module, and the
   first retained typed symbol inside that module without inventing ownership.
4. `pdb2` exposes a DBI section contribution's module index as zero-based. An
   initial ownership join subtracted one and attributed every address to the
   previous module. The deleting-destructor fixture made the error obvious by
   assigning the root entry point to the defining object. The join now uses the
   index directly and has explicit zero-based, half-open-range, overlap, and
   global-subsequence tests.

These corrections matter before production interpretation: otherwise the tool
can turn ordinary string-table relocation or function movement into a misleading
source-level difference.

## Whole-game baseline residual inventory

The complete pre-correction production snapshot remains in
[`pdb_comparison_audit.md`](../binary_matching/pdb_comparison_audit.md). It is a
scheduling inventory, not a count of actionable source edits:

| channel | current observation | likely evidence to collect next |
|---|---|---|
| DBI modules | 2,331 shared unique objects; 531,234 pair inversions (19.5622%); many are intact rotations | direct/link response order, archive member order, full `/VERBOSE` pass 1 and pass 2 |
| named complete TPI | 30,927 shared unique records; 79,962,967 pair inversions (16.7208%); a 1,715-record contiguous run survives | named-type direct-module references, DBI grouping, then PCH/header evidence |
| module source files | 1,679 of 2,331 paired scopes differ | module-local direct/PCH include graph and first differing adjacency |
| module checksums and lines | 1,679 checksum, 1,308 line, and 1,192 C13-subsection scopes differ | actual source roster, checksum content, statement/function ownership |
| public address map | 114,884,712 pair inversions (3.7909%) among 77,853 shared unique publics | extraction and final section/function placement |
| FPO and frame data | 1,852,818 and 37,236,749 pair inversions among 10,440 and 19,364 stable shared identities | downstream extraction, placement, size, and frame-shape validation |
| MSF allocation | 2,163 candidate stream slots versus 2,408 present retail slots | stream roster/size, clean/incremental mode, writer history |

GSI has only three pair inversions among 67,252 shared unique records and PSI
hash-record order has none among 77,853. That control remains important: large
movement in one physical channel does not imply every internal index is wrong.

### Focused named-TPI attribution

The first adjacency pass found 232 physically adjacent swaps among named TPI
records. Sixty-one pairs are wholly first-party. Ten of those 61 have the same
earliest direct-reference module for both types in both PDBs, so module order
alone cannot explain them. This is a triage reduction, not ten authorized source
edits: the retained symbol sequence inside the module must also be checked.

`pdb_topology --order --type <substring>` now performs that narrower query. It
reports each selected type's physical TPI position, direct module sequence, and
first retained typed symbol in the earliest module, including its containing
procedure where available. Two production pairs establish the current limits:

- `survarium::triangle_orientation` precedes `survarium::collision_result` in
  retail TPI, while the candidate has the reverse. Both PDBs first reference
  them in `bullet.obj`; more importantly, both retain the same direct-symbol
  order: local `orientation` in `bullet::fix_collision_point_and_time`, then
  local `result` in `bullet::check_collision`. The complete `survarium::bullet`
  class, including method declaration order, is identical. Reordering these
  uses or methods would contradict the stronger evidence. The causal boundary
  lies earlier than the retained symbols, for example in dependency/PCH
  contribution or type-server/link merging; direct declaration provenance must
  be established before editing either type definition.
- `survarium::vostok_scaleform_log` precedes
  `survarium::vostok_file_opener` in retail TPI and in the retail data-symbol
  sequence, while both orders are reversed in the candidate `factory.obj`.
  Both complete class records are identical, both dynamic-initializer structures
  match, and the global construction order already agrees with retail. A
  controlled edit swapped only the two direct header includes. After an owning
  library rebuild and authoritative relink, candidate TPI remained file opener
  then logger and the first data symbols remained `g_file_opener` then
  `g_vostok_logger`. The edit was reverted. Textual include order at that site
  is therefore not the lever; the next evidence boundary is the Scaleform PCH,
  dependency graph, or LTCG type merge.

Thus order comparison has produced one useful exclusion rule and a bounded
queue, but no accepted TPI-only production correction yet. Its demonstrated
production fixes remain the sound C13 header topology and the Particle
callee/header correction described below. Counting all 61 first-party swaps as
actionable would overstate the result just as badly as dismissing all physical
order as noise.

## First production application: VFS DBI rotation

`vostok_vfs-static-gold.lib` is the first real application. Its 60 shared DBI
modules are one exact block rotation, not 60 independent discrepancies:

- retail is `A(26) + B(34)`;
- the current candidate is `B(34) + A(26)`;
- this gives exactly `26 * 34 = 884` inversions, an LIS of 34, 58 retained
  adjacencies, two increasing runs, and no one-sided module.

The production evidence collected so far rejects the earlier extraction-order
explanation:

1. The current archive has 68 members. Its raw member sequence matches neither
   PDB's DBI sequence.
2. An isolated full `/VERBOSE` relink extracts the 60 linked VFS objects in
   retail's exact `A + B` order during pass 1.
3. That same relink emits the current candidate's exact `B + A` DBI rotation.
   Thus candidate DBI order does **not** reflect pass-1 extraction/demand order.
4. The `/GL` pass-2 log exposes only `CIL library (CIL module)`, not the
   individual VFS compilands, so it cannot directly reveal the internal
   integration order.
5. A split-library control using existing objects forced pass-1 extraction to
   stay `A + B`, but DBI became `mount_referers + B + A-without-mount_referers`.
   Repeating the first library on the response line did not turn the split into
   a clean retail-order lever.

6. The compiler response-file order matches neither PDB block, and reversing
   source arguments within one `/GL` invocation or the chronology in a shared
   compiler PDB is neutral in the small fixtures.
7. Giving a small archive a librarian `/LTCG` index leaves both archive-member
   and unresolved-root-demand reversals neutral.
8. A temporary real 68-member VFS archive was rebuilt with all 60 extracted
   members in retail's exact `A + B` order, followed by the eight unused
   members. An otherwise unchanged full link still emitted the exact candidate
   `B + A` DBI sequence and all 884 inversions. The repository archive was
   restored byte-for-byte after the experiment.

The honest status is therefore: **not fixed, but parked behind a concrete
CIL-internal integration-order wall under the faithful one-library topology**.
Raw archive order, pass-1 extraction, root demand, compiler invocation order,
shared compiler-PDB chronology, and librarian indexing have all been varied
independently without producing retail order. Splitting VFS into multiple
libraries can steer order, but it changes PDB ownership/topology and already
produced a third, non-retail sequence; retaining that split would falsify the
shipped build structure. A future attempt needs new evidence about the CIL
scheduler or an object-content correction that changes its dependency graph.
The failed layouts are evidence, not changes to keep.

After this VFS rotation is explained end to end or parked behind a concrete
non-steerable linker boundary, apply the proven rule to other intact groups. Do
not schedule hundreds of moved rows as hundreds of edits.

## First production C13 correction: sound header ownership

The first accepted source correction started from one small compiland rather
than treating the whole-PDB inversion inventory as a work queue.
`sound\encoded_sound_interface.obj` originally exposed this relevant C13
source-file sequence:

- candidate: `api.h`, `playback_mode.h`, `sound_cone_type.h`;
- retail: `playback_mode.h`, `channels_type.h`, `api.h`.

The cone/channels pair was initially one-sided, so the old comparator population
showed only one inversion among the shared API/playback records. Inspection of
the sound headers found two concrete source problems: `sound.h` included the
cone header instead of the retail channels header, and `channels_type.h`
contained a duplicate `channels` enum already owned by `speakers.h`.

The retained correction does four related things:

1. while building the sound library, `sound.h` includes `playback_mode.h`,
   `channels_type.h`, then `api.h`;
2. `channels_type.h` retains only `channels_type` and removes the duplicate
   speaker-channel enum;
3. `sound_emitter.h` directly includes the cone type that its interface uses;
4. `sound_memory.h` declares the allocator typedef it needs instead of pulling
   all of `api.h` into the sound PCH.

The fourth change matters independently. After the first three edits, the
candidate sequence was `api.h`, `playback_mode.h`, `channels_type.h` because
`sound_memory.h` had already contributed API through `pch.h`; retail remained
`playback_mode.h`, `channels_type.h`, `api.h`, for two inversions. Moving that
dependency out of the PCH made all eight comparable source-file records agree
in order. The remaining one-sided source-file row is only the expected absolute
path spelling of the generated `.pch` file. `panning_lut.obj` also moved from
two inversions to zero.

For the same fixed population of 54 sound scopes:

| state | inversions | moved rows | only candidate | only retail | candidate-only cone | candidate-only channels | retail-only channels |
|---|---:|---:|---:|---:|---:|---:|---:|
| before the correction | 84,514 | 3,836 | 1,370 | 472 | 46 | 0 | 41 |
| header ownership/order corrected, API still in PCH | 87,886 | 3,875 | 1,359 | 433 | 31 | 4 | 2 |
| API removed from PCH | 87,130 | 3,870 | 1,359 | 433 | 31 | 4 | 2 |

The first step removes 39 retail-only `channels_type.h` omissions and 50 net
one-sided rows. Its inversion count rises because records that were absent and
therefore incomparable become shared; that is not a regression and is why raw
inversions cannot serve as a flat progress score. The PCH correction then
removes 756 inversions from that comparable population. Most scopes improve,
a few shift slightly, and substantial unrelated sound C13 differences remain.

The authoritative full build reports zero improved, regressed, added, removed,
or fold-churn functions, with the code score unchanged at 75.66% and
37,041/44,600 exact functions. Thus this is a demonstrated PDB-structure fix,
not a claimed code-match gain.

The full-PDB check also rejected an initially unconditional `channels_type.h`
include in public `sound.h`: despite fixing the sound population, it made that
header candidate-only in 76 engine/game compilands where retail does not record
it. Guarding the include with the sound library's existing
`VOSTOK_SOUND_BUILDING` boundary retains the intended sound order without
exporting the private contribution to those consumers. In the full PDB it
reduces candidate-only `channels_type.h` scopes from 76 to 4; the remaining four
are inside the sound population and are included in the table above.

A separate attempt to move a direct include in `node_lock.h` had no effect: both
types had already entered through that module's PCH. It was reverted. This is
the production counterpart of the PCH-boundary negative control above and is a
reason to inspect actual contribution provenance before editing include order.

## Second production C13 correction: Particle callee provenance

`particle\evaluate_type.obj` was selected because its source-file stream had
only three inversions among six shared unique files and one genuine retail-only
first-party row: `vostok/strings_functions.h`. The source called
`vostok::strings::compare(...) == 0` while depending on the Particle PCH for
string declarations. Direct target disassembly instead called
`vostok::strings::equal` twice, and that helper is independently byte-exact in
the candidate.

Adding the direct `strings_functions.h` include and spelling the two conditions
with `strings::equal` produced all of the intended evidence in the authoritative
build:

- the function remains `STRUCTURE MATCH`, with three statements and target/base
  size `0x3f`;
- all 24 instructions now agree and the ledger score rises from 86.25% to
  100%;
- the checksum roster changes from candidate 7 / retail 8 / shared 6 to 8 / 8 /
  7, removing the only retail-only first-party file;
- the module's C13 subsection comparison changes from one differing payload to
  identical; and
- checksum order retains three inversions. They are the position of
  `strings_functions_inline.h` relative to the local source/header rows, not a
  missing include or a remaining function-body mismatch.

This result also corrects an earlier incomplete park conclusion. Testing the
`equal` spelling without the C13 provenance had looked like an unhelpful
conversion/inlining variant. The missing-header row identified the complete
source change; on a fresh authoritative LTCG link it emits the retail call,
including the boolean return conversion, and matches exactly. The remaining
three checksum inversions are therefore kept as downstream emitted-line/COMDAT
placement evidence rather than chased with redundant includes or artificial
inlining controls.

The first required relink also changed the global LTCG fold selection: the raw
objdiff report moved from 75.66% to 75.60%, with 133 regressed, 184 improved,
and 103 separately classified fold-churn functions. The derived ledger, which
recovers exact folded aliases and preserves hash-scoped evidence, moved in the
opposite direction: current exact functions increased from 10,304 to 10,306 and
exact-MAX functions from 10,875 to 10,876. An unchanged authoritative rebuild
then reported zero added, removed, improved, regressed, or fold-churn functions,
and the Particle function remained exact. Both views are recorded because the
local correction is proven, but the broad raw-link redistribution is a review
risk rather than something to conceal behind the net ledger improvement.

## Third production application: false exports and sound PCH retention

The focused sound DBI report initially contained 56 candidate modules versus 54
retail modules. Its one-sided rows were `pch.obj` and
`sound_library_linkage.obj`. The archive itself legitimately contains both, so
deleting members or PCH support would have been the wrong fix; the question was
why the final link extracted them.

The sound PCH object's `.drectve` section exported five inline definitions:
Boost's `bad_function_call` constructor, `float3_pod::normalize`,
`float4x4::identity`, `create_translation`, and `create_rotation`. Their
consumers also carried `PchSym` references. The small PCH-retention probe above
reproduced that exact edge. An independent PE check then showed 27 candidate
exports versus 21 retail exports. The eight candidate-only exports were those
five plus `mul4x3`, `interlocked_and`, and `interlocked_or`; none existed in the
retail export table. They all came from the reconstruction-only
`DISABLE_LINKER_GL` export macro or an explicit Boost `dllexport`.

Removing those annotations gives three mutually supporting results:

- the eight candidate-only exports disappear; the remaining export-table
  difference is only the two retail-only Scaleform allocator exports;
- the seven inspected consumers no longer reference `PchSym`, and the final
  PDB no longer contains `sound\pch.obj`; and
- the affected functions themselves remain present and byte-exact, including
  the five PCH definitions and the three out-of-line definitions.

That authoritative build moved raw code match from 75.60% to 76.29% and exact
functions from 36,926 to 37,223. It reported 495 improvements, 143 regressions,
and 84 separately classified fold-churn rows. The +0.69 points and +297 exact
functions are the measured link outcome, not a claim that every moved function
was independently repaired by its own source edit.

The other one-sided module was caused by an explicit
`VOSTOK_INCLUDE_TO_LINKAGE(sound_library_linkage)` in the Survarium executable's
linkage TU. Removing only that pin reduced sound from 55 candidate modules to
54, exactly matching the retail population. It left the sound library and its
entry-point linkage intact. This second clean link moved 76.29% to 76.24% and
37,223 to 37,103 exact functions, with 196 improvements, 129 regressions, and
105 fold-churn rows. The PDB-roster correction is retained despite that local
score cost because the candidate-only compiland is direct contrary evidence.

Across both corrections, raw code match is up 0.64 points and 177 exact
functions. Sound is now 54/54 modules with no one-sided row. Neither export
cleanup nor linkage-root removal changed the 51 order inversions that remained
at that point; the separate archive-order application below addresses them.

## Fourth production application: sound archive-member order

The remaining sound order was initially investigated alongside the `/GL` VFS
rotation. That classification was wrong. `sound.vcproj` has
`WholeProgramOptimization="0"` in Master Gold, and `sound_cl_1.rsp` contains
`/Od /Zi` without `/GL`. Sound is the only large first-party library in this
configuration; the other non-LTCG libraries are the vendor zlib variants.

Three production observations identify the lever rather than merely correlate
with it:

1. the candidate DBI sequence for all 54 retained sound modules exactly matched
   the generated `sound_lib.rsp` archive-member sequence;
2. retail differed from that response by 51 inversions, while it differed from
   the raw `.vcproj` file sequence by only 14 and shared the first 43 objects;
3. the small non-LTCG archive probe had already shown that member order moves
   DBI and named TPI even when pass-1 extraction and final placement stay fixed.

The extra candidate movement came from generator traversal: `vcproj2ninja`
emits child-filter files before files directly owned by their parent filter. The
retained correction therefore normalizes the evidenced 54-member subset in
`sound_lib.rsp` after generation. It activates only if every evidenced member
is present, rejects duplicates, and leaves members absent from the retail PDB
in their original generated slots. A regeneration dry run changes only
`sound_lib.rsp`; no source compile order, library boundary, or symbol presence
is invented.

The focused relink produced exact sound DBI equality:

| metric | before | after |
|---|---:|---:|
| shared modules | 54 | 54 |
| pair inversions | 51 | 0 |
| longest ordered subsequence | 36 | 54 |
| preserved adjacent pairs | 18 / 53 | 53 / 53 |
| increasing runs | 17 | 1 |
| rank displacement sum | 86 | 0 |

At whole-PDB scope, DBI inversions changed from 531,285 to 531,234 and named
complete-TPI inversions from 79,969,353 to 79,962,967. Public-address
inversions changed in the opposite direction, from 114,882,652 to 114,884,712.
FPO, frame-data, and C13 differing-scope counts were unchanged. The
authoritative full build was a compile/link no-op and reported no added,
removed, improved, regressed, or fold-churn functions: code remained 76.24%
with 37,103/44,600 exact functions.

This result closes the sound DBI-order discrepancy and demonstrates that a
target-supported input-order correction can be code-body neutral while moving
several physical PDB channels in different directions. It does not make public
order a source-edit target, solve the LTCG VFS rotation, or reduce the semantic
stream-roster gap.

## Fifth production application: `object_volumetric_sound` placement

The ownership summaries selected `object_volumetric_sound.obj` as the smallest
first-party legacy-FPO group with a pure order difference: seven records existed
on each side with identical payloads, and the only inversion was
`resolve_links` versus `insert`. Three independent target channels agreed on the
cause:

- the retail source reconstruction defines `resolve_links` before `insert`;
- the target module symbol stream emits them in that order; and
- target FPO/address order places `insert` at `0x5b3680` and `resolve_links` at
  `0x5b3690`, consistent with the target's reverse-emission behavior for this
  pair.

The current header already had the retail declaration order, `insert` then
`resolve_links`; declaration and definition order are separate evidence. Both
empty bodies were already byte- and structure-exact. The retained edit therefore
changes only their out-of-class definition order to match the retail source.

| owned channel in `object_volumetric_sound.obj` | before | after |
|---|---:|---:|
| module-symbol inversions among 8 shared unique records | 1 | 0 |
| legacy-FPO inversions among 7 shared unique records | 1 | 0 |
| public-address inversions among 27 shared unique records | 2 | 1 |

The whole legacy-FPO inventory correspondingly fell from 477 to 476 differing
module groups. The authoritative code comparison remained 76.24%, with
37,103/44,600 exact functions and zero improved, regressed, added, or removed
rows. This is one source-faithful order correction with three matching PDB
effects; it is not a function-body score gain.

The remaining public inversion is the generated scalar- versus vector-deleting
destructor pair. A dedicated fixture reversed only the order of `delete p` and
`delete[] q`, under ordinary `/O2 /Gy /OPT:ICF` and `/GL /LTCG` links. Neither
variant moved the generated destructor order, public map, DBI, or named TPI. The
ordinary root frame record changed size because its call sequence generated a
different body, but no frame-order inversion appeared. Consequently local
delete-demand order is excluded as the lever; the production destructor pair
remains open rather than being forced with an unrelated call-site edit.

## Sixth production application: game-core network header ownership

The compact C13 ranking selected `game_core\entry_point.obj` because retail had
eight checksum/file identities while the candidate had only three. The two
shared source identities were already in the same relative order; the useful
signal was instead three retail-only first-party paths:

- `vostok/network/message_types.h`;
- `vostok/game_core/network_messages.h`; and
- `vostok/network_core/udp_match_packets_orderer.h`.

The current reconstruction had collapsed the client/server message enums into
`game_core/network_messages.h`, and had collapsed the UDP count tag, message
metadata, and abstract orderer into `network_core/udp_match_types.h`. The retail
C13 paths prove that the first and third source files existed, while the target
type structure independently confirms the orderer's class name and layout. The
retained correction restores those two retail-named headers, leaves
`udp_match_types.h` as a compatibility include, updates direct consumers, and
includes `network_messages.h` in the entry-point TU.

After the authoritative build, the entry-point file stream changed as follows:

| metric | before | after | retail |
|---|---:|---:|---:|
| total file rows | 3 | 6 | 8 |
| shared unique identities | 2 | 5 | 5 of the candidate rows |
| retail-only first-party identities | 5 | 2 | 0 is the eventual goal |
| inversions among the shared population | 0 among 2 rows | 4 among 5 rows | 0 |

The rising inversion count is population expansion, not evidence that the old
three-row roster was closer: absent rows could not participate in any pair. All
three target-backed header identities are now present. Their candidate sequence
is still `entry_point.cpp`, `network_messages.h`, `message_types.h`,
`udp_match_packets_orderer.h`, `pch.h`, while retail places the message-types
contribution before the generated PCH row and the network/orderer contributions
after the source and PCH-header rows. A pre-PCH direct include was tested and
MSVC correctly diagnosed it as skipped while looking for the PCH; it was
reverted. The two remaining retail-only first-party files are
`stdlib_extensions.h` and `stdlib_extensions_inline.h`.

The full code report remained 76.24% and 37,103/44,600 exact functions, with
zero improved, regressed, added, removed, or fold-churn rows. Therefore the
accepted result is exactly three recovered PDB source identities and more
faithful header ownership. The remaining four inversions and two missing stdlib
rows are open; neither a warning-producing include before `pch.h` nor artificial
references will be retained to make the file stream look better.

A broader header split was explored but is not counted as a negative source
result. Nine network login/match compilands have a retail-only
`network/message_types.h` row where the candidate retains
`login_server/message_types.h`, so moving the legacy declarations wholesale
initially looked attractive. During that experiment an unrelated
`vostok tool libs` invocation restaged the GFx shipping archives. The pinned
archive payload records its builder's old
`Z:\home\sheep\Projects\survarium\vostok-gfx\binaries\gfx-sdk` source root,
while the current readers accept the release contract's
`C:\survarium\gfx-sdk` root. The next relink consequently reduced the indexed
base population from 35,665 to 19,434 and printed a false 42.34% comparison.
Direct PDB inspection found essentially the same procedure population as the
unaffected xray worktree PDB (48,698 versus 48,712), including 18,588 procedures
under the unrecognised legacy root. The source experiment and restage were therefore
confounded; the broad source move was reverted without assigning the collapse
to it, and the fixed-root archives were rebuilt before further measurement.
The base rich, delink, and data-index drivers now derive the actual GFx roots
from compile-command records in the PDB being parsed, falling back to the
staged archives before the first link. They accept those roots alongside the
fixed release contract, so another restage cannot silently turn a
path-provenance difference into thousands of missing functions. Raw PDB
comparison still exposes the different paths. An intermediate end-to-end replay
that recognized the C++ archive root expanded the rich index from 25,233 to
43,821 rows, exactly recovering those 18,588 procedures; the generalized reader
also covers the separately recorded C-library SDK root. That second replay
produced 44,555 rich rows versus 44,542 in the unaffected xray worktree and
recovered the report from 42.34% to 75.84% with 35,618 exact functions. It did
not recover the pre-restage 76.24% and 37,103 exact functions, proving that the
staged archive payload also changed real link/pairing state independently of
the source-root filtering defect. The from-source archive rebuild is therefore
part of restoring the measured state, not merely cosmetic PDB-path cleanup.

Those nine C13 rows remain evidence for a future controlled split of generic
network error/message declarations from the login-server configuration macro.
They do not prove that the two current headers should be merged, and the
confounded run proves nothing about the proposed split's executable effect.

## Ranked address-order rows rejected as source edits

Owner-scoped public/FPO ranking made several global differences small enough to
audit. It did not turn them into automatic source edits. The following rows were
checked and left unchanged because stronger retail evidence already agrees with
the candidate source:

- `console_impl_input.obj` (`make_next_tip` / `apply_new_tip`) and
  `game_resman_free.obj` (`collect_to_free` /
  `try_collect_parents_to_free`) have byte- and structure-exact functions with
  target-faithful lexical order; only downstream addresses differ.
- `synchronous_device_interface.obj`, `weapon_core_base_state.obj`, and
  `effect_options_descriptor.obj` likewise retain the retail source-line order
  even though their final public/FPO order differs.
- `effect_editor_gbuffer_to_screen.obj` differs only in two literal addresses
  inside an otherwise byte-exact function, so changing statement order would
  contradict the instruction and line evidence.
- the smallest Particle and panning-LUT public groups are scalar/vector deleting
  destructor pairs. The dedicated fixture already showed that reversing delete
  demand does not steer their generated order.
- `affects_threshold.obj` reverses two static arrays, but another target/base
  game-core compiland emits the same pair in the opposite order. A global header
  reorder would fix one owner by breaking the other.

These are actionable findings in the sense that their causal boundary is now
known: they are placement/compiler-emission residuals, not unworked declaration
or definition order. They are not actionable *source edits* without a new lever
that also preserves the exact retail bodies, source lines, and ownership.

## Subsequent production applications

For TPI, group named records by `named_type_module_references` first. Test
object/archive/library order when a whole group follows one module; inspect PCH
or direct include order only when module provenance stays fixed.

For the next C13 correction, choose a first-party module with the same file set
and a small adjacent inversion. Compare checksum and line records with the
actual direct/PCH include graph, change one adjacency, rebuild the owning
library, then run the mandatory full build before accepting it. Keep roster
repairs separate from inversion changes, as the sound example demonstrates.

Use public/frame order as validation of a justified placement change. Never tune
function source from an address-map or FPO row alone: require the target's
definition, module-symbol, statement, and body evidence to agree, as they did
for `object_volumetric_sound`. Leave MSF page numbers until semantic content,
stream sizes, link mode, and history converge.

## Experiment record and completion criteria

For every production lever record the base commit, one changed variable, exact
link evidence, intended and adjacent topology deltas, `report-changes.json`, and
clean-relink reproduction. Classify the result as causal and usable, causal but
conflicting with stronger evidence, no effect, or still ambiguous.

This TODO is complete only when:

1. real DBI rotations are explained and corrected or parked with controlled
   negative evidence;
2. named TPI movement is grouped by retained compiland evidence and remaining
   first-party declaration/include rows have been worked;
3. first-party C13 differences have concrete direct-include, PCH, source-roster,
   or statement causes;
4. public/frame and MSF channels are used within the limits established above;
5. every accepted production change passes a full build and regression/structure
   review.
