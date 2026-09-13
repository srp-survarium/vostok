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

Work proceeds one measured unit at a time. Build notifications trigger prompt
return to the measured unit. Any next-unit preparation stays outside its build
inputs. No statement, local, type variant or one-sided procedure is deleted
merely to make a counter reach zero.
