# game_core::client_player_update::serialize

Target asm (rva 0x771190 in target index):

```
push  ebp / mov ebp,esp / sub esp,0Ch / mov [ebp-0Ch],ecx
mov   eax,[ebp+8]      ; packet
push  eax
mov   ecx,[ebp-0Ch]    ; &input (offset 0)
call  survarium::player_input::serialize
mov   ecx,[ebp+8]
push  ecx
mov   ecx,[ebp-0Ch]
add   ecx,14h          ; &state (offset 0x14)
call  survarium::player_state::serialize
mov   edx,[ebp-0Ch]
mov   eax,[edx+58h]    ; time_in_ms
push  eax
mov   ecx,[ebp+8]      ; packet (this)
call  packet<udp_match_packet>::append
mov   esp,ebp / pop ebp / ret 4
```

Three statements, mapping 1:1 to:

```cpp
input.serialize( packet );
state.serialize( packet );
packet.append( time_in_ms );   // append( u32 )
```

## Blocker (why it can't be diffed yet)
The body needs the full `vostok::network_core::udp_match_packet` type so
`packet.append( time_in_ms )` resolves to `packet<udp_match_packet>::append(u32)`.
That type's header (`sources/vostok/network_core/udp_match_packet.h`) and its
whole dependency cluster are never-compiled stubs:

- inheritance is written `: packet<...>` (private) but the target structure has
  `: public packet<...>` - private base hides `append()`;
- members `sequence_number<u8>` / `<u16>` exist only as explicit specializations
  with **no primary `template<typename> class sequence_number`** declared, and
  their `/* no source */` bodies don't return values;
- `udp_match_client_session*` member has no forward declaration;
- `boost::array<u8,256>` member with no `<boost/array.hpp>` include;
- `udp_match_packet::buffer_to_send()` returns `base_packet::m_buffer`, which is
  private in `base_packet`.

No compiled translation unit includes `udp_match_packet.h` today, so these never
surfaced. The first include (this `.cpp`) breaks the whole game_core build.
Fixing the cluster (writing a `sequence_number` primary template, fwd-decls,
includes, base access fix) is a separate, larger task with its own regression
surface - out of scope for this one function.

This is the same blocker as `player_input::serialize`,
`player_state::serialize`, and `inventory_item::serialize` (all `BLOCKED`, empty
bodies). They should all unblock together when the packet cluster is made to
compile.

## Attempts
1. `: public packet<...>` + include `udp_match_packet.h` + the 3-statement body
   -> game_core build failed: udp_match_packet.h does not compile standalone
   (errors C4430/C2238/C2039/C2248/C2118 at sequence_number / boost::array /
   m_buffer). Reverted; kept the matched body as a comment.

## Outcome
STATE[BLOCKED]: body is matched against the asm (3 statements, verified) but
cannot be compiled/diffed until the udp_match_packet type cluster compiles.
Regressions caused: none (reverted to a building state; empty body).
Inlining: n/a (not yet diffable).
