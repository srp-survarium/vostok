# `dest = src & 0xffff` masks through a 32-bit register; a `(u16)` cast does a direct 16-bit move
tags: cpp:cast | asm:and asm:mov | topic:codegen-idiom
symptoms: mov eax and eax 0FFFFh mov [dst] ax <0xe> vs mov cx mov [dst] cx <0xa>
confidence: 6/10

`u16 dest = (u16)u32_local` is a direct 16-bit load/store (<0xa>). Masking through a full
register (<0xe>) means the source wrote the explicit mask.

```cpp
dest_port = port & 0xffff;
```
```asm
mov eax,[local]; and eax,0FFFFh; mov [dst],ax
```
Evidence: network_core_entry_point get_connection_info_from_string (recovered from pre-restack PR #268).
