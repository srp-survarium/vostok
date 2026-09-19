<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Network / network-core structural closure

## Measurement f8aff026 (2026-09-14)

### Next-pass candidates (not yet measured)

`match_client` explicitly initializes `m_response_packets_allocator(0)`:
retail invokes the pointer-taking intrusive_ptr constructor with a null
argument, whereas the omitted initializer selected our zero-argument overload.
This is a local overload correction, not a shared-template inlining change.

`clone_packet` now reads `packet.m_buffer.data()[0]`. Retail's call at
RVA 0x766201 decodes directly to 0x3f210, a folded empty body with a recorded
`boost::array<unsigned char,256>::data() const` identity in boost/array.hpp.
No const array-index accessor record was found for this specialization. The
target adds the buffer offset, calls that body, materializes its returned
pointer and reads the first byte; the prior direct index omitted 13 bytes.
This is an accessor hypothesis, not proof of exact original spelling: folding
also admits other empty aliases. Verify all nine spans and 218-byte target
body in the next combined build. Keep the destination access unchanged.

Full game build succeeded in 11m15s. `is_low_level_packet` now has eight
matching spans, 120 bytes, and 100% current/banked score: the sequence wrapper
deserializations recover the missing word copies. `process_low_level_message`
now has twelve matching spans and 283 bytes, rising to 99.9873%; the default
`NODEFAULT` restores the three enum comparisons. Its logging line immediate
still differs (base 544, target 591), so this is not byte-exact closure.

`match_client_impl::on_packet_received` now has zero named locals on both sides.
Its unnamed state selector matches the target shape; eleven spans remain,
383 target / 392 base bytes, with a nine-byte first callback-check residual.
Current/banked score rises from 95.8333 to 95.8726. No module score decreased.

A fresh 286-row inspection pass using the isolated, tested exact-signature
candidate selector reports network: 107 addressed matches, 48 differences,
4 selector failures; network_core: 93 matches, 24 differences, 10 selector
failures. Selector failures include target-side folded aliases and genuinely
missing base records; none are counted as matches. These totals are not
whole-function byte closure or a substitute for declaration/class verification.

The build emits the existing two `udp_match_client` C4189 warnings for
`registered_packets_count` and `allocated_count`, but none in the edited
functions. External/linker warnings and two Scaleform UnexpectedEof extraction
skips remain. Incremental warning counts cannot establish whole-tree cleanliness.

Global report review remains open: 100 reported drops, 218 improvements,
95 fold-churn entries, no additions/removals. Final report/cross-unit scores
recover at least the prior score for 71 drops; the change list is generated
before cross-unit scoring. The remaining 29 entries cover 25 unique symbols:
five rendered-instruction matches, seven call-name-only differences, two
instruction/operand differences, eleven unresolved selectors. Preserve these
findings rather than calling every zero harmless. `render_model` has an ESI/EDI
difference; `DIPixelProvider` has a vtable-symbol difference requiring data-alias
verification. Unchanged source outside the two edited engine files does not by
itself prove all emitted code is unchanged.

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

## Follow-up preparation after c83a43d03

Two source hypotheses are queued: explicitly narrow handle_send's enum-valued
tag to u8, and use empty aggregate initialization for sign_in_on_connected's
version[8]. Retail's line261 initializes the array with xor plus two dword
stores (8 bytes); the current explicit first zero produces a byte store plus
seven-byte remainder initialization (16 bytes). Both sides record exactly
buffer:u8*, version:char[8], account_name_length:const u8. No initialization is
removed and no local is introduced. The exact resolver's stale residual comment
is removed without shifting any physical line. These are not measured results.

Additional investigation: HTTP status find calls retain an out-of-line two-arg
string overload in retail, whereas base expands length() and calls the three-arg
overload. The current expression already invokes the right overload; no vendor
helper change or alternate search algorithm is justified. Packet deletion's
15-byte deficit is allocator-policy deallocate/decrement calls versus retail
expansions. Enqueue-order destruction's14-byte excess is expanded intrusive_ptr
dereference/assert versus the target's folded dereference call. These are
concrete outstanding helper-boundary questions, not attributed to argument
promotion and not closed. Serialization similarly retains its pbyte& signature
but differs in materializing that reference at each inline site.

Raw class audit of the latest measured image:50 identical,2 duplicate-record
multiplicities,5 overlapping variant sets,2 missing classes,1 differing server
class. The live sequence_number<u16> is identical with19 declarations; an older
generated header omits/reorders information, so no order change is retained.
Do not add artificial instantiations for missing sequence_number<u8> or the
2048-packet arena. The server hook offset and alternate variants remain open.

## Follow-up measurement: build08912eab

Succeeded13m05s. The explicit narrowing removes the owned C4244;
handle_send remains99.8779,19 spans745B with matching structure. Resolver
comment cleanup is byte-neutral:100,7 spans623B. Empty aggregate initialization
reproduces the target's8-byte initialization span; sign_in_on_connected
improves current77.6519 to79.6296 with18 spans and the same3 locals. Its base
total shrinks631 to623 bytes versus retail539. The remaining differences are
callback expansion61B, buffer and strlen spans1B each, and async bind21B.

Important: changing sign-in's body resets its banked max93.3481 to79.6296.
This historical headroom remains unresolved; a better current score is not
proof that the old banked peak was exceeded. Global current report has exactly
one improvement and zero regressions/additions/removals/fold changes. All other
network/network_core ledger scores and classes are unchanged.

The two check_consistency C4189 counters remain. Existing dependent-module
warnings,234 LNK4099,2 LNK4049 and2 Scaleform UnexpectedEof extraction skips
also remain. No warning-free or complete-module claim is made.

## Full-pass checkpoint at f9b1819ae (not yet built)

All286 ledger selectors were swept against the fresh databases:270 were
directly pairable,196 of those report addressed STRUCTURE MATCH,74 retain an
addressed diff. Sixteen selectors need alias/RVA or missing-base handling;
pinning recovers the unsigned-int append and folded functor/string execution
comparisons, and both destroy_client overloads. The scratch per-function
capture is /tmp/vostok-network-full-pass-f9b1819.tsv. These are inspection
counts, not closure counts. No full game build has been launched for this pass.

The sweep exposed an inspect CLI defect: --view base --json returned the
target record before resolving the candidate. Earlier assertions of identical
rendered instructions using paired JSON inspection are invalid. The text
structure comparisons, raw class comparisons and objdiff scores are unaffected.
The Rust fix selects the base first, rejects missing/ambiguous candidates, and
rejects unsupported JSON diff views instead of returning misleading records.
All76 Rust tests pass, including3 new JSON-selection regression tests.

Independent rerun using --database separately for each side yields270 paired
records,59 identical rendered instruction streams and two differing local
sets: previous_state and the packet-reader destination parameter already
tracked above. In particular handle_send is NOT instruction-identical: target
frame0xdc versus base0xcc, shifted temporary slots, differing folded call names,
and LOG line immediates119/125 versus101/107 remain despite equal19 spans745B.
Its local set does agree. Do not attribute those frame differences to argument
promotion or describe the function as fully closed.

Queued engine hypotheses, to be measured only after the complete module pass:
- is_low_level_packet: consume the two sequence fields through existing
  sequence_number<u16>::deserialize rather than discarding two raw u16 reads.
  Retail emits a u16 reader call plus two word copies per field (20B) while
  base discards each result (8B). The helper constructs the sequence wrapper
  without introducing a named caller local; keep reader and bits only.
- process_low_level_message: separate the unreachable default from case zero.
  Retail compares against0,1,2; current case0/default merging removes the first
  comparison. NODEFAULT is a hypothesis for the retail fall-through dispatch,
  not yet a measured reconstruction. Do not change the recorded bool read or
  invent a reachable default action.
- match_client_impl::on_packet_received: a single-state switch is now queued
  instead of the reconstructed previous_state local. The target copies m_state
  to an unnamed selector-region slot at line45 and starts the message guard at
  line47, with no named locals. waiting_for_permission is the callback's initial
  state; the success path replaces this callback before entering normal packet
  processing. Test the sole-state dispatch plus unreachable default without
  inventing another local. This is not yet proven; its source TODO and external
  review row remain open, and its callback paths must be measured intact.

The presence pass also found that folded representatives could differ while
their full demangled signatures still agreed. Comparator pairing now narrows
shared representatives by full signature, and falls back to exact signatures
when the decorated key is absent. Such fallback preserves an explicit correlated
identity mismatch with both names; it does not suppress source-location or body
differences. Two new tests cover signature disambiguation and preservation of
identity/location discrepancies. All78 Rust tests pass. Refreshed raw findings:
13 target-only,7 base-only,4 correlated identity mismatches; zero ambiguous
identity findings. The explicit identity mismatches preserve representative
differences rather than hiding them as matches. Location now has11 mismatches,
statement-files4, locals2, statement-structure97 and geometry62. Newly paired
bodies explain the increased comparison coverage; these are not engine gains.

The total-size audit additionally identifies ten functions whose addressed
spans match but total sizes do not: async_connector ctor128/134,
match_client ctor1046/1082, network tcp_packet_client ctor339/345,
udp_match_connection ctor553/546, udp_match_packet ctor179/191,
login_client dtor315/313, match_client_impl dtor104/113,
client_destroyer deleting dtor46/44, packet_reader::r24/18,
login_client::sign_in_impl201/199 (target/base bytes). Initializer and epilogue
differences remain work even when the addressed renderer says STRUCTURE MATCH.
The async_connector ctor6B excess is an extra this-pointer temporary around
the m_on_error default-construction call; the declared member and operation
already agree. Do not add an assertion or fabricated local to compensate.

## Build f195b4927af84b589bb6ba024bd5230e verification

The full game build succeeded in 11m06s. The module-owned warnings remain
C4189 for allocated_count and registered_packets_count in udp_match_client;
the log also contains 234 LNK4099 warnings and two skipped Scaleform PDB
modules (UnexpectedEof). This is not a warning-free or whole-PDB closure.

The explicit response-allocator null initializer selects the pointer-taking
constructor used by retail, but base expands zeroing plus set(0) instead of
retaining the call. Constructor size is now target1046/base1095 bytes, versus
base1082 previously; cur falls from93.5885 to93.2305. The three addressed
spans still report STRUCTURE MATCH, demonstrating why initializer and total
size checks must accompany that verdict. The residual remains open.

clone_packet's const data()[0] spelling leaves the result unchanged:
9/9 addressed spans, target218/base205 bytes, cur93.6842. The first-byte
assignment remains target25/base12 bytes. Its accessor-call hypothesis has
not closed the mismatch and is not a measured gain.

The global report-changes summary has117 regressions,218 improvements,
95 fold-churn entries and no added/removed entries. These summary labels
are not proof that every global regression is harmless; final cross-unit
pairing and affected bodies still require the ongoing regression audit.
Exact demangled-name lookup across the final report plus cross-unit scores
recovers at least the prior score for94 of117 entries;23 remain unresolved by
that check. Network-related network_world deleting destructor and SSL io_op
copy constructor recover100 and52.333332 respectively. The login callback's
Boost storage3 copy entry remains0 (previous84.95652), and match_client retains
the measured decrease above. Missing or folded pairing is not absence proof.
The storage3 copy is confirmed present by exact target RVA0xe57f0 and paired
base RVA0x4b000: target63/base72 bytes, no addressed spans on either side.
Retail retains a Boost function copy-constructor call; base expands its empty
construction and assign_to_own call. Thus the report's zero is not an absent
implementation, but the body still has an unresolved9-byte expansion residual.
The empty addressed projection again cannot certify byte closure.

## Next batch: generator fidelity, handshake identity, warning cleanup

Raw retail enum0x1a64a/field-list0x1a649 records
match_server_connection_successful=128. Use the existing network/message_types.h
enumerator at the handshake consumer and remove the guessed anonymous
set_status_ready_for_battle alias from login_server/message_types.h. Its open
review row is resolved by raw enum evidence, not inferred from the immediate.

The generator now retains non-compiler-generated copy declarations without
standalone bodies, preserves base access and virtual status, and stops guessing
explicit from argument count. Signed-minimum enum overflow and unsigned64
rendering are corrected. Isolated Rust tests:86 passed. Generated artifacts are
not hand-edited; fresh generation is required to validate these corrections.

At the user's request, both check_consistency locals receive the project's
VOSTOK_UNREFERENCED_PARAMETER macro after the assertion. Preserve calculations,
const types and assertion. Baseline target/base:3 addressed spans,119 bytes.
Next full build must confirm C4189 removal and unchanged compiled structure.

## Build c0dd07dd7f234ceeb49039352a74c440 warning verification

Full game build succeeded in12m58s. Neither registered_packets_count nor
allocated_count appears in a warning. No warning was attributed to a scoped
network/network_core path or matching relative source basename. Remaining
C4189s belong to animated_model_instance_cook (res_user_data) and
network_client_processing (team_2_points), outside these modules. This broader
compilation also reports many other-module/header warnings; counts are not
directly comparable to the previous incremental log.234 LNK4099 and two known
Scaleform UnexpectedEof warnings remain.

check_consistency retains3/3 addressed statements and119/119 bytes. Its current
score remains99.9444 before/after; target/base local slots are still reversed
and the empty assertion callee has a folded representative-name difference.
Warning cleanup is verified, not a new byte-exact closure. Whole-batch regression
and handshake-enum verification remain pending before committing this build.
Completion audit: report-changes contains zero regressions, improvements,
added/removed entries or fold churn for this build. The canonical handshake
enum consumer retains11/11 spans and383/392 bytes, with the same first-guard9B
residual. No new score changes are attributed to this batch. Earlier-build
global unresolved findings are not erased by this empty delta report.

## Next batch after c0dd

First handshake success guard now tests !m_on_connected.empty(). Retail calls
the empty-result alias atRVA1dd0 then directly tests the returned bool; failure
guard retains safe-bool neg/sbb/not/and and stays unchanged. Broad callback
inspection found no other justified direct-empty replacement, including both
TCP size-receive template specializations. This remains unmeasured source work.

Prepared generator changes now integrated: preserve raw overload-list order,
extract static fields without instance offsets, retain supported field/method
interleaving and access transitions.89 isolated tests passed, including a mixed
declaration-order fixture. Retail generation succeeded: predicate copy then
ordinary constructor; private fields before public destructor; async_connector
fields before destructor; protected static resource_class in a validation class.
Nested declarations remain outside this renderer's order guarantee. No engine
class is reordered from generated output alone.

## Build c2f22d02cdf346919eefae52d65b5953 verification

Full game build succeeded in10m35s. The direct-empty success guard closes the
nine-byte excess: on_packet_received now has11 addressed spans and383 bytes on
both sides, zero named locals, and cur/max99.9902 (previous95.87255). The failure
guard retains its safe-bool conversion. The instruction diff still shows the
LOG_ERROR line immediate53 versus retail56; this is not byte-exact closure.
Folded call names differ and must not be confused with different callees solely
from their displayed aliases. Original guard spelling is not uniquely recoverable
from this folded empty-result test. Its pending source TODO will be cleaned up
in the next measured source batch, retaining the remaining geometry question.

report-changes has one improvement and zero regressions, added, removed or
fold-churn entries. No network/network_core warnings, including the two requested
unused locals. Other warnings persist:18 C4995,2 C4715,5 C4702,4 C4701,2 C4706,
2 LNK4049 and234 LNK4099; two Scaleform module reads still report UnexpectedEof.
The previous broader f195 regression audit remains open despite this zero-delta
regression report. Generator batch passed89 tests before this measured build.

Independent retail source-checksum audit confirms boost/array.hpp is identical
to the repository content modulo LF/CRLF: all232 recorded checksums, including
match_client_impl.obj, are82302fcac1308c4e8c761a8e69b43f3e, exactly our CRLF MD5.
Thus clone_packet's unresolved accessor-call residual does not justify changing
the shared Boost array accessor body. Prepared verifier improvements remain
outside this measured source state.

## Next pass: full-size projection audit (unmeasured source)

Integrated verifier/static-attribute changes pass94 tests (88 library,6 binary).
A read-only scan of both module path scopes found303 target records;275 have a
unique base match by mangled name (or full name if unmangled),28 require presence
or ambiguity triage. This inventory is not the ledger denominator. Ten unique
pairs have equal ordered projected-body sizes but differing whole-function sizes:

| Function | Target/base bytes | Body rows |
|---|---:|---:|
| udp_match_connection constructor |553/546|0|
| async_connector constructor |128/134|0|
| login_client::sign_in_impl |201/199|1|
| login_client destructor |315/313|1|
| match_client constructor |1046/1095|3|
| udp_match_packet constructor |179/191|2|
| client_destroyer deleting destructor |46/44|0|
| network::tcp_packet_client constructor |339/345|1|
| match_client_impl destructor |104/113|1|
| packet_reader::r(void*,u32,u32) |24/18|1|

Initial assembly triage: packet_reader::r differs at its argument boundary
(target loads a stack argument and ret4; base pushes ECX and returns normally).
client_destroyer's deleting destructor differs in frame allocation20h versus4h.
The async/TCP/UDP connection constructors differ in temporary spills around
callback construction; packet constructor retains a base constructor call in
retail while base expands it. match_client_impl destructor's final allocator
destructor call receives explicit this setup only in base. These are concrete
residual locations, not proof of unsteerability or completed constructor matching.
Login residuals and the28 non-unique/missing pairs still need focused triage.

Callback line evidence suggests an if/else rather than early-return shape:
retail jump at+c1 maps to55, immediately followed by LOG_ERROR at56; current
early return maps to50 followed by a brace/blank and log53. Candidate now uses
an else branch and the existing switch break. No padding or #line was added.
Both callback guards and arguments are unchanged; verify the whole function,
line geometry and byte score in the next full build before accepting the shape.

Presence follow-up resolves19 of the28 non-unique/missing-key findings:18 by
exact demangled signature and one dynamic initializer by signature plus file.
The remaining9 lack a standalone base record: http_client::set_on_error,
base_packet constructor, handler_allocator constructor, packet<udp_match_packet>
constructor, and its bool/u8/u16/float2/float3 append overloads. Source bodies
exist for all9; standalone absence is not a missing-body verdict, nor proof of
correct inline consumers. No anchors or replacement stubs were introduced.
Removed the obsolete core HTTP header diagnosis claiming the network HTTP
caller was unwritten; network/sources/http_client.cpp now calls set_on_error.

The two login full-size residuals include retail ESI preservation around
Boost callback clear calls, whereas base uses ECX; each explains the two-byte
total difference with matching body sizes. Frame-size/spill differences remain
visible and are not automatically dismissed as noise. This pass retains those
implementations rather than inventing locals to reproduce register allocation.

## Build6e628a154d1e4961b95eaaedc148f9ce verification

Full build succeeded in10m58s. The if/else callback candidate retains11 body
statements,383 bytes and zero locals, with unchanged99.9902 score. Its jump/log
source lines are now50/51 (retail55/56), matching the consecutive relationship;
the log immediate is0x33 versus retail0x38. Statement offsets all agree. This
supports the branch shape but does not close source locations or prove unique
original spelling. A separate prepared line-pin patch remains unmeasured.

All report-changes arrays are empty. No scoped network/core warnings; other
warnings remain18C4995,2C4715,5C4702,4C4701,2C4706,2LNK4049,234LNK4099 and the
two known Scaleform UnexpectedEof skips. Integrated verifier tests passed94.
Twenty-five base S_COMPILE records match all25 retail records: frontend/backend
15.0.30729.1,Cpp,Pentium3,LTCG enabled,security checks disabled. This metadata
does not record the optimization/inlining switches or complete command line.

## Review follow-up batch (build d178f08c)

Local branch codex/network-review-followup-20260914 starts above measured
4e1a18303 without rewriting PR579. Requested resolve cleanup replaces padding
with #line72, preserving target brace73/log74. Callback uses #line43 and a
single-line failure callback to align the body records; closing-brace location
still differs and remains explicitly open. No arbitrary blank padding added.

PR579 questions now have source TODOs plus review_todos rows for the constant
true spelling and both suppression-placement questions. Raw PDB confirms both
timestamp members are volatile long; the duplicate obsolete investigate row was
removed while retaining the existing resolved record. Review notes for the send
length and unreachable break are supported by the exact retail operand/edge.
The old default grouping entered in d5d40dad1 and was corrected in bf09ac11b;
the historical commit does not establish why that reconstruction was chosen.

Prepared OFFSET diagnostics are integrated. Read-only matching with exact-name
and file disambiguation finds no additional equal-total/equal-body-size but
shifted-body cases in this snapshot. The ten differing-total cases remain open;
the new diagnostic must not be presented as an engine matching gain.

Full build d178f08cea324c1ab30413507ae807a0 succeeded in 13m05s. The callback
improved from 99.9902 to 100 with eleven body spans, 383 bytes and zero named
locals; its log is now at retail line 56. Closing line 63 versus 65 remains
an open geometry question. No report regressions, additions, removals or fold
churn were recorded. No warnings were attributed to either scoped module;
the additional game recompilation emitted out-of-scope warnings, and the two
known Scaleform extraction skips remain. The pre-build Rust suite passed 95 tests.

The subsequent raw class audit covers 60 target names: 50 identical, two with
record multiplicity only, five overlapping variant sets, two missing base
records, and one different class (server container types embed differing session
hook offsets). No declaration-order discrepancy was reported. In particular,
sequence_number<u16> already has identical raw class structure: the proposed
source overload swap was not justified and was never applied. The u8 complete
record is absent from base. Source-definition comparison reports no scoped
order inversion, subject to its existing pairing/line/symbol coverage limits.

A fresh source AST audit covers 25 translation units and 1002 facts. Its 46
reported declaration-order checks pass, but an independent Rust coverage audit
finds 52 ambiguous-position and 236 unpaired qualified declaration names excluded
from order comparison. These are exclusions, not 288 source bugs. Both sequence
specializations are unpaired in that source view. Make these limitations explicit
before interpreting a clean source-order summary as module-wide closure.

## Reproduction

### Measured follow-up: reliable source text and order coverage

The source-order comparer now exposes excluded names and owners with fewer than
two comparable declarations. The real scoped query reports 46 subset matches,
62 unobservable owner comparisons, 52 ambiguous-position names and 236 unpaired
names. These diagnostics retain uncertainty rather than inventing source errors.

Candidate PDB source-text lookup now follows literal #line directives. Previously
on_packet_received's logical line44 displayed the physical line44 assignment,
not the opening brace. Duplicate logical locations and unsupported conditional
or macro-valued directives withhold text. Re-indexing is required to refresh
existing databases; build a87cfce055af4209a71b715a19db09a8 refreshed them. The Rust suite passed
101 tests before formatting, including six new coverage/mapping regressions.

That full build succeeded in 7m48s, with no compilation needed and no report
changes. The regenerated callback record now displays the opening brace at
logical44, switch at45 and log at56 correctly. EGL warnings and the two known
Scaleform extraction skips remain; no compiler/linker warnings were emitted.

The two current QUANTITY rows were rechecked against raw retail records:
send_queued_packets has one whole-function line54/53B record; packet<tcp_packet>
reallocate has one line46/56B record. Base exposes two and one projected interior
statements respectively. The retail operations remain present in source and the
bytes score100. Do not delete the guard or wrapper call to force an empty body
projection. The attribution differences remain open, not silently classified
as complete because the byte score is exact.

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
