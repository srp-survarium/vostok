# `cmp [X],0 / call iterate_keys<key_initializator> / cmp [X],1` = an inlined `command_line::key` guard
tags: cpp:if cpp:static cpp:global | asm:cmp asm:call asm:jcc | topic:codegen-idiom topic:data-recovery
symptoms: cmp dword ptr [G],0 / jne / mov byte [esp+N],0 / push / mov [G],1 / call iterate_keys<key_initializator> / cmp [G],1 / jne skip
confidence: 9/10

A file-static `command_line::key` tested with `operator bool` (`is_set()`) inlines to a
lazy-init guard followed by a compare against `type_unset`. `key::is_set()` is
`if ( m_type == type_uninitialized ) initialize(); return m_type != type_unset;` and
`key::initialize()` is `m_type = type_unset; iterate_keys( key_initializator() );` - so the
SAME global (`&key.m_type`) is first tested for 0, then written with 1, then compared to 1.
`jne <skip>` on the second compare means the guarded block runs only when the key is NOT set,
i.e. the source read `if ( !s_key )`.

```cpp
static command_line::key    s_no_level( "no_level", "", "", "" );   // file scope
...
if ( !s_no_level )                       // <- the whole 0x3d-byte statement
    while ( !scene_manager.scene_count() ) { ... }
```
```asm
cmp   dword ptr [0A3F834h], 0            ; m_type == type_uninitialized ?
jne   short .1
mov   byte ptr [esp+8], 0                ; key_initializator() by value
mov   edx, [esp+8]
push  edx
mov   [0A3F834h], edi                    ; edi == 1 == type_unset
call  vostok::command_line::iterate_keys<vostok::command_line::key_initializator>
add   esp, 4
.1:
cmp   [0A3F834h], edi                    ; m_type != type_unset -> is_set()
jne   .skip                              ; source guard was `if ( !s_key )`
```

RECOVERING THE KEY (name + ctor args) - all from the binary, no guessing:
1. The TU's structure dump lists a `` `dynamic initializer for 's_<name>'` `` pseudo-function
   (`binaries/structure/target/sources/.../<tu>.cpp`); its body is
   `push s_<name>; push protected_key_construct; call debug::protected_call`, which names
   the static.
2. The five `pcstr` members are compile-time constants folded into `.data`, so read them
   from the image: the 5 dwords immediately BELOW `m_type` are `m_full_name, m_short_name,
   m_category, m_description, m_argument_description`, i.e. `m_full_name == m_type - 0x14`;
   the object itself starts `0x224` below `m_type` (`fixed_string512 m_string_value` at +0,
   then the 5 pointers at +0x210, then `m_type` at +0x224). Do NOT hardcode the object
   offset - disassemble the `dynamic initializer` and take the `push <addr>` operand, then
   read the strings at `<addr>+0x210 .. +0x220`. Convert VMA->file offset with `objdump -h`
   (careful: `.rdata` and `.data` can share one VMA-minus-file delta); empty args all point
   at one pooled `""`.

Evidence: `render::engine::world::initialize` - the missing `if ( !s_no_level )` was the
whole gap between its 7 base statements and the target's 8 (56.7% -> 99.9%, STRUCTURE MATCH).

Evidence 2 - the key is often a SECOND disjunct of an existing guard, and one key can gate a
whole class. `render::effect_compiler`'s eleven `if (m_shaders_cache_mode)` early-outs are
really `if (m_shaders_cache_mode || s_no_effect_result)`: `cmp byte ptr [this+9090h],0 /
jne .end` followed immediately by the `0A5EFCC` lazy-init pair, all inside ONE statement
record. Widening the guard in one edit moved set_fill_mode/set_cull_mode 30->99, set_depth
32->99, set_stencil 42->99.6, bind_constant 68->99.6, and un-DCE'd both `set_texture`
overloads. `begin_technique` in the SAME class has only the `m_shaders_cache_mode` compare -
read each function's asm, never blanket-apply. When a TU holds two keys
(`s_no_effect_result`, `s_one_texture_result`), the one used through `operator bool` inlines
to the lazy-init pair while an explicit `.is_set()` stays an out-of-line
`call vostok::command_line::key::is_set` - that tells you which spelling the source used.
