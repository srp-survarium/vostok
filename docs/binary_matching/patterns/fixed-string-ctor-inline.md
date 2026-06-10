# fixed_string<N>("literal"): the 3-arg buffer_string base-ctor expansion vs a ctor `call`
tags: cpp:ctor cpp:string cpp:template | asm:push asm:lea asm:call asm:mov | topic:inline-vs-call
symptoms: mov [ebp-XX] N lea push &capacity, call buffer_string::buffer_string 3 args, ??_C@ literal push
confidence: 7/10
variants: inline-vs-call-template-comdat.md

`fixed_string<N>(value_type const* src) : buffer_string(m_buffer, Size, src) {}` - the
whole ctor IS one 3-arg buffer_string call (capacity N materialized to a slot and passed
by ADDRESS, `size_type const&`; m_buffer @ this+0xC past the 12-byte buffer_string base).
Target may inline this wrapper at the site while base emits `call fixed_string<N>::
fixed_string<N>(char const*)` - per-ctor whole-program LTCG, not per-site steerable.
Source `fixed_string<N>("literal")` is already correct; mark PARTIAL.

```cpp
vostok::fixed_string< 46 > temp( "literal" );
```
```asm
mov  dword ptr [ebp-XX], 2Eh        ; capacity = N (46), to a SLOT
push ??_C@...@literal               ; src
lea  ecx,[ebp-XX]; push ecx         ; &capacity
lea  edx,[this+0Ch]; push edx       ; &m_buffer
lea  ecx,[this]; call vostok::buffer_string::buffer_string
```
TELL: target index has NO out-of-line (char const*) ctor, base keeps it (base rva 0x030b00, body == the inlined sequence). If base instead passes (src, &count) it used the 2-arg `(src, size_type const&)` ctor - a DIFFERENT ctor (construct-empty-then-append) = a source bug.
Evidence: body_part_parameters::fill_new_stats_item<statistics_item<46,16>> (rva 0x0ba3c0, 91.78% PARTIAL); damage_model_cook::on_hit_params_received (kept faithful at 90.7).
