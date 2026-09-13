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
