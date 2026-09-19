<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Whole network/network_core structure pass

Status: in progress, not a module completion certificate. Baseline game build:
`a87cfce055af4209a71b715a19db09a8`; measured tooling checkpoint `a1dda61c4`.
The combined source batch was measured by successful full build
`857613fc9c48428392dffe4aa5bd02c8` (14m37s). This is a measured checkpoint,
not closure of the remaining queues.

## Scope and coverage

The inventory is all 94 files below `sources/vostok/network` and
`sources/vostok/network_core`, not just ledger rows or qualified namespace names.
It includes project/PCH/macro files, inline bodies, templates and global helpers.
Literal placeholder scan finds no `/* no source */`, `STATE[STUB]` or
`STATE[REMOVED]` bodies. Comments describing missing source evidence are not
themselves placeholder bodies.

The existing AST snapshot has 1002 facts from 25 translation units and facts
attributed to 61 files. The other 33 files must not be reported as AST-verified:
many contain out-of-class definitions that the declaration index does not retain,
while others are macro/PCH/project files or unmaterialized declarations. The PDB
procedure channel and explicit file review supplement, not replace, that gap.

The baseline procedure comparison covers 303 retail records under both source
paths, 294 paired. Findings overlap and include folded aliases:

| Channel | Match | Other findings |
|---|---:|---|
| Recorded locals | 293 | 1 mismatch |
| Procedure location | 279 | 15 mismatches |
| Projected statement files | 291 | 3 mismatches |
| Relative line geometry | 234 | 60 mismatches |
| Statement structure | 200 | 94 mismatches |
| Presence | — | 9 target-only, 7 base-only |
| Identity | — | 4 correlated representative mismatches |

The local discrepancy is `packet_reader::r(void*,u32,u32)`: candidate records
the destination parameter as well as size; retail records size only. Source has
no added named body local. The recorded parameter difference accompanies the
stack-versus-register argument boundary; deleting a parameter or introducing a
dummy local is not a supported correction.

## Class scope correction

The earlier namespace-only report omitted global helpers. Adding the six scoped
global helpers gives 66 class names: 54 identical, two record-multiplicity-only,
seven variant-overlap, two missing-base, and one different. No reported raw
declaration-order discrepancy. Do not collapse the unresolved variant sets.

| Global helper | Evidence |
|---|---|
| client_destroyer | Identical, 20B, 10 declarations; network/match_client.cpp |
| delayed_packets_predicate | Identical, 8B, 9 declarations; network_core |
| packets_predicate | Identical, 1B, 1 declaration; network_core |
| remove_all_predicate | Identical, 12B, 10 declarations; network_core |
| packets_in_list_predicate | Retail u8 and u16 variants; candidate matches u16 |
| sequence_id_predicate | Retail u8 and u16 variants; candidate matches u16 |

Raw access/virtual/static/member-order checks on the four identical global
helpers agree. The normalized header comparator chooses the first u8 variant
for the other two, producing a misleading direct source mismatch. Live callers
use word loads and support the u16 model; do not add duplicate global classes.
The missing u8 variants remain coverage debt, not proof of missing live bodies.

## UDP whole-model review

An independent Holista review found equal parsed local names/types/scopes in
all 53 uniquely paired network_core UDP-file functions. This is parser-level
evidence, not an independent proof of every raw type modifier.

- `process_low_level_message`: same twelve body spans, 283B, same message_type
  local, and same operations. Logging uses line538 instead of retail591.
  The measured patch maps observed statements with literal line directives:
  brace574, switch575, guard578, break580, state584, timestamp585, break586,
  guard590, log591, break592, guard595, assert596, disconnect598, closing618.
  No logic is fabricated to explain the unobserved source gaps.
- `fill_packet_header`: seven spans and two locals agree. Retail preserves
  unnamed pointer-reference slots for each serializer expansion; candidate
  propagates those references away. Two 12B differences remain. No evidence
  supports adding named locals or changing to a pointer-value helper.
- `clone_packet`: nine spans and two locals agree. Retail retains the const
  array accessor call; candidate inlines it. Do not change checksum-matched
  Boost or introduce an unsupported local to force a call.

## Reproduction and remaining work

### Combined candidate batch

The location candidate covers 56 procedures in 20 files (including inline
headers), listed individually in `geometry_batch_20260919.json`. Selection used
the existing Rust comparator's SQLite evidence: unique mangled-name pairing,
same source file, identical ordered instruction-span offsets and sizes, and a
different relative line map. Every base source row was checked against the
measured HEAD before producing a patch. Literal directives map observed rows;
baseline locations are initially restored after each function so unreviewed
neighbours are not silently remapped. A source-level order check then exposed
nine inversions caused by mixing retail and baseline locations. Fifteen adjacent
procedures received observed retail boundary anchors to preserve order:
network tcp_packet_client's destroy_client, destructor, create_client and
connect_impl; core http_client's constructor; core initialize/finalize;
core tcp_packet_client::send; udp_match_client's process_incoming_packet and
disconnect; udp_match_connection's enqueue_impl and packets_count; and the flow
emulator's constructor/destructor and delayed_packets_predicate::operator().
The final source-level check validates every mapped row in the 56 functions,
finds no newly introduced inversions among observable paired definitions, and
confirms no source-token changes except the iterator operand. These boundary
anchors do not claim to close unequal interior spans. This does not recover the
contents of source gaps.
The repeated logical locations may make source-text attribution ambiguous;
the evidence reader must omit ambiguous text rather than guess. Actual PDB
line partitions and byte effects are checked below against the combined build.

Holista's independent HTTP/async review covered 17 emitted TU procedures with
matching parsed locals. It identified one operand-level correction in
`async_connector::on_resolved`: retain `m_host = iterator`, then call
`connect(iterator)` instead of `connect(m_host)`. Retail offset0x5b8 takes the
incoming parameter's address (`lea edx,[ebp+10h]`); the measured candidate loaded
`this`, because m_host is the first member. Prediction: remove the final call
span's three excess bytes without adding a local or changing its 20 body spans.

The 15 procedure-location findings contain 13 unique subjects. The execute
aliases are confirmed folds. Eight generated constructor/deleting-destructor
subjects have only synthetic line0 metadata, not recoverable definition
locations. Boost bind/function and order/response channel representatives are
folded or generated. No source move is justified by these findings. Separate
size residuals remain (including client_destroyer's deleting destructor46/44B
and a Boost function destructor21/38B); location uncertainty does not erase them.

Of three statement-file findings, string_response::execute is folded. The
other two (`match_client_impl::send_queued_packets` and `packet<tcp>::reallocate`)
are in the same header on both sides: the target's single raw row disappears
from the body-only projection. Their raw line packing remains unresolved;
there is no evidence for moving or deleting their implementations.

All functions in the 94-finding structure-diff batch were inspected. The
ambiguous destroy_client selector was retried at retail RVAs0x56a4d0 and
0x74b6d0: each has one body span, with four excess candidate bytes in deletion.
Unequal-size callback expansion, allocator/accessor call boundaries, constructor
and singleton line-packing cases are not mechanically mapped or declared fixed.
The changed access/virtual/static/class declarations count in this batch is zero:
the duplicate-preserving audit does not justify changing them just to match a
normalized comparator's first variant.

Use `vostok-pdb compare pdb --file vostok/network` against target/base SQLite
for both modules. Use whole `topology --classes --json` output, then select by
namespace **and source-owned global class names**. `--class` takes an exact name.
The normalized `divergence --headers` view is useful for enums but is not a
replacement for duplicate-preserving raw class comparison.

## Measured checkpoint: 857613fc

The refreshed SQLite records reproduce all observed `(file,line)` rows and
ordered `(offset,size)` spans for all 56 selected functions. The iterator
correction in `async_connector::on_resolved` removes the predicted three bytes:
both procedures are now 1520B with 20 equal projected body spans. Its byte score
is 99.987274, not byte equality.

Using the unchanged checkpoint comparator, statement structure improves from
200 matches / 94 mismatches to 225 / 69; relative line geometry improves from
234 / 60 to 272 / 22. Other channels in the baseline table are unchanged.
These body-projection totals still contain the documented singleton and
empty-evidence limitations. Prepared comparator corrections are not yet part
of this measurement; their future counts must not be compared as if the
definition of each channel were unchanged.

`report-changes.json` reports ten byte-score improvements, zero regressions,
zero added/removed functions and zero fold churn for this build. This does not
resolve previously recorded regressions from other builds. Network_core has
no compiler warnings; network has 18 C4995 diagnostics in third-party
OpenSSL `x509_vfy.h:145`, none in its own sources. The full build retains
out-of-scope compiler warnings, 234 LNK4099 diagnostics, and two known
Scaleform UnexpectedEof extraction skips. Both module data gates have zero
open rows (the whole-project total remains 14).

An independent raw-line definition-order check covers 255 unique paired
definitions and 1215 comparable pairs, excluding equal-line ties. Four
inversions remain in unchanged headers:

- `packet_reader_inline.h`: retail `eof`, `advance`, and `size_to_eof` precede
  `r_string<16>`; candidate definitions put that template first.
- `udp_match_packet.h`: retail `header_size` precedes
  `helper::call_destructor`; candidate definitions reverse that relation.

The existing `divergence` source-order reader explicitly excludes header
procedures (`extract_sources`), explaining why its scoped clean report did not
catch these. This is a coverage gap, not evidence that header order is correct.
Before changing the nested helper, preserve the enclosing class's separately
observed method declaration order; do not conflate declaration order with body
definition order or infer missing source from the gaps alone.

This pass still must finish per-file source coverage and triage remaining
location/geometry/statement, presence, class-variant, and header-order findings.
Literal line mappings are reconstruction scaffolding: matching their records
does not prove recovery of original whitespace, guards, or unobserved code.

## Following comparator batch: c5cb89c1

The previously isolated `evidence.rs` and `rich_context.rs` corrections are now
integrated. All 113 Rust tests pass (107 library, six binary), including twelve
new regression tests. Whole-procedure size and raw boundary rows are separate
channels; missing body evidence is not counted as agreement. File runs retain
singleton header provenance without hiding line-packing differences.

Comparison against the unchanged 857613fc databases now reports whole size
216 MATCH / 78 MISMATCH; raw rows 188 MATCH / 87 MISMATCH / 19 UNOBSERVABLE;
body structure 168 / 69 / 57; and body geometry 215 / 22 / 57. The unchanged
69 and 22 mismatch totals are important: removing empty-evidence MATCH results
does not repair source. File runs give 271 / 4 / 19; the four remaining records
are the previously identified functor execute aliases (two), string execute
alias, and Boost bind list3 alias. The two singleton packing cases no longer
pretend to be different-file bodies. These counts use the new channel semantics
and are not directly interchangeable with the preceding baseline table.

Holista's source-reader review drove four conservative safeguards: reject text
attribution for backslash-continued files, track uncertain filenames separately
from line numbers, require an explicit filename after macro/conditional filename
changes, and recognize `#if(FLAG)` without whitespace. Conventional whole-file
guards permit lookup; nested feature conditions still do not. This modifies
tool interpretation, not engine guards or retail behavior.

Full build `c5cb89c18956434db77c9fae7e744792` succeeded in10m44s. The refreshed
report has zero regressions, improvements, additions, removals or fold churn;
the generated ledger is unchanged from a6aab4917. Network was recompiled and
has only18 third-party OpenSSL C4995 diagnostics. No network/network_core-owned
warning appears; network_core was not recompiled in this batch. The remaining
log contains13 out-of-scope compiler diagnostics, two LNK4049,234 LNK4099,
and the two previously recorded Scaleform UnexpectedEof skips. Both network
data gates remain OPEN=0; whole-project open data rows remain14.

The resolved `match_client_impl::on_packet_received` location TODO is removed,
preserving its physical line with a blank. Header-order implementation remains
queued separately: do not reorder the UDP packet's method declarations simply
to move its helper body. Both target class variants declare nested `helper`
before the accessors, while the observed helper body follows `header_size`.
A forward declaration with a later nested-class definition is a candidate to
test; moving method declarations would contradict that evidence.

## Header/helper batch (builds4777534b and274b78eb)

Baseline: measured commit6b04d7231. The chosen UDP helper reconstruction keeps
the nested class and its private static inline declarations in place and moves
only its method bodies after the enclosing class. This preserves the observed
nested declaration slot while restoring the helper body's later order. The
equally plausible forward-class alternative is not claimed as recovered source.
header_size is written on one line, matching its single8B retail line record;
its arithmetic is unchanged. Constructor/factory/disposer locations are mapped
without claiming their remaining unequal spans are repaired.

packet_reader_inline.h now places eof/pointer/advance/size_to_eof before the
string-reader definitions. One opening-line anchor20 then reproduces the
observed r<T>31..35, eof38..40, pointer43..45, advance48..54, size_to_eof57..59,
and string-template72..74 sequence using ordinary spacing. The non-template
string reader fits the intervening gap; its exact position and get_packet's
later position are inferred, not separately observed. Bodies and header member
declarations are unchanged.

The shared single-threading allocator policy receives ordinary inline on all
five static declarations/definitions. This is a whole-helper-family hypothesis,
not a caller-side implementation or forced-inlining workaround. Target retains
eight allocate specializations and none of the four small policy operations;
base retains all40. All eight allocate controls currently have equal spans
and serialized locals (DataSize44/300:221B;136/28/44200/8/16/108:214B). Preserve
those controls and audit all consumers outside network too. The original
inline spelling is unobserved; compiler context remains an alternative cause.

The new optional Rust attributed-order diagnostic reproduces four inversions
on the baseline across257 unique paired procedures and1253 comparable pairs;
eight tied pairs and46 procedures are explicitly excluded. Six regression
tests cover singleton headers, ties, folds outside scope, missing/ambiguous
evidence, invalid spans, and shifted lines. It does not replace syntax-order,
presence, or class-variant verification.

The full4777534b build succeeded in15m54s. All four observed header inversions
disappeared; the diagnostic reports37 MATCH,33 UNOBSERVABLE and23 AMBIGUOUS
coverage entries. This is not whole-module closure. The allocator policy now
retains exactly the same eight procedures as retail, with all eight sizes,
raw offset/size arrays and serialized locals preserved. No ledger max dropped;
50 rose. No network/network_core-owned warnings were emitted. Broad dependency
compilation emitted11521 compiler and237 linker warnings plus the two known
Scaleform extraction skips.

The final274b78eb build succeeded in8m28s after the Python audit-tool changes;
Ninja had no source work. No compiler/linker warnings or score changes; the two
Scaleform extraction skips persist. Rust119 tests and Python144 tests pass,
and ruff passes. Generated-ledger trailing empty TSV columns are preserved.

Regression audit retains the pre477 snapshot report-20260919-172140.json rather
than allowing the unchanged final build to erase history. Of143 initially
reported zero-score drops,75 are proven by the new conservative map-aware
instruction check. Raw public-symbol/PE inspection accounts for another64:
53 unique bodies with verified relocation/branch references, nine rows whose
multiple candidate addresses all have equivalent checked bodies, one exact
five-byte indirect vcall thunk, and AcquireInterface's scalar/array-delete
reference difference. For the latter, base array delete atRVA0x17c09f is
8bff558bec5de9d2daffff: it restores its frame and tail-jumps to scalar delete
atRVA0x179b7c. This is a verified CRT forwarding distinction, not literal
same-reference code. These manual classifications do not alter ledger scores.

Four pre-existing partial matches remain open: game_options and
stage_light_propagation_volumes deleting destructors (previous91.666664 each),
streaming_ready_texture::operator= (54.94737), and npc_statistics constructor
(48.366665). Their raw object scores became unscored, not newly nonexact from
100. The current bodies remain unequal; streaming/npc historical body identity
is not proven by the archived score alone. No source edits were made in those
units, and no hash-scoped max regressed. Preserve this limitation; do not report
global current-byte equality or module completion from this checkpoint.

report-changes.json now preserves unit and decorated symbol for each category.
The effective-map proof rejects multiple target/base addresses, missing bodies,
and differing nonempty instruction streams; raw-public/manual proofs above are
not silently generalized into that automated classifier.

## Five-file attribution batch after6d0fb1cde

Prepared patches integrated together: login_client.cpp (80 observed raw line
records across20 procedures), async_connector.cpp (on_resolved, public connect
and reset), HTTP handle_read_status_line, connect_order and enqueue_order
destructor/execute locations. Source comparisons confirm unchanged C++ statements,
types and scopes; only literal line directives and stale order-header matching
comments changed. No assertions, guards or helper bodies were fabricated.

Build78bbcf703a8a4a2c9bd128fdc2bb2dd6 succeeded in10m44s. No network or
network_core-owned warnings;31 compiler warnings outside those modules and236
linker warnings, plus the two known Scaleform PDB parser skips. Report changes:
zero regressions, three improvements, no added/removed functions or fold churn.
Both module data gates remain OPEN=0.

All28 selected procedures now have equal target/base raw source-line maps.
All43 paired procedures in the five files have equal serialized locals.
The three unselected location residuals are async_connector's constructor and
HTTP's constructor/close_connection. on_resolved reaches100% in the byte report,
with equal1520B extent, all raw offset/size/line tuples equal, and20 matching
body statements. Other remaining size differences are not closed by attribution.
Independent assembly checks confirm the eight affected logging line operands:
async on_resolved64/65/83/84/91, public connect110 and HTTP status132/138.
Verbosity literals also agree (INFO4, ERROR2). Attributed-order audit remains
37MATCH/33UNOBSERVABLE/23AMBIGUOUS with no observed inversions. Physical source
definition order is unchanged by this directive-only batch. The only diff-check
warning is the generated ledger's empty final TSV field; it was not hand-edited.
HTTP's two string-find wrapper expansions are expected to remain; callback
assignment/construction expansions in login/async and intrusive-pointer helper
expansions in the two destructors remain open. Line attribution is not evidence
of exact original whitespace and does not close those byte discrepancies.

## UDP and login implementation batch after db4562e45

Integrated three prepared files; all non-directive C++ is unchanged. UDP connection
maps170 observed records across constructor, destructor, on_error,
fill_packet_header, enqueue_impl, new_low_level_packet, send, send_packets_list
and send_queued_packets. Function-scoped lookup resolves repeated logical lines
caused by backward directives; already-correct neighboring functions are restored
to their baseline attribution. No named locals or artificial scopes were added.

Login implementation maps destructor callback closing lines66/81, certificate
verifier opening94/return109/closing110, and connect opening51/state67/callback
closing80/closing81. Twelve ledger attempts are recorded, preserving prior notes.

The full build checks raw line maps, statement sizes, locals,
observable definition order and global regressions. The seven
UDP size-residual procedures remain constructor, fill_packet_header, enqueue_impl,
new_low_level_packet, send, send_packets_list and send_queued_packets. Serializer
reference-slot propagation and accessor/scalar append retention are unresolved;
the candidate does not pretend line directives implement those missing codegen
properties. Login callback construction size residuals also remain open.

Build b95c6bdbb1dd440bac31d7a2530b4819 succeeded in10m18s. Zero warnings
owned by network/network_core;31 compiler and236 linker warnings elsewhere,
plus two known Scaleform parser skips. Both module data gates OPEN=0.
Zero reported regressions, no added/removed/fold-churn entries; login connect
improves97.96135 to97.971016. All26 paired procedures across the three files
(UDP19, login implementation5, login connect2) now have equal raw source-line
maps and serialized locals, including neighboring functions not changed in the
batch. Byte residuals above remain open; no semantic closure is claimed.
Attributed-order coverage remains37MATCH/33UNOBSERVABLE/23AMBIGUOUS, with no
observed inversions. Source diff-check passes; no physical definitions moved.
