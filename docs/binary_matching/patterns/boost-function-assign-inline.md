# `m_fn = value` (boost::function): target calls folded operator=, base may inline copy-swap-clear
tags: cpp:operator cpp:template | asm:call asm:lea asm:push | topic:inline-vs-call
symptoms: one call folded operator= vs function ctor + swap + TWO call clear, frame +0x0C..0x10, missing push esi/edi
confidence: 6/10
variants: boost-function-byvalue-copy.md, boost-bind-icf-sibling-fold.md

boost::function operator=(function const&) in TARGET is one `call <ICF-folded operator=
rep>` (shown under an unrelated signature, args LTCG-reg-promoted). The /Od+LTCG BASE may
inline the body: construct a 0x20-byte temp copy, `call function1::swap`, then TWO
`call clear` - growing the frame and freeing esi/edi (missing prologue pushes are the
TELL). Per-call-site whole-program inline-vs-call; `m_fn = value;` is correct, mark PARTIAL.

The decision is symmetric per call site: the TARGET may instead be the one that inlines
while our BASE emits the folded `call operator=`. Then the structure-diff is a single
`SIZE -0xNN` statement (base too SMALL), and the asm shows base `call
function<...>::operator=<bind_t<...>>` against target function-ctor (default-ctor +
`assign_to<bind_t>`) + `swap` + `clear`(temp dtor) = the partial-spec `self_type(f).swap(*this)`
body inlined. Either way `m_fn = boost::bind(...);` is correct; it is not source-steerable
from the assigning TU - leave it at the inline-vs-call %.

```cpp
m_on_disconnect = value;                                        // target calls, base inlines
m_stamina_subscriber.subscription_callback = boost::bind(...);  // target inlines, base calls
```
Evidence: network_core/udp_match_client::udp_match_client (set_on_disconnect inlined; stmt
0x73 vs target 0x59, 79.20% PARTIAL). game_core/player_logic_sprint_state ctor (function0<void>
operator= inlined in TARGET; single-stmt structure-diff SIZE -0x2e, base out-of-line
`call operator=`, 66.70%).
