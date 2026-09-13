<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Network and network-core PDB evidence audit

This is the durable record for the unified `vostok-pdb` audit of `network` and
`network_core`. SQLite evidence databases are generated and uncommitted; this
document records the source decisions and proof boundaries, not a copied dump.

## Deterministic gates

The scoped candidate-source comparison checks every parsed class, base, nested
declaration, method, and field for the target-observable subset of:

- public/protected/private access;
- static/instance and virtual/non-virtual method status;
- method const qualification;
- field offset and static/instance status;
- declaration presence and relative declaration order; and
- literal placeholder bodies (`/* no source */`, `STATE[STUB]`, and
  `STATE[REMOVED]`).

`mutable` and pure-virtual source syntax are retained as explicit
`UNOBSERVABLE` findings because this retail CodeView does not encode them
reliably. Duplicate PDB type variants are retained rather than canonicalized;
an overlapping variant set is `AMBIGUOUS`, never a guessed match.

The scoped index parses 25 translation units and currently records 1,000 source
facts. The strict comparison result is 734 `MATCH`, 94 `AMBIGUOUS`, 253
`UNOBSERVABLE`, and zero `MISMATCH`, `TARGET_ONLY`, `BASE_ONLY`, or
`UNSUPPORTED`. Thus every deterministic finding in this gate is closed without
mislabeling unobservable source syntax as proved.

## Nested declarations restored from the retail type stream

These aliases are semantic names, not decoration. They identify the callback,
packet, statistics, allocator, endpoint, and queue roles used by the adjacent
methods and fields. Each was restored at its observed relative position and is
used by the relevant declarations rather than left as an unused PDB prop.

| Class | Restored declarations | Meaning at use sites |
|---|---|---|
| `connect_order` | `udp_match_packet`, `udp_match_packets_allocator_ptr`, `connect_type` | the retained packet, its owning allocator, and the deferred connection callback |
| `enqueue_order` | `udp_match_packet`, `udp_match_stats`, `udp_match_packets_allocator_ptr`, `enqueue_type` | queued packet ownership and the source/target delivery counters |
| `http_client` | `on_content_downloaded_type`, `on_error_type` | completion and failure callbacks shared by setters, request entry points, and stored fields |
| `login_client` | `sign_in_callback_type`, `sign_out_callback_type`, `sign_up_callback_type` | the three protocol completion contracts and their retained callback fields |
| `receive_response` | `on_packed_received_type` | the retail spelling (including its `packed` typo) of the packet-reader callback |
| `receive_udp_response` | `on_packed_received_type`, `udp_match_packets_allocator_ptr`, `udp_match_packet`, `udp_match_stats` | callback, packet lifetime, and delivery-stat ownership |
| `send_queued_order` | `functor_type`, `udp_match_stats` | deferred send operation and synchronized delivery statistics |
| `string_order`, `string_response` | `functor_type0`, `functor_type1`, `functor_type2` | one-, two-, and three-string callback shapes used by constructors and fields |
| `tcp_packet_client` | four `client_on_*_type` aliases | receive, connect, disconnect, and error callback contracts |
| `network_core::http_client` | `tcp`, `on_content_downloaded_type`, `on_error_type` | socket/resolver family and its two callbacks |
| `udp_match_client_session` | `on_packet_received_type`, `udp` | server receive callback and endpoint/socket family |
| `udp_network_flow_emulator` | `on_packet_received_functor_type`, `delayed_packets_type` | delivery callback and owned delayed-packet container; the container alias belongs immediately before the fields |

The four queue/channel aliases used by `two_way_threads_channel` were moved to
namespace scope. The target class record contains the two channel fields but no
nested aliases, so retaining those aliases inside the struct was a real
class-structure mismatch.

## Explicit proof boundaries

- `udp_match_fixed_packets_allocator<2048>` is present as a complete retail
  type but has no emitted retail procedure. Candidate source contains the exact
  template and documented size specialization; whether MSVC emits that complete
  specialization record is a debug-type reachability effect, not evidence for
  another runtime helper.
- `udp_match_server` container types differ only in numeric CodeView rendering
  of pointer-to-member template arguments while their declarations, field
  offsets, and actual pointer-to-member identities agree in the reviewed source.
  The raw values (1364/1372 and 1380/1388) remain visible as `nested-type` and
  `field-type` differences; the comparator does not erase these differences or
  claim that the differing complete target variants all match.
- Class-property, nested-type, and duplicate-variant residuals are not collapsed
  into access or declaration-order claims. Their exact variants remain
  available through `vostok-pdb topology --classes --class NAME --json`.

## `destroy_predicate`

Retail and base both record a compiler-generated public copy constructor at
order 1 and the explicit `udp_match_server&` constructor at order 2. The private
`boost::noncopyable` base makes an attempted copy unusable. No explicit copy
constructor is warranted, and none is present in the reconstructed source.

## Full-build integration check

Build `0ea30c87b37d4450bb350e0bb4a88f80` compiled and linked successfully with
zero objdiff regressions, then failed in data-manifest generation because two
remaining readers treated SQLite as JSONL. Both readers now use the shared
SQLite loader. A regression test covers target/base reads and same-address
aliases; the real manifest stage also completes successfully after the fix.

The linked PDB reduces the network/network-core classes with raw differences
from 22 to 10. Remaining categories are record multiplicity, variant sets,
class presence/properties, and the server container types described above.
No access, static/virtual status, declaration-order, or field-offset differences
remain in this class comparison. This does not close statement/byte mismatches
or the explicitly retained variant and unobservable findings.

Build `4629aa8bd302498e9800357e6b942040` subsequently completed every stage
successfully in 11m20s, with zero objdiff delta regressions and `OPEN=0` in
both network data gates. Relative to the preceding committed source, the
generated deleting destructors for `connect_order` and `enqueue_order` have
changed source hashes: their banked `max` resets from 100 to 99.7059. Their
current byte scores did not change. This source-hash peak loss remains visible
in the generated ledger; it is not described as an unchanged maximum.

Review also fixed the source adapter's stripped `/clang:` forwarding flags and
Wine-only VFS overlay paths. Before that correction, `.cpp` parse errors had
been excluded from the coverage count. All in-scope errors now invalidate the
strict gate, and the corrected adapter parses all 25 units without those errors.
Static fields and free functions are indexed using storage class and semantic
owner, rather than the member-function-only static query. A separate libclang
fixture verifies instance/static fields, instance/static methods, and a static
free function.
