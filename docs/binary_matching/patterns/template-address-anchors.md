# Template carcasses need member-fn-ADDRESS anchors (a call site inlines them away)
tags: cpp:template | topic:anchoring
symptoms: 0 symbols for anchored template members, body inlined into the anchor
confidence: 6/10
variants: real-call-sites-for-templates.md, addressof-virtual-anchoring-trap.md

Calling `packet.append(...)` / `reader.r<u8>()` from the IncludeAll anchor inlines the
body into the anchor at /Od and emits NO standalone COMDAT. Take each member's ADDRESS
instead - ODR-uses the out-of-line body without a call site.

```cpp
void ( tcp_base::*p )( u8 ) = &tcp_base::append; example_callback( (pcstr)&p );
// template members: &reader::r<u8>, &reader::r_string<16>
```
Private members (packet_reader::pointer) need the anchor befriended via the `namespace vostok { void use_network_core_packet_reader(); }` forward-decl + `friend void ::vostok::use_network_core_packet_reader();` idiom.
evidence-basis: mechanism only; entry names no verified anchor outcome
