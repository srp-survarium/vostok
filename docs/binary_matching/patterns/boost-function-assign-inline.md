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

Variant - `assign_to` inside a `boost::bind(...)` ARGUMENT (temp function constructed at the
call site, not an `operator=`): the same inline-vs-call choice falls on
`function7<...>::assign_to<bind_t<...>>`. The TARGET keeps the outer `call
function7::assign_to` out-of-line; our /Ob2 BASE inlines it ONE LEVEL deeper to `call
basic_vtable1::assign_to` plus the inline vtable-tag manipulation (`movzx eax,al` / `stored_vtable`
/ `or edx,1`), growing the statement (`SIZE +0x3b..+0x41`). The driver is the CALLER's /Ob2
inline budget, not the bind type: a tiny single-bind function (debug_win.cpp dump_call_stack)
inlines on BOTH sides and matches 100%, while a large function with two bind/iterate sites
(debug_win.cpp platform::on_error) has the target out-line BOTH. Not source-steerable - the
body is a full structural match; leave it at the inline-vs-call %. Evidence:
debug/platform::on_error 85.11% (2 residual iterate stmts), sibling dump_call_stack 100%.

Sibling walls in the same family (header-inline wrapper / CRT secure-template the target keeps
out-of-line, our /Ob2 inlines, no operator= involved): debug/save_minidump - target `call
strcat_s<260>` (the template-instance COMDAT __delayLoadHelper2 also emits, ICF-reused) vs base
inlined 3-arg `_strcat_s`, 93.11%; debug/on_error(va) - target `call vostok::vsnprintf` (the
shared `inline` in stdlib_extensions.h) vs base inlined `__vsnprintf_s`, 79.44%. Both need a
cross-unit linkage change to force the out-of-line emission (off-limits); PARK at the
inline-vs-call %.
