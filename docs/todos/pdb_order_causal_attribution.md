# PDB order causal attribution

Status: **open**. Small VS2008 experiments establish several causal rules, and
two bounded C13-guided corrections have now been accepted in the game. The
large DBI/TPI/public residuals remain open; the current MSF slot gap is fully
attributed to the unequal semantic module roster, while physical equality must
wait for that roster and the stream sizes to converge.

## Honest progress statement

The expanded PDB comparison originally found large, stable order differences.
Those inventories were findings, not fixes. The production matching yield before
this causal campaign was limited to:

- two moved `engine_world` definitions from the source-definition comparison;
- two reordered `mesh_type_enum` entries from the complete-enum comparison.

The work recorded here added a reproducible 29-case toy harness, corrected two
false comparison signals, established the cause of several PDB channels, and
applied the first rule to the sound library. That production change corrected
header presence/order in 39 measured sound compilands, brought two small
compilands to zero source-file-order inversions, and left the authoritative code
comparison exactly neutral. A second application recovered a missing direct
header dependency and the target callee in `particle/evaluate_type.cpp`, taking
its only function from 86.25% to byte-exact 100%. These changes did not correct
VFS DBI order, the global named TPI sequence, or physical MSF allocation.

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

The harness compiles only tiny VS2008 `/Z7` fixtures, links without the CRT, and
never builds game sources. It keeps source/output paths fixed and writes exact
commands, linker logs, PDB/EXE hashes, copied artifacts, full
`pdb_topology --order --json` reports, and compact channel summaries under
`binaries/gen/pdb-order-probes/`. That directory is generated and replaced by
the next run.

The current matrix contains 29 cases and emits 35 comparisons because clean and
incremental relink cases compare multiple states. Wine output goes directly to
each command's log file: VS2008's persistent `mspdbsrv.exe` can inherit a pipe
after `cl.exe` or `link.exe` has finished and otherwise make a completed probe
look hung.

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
| definitions of two `/Gy` functions reversed inside one `/GL` source file | 0 | 0 | 1 | final placement follows definition order in this fixture; legacy FPO has the same one inversion, while DBI, TPI, and C13 sequence order stay fixed |

These `/GL` controls invalidate a simple production recipe of “put archive
members in retail DBI order.” The LTCG integration phase can discard that
ordering evidence, while direct-input and separate-library boundaries remain
observable levers.

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
| the same PCH-owned header is redundantly included immediately before versus after `pch.h` | no semantic sequence moves | only the edited source checksum changes; include-guard placement across the PCH boundary is not an order lever here |
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

- all 56 candidate-only and all 299 retail-only identified stream roles are DBI
  modules; neither PDB has an unidentified stream;
- the net semantic-role difference is `299 - 56 = 243`, exactly the difference
  between 2,408 retail and 2,165 candidate present streams;
- 234 of the retail-only module streams come from `LIBCMT.lib`; another 49 are
  retail `libjpeg.lib`, `libpng.lib`, and `zlib.lib` modules whose candidate
  counterparts appear under the reconstructed `libgfx_libjpeg.lib`,
  `libgfx_libpng.lib`, and `zlibN.lib` ownership;
- only 10 shared stable roles retain the same stream index, and only PDB-info
  retains the same index, size, and page count. Even that fixed stream occupies
  a different physical page (`101496` candidate versus `61511` retail).

Therefore the current 243-slot gap is completely explained as a semantic module
roster gap. It is not evidence for 243 independently missing allocator actions.
MSF page/run comparison becomes a source/linker task only after module ownership,
CRT/vendor participation, stream sizes, and link history agree; before then it
is useful as a deterministic end-state check.

## Comparator defects exposed by the probes

The small cases found two false signals in the parser and one missing evidence
channel:

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

These corrections matter before production interpretation: otherwise the tool
can turn ordinary string-table relocation or function movement into a misleading
source-level difference.

## Whole-game baseline residual inventory

The complete pre-correction production snapshot remains in
[`pdb_comparison_audit.md`](../binary_matching/pdb_comparison_audit.md). It is a
scheduling inventory, not a count of actionable source edits:

| channel | current observation | likely evidence to collect next |
|---|---|---|
| DBI modules | 2,331 shared unique objects; 534,038 pair inversions (19.6655%); many are intact rotations | direct/link response order, archive member order, full `/VERBOSE` pass 1 and pass 2 |
| named complete TPI | 30,926 shared unique records; 80,461,493 pair inversions (16.8261%); a 1,715-record contiguous run survives | named-type direct-module references, DBI grouping, then PCH/header evidence |
| module source files | 1,679 of 2,331 paired scopes differ | module-local direct/PCH include graph and first differing adjacency |
| module checksums and lines | 1,679 checksum, 1,316 line, and 1,200 C13-subsection scopes differ | actual source roster, checksum content, statement/function ownership |
| public address map | 112,023,306 pair inversions (3.6975%) among 77,843 shared unique publics | extraction and final section/function placement |
| FPO and frame data | 1,859,789 and 37,230,579 pair inversions among 10,427 and 19,347 stable shared identities | downstream extraction, placement, size, and frame-shape validation |
| MSF allocation | 2,165 candidate stream slots versus 2,408 present retail slots | stream roster/size, clean/incremental mode, writer history |

GSI has only three pair inversions among 67,240 shared unique records and PSI
hash-record order has none among 77,843. That control remains important: large
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
function source directly for an address-map or FPO order row. Leave MSF page
numbers until semantic content, stream sizes, link mode, and history converge.

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
