<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Network / network-core structural closure

This campaign covers both modules. A module is not complete merely because
the byte score is high or a declaration gate passes. Every observable source
defect must be corrected and measured; unresolved compiler, type-provenance,
or extraction cases stay explicit, with a next investigation step. An
unobservable fact is not a proved match.

The per-finding record is [structure_campaign.tsv](structure_campaign.tsv).
Its initial observations are an immutable baseline; the disposition column
tracks subsequent work. `aligned_match_only` means only the aligned statement
projection passed, not that semantics, bytes, or every other channel passed.
`needs_assembly_review` is not a declaration of an LTCG wall.

## Measured baseline

- Commit: `c8ac5dd58f642243c8d14719d6643ce2347d9fa7`.
- Full build: `55cb785a6f3e4f7ca5fe28936f0cab35`, successful in 14m22s.
- Target PDB SHA-256: `0ffe85c27f8b95f23a65d91866af3384ab24ca343b3865a57f71a08902d5a238`.
- Base PDB SHA-256: `968ca3b0955c58d204233352f1aa02e8781185d28346b9091a79b836e85017cf`.
- Procedure scope: normalized source-file substring `vostok/network`, covering
  both `network/` and `network_core/`. This deliberately includes folded and
  third-party procedures attributed to these files; ownership needs review.
- Class scope: fully qualified class names starting with `vostok::network`.
  Vendor template names merely containing this namespace are not included.

The earlier 56 statement/geometry identities and 19 presence findings were
the `network_core` subset, not the whole two-module campaign. Across both
modules, raw comparison reports 102 statement-structure findings, 59 geometry
findings, 27 presence findings (17 target-only / 10 base-only), two local
findings, six location findings, three statement-file findings, and six
ambiguous identities. The raw statement/geometry union contains 111 selector
strings. The free name `destroy_client` identifies two procedures; resolving
both by retail RVA yields 112 procedures for aligned review.

The fresh complete-class audit has 13 differing owned classes, not the older
ten-class snapshot. Three additional method-signature discrepancies were
introduced by qualifying the header value parameters in the preceding commit.
Retail class method records keep non-const parameters for both packet template
instantiations' `append` and for `packet_reader::r`, while procedure locals
record const. Restore the two header declarations without changing the const
definitions. This distinction is invisible to the narrower qualifier gate and
must be verified through both class topology and emitted procedure locals.

| Aligned inspection result | Procedures | Next step |
|---|---:|---|
| Projected structure match | 25 | Keep other evidence channels independent |
| Statement-size residual only | 78 | Inspect assembly/callees; identify concrete cause |
| One-sided statement rows | 9 | Verify operation, order and file/line attribution before editing |

This distinction already prevents a false fix:
`get_connection_info_from_string` has unequal raw body-row counts but an
8/8-statement, 123/123-byte aligned match. Raw blank/non-addressed rows must
not be treated as missing runtime statements. Both `destroy_client` procedures
have one aligned statement each with a four-byte size residual, not ambiguous
source identity after their RVAs are selected.

## First measured bundle: packet declarations and message-info packing

Retail `udp_match_message_type_info` has CodeView properties `0x203`; base
has `0x202`. The sole differing bit is `packed` (bit zero, decoded by the
in-tree tool's `pdb2::TypeProperties::packed`). Both records have size one,
the same constructor and the same 6/1/1-bit channel/reliable/ordered fields.
Restore a local `#pragma pack(push, 1)` / `pop` around this struct only. The
neighboring polymorphic orderer must not inherit the packing scope. The
directives replace blank lines, preserving existing source-line geometry.

Target `udp_match_connection::construct_packet` at RVA `0xdac90` confirms
the consumer writes channel, reliable, then ordered, in that order. Its
one-sided aligned bitfield row does not justify swapping these assignments.
Full build `4d318f726b6448dd807c08ed98ef769b` succeeded in 14m04s. Complete
class comparison now matches both packet template declarations, packet_reader,
and the packed message-info struct: four cases closed, nine owned class cases
remain. Procedure locals still retain const `buffer_size` and `size`.
The consumer's 8/8 aligned statement count and remaining byte/attribution
differences are unchanged; its baseline finding remains open.

The build reports zero byte-score changes or source-maximum losses, with
`OPEN=0` for both network data gates. The refreshed source gate remains
734 MATCH / 94 AMBIGUOUS / 253 UNOBSERVABLE. Compiler/linker warning messages
match the preceding build, including four C4701 and two C4715 diagnostics in
other modules; Wine EGL warnings and both known Scaleform `UnexpectedEof`
skips remain. The generated README is byte-identical to its parent.
The measured base PDB SHA-256 is
`adef9b524139a53b949783560f440183b6efa2d1cc393723854734182bdfeafa`.

## Connector unit in progress

The next measured unit is `sources/async_connector.cpp`, together with its
unexpanded private helper definitions in `async_connector_inline.h`.
Two source corrections are prepared:

- Public `connect` at RVA `0x5458a0`: use `query(tcp::v4(), host, port)`.
  Retail writes protocol family 2 to a temporary and passes its address at
  instruction offsets `0x1e3`/`0x200`; base previously omitted this argument
  and selected the `PF_UNSPEC` overload. The query's flags remain `0x400`.
  Apply the same IPv4 behavior to the source-prior `resolve` helper for
  consistency; its body is not independently observable in retail.
- Iterator `connect` at RVA `0x5451f0`: use direct `_1`/`_2` bind placeholders.
  Retail loads their bytes directly; base previously loaded Asio reference
  placeholders first. Both resolver callbacks retain their observed Asio
  placeholders. This is a per-call evidence decision, not a global convention.

All six emitted CPP procedures were inspected before editing. `reset` has a
1/1-statement, 24/24-byte baseline match. The constructor's zero addressed
statements do not prove equal bytes (128/134). `on_connected` is 10/10 but
retains guard attribution and LOG-line differences. `on_resolved` is 20/20
with a three-byte residual at its final `connect(m_host)` call. Those findings
remain open. The five-line source-status comment that misidentified the query
as an inline-boundary issue is removed.

Full build `249098eeae3c42ea8cd732a28c150a05` passed in 11m31s.
Iterator-connect is now 100 (from 91.8868), with a 2/2-statement, 178/178-byte
match. Public connect improves from 90.6215 to 93.4689: all 11 addressed
statements now align, including the IPv4 query. The remaining aligned size
residual is +38 bytes at `m_on_error = on_error` (base expands temporary
copy/swap/clear while retail calls assignment). Frame/register choices and
the LOG source-line immediate also differ; this function remains open.
Class topology matches and the three recorded public-connect locals are
retained. The strict source gate remains 734/94/253 and both network data
gates have `OPEN=0`.

Global objdiff reports eight regressions in untouched Scaleform functions,
19 improvements, and four separately classified fold-churn entries. No banked
source maximum falls. Keep the eight raw findings visible; absence of a
source-max loss is not proof that every external score difference is harmless.
Existing C4701/C4715 and missing-CRT-PDB warnings remain, alongside the two
Scaleform extraction skips. The generated README and ledger retain this build's
results; there is no claim of a regression-free global report.

## HTTP unit in progress

The owned unit is `sources/vostok/network_core/sources/http_client.cpp`, including the
free `read_lines_from_stream` helper. Its eleven emitted procedures were
checked before edits; no header/library rewrite is part of this unit.

`handle_read_status_line` at RVA `0x77a3e0` records `s32 found`, so remove the
incorrect suggestion to replace it with `size_type`. At offsets `0x19d` and
`0x271`, retail independently destroys the status string and response stream
then jumps to the epilogue. These are the two explicit returns after error
logging, not nested else-join branches. Restore the guards while keeping the
three locals inside the outer successful-read block. The find-overload
call/inline residual remains separate; do not label the entire function a
library-version wall.

`handle_connect` at RVA `0x77a820`, offset `0x1c5`, calls the socket member
`async_connect(endpoint, handler)` on its retry path. The reconstructed free
`boost::asio::async_connect(socket, &endpoint, handler)` instead treats a local
endpoint pointer as an iterator and retains different retry/lifetime behavior.
Restore the member call, preserving the observed Asio error placeholder and
incremented resolver iterator stored in the callback.

Remove the obsolete prefix/assert narrative: the current
`VOSTOK_UNREFERENCED_PARAMETERS(prefix)` already emits the target's guarded
single-argument helper call, and the remaining disassembly name differences
are folded aliases. No shared assertion macro is changed. The two stale
source-status paragraphs are deleted rather than retained as layout padding.
Full build `8a2e9e8411bd4bf891ae5f9344b955ee` passed in 10m42s.
`handle_connect` improves from 97.9796 to 100, with all eight statements and
549 bytes matching. `handle_read_status_line` improves from a banked 90.8722
(previous current 90.4977) to 94.3516. All 16 statements now align, including
both independent 24-byte string/stream cleanup-and-return paths. The three
named locals and their types match. Remaining sizes are +13 at each `find`,
-4 at the first LOG, +1 at `read_lines_from_stream`, and -1 at async-read.
The target retains a two-argument string-find call while base expands length
and calls the three-argument overload. LOG line immediates are still 126/133
versus retail 132/138; these and register choices remain open, not a claim of
complete HTTP closure. Removing the stale helper preamble also restores
`on_error`'s retail LOG line 58 and raises that function to 100.

The other nine procedures retain their projected structure matches (the
constructor has no addressed statements). HTTP class topology is identical;
the strict source gate remains 734 MATCH / 94 AMBIGUOUS / 253 UNOBSERVABLE.
Both network data gates report `OPEN=0`.

The uncapped global report has 140 regressions, all to zero, 212 improvements,
and 115 separately classified fold-churn entries; no banked source maximum
falls. These raw regressions are retained for attribution review, not declared
harmless. For example, the report's network-world deleting-destructor drop
does not mean its body disappeared: direct PDB inspection still pairs the
44-byte scalar deleting destructor and the ledger records 100. That sample
does not adjudicate all other aliases. Existing C4701/C4715, missing-CRT-PDB
and other compiler warnings remain, along with the two Scaleform extraction
skips. No global regression-free claim is made.

## TCP packet socket unit in progress

The next owned unit is `tcp_packet_socket_inline.h`, with its eight emitted
TCP specializations. Send at retail RVA `0x123ee0`, offset `0x9e`, reads the
const-buffer reference and then its size field at offset 4. It passes that
wire-buffer size to `on_packet_has_been_sent`, not the cloned packet's payload
size and not the return value of `write`. Restore
`boost::asio::buffer_size(buffer)`: the local Boost overload exposes exactly
that size field. Keep the synchronous `write(..., transfer_all(), error_code)`
and the three PDB-recorded locals. The target's 27-line gap before write is
not evidence for inventing asynchronous send code or extra statements.

Both sides currently have six send statements, but target's last two spans
are 45/32 bytes versus base 45/28; the line-weighted alignment mispairs them.
Verification must check the final argument's instructions as well as the
post-build aligned verdict. The completion callback deletes the packet first
and tests error/nonzero length, so this correction is not a claim that every
nonempty send had observably different behavior.

The receive callback remains unchanged: retail offset `0x1be` explicitly
calls `tcp_packet::allocated_size`, matching the existing comparison.
`on_packet_received` and `new_packet` have projected structure matches.
The size-reader specializations retain +42/+38-byte async-read residuals;
start-receiving retains an extra 18-byte handler-copy sequence, and
stop-receiving retains nine bytes from different stack-slot encodings.
The send callback retains a three-byte LOG-size residual. These remain open;
no shared Boost helper is rewritten to steer this caller.

Full build `8453e1826d534d8d8ef48ae9055aef02` passed in 10m24s.
Send improves from 95.2833 to 99.9. All six ordinal spans now have the same
offsets and sizes, and both bodies are 197 bytes. The completion argument
loads the buffer's size at +4 exactly as retail does. Three named locals
and their types remain identical. Buffer/error-code stack slots are still
exchanged, and the source-line gap still causes the aligned comparator to
report the write span as BASE_ONLY/TRGT_ONLY. This is not a structure-closure
claim despite the ledger's approximate MATCH class. No padding or invented
intervening statements were added. The seven other emitted header procedures
retain their previous structure verdicts.

No banked source maximum falls. The uncapped global report has three raw
regressions to zero in untouched Scaleform Vector_int AS3join/fixedGet/AS3some,
three improvements, and two separately classified fold-churn entries.
Those raw attribution findings remain visible. Both network data gates
report `OPEN=0`. Existing unused-variable/parameter, truncation, C4701/C4715,
unreachable-code, deprecated OpenSSL free, CRT-PDB and linker warnings remain;
there is no warning on the changed send expression. The same two Scaleform
UnexpectedEof extraction skips remain.

## Entry-point unit in progress

`get_ip_address` at RVA `0x577040` has one missing two-byte statement at
offset `0x216`, retail line 148, immediately after the return on line 147.
It jumps to loop exit `0x21d`, not the backedge `0x218`, and is unreachable
because the return's cleanup already jumps to the epilogue. Test the ordinary
C++ `break;` after return. This is stronger evidence than the old generic
closing-brace-breakpoint explanation; the review question stays open until
full-build verification. Preserve the five locals, host/service query and
IPv4/non-loopback filtering. No padding, assembly, or flag change is involved.

The complete owned unit is `network_core_entry_point.cpp`; memory_allocator,
get_connection_info_from_string, initialize and finalize retain their
existing bodies. The two nonempty sibling procedures have projected structure
matches. Do not alter forced header includes or type emission as part of this
function correction. Baseline get_ip_address: 10/11 statements, 674/676 bytes,
99.0116 current and banked maximum.

Build `bb8b533d29bb49c2ac2e6b406c16891b` passed in 10m44s and verifies the
unreachable break: 100%, STRUCTURE MATCH, 11 statements and 676 bytes on
both sides. Its two-byte jump at offset `0x216` reaches the loop exit exactly.
All five named locals match, and the four sibling procedures retain their
projected structure matches. The source gate remains 734/94/253. The global
report has zero regressions, one improvement, zero fold-churn and no banked
maximum falls. Existing compiler/linker warnings and two Scaleform extraction
skips remain; there is no new warning on this break. Both network data gates
report `OPEN=0`.

The source/review TODO is cleared together, retaining a lean explanation for
the dead break. Final cleanup build `bd28186b79c34f1aa5d20331c7c04a1a` passed
in 10m41s: the function remains 100% with all statements and locals matching.
The global report is unchanged (zero regressions, improvements or fold-churn),
and no banked maximum falls. Warning categories and both extraction skips
are unchanged. This disproves the previous claim that the missing statement was
a non-steerable compiler/ICF wall; it does not establish that other dead jumps
have this cause without checking their destinations and statement evidence.

## Remaining class evidence after the entry-point build

The fresh full topology comparison from build `bd28186b79c34f1aa5d20331c7c04a1a`
still reports nine owned class cases: two record-multiplicity, five variant
overlaps, one missing-base complete type, and one different class. None is
silently closed by choosing one retail record.

Three cases share a concrete layout dependency. Retail connection records
`0x19026` and `0x7152e` use four byte-sized versus four word-sized sequence
numbers respectively. The latter agrees with base and the emitted
`process_incoming_packet` at RVA `0x121980`, which reads words and uses remote
sequence offset `0x126`. Shrinking the shared class would contradict that body.

| Field/layout | Byte-sequence retail variant | Word-sequence retail/base variant |
| --- | --- | --- |
| connection size | `0x530` | `0x538` |
| connection handler allocator | `0x128` | `0x130` |
| session endpoint | `0x538` | `0x540` |
| session set hook | `0x554` (1364) | `0x55c` (1372) |
| session destroy-list link | `0x564` (1380) | `0x56c` (1388) |
| session size | `0x568` | `0x570` |

The server's four nested/field-type differences embed exactly those two hook
offsets. They are consequences of the session variant, not four incorrect
visibility declarations. The target procedure inventory has no
`udp_match_server` entry; its existing server-target review TODO remains open.
Do not fabricate a second same-named class or change all client layouts to
make these type-only records disappear.

Likewise, emitted `match_client_impl` constructor RVA `0x766240` uses allocator
offset `0x258000`, client offset `0x258038`, and state offset `0x258ba0`.
These support the current 8192-packet / larger-client variant, not the unmatched
2048-packet complete type. This does not prove every alternative type record
was unused; broader compiland/consumer provenance remains open. Missing a
complete TPI record is not permission to add a forced runtime instance.

## Reopened parked-function audit (2026-09-14)

The 55 parked `network_core` procedures are a re-audit queue, not proven
compiler limitations. Inspection against build `bd28186b79c34f1aa5d20331c7c04a1a`
found 17 projected statement matches, 28 aligned size residuals, four
statement-presence/alignment findings, and six procedures without a paired
base. Three of the projected matches contain no addressed statements; five
have different total procedure sizes. Neither a park nor a projected match
establishes complete source equivalence.

The [per-function re-audit record](../network_core/parked_reaudit.tsv) preserves
all 55 selectors, baseline scores, observations, next checks, and old notes.
The old notes are historical inputs to challenge, not endorsed explanations.

The first confirmed counterexample is `tcp_packet_client::close_connection`
(retail RVA `0x77cb10`). Its five statements total 121 bytes against the
previous base's 150 bytes. The extra 29 bytes occur at `m_socket.close(ec)`.
Retail calls the **no-argument** socket overload at RVA `0xc2970`, which owns
an error-code local and invokes `do_throw_error` on failure. The reconstruction
instead used the nonthrowing error-code overload. The prior claim that this was
merely an inline boundary was incorrect. The source now calls `close()` while
retaining the separate `ec` local required by `shutdown`. Full build
`d19d3d685ec848718a2e4e4ae84141a9` (10m40s) raises the score from 66.303 to
100: all five spans, the 121-byte procedure, and the single named `ec` local
match. The remaining disassembly labels are folded callee aliases, not different
operations. All other TCP-client TU scores are unchanged; eight of its nine
procedures have matching projected structure, with only the constructor's size
finding remaining. Global report: one improvement, zero regressions, additions,
removals or fold churn; no banked source maximum falls.

The successful build is not warning-free: it reports unused variables/parameters,
conversions, four C4701 and two C4715 diagnostics outside this TU, unreachable
code, assignment conditions, two LNK4049 and 234 missing-CRT-PDB LNK4099 warnings.
The two known Scaleform `UnexpectedEof` extraction skips and Wine EGL diagnostics
remain. Both network data gates report `OPEN=0`.

The constructor in the same TU also needs renewed investigation: its sole
addressed callback statement is 94 bytes in retail versus 121 in base, while
the prologue contains additional differences, including an extra base store to
`m_first_packet`. An omitted optimized store alone does not prove an absent
source initializer. This remains a hypothesis to resolve, not permission to
delete the field or a claim that the constructor is byte-correct.

Other stale notes must be checked against current instructions before reuse.
For example, `udp_match_client::handle_receive` currently has 23 aligned spans
and 950 bytes on both sides; its visible LOG line constants differ by 33 lines.
The old note's callback-cleanup scheduling description does not describe the
current diff. `update_acknowledgements` still has genuine subtraction and
maximum-update size differences; these need callee/type/source checks rather
than a blanket compiler explanation.

### Sequence-distance reconstruction measured

Retail `operator-<u16>` (RVA `0x127b20`) has three statements, 77 bytes and no
named locals. Its first return adds 65,536, subtracts the right operand and
computes a signed remainder modulo 65,536. The previous `s16(left-right)`
substitute emitted only 17 bytes against the target's 35-byte return span.
It also changed half-window behavior: `(32768,0)` produced +32768 instead of
retail's -32768; `(0,32768)` produced -32768 instead of +32768. A comparison
over all 65,536 left operands against zero isolates the first disagreement.

The reconstructed expression uses the existing `max_sequence_number` enum and
retains the `right <= left` guard and recursive negated mirror. No class field,
access, signature or helper placement changes. Full build
`c7afede9203b4c81a40e5a5255c0ce9e` succeeds in 14m32s. Subtraction rises from
81.3333 to 100: three matching statement spans, 77 bytes, zero locals, and
30/30 normalized instructions. The two comparison procedures remain exact.

Without editing its source, `update_acknowledgements` rises from 75.7738 to
100: all 33 spans, 1,060 bytes and eight named locals (including their const
qualification) match. The correct subtraction body restores three helper calls
and the original frame size. The apparent `math::max` discrepancy was seven
expanded frame displacements, three bytes each; it disappears too. No shared
math helper needed adjustment. This disproves both old parked explanations.

The global raw report records 37 improvements, 12 regressions (all 100-to-zero
entries outside network), 12 fold-churn entries and no additions/removals.
The derived ledger has no current-score or banked-maximum decrease; several
generated aliases receive different source hashes while retaining 100. These
are separate evidence channels, not a claim that the raw report is empty.
Header-dependent recompilation emits more warnings than the prior incremental
build: unused parameters, class/struct spelling, conversions, deprecations,
enum-case and related diagnostics. The same four C4701, two C4715, two LNK4049,
234 LNK4099 and two Scaleform extraction skips remain. Both network data gates
retain `OPEN=0`.

### Flow-emulator source-shape follow-up

The destructor at retail RVA `0x728990` materializes the vector's `empty()`
result with `cmp/sete/movzx/test`. The direct `begin()!=end()` reconstruction
instead creates iterator temporaries and omits that boolean-return boundary.
The loop now calls `!m_delayed_packets.empty()` without changing ownership or
the four-statement, zero-local shape.

In `add_packet` (RVA `0x728cf0`), retail copies both ping bounds into inline
argument slots before subtracting them, calls the random generator, then adds
the preserved minimum. Its four named locals include no ping-bound temporaries.
This supports the existing two-argument `random(min,max)` overload, not the
open-coded `operator()(max-min)+min` expression. The caller now uses that
overload; the math header is unchanged. The packet-pressure sum also follows
retail's observed `unacknowledged_packets_count + delayed_size` operand order.
Build `d3b87dd09ab04a0fbc4009a81c582c99` succeeds in 11m21s. The destructor
improves from 79.0732 to 99.7073 with all four statement spans (111 bytes) and
zero locals matching. `add_packet` improves from 94.4057 to 99.7264 with all ten
spans (356 bytes) and four locals matching. The helper-call reconstruction is
confirmed, but neither function is closed: destructor frame size is 0x70 versus
retail 0x78; `add_packet` is 0xb4 versus 0xb8, with shifted temporary slots and
the two u16 locals allocated oppositely. Do not add unused locals or reorder
declarations just to move stack slots. The other five procedure scores in the TU
are unchanged, including the constructor's zero-statement projection and
`tick`'s residual.

Global report: 191 improvements, 129 raw regressions (all to zero), 88 fold-churn
entries and no additions/removals. The ledger loses current attribution on 17
previously scored records, but no banked maximum falls. These losses remain
recorded rather than being called zero regressions. Warning counts match the
earlier incremental TCP build: four C4701 and two C4715 outside this TU,
unused/conversion/unreachable/assignment warnings, two LNK4049 and 234 LNK4099,
plus the two known Scaleform extraction skips. Both network data gates are open-free.

### Packet variant selected by emitted consumers

The class comparator reported `udp_match_packet` as variant-overlap: the old
source matched one complete type record, but not the variant used by the live
procedures. The two target records both have size 0x12c and identical offsets
and access. Type index 26566 has an eight-bit `sequence_id_type`/`sequence_id`
and `packet_header_size=4`; index 464170 has sixteen-bit sequences and
`packet_header_size=6`. Those three declaration differences must stay together.

Retail constructor RVA `0xda900` stores a word at offset 0x24 (+0x51), not a byte.
`fill_packet_header` RVA `0x545bc0` reads that word (+0x35) and advances by two;
`send_packets_list` RVA `0x547180` copies a word from packet offset 0x24 (+0x138).
The send queue's packet copies and by-value predicate argument also load words.
The current connection already uses sixteen-bit sequence state and serializes
three words into its six-byte transport header. These emitted uses identify the
second complete variant; merely accepting an overlapping class record missed
the incompatible field in the constructor.

The source now selects that variant consistently (alias, field and enum), and
the connection/predicate code directly accesses the correctly typed field.
Eight lines of reinterpretation workarounds are removed. No object size, field
offset, access, virtual status, declaration order or source-file ownership changes.
The already-resolved acknowledgement inline-wall comment is also removed; its
source-line effects will be included in the full build. Neither raw target type
variant is deleted from the evidence or forced into an artificial runtime object.

This is a header/consumer cluster, not a shared-helper inlining tweak. Recheck
the packet constructor, header writer, both send paths, predicate expansions,
acknowledgements, strict source/class gates and global measurements before closure.

The fresh pre-build source index parses all 25 scoped translation units and
records 1,000 facts. The strict gate passes with 734 `MATCH`, 94 `AMBIGUOUS`,
253 `UNOBSERVABLE` and zero deterministic failures. This unchanged gate count
does not erase the runtime difference between variants; consumer provenance
supplies the selection evidence that the declaration gate alone cannot.

Build `1af7e46ab1ab4b23b107a840680b3021` succeeds in 14m58s. The constructor
improves from 81.4068 to 84.9831 and now initializes both sequence fields as
words. Its two addressed spans match, but total size is still 179/191 bytes:
the base packet constructor is inlined in base and called in retail. This
zero-row initializer difference is not covered by the projected MATCH verdict.
The three connection consumers retain 71.0556, 95.811 and 97.5688 respectively,
with 7, 42 and 71 aligned spans and the previously observed SIZE residuals.
Acknowledgements retain 100 and all 33 spans/1,060 bytes. The two expanded
predicates retain the retail word loads, including remove_if offset 0x7d and
all seven find_if loads; their total sizes remain 279 and 376 bytes.

Rebuilt packet type index 32633 matches the complete sixteen-bit retail
variant, including all 29 declarations and size 300. The class verdict remains
variant-overlap because the other retail variant still exists; it is not erased.
The pre-build strict source gate applies to the unchanged measured source.

Global raw report: 246 improvements, 129 regressions (all to zero), 80 fold-churn
entries, no additions/removals. No banked maximum or numeric current score in
the derived ledger decreases. Thirty-two current attributions become absent:
15 Boost, seven render, five GFx, two game_core, two Bullet and one Opcode;
none belong to the network/network_core module roster. These measurement
losses are retained as unresolved attribution evidence, not silently discarded.

Both network modules recompiled. Packet truncation warnings C4305/C4309 are
gone. Remaining owned warnings are new_size, registered_packets_count,
allocated_count, success, and network's previous_state; OpenSSL contributes
included-header deprecations. Four C4701 and two C4715 remain outside these
modules, as do two LNK4049, 234 LNK4099 and two Scaleform extraction skips.
Source/docs pass git diff --check; generated TSV empty final fields naturally
trigger its trailing-tab diagnostic and are not hand-normalized.

### Unused-variable and parameter audit (in progress)

The singular `VOSTOK_UNREFERENCED_PARAMETER(x)` is `(void)(&x)`, unlike the
plural guarded helper. An unaddressed source line can contain the singular
macro, but blank space alone does not prove its spelling. Suppression must
preserve target-backed locals and statement positions, not hide a mismatch.

| Candidate | Target evidence | Current action |
| --- | --- | --- |
| `registered_packets_count`, `allocated_count` in `check_consistency` | Both const u32 locals are recorded. Declaration lines 194/195 and assertion line 196 are consecutive; the assertion emits only the 12-byte identity call, not an argument eater. | Retain declarations and plain assertion. No inter-statement gap supports inserting separate macros there. Check terminal/source boundary evidence before placing zero-byte statements after the assertion. |
| `success` in `handle_send` | Const bool local is recorded. Erase-result declaration line 101, plain assertion line 102, next guard line 104. | A zero-byte suppression could occupy line 103. Candidate remains untested; singular only, not ASSERT_U or a plural eater. |
| `new_size` in `udp_match_packet::reallocate` | Private const-u32 method declaration exists, but no standalone target procedure/line table is found. Existing grow-path recovery remains ambiguous. | A macro would silence the warning, but no recovered source-line gap establishes placement. Keep body-recovery question open; do not report source-exactness. |
| `previous_state` in `match_client_impl::on_packet_received` | Retail store at line 45, unaddressed line 46, guard at 47. No target named local; base has a const state local and a 3-byte-short first span. | Testing singular macro in the existing line-46 gap. It neither adds a physical line nor changes the guard/LOG positions. Previous failed ASSERT_U experiment is not evidence against this variant. |

The first measured test owns `match_client_impl.cpp` only, all eight procedures.
Baseline scores: ctor 92.6739, dtor 93.3333, clone_packet 93.6842, connect
65.3333, on_packet_received 95.8333; disconnect, on_disconnect and
set_on_packet_received are 100. The handler has 11 addressed spans and
383/389 total bytes, with first-span -3B and callback guard +9B. Check warning
removal independently of whether the local-record mismatch changes; a clean
warning log is not proof of a recovered original statement.

Build `3b1d81f8504c4f7396b84063822d92a1` succeeds in 10m39s. The previous_state
C4189 warning disappears; the raw global report has zero improvements,
regressions, additions, removals or fold changes. All eight TU scores remain
at their baseline. The handler still records one base local versus zero target
locals, 11 addressed spans and 383/389 bytes, with the same -3B/+9B residuals.
The singular macro is confirmed warning-suppressing and byte-neutral, not a
recovery of the local projection or proof of the original macro spelling.
The open review question remains open. This build recompiles network only;
it still emits new_size twice through the included packet header, 18 OpenSSL
deprecations, four external C4701, two C4715, five C4702, two C4706, two LNK4049,
234 LNK4099 and two Scaleform extraction skips. It cannot remeasure the
network_core source warnings that were not recompiled.

The user requests a committed checkpoint followed by one combined
network/network_core source pass and one full build/audit. Subsequent work
therefore batches independent changes across the module pair, retaining
per-function evidence and whole-scope regression controls. Parked status is
not a completion criterion. Isolate variants only when measured failure or
regression requires diagnosis, not by default for every small source change.

### Combined module pass: baseline f333b5785 (measurement pending)

`oneshot_audit.tsv` snapshots all 286 roster entries (159 network, 127
network_core) before the combined build. It is a dated audit, not a replacement
for the live ledger. The PDB comparator produces 1,441 raw findings: 99
statement-structure, 60 relative-line-geometry, two locals, seven location and
two statement-file mismatches, plus 13 target-only, ten base-only and eight
ambiguous identity records. Counts are evidence-channel findings, not unique
unfinished functions. The two-sided addressed diff was run for all 99 subjects
flagged by structure/locals; the ambiguous destroy_client selector requires
its known RVA. Many raw row mismatches collapse to aligned addressed spans.
No-source/STUB grep finds no remaining no-source body or STATE[STUB] in either
module; target-unobservable server surfaces remain explicitly qualified.

This pass batches the following source hypotheses into one full measurement:

- `handle_send`: singular success suppression in the existing target line103
  gap, preserving the const bool local. At line113 retail stores buffer+4 in
  a pointer temp and normalizes the low bit with neg/sbb/neg. The existing
  pointer_cast helper plus an enum-valued packet-count ternary reproduces that
  source hypothesis without a fabricated named local (base was17B short).
- `fill_packet_header`: pointer_cast for the final word destination, matching
  retail's otherwise missing pointer-value temp. The two12B-short serialize
  reference-slot expansions are separate residuals, not silently declared fixed.
- Flow-emulator constructor: allocator reference selects vectora's existing
  allocator-value overload. Retail copies the allocator value directly; base
  pointer overload added a returned-pointer store/reload and five bytes.
- Statistics comparison: retail checks a constant true after the six stream
  comparisons (mov1/test/je). Restore the final &&true expression, including
  its branch-distance effect; do not invent scalar comparisons absent in bytes.
- Login resolver: use the array-sized three-argument _itoa_s overload, as the
  core async connector already does. Retail materializes the integer argument
  in an unnamed slot before the four-argument CRT call (12B gap); keep its three
  recorded locals, with no new port temporary.
- Packet reallocate: add singular new_size suppression on the existing physical
  line while retaining UNREACHABLE_CODE and its argument. This is explicitly
  warning hygiene for an unobservable body, not proof of original placement.
  The body-recovery question remains open. Counter suppression in
  check_consistency is not invented in its consecutive addressed lines.

No Boost/STL/allocator/cast helper bodies outside these modules are changed.
The live previous_state review question now reflects the measured warning-only
result. All original scores, raw findings and untouched functions remain
controls for the single combined build. Recheck source/class declarations,
statement geometry, local provenance, all module scores, global maxima and
attribution changes after that build; no pre-build result constitutes closure.

Fresh source indexing parses all25 scoped TUs and1,000 facts. The strict source
comparison exits0 with734 MATCH,94 AMBIGUOUS and253 UNOBSERVABLE; no deterministic
failure. Use the namespace scope `vostok::network` for compare-source (which
includes network_core), not its filesystem indexing scope. The initial attempt
used the latter and reported unsupported; the corrected invocation is the gate
result above. Source/docs diff checks pass. Original target field variants and
unobservable mutability remain represented, not coerced into apparent matches.

## Measured partial batch: build 8f7907 (2026-09-14)

The full build succeeded in14m06s. This is a partial two-module batch, not
module closure. Flow-emulator construction (88.3276 to100), stats comparison
(94.5455 to100), and login resolve (96 to100) now have matching addressed
structure and total sizes. Their named-local sets also agree. handle_send
improves96.9531 to99.8779 with19 statements/745 bytes on both sides and the
same two locals including buffer's nested scope; rendered instruction streams
are identical, but the remaining object-score difference is not yet explained.
fill_packet_header improves71.0556 to74.6667; only its two serialize spans
remain12 bytes short apiece (target160/base136 total).

The success/new_size warnings are gone; the two check_consistency counters
remain unused. The enum-valued ternary introduces C4244 on its byte store.
Explicit narrowing is queued for the next measured pass, not silently included
in this snapshot. External linker warnings and two Scaleform extraction skips
remain. This checkpoint is not a warning-free or merge-ready closure claim.

The strict source gate still passes. Fresh raw PDB channels report96 structure
mismatches,60 geometry,2 locals,6 location,2 statement-file,17 target-only,
10 base-only and6 ambiguous identities. These are overlapping raw findings,
not unique unfinished-function counts. Global report:223 improvements,
121 drops to zero,100 fold changes, no additions/removals. Neither module has
a numeric score or banked-maximum drop. Three roster functions lose current
attribution (channel destructor and order/response deleting destructors);
their maxima remain. Lost attributions remain review work, not proven harmless.

The packet_reader::r raw-local discrepancy is parameter provenance: target
passes destination on the stack and returns with ret4; base promotes it to ECX
and uses ret. Both perform the same copy and cursor advance; the6-byte gap is
at that boundary. destination and size are parameters, not extra body locals.
Do not invent or delete body locals to clear that comparator finding.

## Reproduction

```sh
vostok-pdb compare pdb --target binaries/pdb/target/evidence.sqlite \
  --base binaries/pdb/base/evidence.sqlite --file vostok/network --json
vostok-pdb inspect --target binaries/pdb/target/evidence.sqlite \
  --base binaries/pdb/base/evidence.sqlite --function 'EXACT_SELECTOR' \
  --view structure-diff
# Add --rva 0x56a4d0 or --rva 0x74b6d0 for the two destroy_client procedures.
vostok-pdb topology --target-pdb RETAIL_PDB --base-pdb BUILT_PDB --classes --json
```

Filter the uncapped class JSON by anchored namespace after extraction.
`topology --class` takes an exact class name, not a module-prefix selector;
an empty result is not a successful namespace audit. Raw captures live under
`binaries/` or task scratch space, not as a second committed evidence database.

Work now proceeds in user-requested module-scale batches. Build notifications
trigger prompt return to the measured batch. Any preparation stays outside its build
inputs. No statement, local, type variant or one-sided procedure is deleted
merely to make a counter reach zero.
