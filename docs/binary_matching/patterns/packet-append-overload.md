# `packet.append( field )`: scalar overloads push the VALUE, buffer/float3 push an ADDRESS
tags: cpp:member cpp:operator | asm:push asm:lea | topic:codegen-idiom
symptoms: push eax value vs lea/push of a stack slot before append
confidence: 8/10

The scalar `packet<T>::append(uN/sN/bool)` overloads push the value itself; the
`append(pcvoid,u32)` and `append(float3 const&)` overloads push an address (lea/push).
Use the push shape to disambiguate which overload was called.

```cpp
packet.append( field );    // u32 overload - by value
```
```asm
mov  edx, [ebp-0Ch]; mov eax, [edx+58h]   ; this->field
push eax                                  ; value, not address
mov  ecx, [ebp+8]                         ; &packet
call vostok::network_core::packet<...>::append
```
Evidence: game_core/client_player_update::serialize.
