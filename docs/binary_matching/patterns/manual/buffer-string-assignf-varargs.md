# `str.assignf("fmt", ...)` = cdecl varargs: push args R-to-L, push &fmt literal, push `this` (lea), call, add esp,N
tags: cpp:string cpp:varargs | asm:push asm:call asm:lea asm:add | topic:codegen-idiom
symptoms: push <vararg> / push ??_C@ format literal / lea eax,[ebp-X]; push / call ...::assignf / add esp,0Ch, return value discarded
confidence: 9/10

`buffer_string::assignf`/`appendf(const char* fmt, ...)` (and the `fs_new` path_string variants)
are out-of-line **cdecl varargs** — the printf shape: variadic args pushed right-to-left, then
the format-string literal, then the string object's address (by `lea`), then `call`, then a
caller `add esp,N` to pop. The returned `*this` ref is normally discarded.

```cpp
config_name.assignf( "resources/%s", parent.get_requested_path( ) );
```
```asm
call  ...::get_requested_path
push  eax                                       ; the %s vararg
push  ??_C@_0N@BHGPFDAK@resources?1?$CFs?$AA@   ; "resources/%s"
lea   eax, [ebp-118h]                           ; &config_name (this)
push  eax
call  vostok::fs_new::path_string_impl::assignf
add   esp, 0Ch                                  ; cdecl cleanup, 3 args * 4
```
Steerable: the arg count drives `add esp,N`; a wrong N or a missing/extra push = a mismatched format-arg list in source. cite: survarium::items_cook::translate_query (rva 0x761b20) items_cook.cpp:23; object_wire.cpp:115 `wire_name.assignf("wire_%X", this)`.
