# Single-TU anchor INLINES a now-real inline helper the target CALLs
tags: cpp:inline cpp:template | asm:call | topic:inline-vs-call topic:anchoring
symptoms: target single call <helper> vs base inlines helper body at site, helper absent from base index, 0%
confidence: 9/10
variants: real-call-sites-for-templates.md, inline-vs-call-trivial-getter.md

Consumers of a small inline helper (free fn or template: packet_reader::r<u16>,
delete_udp_match_packet, ctors) show target emitting one `call <helper>` while the base
inlines the helper's body at the site. The target's /GL build keeps these as one
out-of-line COMDAT called from every site; our single-anchor-TU base inlines the small
body at each /Od call site - a pure inline free function may emit NO standalone COMDAT
at all (0%/absent from base index). Making the helper's body real does NOT fix the
consumer (it now inlines the real body). Mark consumers PARTIAL with this residual.

Evidence: whole udp_match_connection unit (enqueue 4/5 stmts, is_low_level_packet, new_udp_match_packet); the packet_reader_inline.h header note ("a single-TU anchor cannot reproduce that").
