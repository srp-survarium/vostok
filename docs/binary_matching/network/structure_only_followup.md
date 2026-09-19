<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Structure-only follow-up

User requested no further builds and stopped Holista. The changes below follow
completed build `077f93f3d82742a29a42ab0b66b8287d`; they are unbuilt source
reconstructions, not measured closure. Do not commit them with that build's
ledger as though the ledger measured this new source state.

| Source/function | Retail evidence | Source change |
| --- | --- | --- |
| network/match_client.cpp: create_client | Raw lines45,46,48,52,53,54,55; one const pointer local | Restore observed locations; preserve allocator expression, assertion, publication, callbacks and local |
| network/match_client_impl.h: send_queued_packets | Single53B record on line54; assembly branches over the send call when disconnected | Compact inline body, preserving guard and call |
| network_core/http_client.cpp: constructor | Single286B record on line28, no named locals | Put empty braces together, preserving all initializers |
| network_core/udp_match_connection.h: channel constructor | Single83B record on line153, no named locals | Put empty braces together, preserving both explicit initializers |
| network_core/base_packet.h: constructor, const buffer, buffer_size | Header-attributed singleton records on lines15,21,23 | Restore observed locations without changing expressions, fields, access or order |

Singleton records support these compact-body hypotheses, not exact recovery of
original spelling. Logical line resets preserve unreviewed neighbouring source.
No runtime checks, named locals, helper declarations or compiler controls added.

## Checked without changing source

The existing PDB attributed-order comparison finds no inversions among its
comparable paired definitions, including headers. It excludes folded aliases,
missing records and ambiguous pairings; it does not prove all physical syntax
order or module closure.

packet_reader::pointer has the same raw records on both sides: (0,3,43),
(3,1,45). Its decorated representative also names unrelated folded helpers;
joining only on that representative creates spurious line-count differences.
Do not compact this accessor to repair such a join.

string_response::execute has seven matching body spans. Retail attribution is
shared with string_order::execute; preserve the response implementation instead
of moving it to the order header or rewriting its scopes from that folded record.

The direct-module-allocator alternative for match_client::create_client remains
scratch-only. The current responses_allocator accessor returns the same global
allocator; no allocator ownership correction is claimed or applied here.

## Lexical-block pass

Across291 unique full-name pairs in the completed databases,25 have explicit
statement-depth records on both sides. All25 depth sequences agree. This is
parsed block evidence, not proof that every lexical scope survived compilation.
Another scoped target procedure is excluded by this exact-name pairing, so do
not claim complete scope coverage. Two of the25 have different absolute block
locations: match_client::on_packet_received and the sign-in answer handler.

For login_client_impl::on_sign_in_answer_received, remove the two blank lines
before the assertion and anchor opening19/assertion20. Existing body statements
then occupy their retail lines through the final callback79, including scope
entry37. Anchor closing83 without guessing the contents of the preceding source
gap. Preserve following method attribution with a reset. No switch, local,
callback expression or branch was changed.

The match-client whole-file boundary sequence is now reconciled. After the
create_client anchors, use observed openings58 (response allocator creation),89
(client constructor),117 (destroyer destructor),135 (client destructor), and200
(enqueue_impl). Existing source spacing then reproduces every subsequent boundary,
including the receive-handler opening235, scope entry237 and closing249.
All20 observable function opening/closing pairs agree with the retail map in a
source-level #line walk, with definitions remaining in monotonic source order.
The generated deleting destructor's synthetic line0 is excluded. No function
body, field, access specifier or helper expression was changed in this pass.
This is source-attribution validation, not a claim of rebuilt PDB or byte equality.

Still open: source-invisible helpers, class variants, source-gap interpretation,
statement-size differences and verification of the new emitted line records.
Matching observed locations alone does not close any of those channels.

## Login sign-in/sign-out file boundaries

Follow-up source-attribution check compares each target row's line to the
corresponding existing base row's source text (unique full-name pairs, equal row
counts). All159 available sign-in rows,78 sign-out rows and81 match-client rows
resolve to the expected text after the edits:318 checks, zero mismatches. This
tests reconstructed attribution, not compiler output or original source spelling.

The wider scan found additional absolute drift. Corrected: ping opening34;
core memory_allocator opening17; async_connector constructor16; UDP client
constructor39 and enqueue167; TCP client constructor19. The latter also moves
its following destructor, start_reading and on_connected onto observed lines
24,32,37 before the existing connect anchor45. All expressions are unchanged;
the speculative explicit-member-initializer candidate remains unapplied.
Network HTTP, sign-up and match_client_impl absolute locations are now corrected
as a20-procedure batch (six, seven and seven respectively). Sign-up's first
handler loses one unsupported blank line after the opening brace. Existing
expressions, declaration order, scopes and callback lifetimes remain unchanged.

The resulting source-attribution scan covers1365 available rows across23 cpp
files using unique full-name pairs, equal record counts, and existing base source
text. It reports two intentional ASSERT-to-ASSERT_U text changes and one lookup
ambiguity: udp_match_client.cpp reuses logical154 for connect's consistency call
and a later annotation. The expected call still occupies154; a single-value
line dictionary would overwrite it. Preserve this ambiguity rather than treating
the annotation as the call's replacement or declaring all source mappings exact.
Missing source text, nonunique identities, unequal record counts, headers and
unretained procedures are outside this particular check. No emitted-PDB or
byte-equality claim follows from these source-level results.

All12 retained procedures across these two files now have retail opening and
closing logical lines in a source-level walk. Sign-in pairs:19..83,90..124,
127..154,157..205,208..239,242..279,282..311. Sign-out pairs:18..40,43..71,
74..106,109..139,142..163. Password-written handlers lose the extra blank
before their first error test; remaining bodies, callback expressions, locals
and scopes are unchanged. These boundary checks do not replace verification of
every emitted interior record. No build was launched.

## Header location batch

The separate header scan checks339 available source-attribution rows, initially
finding115 text/location disagreements. It is not the full header inventory:
same-file/full-name uniqueness, equal row counts and base source text are required.

Applied observed body openings for receive_response destructor36/execute42,
receive_udp_response destructor43/execute49, send_order destructor38/execute44,
and send_queued_order destructor36/execute42. TCP packet constructor16 naturally
places its destructor at20; UDP packets allocator constructor21 and connection
channel reset155 are also anchored. No declarations, access sections, initializers,
locals, deletion operations or stats-copy operations changed. All remain unbuilt.
The follow-up header batch anchors packet_inline.h's constructor at15; ordinary
spacing then places resize35, the observable scalar/vector overloads, and bulk
append136..148 at their retail locations. UDP construct_packet is anchored21
through the final ordered append31, with closing36; no source is invented for
the unobserved gap. Its message-info lookup and serialized fields are unchanged.

String-order constructor bodies use their own retail pairs29..30,38..39,47..48;
destructor50..59 and execute61..69 follow. String-response's own retained
constructor31..32 and destructor52..61 are mapped, without imposing the folded
string-order execute attribution on its separate response body.

After these changes the same339 header-row source-attribution checks report
zero text/location disagreements. The lookup preserves multiple texts at reused
logical lines and checks membership rather than silently overwriting a prior
location. These checks still exclude unpaired/missing-text/unequal-count records;
they prove neither unique original spelling nor future emitted PDB equality.

## Exact-name exclusions

Twelve target records fall outside unique full-name pairing: the nonunique
s_world initializer name, the core HTTP error setter, base_packet's default
constructor, get_connection_info_from_string, and eight packet-template records.
These exclusions are not missing-source verdicts.

The connection-string parser is present under a signature displaying char* const
instead of char*. Its ten raw offset/size/line records agree with target, including
depth1 on line160; delim/result/port locals are preserved. This recovers the26th
target procedure with explicit depth evidence without changing its source or
reopening parameter qualification work.

The core HTTP setter's retail record is a66B singleton on header line22; its
existing single-line assignment is anchored there, preserving the declaration
and callback reference. Presence/size remain unverified. Default packet constructor
and template bodies already exist; folded scalar aliases and unretained vector
overloads must not be given fabricated retention anchors or duplicate definitions.

## Assertion-only locals (source model)

check_consistency's declarations194/195, assertion196 and closing197 leave no
standalone source lines for the two added unused-variable suppressions. Replace
ASSERT plus those suppressions with the existing ASSERT_U macro, keeping the
UNKNOWN_EXPRESSION_T condition marker. This naturally restores the five-line
body without resetting the closing line backwards. Both target const locals
remain declared and calculated in the same order.

Use the same model for handle_send's success local: ASSERT_U at102, blank103,
guard104. Preserve the local, erase operation, branch scopes and cleanup order.
The previous body projections already matched (3 and19 statements respectively);
this change addresses plausible source spelling rather than adding statements.

In release, ASSERT_U expands through VOSTOK_EMPTY_EXPRESSION_U_VA_ARGS, retaining
the expression in an unreachable expression_eater call; ordinary ASSERT discards
it. The macro therefore supplies the missing syntactic use. Sibling engine code
uses this idiom, including logging/log_file.cpp's ASSERT_U(success). No shared
macro is changed. Exact original spelling, emitted bytes and warning suppression
remain unverified; keep both review questions open until measurement is allowed.
