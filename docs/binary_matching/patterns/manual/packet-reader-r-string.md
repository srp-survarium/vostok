# `reader.r_string<N>(buf)` = inlined u8 length-prefix read, then memcpy, then NUL-terminate at `buf[len]`
tags: cpp:template cpp:member cpp:string | asm:movzx asm:call asm:mov | topic:wire-io topic:codegen-idiom
symptoms: mov cl,[ptr]; movzx; inc ptr; push len,src,dst; call memcpy; add [this+4],len; mov byte ptr [len+dst],0
confidence: 8/10

A length-prefixed wire string reads ONE byte (the length, an inlined `r<u8>`), advances past it,
`memcpy`s that many bytes into the destination `char[]`, advances `m_pointer` by the length, and
writes a 0 terminator at `dst[length]`. The inner `r<u8>` is inlined even where `r<float>`/`r<bool>`
stay out-of-line, because `r_string` itself is the standalone symbol.

```cpp
char name[16];
reader.r_string( name );     // reads u8 len, memcpy len bytes, name[len]=0
```
```asm
mov   eax, [esi+4]    ; m_pointer
mov   cl, [eax]       ; length byte (inlined r<u8>)
movzx edi, cl / inc eax
push  edi / push eax / push ebx   ; len / src / dst
mov   [esi+4], eax / call memcpy
add   [esi+4], edi    ; advance m_pointer by length
mov   byte ptr [edi+ebx], 0   ; dst[length] = 0
```
The length-byte read + the trailing `byte ptr [len+dst],0` NUL store bracket the memcpy and fingerprint a wire string field. cite: packet_reader_inline.h:46-59; target packet_reader::r_string<16> + survarium::hit_info::deserialize rva 0x73eaf0.
