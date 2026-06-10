# `(T)config_value`: target keeps operator T out-of-line, base keeps cast_number
tags: cpp:cast cpp:operator | asm:call asm:movzx | topic:inline-vs-call
symptoms: call binary_config_value::operator unsigned char vs call cast_number<unsigned char,unsigned __int64,unsigned int>
confidence: 8/10
variants: static-cast-checked.md

`(u8)cfg["x"]["y"]` lowers through `operator u8() -> cast_unsigned_number<u8>() ->
cast_number<u8,u64,u32>()` (configs_binary_config_value_inline.h). LTCG picks a DIFFERENT
cut: target keeps the OUTER operator out-of-line (rva 0x52160) inlining cast_number into
it; base inlines the operator and keeps the INNER cast_number standalone (rva 0x79dd0).
TELL: each symbol exists in only one index. Source `(T)cfg[...]` is correct; mark PARTIAL.

```cpp
u8 const value = (u8)config["x"]["y"];
```
Evidence: game_core/player_parameters_modifyer_cook::translate_query (89.62% PARTIAL).
