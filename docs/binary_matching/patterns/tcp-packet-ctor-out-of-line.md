# `network_core::tcp_packet packet( allocator )`: target CALLs the ctor, base inlines it
tags: cpp:ctor cpp:inline | asm:call asm:mov | topic:inline-vs-call topic:structure-shape
symptoms: target `push g_mt_allocator; lea ecx,[esp+N]; call tcp_packet::tcp_packet`; base inlines `mov [slot],0; mov dword [slot+8], g_mt_allocator; mov [slot+C],0` (the m_buffer/m_buffer_size/m_allocator/m_allocated_size field stores)
confidence: 7/10
variants: base-ctor-call-vs-inlined-init.md, template-accessor-deinline-global-knob.md

Every `lobby_client`/`messaging_client` packet-builder opens with a stack
`network_core::tcp_packet packet( memory::g_mt_allocator );`. In the TARGET this is a
real out-of-line `call vostok::network_core::tcp_packet::tcp_packet`; our `/Ob2 /GL`
build INLINES the ctor (it is `inline` in `network_core/tcp_packet_inline.h`), spilling
the four member field stores into the caller. The function STILL STRUCTURE-MATCHES (the
`tcp_packet packet(...)` statement is one statement on both sides) but the byte residual
caps the packet builders at ~74-82% (`query_*`, `set_status_ready_for_match`, `buy_item`,
`move_item`, `discard_playing_order`, `reroll_player_skills`, `set_player_skills`,
`on_connected`).

```cpp
network_core::tcp_packet packet( memory::g_mt_allocator );   // correct source
```
```asm
; target: push ?g_mt_allocator@...; lea ecx,[esp+0Ch]; call tcp_packet::tcp_packet
; base  : mov [esp+18h],esi(0); mov [esp+1Ch],esi; mov [esp+20h], g_mt_allocator; mov [esp+24h],esi
```

NOT steerable from the caller's source: making the ctor `call` requires stripping the
`inline` from `tcp_packet`'s ctor (move it to a `.cpp`) - a `network_core` header change
that flips the inline/out-of-line knob ENGINE-WIDE for every TU that builds a tcp_packet,
so it belongs to a deliberate network_core de-inline pass, not the consuming unit. Same
family as `strip_pointer<>` (FREE/ALLOC readers, `strip-pointer-delete-resource.md`) and
the `boost::function` safe-bool / `operator()` out-of-lining: the target out-lined a
header-`inline` helper our build inlines. Leave the consumer at the inline-vs-call %.

DTOR variant (the INVERSE knob, same family): `messaging_client_process_messagess.cpp`'s
`query_for_friend_list / query_for_friends_status / add_to_friend_list /
remove_from_friend_list / query_for_ignore_list / add_to_ignore_list /
remove_from_ignore_list / find_players_by_name` all STRUCTURE-MATCH (5 stmts) with the
ctor `call` matching, but the TARGET INLINES `~tcp_packet` at the epilogue (the
`mov eax,[buffer]; test; add eax,-3; je; mov ecx,[allocator]; mov edx,[ecx]; mov
eax,[edx+18h]; call eax` = `if(buffer()){ free_helper(m_allocator, buffer()-3) }` with
`free_helper` -> the allocator's virtual deallocate, all inlined) while our `/Ob2 /GL`
build OUT-lines it (`mov ecx,edi; call tcp_packet::~tcp_packet`). Caps these builders at
~75-79%, ~25/37 instructions equal, all 12 differing instructions in the dtor tail. Same
header-inline knob (`~tcp_packet` is `inline` in `tcp_packet_inline.h`); flipping it is the
same engine-wide network_core de-inline pass. Leave the consumers at the inline-vs-call %.
