# `boost::functionN::operator()` inline-vs-call is per-INSTANTIATION (function4 called, function5 inlined, same exe)
tags: cpp:template | asm:call | topic:inline-vs-call
symptoms: inlined safe-bool + throw_bad_function_call + get_vtable + invoker ~0x5d per site, small handlers 47-67%
confidence: 8/10
variants: boost-function-safe-bool-inline.md, boost-function-call-site-frame.md, boost-function-byvalue-copy.md

The login-chain target calls `function4<...login_server...>::operator()` OUT-OF-LINE at
every callback site (a 0x10-byte push x4 + call) while INLINING
`function5<...,sign_up_info const&>::operator()` (safe-bool + throw_bad_function_call +
get_vtable + invoker, ~0x5d bytes) at every site of the SAME exe. Our /Od+LTCG base made
the OPPOSITE choice for function4 (inlined everywhere, +0x3d per site) and the SAME
choice for function5. Same whole-program class as the function0 safe-bool wall; not
steerable from the call site - the source `callback( a, b, c, d );` is already correct.

Evidence: login_client_impl_sign_{in,out}.cpp - sign_up TU ~90% while small function4-heavy handlers drop to 47-67% (on_user_name_answer_received 47.34% worst case).
