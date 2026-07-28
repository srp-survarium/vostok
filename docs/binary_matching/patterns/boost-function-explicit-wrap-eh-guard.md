# Explicit `boost::function<...>( bind(...) )` wrap mis-schedules the temp's EH guard `or` - pass the bind_t directly
tags: cpp:template cpp:ctor | asm:or | topic:structure-shape
symptoms: or dword [ebp-NN],1 lands early (before the bind call), only diff is the guard-bit position, sibling arg reordered
confidence: 8/10
variants: log-callback-ctor-schedule.md, boost-bind-cref.md

A functor temp built for an out-of-line ctor arg can diverge ONLY in where
`or dword ptr [ebp-NN], 1` (the temp's EH guard bit) lands: the target sets it right
AFTER the function's assign_to completes; a base with an explicit
`boost::function< void ( pcstr ) >( boost::bind( ... ) )` wrap sets it EARLY (before
even the bind call) and may reorder a sibling arg's inline evaluation around it. Pass
the bind_t DIRECTLY and let it convert at the ctor-param boundary - when the overload
set is already disambiguated by ARITY the legacy-style explicit wrap is unnecessary AND
wrong for the guard schedule.

```cpp
new string_order ( m_allocator, boost::bind( &c::on_x, this, _1 ), s );  // no function<> wrap
```
Evidence: string_order's 1/2/3-string ctors take 3/4/5 args (arity disambiguates); direct-bind sites (functor_order/receive_response, http ctor/dtor) matched 100% with the late `or`; confirmed on tcp_packet_client::connect / http_client::get / on_content_downloaded.
