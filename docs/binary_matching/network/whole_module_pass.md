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
