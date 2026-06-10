# `m_fn = value` (boost::function): target calls folded operator=, base may inline copy-swap-clear
tags: cpp:operator cpp:template | asm:call asm:lea asm:push | topic:inline-vs-call
symptoms: one call folded operator= vs function ctor + swap + TWO call clear, frame +0x0C..0x10, missing push esi/edi
confidence: 8/10
variants: boost-function-byvalue-copy.md, boost-bind-icf-sibling-fold.md

boost::function operator=(function const&) in TARGET is one `call <ICF-folded operator=
rep>` (shown under an unrelated signature, args LTCG-reg-promoted). The /Od+LTCG BASE may
inline the body: construct a 0x20-byte temp copy, `call function1::swap`, then TWO
`call clear` - growing the frame and freeing esi/edi (missing prologue pushes are the
TELL). Per-call-site whole-program inline-vs-call; `m_fn = value;` is correct, mark PARTIAL.

```cpp
m_on_disconnect = value;
```
Evidence: network_core/udp_match_client::udp_match_client (set_on_disconnect inlined; stmt 0x73 vs target 0x59, 79.20% PARTIAL).
