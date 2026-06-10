# Real call sites keep r<T>/append standalone (the actual lever for template bodies)
tags: cpp:template | topic:anchoring topic:inline-vs-call
symptoms: address-anchored template scores poorly, append statement silently dropped at call site
confidence: 6/10
variants: template-address-anchors.md, single-tu-anchor-inlines-helper.md

An address-anchored template body is the debug-quality single-TU emission; the target's
standalone bodies survive because real serialize/deserialize functions CALL them.
Implement the typed `serialize(udp_match_packet&)` / `deserialize(packet_reader&)`
methods (player_input, player_state, hit_info, sequence_number, ...) and anchor those -
the primitives get genuine call sites.

Pair with: every scalar `packet<T>::append(uN/sN/bool)` overload must have the
`append(&value, sizeof(value))` body - an empty `/* no source */` overload inlines to
nothing and SILENTLY DROPS the append at the call site (caught as an ONLY-target stmt:
client_player_update::serialize missing append(time_in_ms)).
evidence-basis: silent-drop caught once; main lever claim has no stated outcome
