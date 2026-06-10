# 0%/unpaired/None with plausible source = check the mangled join FIRST
tags: cpp:member cpp:const | topic:mangling topic:scoring-artifact
symptoms: 0% unpaired, scores None, not found in BASE index, @@EBE vs @@UBE, QAV vs PAV, whole unit at 0
confidence: 9/10
variants: mangled-access-instance.md, mangled-access-virtual.md, mangled-access-static-fn.md, mangled-access-static-data.md, param-pointer-const-mangling.md

The base/target pairing joins on the EXACT mangled symbol; access letters, virtual-ness,
const `this`, and parameter top-level consts are all baked in. A wrong letter fails
SILENTLY (no compiler/objdiff error) - 0%/unpaired even with byte-identical code. Before
banking any 0/None, diff the mangled names in both rich indexes (`mangled` field). A
WHOLE unit at 0/None = the class's access sections are wrong class-wide: grep both
indexes for `@<class>@survarium@@[A-Z]{3}` and fix the header sections.

Caveat: privatizing members can break temp_include_all anchors - add the
`friend void ::vostok::use_...( );` declaration (chamber_a_round_state_base precedent).
Evidence: tcp_packet_socket (PR #291) four public->private flips 0 -> 85-100 (new_packet 100 byte-perfect); weapon_core_chamber_a_round_aimed_state_base target IAE/MAE/EAE/EBE vs our QAE/UAE/UBE - header fix paired all six (ctor 0->82.6, serialize 0->69.3, 0 regressions); weapon_core::serialize 0->62, target_predicate 0->100.
