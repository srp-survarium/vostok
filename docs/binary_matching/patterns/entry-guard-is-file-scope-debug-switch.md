# An unexplained entry guard over the whole body = a file-scope debug switch (cc_bool / command_line::key)
tags: cpp:if cpp:global cpp:static | asm:cmp asm:je asm:call | topic:reconstruct-global topic:data-recovery
symptoms: cmp byte ptr [<data addr>], 0 / je <end> as the FIRST statement; `dynamic initializer for 's_xxx'` in the target's file dump; call iterate_keys<key_initializator>

A render/engine function whose whole body sits under a compare against a lone
`.data` byte is guarded by a file-scope debug switch that the carcass never had.
Two flavours, both recoverable in full (name, limits, default) without guessing:

**console command** - `cmp byte ptr [s_x_value], 0; je end`. The target's
`binaries/structure/target/sources/.../<tu>.cpp` dump lists a matching
`dynamic initializer for 's_x_cc'`; disassemble it to read the object layout back
out (`console_command`: `+0x08 m_next`, `+0x0c m_prev`, `+0x10 m_name`,
`+0x14 command_type`, `+0x18 execution_filter`, `+0x1c need_args`,
`+0x1d serializable`; `cc_value<T>`: `+0x40 &value`, `+0x44 min`, `+0x48 max`).
The initializer writes only the dynamic fields - `m_name`, `command_type` and
`serializable` sit pre-initialized in `.data`, so read them out of the image at
the object address. The `mov dword ptr [obj+0x40], <addr>` operand IS the address
of the backing value global: read its initial bytes for the default.

**command_line::key** - `cmp [k+0x218],0 / jne .1 / call iterate_keys<key_initializator> /
mov [k+0x218],1 / .1: cmp [k+0x218],1 / jne <skip>` is an inlined `key::is_set()`
(`m_type == type_uninitialized -> initialize(); return m_type != type_unset`).
`jne <skip>` after the second compare = the body runs when the key is NOT set, i.e.
`if ( s_key ) return;`. The out-of-line `dynamic initializer for 's_key'` is
`push &s_key; push protected_key_construct; call debug::protected_call`; the ctor
strings are in `.data` at `key+0x10..0x20` (full_name, short_name, category,
description, argument_description).

```cpp
static u32 s_max_triagles_per_dip_value = 1000000;
static console_commands::cc_u32 s_max_triagles_per_dip(
	"max_triagles_per_dip", s_max_triagles_per_dip_value, 0, 100000, false,
	console_commands::command_type_user_specific );

static command_line::key s_no_effects_initialize( "no_effects_initialize", "", "", "" );
```
Initialize the backing value to the byte found in the image (`true` for the two
clearing switches) - a `static bool` that nothing writes would otherwise let the
optimizer fold the guard away, and the cc object is what makes its address escape.

Evidence: render backend::clear_depth_stencil 88.6 -> 99.4, clear_render_targets(f,f,f,f)
91.5 -> 99.7, render_indexed 54 -> 99.9 (s_max_triagles_per_dip_value);
effect_manager::on_effect_created 51.7 -> 99.8, on_async_effect_created 54.3 -> 99.8
(s_no_effects_initialize).

## Batch recovery: read the whole TU's switch table out of the image in one pass

When a TU holds a dozen or more switches (render's `renderer.cpp` has 21), recover them
together instead of one at a time - the target's `binaries/structure/target/.../<tu>.cpp`
dump lists every `` `dynamic initializer for 's_x'` `` **in declaration order**, which is
also the order the definitions must appear in your source.

1. `pdb_fetch --va <each initializer> --view target` in one shell loop. Each body is the
   same 0x4f/0x57-byte shape and hands you three things: the cc object address
   (`mov [X], eax` at 0x05, where `obj == X - 0x0c`), the *backing value* symbol name
   (`mov dword ptr [obj+0x40], s_x_value`) and the limits (`[obj+0x44]`/`[obj+0x45]` for
   `cc_bool` = 0/1, `[obj+0x44]`/`[obj+0x48]` dwords for `cc_u32`). A `cc_u32` initializer
   is ~0x57 bytes and lacks the `mov al, 1`; a `cc_bool` is ~0x4f.
2. The *statically* initialized fields never appear in the initializer - read them from the
   image at `obj+0x10` (name `pcstr`), `+0x14` (`command_type`), `+0x18` (`execution_filter`),
   `+0x1c` (need_args), `+0x1d` (serializable).
3. The **default value** is likewise not written by the initializer. Take the `mov dword ptr
   [obj+0x40], <imm32>` operand *from the raw bytes* (`C7 05 <dst> <imm32>` at the recorded
   offset - the delinker prints the symbol, not the number), then read the byte/dword the
   immediate points at. Defaults matter: a guard whose backing `bool` defaults to `true`
   (`s_enable_rendering`, `s_ui_enabled`, `s_execute_stages`, `s_use_gpu_sync_value`) reads
   `cmp byte ptr [g], 0 / jne <body>`, i.e. the source guard is `if ( !s_x ) { ...early out... }`.
4. Backing-value naming is NOT uniform in the original - `s_enable_rendering`,
   `s_debug_remove_trample` and `s_do_stages_profiling` carry no suffix while their siblings
   use `_value`, and one pair is outright mismatched (`s_draw_fps_histogram_cc` backs
   `s_draw_frame_histogram_value`). Take the name the PDB gives, never regularize it.
5. The initializer whose demangled name has **no namespace qualifier** while its siblings all
   carry one is declared at global scope, ahead of `namespace vostok {`
   (`s_ui_enabled_console_command` in `renderer.cpp`).

Evidence: `renderer::render` 22.3 -> 57.0 in one edit (21 switches recovered; the guards also
gate `execute_stages` and `draw_debug` in the same TU), and 20 new `dynamic initializer`
COMDATs paired at 78-100%.
