# `reader.r<T>()` inlined = load m_pointer (+4), deref, advance by sizeof(T), store back
tags: cpp:template cpp:member | asm:mov asm:add asm:inc | topic:codegen-idiom topic:wire-io
symptoms: mov reg,[this+4]; mov reg,[reg]; add reg,4 (or inc reg); mov [this+4],reg, then store to a target member
confidence: 9/10

The whole `packet_reader::r<T>()` family is a single member-pointer (`m_pointer` at +4): load it,
dereference for the value, bump it by `sizeof(T)` (`add ,4` for u32/float, `inc` for u8/bool), store
it back. In a /Ox (gold) TU the call vanishes into this 4-instruction sequence per field; a
multi-field deserialize is N of these back-to-back, each followed by a store to the target member.
(In a /Od TU the same source keeps a `call r<T>` per field — tu-optimization-level-od-vs-ox.md.)

```cpp
m_money     = reader.r< u32 >( );
m_some_byte = reader.r< bool >( );
```
```asm
mov   eax, [esi+4]      ; m_pointer  (esi = packet_reader)
mov   ecx, [eax]        ; *m_pointer -> value
add   eax, 4            ; advance sizeof(u32)
mov   [esi+4], eax      ; store m_pointer
mov   [edi+828h], ecx   ; -> this->m_money
; u8 field: mov cl,[eax]; inc eax; ...; mov [edi+834h],cl
```
Steerable only via the global inline knob; the +4 offset, add-vs-inc stride, and store-back are the tell a field came off a packet_reader. cite: packet_reader_inline.h:28-44; target survarium::lobby_client::read_account_money rva 0x5c7930.
